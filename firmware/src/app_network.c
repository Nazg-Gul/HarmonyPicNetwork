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
#include "app_network_utils.h"
#include "system_definitions.h"

static bool app_network_tcpip_init_wait(AppNetworkData* app_network_data) {
  SYS_STATUS tcpip_status =
      TCPIP_STACK_Status(app_network_data->system_objects->tcpip);
  if (tcpip_status < 0) {
    SYS_CONSOLE_MESSAGE("TCP/IP stack initialization failed.\r\n");
    app_network_data->state = APP_NETWORK_TCPIP_ERROR;
    return true;
  } else if (tcpip_status == SYS_STATUS_READY) {
    SYS_CONSOLE_MESSAGE("TCP/IP stack initialization succeeded.\r\n");
    // If we don't have WiFi configured, we skip corresponding
    // initialization step.
    bool has_wifi = false;
    int i, num_nets = TCPIP_STACK_NumberOfNetworksGet();
    for (i = 0; i < num_nets; ++i) {
      TCPIP_NET_HANDLE net = TCPIP_STACK_IndexToNet(i);
      const char *net_name = TCPIP_STACK_NetNameGet(net);
      if (IS_WIFI_INTERFACE(net_name)) {
        has_wifi = true;
      }
    }
    if (has_wifi) {
      SYS_CONSOLE_MESSAGE("APP: Waiting WiFI module to finish configuration\r\n");
      app_network_data->state = APP_NETWORK_WIFI_CONFIG;
    } else {
      app_network_data->state = APP_NETWORK_TCPIP_MODULES_ENABLE;
    }
    return true;
  }
  return false;
}

