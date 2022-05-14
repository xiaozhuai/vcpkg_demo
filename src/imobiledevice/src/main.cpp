/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>
#include <vector>

#include "libimobiledevice/libimobiledevice.h"
#include "libimobiledevice/lockdown.h"

#define TOOL_NAME "ideviceinfo"

int main() {
    idevice_info_t *devices = nullptr;
    int device_count = 0;
    idevice_get_device_list_extended(&devices, &device_count);
    for (int i = 0; i < device_count; i++) {
        if (devices[i]->conn_type != CONNECTION_USBMUXD) {
            continue;
        }
        idevice_error_t ret;
        lockdownd_error_t ld_ret;
        idevice_t device = nullptr;
        lockdownd_client_t client = nullptr;

        ret = idevice_new_with_options(&device, devices[i]->udid, IDEVICE_LOOKUP_USBMUX);
        if (ret != IDEVICE_E_SUCCESS) {
            printf("Error: Failed to connect to device %s, %d\n", devices[i]->udid, ret);
            continue;
        }

        ld_ret = lockdownd_client_new_with_handshake(device, &client, TOOL_NAME);
        if (ld_ret != LOCKDOWN_E_SUCCESS) {
            printf("Error: Failed to connect to lockdownd: %s (%d)\n", lockdownd_strerror(ld_ret), ld_ret);
            idevice_free(device);
            continue;
        }

        char *device_name = nullptr;
        lockdownd_get_device_name(client, &device_name);

        printf("- [%d]\n", i);
        printf("  UDID      : %s\n", devices[i]->udid);
        printf("  Conn Type : %s\n", devices[i]->conn_type == CONNECTION_USBMUXD ? "USB" : "Network");
        printf("  Name      : %s\n", device_name);

        free(device_name);
        lockdownd_client_free(client);
        idevice_free(device);
    }
    idevice_device_list_extended_free(devices);

    return 0;
}
