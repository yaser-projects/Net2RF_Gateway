//******************************************************************************************************
//
// File Name    : Firmware ESP32
// Title        : Firmware interface header file
// software     : version 1.0
// Target MCU   : ESP32 Series
// Created:     : 04/03/1404
// Created:     : 25/05/2025
// Author:      : Yaser Rashnabady
//
//******************************************************************************************************
//======================================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Interface Header Library use Firmware <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//======================================================================================================
#include <Arduino.h>

#include <CC1101_RFToolkit.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

// #include <DNSServer.h>
#include <esp_wifi.h>
// #include <esp_netif.h>
#include <ETH.h>

#include "SPIFFS.h"
#include <EEPROMStorage.h>
#include <ArduinoJson.h>
#include "Button2.h"

#define BLUETOOTH false
#if BLUETOOTH
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#endif

#define WPS_ENABLE false
#if WPS_ENABLE
#include <esp_wps.h>
#endif

#define OTA_ENABLE true
#if OTA_ENABLE
#include <ElegantOTA.h>
#endif
//======================================================================================================
//  ----------------------------------------- Macro Setting ---------------------------------------------
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

#define MANUFACTURER "Metal Brain"
#define DEVICE__NAME "Praid"
#define MODEL_NUMBER "ES 131"
#define DEVICE_MODEL "MB-WBSR-V1.0"
#define PRODUCTION__DATE "2025/06"
#define FIRMWARE_VERSION "1.0"

#define Device_User "admin"
#define Device_Pass "admin"

#define Default_SSID "Metal Brain"
#define Default_PreSharedKey ""
#define Default_APHOSTNAME "MB_Device"

#define Default_IPv4 \
  {192, 168, 1, 2}
#define Default_IPv6 "2001:db8::1"
#define Default_Gateway \
  {192, 168, 1, 2}
#define Default_Subnet \
  {255, 255, 255, 0}
#define Default_PrimaryDNS \
  {192, 168, 1, 2}
#define Default_SecondaryDNS \
  {0, 0, 0, 0}

#define Default_Port 80

#define Wifi_Channel 1
#define Ssid_Hidden 0
#define Max_Connection 1

#define Default_ModemSSID ""
#define Default_ModemPreSharedKey ""
#define Default_Modem_WPS_PIN ""
#define Default_HOSTNAME "MB-WBSR"
#define Default_MODEMIP \
  {192, 168, 1, 20}

#if WPS_ENABLE
#define ESP_WPS_MODE WPS_TYPE_PBC 1 // WPS_TYPE_PBC = 1  WPS_TYPE_PIN = 2
#endif

#if BLUETOOTH
#define Default_BluetoothName "MB-WBSR"
#endif

#define SCANSIZE 9
#define WIFIINFO 3
#define WIFISSID 0
#define WIFIRSSI 1
#define WIFIENCRYPTION 2

#define DEBOUNCE 50
#define LONGCLICK 7000

#define BUTTON_DEF_ENABLE true
#define DEF_Button 34

#define BUTTON_AP__ENABLE true
#define AP__Button 35

#define BUTTON_STA_ENABLE false
#define STA_Button 25

#if WPS_ENABLE
#define BUTTON_WPS_ENABLE true
#define WPS_Button 26
#endif

#if BLUETOOTH
#define BUTTON_BTH_ENABLE true
#define BTH_Button 27
#endif

#define DEF_LED_ENABLE true
#define Def_LED 27

#define AP__LED_ENABLE true
#define AP__LED 14

#define STA_LED_ENABLE false
#define STA_LED 12

#if WPS_ENABLE
#define WPS_LED_ENABLE true
#define WPS_LED 04
#endif

#if BLUETOOTH
#define BTH_LED_ENABLE true
#define BTH_LED 05
#endif

#define SERIAL_ENABLE false
#define Serial_Baudraite 115200

#define OKMESSAGE "Succeed"
#define ERRORMESSAGE "Error"
//==================================================================================================
// --------------------------------------- Global Setting ------------------------------------------
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// =================================== IP Address details
IPAddress ipv4;
IPAddress gateway;
IPAddress subnet;
// =================================== Create AsyncWebServer object on port
// DNSServer dnsServer;
AsyncWebServer *server = nullptr;
AsyncWebSocket websocket("/ws");
// =================================== All buttonState
bool STA_____State = false;
bool AP______State = false;

#if WPS_ENABLE
bool WPS_____State = false;
#endif

#if BLUETOOTH
bool BTH_____State = false;
#endif
// =================================== Class initialize digital pin BUTTON input.
#if BUTTON_DEF_ENABLE
Button2 Defulte_btn(DEF_Button, INPUT_PULLUP, true);
#endif

#if BUTTON_STA_ENABLE
Button2 STA_____btn(STA_Button, INPUT_PULLUP, false);
#endif

#if BUTTON_AP__ENABLE
Button2 AP______btn(AP__Button, INPUT_PULLUP, false);
#endif

#if BUTTON_WPS_ENABLE
Button2 WPS_____btn(WPS_Button, INPUT_PULLUP, false);
#endif

#if BUTTON_BTH_ENABLE
Button2 BTH_____btn(BTH_Button, INPUT_PULLUP, false);
#endif
// =================================== Timer Value
hw_timer_t *timer = NULL;
// =================================== All Settting
struct IP4
{
  uint8_t field0;
  uint8_t field1;
  uint8_t field2;
  uint8_t field3;
};

struct mAC
{
  uint8_t field0;
  uint8_t field1;
  uint8_t field2;
  uint8_t field3;
  uint8_t field4;
  uint8_t field5;
};
struct IP6
{
  uint8_t field0;
  uint8_t field1;
  uint8_t field2;
  uint8_t field3;
  uint8_t field4;
  uint8_t field5;
  uint8_t field6;
  uint8_t field7;
  uint8_t field8;
  uint8_t field9;
  uint8_t field10;
  uint8_t field11;
  uint8_t field12;
  uint8_t field13;
  uint8_t field14;
  uint8_t field15;
};

struct AP__t
{
  char SSID[32];
  char PreSharedKey[64];
  char HostName[16];
  uint8_t WifiChannel;
  uint8_t SsidHidden;
  uint8_t MaxConnection;
  IP4 IPv4;
  IP6 IPv6;
  uint16_t Port;
  IP4 Gateway;
  IP4 Subnet;
  IP4 PrimaryDNS;
  IP4 SecondaryDNS;
  mAC MAC;
};

struct STA_t
{
  char SSID[32];
  char PreSharedKey[64];
  char WPS_PIN[9];
  char HostName[16];
  IP4 ModemIP;
  IP6 ModemIPV6;
  mAC ModemMAC;
  mAC STA_MAC;
  mAC ETHERNET_MAC;
};

#if BLUETOOTH
struct BTH_t
{
  char Name[16];
  mAC MAC;
};
#endif

struct settings_t
{
  char DeviceInfo[18];
  uint8_t Status;
  char Username[16];
  char Password[16];
  char DNS[32];
  mAC Board_BASE_MAC;
  AP__t AccessPoint;
  STA_t Station;
#if BLUETOOTH
  BTH_t Bluetooth;
#endif
  char Format_bit;
};

settings_t settings;
EEPROMStorage storage;
DynamicJsonDocument doc(1028);
DynamicJsonDocument res(1028);

String scanArray[WIFIINFO * SCANSIZE];
uint8_t scanSize;

uint8_t APstationNum;
String APstationArray[12];
uint8_t scanSize2;

#if WPS_ENABLE
enum class WPSMode
{
  PBC,
  PIN
};
#endif

enum class WiFiModeStatus : uint8_t
{
  STA_ENABLED = 0x01,
  AP_ENABLED = 0x02,
  WPS_ENABLED = 0x04,
  BTH_ENABLED = 0x08,

