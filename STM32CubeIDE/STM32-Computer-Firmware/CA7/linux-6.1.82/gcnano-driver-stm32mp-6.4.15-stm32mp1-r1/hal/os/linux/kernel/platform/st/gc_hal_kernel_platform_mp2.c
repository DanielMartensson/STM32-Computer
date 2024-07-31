/****************************************************************************
*
*    The MIT License (MIT)
*
*    Copyright (c) 2014 - 2023 Vivante Corporation
*
*    Permission is hereby granted, free of charge, to any person obtaining a
*    copy of this software and associated documentation files (the "Software"),
*    to deal in the Software without restriction, including without limitation
*    the rights to use, copy, modify, merge, publish, distribute, sublicense,
*    and/or sell copies of the Software, and to permit persons to whom the
*    Software is furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in
*    all copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
*    DEALINGS IN THE SOFTWARE.
*
*****************************************************************************
*
*    The GPL License (GPL)
*
*    Copyright (C) 2014 - 2023 Vivante Corporation
*
*    This program is free software; you can redistribute it and/or
*    modify it under the terms of the GNU General Public License
*    as published by the Free Software Foundation; either version 2
*    of the License, or (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*****************************************************************************
*
*    Note: This software is released under dual MIT and GPL licenses. A
*    recipient may use this file under the terms of either the MIT license or
*    GPL License. If you wish to use only one license not the other, you can
*    indicate your decision by deleting one of the above license notices in your
*    version of this file.
*
*****************************************************************************/


#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <linux/regulator/consumer.h>
#include <linux/reset.h>
#include <linux/slab.h>

#include "gc_hal_kernel_linux.h"
#include "gc_hal_kernel_platform.h"
#define __gc_hal_kernel_allocator_array_h_ /* This is to avoid double definition*/
#include "gc_hal_kernel_allocator.h"
#include "gc_hal_kernel_allocator_array.h"

#ifdef DEBUG
#define DEBUG_LEVEL gcvLEVEL_WARNING
#define DEBUG_ZONE  gcdZONE_ALL

#define xstr(s) str(s)
#define str(x) #x
#endif

struct st_priv {
    gcsPLATFORM * platform;

    /*  Reset management */
    struct reset_control *rstc;

    /*  Regulator Management */
    struct regulator *supply;

    /* Clock management.*/
    struct clk  *clk_3d_ahb;
    struct clk  *clk_3d_axi;
    struct clk  *clk_3d_ref;
};

static struct st_priv *stpriv;

static int
_AllocPriv(IN gcsPLATFORM * Platform)
{
    struct st_priv *priv;

    priv = kzalloc(sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        gckOS_Print("galcore platform st: cannot allocate memory.\n");
        return -ENOMEM;
    }

    stpriv = priv;
    stpriv->platform = Platform;

    return 0;
}

static int
_FreePriv(IN gcsPLATFORM * Platform)
{
    struct st_priv *priv = stpriv;

    if (priv)
        kfree(priv);

    stpriv = NULL;
    return 0;
}

static int _reset(void)
{
    struct st_priv* priv = stpriv;
    struct platform_device* pdev = stpriv->platform->device;
    struct device *dev = &pdev->dev;

    struct reset_control *rstc = priv->rstc;
    int ret = 0;

    if (!rstc)
         goto end;

    ret = reset_control_reset(rstc);
    if (ret)
        dev_err(dev, "galcore platform st: reset timeout\n");

end:
    return ret;
}

