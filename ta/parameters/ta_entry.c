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
#include <tee_ta_api.h>
#include <trace.h>
#include <tee_api.h>
#include <tee_api_defines.h>

#include "ta_parameters.h"

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

TEE_Result TA_InvokeCommandEntryPoint(void *session_id,
                                      uint32_t command_id,
                                      uint32_t param_types,
                                      TEE_Param parameters[4])
{
	(void)(session_id);
	(void)(parameters);
	uint8_t *in = NULL, *out = NULL;

	EMSG("%s enter", __func__);

	switch (command_id) {
		case TA_PARAMETERS_VALUE:
			EMSG("--TA_PARAMETERS_TEMPREF");
			if ((TEE_PARAM_TYPE_GET(param_types, 0) != TEE_PARAM_TYPE_VALUE_INPUT) ||
				(TEE_PARAM_TYPE_GET(param_types, 1) != TEE_PARAM_TYPE_VALUE_OUTPUT) ||
				(TEE_PARAM_TYPE_GET(param_types, 2) != TEE_PARAM_TYPE_NONE) ||
				(TEE_PARAM_TYPE_GET(param_types, 3) != TEE_PARAM_TYPE_NONE)) {
				EMSG("The param_types is error!");
				return TEE_ERROR_BAD_PARAMETERS;
			}
			parameters[1].value.a = parameters[0].value.a + parameters[0].value.b;
			parameters[1].value.b = parameters[0].value.a * parameters[0].value.b;
			break;

		case TA_PARAMETERS_TEMPREF:
			EMSG("--TA_PARAMETERS_TEMPREF");
			if ((TEE_PARAM_TYPE_GET(param_types, 0) != TEE_PARAM_TYPE_MEMREF_INPUT) ||
				(TEE_PARAM_TYPE_GET(param_types, 1) != TEE_PARAM_TYPE_MEMREF_OUTPUT) ||
				(TEE_PARAM_TYPE_GET(param_types, 2) != TEE_PARAM_TYPE_NONE) ||
				(TEE_PARAM_TYPE_GET(param_types, 3) != TEE_PARAM_TYPE_NONE)) {
				EMSG("The param_types is error!");
				return TEE_ERROR_BAD_PARAMETERS;
			}

			out = parameters[1].memref.buffer;
			in = parameters[0].memref.buffer;
			EMSG("in[0] = %d, in[1] = %d", in[0], in[1]);
			out[0] = in[0] + in[1];
			out[1] = in[0] * in[1];
			EMSG("out[0] = %d, out[1] = %d", out[0], out[1]);
			parameters[1].memref.size = 2;
			break;

		case TA_PARAMETERS_MEMREF1:
		case TA_PARAMETERS_MEMREF2:
			EMSG("--TA_PARAMETERS_MEMREF");
			if ((TEE_PARAM_TYPE_GET(param_types, 0) != TEE_PARAM_TYPE_MEMREF_INPUT) ||
				(TEE_PARAM_TYPE_GET(param_types, 1) != TEE_PARAM_TYPE_MEMREF_OUTPUT) ||
				(TEE_PARAM_TYPE_GET(param_types, 2) != TEE_PARAM_TYPE_NONE) ||
				(TEE_PARAM_TYPE_GET(param_types, 3) != TEE_PARAM_TYPE_NONE)) {
				EMSG("The param_types is error!");
				return TEE_ERROR_BAD_PARAMETERS;
			}

			out = parameters[1].memref.buffer;
			in = parameters[0].memref.buffer;
			EMSG("in[0] = %d, in[1] = %d", in[0], in[1]);
			out[0] = in[0] + in[1];
			out[1] = in[0] * in[1];
			EMSG("out[0] = %d, out[1] = %d", out[0], out[1]);
			parameters[1].memref.size = 2;
			break;

		default:
			EMSG("--TA_PARAMETERS_CMD_UNKNOW!");
			break;
	}

	EMSG("%s exit", __func__);

	return TEE_SUCCESS;
}
