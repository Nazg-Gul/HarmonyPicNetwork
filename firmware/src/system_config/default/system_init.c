/*******************************************************************************
  System Initialization File

  File Name:
    system_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits, 
    and allocates any necessary global system resources, such as the 
    sysObj structure that contains the object handles to all the MPLAB Harmony 
    module objects in the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="Configuration Bits">

/*** DEVCFG0 ***/

#pragma config DEBUG =      OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config PWP =        OFF
#pragma config BWP =        OFF
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      PRIPLL
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF
#pragma config POSCMOD =    HS
#pragma config OSCIOFNC =   OFF
#pragma config FPBDIV =     DIV_2
#pragma config FCKSM =      CSECMD
#pragma config WDTPS =      PS32768
#pragma config FWDTEN =     OFF
/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_12
#pragma config FPLLMUL =    MUL_20
#pragma config FPLLODIV =   DIV_1
#pragma config UPLLIDIV =   DIV_12
#pragma config UPLLEN =     ON
/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config FSRSSEL =    PRIORITY_7
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     ON
#pragma config FCANIO =     OFF
#pragma config FUSBIDIO =   OFF
#pragma config FVBUSONIO =  OFF
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************

/* MIIM Driver Configuration */
static const DRV_MIIM_INIT drvMiimInitData =
{
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    .ethphyId = DRV_MIIM_ETH_MODULE_ID,
};
// <editor-fold defaultstate="collapsed" desc="DRV_SPI Initialization Data"> 
 /*** SPI Driver Initialization Data ***/
  /*** Index 0  ***/
 DRV_SPI_INIT drvSpi0InitData =
 {
    .spiId = DRV_SPI_SPI_ID_IDX0,
    .taskMode = DRV_SPI_TASK_MODE_IDX0,
    .spiMode = DRV_SPI_SPI_MODE_IDX0,
    .allowIdleRun = DRV_SPI_ALLOW_IDLE_RUN_IDX0,
    .spiProtocolType = DRV_SPI_SPI_PROTOCOL_TYPE_IDX0,
    .commWidth = DRV_SPI_COMM_WIDTH_IDX0,
    .spiClk = DRV_SPI_SPI_CLOCK_IDX0,
    .baudRate = DRV_SPI_BAUD_RATE_IDX0,
    .bufferType = DRV_SPI_BUFFER_TYPE_IDX0,
    .clockMode = DRV_SPI_CLOCK_MODE_IDX0,
    .inputSamplePhase = DRV_SPI_INPUT_PHASE_IDX0,
    .dummyByteValue = DRV_SPI_TRANSMIT_DUMMY_BYTE_VALUE_IDX0,
    .queueSize = DRV_SPI_QUEUE_SIZE_IDX0,
    .jobQueueReserveSize = DRV_SPI_RESERVED_JOB_IDX0,
 };
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_Timer Initialization Data">
/*** TMR Driver Initialization Data ***/