  STA_CONFIG = 0x10,
  AP_CONFIG = 0x20,
  WPS_CONFIG = 0x40,
  BTH_CONFIG = 0x80,
};
//==================================================================================================
// ----------------------------------------------------------------------------- Auxiliary functions
//==================================================================================================
// ----------------------- Timer Interrupt: Blink AP LED if config is not set ----------------------
#if AP__LED_ENABLE
void IRAM_ATTR onTimer()
{
  static bool state = 0;
  if ((settings.Status & static_cast<uint8_t>(WiFiModeStatus::AP_CONFIG)) == 0)
  {
    state = !state;
    digitalWrite(AP__LED, state);
  }
};
#endif
//==================================================================================================
// ---------------- Sends a message to all WebSocket clients + serial (if enabled) -----------------
void notifyClients(String message)
{
  websocket.textAll(message);
#if SERIAL_ENABLE
  Serial.println("Message sent:");
  Serial.println(message);
#endif
};
//==================================================================================================
// ---------------------------- Converts byte array to dot-separated string ------------------------
String Input(const uint8_t *data, size_t len)
{
  String str;
  for (size_t i = 0; i < len; i++)
  {
    str += String(data[i]);
    if (i < len - 1)
      str += ".";
  }
  return str;
  // Example: {192, 168, 1, 1}  → "192.168.1.1"
  // Example: {164, 59, 91, 84, 72, 12} → "164.59.91.84.72.12"
};
//==================================================================================================
// ----------------------- Converts 6-byte MAC to continuous decimal string ------------------------
String InputId(uint8_t *data_source)
{
  String str;
  for (uint8_t i = 0; i < 6; i++)
  {
    str += String(data_source[i]);
  }
  return str; //  Example: {164, 59, 91, 84, 72, 12} → "1645991847212"
};
//==================================================================================================
// ----------------------------- Updates a single status bit (persistent) --------------------------
void Mode(uint8_t status, bool state)
{
  storage.read();
  settings.Status &= ~status;
  if (state == true)
    settings.Status |= status;
  storage.write();
};
//==================================================================================================
// -------------------- Converts IPv6 structure to full and compressed string ----------------------
String ipv6ToString(IP6 &ip)
{
  // Full Notation
  char fullStr[40];
  sprintf(fullStr, "%x:%x:%x:%x:%x:%x:%x:%x",
          (ip.field0 << 8) | ip.field1,
          (ip.field2 << 8) | ip.field3,
          (ip.field4 << 8) | ip.field5,
          (ip.field6 << 8) | ip.field7,
          (ip.field8 << 8) | ip.field9,
          (ip.field10 << 8) | ip.field11,
          (ip.field12 << 8) | ip.field13,
          (ip.field14 << 8) | ip.field15);

  // Compressed Notation (ESP32 Arduino doesn't provide direct API, we build it manually)
  uint16_t blocks[8] = {
      (ip.field0 << 8) | ip.field1,
      (ip.field2 << 8) | ip.field3,
      (ip.field4 << 8) | ip.field5,
      (ip.field6 << 8) | ip.field7,
      (ip.field8 << 8) | ip.field9,
      (ip.field10 << 8) | ip.field11,
      (ip.field12 << 8) | ip.field13,
      (ip.field14 << 8) | ip.field15};

  // Compress zeros for :: notation
  int bestStart = -1, bestLen = 0;
  for (int i = 0; i < 8; i++)
  {
    if (blocks[i] == 0)
    {
      int len = 1;
      while (i + len < 8 && blocks[i + len] == 0)
        len++;
      if (len > bestLen)
      {
        bestStart = i;
        bestLen = len;
      }
      i += len;
    }
  }

  String compressedStr = "";
  for (int i = 0; i < 8;)
  {
    if (i == bestStart)
    {
      compressedStr += "::";
      i += bestLen;
      if (i >= 8)
        break;
    }
    else
    {
      compressedStr += String(blocks[i], HEX);
      if (i < 7)
        compressedStr += ":";
      i++;
    }
  }

  // Final formatted string
  String result;
  result += "Full IPv6: ";
  result += String(fullStr);
  result += "\r\nCompressed IPv6: ";
  result += compressedStr;
  return result;
};
//==================================================================================================
// --------------------------- Converts string IPv6 to IP6 structure -------------------------------
bool parseIPv6(const char *ipv6Str, IP6 &out)
{
  uint16_t blocks[8] = {0};
  int blockIndex = 0;
  bool doubleColonUsed = false;

  String ip = String(ipv6Str);
  int doubleColonPos = ip.indexOf("::");

  std::vector<String> parts;
  if (doubleColonPos >= 0)
  {
    doubleColonUsed = true;

    String firstPart = ip.substring(0, doubleColonPos);
    String secondPart = ip.substring(doubleColonPos + 2);

    int count1 = 0, count2 = 0;
    if (firstPart.length())
    {
      int start = 0;
      while (true)
      {
        int end = firstPart.indexOf(':', start);
        if (end < 0)
          end = firstPart.length();
        parts.push_back(firstPart.substring(start, end));
        count1++;
        if (end == firstPart.length())
          break;
        start = end + 1;
      }
    }

    parts.push_back("");

    if (secondPart.length())
    {
      int start = 0;
      while (true)
      {
        int end = secondPart.indexOf(':', start);
        if (end < 0)
          end = secondPart.length();
        parts.push_back(secondPart.substring(start, end));
        count2++;
        if (end == secondPart.length())
          break;
        start = end + 1;
      }
    }

    while (parts.size() < 8)
      parts.insert(parts.begin() + count1, "0");
  }
  else
  {

    int start = 0;
    while (true)
    {
      int end = ip.indexOf(':', start);
      if (end < 0)
        end = ip.length();
      parts.push_back(ip.substring(start, end));
      if (end == ip.length())
        break;
      start = end + 1;
    }
  }

  if (parts.size() != 8)
    return false;

  for (int i = 0; i < 8; i++)
  {
    blocks[i] = (uint16_t)strtol(parts[i].c_str(), nullptr, 16);
  }

  uint8_t *target = reinterpret_cast<uint8_t *>(&out);
  for (int i = 0; i < 8; i++)
  {
    target[i * 2] = blocks[i] >> 8;
    target[i * 2 + 1] = blocks[i] & 0xFF;
  }

  return true;
};
//==================================================================================================
// ------------------------ Converts MAC address to standard string format -------------------------
String macToString(const uint8_t *mac)
{
  char str[18];
  sprintf(str, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(str); // Example: {0xA4, 0x3B, 0x4F, 0x12, 0x5C, 0x01} → "A4:3B:4F:12:5C:01"
};

String macToString(const mAC &mac)
{
  char str[18];
  snprintf(str, sizeof(str), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac.field0, mac.field1, mac.field2,
           mac.field3, mac.field4, mac.field5);
  return String(str);
};
//==================================================================================================
// ----------------------------- Copies IPAddress to custom IP4 struct -----------------------------
void copyIP(IPAddress src, IP4 &dst)
{
  dst.field0 = src[0];
  dst.field1 = src[1];
  dst.field2 = src[2];
  dst.field3 = src[3];
};
//==================================================================================================
// ---------------------- Convert string MAC "88:36:6C:12:AB:34" to mAC struct ---------------------
void parseMacStrToStruct(const char *macStr, mAC &macStruct)
{
  uint8_t values[6];
  int result = sscanf(macStr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                      &values[0], &values[1], &values[2],
                      &values[3], &values[4], &values[5]);
  if (result == 6)
  {
    macStruct.field0 = values[0];
    macStruct.field1 = values[1];
    macStruct.field2 = values[2];
    macStruct.field3 = values[3];
    macStruct.field4 = values[4];
    macStruct.field5 = values[5];
  }
};
//==================================================================================================
// -------------------------- Converts WiFi disconnect reason code to string -----------------------
String reasonToString(uint8_t reason)
{
  switch (reason)
  {
  case 1:
    return "Unspecified";
  case 2:
    return "Auth Expired";
  case 3:
    return "Auth Leave";
  case 4:
    return "Assoc Expired";
  case 8:
    return "Deauth Inactivity";
  case 201:
    return "No AP Found";
  case 202:
    return "Wrong Password";
  case 203:
    return "Assoc Failed";
  default:
    return "Unknown Reason (" + String(reason) + ")";
  }
};
//==================================================================================================
// -------------------------- Converts WiFi auth mode enum to readable string ----------------------
String authModeToString(uint8_t mode)
{
  switch (mode)
  {
  case WIFI_AUTH_OPEN:
    return "Open";
  case WIFI_AUTH_WEP:
    return "WEP";
  case WIFI_AUTH_WPA_PSK:
    return "WPA-PSK";
  case WIFI_AUTH_WPA2_PSK:
    return "WPA2-PSK";
  case WIFI_AUTH_WPA_WPA2_PSK:
    return "WPA/WPA2-PSK";
  case WIFI_AUTH_WPA2_ENTERPRISE:
    return "WPA2-Enterprise";
  default:
    return "Unknown (" + String(mode) + ")";
  }
};
//==================================================================================================
// ------------------------ Initializes WPS configuration with custom branding ---------------------
#if WPS_ENABLE
void initWPSConfig(esp_wps_config_t *config, wps_type_t type)
{
  memset(config, 0, sizeof(esp_wps_config_t));
  config->wps_type = type;

  strncpy((char *)config->factory_info.manufacturer, MANUFACTURER, sizeof(config->factory_info.manufacturer));
  strncpy((char *)config->factory_info.model_number, MODEL_NUMBER, sizeof(config->factory_info.model_number));
  strncpy((char *)config->factory_info.model_name, DEVICE_MODEL, sizeof(config->factory_info.model_name));
  strncpy((char *)config->factory_info.device_name, DEVICE__NAME, sizeof(config->factory_info.device_name));
};
#endif
//==================================================================================================
// --------------------------------------------- Set MAC -------------------------------------------
void setMAC(mAC &macField, esp_mac_type_t type)
{
  uint8_t buffer[6];
  esp_read_mac(buffer, type);

  macField.field0 = buffer[0];
  macField.field1 = buffer[1];
  macField.field2 = buffer[2];
  macField.field3 = buffer[3];
  macField.field4 = buffer[4];
  macField.field5 = buffer[5];
};
//==================================================================================================
// ----------------------- Check if specific WiFi mode/status bit is active ------------------------
inline bool hasFlag(WiFiModeStatus flag)
{
  return settings.Status & static_cast<uint8_t>(flag);
};
//==================================================================================================
// -------------------------------------------------------------------- Function Code Use WiFi Event
//==================================================================================================
void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
  notifyClients("[WiFi-event] event:" + String((int)event));
  String Message;

  switch (event)
  {
  case ARDUINO_EVENT_WIFI_READY:
    Message = "WiFi interface ready\r\n";
    break;

  case ARDUINO_EVENT_WIFI_SCAN_DONE:
    Message = "Completed scan for access points\r\n";
    break;

  case ARDUINO_EVENT_WIFI_STA_START:
    Message = "WiFi client started\r\n";
#if STA_LED_ENABLE
    digitalWrite(STA_LED, HIGH);
#endif
    break;

  case ARDUINO_EVENT_WIFI_STA_STOP:
    Message = "WiFi client stopped\r\n";
#if STA_LED_ENABLE
    digitalWrite(STA_LED, LOW);
#endif
    break;

  case ARDUINO_EVENT_WIFI_STA_CONNECTED:
    parseMacStrToStruct(WiFi.BSSIDstr().c_str(), settings.Station.ModemMAC);

    Message = "Connected Form WIFI to Modem\r\n";
    Message += "Modem SSID: " + WiFi.SSID() + "\r\n";
    Message += "Modem Pre Shared Key: " + WiFi.psk() + "\r\n";
    Message += "Station MAC Address = " + WiFi.macAddress() + "\r\n";
    Message += "Station Hostname = " + String(WiFi.getHostname()) + "\r\n";
    Message += "Modem BSSID = " + macToString(settings.Station.ModemMAC) + "\r\n";
    // Message += "Modem BSSID = " + WiFi.BSSIDstr() + "\r\n";
    Message += "Mode: " + String(WiFi.getMode()) + "\r\n";
#if STA_LED_ENABLE
    digitalWrite(STA_LED, HIGH);
#endif
    break;

  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    Message = "Disconnected from WiFi modem\r\n";
    Message += "WiFi Mode: " + String(WiFi.getMode()) + "\r\n";
    Message += "Reason code: " + reasonToString(info.wifi_sta_disconnected.reason) + "\r\n";
#if STA_LED_ENABLE
    digitalWrite(STA_LED, LOW);
    delay(500);
    digitalWrite(STA_LED, HIGH);
#endif
    break;

  case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
    Message = "Access point authentication mode changed\r\n";
    Message += "Old Mode: " + authModeToString(info.wifi_sta_authmode_change.old_mode) + "\r\n";
    Message += "New Mode: " + authModeToString(info.wifi_sta_authmode_change.new_mode) + "\r\n";
    break;

  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    copyIP(WiFi.localIP(), settings.Station.ModemIP);
    storage.write();
    Message = "Obtained IP address: " + WiFi.localIP().toString() + "\r\n";
    break;

  case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
  {
    const uint8_t *src = (const uint8_t *)info.got_ip6.ip6_info.ip.addr;
    memcpy(&settings.Station.ModemIPV6, src, 16);
    storage.write();
    Message = "STA IPv6 preferred: " + ipv6ToString(settings.Station.ModemIPV6) + "\r\n";
    break;
  }
  case ARDUINO_EVENT_WIFI_STA_LOST_IP:
    Message = "Lost IP address. Reset to 0.0.0.0\r\n";

    settings.Station.ModemIP.field0 = 0;
    settings.Station.ModemIP.field1 = 0;
    settings.Station.ModemIP.field2 = 0;
    settings.Station.ModemIP.field3 = 0;
    storage.write();
#if STA_LED_ENABLE
    digitalWrite(STA_LED, LOW);
#endif
    break;

  case ARDUINO_EVENT_WIFI_AP_START:
    Message = "=== AP Mode Started ===\r\n";
    Message += "Hostname: " + String(WiFi.softAPgetHostname()) + "\r\n";
    Message += "SSID: " + WiFi.softAPSSID() + "\r\n";
    Message += "Password: " + String(settings.AccessPoint.PreSharedKey) + "\r\n";
    Message += "IP Address: " + WiFi.softAPIP().toString() + "\r\n";
    Message += "MAC Address: " + WiFi.softAPmacAddress() + "\r\n";
    Message += "Current Mode: " + String(WiFi.getMode()) + "\r\n";
#if AP__LED_ENABLE
    digitalWrite(AP__LED, HIGH);
#endif
    break;

  case ARDUINO_EVENT_WIFI_AP_STOP:
    Message = "=== AP Mode Stopped ===\r\n";
    Message += "Current Mode: " + String(WiFi.getMode()) + "\r\n";
#if AP__LED_ENABLE
    digitalWrite(AP__LED, LOW);
#endif
    break;

  case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
  {
    APstationNum = WiFi.softAPgetStationNum();

    wifi_sta_list_t staList;
    tcpip_adapter_sta_list_t ipList;

    if (esp_wifi_ap_get_sta_list(&staList) == ESP_OK &&
        tcpip_adapter_get_sta_list(&staList, &ipList) == ESP_OK)
    {
      Message = "Client connected\r\nTotal: " + String(APstationNum) + "\r\n";

      for (int i = 0; i < ipList.num; i++)
      {
        String macStr = macToString(ipList.sta[i].mac);
        IPAddress ipAddr(ipList.sta[i].ip.addr);

        Message += "Client #" + String(i + 1) + ":\r\n";
        Message += "  MAC: " + macStr + "\r\n";
        Message += "  IP : " + ipAddr.toString() + "\r\n";
      }
    }
    else
    {
      Message = "Client connected but failed to get IP list\r\n";
    }
    break;
  }

  case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
    Message = "Client disconnected\r\n";
    Message += "MAC: " + macToString(info.wifi_ap_stadisconnected.mac) + "\r\n";
    APstationNum = WiFi.softAPgetStationNum();
    Message += "Remaining clients: " + String(APstationNum) + "\r\n";
    break;

  case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
  {
    wifi_sta_list_t staList;
    tcpip_adapter_sta_list_t adapterList;

    if (esp_wifi_ap_get_sta_list(&staList) == ESP_OK &&
        tcpip_adapter_get_sta_list(&staList, &adapterList) == ESP_OK)
    {
      Message = "Client assigned IP\r\n";
      Message += "Connected Stations: " + String(adapterList.num) + "\r\n";

      for (int i = 0; i < adapterList.num; i++)
      {
        Message += "Client " + String(i + 1) + " → IP: " + IPAddress(adapterList.sta[i].ip.addr).toString();
        Message += ", MAC: " + macToString(adapterList.sta[i].mac) + "\r\n";

        APstationArray[(i * 3) + 0] = "Client " + String(i + 1);
        APstationArray[(i * 3) + 1] = IPAddress(adapterList.sta[i].ip.addr).toString();
        APstationArray[(i * 3) + 2] = macToString(adapterList.sta[i].mac);
      }
    }
    else
    {
      Message = "Error: Failed to get station list\r\n";
    }
    break;
  }

  case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
    Message = "Probe request received\r\n";
    Message += "MAC or RSSI info not available in this core version\r\n";
    break;

  case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
  {
    const uint8_t *src = (const uint8_t *)info.got_ip6.ip6_info.ip.addr;
    memcpy(&settings.AccessPoint.IPv6, src, 16);
    storage.write();
    Message = "AP IPv6 is preferred: " + ipv6ToString(settings.AccessPoint.IPv6) + "\r\n";
    break;
  }

  case ARDUINO_EVENT_WIFI_FTM_REPORT:
    Message = "FTM report received (Fine Time Measurement)\r\n";
    break;

  case ARDUINO_EVENT_ETH_START:
    Message = "Ethernet interface started\r\n";
    break;

  case ARDUINO_EVENT_ETH_STOP:
    Message = "Ethernet interface stopped\r\n";
    break;

  case ARDUINO_EVENT_ETH_CONNECTED:
    Message = "Ethernet connected (Link UP, waiting for IP...)\r\n";
    break;

  case ARDUINO_EVENT_ETH_DISCONNECTED:
    Message = "Ethernet disconnected (Link DOWN)\r\n";
    break;

  case ARDUINO_EVENT_ETH_GOT_IP:
  {
    if (ETH.linkUp())
    {
      IPAddress ip = ETH.localIP();
      Message = "Ethernet IP Address: " + ip.toString() + "\r\n";
      break;
    }
    else
    {
      Message = "Ethernet not connected\r\n";
      break;
    }
  }

  case ARDUINO_EVENT_ETH_GOT_IP6:
  {
    const uint8_t *src = (const uint8_t *)info.got_ip6.ip6_info.ip.addr;
    IP6 ipv6;
    memcpy(&ipv6, src, 16);

    Message = "Ethernet IPv6 obtained: " + ipv6ToString(ipv6) + "\r\n";
    break;
  }

#if WPS_ENABLE
  case ARDUINO_EVENT_WPS_ER_SUCCESS:
  {
    esp_wifi_wps_disable();
    delay(10);

    String ssid = WiFi.SSID();
    String psk = WiFi.psk();
    String mac = WiFi.macAddress();
    String hostname = WiFi.getHostname();

    strlcpy(settings.Station.SSID, ssid.c_str(), sizeof(settings.Station.SSID));
    strlcpy(settings.Station.PreSharedKey, psk.c_str(), sizeof(settings.Station.PreSharedKey));
    strlcpy(settings.Station.HostName, hostname.c_str(), sizeof(settings.Station.HostName));
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::STA_ENABLED) |
                       static_cast<uint8_t>(WiFiModeStatus::STA_CONFIG);
    storage.write();

#if WPS_LED_ENABLE
    digitalWrite(WPS_LED, HIGH);
#endif

    WiFi.begin();

    Message = "WPS Success!\r\n";
    Message += "SSID: " + ssid + "\r\n";
    Message += "Pre-Shared Key: " + psk + "\r\n";
    Message += "MAC: " + mac + "\r\n";
    Message += "Hostname: " + hostname + "\r\n";
    Message += "Connecting...\r\n";
    break;
  }

  case ARDUINO_EVENT_WPS_ER_FAILED:
  {
    esp_wifi_wps_disable();
    delay(10);

    Message = "WPS Failed!\r\nRetrying...\r\n";
    notifyClients(Message);

    esp_wps_config_t config;
    initWPSConfig(&config, WPS_TYPE_PBC);

    esp_wifi_wps_enable(&config);
    esp_wifi_wps_start(0);

#if WPS_LED_ENABLE
    digitalWrite(WPS_LED, LOW);
#endif
    break;
  }

  case ARDUINO_EVENT_WPS_ER_TIMEOUT:
  {
    esp_wifi_wps_disable();
    delay(10);

    Message = "WPS Timeout!\r\nRetrying...\r\n";
    notifyClients(Message);

    esp_wps_config_t config;
    initWPSConfig(&config, WPS_TYPE_PBC);

    esp_wifi_wps_enable(&config);
    esp_wifi_wps_start(0);

#if WPS_LED_ENABLE
    digitalWrite(WPS_LED, LOW);
#endif
    break;
  }

  case ARDUINO_EVENT_WPS_ER_PIN:
  {
    char pinStr[9]; // 8 digits + null terminator
    sprintf(pinStr, "%08u", info.wps_er_pin.pin_code);

    Message = "WPS PIN Mode Active\r\n";
    Message += "Generated WPS PIN: " + String(pinStr) + "\r\n";

    notifyClients(Message);

#if WPS_LED_ENABLE
    digitalWrite(WPS_LED, HIGH);
#endif
    break;
  }
#endif

  default:
    Message = "Unhandled event: " + String((int)event) + "\r\n";
    break;
  }
  notifyClients(Message);
};
//==================================================================================================
// ------------------------------- Function Code Use Server Handler --------------------------------
void ServerNewHandler(void)
{
  if (server != nullptr)
  {
    server->end();
    delete server;
    server = nullptr;
  }

  server = new AsyncWebServer(settings.AccessPoint.Port);
  server->addHandler(&websocket);

  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

#if OTA_ENABLE
  if (strlen(settings.Username) > 0 && strlen(settings.Password) > 0)
  {
    ElegantOTA.begin(server, settings.Username, settings.Password);
  }
  else
  {
    notifyClients("⚠️ OTA not enabled: username or password is empty");
  }
#endif

  server->begin();
  notifyClients("✅ HTTP server started on port " + String(settings.AccessPoint.Port));
};
//=================================================================================================
// ---------------------------------------------------------------------------------- Config Default
//==================================================================================================
void Default()
{
  uint8_t buffer_MAC[6];
  // Mode(static_cast<uint8_t>(WiFiModeStatus::AP_CONFIG), false);
  // Mode(static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED), true);

  settings.Status = 0x02;

#if BLUETOOTH
  settings.Status |= 0x88;
  // Mode(static_cast<uint8_t>(WiFiModeStatus::BTH_ENABLED), true);
  // Mode(static_cast<uint8_t>(WiFiModeStatus::BTH_CONFIG), true);
#endif

  strlcpy(settings.Username, Device_User, sizeof(settings.Username));
  strlcpy(settings.Password, Device_Pass, sizeof(settings.Password));

  strlcpy(settings.AccessPoint.SSID, Default_SSID, sizeof(settings.AccessPoint.SSID));
  strlcpy(settings.AccessPoint.PreSharedKey, Default_PreSharedKey, sizeof(settings.AccessPoint.PreSharedKey));
  strlcpy(settings.AccessPoint.HostName, Default_APHOSTNAME, sizeof(settings.AccessPoint.HostName));

  settings.AccessPoint.WifiChannel = Wifi_Channel;
  settings.AccessPoint.SsidHidden = Ssid_Hidden;
  settings.AccessPoint.MaxConnection = Max_Connection;
  settings.AccessPoint.Port = Default_Port;

  settings.AccessPoint.IPv4 = Default_IPv4;
  parseIPv6(Default_IPv6, settings.AccessPoint.IPv6);
  settings.AccessPoint.Gateway = Default_Gateway;
  settings.AccessPoint.Subnet = Default_Subnet;
  settings.AccessPoint.PrimaryDNS = Default_PrimaryDNS;
  settings.AccessPoint.SecondaryDNS = Default_SecondaryDNS;

  esp_efuse_mac_get_default(buffer_MAC);
  settings.Board_BASE_MAC = {buffer_MAC[0],
                             buffer_MAC[1],
                             buffer_MAC[2],
                             buffer_MAC[3],
                             buffer_MAC[4],
                             buffer_MAC[5]};

  strlcpy(settings.DeviceInfo, InputId(buffer_MAC).c_str(), sizeof(settings.DeviceInfo));

  setMAC(settings.AccessPoint.MAC, ESP_MAC_WIFI_SOFTAP);
  setMAC(settings.Station.STA_MAC, ESP_MAC_WIFI_STA);
  setMAC(settings.Station.ETHERNET_MAC, ESP_MAC_ETH);

  strlcpy(settings.Station.SSID, Default_ModemSSID, sizeof(settings.Station.SSID));
  strlcpy(settings.Station.PreSharedKey, Default_ModemPreSharedKey, sizeof(settings.Station.PreSharedKey));
  strlcpy(settings.Station.WPS_PIN, Default_Modem_WPS_PIN, sizeof(settings.Station.WPS_PIN));
  strlcpy(settings.Station.HostName, Default_HOSTNAME, sizeof(settings.Station.HostName));
  settings.Station.ModemIP = Default_MODEMIP;

#if BLUETOOTH
  strlcpy(settings.Bluetooth.Name, Default_BluetoothName, sizeof(settings.Bluetooth.Name));
  setMAC(settings.Bluetooth.MAC, ESP_MAC_BT);
#endif

#if DEF_LED_ENABLE
  digitalWrite(Def_LED, LOW);
#endif

#if AP__LED_ENABLE
  digitalWrite(AP__LED, LOW);
#endif

#if STA_LED_ENABLE
  digitalWrite(STA_LED, LOW);
#endif

#if WPS_LED_ENABLE
  digitalWrite(WPS_LED, LOW);
#endif

#if BTH_LED_ENABLE
  digitalWrite(BTH_LED, LOW);
#endif

  storage.write();
  String message = "Device Set Default\nRestart Device\r\n";
  notifyClients(message);
  delay(1000);
  ESP.restart();
};
//==================================================================================================
// ---------------------------- Configure Station Mode (WiFi to Modem) -----------------------------
void Config_ST(void)
{
  // خاموش کردن WiFi و پاکسازی کامل استک
  // WiFi.mode(WIFI_OFF);
  // delay(500);
  esp_wifi_deinit();
  delay(500);

  WiFi.mode(WIFI_STA);
  delay(300);

  WiFi.disconnect(true, true); // دیسکانکت کامل
  delay(200);

  WiFi.setHostname(settings.Station.HostName);
  WiFi.begin(settings.Station.SSID, settings.Station.PreSharedKey);
  WiFi.reconnect();

  delay(300);
 // ServerNewHandler();
};
//==================================================================================================
// ----------------------------- Configure Access Point Mode (SoftAP) ------------------------------
void Config_AP(void)
{
  // WiFi.mode(WIFI_OFF);
  // delay(500);
  esp_wifi_deinit(); // پاکسازی کامل استک WiFi
  delay(500);

  WiFi.mode(WIFI_AP);
  delay(300);

  WiFi.softAPdisconnect(true);

  ipv4.fromString(Input(&settings.AccessPoint.IPv4.field0, 4));
  gateway.fromString(Input(&settings.AccessPoint.Gateway.field0, 4));
  subnet.fromString(Input(&settings.AccessPoint.Subnet.field0, 4));

  WiFi.softAPsetHostname(settings.AccessPoint.HostName);
  WiFi.softAPConfig(ipv4, gateway, subnet);
  esp_wifi_set_mac(WIFI_IF_AP, &settings.AccessPoint.MAC.field0);

  WiFi.softAP(settings.AccessPoint.SSID,
              settings.AccessPoint.PreSharedKey,
              settings.AccessPoint.WifiChannel,
              settings.AccessPoint.SsidHidden,
              settings.AccessPoint.MaxConnection);

  delay(300); // بعد از راه‌اندازی هم یه تاخیر کوتاه
  //ServerNewHandler();
};
//==================================================================================================
// ---------------------------- Configure Dual Mode (Access Point + Station) ------------------------
void Config_AP_STA(void)
{
  // خاموش کردن WiFi و پاکسازی کامل استک
  // WiFi.mode(WIFI_OFF);
  // delay(500);
  esp_wifi_deinit();
  delay(500);

  WiFi.mode(WIFI_AP_STA);
  delay(300);

  // ------------------- Access Point -------------------
  ipv4.fromString(Input(&settings.AccessPoint.IPv4.field0, 4));
  gateway.fromString(Input(&settings.AccessPoint.Gateway.field0, 4));
  subnet.fromString(Input(&settings.AccessPoint.Subnet.field0, 4));

  WiFi.softAPsetHostname(settings.AccessPoint.HostName);
  WiFi.softAPConfig(ipv4, gateway, subnet);
  esp_wifi_set_mac(WIFI_IF_AP, &settings.AccessPoint.MAC.field0);

  WiFi.softAP(settings.AccessPoint.SSID,
              settings.AccessPoint.PreSharedKey,
              settings.AccessPoint.WifiChannel,
              settings.AccessPoint.SsidHidden,
              settings.AccessPoint.MaxConnection);

  delay(300);

  // ------------------- Station -------------------
  WiFi.disconnect(true, true); // Disconnect previous station (if any)
  delay(200);

  WiFi.setHostname(settings.Station.HostName);
  WiFi.begin(settings.Station.SSID, settings.Station.PreSharedKey);
  WiFi.reconnect();

  delay(300);
  //ServerNewHandler();
};
//==================================================================================================
// --------------------------------- Config Mode Start WPS With PBC --------------------------------
#if WPS_ENABLE
void StartWPS_WithPBC()
{
  WiFi.mode(WIFI_STA);
  delay(250);

  WiFi.disconnect(true, true);
  esp_wifi_wps_disable();
  delay(10);

  esp_wps_config_t config;
  initWPSConfig(&config, WPS_TYPE_PBC);

  esp_wifi_wps_enable(&config);
  esp_wifi_wps_start(0);

#if WPS_LED_ENABLE
  digitalWrite(WPS_LED, HIGH);
#endif

  Message = "WPS started. Press the WPS button on your modem...\r\n";
  notifyClients(Message);
};
//==================================================================================================
// --------------------------------- Config Mode Start WPS With PIN --------------------------------
void StartWPS_WithPIN(const char *pin)
{
  WiFi.mode(WIFI_STA);
  delay(250);

  WiFi.disconnect(true, true);
  esp_wifi_wps_disable();
  delay(10);

  esp_wps_config_t config;
  initWPSConfig(&config, WPS_TYPE_PIN);

  esp_err_t err = esp_wifi_wps_enable(&config);

  if (err == ESP_OK)
  {
    // ⚠️ This function does NOT exist in Arduino Core (only in ESP-IDF)
    // esp_wifi_wps_start_with_pin(pin);

    Message = "WPS with PIN mode initialized.\r\n";
    Message += "Entered PIN: ";
    Message += pin;
    Message += "\r\nNote: Actual PIN-based WPS connection is only supported in ESP-IDF.";
  }
  else
  {
    Message = "WPS with PIN failed to initialize.\r\nError code: " + String(err);
  }

  notifyClients(Message);
};
//==================================================================================================
// ------------------------------------ StartWPS Function ------------------------------------------
void StartWPS(WPSMode mode, const char *pin = nullptr)
{
  WiFi.mode(WIFI_STA);
  delay(250);
  WiFi.disconnect(true, true);
  esp_wifi_wps_disable();
  delay(10);

  esp_wps_config_t config;
  initWPSConfig(&config, (mode == WPSMode::PBC) ? WPS_TYPE_PBC : WPS_TYPE_PIN);

  esp_err_t err = esp_wifi_wps_enable(&config);

  if (err != ESP_OK)
  {
    Message = "WPS failed to initialize.\r\nError code: " + String(err);
    notifyClients(Message);
    return;
  }

  if (mode == WPSMode::PBC)
  {
    esp_wifi_wps_start(0);
    Message = "WPS (PBC) started.\r\nPress the WPS button on your modem.";
  }
  else if (mode == WPSMode::PIN)
  {
    Message = "WPS (PIN) mode initialized.\r\nPIN: ";
    Message += (pin ? pin : "(none)");
    Message += "\r\n⚠️ WPS with PIN is only supported in ESP-IDF.\r\n";

    // Arduino Core doesn’t support this:
    // esp_wifi_wps_start_with_pin(pin);
  }

#if WPS_LED_ENABLE
  digitalWrite(WPS_LED, HIGH);
#endif

  notifyClients(Message);
};
#endif
//==================================================================================================
// --------------------------- Initialize Bluetooth Mode (Placeholder) -----------------------------
#if BLUETOOTH
void BluetoothInit()
{
  if (!BTH_____State)
  {
    Message = "🔵 Bluetooth is disabled. Skipping initialization...\r\n";
    notifyClients(Message);
    return;
  }

  if (!SerialBT.begin(settings.Bluetooth.Name))
  {
    Message = "❌ Failed to initialize Bluetooth Serial!\r\n";
  }
  else
  {
    Message = "✅ Bluetooth Serial started successfully\r\n";
    Message += "Device Name: ";
    Message += settings.Bluetooth.Name;
    Message += "\r\n";
  }
  notifyClients(Message);
}
#endif
//==================================================================================================
// ------------------------------------------ Configuring -----------------------------------------
void Config(void)
{
  storage.read();
  static bool JustOnce = true;
  String message;

  // State Log (for debug)
  // message = "\r\nsettings.Status: " + String(settings.Status);
  // message += "\r\nAP_ENABLED: " + String(hasFlag(WiFiModeStatus::AP_ENABLED));
  // message += "\r\nAP_CONFIG: " + String(hasFlag(WiFiModeStatus::AP_CONFIG));
  // message += "\r\nSTA_ENABLED: " + String(hasFlag(WiFiModeStatus::STA_ENABLED));
  // message += "\r\nSTA_CONFIG: " + String(hasFlag(WiFiModeStatus::STA_CONFIG));

  uint8_t state = settings.Status & 0x33;

  switch (state)
  {
  // Row 1
  case 0x00:
    AP______State = true;
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED);
    message += "\r\nDisabling both Access Point and Station is not allowed.";
    break;

  // Row 2
  case 0x02:
    Config_AP();
#if AP__LED_ENABLE
    digitalWrite(AP__LED, HIGH);
#endif
#if STA_LED_ENABLE
    digitalWrite(STA_LED, LOW);
#endif
    message += "\r\nAccess Point activated with temporary settings (SSID or password not set).";
    break;

  // Row 3
  case 0x20:
    AP______State = true;
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED);
    message += "\r\nDisabling Access Point is not allowed.";
    break;

  // Row 4
  case 0x22:
    Config_AP();
