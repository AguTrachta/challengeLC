// include/command_parser.h

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <vector>

struct ParsedCommand {
  std::string name;
  std::vector<std::string> args;
};

class CommandParser {
public:
  ParsedCommand parse(const std::string &input);
};

#endif // COMMAND_PARSER_H
