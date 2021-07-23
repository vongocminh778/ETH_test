#include <Arduino.h>
/*
    This sketch shows how to configure different external or internal clock sources for the Ethernet PHY
*/

#include <ETH.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "RL65_RFID_Reader.h"
#include <HTTPClient.h>

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
#define alarm 2

RFID nano; //Create instance

int localUdpPort = 1969; // Local port number
char* udpAddress = "192.168.0.100";

const char* ssid = "Matsuya MIC";
const char* password = "M@tsuyaR&D2020";
const char *test_rfid = "http://113.161.152.35:2030/api/Mobile/testrfid";

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
void Send_data_firmware(String _path, String rfid);

void setup() {
  pinMode(ETH_POWER_PIN_ALTERNATIVE, OUTPUT);
  digitalWrite(ETH_POWER_PIN_ALTERNATIVE, HIGH);
  Serial.begin(115200);
  //setup alarm
  pinMode(alarm, OUTPUT);
  digitalWrite(alarm, LOW);
  
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
  // Serial.println();
  if (eth_connected) {
    byte myEPC[12]; //Most EPCs are 12 bytes
    byte myEPClength;
    byte responseType = 0;

    while (responseType != RESPONSE_SUCCESS)//RESPONSE_IS_TAGFOUND)
    {
      myEPClength = sizeof(myEPC); //Length of EPC is modified each time .readTagEPC is called

      responseType = nano.readTagEPC(myEPC, myEPClength, 500); //Scan for a new tag up to 500ms
      // Serial.println(F("Searching for tag"));
      // delay(500);
    }
    //Print EPC
  String str = "";
  Serial.print(F(" epc["));
  for (byte x = 0 ; x < myEPClength ; x++)
  {
    if (myEPC[x] < 0x10) {
      Serial.print(F("0"));
      str += "0";
    }
    str += String((uint8_t)myEPC[x],HEX);
    Serial.print(myEPC[x], HEX);
    Serial.print(F(" "));
  }
  Serial.println(F("]"));
  
  Serial.println("rfid: " + str);
  Send_data_firmware(test_rfid, str);
    // nano.setOutputPower(51);
    // nano.setAntennaPort();
    // nano.readTagEPC(); 
  }
}

void printHex(uint8_t num) {
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(hexCar);
}

void Send_data_firmware(String _path, String rfid)
{
    HTTPClient http;
    http.begin(_path);
    String data_confirm ="{\"message\" : \"";
    data_confirm += rfid;  
    data_confirm += "\"}\n"; 
    // Serial.println(data_confirm);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.sendRequest("POST", (uint8_t *)data_confirm.c_str(), data_confirm.length());
    if(httpResponseCode>0)
    {
      digitalWrite(alarm, HIGH);
      delay(100);
      digitalWrite(alarm, LOW);
      String url_firmware = http.getString();    
      Serial.println(url_firmware);//Get the response to the request
      Serial.println(httpResponseCode);   //Print return code     
   }
   else
   {
      Serial.print(F("Error on sending POST: "));
      Serial.println(httpResponseCode);
   }
      http.end();
      ESP.getFreeHeap();
}



