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

#include "app_usb_hid.h"

#include "app.h"
#include "app_usb_hid_utils.h"

#define BUFFER_DMA_READY

AppUSBHIDData* g_app_usb_hid_data;

static uint8_t receiveDataBuffer[64] BUFFER_DMA_READY;
static uint8_t transmitDataBuffer[64] BUFFER_DMA_READY;

void APP_USB_HID_Initialize(AppUSBHIDData* app_usb_hid_data) {
  app_usb_hid_data->state = APP_USB_HID_STATE_INIT;

  app_usb_hid_data->us_handle = USB_DEVICE_HANDLE_INVALID;
  app_usb_hid_data->is_device_configured = false;
  app_usb_hid_data->tx_transfer_handle = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
  app_usb_hid_data->rx_transfer_handle = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
  app_usb_hid_data->is_hid_data_received = false;
  app_usb_hid_data->is_hid_data_transmitted = true;
  app_usb_hid_data->receive_data_buffer = &receiveDataBuffer[0];
  app_usb_hid_data->transmit_data_buffer = &transmitDataBuffer[0];

  g_app_usb_hid_data = app_usb_hid_data;
}

void APP_USB_HID_Tasks(AppUSBHIDData* app_usb_hid_data) {
  switch (app_usb_hid_data->state) {
    case APP_USB_HID_STATE_INIT:
      // Open the device layer.
      app_usb_hid_data->us_handle =
          USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);
      if (app_usb_hid_data->us_handle != USB_DEVICE_HANDLE_INVALID) {
        SYS_CONSOLE_MESSAGE("APP USB: USB device opened\r\n");
        // Register a callback with device layer to get event notification
        // (for end point 0).
        USB_DEVICE_EventHandlerSet(app_usb_hid_data->us_handle,
                                   app_usb_device_event_handler,
                                   0);
        app_usb_hid_data->state = APP_USB_HID_STATE_WAIT_FOR_CONFIGURATION;
      } else {
        // The Device Layer is not ready to be opened.
        // We should try again later
      }
      break;
    case APP_USB_HID_STATE_WAIT_FOR_CONFIGURATION:
      if (app_usb_hid_data->is_device_configured == true) {
        SYS_CONSOLE_MESSAGE("APP USB: USB device configured\r\n");
        // Device is ready to run the main task.
        app_usb_hid_data->is_hid_data_received = false;
        app_usb_hid_data->is_hid_data_transmitted = true;
        app_usb_hid_data->state = APP_USB_HID_STATE_MAIN_TASK;
        // Place a new read request.
        USB_DEVICE_HID_ReportReceive(USB_DEVICE_HID_INDEX_0,
                                     &app_usb_hid_data->rx_transfer_handle,
                                     app_usb_hid_data->receive_data_buffer,
                                     64);
      }
      break;
    case APP_USB_HID_STATE_MAIN_TASK:
      if (!app_usb_hid_data->is_device_configured) {
        SYS_CONSOLE_MESSAGE("APP USB: Waiting for configuration\r\n");
        app_usb_hid_data->state = APP_USB_HID_STATE_WAIT_FOR_CONFIGURATION;
      } else if (app_usb_hid_data->is_hid_data_received) {
        SYS_CONSOLE_MESSAGE("APP USB: Got received data\r\n");
        app_usb_hid_data->is_hid_data_received = false;
        // Place a new read request.
        USB_DEVICE_HID_ReportReceive(USB_DEVICE_HID_INDEX_0,
                                     &app_usb_hid_data->rx_transfer_handle,
                                     app_usb_hid_data->receive_data_buffer,
                                     64);
      }
      break;
    case APP_USB_HID_STATE_ERROR:
      break;
  }
}