#if AP__LED_ENABLE
    digitalWrite(AP__LED, HIGH);
#endif
#if STA_LED_ENABLE
    digitalWrite(STA_LED, LOW);
#endif
    message += "\r\nAccess Point configured and activated successfully.";
    break;

  // Row 5
  case 0x10:
    AP______State = true;
    STA_____State = true;
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED);
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::STA_ENABLED);
    message += "\r\nDisabling Station is not allowed.";
    break;

  // Row 6
  case 0x12:
    STA_____State = true;
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED);
    message += "\r\nDisabling Station is not allowed; Access Point activated with temporary settings.";
    break;

  // Row 7
  case 0x30:
    AP______State = true;
    STA_____State = true;
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED);
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::STA_ENABLED);
    message += "\r\nDisabling both Access Point and Station is not allowed.";
    break;

  // Row 8
  case 0x32:
    Config_AP();
#if AP__LED_ENABLE
    digitalWrite(AP__LED, HIGH);
#endif
#if STA_LED_ENABLE
    digitalWrite(STA_LED, LOW);
#endif
    message += "\r\nStation disabled; Access Point configured and activated.";
    break;

  // Row 9
  case 0x01:
    AP______State = true;
    STA_____State = false;
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED);
    settings.Status &= ~static_cast<uint8_t>(WiFiModeStatus::STA_ENABLED);
    message += "\r\nDisabling Access Point is not allowed and Station is not configured yet.";
    break;

  // Row 10
  case 0x03:
    STA_____State = false;
    settings.Status &= ~static_cast<uint8_t>(WiFiModeStatus::STA_ENABLED);
    message += "\r\nEnabling Station is not allowed; configuration is incomplete.";
    break;

  // Row 11
  case 0x21:
    AP______State = true;
    STA_____State = false;
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED);
    settings.Status &= ~static_cast<uint8_t>(WiFiModeStatus::STA_ENABLED);
    message += "\r\nDisabling Access Point is not allowed and Station is not configured yet.";
    break;

  // Row 12
  case 0x23:
    STA_____State = false;
    settings.Status &= ~static_cast<uint8_t>(WiFiModeStatus::STA_ENABLED);
    message += "\r\nEnabling Station is not allowed; configuration is incomplete.";
    break;

  // Row 13
  case 0x11:
    AP______State = true;
    settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED);
    message += "\r\nDisabling Access Point is not allowed.";
    break;

  // Row 14
  case 0x13:
    Config_AP_STA();
