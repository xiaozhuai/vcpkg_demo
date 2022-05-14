/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>
#include <vector>

#include "libusb.h"

std::vector<const char *> SPEED_ENUM_MAP = {
    // clang-format off
    "0 UNKNOWN",
    "1 LOW (1.5Mbps)",
    "2 FULL (12Mbps)",
    "3 HIGH (480Mbps)",
    "4 SUPER (5000Mbps)",
    "5 SUPER+ (10000Mbps)",
    // clang-format on
};

int main() {
    libusb_device **devices;
    libusb_context *context = nullptr;
    libusb_init(&context);
    ssize_t nDevice = libusb_get_device_list(context, &devices);
    printf("Found %zd usb devices:\n", nDevice);
    for (int i = 0; i < nDevice; i++) {
        libusb_device *device = devices[i];
        libusb_device_descriptor desc{};
        libusb_get_device_descriptor(device, &desc);
        int speed = libusb_get_device_speed(device);
        const char *speedDesc = SPEED_ENUM_MAP[speed];
        if (desc.idVendor == 0x18D1 && desc.idProduct == 0x4EE2) {
            printf(
                "[Android] Device[%d] "
                "VendorID: 0x%04X, "
                "ProductID: 0x%04X, "
                "Speed: %s\n",
                i, desc.idVendor, desc.idProduct, speedDesc);
        } else if (desc.idVendor == 0x05AC && desc.bDeviceClass == 0x0) {
            printf(
                "[iPhone]  Device[%d] "
                "VendorID: 0x%04X, "
                "ProductID: 0x%04X, "
                "Speed: %s\n",
                i, desc.idVendor, desc.idProduct, speedDesc);
        } else {
            printf(
                "[Unknown] Device[%d] "
                "VendorID: 0x%04X, "
                "ProductID: 0x%04X, "
                "Speed: %s\n",
                i, desc.idVendor, desc.idProduct, speedDesc);
        }
    }
    libusb_free_device_list(devices, 1);
    libusb_exit(context);
    return 0;
}
