#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "RL65_RFID_Reader.h"

WiFiUDP _Udp;                      // create UDP object
int _localUdpPort = 0; // Local port number
char * _udpAddress = "0.0.0.0";

RFID::RFID(void)
{
  // Constructor
}
void RFID::begin(int _p, char * _add){
  _Udp.begin(_p);
  _localUdpPort = _p;
  _udpAddress = _add;
}

void RFID::setBaud(long baudRate)
{
  //Copy this setting into a temp data array
  uint8_t size = sizeof(baudRate);
  uint8_t data[size];
  for (uint8_t x = 0; x < size; x++)
    data[x] = (uint8_t)(baudRate >> (8 * (size - 1 - x)));

  sendMessage(TMR_SR_OPCODE_SET_BAUD_RATE, data, size, COMMAND_TIME_OUT, false);
}

//Get the version number from the module
void RFID::getVersion(void)
{
  sendMessage(TMR_SR_OPCODE_VERSION);
}
// Function: Select antenna
// Commmand packet: 40H  03H  0AH  No.  CheckSum
void RFID::setAntennaPort(void)
{
  uint8_t configBlob[] = {0x01}; //01H antenna 1 -- 02H antenna 2 -- 03H antenna 3 -- 04H antenna 4
  sendMessage(TMR_SR_OPCODE_SET_ANTENNA_PORT, configBlob, sizeof(configBlob));
}

void RFID::readTagEPC(void)
{
  // Serial.print("version: ");
  // sendMessage(TMR_SR_OPCODE_VERSION);
  uint8_t configBlob[] = {0x01, 0x00, 0x00, 0x00};
  sendMessage(0xEE, configBlob, sizeof(configBlob));
}



//Given an opcode, a piece of data, and the size of that data, package up a sentence and send it
void RFID::sendMessage(uint8_t opcode, uint8_t *data, uint8_t size, uint16_t timeOut, boolean waitForResponse)
{
  msg[1] = size + 2; //Load the length of this operation into msg array
  msg[2] = opcode;

  //Copy the data into msg array
  for (uint8_t x = 0; x < size; x++)
    msg[3 + x] = data[x];

  sendCommand(timeOut, waitForResponse); //Send and wait for response
}

void RFID::sendCommand(uint16_t timeOut, boolean waitForResponse)
{
  msg[0] = 0x40; //Universal header
  int messageLength = msg[1] - 2;
  uint8_t crc = CheckSum(&msg[0], messageLength + 3);
  msg[messageLength + 3] = crc;

  Serial.print("Command: ");
  for (uint8_t x = 0; x < messageLength + 4; x++){
    printHex(msg[x]);
  }
  Serial.println();

  _Udp.beginPacket(_udpAddress, _localUdpPort);
  _Udp.write((const uint8_t*)msg, messageLength + 4);
  _Udp.endPacket();
  memset(msg, 0, messageLength + 4);
  _Udp.parsePacket();

  //receive response from server, it will be HELLO WORLD
  if(_Udp.read(msg, 10) > 0){
    Serial.print("Server to client: ");
    Serial.println("Lenght: "+ String(int(msg[1]) + 2));
    for(int i=0; i<int(msg[1]) + 2; i++){
      printHex(msg[i]);
    }
  }
  Serial.println();
}
void RFID::printHex(uint8_t num) {
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(hexCar);
}

uint8_t RFID::CheckSum(unsigned char* uBuff, unsigned char uBuffLen)
{
    unsigned char i, uSum = 0;
    for (i = 0; i < uBuffLen; i++)
    {
        uSum = uSum + uBuff[i];
    }
    uSum = (~uSum) + 1;
    return uSum;
}
