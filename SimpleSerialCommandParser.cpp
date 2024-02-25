#include "SimpleSerialCommandParser.h"
#include "Esp32Utils.h"

CommandParser::RtnValue CommandParser::readCommand(CommandParser::CmdData &cmdData)
{
  static String incommingBytes;

  cmdData.cmdId = 0;
  while( Serial.available() )
  {
    char c = Serial.read();
    if( (c != '\n') && (c != '\r') )
    {
      incommingBytes += c;
      continue;
    }
    if( !incommingBytes.length() )
      continue;

    std::vector<String> words = Esp32Utils::splitString(incommingBytes, ' ');
    incommingBytes = "";

    size_t i;
    for( const CommandParser::CmdInfo &cmdInfo : cmdsInfo )
    {
      cmdData.cmdId = cmdInfo.cmdId;
      cmdData.params.resize(0);
      if( words.size() < cmdInfo.cmdWords.size() )
        continue;
      for( i = 0; i < cmdInfo.cmdWords.size(); ++ i )
      {
        if( cmdInfo.cmdWords[i][0] == '%' )
        {
          switch( cmdInfo.cmdWords[i][1] )
          {
          case 'f': cmdData.params.push_back( CmdParamValue(static_cast<float>(words[i].toFloat())) ); break;
          case 'd': cmdData.params.push_back( CmdParamValue(static_cast<int32_t>(words[i].toInt())) ); break;
          case 'u': cmdData.params.push_back( CmdParamValue(static_cast<uint32_t>(words[i].toInt())) ); break;
          case 's': cmdData.params.push_back( CmdParamValue(words[i]) ); break;
          }
        }
        else
        if( cmdInfo.cmdWords[i] != words[i] )
        {
          cmdData.params.resize(0);
          break;
        }
      }
      // Note that i will has the size value just in case that all words match (not "break" called inside loop)
      if( i == cmdInfo.cmdWords.size() )
        return CommandParser::RtnValue::CmdFound;
    }
  }
  // Note that cmdData could has some data, but id is not valid.
  return cmdData.cmdId == 0 ? CommandParser::RtnValue::NoData : CommandParser::RtnValue::NoCmdFound ;
}

String CommandParser::cmdSintaxis(const CommandParser::CmdInfo &cmdInfo) const
{
  String rtn;
  for( String str : cmdInfo.cmdWords )
  {
    if( str[0] == '%' )
    {
      rtn += "<";
      rtn += str;
      rtn += "> ";
    }
    else
    {
      rtn += str;
      rtn += " ";
    }
  }
  return rtn;
}

String CommandParser::cmdSintaxis(uint16_t cmdId) const
{
  for( const CommandParser::CmdInfo &cmdInfo : cmdsInfo )
    if( cmdInfo.cmdId == cmdId )
      return cmdSintaxis(cmdInfo);
  return "Not found";
}

void CommandParser::printAllCommands()
{
  for( const CommandParser::CmdInfo &cmdInfo : cmdsInfo )
    Serial.println(cmdSintaxis(cmdInfo));
}