#if AP__LED_ENABLE
    digitalWrite(AP__LED, HIGH);
#endif
#if STA_LED_ENABLE
    digitalWrite(STA_LED, HIGH);
#endif
    message += "\r\nStation configured and activated; Access Point activated with temporary settings.";
    break;

  // Row 15
  case 0x31:
    Config_ST();
#if AP__LED_ENABLE
    digitalWrite(AP__LED, LOW);
#endif
#if STA_LED_ENABLE
    digitalWrite(STA_LED, HIGH);
#endif
    message += "\r\nAccess Point disabled; Station configured and activated.";
    break;

  // Row 16
  case 0x33:
    Config_AP_STA();
#if AP__LED_ENABLE
    digitalWrite(AP__LED, HIGH);
#endif
#if STA_LED_ENABLE
    digitalWrite(STA_LED, HIGH);
#endif
    message += "\r\nAccess Point and Station configured and activated successfully.";
    break;

  default:
    message += "\r\nUnknown WiFi mode configuration!";
    break;
  }

  // ---- WPS
#if WPS_ENABLE
  if (hasFlag(WiFiModeStatus::WPS_ENABLED))
  {
    WPS_____State = true;
    StartWPS(WPSMode::PBC);
    message += "\r\nWPS Mode: Starting WPS (PBC)";
  }
  else
  {
    WPS_____State = false;
#if WPS_LED_ENABLE
    digitalWrite(WPS_LED, LOW);
#endif
  }
