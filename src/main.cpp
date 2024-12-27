#include "commands.hpp"
#include <cstdlib>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct PATH isInPath(std::string program) {
  struct PATH res;
  auto path = split(getenv("PATH"), ':');
  res.dir = "";
  res.exists = false;
  for (auto val : path) {
    std::ifstream file;
    file.open(val + "/" + program);
    if (file) {
      res.dir = val + "/" + program;
      res.exists = true;
      return res;
    }
  }
  return res;
}

std::unordered_set<std::string> parse_env(const std::string env) {
  std::unordered_set<std::string> result{};

  auto paths{split(env, ':')};

  for (auto val : paths) {
    result.insert(val);
  }

  return result;
};

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
  std::stringstream ss;
  for (size_t i = 0; i < vect.size(); ++i) {
    ss << vect[i];
    if (i != vect.size() - 1) {
      ss << separator;
    }
  }
  return ss.str();
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
  case eUnkown: {
    auto program = isInPath(parsed[0]);
    if (program.exists) {
      if (parsed.size() > 1) {
        auto program_string{join(parsed, ' ')};
        int call = system(program_string.c_str());
      } else {
        int call = system(program.dir.c_str());
      }
      auto e = std::make_unique<UserProgram>();
      return e;
    }
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
