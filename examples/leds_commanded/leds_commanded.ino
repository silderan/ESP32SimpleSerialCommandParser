#include "SimpleSerialCommandParser.h"
#include "Esp32Utils.h"

CommandParser cmdParser;

#define CMD_LED_ON  (1)
#define CMD_LED_OFF (2)

void setup()
{
  Esp32Utils::setupSerial();
  cmdParser.addCommandInfo( CMD_LED_ON, {"turn", "led", "%d", "on"} );
  cmdParser.addCommandInfo( CMD_LED_OFF, {"turn", "off", "the", "led", "%d"} );
}

void powerOnLed( uint16_t led )
{
  Serial.printf("Now imagine that the led %d lightes up.\n", led);
}

void powerOffLed( uint16_t led )
{
  Serial.printf("Now imagine that the led %d turns off.\n", led);
}

void loop()
{
  // This will be the struct that the parser will fill with de data readed. It's not necessary to be static.
  static CommandParser::CmdData cmdData;

  switch( cmdParser.readCommand(cmdData) )
  {
  case CommandParser::RtnValue::NoData:
    // Do your usual loop here.
    break;
  case CommandParser::RtnValue::NoCmdFound:
    Serial.println("This command is not valid.");
    break;
  case CommandParser::RtnValue::CmdFound:
    switch( cmdData.cmdId )
    {
    case CMD_LED_ON:
      powerOnLed(cmdData.params[0].d);
      break;
    case CMD_LED_OFF:
      powerOffLed(cmdData.params[0].d);
      break;
    default:
      Serial.println("This command is not programed yet.");
      break;
    }
  }
}
