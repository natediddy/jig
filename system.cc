//===--- system.cc ------------------------------------------------------===//
// Copyright (c) 2017 Nathan Forbes
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//===--------------------------------------------------------------------===//

#include "system.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <sys/stat.h>
#include <unistd.h>

#include "app.h"
#include "logger.h"

namespace jig {
namespace {

constexpr char CONFIG_FILENAME[] = "config";
constexpr char LOG_FILENAME[] = "log";

} // namespace

const char *System::getEnvironmentVariable(const char *key) {
  return std::getenv(key);
}

Path System::getCurrentDirectory() {
  char cwd[1024];
  if (getcwd(cwd, 1024))
    return Path{cwd};
  return Path{"."};
}

Path System::getHomeDirectory() {
  return Path{getEnvironmentVariable("HOME")};
}

Path System::getProgramDirectory() {
  std::string base{"."};
  base += App::PROGRAM_NAME;
  return getHomeDirectory() + base;
}

Path System::getConfigPath() {
  return getProgramDirectory() + CONFIG_FILENAME;
}

Path System::getLogPath() {
  return getProgramDirectory() + LOG_FILENAME;
}

void System::makeDirectory(const Path &path) {
  if (File::directoryExists(path))
    return;

  const std::string &str = path.getString();
  std::string s;
  std::size_t n = str.size();

  for (std::size_t i = 0; i < n; ++i) {
    if (str[i] == Path::SEPARATOR) {
      s = str.substr(0, i + 1);
      if (File::directoryExists(s))
        continue;
      if (mkdir(s.c_str(), S_IRWXU) != 0) {
        Logger::fatal("failed to create directory `%s' -- %s", s.c_str(),
                      std::strerror(errno));
        std::exit(EXIT_FAILURE);
      }
    }
  }

  if (mkdir(str.c_str(), S_IRWXU) != 0) {
    Logger::fatal("failed to create directory `%s' -- %s", str.c_str(),
                  std::strerror(errno));
    std::exit(EXIT_FAILURE);
  }
}

} // namespace jig
