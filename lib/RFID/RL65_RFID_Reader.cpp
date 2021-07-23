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

void RFID::setOutputPower(int8_t powerSetting)
{
  if (powerSetting > 63)
    powerSetting = 63; //Limit to 63
  if(powerSetting < 0)
    powerSetting = 0; 
  //Copy this setting into a temp data array
  uint8_t size = sizeof(powerSetting);
  uint8_t data[size];
  for (uint8_t x = 0; x < size; x++){
    data[x] = (uint8_t)(powerSetting >> (8 * (size - 1 - x)));// convert hex to dec
  }
  sendMessage(TMR_SR_OPCODE_SET_OUTPUT_POWER, data, size);
}

// void RFID::readTagEPC(void)
// {
//   // Serial.print("version: ");
//   // sendMessage(TMR_SR_OPCODE_VERSION);
//   uint8_t configBlob[] = {0x01, 0x00, 0x00, 0x00};
//   sendMessage(0xEE, configBlob, sizeof(configBlob));
// }

uint8_t RFID::readTagEPC(uint8_t *epc, uint8_t &epcLength, uint16_t timeOut)
{
  uint8_t bank = 0x01;    //memory bank -- 01.EPC -- 02.TID -- 03.User
  uint16_t address = 0x0000; 
  uint8_t len = 0x00;
  return (readData(bank, address, len, epc, epcLength, timeOut));
}
uint8_t RFID::readData(uint8_t bank, uint32_t address, uint8_t len, uint8_t *dataRead, uint8_t &dataLengthRead, uint16_t timeOut)
{
  uint8_t data[4];
  //Insert timeout
  data[0] = bank;
  data[1] = address >> 8 & 0xFF;
  data[2] = address & 0xFF;
  data[3] = len;

  sendMessage(TMR_SR_OPCODE_READ_TAG_DATA, data, sizeof(data), timeOut);

  if (msg[0] == 0xF0)
  {
    Serial.print("mmmmmm: ");
    for (uint8_t x = 0; x < dataLengthRead; x++){
      dataRead[x] = msg[4 + x];
      printHex(dataRead[x]);
    }
    Serial.println();
    return (RESPONSE_SUCCESS);
  }
  Serial.println();
  dataLengthRead = 0; //Inform caller that we weren't able to read anything

  return (RESPONSE_FAIL);
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
  if(_Udp.read(msg, 50) > 0){
    // msg[0] = ALL_GOOD;
    // Serial.print("Server to client: ");
    // Serial.println("Lenght: "+ String(int(msg[1]) + 2));
    // for(int i=0; i<int(msg[1]) + 2; i++){
    //   printHex(msg[i]);
    // }
  }
  Serial.println();

   //If everything is ok, load all ok into msg array
  
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
