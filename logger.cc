//===--- logger.cc ------------------------------------------------------===//
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

#include "logger.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "app.h"
#include "system.h"
#include "timeutils.h"

namespace jig {
namespace {

std::FILE *logPtr = nullptr;
time::Timer timer;

} // namespace

void Logger::init() {
  Path path{System::getLogPath()};

  logPtr = std::fopen(path.getCString(), "a+");
  if (!logPtr) {
    std::fprintf(stderr, "%s: error: failed to open log `%s' -- %s",
                 App::getInstance().getProgramName(), path.getCString(),
                 std::strerror(errno));
    std::exit(EXIT_FAILURE);
  }

  timer.start();
  std::fprintf(logPtr, "Initializing %s (%s)\n", App::DISPLAY_NAME,
               time::getDateTimeFormatString().c_str());
}

void Logger::terminate() {
  timer.stop();
  std::fprintf(logPtr, "Terminating %s (runtime: %s)\n", App::DISPLAY_NAME,
               timer.getFormatString().c_str());
}

void Logger::info(const char *fmt, ...) {
  std::fputs("\tINFO: ", logPtr);
  va_list args;
  va_start(args, fmt);
  std::vfprintf(logPtr, fmt, args);
  va_end(args);
  std::fputc('\n', logPtr);
}

void Logger::warn(const char *fmt, ...) {
  std::fputs("\tWARNING: ", logPtr);
  va_list args;
  va_start(args, fmt);
  std::vfprintf(logPtr, fmt, args);
  va_end(args);
  std::fputc('\n', logPtr);
}

void Logger::error(const char *fmt, ...) {
  std::fputs("\tERROR: ", logPtr);
  va_list args;
  va_start(args, fmt);
  std::vfprintf(logPtr, fmt, args);
  va_end(args);
  std::fputc('\n', logPtr);
}

void Logger::fatal(const char *fmt, ...) {
  std::fputs("\tFATAL: ", logPtr);
  va_list args;
  va_start(args, fmt);
  std::vfprintf(logPtr, fmt, args);
  va_end(args);
  std::fputc('\n', logPtr);
}

#ifndef NDEBUG
void Logger::debug(const char *file, int line, const char *func,
                   const char *fmt, ...) {
  std::fprintf(logPtr, "\tDEBUG:%s:%d:%s: ", file, line, func);
  va_list args;
  va_start(args, fmt);
  std::vfprintf(logPtr, fmt, args);
  va_end(args);
  std::fputc('\n', logPtr);
}
#endif

} // namespace jig
