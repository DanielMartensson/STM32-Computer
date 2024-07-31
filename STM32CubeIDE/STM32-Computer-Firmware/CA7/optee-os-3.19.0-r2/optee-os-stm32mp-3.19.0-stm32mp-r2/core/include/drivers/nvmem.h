/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2023, Microchip
 */

#ifndef __DRIVERS_NVMEM_H
#define __DRIVERS_NVMEM_H

#include <kernel/dt_driver.h>
#include <stdlib.h>
#include <tee_api_defines.h>
#include <tee_api_types.h>
#include <types_ext.h>

struct nvmem_cell;

/*
 * struct nvmem_ops - Nvmem device driver operations
 * @read_cell: Load at max @buf_len bytes in @data and output effective number
 *             of loaded byte in @read_len(if @read_len != NULL)
 * @free_cell: Release resources allocated from nvmem_dt_get_func callback
 *             function
 */
struct nvmem_ops {
	TEE_Result (*read_cell)(struct nvmem_cell *cell, uint8_t *data,
				size_t buf_len, size_t *read_len);
	TEE_Result (*write_cell)(struct nvmem_cell *cell, uint8_t *data,
				 size_t len);
	TEE_Result (*free_cell)(struct nvmem_cell *cell);
};

/*
 * struct nvmem_cell - Description of an nvmem cell
 * @offset: Cell byte offset in the NVMEM device
 * @len: Cell byte size
 * @ops: Nvmem device driver operation handlers
 * @drv_data: Nvmem device driver private data
 */
struct nvmem_cell {
	paddr_t offset;
	size_t len;
	const struct nvmem_ops *ops;
	void *drv_data;
};

/*
 * nvmem_dt_get_func - Typedef of function to get an nvmem reference from a DT
 * node
 *
 * @a: Reference to phandle arguments
 * @data: Pointer to data given at nvmem_register_provider() call
 * @res: Output result code of the operation:
 *	 TEE_SUCCESS in case of success
 *	 TEE_ERROR_DEFER_DRIVER_INIT if NVMEM device is not initialized
 *	 Any TEE_Result compliant code in case of error.
 */
typedef struct nvmem_cell *(*nvmem_dt_get_func)(struct dt_pargs *a, void *data,
						TEE_Result *res);

#ifdef CFG_DRIVERS_NVMEM
/**
 * nvmem_register_provider() - Register a nvmem controller
 *
 * @fdt: Device tree to work on
 * @nodeoffset: Node offset of nvmem cell consumer
 * @get_dt_nvmem: Callback to match the devicetree nvmem reference with
 *                nvmem_cell
 * @data: Data which will be passed to the get_dt_nvmem callback
 * Return a TEE_Result compliant value
 */
static inline TEE_Result nvmem_register_provider(const void *fdt,
						 int nodeoffset,
						 nvmem_dt_get_func get_dt_nvmem,
						 void *data)
{
	return dt_driver_register_provider(fdt, nodeoffset,
					   (get_of_device_func)get_dt_nvmem,
					   data, DT_DRIVER_NVMEM);
}

/**
 * nvmem_get_cell_by_name() - Obtain a nvmem cell from its name in the DT node
 * The NVMEM name shall match an entry of DT node property nvmem-cell-names.
 *
 * @fdt: Device tree to work on
 * @nodeoffset: Node offset of nvmem cell consumer
 * @name: name of the nvmem cell to obtain from the device tree
 * @cell: Pointer filled with the retrieved cell, must be freed after use
	  using nvmem_put_cell()
 * Return a TEE_Result compliant value
 */
TEE_Result nvmem_get_cell_by_name(const void *fdt, int nodeoffset,
				  const char *name, struct nvmem_cell **cell);

/**
 * nvmem_get_cell_by_index() - Obtain a nvmem cell from property nvmem-cells
 *
 * @fdt: Device tree to work on
 * @nodeoffset: Node offset of nvmem cell consumer
 * @index: Index of the nvmem cell to obtain from device-tree
 * @cell: Pointer filled with the retrieved cell, must be freed after use
 *        using nvmem_put_cell()
 * Return a TEE_Result compliant value
 */
static inline TEE_Result nvmem_get_cell_by_index(const void *fdt,
						 int nodeoffset,
						 unsigned int index,
						 struct nvmem_cell **cell)
{
	TEE_Result res = TEE_ERROR_GENERIC;

