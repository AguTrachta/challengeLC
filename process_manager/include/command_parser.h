/**
 * @file command_parser.h
 * @brief Defines the structures and class for parsing commands from input
 * strings.
 *
 * This file contains the definition of the `ParsedCommand` struct and the
 * `CommandParser` class, which is used to parse a string containing a command
 * and its arguments.
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <vector>

/**
 * @brief Represents a parsed command consisting of a command name and its
 * arguments.
 *
 * This structure holds the command name and a list of arguments extracted from
 * an input string.
 */
struct ParsedCommand {
  /**
   * @brief The name of the command.
   *
   * This is the main command extracted from the input string.
   */
  std::string name;

  /**
   * @brief A vector of arguments for the command.
   *
   * This vector holds the arguments associated with the command, extracted from
   * the input string.
   */
  std::vector<std::string> args;
};

/**
 * @class CommandParser
 * @brief A class responsible for parsing commands from an input string.
 *
 * The `CommandParser` class provides a method to parse a command string into a
 * `ParsedCommand` structure, which consists of the command's name and its
 * arguments.
 */
class CommandParser {
public:
  /**
   * @brief Parses an input string into a `ParsedCommand` structure.
   *
   * This method takes a string, splits it into the command name and its
   * arguments, and returns a `ParsedCommand` structure representing the parsed
   * command.
   *
   * @param[in] input The input string to be parsed.
   * @return A `ParsedCommand` structure containing the command name and a
   * vector of arguments.
   */
  ParsedCommand parse(const std::string &input);
};

#endif // COMMAND_PARSER_H
