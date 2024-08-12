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
#include <linux/reset.h>
#include <linux/slab.h>

#include "gc_hal_kernel_linux.h"
#include "gc_hal_kernel_platform.h"
#define __gc_hal_kernel_allocator_array_h_ /* This is to avoid double definition*/
#include "gc_hal_kernel_allocator.h"
#include "gc_hal_kernel_allocator_array.h"

#define ST_GPU_AUTOSUSPEND_DELAY_MS              200

#ifdef DEBUG
#define DEBUG_LEVEL gcvLEVEL_WARNING
#define DEBUG_ZONE  gcdZONE_ALL

#define xstr(s) str(s)
#define str(x) #x
#endif

struct st_priv {
    /*  Reset management */
    struct reset_control *rstc;

    /* Clock management.*/
    struct clk  *clk_3d_ahb;
    struct clk  *clk_3d_axi;
    struct clk  *clk_3d_clk2x;
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

static int
set_clock(IN gctBOOL Enable)
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

        ret = clk_prepare_enable(priv->clk_3d_clk2x);
        if (ret) {
            clk_disable_unprepare(priv->clk_3d_ahb);
            clk_disable_unprepare(priv->clk_3d_axi);
            gckOS_Print("galcore platform st: failed to clock clk_3d_clk2x: %i\n",
                        ret);
            goto error;
        }
    } else {
        clk_disable_unprepare(priv->clk_3d_ahb);
        clk_disable_unprepare(priv->clk_3d_axi);
        clk_disable_unprepare(priv->clk_3d_clk2x);
    }

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


    } else {
        /* Register base address */
        res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "iobase_3d");

        if (res) {
            Args->registerBases[core] = res->start;
            Args->registerSizes[core] = resource_size(res);
        }

        /* 3D pipeline IRQ */
        irq = platform_get_irq_byname(pdev, "irq_3d");
        if (irq)
            Args->irqs[core] = irq;

        /* Contiguous area */
        res = platform_get_resource_byname(pdev, IORESOURCE_MEM,
                                           "contig_baseaddr");

        if (res) {
            Args->contiguousBase = res->start;
            Args->contiguousSize = res->end - res->start + 1;
        }
    }

    return gcvSTATUS_OK;
}

static const struct of_device_id gcnano_of_match[] = {
    { .compatible = "st,gcnano" },
    { .compatible = "vivante,gc" },
    { /* end node */ }
};
MODULE_DEVICE_TABLE(of, gcnano_of_match);

gceSTATUS
_Reset(IN gcsPLATFORM * Platform, IN gctUINT32 DevIndex, IN gceCORE GPU);

