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

#include "app_network_utils.h"

#include "driver/wifi/mrf24w/src/drv_wifi_iwpriv.h"

void app_network_wifi_ipv6_multicast_filter_set(TCPIP_NET_HANDLE net) {
#ifdef TCPIP_STACK_USE_IPV6
  const uint8_t *mac_addr = TCPIP_STACK_NetAddressMac(net);
  int i;
  uint8_t link_local_solicited_multicast_mac_addr[6];
  uint8_t solicited_node_multicast_mac_addr[] = {0x33, 0x33, 0xff,
                                                 0x00, 0x00, 0x00};
  uint8_t all_nodes_multicast_mac_addr[] = {0x33, 0x33, 0x00, 0x00, 0x00, 0x01};

  link_local_solicited_multicast_mac_addr[0] = 0x33;
  link_local_solicited_multicast_mac_addr[1] = 0x33;
  link_local_solicited_multicast_mac_addr[2] = 0xff;

  for (i = 3; i < 6; i++) {
    link_local_solicited_multicast_mac_addr[i] = mac_addr[i];
  }

  IWPRIV_SET_PARAM wifi_set_param;
  wifi_set_param.multicast.addr = link_local_solicited_multicast_mac_addr;
  iwpriv_set(MULTICASTFILTER_SET, &wifi_set_param);
  wifi_set_param.multicast.addr = solicited_node_multicast_mac_addr;
  iwpriv_set(MULTICASTFILTER_SET, &wifi_set_param);
  wifi_set_param.multicast.addr = all_nodes_multicast_mac_addr;
  iwpriv_set(MULTICASTFILTER_SET, &wifi_set_param);
#endif
}

void app_network_wifi_powersave_config(bool enable) {
#if DRV_WIFI_DEFAULT_POWER_SAVE == WF_ENABLED
  IWPRIV_SET_PARAM wifi_set_param;
  wifi_set_param.powerSave.enabled = enable;
  iwpriv_set(POWERSAVE_SET, &wifi_set_param);
#endif
}

void app_network_wifi_DHCPS_sync(TCPIP_NET_HANDLE net) {
#ifdef TCPIP_STACK_USE_DHCP_SERVER
  bool updated;
  TCPIP_MAC_ADDR addr;
  IWPRIV_GET_PARAM wifi_get_param;

  wifi_get_param.clientInfo.addr = addr.v;
  iwpriv_get(CLIENTINFO_GET, &wifi_get_param);
  updated = wifi_get_param.clientInfo.updated;

  if (updated) {
    TCPIP_DHCPS_LeaseEntryRemove(net, (TCPIP_MAC_ADDR *)&addr);
  }
#endif
}

void app_network_tcpip_ifmodules_enable(TCPIP_NET_HANDLE net) {
  int net_index = TCPIP_STACK_NetIndexGet(net);
  const char *net_name = TCPIP_STACK_NetNameGet(net);
  TCPIP_DHCP_Enable(net);
  TCPIP_DNS_Enable(net, TCPIP_DNS_ENABLE_DEFAULT);
  if (IS_WIFI_INTERFACE(net_name)) {
    app_network_wifi_ipv6_multicast_filter_set(net);
  }
#ifdef TCPIP_STACK_USE_NBNS
  const char *netbios_name = TCPIP_STACK_NetBIOSName(net);
  SYS_CONSOLE_PRINT("Interface %s on host %s - NBNS enabled\r\n",
                    net_name,
                    netbios_name);
#endif
#if defined(TCPIP_STACK_USE_ZEROCONF_MDNS_SD)
  // TODO(sergey): Skip this or support actual HTTP server.
  // NOTE: Base name of the service Must not exceed 16 bytes long.
  char mdns_service_name[] = "MyWebServiceNameX ";
  // NOTE: The last digit will be incremented by interface.
  mdns_service_name[sizeof(mdns_service_name) - 2] = '1' + net_index;
  TCPIP_MDNS_ServiceRegister(net,
                             mdns_service_name,
                             "_http._tcp.local", 80,
                             ((const uint8_t *)"path=/index.htm"),
                             1,
                             NULL, NULL);
#endif
}

void app_network_tcpip_ifmodules_disable(TCPIP_NET_HANDLE net) {
  const char *net_name = TCPIP_STACK_NetNameGet(net);
  if (IS_WIFI_INTERFACE(net_name) && TCPIP_STACK_NetIsUp(net)) {
    app_network_wifi_powersave_config(false);
  }
  TCPIP_DHCP_Disable(net);
  TCPIP_DNS_Disable(net, true);
  TCPIP_MDNS_ServiceDeregister(net);
}

void app_network_tcpip_iface_down(TCPIP_NET_HANDLE net) {
  TCPIP_STACK_NetDown(net);
}

void app_network_tcpip_iface_up(TCPIP_NET_HANDLE net) {
  SYS_MODULE_OBJ tcpip_stack_object;
  TCPIP_STACK_INIT tcpip_init_data;
  const TCPIP_NETWORK_CONFIG *network_config;
  uint16_t net_index = TCPIP_STACK_NetIndexGet(net);
  tcpip_stack_object = TCPIP_STACK_Initialize(0, 0);
  TCPIP_STACK_InitializeDataGet(tcpip_stack_object, &tcpip_init_data);
  network_config = tcpip_init_data.pNetConf + net_index;
  TCPIP_STACK_NetUp(net, network_config);
}
