#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

enum allowed_commands { eEcho, eType, eExit, eUnkown };

struct PATH {
  std::string dir;
  bool exists;
};

allowed_commands get_command(const std::string cmd);

PATH isInPath(std::string);

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

class Type : public ICommand {
private:
  std::string cmd;

public:
  Type(std::string cmd) : cmd{cmd} {};

  void exec() {
    auto t{get_command(cmd)};
    if (t != eUnkown) {
      std::cout << cmd << " is a shell builtin\n";
      return;
    }
    auto pathProgram = isInPath(cmd);
    if (pathProgram.exists) {
      std::cout << cmd << " is " << pathProgram.dir << '\n';
      return;
    }

    std::cout << cmd << ": not found\n";
  };
};
class UserProgram : public ICommand {
public:
  UserProgram(){};

  void exec() {};

  ~UserProgram(){};
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

std::unordered_set<std::string> parse_env(const std::string);

class Environment {

public:
  std::unordered_set<std::string> path;
  Environment(std::string env) : path{parse_env(env)} {};
};
