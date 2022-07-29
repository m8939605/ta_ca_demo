/*
 * Copyright (c) 2022, storage_demo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef TA_STORAGE_H
#define TA_STORAGE_H

#define TA_STORAGE_UUID { 0x06af7e09, 0x3557, 0x4793, { 0x35, 0x04, 0x81, 0x2e, 0x64, 0x82, 0x79, 0x55 } }

#define TA_STORAGE_CMD_1	1
#define TA_STORAGE_CMD_2	2
#define TA_STORAGE_CMD_3	3

#define DELETE_FLAG	(TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE_META)
#define READ_FLAG	(TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_SHARE_READ)
#define WRITE_FLAG	(TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_OVERWRITE)

TEE_Result write_raw_object(const char *obj_id, size_t obj_id_sz, const char *data, size_t data_sz);
TEE_Result read_raw_object(const char *obj_id, size_t obj_id_sz, const char *data, size_t *data_sz);
TEE_Result delete_object(const char *obj_id, size_t obj_id_sz);
TEE_Result list_object(void);
void test_storeage(void);

#endif /*TA_STORAGE_H */