gceSTATUS
_GetPower(IN gcsPLATFORM * Platform)
{
    struct device *dev = &Platform->device->dev;
    struct st_priv *priv = stpriv;
    gceSTATUS ret_val = gcvSTATUS_OK;
    struct reset_control *rstc;

    rstc = devm_reset_control_get(dev, NULL);
    priv->rstc = IS_ERR(rstc) ? NULL : rstc;

    /*Initialize the clock structure*/
    if (of_device_is_compatible(dev->of_node, "vivante,gc")) {
        priv->clk_3d_axi = devm_clk_get(dev, "bus");
        if (IS_ERR(priv->clk_3d_axi))
            priv->clk_3d_axi = NULL;

        priv->clk_3d_ahb = devm_clk_get(dev, "reg");
        if (IS_ERR(priv->clk_3d_ahb))
            priv->clk_3d_ahb = NULL;

        priv->clk_3d_clk2x = devm_clk_get(dev, "core");
        if (IS_ERR(priv->clk_3d_clk2x))
            priv->clk_3d_clk2x = NULL;

    } else {
        priv->clk_3d_ahb = devm_clk_get(dev, "clk_3d_ahb");
        if (IS_ERR(priv->clk_3d_ahb)) {
            gckOS_Print("galcore platform st: unable to get clk_3d_ahb clock.\n");
            priv->clk_3d_ahb = NULL;
            ret_val = gcvSTATUS_CLOCK_ERROR;
        }

        priv->clk_3d_axi = devm_clk_get(dev, "clk_3d_axi");
        if (IS_ERR(priv->clk_3d_axi)) {
            gckOS_Print("galcore platform st: unable to get clk_3d_axi clock.\n");
            priv->clk_3d_axi = NULL;
            ret_val = gcvSTATUS_CLOCK_ERROR;
        }

        priv->clk_3d_clk2x = devm_clk_get(dev, "clk_3d_clk2x");
        if (IS_ERR(priv->clk_3d_clk2x)) {
            gckOS_Print("galcore platform st: unable to get clk_3d_clk2x clock.\n");
            priv->clk_3d_clk2x = NULL;
            ret_val = gcvSTATUS_CLOCK_ERROR;
        }
    }

    if (ret_val == gcvSTATUS_OK) {
        int ret;

        ret = set_clock(gcvTRUE);
        if (ret) {
            ret_val = gcvSTATUS_CLOCK_ERROR;
            goto error;
        }

        ret_val = _Reset(Platform, 0, gcvCORE_MAJOR);
        ret = set_clock(gcvFALSE);
        if (ret) {
            ret_val = gcvSTATUS_CLOCK_ERROR;
            goto error;
        }
    }

    if (ret_val != gcvSTATUS_OK)
        goto error;

#ifdef CONFIG_PM
    pm_runtime_set_autosuspend_delay(dev, ST_GPU_AUTOSUSPEND_DELAY_MS);
    pm_runtime_use_autosuspend(dev);
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
    pm_runtime_dont_use_autosuspend(dev);
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

    if (priv->clk_3d_clk2x) {
        devm_clk_put(dev,priv->clk_3d_clk2x);
        priv->clk_3d_clk2x = NULL;
    }

    return gcvSTATUS_OK;
}

#ifdef CONFIG_PM
static int st_gpu_runtime_suspend(struct device *dev)
{
    return set_clock(gcvFALSE);
}

static int st_gpu_runtime_resume(struct device *dev)
{
    return set_clock(gcvTRUE);
}

static struct dev_pm_ops gpu_pm_ops;
#endif

gceSTATUS
_SetClock(IN gcsPLATFORM * Platform, IN gctUINT32 DevIndex, IN gceCORE GPU, IN gctBOOL Enable)
{
#ifdef CONFIG_PM
    struct device *dev = &Platform->device->dev;
    int ret = 0;

    if (Enable) {
        ret = pm_runtime_get_sync(dev);
        if (ret < 0) {
            pm_runtime_mark_last_busy(dev);
            ret = pm_runtime_put_autosuspend(dev);
        }
    }
    else {
        pm_runtime_mark_last_busy(dev);
        ret = pm_runtime_put_autosuspend(dev);
    }

    return (ret < 0 ? gcvSTATUS_INVALID_REQUEST : gcvSTATUS_OK);
#else
    int ret;
    ret = set_clock(Enable);

    return (ret ? gcvSTATUS_INVALID_REQUEST : gcvSTATUS_OK);
#endif
}

gceSTATUS
_Reset(IN gcsPLATFORM * Platform, IN gctUINT32 DevIndex, IN gceCORE GPU)
{
    struct st_priv* priv = stpriv;
    struct reset_control *rstc = priv->rstc;
    int timeout = 10;
    int ret = 0;

    /*
     *  In order to reset the GPU the user has to write this bit to '1', and
     *  read the GPURST bit until it is read to'0'. This bit is cleared by
     *  hardware.
     */
    if (GPU != gcvCORE_MAJOR)
        goto end;

    if (!rstc)
         goto end;

    ret = reset_control_assert(rstc);
    if (ret) {
        gckOS_Print("galcore platform st: reset error(%d)\n", ret);
        goto end;
    }

    do {
        ret = reset_control_status(rstc);
        if (ret == 0)
             break;

        udelay(2);
    } while (timeout--);

    if (timeout <= 0)
        gckOS_Print("galcore platform st: reset timeout\n");

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
    .setClock      = _SetClock,
    .reset         = _Reset,
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