#endif

  // ---- Bluetooth
#if BLUETOOTH
  if (hasFlag(WiFiModeStatus::BTH_ENABLED))
  {
    BTH_____State = true;
    message += "\r\nBluetooth Mode: Enabled";
  }
  else
  {
    BTH_____State = false;
#if BTH_LED_ENABLE
    digitalWrite(BTH_LED, LOW);
#endif
  }
#endif

  storage.write();
  notifyClients(message);
};
//==================================================================================================
// ----------- Handle long-press on Default button: Resets all settings to factory defaults --------
#if BUTTON_DEF_ENABLE
void handleDefaultButton(Button2 &btn)
{
  String message = "🛠️ Default button pressed - resetting to factory settings...\r\n";
  notifyClients(message);
  Default();
};
#endif
//==================================================================================================
// ---------------------------------- Setting of the station status --------------------------------
void Cmd_Config_STA(void)
{
  STA_____State = !STA_____State;
  Mode(static_cast<uint8_t>(WiFiModeStatus::STA_ENABLED), STA_____State);
  Config();
};
//==================================================================================================
// -------- Handle tap on Station button: Toggles STA (WiFi to Modem) mode and reconfigures --------
#if BUTTON_STA_ENABLE
void handleStationButton(Button2 &btn)
{
  String message = "🌐 Station button tapped - toggling modem mode...\r\n";
  notifyClients(message);
  Cmd_Config_STA();
};
#endif
//==================================================================================================
// ---------------------------------- Setting of the access point ----------------------------------
void Cmd_Config_AP(void)
{
  AP______State = !AP______State;
  Mode(static_cast<uint8_t>(WiFiModeStatus::AP_ENABLED), AP______State);
  Config();
};
// ==================================================================================================
// ----- Handle tap on Access Point button: Toggles AP mode and reconfigures device accordingly -----
#if BUTTON_AP__ENABLE
void handleAccessPointButton(Button2 &btn)
{
  String message = "📡 Access Point button tapped - toggling AP mode...\r\n";
  notifyClients(message);
  Cmd_Config_AP();
};
#endif
//==================================================================================================
// ---------------------------------- Setting of the access point ----------------------------------
#if WPS_ENABLE
void Cmd_Config_WPS(void)
{
  WPS_____State = !WPS_____State;
  Mode(static_cast<uint8_t>(WiFiModeStatus::WPS_ENABLED), WPS_____State);
  Config();
};
#endif
// ==================================================================================================
// ----------- Handle tap on WPS button: Starts WPS using Push Button Configuration (PBC) -----------
#if BUTTON_WPS_ENABLE
void handleWPSButton(Button2 &btn)
{
  String message = "🔐 WPS button tapped - starting WPS (PBC mode)...\r\n";
  notifyClients(message);
  Cmd_Config_WPS(); // ← Toggle AP state and reconfigure
  StartWPS(WPSMode::PBC);
};
#endif
//==================================================================================================
// ---------------------------------- Setting of the access point ----------------------------------
#if BLUETOOTH
void Cmd_Config_BTH(void)
{
  BTH_____State = !BTH_____State;
  Mode(static_cast<uint8_t>(WiFiModeStatus::BTH_ENABLED), BTH_____State);
  Config();
};
#endif
//==================================================================================================
// ----------- Handle tap on Bluetooth button: Toggles Bluetooth mode and reconfigures -------------
#if BUTTON_BTH_ENABLE
void handleBluetoothButton(Button2 &btn)
{
  String message = "🔵 Bluetooth button tapped - toggling state...\r\n";
  notifyClients(message);
  Cmd_Config_BTH(); // ← Toggle AP state and reconfigure
};
#endif
//==================================================================================================
// -------------------------------------- Use Scan Networks ----------------------------------------
bool scanNetworks(void)
{
  wifi_mode_t previousMode = WiFi.getMode();
  bool restoreMode = false;

  if (previousMode != WIFI_AP_STA)
  {
    WiFi.mode(WIFI_AP_STA);
    delay(100);
    restoreMode = true;
  }

  scanSize = WiFi.scanNetworks();
  if (scanSize <= 0)
    return false;

  uint8_t count = 0;
  for (int i = 0; i < scanSize && count < SCANSIZE; i++)
  {
    int rssi = WiFi.RSSI(i);
    String ssid = WiFi.SSID(i);

    // حذف SSID خالی و RSSI ضعیف
    if (ssid.length() == 0 || rssi < -90)
      continue;

    // بررسی تکراری بودن SSID
    bool duplicate = false;
    for (int j = 0; j < count; j++)
    {
      if (scanArray[(j * WIFIINFO) + WIFISSID] == ssid)
      {
        int existingRssi = scanArray[(j * WIFIINFO) + WIFIRSSI].toInt();
        if (rssi > existingRssi)
        {
          scanArray[(j * WIFIINFO) + WIFIRSSI] = String(rssi);
        }
        duplicate = true;
        break;
      }
    }

    if (duplicate)
      continue;

    scanArray[(count * WIFIINFO) + WIFISSID] = ssid;
    scanArray[(count * WIFIINFO) + WIFIRSSI] = String(rssi);
    scanArray[(count * WIFIINFO) + WIFIENCRYPTION] =
        (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "OPEN" : "SECURED";
    count++;
  }

  scanSize = count;

  if (restoreMode)
  {
    WiFi.mode(previousMode);
    delay(100);
  }

  return (scanSize > 0);
};
//==================================================================================================
// ------------------------------------- Use Send Json Error ---------------------------------------
void sendJsonError(const char *msg)
{
  DynamicJsonDocument response(256);
  response["error"] = true;
  response["message"] = msg;
  String out;
  serializeJson(response, out);
  notifyClients(out);
};
//==================================================================================================
// ------------------------------------- Use send Json Status --------------------------------------
void sendJsonStatus(const char *msg)
{
  DynamicJsonDocument response(256);
  response["error"] = false;
  response["message"] = msg;
  String out;
  serializeJson(response, out);
  notifyClients(out);
};
//==================================================================================================
// --------------------------- Parse Byte Array for IPv4/IPv6/MAC fields ---------------------------
bool parseByteArray(JsonVariant val, uint8_t *target, size_t expectedLength, const char *fieldName)
{
  if (!val.is<JsonArray>())
  {
    sendJsonError((String(fieldName) + " must be an array").c_str());
    return false;
  }

  JsonArray arr = val.as<JsonArray>();
  if (arr.size() != expectedLength)
  {
    sendJsonError((String(fieldName) + " must contain " + expectedLength + " values").c_str());
    return false;
  }

  for (size_t i = 0; i < expectedLength; i++)
  {
    if (!arr[i].is<uint8_t>() || arr[i].as<uint8_t>() > 255)
    {
      sendJsonError((String(fieldName) + " values must be in 0-255 range").c_str());
      return false;
    }
  }

  for (size_t i = 0; i < expectedLength; i++)
    target[i] = arr[i].as<uint8_t>();

  return true;
};
//==================================================================================================
// ------------------------------------- Use Set Item Json[] ---------------------------------------
bool jsonSet(void)
{
  uint16_t i = 0;

  if (!doc["fields"].is<JsonObject>())
  {
    sendJsonError("Missing or invalid 'fields' (must be a object)");
    return false;
  }

  JsonObject f = doc["fields"].as<JsonObject>();
  for (JsonPair kv : f)
  {
    const char *key = kv.key().c_str();
    //=========================================================================
    //============================================================= AccessPoint
    //=========================================================================
    if (strcmp(key, "username") == 0)
    {
      const char *u = kv.value();
      if (strlen(u) > sizeof(settings.Username) - 1 || strlen(u) == 0)
      {
        sendJsonError("Username Input exceeds max length (16 chars)");
        return false;
      }
      strlcpy(settings.Username, u, sizeof(settings.Username));
    }
    //=========================================================================
    else if (strcmp(key, "password") == 0)
    {
      const char *p = kv.value();
      if (strlen(p) > sizeof(settings.Password) - 1 || strlen(p) == 0)
      {
        sendJsonError("Password Input exceeds max length (16 chars)");
        return false;
      }
      strlcpy(settings.Password, p, sizeof(settings.Password));
    }
    //=========================================================================
    else if (strcmp(key, "AP SSID") == 0)
    {
      const char *s = kv.value();
      if (strlen(s) > sizeof(settings.AccessPoint.SSID) - 1 || strlen(s) == 0)
      {
        sendJsonError("AP SSID Input exceeds max length (32 chars)");
        settings.Status &= ~static_cast<uint8_t>(WiFiModeStatus::AP_CONFIG);
        return false;
      }
      strlcpy(settings.AccessPoint.SSID, s, sizeof(settings.AccessPoint.SSID));
      settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_CONFIG);
    }
    //=========================================================================
    else if (strcmp(key, "AP Pre-Shared Key") == 0)
    {
      const char *ap = kv.value();
      if (strlen(ap) > sizeof(settings.AccessPoint.PreSharedKey) - 1 || strlen(ap) == 0)
      {
        sendJsonError("AP Pre-Shared Key Input exceeds max length (64 chars)");
        settings.Status &= ~static_cast<uint8_t>(WiFiModeStatus::AP_CONFIG);
        return false;
      }
      strlcpy(settings.AccessPoint.PreSharedKey, ap, sizeof(settings.AccessPoint.PreSharedKey));
      settings.Status |= static_cast<uint8_t>(WiFiModeStatus::AP_CONFIG);
    }
    //=========================================================================
    else if (strcmp(key, "AP HostName") == 0)
    {
      const char *ah = kv.value();
      if (strlen(ah) > sizeof(settings.AccessPoint.HostName) - 1 || strlen(ah) == 0)
      {
        sendJsonError("AP HostName Input exceeds max length (16 chars)");
        return false;
      }
      strlcpy(settings.AccessPoint.HostName, ah, sizeof(settings.AccessPoint.HostName));
    }
    //=========================================================================
    else if (strcmp(key, "Wifi Channel") == 0)
    {
      uint8_t p = kv.value().as<uint8_t>();
      if (p == 0 || p > 13)
      {
        sendJsonError("Wifi Channel value must be between 1 and 13");
        return false;
      }
      settings.AccessPoint.WifiChannel = p;
    }
    //=========================================================================
    else if (strcmp(key, "Ssid Hidden") == 0)
    {
      if (!kv.value().is<bool>() || kv.value().isNull())
      {
        sendJsonError("Ssid Hidden must be boolean (true/false)");
        return false;
      }
      settings.AccessPoint.SsidHidden = kv.value().as<bool>();
    }
    //=========================================================================
    else if (strcmp(key, "Max Connection") == 0)
    {
      uint8_t p = kv.value().as<uint8_t>();
      if (p == 0 || p > 4)
      {
        sendJsonError("Max Connection value must be 1 or 4");
        return false;
      }
      settings.AccessPoint.MaxConnection = p;
    }
    //=========================================================================
    else if (strcmp(key, "AP Port") == 0)
    {
      uint16_t ap = kv.value().as<uint16_t>();
      if (ap < 1 || ap > 65535)
      {
        sendJsonError("Port number must be between 1 and 65535");
        return false;
      }
      settings.AccessPoint.Port = ap;
    }
    //=========================================================================
    else if (strcmp(key, "AP IPv4") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.AccessPoint.IPv4.field0, 4, "AP IPv4"))
        return false;
    }
    //=========================================================================
    else if (strcmp(key, "AP IPv6") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.AccessPoint.IPv6.field0, 16, "AP IPv6"))
        return false;
    }
    //=========================================================================
    else if (strcmp(key, "Gateway") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.AccessPoint.Gateway.field0, 4, "Gateway"))
        return false;
    }
    //=========================================================================
    else if (strcmp(key, "Subnet") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.AccessPoint.Subnet.field0, 4, "Subnet"))
        return false;
    }
    //=========================================================================
    else if (strcmp(key, "Primary DNS") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.AccessPoint.PrimaryDNS.field0, 4, "Primary DNS"))
        return false;
    }
    //=========================================================================
    else if (strcmp(key, "Secondary DNS") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.AccessPoint.SecondaryDNS.field0, 4, "Secondary DNS"))
        return false;
    }
    //=========================================================================
    else if (strcmp(key, "AP MAC") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.AccessPoint.MAC.field0, 6, "AP MAC"))
        return false;
    }
    //=========================================================================
    //================================================================= Station
    //=========================================================================
    else if (strcmp(key, "Modem SSID") == 0)
    {
      const char *s = kv.value();
      if (strlen(s) > sizeof(settings.Station.SSID) - 1 || strlen(s) == 0)
      {
        sendJsonError("Modem SSID Input exceeds max length (32 chars)");
        settings.Status &= ~static_cast<uint8_t>(WiFiModeStatus::STA_CONFIG);
        return false;
      }
      strlcpy(settings.Station.SSID, s, sizeof(settings.Station.SSID));
      settings.Status |= static_cast<uint8_t>(WiFiModeStatus::STA_CONFIG);
    }
    //=========================================================================
    else if (strcmp(key, "Modem Pre-Shared Key") == 0)
    {
      const char *s = kv.value();
      if (strlen(s) > sizeof(settings.Station.PreSharedKey) - 1 || strlen(s) == 0)
      {
        sendJsonError("Modem Pre-Shared Key Input exceeds max length (64 chars)");
        settings.Status &= ~static_cast<uint8_t>(WiFiModeStatus::STA_CONFIG);
        return false;
      }
      settings.Status |= static_cast<uint8_t>(WiFiModeStatus::STA_CONFIG);
      strlcpy(settings.Station.PreSharedKey, s, sizeof(settings.Station.PreSharedKey));
    }
