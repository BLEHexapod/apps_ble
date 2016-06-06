/*
 * Copyright 2016 Bart Monhemius.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "ble.h"
#include "ble_types.h"

#define HEXA_UUID_BASE  		{ 0x98, 0x4B, 0xCC, 0x12, 0x0A, 0xE3, 0x4F, 0xEC, 0xBE, \
									0xA3, 0x4A, 0xD3, 0x39, 0xD6, 0x74, 0xD0}
#define HEXA_UUID_SERVICE		0x9E5D
#define HEXA_UUID_DIRECTION     0x9E5E
#define HEXA_UUID_DISTANCE      0x9E5F

typedef enum {
	HEXA_FORWARD = 0,
	HEXA_FORWARD_LEFT,
	HEXA_FORWARD_RIGHT,
	HEXA_LEFT,
	HEXA_RIGHT
} hexaSrvDirs_t;

typedef struct hexaSrvHandle *ble_hexaSrvHandle_t;
typedef void(*hexaSrvEventHandler_t)(hexaSrvDirs_t);
/**
 * @brief	Initialise the hexapod BLE service.
 * @return
 */
ble_hexaSrvHandle_t ble_hexaSrvInit(hexaSrvEventHandler_t eventHandler);

/**
 * @brief Handle the hexapod BLE events.
 * @param handle	Service handle.
 * @param bleEvent	BLE
 */
void ble_hexaSrvBleHandleEvent(ble_hexaSrvHandle_t handle, ble_evt_t *bleEvent);
