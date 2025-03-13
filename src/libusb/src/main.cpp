/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>
#include <thread>
#include <vector>

#include "libusb.h"

std::vector<const char *> SPEED_ENUM_MAP = {
    // clang-format off
    "0 UNKNOWN",
    "1 LOW (1.5Mbps) USB 1.0",
    "2 FULL (12Mbps) USB 1.1",
    "3 HIGH (480Mbps) USB 2.0",
    "4 SUPER (5000Mbps) USB 3.0",
    "5 SUPER+ (10000Mbps) USB 3.1",
    // clang-format on
};

struct DeviceDesc {
    uint8_t bus_number;
    uint8_t device_address;
    uint16_t vendor_id;
    uint16_t product_id;
    int speed;
    std::vector<std::string> descriptors;

    bool is_android() const {
        return std::any_of(descriptors.begin(), descriptors.end(), [](const std::string &desc) {
            return desc.find("ADB Interface") != std::string::npos || desc.find("Android") != std::string::npos;
        });
    }

    bool is_apple() const {
        return std::any_of(descriptors.begin(), descriptors.end(), [](const std::string &desc) {
            return desc.find("Apple Mobile Device") != std::string::npos;
        });
    }

    bool is_mobile() const { return is_android() || is_apple(); }
};

std::vector<DeviceDesc> list_usb_devices() {
    std::vector<DeviceDesc> device_list;
    libusb_context *context = nullptr;
    libusb_init(&context);
    libusb_device **devices;
    ssize_t device_count = libusb_get_device_list(context, &devices);
    for (int device_index = 0; device_index < device_count; device_index++) {
        libusb_device *device = devices[device_index];
        uint8_t bus_number = libusb_get_bus_number(device);
        uint8_t device_address = libusb_get_device_address(device);
        libusb_device_descriptor desc{};
        libusb_get_device_descriptor(device, &desc);
        int speed = libusb_get_device_speed(device);

        std::vector<std::string> descriptors;
        libusb_device_handle *device_handle = libusb_open_device_with_vid_pid(context, desc.idVendor, desc.idProduct);
        if (device_handle) {
            for (int desc_index = 1; desc_index < 256; desc_index++) {
                unsigned char buffer[1024] = {0};
                int ret = libusb_get_string_descriptor_ascii(device_handle, desc_index, buffer, sizeof(buffer));
                if (ret <= 0) {
                    break;
                }
                descriptors.emplace_back((char *)buffer);
            }
            libusb_close(device_handle);
        }

        device_list.emplace_back(
            DeviceDesc{bus_number, device_address, desc.idVendor, desc.idProduct, speed, descriptors});
    }
    libusb_free_device_list(devices, 1);
    libusb_exit(context);
    return device_list;
}

void show_devices(const std::vector<DeviceDesc> &devices) {
    for (int device_index = 0; device_index < devices.size(); device_index++) {
        const auto &device = devices[device_index];
        const char *speed_desc = SPEED_ENUM_MAP[device.speed];
        printf(
            "- Device[%d]\n"
            "  - Bus: 0x%02X\n"
            "  - Address: 0x%02X\n"
            "  - VID: 0x%04X\n"
            "  - PID: 0x%04X\n"
            "  - Speed: %s\n"
            "  - Mobile: %s\n",
            device_index, device.bus_number, device.device_address, device.vendor_id, device.product_id, speed_desc,
            !device.is_mobile()   ? "No"
            : device.is_android() ? "Android"
                                  : "Apple");
        printf("  - Descriptors:\n");
        for (int desc_index = 0; desc_index < device.descriptors.size(); desc_index++) {
            printf("    - [%d] %s\n", desc_index, device.descriptors[desc_index].c_str());
        }
    }
}

void compare_devices(const std::vector<DeviceDesc> &devices_old, const std::vector<DeviceDesc> &devices_new,
                     std::vector<DeviceDesc> &devices_plug, std::vector<DeviceDesc> &devices_unplug) {
    for (const auto &device_new : devices_new) {
        bool found = false;
        for (const auto &device_old : devices_old) {
            if (device_old.vendor_id == device_new.vendor_id && device_old.product_id == device_new.product_id &&
                device_old.bus_number == device_new.bus_number &&
                device_old.device_address == device_new.device_address) {
                found = true;
                break;
            }
        }
        if (!found) {
            devices_plug.emplace_back(device_new);
        }
    }
    for (const auto &device_old : devices_old) {
        bool found = false;
        for (const auto &device_new : devices_new) {
            if (device_old.vendor_id == device_new.vendor_id && device_old.product_id == device_new.product_id &&
                device_old.bus_number == device_new.bus_number &&
                device_old.device_address == device_new.device_address) {
                found = true;
                break;
            }
        }
        if (!found) {
            devices_unplug.emplace_back(device_old);
        }
    }
}

int main() {
    auto devices = list_usb_devices();
    printf("----------------------------------\n");
    printf("All Devices:\n");
    show_devices(devices);

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto devices_new = list_usb_devices();
        std::vector<DeviceDesc> devices_plug;
        std::vector<DeviceDesc> devices_unplug;
        compare_devices(devices, devices_new, devices_plug, devices_unplug);
        devices_new.swap(devices);
        if (!devices_plug.empty()) {
            printf("----------------------------------\n");
            printf("Devices Plug:\n");
            show_devices(devices_plug);
        }
        if (!devices_unplug.empty()) {
            printf("----------------------------------\n");
            printf("Devices Unplug:\n");
            show_devices(devices_unplug);
        }
    }

    return 0;
}
