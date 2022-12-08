#include <libusb.h>
#include <cstdio>

int main() {
    libusb_device **devices;
    libusb_context *context = nullptr;
    libusb_init(&context);
    ssize_t nDevice = libusb_get_device_list(context, &devices);
    printf("Found %ld usb devices:\n", nDevice);
    for (int i = 0; i < nDevice; i++) {
        libusb_device *device = devices[i];
        libusb_device_descriptor desc{};
        libusb_get_device_descriptor(device, &desc);
        if (desc.idVendor == 0x18D1 && desc.idProduct == 0x4EE2) {
            printf("[Android] Device[%d] VendorID: 0x%04X, ProductID: 0x%04X\n", i, desc.idVendor, desc.idProduct);
        } else if (desc.idVendor == 0x05AC && desc.bDeviceClass == 0x0) {
            printf("[iPhone]  Device[%d] VendorID: 0x%04X, ProductID: 0x%04X\n", i, desc.idVendor, desc.idProduct);
        } else {
            printf("[Unknown] Device[%d] VendorID: 0x%04X, ProductID: 0x%04X\n", i, desc.idVendor, desc.idProduct);
        }
    }
    libusb_free_device_list(devices, 1);
    libusb_exit(context);
    return 0;
}
