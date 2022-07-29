/*
 * Copyright (c) 2022, parameters_demo
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
#include <stdio.h>
#include <string.h>
#include <limits.h>
#ifdef USER_SPACE
#include <pthread.h>
#include <unistd.h>
#endif
#include <signed_hdr.h>
#include <tee_client_api.h>
#include <tee_api_defines.h>

#include <ta_parameters.h>

#define LOG_I printf

const TEEC_UUID parameters_user_ta_uuid = TA_PARAMETERS_UUID;
char *_device = "optee-tz";
TEEC_Context teec_ctx;
TEEC_Session *session = NULL;

static void parameters_teec_invoke(TEEC_Session *session)
{
	uint32_t ret;
	uint8_t parameters_out[16];
	uint8_t parameters_in[16] = { 0 };
	TEEC_SharedMemory in_shm = {};
	TEEC_SharedMemory out_shm = {};
	TEEC_SharedMemory shm = {};
	size_t page_size = 4096;
	uint8_t *in = NULL, *out = NULL;


	TEEC_Operation op = { 0 };
	
	LOG_I("parameters_teec_invoke  \n");

	//for value
	op.params[0].value.a = 3;
	op.params[0].value.b = 27;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
					TEEC_VALUE_OUTPUT,
					TEEC_NONE,
					TEEC_NONE);

	ret = TEEC_InvokeCommand(session, TA_PARAMETERS_VALUE, &op, NULL);
	LOG_I("value ret = %d, a = %d, b = %d, (a+b) = %d, (a*b) = %d\n", ret, 
		op.params[0].value.a, op.params[0].value.b, 
		op.params[1].value.a, op.params[1].value.b);

	//for temref
	parameters_in[0] = 2;
	parameters_in[1] = 5;
	memset(parameters_out, 0, sizeof(parameters_out));

	op.params[0].tmpref.buffer = parameters_in;
	op.params[0].tmpref.size = sizeof(parameters_in);
	op.params[1].tmpref.buffer = parameters_out;
	op.params[1].tmpref.size = sizeof(parameters_out);
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					TEEC_MEMREF_TEMP_OUTPUT,
					TEEC_NONE,
					TEEC_NONE);
	ret = TEEC_InvokeCommand(session, TA_PARAMETERS_TEMPREF, &op, NULL);
	LOG_I("temref ret = %d, a = %d, b = %d, (a+b) = %d, (a*b) = %d\n", ret, 
		parameters_in[0], parameters_in[1],
		parameters_out[0], parameters_out[1]);

	//for memref: TEEC_RegisterSharedMemory
	parameters_in[0] = 3;
	parameters_in[1] = 6;
	memset(parameters_out, 0, sizeof(parameters_out));

	in_shm.buffer = parameters_in;
	in_shm.flags = TEEC_MEM_INPUT;
	in_shm.size = sizeof(parameters_in);
	TEEC_RegisterSharedMemory(&teec_ctx, &in_shm);

	out_shm.buffer = parameters_out;
	out_shm.flags = TEEC_MEM_OUTPUT;
	out_shm.size = sizeof(parameters_out);
	TEEC_RegisterSharedMemory(&teec_ctx, &out_shm);

	op.params[0].memref.parent = &in_shm;
	op.params[0].memref.size = sizeof(parameters_in);
	op.params[0].memref.offset = 0;

	op.params[1].memref.parent = &out_shm;
	op.params[1].memref.size = sizeof(parameters_out);
	op.params[1].memref.offset = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT,
					TEEC_MEMREF_PARTIAL_OUTPUT,
					TEEC_NONE,
					TEEC_NONE);
	ret = TEEC_InvokeCommand(session, TA_PARAMETERS_MEMREF1, &op, NULL);
	LOG_I("memref:RegisterSharedMemory ret = %d, a = %d, b = %d, (a+b) = %d, (a*b) = %d\n", ret, 
		parameters_in[0], parameters_in[1],
		parameters_out[0], parameters_out[1]);

	TEEC_ReleaseSharedMemory(&in_shm);
	TEEC_ReleaseSharedMemory(&out_shm);

	//for memref: TEEC_AllocateSharedMemory
	shm.size = 2 * page_size;
	shm.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	TEEC_AllocateSharedMemory(&teec_ctx, &shm);

	in = (uint8_t *) shm.buffer;
	out = (uint8_t*) shm.buffer + page_size;
	in [0] = 7;
	in [1] = 8;
	memset(out, 0, page_size);

	op.params[0].memref.parent = &shm;
	op.params[0].memref.size = sizeof(page_size);
	op.params[0].memref.offset = 0;

	op.params[1].memref.parent = &shm;
	op.params[1].memref.size = sizeof(page_size);
	op.params[1].memref.offset = page_size;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT,
					TEEC_MEMREF_PARTIAL_OUTPUT,
					TEEC_NONE,
					TEEC_NONE);
	ret = TEEC_InvokeCommand(session, TA_PARAMETERS_MEMREF2, &op, NULL);
	LOG_I("memref:AllocateSharedMemory ret = %d, a = %d, b = %d, (a+b) = %d, (a*b) = %d\n", ret, 
		in[0], in[1],
		out[0], out[1]);

	TEEC_ReleaseSharedMemory(&in_shm);
	TEEC_ReleaseSharedMemory(&out_shm);
}

static void open_tee()
{
	TEEC_Result ret = TEEC_SUCCESS;

	TEEC_InitializeContext(_device, &teec_ctx);
	session = (TEEC_Session *)malloc(sizeof(TEEC_Session));
	if(session == NULL)
	{
		LOG_I("%s failed, %d\n",__func__,__LINE__);
	}
	
	ret = TEEC_OpenSession(&teec_ctx, session, &parameters_user_ta_uuid,TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
	if(ret != TEEC_SUCCESS)
	{
		LOG_I("%s failed, %d\n",__func__,__LINE__);
	}
}

static void close_tee()
{
	TEEC_CloseSession(session);
	free(session);
	TEEC_FinalizeContext(&teec_ctx);
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	LOG_I("\nparameters CA enter device=[%s]\n", _device);

	open_tee();
	parameters_teec_invoke(session);
	close_tee();

	LOG_I("parameters CA exit!\n");
	return 0;
}

