// Copyright (c) 2017, Sergey Sharybin
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
// Author: Sergey Sharybin (sergey.vfx@gmail.com)

#ifndef _APP_USB_HID_H
#define _APP_USB_HID_H

#include "system_definitions.h"

typedef enum {
  // USB HID is initializing.
  APP_USB_HID_STATE_INIT,
  // USB HID is waiting for configuration.
  APP_USB_HID_STATE_WAIT_FOR_CONFIGURATION,
  // USB HID is running the main tasks.
  APP_USB_HID_STATE_MAIN_TASK,
  // USB HID system encountered an error.
  APP_USB_HID_STATE_ERROR,
} AppUSBHIDState;


typedef struct {
  AppUSBHIDState state;

  USB_DEVICE_HANDLE  us_handle;

  uint8_t* receive_data_buffer;
  uint8_t* transmit_data_buffer;

  bool is_device_configured;

  USB_DEVICE_HID_TRANSFER_HANDLE tx_transfer_handle;
  USB_DEVICE_HID_TRANSFER_HANDLE rx_transfer_handle;

  uint8_t configuration_value;

  bool is_hid_data_received;
  bool is_hid_data_transmitted;

  uint8_t idle_rate;
} AppUSBHIDData;


void APP_USB_HID_Initialize(AppUSBHIDData* app_usb_hid_data);
void APP_USB_HID_Tasks(AppUSBHIDData* app_usb_hid_data);

#endif  // _APP_USB_HID_H
