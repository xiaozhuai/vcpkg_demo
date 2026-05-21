/**
 * Copyright 2022 xiaozhuai
 */

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "libusb.h"

const char *usb_version_get_speed(int speed) {
    static const char *map[] = {
        // clang-format off
        "#0 UNKNOWN",
        "#1 LOW (1.5Mbps)",
        "#2 FULL (12Mbps)",
        "#3 HIGH (480Mbps)",
        "#4 SUPER (5000Mbps)",
        "#5 SUPER+ 10000Mbps",
        "#6 SUPER+ 20000Mbps",
        // clang-format on
    };
    if (speed < 0 || speed >= std::size(map)) {
        speed = 0;
    }
    return map[speed];
}

const char *usb_version_get_official_name(int speed) {
    static const char *map[] = {
        // clang-format off
        "UNKNOWN",
        "USB 1.0",
        "USB 1.1",
        "USB 2.0",
        "USB 3.2 Gen 1",
        "USB 3.2 Gen 2",
        "USB 3.2 Gen 2x2"
        // clang-format on
    };
    if (speed < 0 || speed >= std::size(map)) {
        speed = 0;
    }
    return map[speed];
}

const char *usb_version_get_marketing_name(int speed) {
    static const char *map[] = {
        // clang-format off
        "UNKNOWN",
        "USB 1.0",
        "USB 1.1",
        "USB 2.0",
        "USB 3.0",
        "USB 3.1",
        "USB 3.2",
        // clang-format on
    };
    if (speed < 0 || speed >= std::size(map)) {
        speed = 0;
    }
    return map[speed];
}

struct StringDescriptor {
    int index;
    std::string description;
};

struct DeviceDescription {
    uint8_t bus_number;
    uint8_t device_address;
    uint16_t vendor_id;
    uint16_t product_id;
    int speed;
    std::vector<StringDescriptor> string_descriptors;
};

std::vector<StringDescriptor> get_device_string_descriptors(libusb_device *device) {
    libusb_device_handle *device_handle = nullptr;
    int ret = libusb_open(device, &device_handle);
    if (ret != LIBUSB_SUCCESS) {
        // printf("libusb_open() failed: %s\n", libusb_error_name(ret));
        return {};
    }
    std::unique_ptr<libusb_device_handle, decltype(&libusb_close)>  //
        auto_close_device_handle(device_handle, libusb_close);
    std::vector<StringDescriptor> string_descriptors;
    if (device_handle) {
        for (int desc_index = 1; desc_index < 256; desc_index++) {
            unsigned char buffer[1024] = {0};
            ret = libusb_get_string_descriptor_ascii(device_handle, desc_index, buffer, sizeof(buffer));
            if (ret < 0) {
                break;
            }
            string_descriptors.emplace_back(StringDescriptor{desc_index, reinterpret_cast<char *>(buffer)});
        }
    }
    return string_descriptors;
}

DeviceDescription get_device_description(libusb_device *device) {
    auto bus_number = libusb_get_bus_number(device);
    auto device_address = libusb_get_device_address(device);
    libusb_device_descriptor desc{};
    libusb_get_device_descriptor(device, &desc);
    auto vendor_id = desc.idVendor;
    auto product_id = desc.idProduct;
    auto speed = libusb_get_device_speed(device);
    auto string_descriptors = get_device_string_descriptors(device);
    return {bus_number, device_address, vendor_id, product_id, speed, string_descriptors};
}

void print_device_description(const DeviceDescription &device) {
    const char *usb_version_speed = usb_version_get_speed(device.speed);
    const char *usb_version_official_name = usb_version_get_official_name(device.speed);
    const char *usb_version_marketing_name = usb_version_get_marketing_name(device.speed);
    printf(
        "- Device:\n"
        "  - Bus: 0x%02X\n"
        "  - Address: 0x%02X\n"
        "  - VID: 0x%04X\n"
        "  - PID: 0x%04X\n"
        "  - USB Version:\n"
        "    - Speed: %s\n"
        "    - Official: %s\n"
        "    - Marketing: %s\n",
        device.bus_number, device.device_address, device.vendor_id, device.product_id,  //
        usb_version_speed, usb_version_official_name, usb_version_marketing_name);
    if (!device.string_descriptors.empty()) {
        printf("  - String Descriptors:\n");
        for (const auto &descriptor : device.string_descriptors) {
            printf("    - [%d] %s\n", descriptor.index, descriptor.description.c_str());
        }
    }
}

void free_device_list(libusb_device **list) { libusb_free_device_list(list, 1); }

std::vector<DeviceDescription> list_usb_devices() {
    libusb_context *context = nullptr;
    int ret = libusb_init(&context);
    if (ret != LIBUSB_SUCCESS) {
        printf("libusb_init() failed: %s\n", libusb_error_name(ret));
        return {};
    }
    std::unique_ptr<libusb_context, decltype(&libusb_exit)> auto_exit_context(context, libusb_exit);

    libusb_device **devices = nullptr;
    ssize_t device_count = libusb_get_device_list(context, &devices);
    if (device_count < 0) {
        printf("libusb_get_device_list() failed\n");
        return {};
    }
    std::unique_ptr<libusb_device *, decltype(&free_device_list)> auto_free_device_list(devices, free_device_list);

    std::vector<DeviceDescription> device_list;
    for (int device_index = 0; device_index < device_count; device_index++) {
        libusb_device *device = devices[device_index];
        device_list.emplace_back(get_device_description(device));
    }
    return device_list;
}

void compare_devices(const std::vector<DeviceDescription> &devices_old,  //
                     const std::vector<DeviceDescription> &devices_new,  //
                     std::vector<DeviceDescription> &devices_plug,       //
                     std::vector<DeviceDescription> &devices_unplug) {
    auto compare_device = [](const DeviceDescription &a, const DeviceDescription &b) {
        return a.vendor_id == b.vendor_id       //
               && a.product_id == b.product_id  //
               && a.bus_number == b.bus_number  //
               && a.device_address == b.device_address;
    };

    for (const auto &device_new : devices_new) {
        bool found = false;
        for (const auto &device_old : devices_old) {
            if (compare_device(device_old, device_new)) {
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
            if (compare_device(device_old, device_new)) {
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
    for (const auto &device : devices) {
        print_device_description(device);
    }

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto devices_new = list_usb_devices();
        std::vector<DeviceDescription> devices_plug;
        std::vector<DeviceDescription> devices_unplug;
        compare_devices(devices, devices_new, devices_plug, devices_unplug);
        devices_new.swap(devices);
        if (!devices_plug.empty()) {
            printf("----------------------------------\n");
            printf("Devices Plug:\n");
            for (const auto &device : devices_plug) {
                print_device_description(device);
            }
        }
        if (!devices_unplug.empty()) {
            printf("----------------------------------\n");
            printf("Devices Unplug:\n");
            for (const auto &device : devices_unplug) {
                print_device_description(device);
            }
        }
    }

    printf("\nSIGINT received, exit.\n");
    return 0;
}