	*cell = dt_driver_device_from_node_idx_prop("nvmem-cells", fdt,
						    nodeoffset, index,
						    DT_DRIVER_NVMEM, &res);
	return res;
}

/**
 * nvmem_cell_parse_dt() - Parse device-tree information to fill a nvmem cell
 *
 * @fdt: Device tree to work on
 * @nodeoffset: Node offset of the nvmem cell controller
 * @cell: Pointer to cell that will be filled
 */
TEE_Result nvmem_cell_parse_dt(const void *fdt, int nodeoffset,
			       struct nvmem_cell *cell);

/**
 * nvmem_put_cell() - Free resource allocated from nvmem_get_cell_by_*()
 *
 * @cell: Cell to be freed
 */
static inline TEE_Result nvmem_put_cell(struct nvmem_cell *cell)
{
	if (cell->ops->free_cell)
		return cell->ops->free_cell(cell);
	else
		return TEE_SUCCESS;
}

/*
 * nvmem_cell_read() - read data from a nvmem cell
 * @cell: Cell to read from nvmem
 * @data: Output buffer (caller's allocated) where nvmem cell data are stored
 *        upon success,
 * @buf_len: Size of the allocated buffer
 * @len: Output byte size of the nvmem cell data read
 */
static inline TEE_Result nvmem_cell_read(struct nvmem_cell *cell, uint8_t *data,
					 size_t buf_len, size_t *read_len)
{
	if (!cell->ops->read_cell)
		return TEE_ERROR_NOT_SUPPORTED;

	return cell->ops->read_cell(cell, data, buf_len, read_len);
}

/*
 * nvmem_cell_alloc_read() - Allocate and read data from a nvmem cell
 * @cell: Cell to read from nvmem
 * @data: Output buffer allocated from the heap (malloc() and friends)
 * @len: Output size of @data, i.e. byte size of the nvmem cell read
 *
 * Caller is responsible for freeing @data buffer when needed.
 */
static inline TEE_Result nvmem_cell_alloc_read(struct nvmem_cell *cell,
					       uint8_t **data, size_t *len)
{
	TEE_Result ret = TEE_ERROR_GENERIC;

	if (!cell->ops->read_cell)
		return TEE_ERROR_NOT_SUPPORTED;

	*data = calloc(1, cell->len);
	if (!*data)
		return TEE_ERROR_OUT_OF_MEMORY;

	ret = cell->ops->read_cell(cell, *data, cell->len, len);

	if (ret != TEE_SUCCESS)
		free(*data);

	return ret;
}

/*
 * nvmem_cell_write() - write data to a nvmem cell
 * @cell: Cell to write
 * @data: Input buffer
 * @buf_len: Size of the buffer to write in the cell
 */
static inline TEE_Result nvmem_cell_write(struct nvmem_cell *cell,
					  uint8_t *data, size_t buf_len)
{
	if (!cell->ops->write_cell)
		return TEE_ERROR_NOT_SUPPORTED;

	return cell->ops->write_cell(cell, data, buf_len);
}
#else /* CFG_DRIVERS_NVMEM */
static inline TEE_Result nvmem_register_provider(const void *fdt __unused,
						 int nodeoffset __unused,
						 nvmem_dt_get_func fn __unused,
						 void *data __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}

static inline TEE_Result nvmem_get_cell_by_name(const void *fdt __unused,
						int nodeoffset __unused,
						const char *name __unused,
						struct nvmem_cell **c __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}

static inline TEE_Result
nvmem_get_cell_by_index(const void *fdt __unused, int nodeoffset __unused,
			unsigned int index __unused,
			struct nvmem_cell **cell __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}

static inline TEE_Result nvmem_cell_parse_dt(const void *fdt __unused,
					     int nodeoffset __unused,
					     struct nvmem_cell *cell __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}

static inline void nvmem_put_cell(struct nvmem_cell *cell __unused)
{
}

static inline TEE_Result nvmem_cell_read(struct nvmem_cell *cell __unused,
					 uint8_t *data __unused,
					 size_t buf_len __unused,
					 size_t *read_len __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}

static inline TEE_Result nvmem_cell_write(struct nvmem_cell *cell __unused,
					  uint8_t *data __unused,
					  size_t buf_len __unused)
{
	return TEE_ERROR_NOT_SUPPORTED;
}

#endif /* CFG_DRIVERS_NVMEM */
#endif /* __DRIVERS_NVMEM_H */