const DRV_TMR_INIT drvTmr0InitData =
{
    .moduleInit.sys.powerState = DRV_TMR_POWER_STATE_IDX0,
    .tmrId = DRV_TMR_PERIPHERAL_ID_IDX0,
    .clockSource = DRV_TMR_CLOCK_SOURCE_IDX0, 
    .prescale = DRV_TMR_PRESCALE_IDX0,
    .mode = DRV_TMR_OPERATION_MODE_IDX0,
    .interruptSource = DRV_TMR_INTERRUPT_SOURCE_IDX0,
    .asyncWriteEnable = false,
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Initialization Data">

const DRV_USART_INIT drvUsart0InitData =
{
    .moduleInit.value = DRV_USART_POWER_STATE_IDX0,
    .usartID = DRV_USART_PERIPHERAL_ID_IDX0, 
    .mode = DRV_USART_OPER_MODE_IDX0,
    .flags = DRV_USART_INIT_FLAGS_IDX0,
    .brgClock = DRV_USART_BRG_CLOCK_IDX0,
    .lineControl = DRV_USART_LINE_CNTRL_IDX0,
    .baud = DRV_USART_BAUD_RATE_IDX0,
    .handshake = DRV_USART_HANDSHAKE_MODE_IDX0,
    .interruptTransmit = DRV_USART_XMIT_INT_SRC_IDX0,
    .interruptReceive = DRV_USART_RCV_INT_SRC_IDX0,
    .interruptError = DRV_USART_ERR_INT_SRC_IDX0,
    .queueSizeTransmit = DRV_USART_XMIT_QUEUE_SIZE_IDX0,
    .queueSizeReceive = DRV_USART_RCV_QUEUE_SIZE_IDX0,
    .dmaChannelTransmit = DMA_CHANNEL_NONE,
    .dmaInterruptTransmit = DRV_USART_XMIT_INT_SRC_IDX0,    
    .dmaChannelReceive = DMA_CHANNEL_NONE,
    .dmaInterruptReceive = DRV_USART_RCV_INT_SRC_IDX0,    
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USB Initialization Data">
/******************************************************
 * USB Driver Initialization
 ******************************************************/
/****************************************************
 * Endpoint Table needed by the Device Layer.
 ****************************************************/
uint8_t __attribute__((aligned(512))) endPointTable[DRV_USBFS_ENDPOINTS_NUMBER * 32];
const DRV_USBFS_INIT drvUSBInit =
{
    /* Assign the endpoint table */
    .endpointTable= endPointTable,

    /* Interrupt Source for USB module */
    .interruptSource = INT_SOURCE_USB_1,
    
    /* System module initialization */
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    
    .operationMode = DRV_USBFS_OPMODE_DEVICE,
    
    .operationSpeed = USB_SPEED_FULL,
    
    /* Stop in idle */
    .stopInIdle = false,

    /* Suspend in sleep */
    .suspendInSleep = false,

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_ID_1
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Module Initialization Data
// *****************************************************************************
// *****************************************************************************
/* Net Presentation Layer Data Definitions */
#include "framework/net/pres/net_pres_enc_glue.h"

static const NET_PRES_TransportObject netPresTransObject0SS = {
    .fpOpen        = (NET_PRES_TransOpen)TCPIP_TCP_ServerOpen,
    .fpLocalBind         = (NET_PRES_TransBind)TCPIP_TCP_Bind,
    .fpRemoteBind        = (NET_PRES_TransBind)TCPIP_TCP_RemoteBind,
    .fpOptionGet         = (NET_PRES_TransOption)TCPIP_TCP_OptionsGet,
    .fpOptionSet         = (NET_PRES_TransOption)TCPIP_TCP_OptionsSet,
    .fpIsConnected       = (NET_PRES_TransBool)TCPIP_TCP_IsConnected,
    .fpWasReset          = (NET_PRES_TransBool)TCPIP_TCP_WasReset,
    .fpDisconnect        = (NET_PRES_TransBool)TCPIP_TCP_Disconnect,
    .fpConnect           = (NET_PRES_TransBool)TCPIP_TCP_Connect,
    .fpClose             = (NET_PRES_TransClose)TCPIP_TCP_Close,
    .fpSocketInfoGet     = (NET_PRES_TransSocketInfoGet)TCPIP_TCP_SocketInfoGet,
    .fpFlush             = (NET_PRES_TransBool)TCPIP_TCP_Flush,
    .fpPeek              = (NET_PRES_TransPeek)TCPIP_TCP_ArrayPeek,
    .fpDiscard           = (NET_PRES_TransDiscard)TCPIP_TCP_Discard,
    .fpHandlerRegister   = (NET_PRES_TransHandlerRegister)TCPIP_TCP_SignalHandlerRegister,
    .fpHandlerDeregister = (NET_PRES_TransSignalHandlerDeregister)TCPIP_TCP_SignalHandlerDeregister,
    .fpRead              = (NET_PRES_TransRead)TCPIP_TCP_ArrayGet,
    .fpWrite             = (NET_PRES_TransWrite)TCPIP_TCP_ArrayPut,
    .fpReadyToRead       = (NET_PRES_TransReady)TCPIP_TCP_GetIsReady,
    .fpReadyToWrite      = (NET_PRES_TransReady)TCPIP_TCP_PutIsReady,
    .fpIsPortDefaultSecure = (NET_PRES_TransIsPortDefaultSecured)TCPIP_Helper_TCPSecurePortGet,
};
static const NET_PRES_TransportObject netPresTransObject0SC = {
    .fpOpen        = (NET_PRES_TransOpen)TCPIP_TCP_ClientOpen,
    .fpLocalBind         = (NET_PRES_TransBind)TCPIP_TCP_Bind,
    .fpRemoteBind        = (NET_PRES_TransBind)TCPIP_TCP_RemoteBind,
    .fpOptionGet         = (NET_PRES_TransOption)TCPIP_TCP_OptionsGet,
    .fpOptionSet         = (NET_PRES_TransOption)TCPIP_TCP_OptionsSet,
    .fpIsConnected       = (NET_PRES_TransBool)TCPIP_TCP_IsConnected,
    .fpWasReset          = (NET_PRES_TransBool)TCPIP_TCP_WasReset,
    .fpDisconnect        = (NET_PRES_TransBool)TCPIP_TCP_Disconnect,
    .fpConnect           = (NET_PRES_TransBool)TCPIP_TCP_Connect,
    .fpClose             = (NET_PRES_TransClose)TCPIP_TCP_Close,
    .fpSocketInfoGet     = (NET_PRES_TransSocketInfoGet)TCPIP_TCP_SocketInfoGet,
    .fpFlush             = (NET_PRES_TransBool)TCPIP_TCP_Flush,
    .fpPeek              = (NET_PRES_TransPeek)TCPIP_TCP_ArrayPeek,
    .fpDiscard           = (NET_PRES_TransDiscard)TCPIP_TCP_Discard,
    .fpHandlerRegister   = (NET_PRES_TransHandlerRegister)TCPIP_TCP_SignalHandlerRegister,
    .fpHandlerDeregister = (NET_PRES_TransSignalHandlerDeregister)TCPIP_TCP_SignalHandlerDeregister,
    .fpRead              = (NET_PRES_TransRead)TCPIP_TCP_ArrayGet,
    .fpWrite             = (NET_PRES_TransWrite)TCPIP_TCP_ArrayPut,
    .fpReadyToRead       = (NET_PRES_TransReady)TCPIP_TCP_GetIsReady,
    .fpReadyToWrite      = (NET_PRES_TransReady)TCPIP_TCP_PutIsReady,
    .fpIsPortDefaultSecure = (NET_PRES_TransIsPortDefaultSecured)TCPIP_Helper_TCPSecurePortGet,
};
static const NET_PRES_TransportObject netPresTransObject0DS = {
    .fpOpen        = (NET_PRES_TransOpen)TCPIP_UDP_ServerOpen,
    .fpLocalBind         = (NET_PRES_TransBind)TCPIP_UDP_Bind,
    .fpRemoteBind        = (NET_PRES_TransBind)TCPIP_UDP_RemoteBind,
    .fpOptionGet         = (NET_PRES_TransOption)TCPIP_UDP_OptionsGet,
    .fpOptionSet         = (NET_PRES_TransOption)TCPIP_UDP_OptionsSet,
    .fpIsConnected       = (NET_PRES_TransBool)TCPIP_UDP_IsConnected,
    .fpWasReset          = NULL,
    .fpDisconnect        = (NET_PRES_TransBool)TCPIP_UDP_Disconnect,
    .fpConnect          = NULL,
    .fpClose             = (NET_PRES_TransClose)TCPIP_UDP_Close,
    .fpSocketInfoGet     = (NET_PRES_TransSocketInfoGet)TCPIP_UDP_SocketInfoGet,
    .fpFlush             = (NET_PRES_TransBool)TCPIP_UDP_Flush,
    .fpPeek              = NULL,
    .fpDiscard           = (NET_PRES_TransDiscard)TCPIP_UDP_Discard,
    .fpHandlerRegister   = (NET_PRES_TransHandlerRegister)TCPIP_UDP_SignalHandlerRegister,
    .fpHandlerDeregister = (NET_PRES_TransSignalHandlerDeregister)TCPIP_UDP_SignalHandlerDeregister,
    .fpRead              = (NET_PRES_TransRead)TCPIP_UDP_ArrayGet,
    .fpWrite             = (NET_PRES_TransWrite)TCPIP_UDP_ArrayPut,
    .fpReadyToRead       = (NET_PRES_TransReady)TCPIP_UDP_GetIsReady,
    .fpReadyToWrite      = (NET_PRES_TransReady)TCPIP_UDP_PutIsReady,
    .fpIsPortDefaultSecure = (NET_PRES_TransIsPortDefaultSecured)TCPIP_Helper_UDPSecurePortGet,
};
static const NET_PRES_TransportObject netPresTransObject0DC = {
    .fpOpen        = (NET_PRES_TransOpen)TCPIP_UDP_ClientOpen,
    .fpLocalBind         = (NET_PRES_TransBind)TCPIP_UDP_Bind,
    .fpRemoteBind        = (NET_PRES_TransBind)TCPIP_UDP_RemoteBind,
    .fpOptionGet         = (NET_PRES_TransOption)TCPIP_UDP_OptionsGet,
    .fpOptionSet         = (NET_PRES_TransOption)TCPIP_UDP_OptionsSet,
    .fpIsConnected       = (NET_PRES_TransBool)TCPIP_UDP_IsConnected,
    .fpWasReset          = NULL,
    .fpDisconnect        = (NET_PRES_TransBool)TCPIP_UDP_Disconnect,
    .fpConnect          = NULL,
    .fpClose             = (NET_PRES_TransClose)TCPIP_UDP_Close,
    .fpSocketInfoGet     = (NET_PRES_TransSocketInfoGet)TCPIP_UDP_SocketInfoGet,
    .fpFlush             = (NET_PRES_TransBool)TCPIP_UDP_Flush,
    .fpPeek              = NULL,
    .fpDiscard           = (NET_PRES_TransDiscard)TCPIP_UDP_Discard,
    .fpHandlerRegister   = (NET_PRES_TransHandlerRegister)TCPIP_UDP_SignalHandlerRegister,
    .fpHandlerDeregister = (NET_PRES_TransSignalHandlerDeregister)TCPIP_UDP_SignalHandlerDeregister,
    .fpRead              = (NET_PRES_TransRead)TCPIP_UDP_ArrayGet,
    .fpWrite             = (NET_PRES_TransWrite)TCPIP_UDP_ArrayPut,
    .fpReadyToRead       = (NET_PRES_TransReady)TCPIP_UDP_GetIsReady,
    .fpReadyToWrite      = (NET_PRES_TransReady)TCPIP_UDP_PutIsReady,
    .fpIsPortDefaultSecure = (NET_PRES_TransIsPortDefaultSecured)TCPIP_Helper_UDPSecurePortGet,
};
static const NET_PRES_INST_DATA netPresCfgs[] = 
{
    {
        .pTransObject_ss = &netPresTransObject0SS,
        .pTransObject_sc = &netPresTransObject0SC,
        .pTransObject_ds = &netPresTransObject0DS,
        .pTransObject_dc = &netPresTransObject0DC,
        .pProvObject_ss = NULL,
        .pProvObject_sc = NULL,
        .pProvObject_ds = NULL,
        .pProvObject_dc = NULL,
    },
};

static const NET_PRES_INIT_DATA netPresInitData = 
{
    .numLayers = sizeof(netPresCfgs) / sizeof(NET_PRES_INST_DATA),
    .pInitData = netPresCfgs
};
  
 
// <editor-fold defaultstate="collapsed" desc="SYS_COMMAND Initialization Data">
/*** System Command Initialization Data ***/

SYS_CMD_INIT sysCmdInit =
{
    .moduleInit = {0},
    .consoleCmdIOParam = SYS_CMD_SINGLE_CHARACTER_READ_CONSOLE_IO_PARAM,
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_CONSOLE Initialization Data">
/*** System Console Initialization Data ***/

SYS_MODULE_OBJ sysConsoleObjects[] = { SYS_MODULE_OBJ_INVALID };

/* Declared in console device implementation (sys_console_uart.c) */
extern SYS_CONSOLE_DEV_DESC consUsartDevDesc;
SYS_CONSOLE_INIT consUsartInit0 =
{
    .moduleInit = {0},
    .consDevDesc = &consUsartDevDesc,
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_DEBUG Initialization Data">
/*** System Debug Initialization Data ***/

SYS_DEBUG_INIT debugInit =
{
    .moduleInit = {0},
    .errorLevel = SYS_ERROR_DEBUG
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_TMR Initialization Data">
/*** TMR Service Initialization Data ***/
const SYS_TMR_INIT sysTmrInitData =
{
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    .drvIndex = DRV_TMR_INDEX_0,
    .tmrFreq = 1000, 
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="TCP/IP Stack Initialization Data">
// *****************************************************************************
// *****************************************************************************
// Section: TCPIP Data
// *****************************************************************************
// *****************************************************************************

/*** ARP Service Initialization Data ***/
const TCPIP_ARP_MODULE_CONFIG tcpipARPInitData =
{ 
    .cacheEntries       = TCPIP_ARP_CACHE_ENTRIES,     
    .deleteOld          = TCPIP_ARP_CACHE_DELETE_OLD,    
    .entrySolvedTmo     = TCPIP_ARP_CACHE_SOLVED_ENTRY_TMO, 
    .entryPendingTmo    = TCPIP_ARP_CACHE_PENDING_ENTRY_TMO, 
    .entryRetryTmo      = TCPIP_ARP_CACHE_PENDING_RETRY_TMO, 
    .permQuota          = TCPIP_ARP_CACHE_PERMANENT_QUOTA, 
    .purgeThres         = TCPIP_ARP_CACHE_PURGE_THRESHOLD, 
    .purgeQuanta        = TCPIP_ARP_CACHE_PURGE_QUANTA, 
    .retries            = TCPIP_ARP_CACHE_ENTRY_RETRIES, 
    .gratProbeCount     = TCPIP_ARP_GRATUITOUS_PROBE_COUNT,
};

/*** telnet Server Initialization Data ***/
const TCPIP_TELNET_MODULE_CONFIG tcpipTelnetInitData =
{ 
};

/*** Announce Discovery Initialization Data ***/
const TCPIP_ANNOUNCE_MODULE_CONFIG tcpipAnnounceInitData =
{ 
};

/*** UDP Sockets Initialization Data ***/
const TCPIP_UDP_MODULE_CONFIG tcpipUDPInitData =
{
    .nSockets       = TCPIP_UDP_MAX_SOCKETS,
    .sktTxBuffSize  = TCPIP_UDP_SOCKET_DEFAULT_TX_SIZE, 
};

/*** TCP Sockets Initialization Data ***/
const TCPIP_TCP_MODULE_CONFIG tcpipTCPInitData =
{
    .nSockets       = TCPIP_TCP_MAX_SOCKETS,
    .sktTxBuffSize  = TCPIP_TCP_SOCKET_DEFAULT_TX_SIZE, 
    .sktRxBuffSize  = TCPIP_TCP_SOCKET_DEFAULT_RX_SIZE,
};






/*** DHCP client Initialization Data ***/
const TCPIP_DHCP_MODULE_CONFIG tcpipDHCPInitData =
{     
    .dhcpEnable     = TCPIP_DHCP_CLIENT_ENABLED,   
    .dhcpTmo        = TCPIP_DHCP_TIMEOUT,
    .dhcpCliPort    = TCPIP_DHCP_CLIENT_CONNECT_PORT,
    .dhcpSrvPort    = TCPIP_DHCP_SERVER_LISTEN_PORT,

};


/*** ICMP Server Initialization Data ***/
const TCPIP_ICMP_MODULE_CONFIG tcpipICMPInitData = 
{
};

/*** NBNS Server Initialization Data ***/
const TCPIP_NBNS_MODULE_CONFIG tcpipNBNSInitData =
{ 
};


/*** ETH PHY Initialization Data ***/

extern void APP_Network_PHY_Reset( const struct DRV_ETHPHY_OBJECT_BASE_TYPE* pBaseObj);

const DRV_ETHPHY_INIT tcpipPhyInitData =
{
    .moduleInit             = {SYS_MODULE_POWER_RUN_FULL},
    .ethphyId               = TCPIP_EMAC_MODULE_ID,
    .phyAddress             = TCPIP_EMAC_PHY_ADDRESS,
    .phyFlags               = TCPIP_EMAC_PHY_CONFIG_FLAGS,
    .pPhyObject             = &DRV_ETHPHY_OBJECT_SMSC_LAN8720,
    .resetFunction          = APP_Network_PHY_Reset,
    .pMiimObject            = &DRV_MIIM_OBJECT_BASE_Default,
    .pMiimInit              = &drvMiimInitData,
    .miimIndex              = DRV_MIIM_DRIVER_INDEX,
};

/*** ETH MAC Initialization Data ***/
const TCPIP_MODULE_MAC_PIC32INT_CONFIG tcpipMACPIC32INTInitData =
{ 
    .nTxDescriptors         = TCPIP_EMAC_TX_DESCRIPTORS,
    .rxBuffSize             = TCPIP_EMAC_RX_BUFF_SIZE,
    .nRxDescriptors         = TCPIP_EMAC_RX_DESCRIPTORS,
    .nRxDedicatedBuffers    = TCPIP_EMAC_RX_DEDICATED_BUFFERS,
    .nRxInitBuffers         = TCPIP_EMAC_RX_INIT_BUFFERS,
    .rxLowThreshold         = TCPIP_EMAC_RX_LOW_THRESHOLD,
    .rxLowFill              = TCPIP_EMAC_RX_LOW_FILL,
    .linkInitDelay          = TCPIP_EMAC_PHY_LINK_INIT_DELAY,
    .ethFlags               = TCPIP_EMAC_ETH_OPEN_FLAGS,
    .ethModuleId            = TCPIP_EMAC_MODULE_ID,
    .pPhyBase               = &DRV_ETHPHY_OBJECT_BASE_Default,
    .pPhyInit               = &tcpipPhyInitData,
};

/*** Wi-Fi Interface MRF24W Initialization Data ***/
const TCPIP_MODULE_MAC_MRF24W_CONFIG macMRF24WConfigData = {
};


/*** Zeroconfig initialization data ***/
const ZCLL_MODULE_CONFIG tcpipZCLLInitData =
{
};




/*** DNS Client Initialization Data ***/
const TCPIP_DNS_CLIENT_MODULE_CONFIG tcpipDNSClientInitData =
{
    .deleteOldLease         = TCPIP_DNS_CLIENT_DELETE_OLD_ENTRIES,
    .cacheEntries           = TCPIP_DNS_CLIENT_CACHE_ENTRIES,
    .entrySolvedTmo         = TCPIP_DNS_CLIENT_CACHE_ENTRY_TMO,    
    .nIPv4Entries  = TCPIP_DNS_CLIENT_CACHE_PER_IPV4_ADDRESS,
    .ipAddressType       = TCPIP_DNS_CLIENT_ADDRESS_TYPE,
    .nIPv6Entries  = TCPIP_DNS_CLIENT_CACHE_PER_IPV6_ADDRESS,
};




TCPIP_STACK_HEAP_INTERNAL_CONFIG tcpipHeapConfig =
{
    .heapType = TCPIP_STACK_HEAP_TYPE_INTERNAL_HEAP,
    .heapFlags = TCPIP_STACK_HEAP_USE_FLAGS,
    .heapUsage = TCPIP_STACK_HEAP_USAGE_CONFIG,
    .malloc_fnc = TCPIP_STACK_MALLOC_FUNC,
    .calloc_fnc = TCPIP_STACK_CALLOC_FUNC,
    .free_fnc = TCPIP_STACK_FREE_FUNC,
    .heapSize = TCPIP_STACK_DRAM_SIZE,
};
 
const TCPIP_NETWORK_CONFIG __attribute__((unused))  TCPIP_HOSTS_CONFIGURATION[] =
{
/*** Network Configuration Index 0 ***/
    {
        TCPIP_NETWORK_DEFAULT_INTERFACE_NAME,       // interface
        TCPIP_NETWORK_DEFAULT_HOST_NAME,            // hostName
        TCPIP_NETWORK_DEFAULT_MAC_ADDR,             // macAddr
        TCPIP_NETWORK_DEFAULT_IP_ADDRESS,           // ipAddr
        TCPIP_NETWORK_DEFAULT_IP_MASK,              // ipMask
        TCPIP_NETWORK_DEFAULT_GATEWAY,              // gateway
        TCPIP_NETWORK_DEFAULT_DNS,                  // priDNS
        TCPIP_NETWORK_DEFAULT_SECOND_DNS,           // secondDNS
        TCPIP_NETWORK_DEFAULT_POWER_MODE,           // powerMode
        TCPIP_NETWORK_DEFAULT_INTERFACE_FLAGS,      // startFlags
       &TCPIP_NETWORK_DEFAULT_MAC_DRIVER,           // pMacObject
    },
/*** Network Configuration Index 1 ***/
    {
        TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX1,       // interface
        TCPIP_NETWORK_DEFAULT_HOST_NAME_IDX1,            // hostName
        TCPIP_NETWORK_DEFAULT_MAC_ADDR_IDX1,             // macAddr
        TCPIP_NETWORK_DEFAULT_IP_ADDRESS_IDX1,           // ipAddr
        TCPIP_NETWORK_DEFAULT_IP_MASK_IDX1,              // ipMask
        TCPIP_NETWORK_DEFAULT_GATEWAY_IDX1,              // gateway
        TCPIP_NETWORK_DEFAULT_DNS_IDX1,                  // priDNS
        TCPIP_NETWORK_DEFAULT_SECOND_DNS_IDX1,           // secondDNS
        TCPIP_NETWORK_DEFAULT_POWER_MODE_IDX1,           // powerMode
        TCPIP_NETWORK_DEFAULT_INTERFACE_FLAGS_IDX1,      // startFlags
       &TCPIP_NETWORK_DEFAULT_MAC_DRIVER_IDX1,           // pMacObject
    },
};

const TCPIP_STACK_MODULE_CONFIG TCPIP_STACK_MODULE_CONFIG_TBL [] =
{
    {TCPIP_MODULE_IPV4,             0},
    {TCPIP_MODULE_ICMP,             0},                             // TCPIP_MODULE_ICMP
    {TCPIP_MODULE_ARP,              &tcpipARPInitData},             // TCPIP_MODULE_ARP
    {TCPIP_MODULE_UDP,              &tcpipUDPInitData},             // TCPIP_MODULE_UDP
    {TCPIP_MODULE_TCP,              &tcpipTCPInitData},             // TCPIP_MODULE_TCP
    {TCPIP_MODULE_DHCP_CLIENT,      &tcpipDHCPInitData},            // TCPIP_MODULE_DHCP_CLIENT
    {TCPIP_MODULE_ANNOUNCE,         &tcpipAnnounceInitData},        // TCPIP_MODULE_ANNOUNCE
    {TCPIP_MODULE_DNS_CLIENT,       &tcpipDNSClientInitData},       // TCPIP_MODULE_DNS_CLIENT
    {TCPIP_MODULE_NBNS,             &tcpipNBNSInitData},            // TCPIP_MODULE_NBNS

    {TCPIP_MODULE_TELNET_SERVER,    &tcpipTelnetInitData},          // TCPIP_MODULE_TELNET_SERVER
    {TCPIP_MODULE_ZCLL,             0},                             // TCPIP_MODULE_ZCLL,
    {TCPIP_MODULE_MDNS,             0},                             // TCPIP_MODULE_MDNS,
    { TCPIP_MODULE_MANAGER,         &tcpipHeapConfig },             // TCPIP_MODULE_MANAGER
    // MAC modules
    {TCPIP_MODULE_MAC_PIC32INT,     &tcpipMACPIC32INTInitData},     // TCPIP_MODULE_MAC_PIC32INT
    {TCPIP_MODULE_MAC_MRF24W,       &macMRF24WConfigData},          // TCPIP_MODULE_MAC_MRF24W

};

/*********************************************************************
 * Function:        SYS_MODULE_OBJ TCPIP_STACK_Init()
 *
 * PreCondition:    None
 *
 * Input:
 *
 * Output:          valid system module object if Stack and its componets are initialized
 *                  SYS_MODULE_OBJ_INVALID otherwise
 *
 * Overview:        The function starts the initialization of the stack.
 *                  If an error occurs, the SYS_ERROR() is called
 *                  and the function de-initialize itself and will return false.
 *
 * Side Effects:    None
 *
 * Note:            This function must be called before any of the
 *                  stack or its component routines are used.
 *
 ********************************************************************/

SYS_MODULE_OBJ TCPIP_STACK_Init()
{
    TCPIP_STACK_INIT    tcpipInit;

    tcpipInit.moduleInit.sys.powerState = SYS_MODULE_POWER_RUN_FULL;
    tcpipInit.pNetConf = TCPIP_HOSTS_CONFIGURATION;
    tcpipInit.nNets = sizeof (TCPIP_HOSTS_CONFIGURATION) / sizeof (*TCPIP_HOSTS_CONFIGURATION);
    tcpipInit.pModConfig = TCPIP_STACK_MODULE_CONFIG_TBL;
    tcpipInit.nModules = sizeof (TCPIP_STACK_MODULE_CONFIG_TBL) / sizeof (*TCPIP_STACK_MODULE_CONFIG_TBL);

    return TCPIP_STACK_Initialize(0, &tcpipInit.moduleInit);
}
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="USB Stack Initialization Data">


/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt0[] =
{
    0x06, 0x00, 0xFF,   // Usage Page = 0xFF00 (Vendor Defined Page 1)
    0x09, 0x01,             // Usage (Vendor Usage 1)
    0xA1, 0x01,             // Colsslection (Application)
    0x19, 0x01,             // Usage Minimum
    0x29, 0x40,             // Usage Maximum 	//64 input usages total (0x01 to 0x40)
    0x15, 0x01,             // Logical Minimum (data bytes in the report may have minimum value = 0x00)
    0x25, 0x40,      	    // Logical Maximum (data bytes in the report may have maximum value = 0x00FF = unsigned 255)
    0x75, 0x08,             // Report Size: 8-bit field size
    0x95, 0x40,             // Report Count: Make sixty-four 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item)
    0x81, 0x00,             // Input (Data, Array, Abs): Instantiates input packet fields based on the above report size, count, logical min/max, and usage.
    0x19, 0x01,             // Usage Minimum
    0x29, 0x40,             // Usage Maximum 	//64 output usages total (0x01 to 0x40)
    0x91, 0x00,             // Output (Data, Array, Abs): Instantiates output packet fields.  Uses same report size and count as "Input" fields, since nothing new/different was specified to the parser since the "Input" item.
    0xC0                    // End Collection
};

/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
    const USB_DEVICE_HID_INIT hidInit0 =
    {
        .hidReportDescriptorSize = sizeof(hid_rpt0),
        .hidReportDescriptor = &hid_rpt0,
        .queueSizeReportReceive = 1,
        .queueSizeReportSend = 1
    };
/**************************************************
 * USB Device Layer Function Driver Registration 
 * Table
 **************************************************/
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[1] =
{
    /* Function 1 */
    { 
        .configurationValue = 1,    /* Configuration value */ 
        .interfaceNumber = 0,       /* First interfaceNumber of this function */ 
        .speed = USB_SPEED_FULL,    /* Function Speed */ 
        .numberOfInterfaces = 1,    /* Number of interfaces */
        .funcDriverIndex = 0,  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit0,    /* Function driver init data*/
    },
};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
/*******************************************
 *  USB Device Descriptor 
 *******************************************/
const USB_DEVICE_DESCRIPTOR deviceDescriptor =
{
    0x12,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,          // DEVICE descriptor type
    0x0200,                         // USB Spec Release Number in BCD format
    0x00,                           // Class Code
    0x00,                           // Subclass code
    0x00,                           // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE,     // Max packet size for EP0, see system_config.h
    0x04D8,                         // Vendor ID
    0x003F,                         // Product ID
    0x0100,                         // Device release number in BCD format
    0x01,                           // Manufacturer string index
    0x02,                           // Product string index
    0x00,                           // Device serial number string index
    0x01                            // Number of possible configurations
};


/*******************************************
 *  USB Full Speed Configuration Descriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor[]=
{
    /* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    41,0,                //(41 Bytes)Size of the Config descriptor.e
    1,                                               // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,                                                 // Max power consumption (2X mA)
    /* Descriptor for Function 1 - HID     */ 
    
	/* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // INTERFACE descriptor type
    0,                                  // Interface Number
    0,                                  // Alternate Setting Number
    2,                                  // Number of endpoints in this interface
    USB_HID_CLASS_CODE,                 // Class code
    USB_HID_SUBCLASS_CODE_NO_SUBCLASS , // Subclass code
    USB_HID_PROTOCOL_CODE_NONE,         // No Protocol
    0,                                  // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,                           // Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    1,                              // Number of class descriptors, see usbcfg.h
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    USB_DEVICE_16bitTo8bitArrange(sizeof(hid_rpt0)),   // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_IN,      // EndpointAddress
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // Size
    0x01,                           // Interval

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_OUT,     // EndpointAddress
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // size
    0x01,                           // Interval
    
    


};

/*******************************************
 * Array of Full speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE fullSpeedConfigDescSet[1] =
{
    fullSpeedConfigurationDescriptor
};


/**************************************
 *  String descriptors.
 *************************************/

 /*******************************************
 *  Language code string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;
        uint8_t bDscType;
        uint16_t string[1];
    }
    sd000 =
    {
        sizeof(sd000),          // Size of this descriptor in bytes
        USB_DESCRIPTOR_STRING,  // STRING descriptor type
        {0x0409}                // Language ID
    };
/*******************************************
 *  Manufacturer string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[2];    // String
    }
    sd001 =
    {
        sizeof(sd001),
        USB_DESCRIPTOR_STRING,
        {'E','z'}
		
    };

/*******************************************
 *  Product string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[10];    // String
    }
    sd002 =
    {
        sizeof(sd002),
        USB_DESCRIPTOR_STRING,
		{'P','i','c','N','e','t','w','o','r','k'}
    }; 

/***************************************
 * Array of string descriptors
 ***************************************/
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002
};

/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &deviceDescriptor,          /* Full speed descriptor */
    1,                          /* Total number of full speed configurations available */
    fullSpeedConfigDescSet,     /* Pointer to array of full speed configurations descriptors*/
    NULL, 
    0, 
    NULL, 
    3,                          // Total number of string descriptors available.
    stringDescriptors,          // Pointer to array of string descriptors.
    NULL, 
    NULL
};


/****************************************************
 * USB Device Layer Initialization Data
 ****************************************************/
const USB_DEVICE_INIT usbDevInitData =
{
    /* System module initialization */
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    
    /* Number of function drivers registered to this instance of the
       USB device layer */
    .registeredFuncCount = 1,
    
    /* Function driver table registered to this instance of the USB device layer*/
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,

    /* Pointer to USB Descriptor structure */
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor,

    /* USB Device Speed */
    .deviceSpeed = USB_SPEED_FULL,
    
    /* Index of the USB Driver to be used by this Device Layer Instance */
    .driverIndex = DRV_USBFS_INDEX_0,

    /* Pointer to the USB Driver Functions. */
    .usbDriverInterface = DRV_USBFS_DEVICE_INTERFACE,
    
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
    See prototype in system/common/sys_module.h.
 */

void SYS_Initialize ( void* data )
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)NULL);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());
    SYS_DEVCON_JTAGDisable();
    SYS_PORTS_Initialize();

    /* Initialize Drivers */

    /*** SPI Driver Index 0 initialization***/

    sysObj.spiObjectIdx0 = DRV_SPI_Initialize(DRV_SPI_INDEX_0, (const SYS_MODULE_INIT  * const)&drvSpi0InitData);
    /* Initialize the MIIM Driver */
    sysObj.drvMiim = DRV_MIIM_Initialize(DRV_MIIM_INDEX_0, (const SYS_MODULE_INIT  * const)&drvMiimInitData);

    sysObj.drvTmr0 = DRV_TMR_Initialize(DRV_TMR_INDEX_0, (SYS_MODULE_INIT *)&drvTmr0InitData);

    SYS_INT_VectorPrioritySet(INT_VECTOR_T2, INT_PRIORITY_LEVEL4);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T2, INT_SUBPRIORITY_LEVEL0);
 
 
     sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *)&drvUsart0InitData);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART1, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART1, INT_SUBPRIORITY_LEVEL0);
    /* Initialize USB Driver */ 
    sysObj.drvUSBObject = DRV_USBFS_Initialize(DRV_USBFS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBInit);

    /* Initialize System Services */

    /*** Command Service Initialization Code ***/
    SYS_CMD_Initialize((SYS_MODULE_INIT*)&sysCmdInit);
    sysObj.sysConsole0 = SYS_CONSOLE_Initialize(SYS_CONSOLE_INDEX_0, (SYS_MODULE_INIT *)&consUsartInit0);


    /*** Debug Service Initialization Code ***/
    sysObj.sysDebug = SYS_DEBUG_Initialize(SYS_DEBUG_INDEX_0, (SYS_MODULE_INIT*)&debugInit);

    /*** Interrupt Service Initialization Code ***/
    SYS_INT_Initialize();

    /*Setup the INT_SOURCE_EXTERNAL_4 and Enable it*/
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT4, INT_PRIORITY_LEVEL3);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT4, INT_SUBPRIORITY_LEVEL1);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE4,INT_EDGE_TRIGGER_FALLING);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_4);






    /*** Random Service Initialization Code ***/
    SYS_RANDOM_Initialize(0, 0);

    /*** TMR Service Initialization Code ***/
    sysObj.sysTmr  = SYS_TMR_Initialize(SYS_TMR_INDEX_0, (const SYS_MODULE_INIT  * const)&sysTmrInitData);
  
    /* Initialize Middleware */
    sysObj.netPres = NET_PRES_Initialize(0, (SYS_MODULE_INIT*)&netPresInitData);

    /* set priority for ETHERNET interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_ETH, INT_PRIORITY_LEVEL5);

    /* set sub-priority for ETHERNET interrupt source */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_ETH, INT_SUBPRIORITY_LEVEL0);
    
    /* TCPIP Stack Initialization */
    sysObj.tcpip = TCPIP_STACK_Init();
    SYS_ASSERT(sysObj.tcpip != SYS_MODULE_OBJ_INVALID, "TCPIP_STACK_Init Failed" );

    /* Set priority of USB interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_USB1, INT_PRIORITY_LEVEL4);

    /* Set Sub-priority of USB interrupt source */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_USB1, INT_SUBPRIORITY_LEVEL0);


    /* Initialize the USB device layer */
    sysObj.usbDevObject0 = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData);

    /* Enable Global Interrupts */
    SYS_INT_Enable();
}


/*******************************************************************************
 End of File
*/