//=========================================================================
#if WPS_ENABLE
    else if (strcmp(key, "Modem WPS PIN") == 0)
    {
      const char *s = kv.value();
      if (strlen(s) > sizeof(settings.Station.WPS_PIN) - 1 || strlen(s) == 0)
      {
        sendJsonError("Modem WPS PIN Input exceeds max length (8 chars)");
        return false;
      }
      strlcpy(settings.Station.WPS_PIN, s, sizeof(settings.Station.WPS_PIN));
      StartWPS(WPSMode::PIN, settings.Station.WPS_PIN);
    }
#endif
    //=========================================================================
    else if (strcmp(key, "STA HostName") == 0)
    {
      const char *u = kv.value();
      if (strlen(u) > sizeof(settings.Username) - 1 || strlen(u) == 0)
      {
        sendJsonError("STA HostName Input exceeds max length (16 chars)");
        return false;
      }
      strlcpy(settings.Station.HostName, u, sizeof(settings.Station.HostName));
    }
    //=========================================================================
    else if (strcmp(key, "Modem IP") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.Station.ModemIP.field0, 4, "Modem IP"))
        return false;
    }
    //=========================================================================
    else if (strcmp(key, "STA MAC") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.Station.STA_MAC.field0, 6, "STA MAC"))
        return false;
    }
//=========================================================================
//=============================================================== BLUETOOTH
//=========================================================================
#if BLUETOOTH
    else if (strcmp(key, "Bluetooth Name") == 0)
    {
      const char *u = kv.value();
      if (strlen(u) > sizeof(settings.Username) - 1 || strlen(u) == 0)
      {
        sendJsonError("Bluetooth Name Input exceeds max length (16 chars)");
        return false;
      }
      strlcpy(settings.Bluetooth.Name, u, sizeof(settings.Bluetooth.Name));
    }
    //=========================================================================
    else if (strcmp(key, "Bluetooth MAC") == 0)
    {
      if (!parseByteArray(kv.value(), &settings.Bluetooth.MAC.field1, 6, "Bluetooth MAC"))
        return false;
    }
