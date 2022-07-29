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
#include <tee_ta_api.h>
#include <trace.h>
#include <tee_api.h>
#include <tee_api_defines.h>

#include "ta_storage.h"

TEE_Result TA_CreateEntryPoint(void)
{
	EMSG("%s enter", __func__);
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
	EMSG("%s enter", __func__);
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t ptype,
                                    TEE_Param param[4],
                                    void **session_id_ptr)
{
	(void)(ptype);
	(void)(param);
	(void)(session_id_ptr);

	EMSG("%s enter", __func__);

	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *sess_ptr)
{
	(void)(sess_ptr);
	EMSG("%s enter", __func__);
}

TEE_Result write_raw_object(const char *obj_id, size_t obj_id_sz, const char *data, size_t data_sz)
{
	TEE_ObjectHandle object;
	TEE_Result res;

	EMSG("%s : enter", __func__);

	res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE, obj_id, obj_id_sz, WRITE_FLAG, TEE_HANDLE_NULL, NULL, 0, &object);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_CreatePersistentObject failed 0x%08x", res);
		return res;
	}

	res = TEE_WriteObjectData(object, data, data_sz);
	if (res != TEE_SUCCESS) {
		EMSG("TEE_WriteObjectData failed 0x%08x", res);
		TEE_CloseAndDeletePersistentObject1(object);
	} else {
		TEE_CloseObject(object);
	}
	
	EMSG("%s : exit", __func__);
	return res;
}

TEE_Result read_raw_object(const char *obj_id, size_t obj_id_sz, const char *data, size_t *data_sz)
{
	TEE_ObjectHandle object;
	TEE_ObjectInfo object_info;
	TEE_Result res;
	
	EMSG("%s : enter", __func__);

	res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, obj_id, obj_id_sz, READ_FLAG, &object);
	if (res != TEE_SUCCESS) {
		EMSG("Failed to open persistent object, res=0x%08x", res);
		return res;
	}

	res = TEE_GetObjectInfo1(object, &object_info);
	if (res != TEE_SUCCESS) {
		EMSG("Failed to create persistent object, res=0x%08x", res);
		TEE_CloseObject(object);
		return res;
	}

	if (object_info.dataSize > *data_sz) {
		TEE_CloseObject(object);
		return TEE_ERROR_SHORT_BUFFER;
	}

	res = TEE_ReadObjectData(object, data, object_info.dataSize, (uint32_t* )data_sz);
	if (res != TEE_SUCCESS || *data_sz != object_info.dataSize) {
		EMSG("Failed to TEE_ReadObjectData, res=0x%08x", res);
		TEE_CloseObject(object);
		return res;
	}

	TEE_CloseObject(object);
	
	EMSG("%s : exit", __func__);
	return res;
}

TEE_Result delete_object(const char *obj_id, size_t obj_id_sz)
{
	TEE_ObjectHandle object;
	TEE_Result res;

	res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, obj_id, obj_id_sz, DELETE_FLAG, &object);
	if (res != TEE_SUCCESS) {
		EMSG("Failed to open persistent object, res=0x%08x", res);
		return res;
	}

	TEE_CloseAndDeletePersistentObject1(object);
	return res;
}

TEE_Result list_object(void)
{
	TEE_ObjectEnumHandle hEnumerator = TEE_HANDLE_NULL;
	TEE_Result nResult = TEE_SUCCESS;
	char     szName[64];
	TEE_ObjectInfo sObjectInfo;
	size_t   nPersistentObjectLength = 0;

	nResult = TEE_AllocatePersistentObjectEnumerator(&hEnumerator);
	
	TEE_ResetPersistentObjectEnumerator(hEnumerator);

	nResult = TEE_StartPersistentObjectEnumerator(hEnumerator, TEE_STORAGE_PRIVATE);
	
	while(true){
		nResult = TEE_GetNextPersistentObject(hEnumerator, &sObjectInfo, szName, &nPersistentObjectLength);
		if (nResult != TEE_SUCCESS) { 
			EMSG("Failed get next persistent object\n"); 
			break; 
		}
		szName[nPersistentObjectLength] = '\0';
		EMSG("%s Found file name: %s : %zd\n", __func__, szName, nPersistentObjectLength);				
	}	
	
	TEE_FreePersistentObjectEnumerator(hEnumerator);
	
	return nResult;
}


void test_storeage(void)
{
	char buf[10] = {0};
	size_t buf_sz = 0;

	EMSG("-------------------------------------------------------");
	write_raw_object("file_1", 6, "storage_1",9);
	write_raw_object("file_2", 6, "storage_2",9);
	write_raw_object("file_3", 6, "storage_3",9);
	write_raw_object("file_4", 6, "storage_4",9);
	write_raw_object("file_5", 6, "storage_5",9);
	write_raw_object("file_6", 6, "storage_6",9);
	write_raw_object("file_7", 6, "storage_7",9);
	write_raw_object("file_8", 6, "storage_8",9);	
	write_raw_object("file_9", 6, "storage_9",9);		

	list_object();
	EMSG("==================================================");
	
	delete_object("file_3", 6);
	delete_object("file_7", 6);
	list_object();

	buf_sz = 10;

	read_raw_object("file_2", 6, buf, &buf_sz);
	EMSG("******file_2: buf = %s\n", buf);
	read_raw_object("file_4", 6, buf, &buf_sz);
	EMSG("******file_4: buf = %s\n", buf);
	read_raw_object("file_6", 6, buf, &buf_sz);
	EMSG("******file_6: buf = %s\n", buf);		
}


TEE_Result TA_InvokeCommandEntryPoint(void *session_id,
                                      uint32_t command_id,
                                      uint32_t param_types,
                                      TEE_Param parameters[4])
{
	(void)(session_id);
	(void)(parameters);

	EMSG("%s enter", __func__);

	if ((TEE_PARAM_TYPE_GET(param_types, 0) != TEE_PARAM_TYPE_MEMREF_INPUT) ||
		(TEE_PARAM_TYPE_GET(param_types, 1) != TEE_PARAM_TYPE_MEMREF_OUTPUT) ||
		(TEE_PARAM_TYPE_GET(param_types, 2) != TEE_PARAM_TYPE_NONE) ||
		(TEE_PARAM_TYPE_GET(param_types, 3) != TEE_PARAM_TYPE_NONE)) {
		EMSG("The param_types is error!");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	switch (command_id) {
		case TA_STORAGE_CMD_1:
			EMSG("--TA_STORAGE_CMD_1");
			test_storeage();
			break;
		case TA_STORAGE_CMD_2:
			EMSG("--TA_STORAGE_CMD_2");
			break;
		case TA_STORAGE_CMD_3:
			EMSG("--TA_STORAGE_CMD_3");
			break;
		default:
			EMSG("--TA_STORAGE_CMD_UNKNOW!");
			break;
	}

	EMSG("%s exit", __func__);

	return TEE_SUCCESS;
}
