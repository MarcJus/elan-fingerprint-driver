//
// Created by marc on 03/04/23.
//

#ifndef FINGERPRINT_READER_FINGERPRINT_USB_REQUEST_H
#define FINGERPRINT_READER_FINGERPRINT_USB_REQUEST_H

#include <linux/usb.h>

// Endpoint 1
static int ep_1_open_fingerprint(struct usb_device *device);
static int ep_1_close_fingerprint(struct usb_device *device);
static int ep_1_ask_for_fingerprint(struct usb_device *device);

// Endpoint 3
static int ep_3_request_fingerprint(struct usb_device *device, u8 *buffer);

// Endpoint 4
static int ep_4_wait_for_fingerprint(struct usb_device *device, u8 *buffer);

#endif //FINGERPRINT_READER_FINGERPRINT_USB_REQUEST_H
