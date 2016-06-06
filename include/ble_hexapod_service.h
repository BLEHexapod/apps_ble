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



#define HEXA_UUID_BASE  {0X98, 0X4B, 0XCC, 0X12, 0X0A, 0XE3, 0X4F, 0XEC, 0XBE,\
                            0XA3, 0X4A, 0XD3, 0X39, 0XD6, 0X74, 0XD0}

#define HEXA_UUID_DIRECTION     0X9E5E
#define HEXA_UUID_DISTANCE      0X9E5F

typedef struct hexaSrvHandle *ble_hexaSrvHandle_t;

/**
 * @brief	Initialise the hexapod BLE service.
 * @return
 */
ble_hexaSrvHandle_t ble_hexaSrvInit(void);

/**
 * @brief Handle the hexapod BLE events.
 * @param handle	Service handle.
 * @param bleEvent	BLE
 */
void ble_hexaSrvBleHandleEvent(ble_hexaSrvHandle_t handle, ble_evt_t *bleEvent);
