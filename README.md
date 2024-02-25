# ESP32SimpleSerialCommandParser
Simple and easy to use ESP32 serial command parser.

Sometimes we need to read from user input, usually while developing, that comes through Serial port.

This library helps to get command, keeping main code clean.

As per the "while developing", it's keeped simple, without value validations, descriptions and such friendly things more suitable for end-users. You, of course, can do this checks in your code.

## How to use.
First and only once, download the library zip and import with Arduino IDE.
Also, you need another (fool) library where I put some handly functions because I'm lazy to copy them all times. Its https://github.com/silderan/ESP32SilderanUtils

Then, include header:
#include <SimpleSerialCommandParser.h>

Now, you're able to create the object class.
CommandParser cmdParser;

Nest step will be defining macros (or a enum, if you prefer)
Just keep in mind that 0 is not a valid number. Start from 1.
```cpp
#define CMD_LED_ON  (1)
#define CMD_LED_OFF (2)
```

Enums allways starts from 0. So, force it properly:
```cpp
enum MyCmds
{
  LedOn = 1,
  LedOff  // This will get number 2, no need to tell compiler that.
};
```
This define or enum value will be the Command ID.

Once we have commands ID, let's create the commands sintaxis.
```cpp
cmdParser.addCommandInfo( CMD_LED_ON, {"turn", "led", "%d", "on"} );
cmdParser.addCommandInfo( CMD_LED_OFF, {"turn", "off", "the", "led", "%d"} );
```
Parameter values accepted are %d, %f, %u and %s, for integer, fload, unsigned and string.

Now, in your loop, call readCommand to do the magic, this way:
```cpp
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
```
Well, here needs some explanation.
CommandParser matches every word you put in the array of the second parameter of addCommandInfo function verbatim, except those starting with %. This ones will be captured and stored in the params vector of CmdData structure. The index of value is the number of % starting word - 1. It reminds regexp captures. But it converts code to integer, unsigned or float. You cant get of by:
```cpp
cmdData[0].u; // Gets the first unsigned value in the command.
cmdData[1].f; // Gets the second value, as float, in de command. For example, in a command like "set gain of potentioneter %d %f"
cmdData[i].d; // Gets the i+1 value, as decimal (integer), in the command.
cmdData[i].s; // Gets the i+1 value, as String, in the command.
```
If you read a String but you configure a decimal on that position, you get empty String.
If you read a number but you configure a String on that position, you get 0.
If you read a float but you configure another type of number, you may get garbage.

This stripped code is put together in the "leds_commanded" example. Currently, no LED to turn on and Off. I'll upload a corrected version and a working example.
esp32_simple_command_parser example shows some more functions. Look at the code. I hope it's readable enouth to don't need to be explained. Let me know otherwise.