static int
set_clock(struct device *dev, IN gctBOOL Enable)
{
    struct st_priv* priv = stpriv;
    int ret = 0;

    if (Enable) {
        ret = clk_prepare_enable(priv->clk_3d_ahb);
        if (ret) {
            gckOS_Print("galcore platform st: failed to clock clk_3d_ahb: %i\n",
                        ret);
            goto error;
        }


        ret = clk_prepare_enable(priv->clk_3d_axi);
        if (ret) {
            clk_disable_unprepare(priv->clk_3d_ahb);
            gckOS_Print("galcore platform st: failed to clock clk_3d_axi: %i\n",
                        ret);
            goto error;
        }

        ret = clk_prepare_enable(priv->clk_3d_ref);
        if (ret) {
            clk_disable_unprepare(priv->clk_3d_axi);
            clk_disable_unprepare(priv->clk_3d_ahb);
            gckOS_Print("galcore platform st: failed to clock clk_3d_ref: %i\n",
                        ret);
            goto error;
        }
        _reset();
    } else {
        _reset();
        clk_disable_unprepare(priv->clk_3d_ahb);
        clk_disable_unprepare(priv->clk_3d_axi);
        clk_disable_unprepare(priv->clk_3d_ref);
    }

error:
    return ret;
}

static int
set_power(struct device *dev, IN gctBOOL Enable)
{
    struct st_priv *priv = stpriv;
    int ret = gcvSTATUS_OK;

    if (!priv->supply) {
        goto error;
    }

    if (Enable) {
        ret = regulator_enable(priv->supply);
        if (ret < 0) {
            dev_err(dev, "failed to enable GPU power supply\n");
            goto error;
        }
    } else {
        ret = regulator_disable(priv->supply);
        if (ret < 0) {
            dev_err(dev, "failed to disable GPU power supply\n");
            goto error;
        }
    }

    return gcvSTATUS_OK;
error:
    return ret;
}


gceSTATUS
_AdjustParam(IN gcsPLATFORM * Platform, OUT gcsMODULE_PARAMETERS *Args)
{
    struct platform_device* pdev = Platform->device;
    struct device *dev = &pdev->dev;
    struct device_node *np;
    struct resource* res;
    struct resource  contig_res;

    int irq;
    int core = gcvCORE_MAJOR;

    if (of_device_is_compatible(dev->of_node, "vivante,gc")) {
        /* Register base address */
        res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        if (!res) {
            gckOS_Print("galcore platform st: missing reg base address.\n");
            return gcvSTATUS_OUT_OF_RESOURCES;
        }
        Args->registerBases[core] = res->start;
        Args->registerSizes[core] = resource_size(res);

    /* Contiguous area */
        np = of_parse_phandle(dev->of_node, "contiguous-area", 0);
        if (np) {
            if (of_address_to_resource(np, 0, &contig_res)) {
                gckOS_Print("galcore platform st: no contiguous-area resource.\n");
                return gcvSTATUS_OUT_OF_RESOURCES;
            }
            Args->contiguousBase = contig_res.start;
            Args->contiguousSize = resource_size(&contig_res);
        }

        /* Core interrupt line */
        irq = platform_get_irq(pdev, 0);
        if (irq < 0) {
            gckOS_Print("galcore platform st: missing core interrupt line.\n");
            return gcvSTATUS_NOT_SUPPORTED;
        }
        Args->irqs[core] = irq;
        allocatorArray = allocatorArray_CMA_First;

        Platform->flagBits |= gcvPLATFORM_FLAG_LIMIT_4G_ADDRESS;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 24)
        *Args->devices[0]->dma_mask = DMA_BIT_MASK(32);
        Args->devices[0]->coherent_dma_mask = DMA_BIT_MASK(32);
#else
        *Args->devices[0]->dma_mask = DMA_32BIT_MASK;
        Args->devices[0]->coherent_dma_mask = DMA_32BIT_MASK;
#endif

    }

    return gcvSTATUS_OK;
}

static const struct of_device_id gcnano_of_match[] = {
    { .compatible = "vivante,gc" },
    { /* end node */ }
};
MODULE_DEVICE_TABLE(of, gcnano_of_match);

