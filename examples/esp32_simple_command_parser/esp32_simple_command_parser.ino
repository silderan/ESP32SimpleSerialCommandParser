#include "SimpleSerialCommandParser.h"
#include "Esp32Utils.h"

CommandParser cmdParser;

#define CMD_1 (1)
#define CMD_2 (2)
#define CMD_3 (3)

void setup()
{
  Esp32Utils::setupSerial();
  cmdParser.addCommandInfo( CMD_1, {"a", "b"} );
  cmdParser.addCommandInfo( CMD_2, {"a", "%d"} );
  cmdParser.addCommandInfo( CMD_3, {"rate", "the", "classmate", "%s", "with", "%d"} );
}

void displayCommandData(const CommandParser::CmdData &cmdData )
{
  Serial.printf("Command with id %d has the sintaxis:\n > %s\nAnd the data introduced is:\n", cmdData.cmdId, cmdParser.cmdSintaxis(cmdData.cmdId).c_str() );
  if( !cmdData.params.size() )
    Serial.println("No data");
  else
  {
    int i = 0;
     Serial.println("           int | uint | float | string");
    for( const CommandParser::CmdParamValue &cmdParam : cmdData.params )
    {
      ++i;
      Serial.printf("Parameter %d: %d | %u | %f | '%s'\n", i, cmdParam.d, cmdParam.u, cmdParam.f, cmdParam.s);
    }
  }
}

void loop()
{
  static CommandParser::CmdData cmdData;
  switch( cmdParser.readCommand(cmdData) )
  {
  case CommandParser::RtnValue::NoData: break;
  case CommandParser::RtnValue::NoCmdFound:
    Serial.println("Los datos introducidos no encajan en ningún comando.");
    break;
  case CommandParser::RtnValue::CmdFound:
    displayCommandData(cmdData);
  }
}
