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


#ifndef INCLUDE_BLE_STACK_H_
#define INCLUDE_BLE_STACK_H_

#include "ble_config.h"

typedef enum {
	STACK_OSC_INTERNAL = 0, /**< Use the internal RC oscillator, calibrated every 4 seconds*/
	STACK_OSC_EXTERNAL		/**< Use an external crystal oscillator*/
} STACK_OSC_SOURCE;

/**
 * Initialize ble stack
 * @param schedCallback Event handler for ble events. This function is called
 * from an interrupt
 * @param oscSource oscillator source.
 * @return NRF_SUCCES if succes, error code if fail.
 */
uint32_t stack_init(softdevice_evt_schedule_func_t schedCallback,
		STACK_OSC_SOURCE oscSource);

/**
 * Configure the ble stack.
 * @param sysEventHandler Callback to hand system evenys.This function is called
 * from an interrupt when a system event occurred is the softdevice.
 * @param bleEventHandler Callback to handle BLE events. This function is called
 * when a BLE event is occurred/
 * @return NRF_SUCCES if succes, error code if fail.
 */
uint32_t stack_config(sys_evt_handler_t sysEventHandler,
		ble_evt_handler_t bleEventHandler);



#endif /* INCLUDE_BLE_STACK_H_ */