gceSTATUS
_SetPower( IN gcsPLATFORM * Platform,
           IN gctUINT32 DevIndex,
           IN gceCORE GPU,
           IN gctBOOL Enable
        )
{
    struct device *dev = &Platform->device->dev;
    int ret = 0;

    if (Enable) {
#ifdef CONFIG_PM
        pm_runtime_resume_and_get(dev);
#endif
        ret = set_power(dev, gcvTRUE);
        if (ret < 0)
            goto error;

        ret = set_clock(dev, gcvTRUE);
        if (ret < 0) {
            set_power(dev, gcvFALSE);
            goto error;
        }

    } else {

        ret = set_clock(dev, gcvFALSE);
        if (ret < 0)
            set_power(dev, gcvFALSE);
        else
            ret = set_power(dev, gcvFALSE);

#ifdef CONFIG_PM
        pm_runtime_put_sync_suspend(dev);
#endif
    }
    return (gcvSTATUS_OK);

error:
#ifdef CONFIG_PM
    pm_runtime_put_sync_suspend(dev);
#endif
    return (ret < 0 ? gcvSTATUS_INVALID_REQUEST : gcvSTATUS_OK);
}

gceSTATUS
_GetPower(IN gcsPLATFORM * Platform)
{
    struct device *dev = &Platform->device->dev;
    struct st_priv *priv = stpriv;
    int ret;
    gceSTATUS ret_val = gcvSTATUS_OK;
    struct reset_control *rstc;

    rstc = devm_reset_control_get(dev, NULL);
    priv->rstc = IS_ERR(rstc) ? NULL : rstc;

    priv->supply = devm_regulator_get(dev, "gpu");
    if (IS_ERR(priv->supply)) {
        /* If no regulator this is likely a Power Domain management */
        dev_warn(dev, "no GPU regulator");
        priv->supply = NULL;
    } else {
        dev_warn(dev, "GPU regulator gotten");
    }

    if (priv->supply) {
       ret = regulator_enable(priv->supply);
       if (ret < 0) {
          dev_err(dev, "failed to enable GPU power supply\n");
          ret_val = gcvSTATUS_CLOCK_ERROR;
          goto error;
       }
    }

    priv->clk_3d_axi = devm_clk_get(dev, "axi");
    if (IS_ERR(priv->clk_3d_axi)) {
        if (PTR_ERR(priv->clk_3d_axi) != -EPROBE_DEFER)
           dev_err(dev, "no AXI clock");
        ret_val = gcvSTATUS_CLOCK_ERROR;
        goto error;
    }

    //priv->clk_3d_ahb = devm_clk_get(dev, "ahb");
    //if (IS_ERR(priv->clk_3d_ahb))
    priv->clk_3d_ahb = NULL;

    priv->clk_3d_ref = devm_clk_get(dev, "core");
    if (IS_ERR(priv->clk_3d_ref)) {
        if (PTR_ERR(priv->clk_3d_ref) != -EPROBE_DEFER)
           dev_err(dev, "no core clock");
        ret_val = gcvSTATUS_CLOCK_ERROR;
        goto error;
    }

    ret = set_clock(dev, gcvTRUE);
    if (ret) {
        ret_val = gcvSTATUS_CLOCK_ERROR;
        goto error;
    }
    /*
     * Reset may fail but this fallthru to clock disabled and reg disabled
     * that put system in good shape
     */
    _reset();

    ret = set_clock(dev, gcvFALSE);
    if (ret) {
        ret_val = gcvSTATUS_CLOCK_ERROR;
        goto error;
    }

    if (priv->supply) {
        ret = regulator_disable(priv->supply);
        if (ret < 0) {
           dev_err(dev, "failed to disable GPU power supply\n");
           ret_val = gcvSTATUS_INVALID_REQUEST;
           goto error;
        }
    }

#ifdef CONFIG_PM
    pm_runtime_enable(dev);
#endif

error:
    return ret_val;
}

gceSTATUS
_PutPower(IN gcsPLATFORM * Platform)
{
    struct st_priv *priv = stpriv;
    struct device *dev = &Platform->device->dev;

#ifdef CONFIG_PM
    pm_runtime_disable(dev);
#endif

    /*Disable clock*/
    if (priv->clk_3d_ahb) {
        devm_clk_put(dev,priv->clk_3d_ahb);
        priv->clk_3d_ahb = NULL;
    }

    if (priv->clk_3d_axi) {
        devm_clk_put(dev,priv->clk_3d_axi);
        priv->clk_3d_axi = NULL;
    }

    if (priv->clk_3d_ref) {
        devm_clk_put(dev,priv->clk_3d_ref);
        priv->clk_3d_ref = NULL;
    }

    if (priv->supply) {
        devm_regulator_put(priv->supply);
        priv->supply = NULL;
    }

    return gcvSTATUS_OK;
}

