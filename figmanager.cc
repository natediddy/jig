//===--- figmanager.cc --------------------------------------------------===//
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

#include "figmanager.h"

#include "buffer.h"
#include "file.h"
#include "logger.h"
#include "strutils.h"
#include "system.h"

namespace jig {

void FigManager::init(Path path /*=""*/) {
  load(std::move(path));
  m_Initialized = true;
}

void FigManager::load(Path &&path) {
  if (!path.isEmpty()) {
    if (File::fileExists(path)) {
      m_Fig = std::make_unique<Fig>(path);
      Logger::info("loading user specified fig: `%s'", path.getCString());
      return;
    }
  }

  auto tryFigPath = [&](Path pathToTry, const char *whereInfo) -> bool {
    if (File::fileExists(pathToTry)) {
      m_Fig = std::make_unique<Fig>(pathToTry);
      Logger::info("loading %s fig: `%s'", whereInfo, pathToTry.getCString());
      return true;
    }
    return false;
  };

  if (tryFigPath(System::getLocalFigPath(), "local"))
    return;
  if (tryFigPath(System::getUserFigPath(), "user-wide"))
    return;

  Logger::info("loading builtin fig");
  m_Fig = std::make_unique<Fig>();
}

} // namespace jig
