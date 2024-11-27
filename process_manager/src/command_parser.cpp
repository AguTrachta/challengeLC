// src/command_parser.cpp

#include "../include/command_parser.h"
#include <sstream>

ParsedCommand CommandParser::parse(const std::string &input) {
  ParsedCommand cmd;
  std::istringstream stream(input);
  stream >> cmd.name;

  std::string arg;
  while (stream >> arg) {
    cmd.args.push_back(arg);
  }

  return cmd;
}
