#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum allowed_commands { eEcho, eExit, eUnkown };

allowed_commands get_command(const std::string cmd);

std::vector<std::string> split(const std::string &text, const char delim);

class ICommand {
public:
  virtual void exec() = 0;
  virtual ~ICommand(){};
};

class Echo : public ICommand {
private:
  std::string message;

public:
  Echo(std::string message) : message{message} {};

  void exec() { std::cout << message << '\n'; };

  ~Echo(){};
};

class NotKnown : public ICommand {
private:
  std::string cmd;

public:
  NotKnown(std::string cmd) : cmd{cmd} {};

  void exec() { std::cout << cmd << ": command not found\n"; };

  ~NotKnown(){};
};

std::unique_ptr<ICommand> parse_input(const std::string in);
