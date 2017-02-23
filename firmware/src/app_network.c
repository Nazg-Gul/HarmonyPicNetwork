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

#include "app_network.h"

#include "app.h"
#include "system_definitions.h"

#define WIFI_INTERFACE_NAME "MRF24W"
#define WIFI_RECONNECTION_RETRY_LIMIT 16
#define WIFI_DHCP_WAIT_THRESHOLD 60 /* seconds */
#define IS_WIFI_INTERFACE(net_name) (strcmp(net_name, WIFI_INTERFACE_NAME) == 0)
#define timestamp_dhcp_kickin(x)             \
  do {                                       \
    x = (TCPIP_DHCP_CLIENT_ENABLED ? 1 : 0); \
  } while (0)

static bool app_network_tcpip_init_wait(AppNetworkData* app_network_data) {
  SYS_STATUS tcpip_status =
      TCPIP_STACK_Status(app_network_data->system_objects->tcpip);
  if (tcpip_status < 0) {
    SYS_CONSOLE_MESSAGE("TCP/IP stack initialization failed.\r\n");
    app_network_data->state = APP_NETWORK_TCPIP_ERROR;
    return true;
  } else if (tcpip_status == SYS_STATUS_READY) {
    SYS_CONSOLE_MESSAGE("TCP/IP stack initialization succeeded.\r\n");
    app_network_data->state = APP_NETWORK_WIFI_CONFIG;
    return true;
  }
  return false;
}

static bool app_network_wifi_config(AppNetworkData* app_network_data) {
  // THe following condition is required in case Wi-Fi interface is reset
  // due to connection error.
  iwpriv_get(DRVSTATUS_GET, &app_network_data->wifi_get_param);
  if (app_network_data->wifi_get_param.driverStatus.isOpen) {
    SYS_CONSOLE_MESSAGE("APP: WiFi driver is open\r\n");
    app_network_data->wifi_get_param.devInfo.data =
        &app_network_data->wifi_device_info;
    iwpriv_get(DEVICEINFO_GET, &app_network_data->wifi_get_param);
    app_network_data->wifi_net_handle =
        TCPIP_STACK_NetHandleGet(WIFI_INTERFACE_NAME);
    app_network_data->wifi_default_ip.Val =
        TCPIP_STACK_NetAddress(app_network_data->wifi_net_handle);
    app_network_data->state = APP_NETWORK_TCPIP_MODULES_ENABLE;
    return true;
  }
  return false;
}

