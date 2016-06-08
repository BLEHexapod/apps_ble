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

#include <string.h>
#include <stdlib.h>
#include "ble_hexapod_service.h"
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "ble.h"
#include "app_util.h"
#include "app_error.h"

struct hexaSrvHandle {
    uint16_t serviceHandle;
    uint16_t connHandle;
    ble_srv_cccd_security_mode_t charAttr;
    ble_gap_conn_sec_mode_t reportReadPerm;
    ble_gatts_char_handles_t charDirHandle;
    ble_gatts_char_handles_t charDistHandle;
    hexaSrvEventHandler_t writeHandler;
    uint8_t uuidType;
};

static void onConnect(ble_hexaSrvHandle_t handle, ble_evt_t *bleEvent)
{
    handle->connHandle = bleEvent->evt.gap_evt.conn_handle;
}

static void onDisconnect(ble_hexaSrvHandle_t handle, ble_evt_t *bleEvent)
{
    handle->connHandle = BLE_CONN_HANDLE_INVALID;
}

static void onWrite(ble_hexaSrvHandle_t handle, ble_evt_t *bleEvent)
{
	ble_gatts_evt_write_t *writeEvt = &bleEvent->evt.gatts_evt.params.write;
	if(writeEvt->handle == handle->charDirHandle.value_handle) {
		if(handle->writeHandler)
			handle->writeHandler((hexaSrvDirs_t)writeEvt->data[0]);
	}
}

static uint32_t addDirChar(ble_hexaSrvHandle_t handle)
{
    uint32_t errCode;
    ble_uuid_t hexaCharUuid;

    ble_gatts_char_md_t gattChar;
    ble_gatts_attr_md_t gattAttr;
    ble_gatts_attr_t gattAttrVal;
    ble_gatts_attr_md_t cccdAttr;

    memset(&cccdAttr, 0, sizeof(cccdAttr));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccdAttr.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccdAttr.write_perm);
    cccdAttr.vloc = BLE_GATTS_VLOC_STACK;

    memset(&gattChar, 0, sizeof(gattChar));
    gattChar.char_props.write = 1;
    gattChar.char_props.write_wo_resp = 1;
    gattChar.p_char_user_desc = NULL;
    gattChar.p_char_user_desc = NULL;
    gattChar.p_char_pf = NULL;
    gattChar.p_cccd_md = &cccdAttr;
    gattChar.p_sccd_md = NULL;

    memset(&gattAttr, 0, sizeof(gattAttr));
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&gattAttr.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&gattAttr.write_perm);
    gattAttr.vloc = BLE_GATTS_VLOC_STACK;
    gattAttr.rd_auth = 0;
    gattAttr.wr_auth = 0;
    gattAttr.vlen = 0;

    hexaCharUuid.type = handle->uuidType;
    hexaCharUuid.uuid = HEXA_UUID_DIRECTION;

    memset(&gattAttrVal, 0, sizeof(gattAttrVal));
    gattAttrVal.p_uuid = &hexaCharUuid;
    gattAttrVal.p_attr_md = &gattAttr;
    gattAttrVal.init_len = sizeof(uint8_t);
    gattAttrVal.max_len = sizeof(uint8_t);
    gattAttrVal.init_offs = 0;
    gattAttrVal.p_value = NULL;

    errCode = sd_ble_gatts_characteristic_add(handle->serviceHandle,
            &gattChar,
            &gattAttrVal,
            &handle->charDirHandle);

    return errCode;

}

static uint32_t addDistChar(ble_hexaSrvHandle_t handle)
{
    uint32_t errCode;
    ble_uuid_t hexaCharUuid;

    ble_gatts_char_md_t gattChar;
    ble_gatts_attr_md_t gattAttr;
    ble_gatts_attr_t gattAttrVal;
    ble_gatts_attr_md_t cccdAttr;

    memset(&cccdAttr, 0, sizeof(cccdAttr));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccdAttr.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccdAttr.write_perm);
    cccdAttr.vloc = BLE_GATTS_VLOC_STACK;

    memset(&gattChar, 0, sizeof(gattChar));
    gattChar.char_props.read = 1;
    gattChar.char_props.notify = 1;
    gattChar.p_char_user_desc = NULL;
    gattChar.p_char_user_desc = NULL;
    gattChar.p_char_pf = NULL;
    gattChar.p_cccd_md = &cccdAttr;
    gattChar.p_sccd_md = NULL;

    memset(&gattAttr, 0, sizeof(gattAttr));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&gattAttr.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&gattAttr.write_perm);
    gattAttr.vloc = BLE_GATTS_VLOC_STACK;
    gattAttr.rd_auth = 0;
    gattAttr.wr_auth = 0;
    gattAttr.vlen = 0;

    hexaCharUuid.type = handle->uuidType;
    hexaCharUuid.uuid = HEXA_UUID_DISTANCE;

    memset(&gattAttrVal, 0, sizeof(gattAttrVal));
    gattAttrVal.p_uuid = &hexaCharUuid;
    gattAttrVal.p_attr_md = &gattAttr;
    gattAttrVal.init_len = sizeof(uint8_t);
    gattAttrVal.max_len = GATT_MTU_SIZE_DEFAULT - 3;
    gattAttrVal.init_offs = 0;
    gattAttrVal.p_value = NULL;

    errCode = sd_ble_gatts_characteristic_add(handle->serviceHandle,
            &gattChar,
            &gattAttrVal,
            &handle->charDistHandle);

    return errCode;
}

ble_hexaSrvHandle_t ble_hexaSrvInit(hexaSrvEventHandler_t eventHandler)
{
    uint32_t errCode;

    ble_hexaSrvHandle_t handle;
    ble_uuid_t hexaSrvUuid;
    ble_uuid128_t baseuuid = {HEXA_UUID_BASE};

    // Add services
    handle = calloc(1, sizeof(struct hexaSrvHandle));

    errCode = sd_ble_uuid_vs_add(&baseuuid, &handle->uuidType);
    APP_ERROR_CHECK(errCode);

    hexaSrvUuid.type = handle->uuidType;
    hexaSrvUuid.uuid = HEXA_UUID_SERVICE;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&handle->reportReadPerm);

    errCode = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
            &hexaSrvUuid, &handle->serviceHandle);
    APP_ERROR_CHECK(errCode);

    // Add characteristic
    addDistChar(handle);
    addDirChar(handle);

    return handle;
}

void ble_hexaSrvBleHandleEvent(ble_hexaSrvHandle_t handle, ble_evt_t *bleEvent)
{
    switch(bleEvent->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            onConnect(handle, bleEvent);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            onDisconnect(handle, bleEvent);
            break;
        case BLE_GATTS_EVT_WRITE:
        	onWrite(handle, bleEvent);
        	break;
        default:
        	break;
    }
}

