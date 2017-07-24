//===--- logger.h -------------------------------------------------------===//
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

#ifndef __JIG_LOGGER_H__
#define __JIG_LOGGER_H__

#ifndef NDEBUG
#define JIG_DEBUG(...)                                                         \
  jig::Logger::debug(__FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)
#else
#define JIG_DEBUG(...)
#endif

namespace jig {

struct Logger {
  static void init();
  static void terminate();
  static void info(const char *fmt, ...);
  static void warn(const char *fmt, ...);
  static void error(const char *fmt, ...);
  static void fatal(const char *fmt, ...);
#ifndef NDEBUG
  static void debug(const char *file, int line, const char *func,
                    const char *fmt, ...);
#endif
};

} // namespace jig

#endif // __JIG_LOGGER_H__