static void app_network_tcpip_ifmodules_enable(TCPIP_NET_HANDLE net) {
  int net_index = TCPIP_STACK_NetIndexGet(net);
  const char *net_name = TCPIP_STACK_NetNameGet(net);
  TCPIP_DHCP_Enable(net);
  TCPIP_DNS_Enable(net, TCPIP_DNS_ENABLE_DEFAULT);
  if (IS_WIFI_INTERFACE(net_name)) {
    // APP_WIFI_IPv6MulticastFilter_Set(net);
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

static void app_network_wifi_powersave_config(AppNetworkData* app_network_data,
                                              bool enable) {
#if DRV_WIFI_DEFAULT_POWER_SAVE == WF_ENABLED
    app_network_data->wifi_set_param.powerSave.enabled = enable;
    iwpriv_set(POWERSAVE_SET, &app_network_data->wifi_set_param);
#endif
}

static void app_network_tcpip_ifmodules_disable(
    AppNetworkData* app_network_data,
    TCPIP_NET_HANDLE net) {
  const char *net_name = TCPIP_STACK_NetNameGet(net);
  if (IS_WIFI_INTERFACE(net_name) && TCPIP_STACK_NetIsUp(net)) {
    app_network_wifi_powersave_config(app_network_data, false);
  }
  TCPIP_DHCP_Disable(net);
  TCPIP_DNS_Disable(net, true);
  TCPIP_MDNS_ServiceDeregister(net);
}

static void app_network_tcpip_iface_down(TCPIP_NET_HANDLE net) {
  TCPIP_STACK_NetDown(net);
}

static void app_network_tcpip_iface_up(TCPIP_NET_HANDLE net) {
  SYS_MODULE_OBJ tcpip_stack_object;
  TCPIP_STACK_INIT tcpip_init_data;
  const TCPIP_NETWORK_CONFIG *network_config;
  uint16_t net_index = TCPIP_STACK_NetIndexGet(net);
  tcpip_stack_object = TCPIP_STACK_Initialize(0, 0);
  TCPIP_STACK_InitializeDataGet(tcpip_stack_object, &tcpip_init_data);
  network_config = tcpip_init_data.pNetConf + net_index;
  TCPIP_STACK_NetUp(net, network_config);
}

static void app_network_wifi_DHCPS_sync(AppNetworkData* app_network_data,
                                        TCPIP_NET_HANDLE net) {
#ifdef TCPIP_STACK_USE_DHCP_SERVER
  bool updated;
  TCPIP_MAC_ADDR addr;

  app_network_data->wifi_get_param.clientInfo.addr = addr.v;
  iwpriv_get(CLIENTINFO_GET, &app_network_data->wifi_get_param);
  updated = app_network_data->wifi_get_param.clientInfo.updated;

  if (updated) {
    TCPIP_DHCPS_LeaseEntryRemove(net, (TCPIP_MAC_ADDR *)&addr);
  }
#endif
}


static void app_network_tcpip_module_enable(AppNetworkData* app_network_data) {
  int i, num_networks = TCPIP_STACK_NumberOfNetworksGet();
  SYS_CONSOLE_PRINT("APP: Enabling %d modules\r\n", num_networks);
  for (i = 0; i < num_networks; ++i) {
    app_network_tcpip_ifmodules_enable(TCPIP_STACK_IndexToNet(i));
  }
  app_network_data->state = APP_NETWORK_TCPIP_TRANSACT;
}

static void app_network_run(AppNetworkData* app_network_data) {
  int i, num_nets;
  static bool isWiFiPowerSaveConfigured = false;
  static bool wasNetUp[2] = {true, true}; // this app supports 2 interfaces so far
  static uint32_t reconn_retries = 0;
  static uint32_t startTick = 0;
  static IPV4_ADDR dwLastIP[2] = { {-1}, {-1} }; // this app supports 2 interfaces so far

  iwpriv_get(CONNSTATUS_GET, &app_network_data->wifi_get_param);
  switch (app_network_data->wifi_get_param.conn.status) {
    case IWPRIV_CONNECTION_SUCCESSFUL:
      // Resetting reconnection retries.
      reconn_retries = 0;
      break;
    case IWPRIV_CONNECTION_FAILED:
      if (reconn_retries++ < WIFI_RECONNECTION_RETRY_LIMIT) {
        SYS_CONSOLE_PRINT("\r\nCouldn't connect to target AP, resetting Wi-Fi module and trying to reconnect, retries left: %u\r\n\n",
                          WIFI_RECONNECTION_RETRY_LIMIT - reconn_retries);
        app_network_tcpip_ifmodules_disable(app_network_data,
                                            app_network_data->wifi_net_handle);
        app_network_tcpip_iface_down(app_network_data->wifi_net_handle);
        app_network_tcpip_iface_up(app_network_data->wifi_net_handle);
        isWiFiPowerSaveConfigured = false;
        app_network_data->state = APP_NETWORK_WIFI_CONFIG;
        return;
      }
      break;
    case IWPRIV_CONNECTION_REESTABLISHED:
      // Restart DHCP client and config power save.
      TCPIP_DHCP_Disable(app_network_data->wifi_net_handle);
      TCPIP_DHCP_Enable(app_network_data->wifi_net_handle);
      isWiFiPowerSaveConfigured = false;
      timestamp_dhcp_kickin(app_network_data->ip_wait);
      break;
    default:
      break;
  }
  // Following for loop is to deal with manually controlling interface down/up
  // (for example, through console commands or web page).
  num_nets = TCPIP_STACK_NumberOfNetworksGet();
  for (i = 0; i < num_nets; ++i) {
    TCPIP_NET_HANDLE net = TCPIP_STACK_IndexToNet(i);
    if (!TCPIP_STACK_NetIsUp(net) && wasNetUp[i]) {
      const char *net_name = TCPIP_STACK_NetNameGet(net);
      wasNetUp[i] = false;
      app_network_tcpip_ifmodules_disable(app_network_data, net);
      if (IS_WIFI_INTERFACE(net_name)) {
        isWiFiPowerSaveConfigured = false;
      }
    }
    if (TCPIP_STACK_NetIsUp(net) && !wasNetUp[i]) {
      wasNetUp[i] = true;
      app_network_tcpip_ifmodules_enable(net);
    }
  }

  // If we get a new IP address that is different than the default one,
  // we will run PowerSave configuration.
  if (!isWiFiPowerSaveConfigured &&
    TCPIP_STACK_NetIsUp(app_network_data->wifi_net_handle) &&
    (TCPIP_STACK_NetAddress(app_network_data->wifi_net_handle) != app_network_data->wifi_default_ip.Val)) {
    app_network_wifi_powersave_config(app_network_data, true);
    isWiFiPowerSaveConfigured = true;
  }

  app_network_wifi_DHCPS_sync(app_network_data,
                              app_network_data->wifi_net_handle);

  // If the IP address of an interface has changed, 
  // display the new value on console.
  for (i = 0; i < num_nets; ++i) {
    IPV4_ADDR ipAddr;
    TCPIP_NET_HANDLE netH = TCPIP_STACK_IndexToNet(i);
    ipAddr.Val = TCPIP_STACK_NetAddress(netH);
    if (dwLastIP[i].Val != ipAddr.Val) {
      dwLastIP[i].Val = ipAddr.Val;
      if (ipAddr.Val != 0) {
        SYS_CONSOLE_PRINT("%s IPv4 Address: %d.%d.%d.%d \r\n",
                          TCPIP_STACK_NetNameGet(netH),
                          ipAddr.v[0], ipAddr.v[1], ipAddr.v[2], ipAddr.v[3]);
        app_network_data->ip_wait = 0;
      }
    }
  }

  if (SYS_TMR_TickCountGet() - startTick >= SYS_TMR_TickCounterFrequencyGet() / 2ul) {
    if (app_network_data->ip_wait && ++app_network_data->ip_wait > WIFI_DHCP_WAIT_THRESHOLD) {
      app_network_data->ip_wait = 0;
      if (app_network_data->wifi_get_param.conn.status == IWPRIV_CONNECTION_SUCCESSFUL)
        SYS_CONSOLE_MESSAGE("\r\nFailed to obtain an IP address from DHCP server\r\n \
          If WEP security is used, double-check if the key is valid\r\n");
    }
    startTick = SYS_TMR_TickCountGet();
  }
}

void APP_Network_Initialize(AppNetworkData* app_network_data,
                            SYSTEM_OBJECTS* system_objects) {
  app_network_data->system_objects = system_objects;

  app_network_data->state = APP_NETWORK_TCPIP_WAIT_INIT;
  app_network_data->ip_wait = 0;
  // Initialize WiFi networking.
  app_network_data->wifi_default_ip.Val = 0;
  app_network_data->wifi_net_handle = NULL;
  app_network_data->wifi_set_param.conn.initConnAllowed = true;
  iwpriv_set(INITCONN_OPTION_SET, &app_network_data->wifi_set_param);
}

void APP_Network_Tasks(AppNetworkData* app_network_data) {
  switch (app_network_data->state) {
    case APP_NETWORK_TCPIP_WAIT_INIT:
      if (!app_network_tcpip_init_wait(app_network_data)) {
        break;
      }
    case APP_NETWORK_WIFI_CONFIG:
      if (!app_network_wifi_config(app_network_data)) {
        break;
      }
    case APP_NETWORK_TCPIP_MODULES_ENABLE:
      app_network_tcpip_module_enable(app_network_data);
      timestamp_dhcp_kickin(app_network_data->ip_wait);
      break;
    case APP_NETWORK_TCPIP_TRANSACT:
      // TODO(sergey): This perhaps belongs to an application-level tasks.
      SYS_CMD_READY_TO_READ();
      app_network_run(app_network_data);
      break;
    case APP_NETWORK_TCPIP_ERROR:
      // TODO(sergey): Do we need to do something here?
      break;
  }
}

void APP_Network_PHY_Reset(const struct DRV_ETHPHY_OBJECT_BASE_TYPE* pBaseObj) {
  // TODO(sergey): Check whether it's LAN8720 PHY.
  ETH_NRSTOff();
  ETH_NRSTOn();
}