#endif

    else
      sendJsonError("Unknown field");
  }
  storage.write();
  return true;
};
//==================================================================================================
// ------------------------------------- Use GET Item Json[] ---------------------------------------
void jsonGet(void)
{
  uint16_t i = 0;
  storage.read();
  res.clear();

  JsonVariant val = doc["fields"];
  if (!val.is<JsonArray>() || val.size() == 0)
  {
    sendJsonError("Invalid or empty 'fields' array");
    return;
  }

  if (!doc["fields"].isNull() && doc["fields"].is<JsonArray>() && doc["fields"].size() > 0)
  {
    JsonArray fields = doc["fields"].as<JsonArray>();
    for (JsonVariant f : fields)
    {
      const char *field = f.as<const char *>();
      //=========================================================================
      //============================================================= Information
      //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
      if (strcmp(field, "Device Info") == 0)
      {
        res["Device Info"].add(MANUFACTURER);
        res["Device Info"].add(DEVICE__NAME);
        res["Device Info"].add(MODEL_NUMBER);
        res["Device Info"].add(DEVICE_MODEL);
        res["Device Info"].add(PRODUCTION__DATE);
        res["Device Info"].add(settings.DeviceInfo);
        res["Device Info"].add(FIRMWARE_VERSION);
      }
      else if (strcmp(field, "Manufacturer") == 0)
        res["Manufacturer"] = MANUFACTURER;

      else if (strcmp(field, "Device Name") == 0)
        res["Device Name"] = DEVICE__NAME;

      else if (strcmp(field, "Model Number") == 0)
        res["Model Number"] = MODEL_NUMBER;

      else if (strcmp(field, "Device Model") == 0)
        res["Device Model"] = DEVICE_MODEL;

      else if (strcmp(field, "Production Date") == 0)
        res["Production Date"] = PRODUCTION__DATE;

      else if (strcmp(field, "Device Id") == 0)
        res["Device Id"] = settings.DeviceInfo;

      else if (strcmp(field, "Firmware Version") == 0)
        res["Firmware Version"] = FIRMWARE_VERSION;

      //=========================================================================
      //========================================================== Active Clients
      //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
      else if (strcmp(field, "Active Clients") == 0)
      {
        JsonArray scanArrayJson = res.createNestedArray("Scan Active Clients");
        for (uint8_t i = 0; i < APstationNum; i++)
        {
          JsonArray item = scanArrayJson.createNestedArray();
          item.add(APstationArray[(i * 3) + 0]);
          item.add(APstationArray[(i * 3) + 1]);
          item.add(APstationArray[(i * 3) + 2]);
        }
        if (!APstationNum)
        {
          res["Active Clients"] = "Status is station";
        }
      }
      //=========================================================================
      //============================================================= AccessPoint
      //=========================================================================
      else if (strcmp(field, "status") == 0)
        res["status"] = settings.Status;
      else if (strcmp(field, "username") == 0)
        res["username"] = settings.Username;
      else if (strcmp(field, "password") == 0)
        res["password"] = settings.Password;
      else if (strcmp(field, "AP SSID") == 0)
        res["AP SSID"] = settings.AccessPoint.SSID;
      else if (strcmp(field, "AP Pre-Shared Key") == 0)
        res["AP Pre-Shared Key"] = settings.AccessPoint.PreSharedKey;
      else if (strcmp(field, "AP HostName") == 0)
        res["AP HostName"] = settings.AccessPoint.HostName;
      else if (strcmp(field, "Wifi Channel") == 0)
        res["Wifi Channel"] = settings.AccessPoint.WifiChannel;
      else if (strcmp(field, "Ssid Hidden") == 0)
        res["Ssid Hidden"] = settings.AccessPoint.SsidHidden;
      else if (strcmp(field, "Max Connection") == 0)
        res["Max Connection"] = settings.AccessPoint.MaxConnection;
      else if (strcmp(field, "AP Port") == 0)
        res["AP Port"] = settings.AccessPoint.Port;
      else if (strcmp(field, "AP Station Num") == 0)
        res["AP Station Num"] = APstationNum;
      else if (strcmp(field, "AP IPv4") == 0)
      {
        for (i = 0; i < 4; i++)
          res["AP IPv4"].add((&settings.AccessPoint.IPv4.field0)[i]);
      }
      else if (strcmp(field, "AP IPv6") == 0)
      {
        for (i = 0; i < 16; i++)
          res["AP IPv6"].add((&settings.AccessPoint.IPv6.field0)[i]);
      }
      else if (strcmp(field, "Gateway") == 0)
      {
        for (i = 0; i < 4; i++)
          res["Gateway"].add((&settings.AccessPoint.Gateway.field0)[i]);
      }
      else if (strcmp(field, "Subnet") == 0)
      {
        for (i = 0; i < 4; i++)
          res["Subnet"].add((&settings.AccessPoint.Subnet.field0)[i]);
      }
      else if (strcmp(field, "Primary DNS") == 0)
      {
        for (i = 0; i < 4; i++)
          res["Primary DNS"].add((&settings.AccessPoint.PrimaryDNS.field0)[i]);
      }
      else if (strcmp(field, "Secondary DNS") == 0)
      {
        for (i = 0; i < 4; i++)
          res["Secondary DNS"].add((&settings.AccessPoint.SecondaryDNS.field0)[i]);
      }
      else if (strcmp(field, "Board BASE MAC") == 0)
      {
        for (i = 0; i < 6; i++)
          res["Board BASE MAC"].add((&settings.Board_BASE_MAC.field0)[i]);
      }
      else if (strcmp(field, "AP MAC") == 0)
      {
        for (i = 0; i < 6; i++)
          res["AP MAC"].add((&settings.AccessPoint.MAC.field0)[i]);
      }
      //=========================================================================
      //================================================================= Station
      //=========================================================================
      else if (strcmp(field, "Modem SSID") == 0)
      {
        res["Modem SSID"] = settings.Station.SSID;
      }
      else if (strcmp(field, "Modem Pre-Shared Key") == 0)
      {
        res["Modem Pre-Shared Key"] = settings.Station.PreSharedKey;
      }
      else if (strcmp(field, "STA HostName") == 0)
      {
        res["STA HostName"] = settings.Station.HostName;
      }
      else if (strcmp(field, "Modem IP") == 0)
      {
        for (i = 0; i < 4; i++)
          res["Modem IP"].add((&settings.Station.ModemIP.field0)[i]);
      }
      else if (strcmp(field, "Modem MAC") == 0)
      {
        for (i = 0; i < 6; i++)
          res["Modem MAC"].add((&settings.Station.ModemMAC.field0)[i]);
      }
      else if (strcmp(field, "STA MAC") == 0)
      {
        for (i = 0; i < 6; i++)
          res["STA MAC"].add((&settings.Station.STA_MAC.field0)[i]);
      }
#if WPS_ENABLE
      else if (strcmp(field, "Modem WPS PIN") == 0)
      {
        res["Modem WPS PIN"] = settings.Station.WPS_PIN;
      }
#endif
      else if (strcmp(field, "Scan Networks") == 0)
      {
        if (scanNetworks())
        {
          JsonArray scanArrayJson = res.createNestedArray("Scan Networks");
          for (uint8_t i = 0; i < scanSize; i++)
          {
            JsonArray item = scanArrayJson.createNestedArray();
            item.add(scanArray[(i * WIFIINFO) + WIFISSID]);
            item.add(scanArray[(i * WIFIINFO) + WIFIRSSI]);
            item.add(scanArray[(i * WIFIINFO) + WIFIENCRYPTION]);
          }
        }
        else
        {
          res["Scan Networks"] = "No Wi-Fi networks found";
        }
      }
      //=========================================================================
      //=============================================================== Bluetooth
      //=========================================================================
#if BLUETOOTH
      else if (strcmp(field, "Bluetooth Name") == 0)
      {
        res["Bluetooth Name"] = settings.Bluetooth.Name;
      }
      else if (strcmp(field, "Bluetooth MAC") == 0)
      {
        for (i = 0; i < 6; i++)
          res["Bluetooth MAC"].add((&settings.Bluetooth.MAC.field1)[i]);
      }
#endif
      else
        res[field] = "Unknown field";
    }
  }
  else
  {
    res["Device Info"].add(MANUFACTURER);
    res["Device Info"].add(DEVICE__NAME);
    res["Device Info"].add(MODEL_NUMBER);
    res["Device Info"].add(DEVICE_MODEL);
    res["Device Info"].add(PRODUCTION__DATE);
    res["Device Info"].add(settings.DeviceInfo);
    res["Device Info"].add(FIRMWARE_VERSION);

    res["status"] = settings.Status;
    res["username"] = settings.Username;
    res["password"] = settings.Password;
    res["AP SSID"] = settings.AccessPoint.SSID;
    res["AP Pre-Shared Key"] = settings.AccessPoint.PreSharedKey;
    res["AP HostName"] = settings.AccessPoint.HostName;
    res["Wifi Channel"] = settings.AccessPoint.WifiChannel;
    res["Ssid Hidden"] = settings.AccessPoint.SsidHidden;
    res["Max Connection"] = settings.AccessPoint.MaxConnection;
    res["AP Port"] = settings.AccessPoint.Port;
    res["AP Station Num"] = APstationNum;

    for (i = 0; i < 4; i++)
      res["AP IPv4"]
          .add((&settings.AccessPoint.IPv4.field0)[i]);

    for (i = 0; i < 16; i++)
      res["AP IPv6"].add((&settings.AccessPoint.IPv6.field0)[i]);

    for (i = 0; i < 4; i++)
      res["Gateway"].add((&settings.AccessPoint.Gateway.field0)[i]);

    for (i = 0; i < 4; i++)
      res["Subnet"].add((&settings.AccessPoint.Subnet.field0)[i]);

    for (i = 0; i < 4; i++)
      res["Primary DNS"].add((&settings.AccessPoint.PrimaryDNS.field0)[i]);

    for (i = 0; i < 4; i++)
      res["Secondary DNS"].add((&settings.AccessPoint.SecondaryDNS.field0)[i]);

    for (i = 0; i < 6; i++)
      res["Board BASE MAC"].add((&settings.Board_BASE_MAC.field0)[i]);

    for (i = 0; i < 6; i++)
      res["AP MAC"].add((&settings.AccessPoint.MAC.field0)[i]);

    for (i = 0; i < 6; i++)
      res["ETHERNET MAC"].add((&settings.Station.ETHERNET_MAC.field0)[i]);
    //=========================================================================
    //=========================================================================
    res["Modem SSID"] = settings.Station.SSID;
    res["Modem Pre-Shared Key"] = settings.Station.PreSharedKey;
    res["STA HostName"] = settings.Station.HostName;

    for (i = 0; i < 4; i++)
      res["Modem IP"].add((&settings.Station.ModemIP.field0)[i]);

    for (i = 0; i < 6; i++)
      res["Modem MAC"].add((&settings.Station.ModemMAC.field0)[i]);

    for (i = 0; i < 6; i++)
      res["STA MAC"].add((&settings.Station.STA_MAC.field0)[i]);

#if WPS_ENABLE
    res["Modem WPS PIN"] = settings.Station.WPS_PIN;
#endif
//=========================================================================
//=========================================================================
#if BLUETOOTH
    res["Bluetooth Name"] = settings.Bluetooth.Name;
    for (i = 0; i < 6; i++)
      res["Bluetooth MAC"].add((&settings.Bluetooth.MAC.field1)[i]);
#endif
  }

  String out;
  serializeJson(res, out);
  notifyClients(out);
};
//==================================================================================================
// ------------------------------------- Use Cmd Item Json[] ---------------------------------------
void jsonCmd(void)
{
  uint16_t i = 0;

  if (doc["fields"].isNull() || !doc["fields"].is<JsonObject>())
  {
    sendJsonError("Missing or invalid 'fields' (must be a object)");
    return;
  }

  JsonObject f = doc["fields"].as<JsonObject>();
  for (JsonPair kv : f)
  {
    const char *key = kv.key().c_str();
    //=========================================================================
    if (strcmp(key, "Reset factory") == 0)
    {
      if (!kv.value().is<bool>() || kv.value().isNull() || kv.value().as<bool>() != true)
      {
        sendJsonError("Reset factory must be boolean (true/false)");
        return;
      }
      sendJsonStatus(OKMESSAGE);
      Default();
    }
    //=========================================================================
    else if (strcmp(key, "Reset device") == 0)
    {
      if (!kv.value().is<bool>() || kv.value().isNull() || kv.value().as<bool>() != true)
      {
        sendJsonError("Reset device must be boolean (true/false)");
        return;
      }
      sendJsonStatus(OKMESSAGE);
      delay(1000);
      ESP.restart();
    }
    //=========================================================================
    else if (strcmp(key, "AP Button") == 0)
    {
      if (!kv.value().is<bool>() || kv.value().isNull() || kv.value().as<bool>() != true)
      {
        sendJsonError("AP Button must be boolean true");
        return;
      }
      sendJsonStatus(OKMESSAGE);
      Cmd_Config_AP();
    }
    //=========================================================================
    else if (strcmp(key, "STA Button") == 0)
    {
      if (!kv.value().is<bool>() || kv.value().isNull() || kv.value().as<bool>() != true)
      {
        sendJsonError("AP Button must be boolean true");
        return;
      }
      sendJsonStatus(OKMESSAGE);
      Cmd_Config_STA();
    }
    //=========================================================================
    else if (strcmp(key, "Config") == 0)
    {
      if (!kv.value().is<bool>() || kv.value().isNull() || kv.value().as<bool>() != true)
      {
        sendJsonError("Config must be boolean true");
        return;
      }
      sendJsonStatus(OKMESSAGE);
      Config();
    }
    else
    {
      sendJsonError((String("Unknown command: ") + key).c_str());
    }
  }
};
//==================================================================================================
// -------------------------------- Use User SET interface Json[] ----------------------------------
bool jsonUserSet(void)
{
  uint16_t i = 0;
  bool updated = false;

  if (doc["fields"].isNull() || !doc["fields"].is<JsonObject>())
  {
    sendJsonError("Missing or invalid 'fields' (must be a object)");
    return false;
  }

  JsonObject f = doc["fields"].as<JsonObject>();

  for (JsonPair kv : f)
  {
    const char *key = kv.key().c_str();

    //============================ SMS
    if (strcmp(key, "SMS") == 0)
    {
      if (!kv.value().is<bool>())
      {
        sendJsonError("SMS must be boolean (true/false)");
        return false;
      }
      // settings.UserInterface.SMS = kv.value();
      updated = true;
    }

    //============================ THEME (مثلاً: dark یا light)
    else if (strcmp(key, "Theme") == 0)
    {
      const char *val = kv.value();
      if (strlen(val) == 0 || strlen(val) > 10)
      {
        sendJsonError("Theme must be a short non-empty string");
        return false;
      }
      // strlcpy(settings.UserInterface.Theme, val, sizeof(settings.UserInterface.Theme));
      updated = true;
    }

    //============================ LANGUAGE
    else if (strcmp(key, "Language") == 0)
    {
      const char *val = kv.value();
      if (strlen(val) == 0 || strlen(val) > 5)
      {
        sendJsonError("Language must be a valid short code (e.g., en, fa)");
        return false;
      }
      // strlcpy(settings.UserInterface.Language, val, sizeof(settings.UserInterface.Language));
      updated = true;
    }

    //============================ UNKNOWN
    else
    {
      sendJsonError((String("Unknown user fields: ") + key).c_str());
      return false;
    }
  }

  if (updated)
  {
    // storage.write();
    sendJsonStatus("User settings saved");
  }
  else
  {
    sendJsonError("No valid fields to update");
  }
  return true;
};
//==================================================================================================
// -------------------------------- Use User GET interface Json[] ----------------------------------
void jsonUserGet(void)
{
  // storage.read();
  res.clear();

  if (doc["fields"].isNull() || !doc["fields"].is<JsonArray>())
  {
    sendJsonError("Missing or invalid 'fields' (must be array)");
    return;
  }

  JsonArray fields = doc["fields"].as<JsonArray>();
  for (JsonVariant f : fields)
  {
    const char *field = f.as<const char *>();

    if (strcmp(field, "SMS") == 0)
    {
      // res["SMS"] = settings.UserInterface.SMS;
    }
    else
    {
      res[field] = "Unknown user fields";
    }
  }

  String out;
  serializeJson(res, out);
  notifyClients(out);
};
//==================================================================================================
// -------------------------------- Use User CMD interface Json[] ----------------------------------
void jsonUserCMD(void)
{
  sendJsonError("User CMD not implemented");
};
//==================================================================================================
// ----------------------------------- Use Process Json Message ------------------------------------
void processJsonMessage(JsonDocument &doc)
{
  const char *setting = doc["setting"];
  const char *action = doc["action"];

  if (!setting || !action)
  {
    sendJsonError((String("Unknown setting or action: ") + setting + "/" + action).c_str());
    return;
  }

  if (strcmp(action, "read") == 0 && (doc["fields"].isNull() || !doc["fields"].is<JsonArray>()))
  {
    sendJsonError("Missing or invalid 'fields' (must be array)");
    return;
  }

  // ------------------------------------ device setting
  if (strcmp(setting, "device") == 0)
  {
    if (strcmp(action, "read") == 0)
    {
      jsonGet();
    }
    else if (strcmp(action, "write") == 0)
    {
      if (doc["fields"].isNull())
      {
        sendJsonError("Missing 'fields' block");
        return;
      }
      if (jsonSet())
        sendJsonStatus("Device settings saved");
    }
    else
    {
      sendJsonError("Unknown action for 'device'");
    }
  }
  // ------------------------------------ user setting
  else if (strcmp(setting, "user") == 0)
  {
    if (strcmp(action, "read") == 0)
    {
      jsonUserGet();
    }
    else if (strcmp(action, "write") == 0)
    {
      if (doc["fields"].isNull())
      {
        sendJsonError("Missing 'fields' block");
        return;
      }
      if (jsonUserSet())
        sendJsonStatus("User settings saved");
    }
    else
    {
      sendJsonError("Unknown action for 'user'");
    }
  }
  // ------------------------------------ system commands
  else if (strcmp(setting, "command") == 0)
  {
    if (strcmp(action, "push button") == 0)
    {
      jsonCmd();
    }
    else
    {
      sendJsonError("Unknown action for 'command'");
    }
  }
  // ------------------------------------ unknown setting
  else
  {
    sendJsonError("Unknown setting");
  }
};
//==================================================================================================
// ------------------------------ Function Code Use WebSocket Message -----------------------------
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
#if SERIAL_ENABLE
    String dataString = (char *)data;
    Serial.println("Message received:");
    Serial.println(dataString);
