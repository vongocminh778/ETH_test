#include "Arduino.h" //Needed for Stream
#include "WiFiUdp.h"

#define MAX_MSG_SIZE 20

#define TMR_SR_OPCODE_VERSION 0x02
#define TMR_SR_OPCODE_SET_BAUD_RATE 0x06
#define TMR_SR_OPCODE_READ_TAG_ID_SINGLE 0x21
#define TMR_SR_OPCODE_READ_TAG_ID_MULTIPLE 0x22
#define TMR_SR_OPCODE_WRITE_TAG_ID 0x23
#define TMR_SR_OPCODE_WRITE_TAG_DATA 0x24
#define TMR_SR_OPCODE_KILL_TAG 0x26
#define TMR_SR_OPCODE_READ_TAG_DATA 0xEE
#define TMR_SR_OPCODE_CLEAR_TAG_ID_BUFFER 0x2A
#define TMR_SR_OPCODE_MULTI_PROTOCOL_TAG_OP 0x2F
#define TMR_SR_OPCODE_GET_READ_TX_POWER 0x62
#define TMR_SR_OPCODE_GET_WRITE_TX_POWER 0x64
#define TMR_SR_OPCODE_GET_POWER_MODE 0x68
#define TMR_SR_OPCODE_GET_READER_OPTIONAL_PARAMS 0x6A
#define TMR_SR_OPCODE_GET_PROTOCOL_PARAM 0x6B
#define TMR_SR_OPCODE_SET_ANTENNA_PORT 0x0A
#define TMR_SR_OPCODE_SET_TAG_PROTOCOL 0x93
#define TMR_SR_OPCODE_SET_OUTPUT_POWER 0x04
#define TMR_SR_OPCODE_SET_WRITE_TX_POWER 0x94
#define TMR_SR_OPCODE_SET_REGION 0x97
#define TMR_SR_OPCODE_SET_READER_OPTIONAL_PARAMS 0x9A
#define TMR_SR_OPCODE_SET_PROTOCOL_PARAM 0x9B

#define COMMAND_TIME_OUT 2000 //Number of ms before stop waiting for response from module

//Define all the ways functions can return
#define ALL_GOOD 0
#define ERROR_COMMAND_RESPONSE_TIMEOUT 1
#define ERROR_CORRUPT_RESPONSE 2
#define ERROR_WRONG_OPCODE_RESPONSE 3
#define ERROR_UNKNOWN_OPCODE 4
#define RESPONSE_IS_TEMPERATURE 5
#define RESPONSE_IS_KEEPALIVE 6
#define RESPONSE_IS_TEMPTHROTTLE 7
#define RESPONSE_IS_TAGFOUND 8
#define RESPONSE_IS_NOTAGFOUND 9
#define RESPONSE_IS_UNKNOWN 10
#define RESPONSE_SUCCESS 11
#define RESPONSE_FAIL 12

//Define all the ways functions can return
#define _no_antenna		      0x01;   //Kết nối ăng-ten thất bại
#define _no_label			      0x02;   //Không có nhãn được phát hiện
#define _invalid_label		  0x03;   //Nhãn bất hợp pháp
#define _less_power		      0x04;   //Đọc và viết là không đủ
#define _write_prot_error	  0x05;   //Khu vực này viết sự bảo vệ
#define _check_sum_error	  0x06;   //Kiểm tra và LRI.
#define _parameter_error	  0x07;   //Thông số Lỗi.
#define _memory_error		    0x08;   //Khu vực dữ liệu không tồn tại
#define _password_error		  0x09;   //mật khẩu không đúng
#define _killpassword_error	0x0a;   //Mật khẩu hủy nhãn G2 là 0
#define _nonlicet_command	  0x0b;   //Trật tự bất hợp pháp
#define _nonlicet_user		  0x0c;   //Những người dùng bất hợp pháp không phù hợp với mật khẩu
#define _invalid_command	  0x1e;   //Cho biết một hướng dẫn không hợp lệ, chẳng hạn như tham số không chính xác
#define _other_error        0x1f;   //Lệnh không xác định
#define _no_cardID_input    0x20;   //Lệnh không xác định

class RFID : public WiFiUDP
{
public:
  RFID(void);
  void begin(int _p, char * _add);

  void setBaud(long baudRate);

  void getVersion(void);

  void setAntennaPort();
  void setOutputPower(int8_t powerSetting);

  void readTagEPC(void);
  uint8_t readTagEPC(uint8_t *epc, uint8_t &epcLength, uint16_t timeOut);
  uint8_t readData(uint8_t bank, uint32_t address, uint8_t len, uint8_t *dataRead, uint8_t &dataLengthRead, uint16_t timeOut);
  void sendMessage(uint8_t opcode, uint8_t *data = 0, uint8_t size = 0, uint16_t timeOut = COMMAND_TIME_OUT, boolean waitForResponse = true);
  void sendCommand(uint16_t timeOut = COMMAND_TIME_OUT, boolean waitForResponse = true);
  void printHex(uint8_t num);
  uint8_t CheckSum(unsigned char* uBuff, unsigned char uBuffLen);

  uint8_t msg[MAX_MSG_SIZE];

private:
  uint8_t _head = 0; //Tracks the length of the incoming message as we poll the software serial
  boolean _printDebug = false; //Flag to print the serial commands we are sending to the Serial port for debug
};
