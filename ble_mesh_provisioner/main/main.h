/*
 * main.h
 *
 *  Created on: 19.03.2021
 *      Author: mail
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#define LED_OFF             0x0
#define LED_ON              0x1

#define COMP_DATA_PAGE_0    0x00

typedef struct esp_ble_mesh_key {
    uint16_t net_idx;
    uint16_t app_idx;
    uint8_t  app_key[16];
} esp_ble_mesh_key_t;



esp_err_t example_ble_mesh_set_msg_common(esp_ble_mesh_client_common_param_t *common,
                                                 esp_ble_mesh_node_info_t *node,
                                                 esp_ble_mesh_model_t *model, uint32_t opcode);

#endif /* MAIN_MAIN_H_ */
