//===--- system.h -------------------------------------------------------===//
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

#ifndef __JIG_SYSTEM_H__
#define __JIG_SYSTEM_H__

#include "path.h"

namespace jig {

struct System {
  static const char *getEnvironmentVariable(const char *key);

  static Path getCurrentDirectory();
  static Path getHomeDirectory();
  static Path getProgramDirectory();

  static Path getUserFigPath();
  static Path getLocalFigPath();

  static Path getLogPath();

  static void makeDirectory(const Path &path);
};

} // namespace jig

#endif // __JIG_SYSTEM_H__
