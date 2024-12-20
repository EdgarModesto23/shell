#include "commands.hpp"
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

allowed_commands get_command(const std::string cmd) {
  if (cmd == "echo") {
    return eEcho;
  };
  if (cmd == "type") {
    return eType;
  }
  if (cmd == "exit") {
    return eExit;
  };
  return eUnkown;
}

std::string join(const std::vector<std::string> &vect, char separator) {
  std::string joined{};

  for (auto val : vect) {
    joined.append(val);
    joined.push_back(separator);
  }

  joined.pop_back();

  return joined;
}

std::vector<std::string> split(const std::string &str, const char delim) {
  std::vector<std::string> strings;
  size_t start;
  size_t end = 0;
  while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
    end = str.find(delim, start);
    strings.push_back(str.substr(start, end - start));
  }
  return strings;
}

std::unique_ptr<ICommand> parse_input(const std::string in) {
  std::vector<std::string> parsed{split(in, ' ')};
  auto cmd{get_command(parsed[0])};
  switch (cmd) {
  case eEcho: {
    std::vector<std::string> v(parsed.begin() + 1, parsed.end());
    auto e = std::make_unique<Echo>(join(v, ' '));
    return e;
  }
  case eType: {
    auto e = std::make_unique<Type>(parsed[1]);
    return e;
  }
  case eExit: {
    exit(0);
  }
  default: {
    auto e = std::make_unique<NotKnown>(parsed[0]);
    return e;
  }
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    auto command{parse_input(input)};
    command->exec();

    // std::cout << input << ": command not found\n";
  }
}