static bool app_network_wifi_config(AppNetworkData* app_network_data) {
  // THe following condition is required in case Wi-Fi interface is reset
  // due to connection error.
  IWPRIV_GET_PARAM wifi_get_param;
  iwpriv_get(DRVSTATUS_GET, &wifi_get_param);
  if (wifi_get_param.driverStatus.isOpen) {
    SYS_CONSOLE_MESSAGE("APP: WiFi driver is open\r\n");
    wifi_get_param.devInfo.data = &app_network_data->wifi_device_info;
    iwpriv_get(DEVICEINFO_GET, &wifi_get_param);
    app_network_data->wifi_net_handle =
        TCPIP_STACK_NetHandleGet(WIFI_INTERFACE_NAME);
    app_network_data->wifi_default_ip.Val =
        TCPIP_STACK_NetAddress(app_network_data->wifi_net_handle);
    app_network_data->state = APP_NETWORK_TCPIP_MODULES_ENABLE;
    return true;
  }
  return false;
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
  // NOTE: This app supports 2 interfaces so far.
  // TODO(sergey): Move static variables to application state.
  static bool is_wifi_power_save_configured = false;
  static bool was_net_up[2] = {true, true};
  static uint32_t reconn_retries = 0;
  static uint32_t start_tick = 0;
  static IPV4_ADDR last_ip[2] = { {-1}, {-1} };
  int i, num_nets;
  TCPIP_NET_HANDLE wifi_net_handle = app_network_data->wifi_net_handle;
  IWPRIV_GET_PARAM wifi_get_param;

  iwpriv_get(CONNSTATUS_GET, &wifi_get_param);
  switch (wifi_get_param.conn.status) {
    case IWPRIV_CONNECTION_SUCCESSFUL:
      // Resetting reconnection retries.
      reconn_retries = 0;
      break;
    case IWPRIV_CONNECTION_FAILED:
      if (reconn_retries++ < WIFI_RECONNECTION_RETRY_LIMIT) {
        SYS_CONSOLE_PRINT("\r\nCouldn't connect to target AP, "
                          "resetting Wi-Fi module and trying to reconnect, "
                          "retries left: %u\r\n",
                          WIFI_RECONNECTION_RETRY_LIMIT - reconn_retries);
        app_network_tcpip_ifmodules_disable(wifi_net_handle);
        app_network_tcpip_iface_down(wifi_net_handle);
        app_network_tcpip_iface_up(wifi_net_handle);
        is_wifi_power_save_configured = false;
        app_network_data->state = APP_NETWORK_WIFI_CONFIG;
        return;
      }
      break;
    case IWPRIV_CONNECTION_REESTABLISHED:
      // Restart DHCP client and config power save.
      TCPIP_DHCP_Disable(wifi_net_handle);
      TCPIP_DHCP_Enable(wifi_net_handle);
      is_wifi_power_save_configured = false;
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
    if (!TCPIP_STACK_NetIsUp(net) && was_net_up[i]) {
      const char *net_name = TCPIP_STACK_NetNameGet(net);
      was_net_up[i] = false;
      app_network_tcpip_ifmodules_disable(net);
      if (IS_WIFI_INTERFACE(net_name)) {
        is_wifi_power_save_configured = false;
      }
    }
    if (TCPIP_STACK_NetIsUp(net) && !was_net_up[i]) {
      was_net_up[i] = true;
      app_network_tcpip_ifmodules_enable(net);
    }
  }

  // If we get a new IP address that is different than the default one,
  // we will run PowerSave configuration.
  if (!is_wifi_power_save_configured &&
    TCPIP_STACK_NetIsUp(wifi_net_handle) &&
    (TCPIP_STACK_NetAddress(wifi_net_handle) != app_network_data->wifi_default_ip.Val)) {
    app_network_wifi_powersave_config(true);
    is_wifi_power_save_configured = true;
  }

  app_network_wifi_DHCPS_sync(wifi_net_handle);

  // If the IP address of an interface has changed, 
  // display the new value on console.
  for (i = 0; i < num_nets; ++i) {
    IPV4_ADDR ipAddr;
    TCPIP_NET_HANDLE netH = TCPIP_STACK_IndexToNet(i);
    ipAddr.Val = TCPIP_STACK_NetAddress(netH);
    if (last_ip[i].Val != ipAddr.Val) {
      last_ip[i].Val = ipAddr.Val;
      if (ipAddr.Val != 0) {
        SYS_CONSOLE_PRINT("%s IPv4 Address: %d.%d.%d.%d \r\n",
                          TCPIP_STACK_NetNameGet(netH),
                          ipAddr.v[0], ipAddr.v[1], ipAddr.v[2], ipAddr.v[3]);
        app_network_data->ip_wait = 0;
      }
    }
  }

  const uint32_t time_delta = SYS_TMR_TickCountGet() - start_tick;
  const uint32_t time_threshold = SYS_TMR_TickCounterFrequencyGet() / 2ul;
  if (time_delta >= time_threshold) {
    if (app_network_data->ip_wait &&
        ++app_network_data->ip_wait > WIFI_DHCP_WAIT_THRESHOLD) {
      app_network_data->ip_wait = 0;
      if (wifi_get_param.conn.status == IWPRIV_CONNECTION_SUCCESSFUL)
        SYS_CONSOLE_MESSAGE(
            "\r\nFailed to obtain an IP address from DHCP server\r\n"
            "If WEP security is used, double-check if the key is valid\r\n");
    }
    start_tick = SYS_TMR_TickCountGet();
  }
}

void APP_Network_Initialize(AppNetworkData* app_network_data,
                            SYSTEM_OBJECTS* system_objects) {
  app_network_data->system_objects = system_objects;

  app_network_data->state = APP_NETWORK_TCPIP_WAIT_INIT;
  app_network_data->ip_wait = 0;
  // Initialize WiFi networking.
  app_network_data->wifi_default_ip.Val = -1;
  app_network_data->wifi_net_handle = NULL;
  IWPRIV_SET_PARAM wifi_set_param;
  wifi_set_param.conn.initConnAllowed = true;
  iwpriv_set(INITCONN_OPTION_SET, &wifi_set_param);
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
