// Copyright 2017-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"

#include "ble_mesh_node_info.h"
#include "main.h"

static const char *TAG = "ble_mesh_reg_cfg_client_cmd";


extern esp_ble_mesh_client_t config_client;
extern esp_ble_mesh_client_t onoff_client;
extern esp_ble_mesh_key_t prov_key;


static void ble_mesh_configuration_client_model_cb(esp_ble_mesh_cfg_client_cb_event_t event,
                                                   esp_ble_mesh_cfg_client_cb_param_t *param)
{
    esp_ble_mesh_client_common_param_t common = {0};
    esp_ble_mesh_node_info_t *node = NULL;

    ESP_LOGD(TAG, "enter %s, event = %x\n, error_code = %x\n", __func__, event, param->error_code);

    int err;








    const uint32_t opcode = param->params->opcode;
    const uint16_t addr = param->params->ctx.addr;


    ESP_LOGI(TAG, "%s, error_code = 0x%02x, event = 0x%02x, addr: 0x%04x, opcode: 0x%04x",
             __func__, param->error_code, event, param->params->ctx.addr, opcode);

	if (param->error_code) {
        ESP_LOGE(TAG, "Send config client message failed, opcode 0x%04x", opcode);
    ESP_LOGI(TAG, "CnfClient:Fail,%d", param->error_code);
        return;
	} 

    node = example_ble_mesh_get_node_info(addr);
    if (!node) {
        ESP_LOGE(TAG, "%s: Get node info failed", __func__);
        return;
    }


	switch (event) {
	case ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT:
		switch (opcode) {
		case ESP_BLE_MESH_MODEL_OP_BEACON_GET:
			ESP_LOGI(TAG, "CfgClient:beacon,0x%x", param->status_cb.beacon_status.beacon);
			break;
		case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET:
			{
			ESP_LOGI(TAG, "CfgClient:page,0x%x,len,0x%x", param->status_cb.comp_data_status.page, param->status_cb.comp_data_status.composition_data->len);

            ESP_LOGI(TAG, "composition data %s", bt_hex(param->status_cb.comp_data_status.composition_data->data,
                     param->status_cb.comp_data_status.composition_data->len));
            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
            set_state.app_key_add.net_idx = prov_key.net_idx;
            set_state.app_key_add.app_idx = prov_key.app_idx;
            memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                return;
            	}
			}
			break;
		case ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_GET:
			ESP_LOGI(TAG, "CfgClient:ttl,0x%x", param->status_cb.default_ttl_status.default_ttl);
			break;
		case ESP_BLE_MESH_MODEL_OP_GATT_PROXY_GET:
			ESP_LOGI(TAG, "CfgClient:proxy,0x%x", param->status_cb.gatt_proxy_status.gatt_proxy);
			break;
		case ESP_BLE_MESH_MODEL_OP_RELAY_GET:
			ESP_LOGI(TAG, "CfgClient:relay,0x%x,retransmit,0x%x", param->status_cb.relay_status.relay, param->status_cb.relay_status.retransmit);
			break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_PUB_GET:
			if (param->status_cb.model_pub_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CfgClient:PublishGet,OK,0x%x", param->status_cb.model_pub_status.publish_addr);
			} else {
				ESP_LOGI(TAG, "CfgClient:PublishGet,Fail");
			}

			break;
		case ESP_BLE_MESH_MODEL_OP_FRIEND_GET:
			ESP_LOGI(TAG, "CfgClient:friend,0x%x", param->status_cb.friend_status.friend_state);
			break;
		case ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_GET:
			if (param->status_cb.heartbeat_pub_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CfgClient:HeartBeatPubGet,OK,destination:0x%x,countlog:0x%x,periodlog:0x%x,ttl:0x%x,features:0x%x,net_idx:0x%x",
						 param->status_cb.heartbeat_pub_status.dst, param->status_cb.heartbeat_pub_status.count,  param->status_cb.heartbeat_pub_status.period,
						 param->status_cb.heartbeat_pub_status.ttl, param->status_cb.heartbeat_pub_status.features, param->status_cb.heartbeat_pub_status.net_idx);
			} else {
				ESP_LOGI(TAG, "CfgClient:HeartBeatGet,Fail,%d", param->status_cb.heartbeat_pub_status.status);
			}
			break;
		case ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_GET:
			if (param->status_cb.heartbeat_sub_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CfgClient:HeartBeatSubGet,OK,source:0x%x,destination:0x%x, periodlog:0x%x,countlog:0x%x,minhops:0x%x,maxhops:0x%x",
						 param->status_cb.heartbeat_sub_status.src, param->status_cb.heartbeat_sub_status.dst, param->status_cb.heartbeat_sub_status.period,
						 param->status_cb.heartbeat_sub_status.count, param->status_cb.heartbeat_sub_status.min_hops, param->status_cb.heartbeat_sub_status.max_hops);
			} else {
				ESP_LOGI(TAG, "CfgClient:HeartBeatSubGet,Fail,%d", param->status_cb.heartbeat_sub_status.status);
			}
			break;
		default:
			ESP_LOGI(TAG, "Not supported config client get message opcode");
			break;
		}
		break;
    case ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT:
        switch (opcode) {
		case ESP_BLE_MESH_MODEL_OP_BEACON_SET:
			ESP_LOGI(TAG, "CfgClient:beacon,0x%x", param->status_cb.beacon_status.beacon);
			break;
		case ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_SET:
			ESP_LOGI(TAG, "CfgClient:ttl,0x%x", param->status_cb.default_ttl_status.default_ttl);
			break;
		case ESP_BLE_MESH_MODEL_OP_GATT_PROXY_SET:
			ESP_LOGI(TAG, "CfgClient:proxy,0x%x", param->status_cb.gatt_proxy_status.gatt_proxy);
			break;
		case ESP_BLE_MESH_MODEL_OP_RELAY_SET:
			ESP_LOGI(TAG, "CfgClient:relay,0x%x, retransmit: 0x%x", param->status_cb.relay_status.relay, param->status_cb.relay_status.retransmit);
			break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET:
			if (param->status_cb.model_pub_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CfgClient:PublishSet,OK,0x%x", param->status_cb.model_pub_status.publish_addr);
			} else {
				ESP_LOGI(TAG, "CfgClient:PublishSet,Fail");
			}
			break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD:
			if (param->status_cb.model_sub_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CnfClient:SubAdd,OK,%x,%x", param->status_cb.model_sub_status.element_addr, param->status_cb.model_sub_status.sub_addr);
			} else {
				ESP_LOGI(TAG, "CnfClient:SubAdd,Fail,%x", param->status_cb.model_sub_status.status);
			}
			break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE:
			if (param->status_cb.model_sub_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CnfClient:SubDel,OK,%x,%x", param->status_cb.model_sub_status.element_addr, param->status_cb.model_sub_status.sub_addr);
			} else {
				ESP_LOGI(TAG, "CnfClient:SubDel,Fail,%x", param->status_cb.model_sub_status.status);
			}
			break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_OVERWRITE:
			break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_ADD:
			break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_DELETE:
			break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_VIRTUAL_ADDR_OVERWRITE:
			break;
		case ESP_BLE_MESH_MODEL_OP_NET_KEY_ADD:
			if (param->status_cb.netkey_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CfgClient:NetKeyAdd,OK");
			} else {
				ESP_LOGI(TAG, "CfgClient:NetKeyAdd,Fail,%d", param->status_cb.netkey_status.status);
			}
			break;
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD: {
			if (param->status_cb.appkey_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CnfClient:AddAppkey,OK,%x,%x,%x", param->status_cb.appkey_status.net_idx, param->status_cb.appkey_status.app_idx, param->params->ctx.addr);
			} else {
				ESP_LOGI(TAG, "CnfClient:AddAppkey,Fail,%x", param->status_cb.appkey_status.status);
			}
            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
            set_state.model_app_bind.element_addr = node->unicast;
            set_state.model_app_bind.model_app_idx = prov_key.app_idx;
            set_state.model_app_bind.model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV;
            set_state.model_app_bind.company_id = ESP_BLE_MESH_CID_NVAL;
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                return;
            }
            break;
		case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND: {
			if (param->status_cb.model_app_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CnfClient:AppkeyBind,OK,%x,%x,%x", param->status_cb.model_app_status.app_idx, param->status_cb.model_app_status.model_id, param->params->ctx.addr);
			} else {
				ESP_LOGI(TAG, "CnfClient:AppkeyBind,Fail,%x", param->status_cb.model_app_status.status);
			}
			esp_ble_mesh_generic_client_get_state_t get_state = {0};
			example_ble_mesh_set_msg_common(&common, node, onoff_client.model, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET);
			err = esp_ble_mesh_generic_client_get_state(&common, &get_state);
			if (err) {
				ESP_LOGE(TAG, "%s: Generic OnOff Get failed", __func__);
				return;


			}
			break;
		case ESP_BLE_MESH_MODEL_OP_FRIEND_SET:
			ESP_LOGI(TAG, "CfgClient:friend: 0x%x", param->status_cb.friend_status.friend_state);
			break;
		case ESP_BLE_MESH_MODEL_OP_HEARTBEAT_PUB_SET:
			if (param->status_cb.heartbeat_pub_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CfgClient:HeartBeatPubSet,OK,destination:0x%x,countlog:0x%x, periodlog:0x%x,ttl:0x%x,features:0x%x,net_idx: 0x%x",
						 param->status_cb.heartbeat_pub_status.dst, param->status_cb.heartbeat_pub_status.count, param->status_cb.heartbeat_pub_status.period,
						 param->status_cb.heartbeat_pub_status.ttl, param->status_cb.heartbeat_pub_status.features, param->status_cb.heartbeat_pub_status.net_idx);
			} else {
				ESP_LOGI(TAG, "CfgClient:HeartBeatSet,Fail,%d", param->status_cb.heartbeat_pub_status.status);
			}
			break;
		case ESP_BLE_MESH_MODEL_OP_HEARTBEAT_SUB_SET:
			if (param->status_cb.heartbeat_sub_status.status == ESP_OK) {
				ESP_LOGI(TAG, "CfgClient:HeartBeatSubSet,OK,source:0x%x,destination:0x%x, periodlog:0x%x,countlog:0x%x,minhops:0x%x,maxhops:0x%x",
						 param->status_cb.heartbeat_sub_status.src, param->status_cb.heartbeat_sub_status.dst, param->status_cb.heartbeat_sub_status.period,
						 param->status_cb.heartbeat_sub_status.count, param->status_cb.heartbeat_sub_status.min_hops, param->status_cb.heartbeat_sub_status.max_hops);
			} else {
				ESP_LOGI(TAG, "CfgClient:HeartBeatSubSet,Fail,%d", param->status_cb.heartbeat_sub_status.status);
			}
			break;


        }
        default:
			ESP_LOGI(TAG, "Not supported config client set message opcode");
			break;
        }
        }
        break;
    case ESP_BLE_MESH_CFG_CLIENT_PUBLISH_EVT:
			ESP_LOGI(TAG, "CnfClient:Publish,OK");
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_STATUS:
            ESP_LOG_BUFFER_HEX("composition data %s", param->status_cb.comp_data_status.composition_data->data,
                               param->status_cb.comp_data_status.composition_data->len);
            break;
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_STATUS:
            break;
        default:
            break;
        }
        break;
	case ESP_BLE_MESH_CFG_CLIENT_EVT_MAX:
		ESP_LOGI(TAG, "CnfClient:MaxEvt");
		break;
    case ESP_BLE_MESH_CFG_CLIENT_TIMEOUT_EVT:
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET: {
            esp_ble_mesh_cfg_client_get_state_t get_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET);
            get_state.comp_data_get.page = COMP_DATA_PAGE_0;
            err = esp_ble_mesh_config_client_get_state(&common, &get_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config Composition Data Get failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD: {
            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
            set_state.app_key_add.net_idx = prov_key.net_idx;
            set_state.app_key_add.app_idx = prov_key.app_idx;
            memcpy(set_state.app_key_add.app_key, prov_key.app_key, ESP_BLE_MESH_OCTET16_LEN);
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND: {
            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
            set_state.model_app_bind.element_addr = node->unicast;
            set_state.model_app_bind.model_app_idx = prov_key.app_idx;
            set_state.model_app_bind.model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV;
            set_state.model_app_bind.company_id = ESP_BLE_MESH_CID_NVAL;
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                return;
            }
            break;
        }
        default:
            break;
        }
        break;
    default:
        ESP_LOGE(TAG, "Not a config client status message event");
        break;
    }
}

void ble_mesh_register_config_client_callback(void)
{
	esp_ble_mesh_register_config_client_callback(ble_mesh_configuration_client_model_cb);
}
