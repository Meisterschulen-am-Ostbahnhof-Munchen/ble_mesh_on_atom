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


static const char *TAG = "ble_mesh_node_info";


static esp_ble_mesh_node_info_t nodes[CONFIG_BLE_MESH_MAX_PROV_NODES] = {
    [0 ... (CONFIG_BLE_MESH_MAX_PROV_NODES - 1)] = {
        .unicast = ESP_BLE_MESH_ADDR_UNASSIGNED,
        .elem_num = 0,
        .onoff = LED_OFF,
    }
};


esp_err_t example_ble_mesh_store_node_info(const uint8_t uuid[16], uint16_t unicast,
                                                  uint8_t elem_num, uint8_t onoff_state)
{
    int i;

    if (!uuid || !ESP_BLE_MESH_ADDR_IS_UNICAST(unicast)) {
        return ESP_ERR_INVALID_ARG;
    }

    /* Judge if the device has been provisioned before */
    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (!memcmp(nodes[i].uuid, uuid, 16)) {
            ESP_LOGW(TAG, "%s: reprovisioned device 0x%04x", __func__, unicast);
            nodes[i].unicast = unicast;
            nodes[i].elem_num = elem_num;
            nodes[i].onoff = onoff_state;
            return ESP_OK;
        }
    }

    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (nodes[i].unicast == ESP_BLE_MESH_ADDR_UNASSIGNED) {
            memcpy(nodes[i].uuid, uuid, 16);
            nodes[i].unicast = unicast;
            nodes[i].elem_num = elem_num;
            nodes[i].onoff = onoff_state;
            return ESP_OK;
        }
    }

    return ESP_FAIL;
}

esp_ble_mesh_node_info_t *example_ble_mesh_get_node_info(uint16_t unicast)
{
    int i;

    if (!ESP_BLE_MESH_ADDR_IS_UNICAST(unicast)) {
        return NULL;
    }

    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (nodes[i].unicast <= unicast &&
                nodes[i].unicast + nodes[i].elem_num > unicast) {
            return &nodes[i];
        }
    }

    return NULL;
}