#ifdef CONFIG_PM
static int st_gpu_runtime_suspend(struct device *dev)
{
    int ret = 0;
    return ret;
}

static int st_gpu_runtime_resume(struct device *dev)
{
    int ret = 0;
    return ret;
}

static struct dev_pm_ops gpu_pm_ops;
#endif

gceSTATUS
_SetClock(IN gcsPLATFORM * Platform, IN gctUINT32 DevIndex,IN gceCORE GPU, IN gctBOOL Enable)
{
    struct device *dev = &Platform->device->dev;
    int ret = 0;

#ifdef CONFIG_PM
    if (Enable) {
        /* returns 0 on success,
         * 1 if the device's runtime PM status was already 'active'
         */
        ret = pm_runtime_resume(dev);
    }
    else {
        ret = pm_runtime_idle(dev);
        if ( (ret == -EAGAIN) || (ret == -EBUSY)) {
            ret = 0;
        }
    }
#else
    ret = set_clock(dev, Enable);
#endif

    return (ret < 0 ? gcvSTATUS_INVALID_REQUEST : gcvSTATUS_OK);
}

gceSTATUS
_Reset(IN gcsPLATFORM * Platform, IN gctUINT32 DevIndex, IN gceCORE GPU)
{
    int ret = 0;

    if (GPU != gcvCORE_MAJOR)
        goto end;

    ret = _reset();
end:
    return (ret ? gcvSTATUS_INVALID_REQUEST : gcvSTATUS_OK);
}

#ifdef CONFIG_PM
static const struct dev_pm_ops st_gpu_pm_ops = {
    SET_RUNTIME_PM_OPS(st_gpu_runtime_suspend,
                       st_gpu_runtime_resume, NULL)
};
#endif

static int _AdjustPlatformDriver(struct platform_driver *driver)
{
    driver->driver.of_match_table = gcnano_of_match;

#ifdef CONFIG_PM
    /* Fill local structure with original value. */
    memcpy(&gpu_pm_ops, driver->driver.pm, sizeof(struct dev_pm_ops));

     /* Add runtime PM callback. */
    gpu_pm_ops.runtime_suspend = st_gpu_pm_ops.runtime_suspend;
    gpu_pm_ops.runtime_resume = st_gpu_pm_ops.runtime_resume;
    gpu_pm_ops.runtime_idle = st_gpu_pm_ops.runtime_idle;

    driver->driver.pm = &gpu_pm_ops;
#endif

    return 0;
}


static struct _gcsPLATFORM_OPERATIONS st_ops =
{
    .adjustParam   = _AdjustParam,
    .getPower      = _GetPower,
    .putPower      = _PutPower,
    .setPower      = _SetPower,
    .setClock      = _SetClock,
    .reset         =_Reset,
};

static struct _gcsPLATFORM st_platform =
{
    .name = __FILE__,
    .ops  = &st_ops,
};

int gckPLATFORM_Init(struct platform_driver *pdrv,
            struct _gcsPLATFORM **platform)
{
    int ret;

    _AdjustPlatformDriver(pdrv);

    ret = _AllocPriv(&st_platform);

    *platform = &st_platform;

#ifdef DEBUG
    gckOS_Print("Init with Level=%s, Zone=%s\n",
                xstr(DEBUG_LEVEL), xstr(DEBUG_ZONE));
    gckOS_SetDebugLevelZone(DEBUG_LEVEL, DEBUG_ZONE);
#endif

    return ret;

}

int gckPLATFORM_Terminate(struct _gcsPLATFORM *platform)
{
    _FreePriv(platform);
    return 0;

}
