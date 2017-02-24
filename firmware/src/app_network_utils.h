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

#ifndef _APP_NETWORK_UTILS_H
#define _APP_NETWORK_UTILS_H

#include "system_definitions.h"

#define WIFI_INTERFACE_NAME "MRF24W"
#define WIFI_RECONNECTION_RETRY_LIMIT 16
#define WIFI_DHCP_WAIT_THRESHOLD 60 /* seconds */
#define IS_WIFI_INTERFACE(net_name) (strcmp(net_name, WIFI_INTERFACE_NAME) == 0)
#define timestamp_dhcp_kickin(x)             \
  do {                                       \
    x = (TCPIP_DHCP_CLIENT_ENABLED ? 1 : 0); \
  } while (0)

void app_network_wifi_ipv6_multicast_filter_set(TCPIP_NET_HANDLE net);
void app_network_wifi_powersave_config(bool enable);
void app_network_wifi_DHCPS_sync(TCPIP_NET_HANDLE net);

void app_network_tcpip_ifmodules_enable(TCPIP_NET_HANDLE net);
void app_network_tcpip_ifmodules_disable(TCPIP_NET_HANDLE net);
void app_network_tcpip_iface_down(TCPIP_NET_HANDLE net);
void app_network_tcpip_iface_up(TCPIP_NET_HANDLE net);

#endif  // _APP_NETWORK_UTILS_H
