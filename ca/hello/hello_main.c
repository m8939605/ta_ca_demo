/*
 * Copyright (c) 2022, hello_demo
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

#include <ta_hello.h>

#define LOG_I printf

const TEEC_UUID hello_user_ta_uuid = TA_HELLO_UUID;
char *_device = "optee-tz";
TEEC_Context teec_ctx;
TEEC_Session *session = NULL;

static void hello_teec_invoke(TEEC_Session *session)
{
	uint32_t ret;
	uint8_t hello_out[16];
	uint8_t hello_in[16] = { 0 };

	TEEC_Operation op = { 0 };
	
	LOG_I("hello_teec_invoke  \n");

	op.params[0].tmpref.buffer = hello_in;
	op.params[0].tmpref.size = sizeof(hello_in);
	op.params[1].tmpref.buffer = hello_out;
	op.params[1].tmpref.size = sizeof(hello_out);
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					TEEC_MEMREF_TEMP_OUTPUT,
					TEEC_NONE,
					TEEC_NONE);

	ret = TEEC_InvokeCommand(session, TA_HELLO_CMD_1, &op, NULL);
	LOG_I("ret=0x%x\n", ret);
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
	
	ret = TEEC_OpenSession(&teec_ctx, session, &hello_user_ta_uuid,TEEC_LOGIN_PUBLIC, NULL, NULL, NULL);
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

	LOG_I("\nhello CA enter device=[%s]\n", _device);

	open_tee();
	hello_teec_invoke(session);
	close_tee();

	LOG_I("hello CA exit!\n");
	return 0;
}

