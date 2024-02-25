#ifndef SimpleSerialCommandParser_H
#define SimpleSerialCommandParser_H

#pragma once

#include <Arduino.h>
#include <vector>

class CommandParser
{
  struct CmdInfo
  {
    uint16_t cmdId;
    std::vector<String> cmdWords;
    CmdInfo( uint16_t id = 0 )
      : cmdId(id)
    { }
    CmdInfo( uint16_t id, const std::vector<String> &words )
      : cmdId(id)
      , cmdWords(words)
    { }
  };
  std::vector<CommandParser::CmdInfo> cmdsInfo;

  String cmdSintaxis(const CommandParser::CmdInfo &cmdInfo) const;

public:
  enum RtnValue
  {
    NoData,
    NoCmdFound,
    CmdFound
  };
  struct CmdParamValue
  {
    String s;
    union
    {
      float f;
      uint32_t u;
      int32_t d;
    };
    CmdParamValue(float fl)
      : f(fl)
    { }
    CmdParamValue(uint32_t ui = 0)
      : u(ui)
    { }
    CmdParamValue(int32_t in)
      : d(in)
    { }
    CmdParamValue(String str)
      : s(str)
      , u(0)
    { }
  };
  struct CmdData
  {
    uint16_t cmdId;
    std::vector<CmdParamValue> params;
  };
  void addCommandInfo(uint16_t cmdId, const std::vector<String> &words)
  {
    if( cmdId == 0 )
      return;
    cmdsInfo.push_back( CmdInfo(cmdId, words) );
  }
  void printAllCommands();

  CommandParser::RtnValue readCommand(CommandParser::CmdData &);
  String cmdSintaxis(uint16_t cmdId) const;
  CommandParser()
  { }
};

#endif
