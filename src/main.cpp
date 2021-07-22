#include <Arduino.h>
/*
    This sketch shows how to configure different external or internal clock sources for the Ethernet PHY
*/

#include <ETH.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "SparkFun_UHF_RFID_Reader.h"

/* 
   * ETH_CLOCK_GPIO0_IN   - default: external clock from crystal oscillator
   * ETH_CLOCK_GPIO0_OUT  - 50MHz clock from internal APLL output on GPIO0 - possibly an inverter is needed for LAN8720
   * ETH_CLOCK_GPIO16_OUT - 50MHz clock from internal APLL output on GPIO16 - possibly an inverter is needed for LAN8720
   * ETH_CLOCK_GPIO17_OUT - 50MHz clock from internal APLL inverted output on GPIO17 - tested with LAN8720
*/
// #define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN   16
#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_ADDR        1
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18
#define ETH_POWER_PIN_ALTERNATIVE 16 // 17    // ???

#define MAX_MSG_SIZE 255
#define COMMAND_TIME_OUT 2000 //Number of ms before stop waiting for response from modul
uint8_t msg[MAX_MSG_SIZE];

RFID nano; //Create instance

int localUdpPort = 1969; // Local port number
char* udpAddress = "192.168.0.100";

const char* ssid = "Matsuya MIC";
const char* password = "M@tsuyaR&D2020";

IPAddress local_ip(192,168,0,99);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress dns1(8,8,8,8);
IPAddress dns2 = (uint32_t)0x00000000;

static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void printHex(uint8_t num);

void setup() {
  pinMode(ETH_POWER_PIN_ALTERNATIVE, OUTPUT);
  digitalWrite(ETH_POWER_PIN_ALTERNATIVE, HIGH);
  Serial.begin(115200);

  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");  delay(100); 
  }
  Serial.println(ssid);
  Serial.println(WiFi.localIP());

  ETH.config(local_ip, gateway, subnet, dns1, dns2);
  nano.begin(localUdpPort, udpAddress);

  Serial.println("Setup done!\n");
}


void loop() {
  // Serial.println("\nLoop ...!\n");
  // Serial.println(hexToDec("10EC"));
  Serial.println();
  if (eth_connected) {
    nano.getVersion(); 
  }
  
  // delay(10);
}

void printHex(uint8_t num) {
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(hexCar);
}



