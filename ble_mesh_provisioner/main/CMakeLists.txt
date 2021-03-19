set(srcs "main.c"
		"ble_mesh_node_info.c"
        "ble_mesh_reg_cfg_client_cmd.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  ".")