#endif

    doc.clear();
    if (deserializeJson(doc, data))
    {
      notifyClients("Invalid JSON");
      return;
    }

    processJsonMessage(doc);
  }
};
//==================================================================================================
// ------------------------------- Function Code Use Event WebSocket ------------------------------
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
#if SERIAL_ENABLE
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
#endif
    break;
  case WS_EVT_DISCONNECT:
#if SERIAL_ENABLE
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
#endif
    break;
  case WS_EVT_DATA:
#if SERIAL_ENABLE
    Serial.println("Command issued from the application");
#endif
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
};
//==================================================================================================
// ----------------------------- Function Code Use Global Error Handler ----------------------------
#if SERIAL_ENABLE
void globalErrorHandler(EEPROMError code, const char *message)
{
  Serial.print("[EEPROM ERROR] ");
  Serial.println(getEEPROMErrorString(code));
  Serial.println(message);
};
#endif
//==================================================================================================
// -------------------------------------------- Setup ---------------------------------------------
//==================================================================================================
void setup()
{
  WiFi.mode(WIFI_OFF);
  //----------------------------------------- initialize digital pin LED output.
#if DEF_LED_ENABLE
  pinMode(Def_LED, OUTPUT);
#endif

#if STA_LED_ENABLE
  pinMode(STA_LED, OUTPUT);
#endif

#if AP__LED_ENABLE
  pinMode(AP__LED, OUTPUT);
#endif

#if WPS_LED_ENABLE
  pinMode(WPS_LED, OUTPUT);
#endif

#if BTH_LED_ENABLE
  pinMode(BTH_LED, OUTPUT);
#endif
//----------------------------------------- Initialize Buttons
#if BUTTON_DEF_ENABLE
  Defulte_btn.setLongClickTime(LONGCLICK);
  Defulte_btn.setLongClickHandler(handleDefaultButton);
#endif

#if BUTTON_STA_ENABLE
  STA_____btn.setDebounceTime(DEBOUNCE);
  STA_____btn.setTapHandler(handleStationButton);
#endif

#if BUTTON_AP__ENABLE
  AP______btn.setDebounceTime(DEBOUNCE);
  AP______btn.setTapHandler(handleAccessPointButton);
#endif

#if BUTTON_WPS_ENABLE
  WPS_____btn.setDebounceTime(DEBOUNCE);
  WPS_____btn.setTapHandler(handleWPSButton);
#endif

#if BUTTON_BTH_ENABLE
  BTH_____btn.setDebounceTime(DEBOUNCE);
  BTH_____btn.setTapHandler(handleBluetoothButton);
#endif
  //----------------------------------------- Initialise Serial
#if SERIAL_ENABLE
  Serial.begin(Serial_Baudraite);
  Serial.println();
  Serial.println("Device start...");
#endif
  digitalWrite(Def_LED, HIGH);
//----------------------------------------- Initialize SPIFFS
#if SERIAL_ENABLE
  if (!SPIFFS.begin(true))
    Serial.println("An error has occurred while mounting SPIFFS");
  else
    Serial.println("SPIFFS mounted successfully");
#else
  SPIFFS.begin(true);
#endif
//----------------------------------------- Initialise EEPROM
#if SERIAL_ENABLE
  EEPROMStorage::setErrorHandler(globalErrorHandler);

  Serial.print("\nEEPROMStorage Object Count: ");
  Serial.println(storage.getObjectCount());

  Serial.println("\nEEPROM Test Start");

  if (storage.Init(&settings, sizeof(settings), 0))
  {
    if (storage.hasStructureChanged())
    {
      Serial.println("➔ Structure changed. Resetting to default...");
      Default();
    }
    else
    {
      Serial.println("➔ Structure OK. Reading from EEPROM...");
    }

    Serial.print("\nStruct Size: ");
    Serial.println(storage.getDataSize());

    Serial.print("Start Address: ");
    Serial.println(storage.getStartAddress());

    Serial.print("EEPROM Total Size: ");
    Serial.println(storage.getEepromTotalSize());

    Serial.print("EEPROM Version: ");
    Serial.println(storage.getProjectVersion());

    Serial.print("EEPROM Structure Hash: 0x");
    Serial.println(storage.getEepromstructureHash(), HEX);
  }
#else

  if (storage.Init(&settings, sizeof(settings), 0))
  {
    if (storage.hasStructureChanged())
    {
      Default();
    }
  }
#endif
  //----------------------------------------- Initialize WiFi modes (AP/STA/WPS) based on saved settings
  Config();
  //----------------------------------------- Register WiFi event handler (e.g., connect, disconnect, IP assigned, etc.)
  WiFiEventId_t eventID = WiFi.onEvent(WiFiEvent);
  //----------------------------------------- Register WebSocket event callback for handling JSON messages
  websocket.onEvent(onEvent);
  //----------------------------------------- Start HTTP server with port from EEPROM configuration
  ServerNewHandler();
  //----------------------------------------- Initialize hardware timer for AP LED blinking if not configured
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 700000, true); // every 0.3 seconds
  timerAlarmEnable(timer);
};
//==================================================================================================
//--------------------------------------------- Loop ----------------------------------------------
//==================================================================================================
void loop()
{
  websocket.cleanupClients();
//==================================================
// loop state of the buttons
//==================================================
#if BUTTON_DEF_ENABLE
  Defulte_btn.loop();
#endif

#if BUTTON_AP__ENABLE
  AP______btn.loop();
#endif

#if BUTTON_STA_ENABLE
  STA_____btn.loop();
#endif

#if BUTTON_WPS_ENABLE
  WPS_____btn.loop();
#endif

#if BUTTON_BTH_ENABLE
  BTH_____btn.loop();
#endif

#if OTA_ENABLE
  ElegantOTA.loop();
#endif
};
