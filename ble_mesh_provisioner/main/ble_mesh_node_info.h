/*
 * ble_mesh_node_info.h
 *
 *  Created on: 19.03.2021
 *      Author: mail
 */

#ifndef MAIN_BLE_MESH_NODE_INFO_H_
#define MAIN_BLE_MESH_NODE_INFO_H_

typedef struct {
    uint8_t  uuid[16];
    uint16_t unicast;
    uint8_t  elem_num;
    uint8_t  onoff;
} esp_ble_mesh_node_info_t;


esp_err_t example_ble_mesh_store_node_info(const uint8_t uuid[16], uint16_t unicast,
                                                  uint8_t elem_num, uint8_t onoff_state);

esp_ble_mesh_node_info_t *example_ble_mesh_get_node_info(uint16_t unicast);

#endif /* MAIN_BLE_MESH_NODE_INFO_H_ */
