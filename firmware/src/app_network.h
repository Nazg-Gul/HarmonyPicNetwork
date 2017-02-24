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

#ifndef _APP_NETWORK_H
#define _APP_NETWORK_H

#include "tcpip/tcpip.h"

#include "driver/wifi/mrf24w/drv_wifi.h"
#include "driver/wifi/mrf24w/src/drv_wifi_config_data.h"
#include "driver/wifi/mrf24w/src/drv_wifi_iwpriv.h"

#include "system_definitions.h"

struct DRV_ETHPHY_OBJECT_BASE_TYPE;

typedef enum {
  // Wait for TCP/IP stack to finish initalization.
  APP_NETWORK_TCPIP_WAIT_INIT,

  // Configure WiFi module.
  APP_NETWORK_WIFI_CONFIG,

  // Configure TCP/IP modules (like DHCP) for all interfaces.
  APP_NETWORK_TCPIP_MODULES_ENABLE,

  // Perform TCP/IP transaction.
  APP_NETWORK_TCPIP_TRANSACT,

  // Error happened in the networking related area.
  APP_NETWORK_TCPIP_ERROR,
} AppNetworkState;

typedef struct {
  SYSTEM_OBJECTS* system_objects;

  AppNetworkState state;

  int16_t ip_wait;

  // WiFi-related fields.
  IPV4_ADDR wifi_default_ip;
  TCPIP_NET_HANDLE wifi_net_handle;
  DRV_WIFI_CONFIG_DATA wifi_config;
  DRV_WIFI_DEVICE_INFO wifi_device_info;
} AppNetworkData;

// Initialize networking-related application routines.
void APP_Network_Initialize(AppNetworkData* app_network_data,
                            SYSTEM_OBJECTS* system_objects);

// Perform all networking related tasks.
void APP_Network_Tasks(AppNetworkData* app_network_data);

// Reset LAN8720 Eth PHY when it's requested.
void APP_Network_PHY_Reset(const struct DRV_ETHPHY_OBJECT_BASE_TYPE* pBaseObj);

#endif  // _APP_NETWORK_H
