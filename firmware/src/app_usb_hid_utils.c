// Based on Microchip Harmony examples with the following license:
//
// Copyright (c) 2013-2015 released Microchip Technology Inc. All rights reserved.
//
// Microchip licenses to you the right to use, modify, copy and distribute
// Software only when embedded on a Microchip microcontroller or digital signal
// controller that is integrated into your product or third party product
// (pursuant to the sublicense terms in the accompanying license agreement).
//
// You should refer to the license agreement accompanying this Software for
// additional information regarding your rights and obligations.
//
// SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
// MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
// CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
// OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
// INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
// CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
// SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
// (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
//
// Modifications are:
//
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

#include "app_usb_hid_utils.h"

#include "app_usb_hid.h"

extern AppUSBHIDData* g_app_usb_hid_data;

USB_DEVICE_HID_EVENT_RESPONSE app_usb_device_hid_event_handler(
    USB_DEVICE_HID_INDEX iHID,
    USB_DEVICE_HID_EVENT event,
    void* event_data,
    uintptr_t user_data) {
  USB_DEVICE_HID_EVENT_DATA_REPORT_SENT* report_sent;
  USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED* report_received;
  switch (event) {
    case USB_DEVICE_HID_EVENT_REPORT_SENT:
      // The eventData parameter will be USB_DEVICE_HID_EVENT_REPORT_SENT
      // pointer type containing details about the report that was sent.
      report_sent = (USB_DEVICE_HID_EVENT_DATA_REPORT_SENT *)event_data;
      if (report_sent->handle == g_app_usb_hid_data->tx_transfer_handle) {
        // Transfer progressed.
        g_app_usb_hid_data->is_hid_data_transmitted = true;
      }
      break;

    case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:
      // The eventData parameter will be USB_DEVICE_HID_EVENT_REPORT_RECEIVED
      // pointer type containing details about the report that was received.
      report_received = (USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED *)event_data;
      if (report_received->handle == g_app_usb_hid_data->rx_transfer_handle ){
        // Transfer progressed.
        g_app_usb_hid_data->is_hid_data_received = true;
      }
      break;

    case USB_DEVICE_HID_EVENT_SET_IDLE:
      // For now we just accept this request as is. We acknowledge
      // this request using the USB_DEVICE_HID_ControlStatus()
      // function with a USB_DEVICE_CONTROL_STATUS_OK flag.
      USB_DEVICE_ControlStatus(g_app_usb_hid_data->us_handle,
                               USB_DEVICE_CONTROL_STATUS_OK);
      // Save Idle rate received from Host.
      g_app_usb_hid_data->idle_rate =
          ((USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)event_data)->duration;
      break;

    case USB_DEVICE_HID_EVENT_GET_IDLE:
      // Host is requesting for Idle rate. Now send the Idle rate .
      USB_DEVICE_ControlSend(g_app_usb_hid_data->us_handle,
                             &(g_app_usb_hid_data->idle_rate),
                             1);
      // On successfully receiving Idle rate, the Host would acknowledge back
      // with a Zero Length packet. The HID function driver returns an event
      // USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the application upon
      // receiving this Zero Length packet from Host.
      // USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates this
      // control transfer event is complete.
      break;
    default:
      break;
  }
  return USB_DEVICE_HID_EVENT_RESPONSE_NONE;
}

void app_usb_device_event_handler(USB_DEVICE_EVENT event,
                                  void* event_data,
                                  uintptr_t context) {
  switch (event) {
    case USB_DEVICE_EVENT_RESET:
    case USB_DEVICE_EVENT_DECONFIGURED:
      // Host has de configured the device or a bus reset has happened.
      // Device layer is going to de-initialize all function drivers.
      // Hence close handles to all function drivers (Only if they are
      // opened previously.
      SYS_CONSOLE_MESSAGE("APP USB: Device reset/de-configured\r\n");
      g_app_usb_hid_data->is_device_configured = false;
      g_app_usb_hid_data->state = APP_USB_HID_STATE_WAIT_FOR_CONFIGURATION;
      break;
    case USB_DEVICE_EVENT_CONFIGURED:
      SYS_CONSOLE_MESSAGE("APP USB: Configured event\r\n");
      // Set the flag indicating device is configured.
      g_app_usb_hid_data->is_device_configured = true;
      // Save the other details for later use.
      g_app_usb_hid_data->configuration_value =
          ((USB_DEVICE_EVENT_DATA_CONFIGURED*)event_data)->configurationValue;
      // Register application HID event handler.
      USB_DEVICE_HID_EventHandlerSet(USB_DEVICE_HID_INDEX_0,
                                     app_usb_device_hid_event_handler, 
                                     (uintptr_t)g_app_usb_hid_data);
      break;

    case USB_DEVICE_EVENT_SUSPENDED:
      break;

    case USB_DEVICE_EVENT_POWER_DETECTED:
      // VBUS was detected. We can attach the device.
      SYS_CONSOLE_MESSAGE("APP USB: Power detected\r\n");
      USB_DEVICE_Attach(g_app_usb_hid_data->us_handle);
      break;

    case USB_DEVICE_EVENT_POWER_REMOVED:
      // VBUS is not available.
      SYS_CONSOLE_MESSAGE("APP USB: Power removed\r\n");
      USB_DEVICE_Detach(g_app_usb_hid_data->us_handle);
      break;

    // These events are not used.
    case USB_DEVICE_EVENT_RESUMED:
    case USB_DEVICE_EVENT_ERROR:
    default:
      break;
  }
}
