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

#ifndef TA_PARAMETERS_H
#define TA_PARAMETERS_H

#define TA_PARAMETERS_UUID { 0xf5871586, 0x14f6, 0x40ca, { 0x16, 0xfc, 0xe2, 0x5c, 0x9c, 0xae, 0x7d, 0xbf } }

enum {
	TA_PARAMETERS_VALUE,
	TA_PARAMETERS_TEMPREF,
	TA_PARAMETERS_MEMREF1,
	TA_PARAMETERS_MEMREF2,
	TA_PARAMETERS_MAX
};

#endif /*TA_PARAMETERS_H */
