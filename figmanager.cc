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
  if (tryFigPath(System::getSystemFigPath(), "system-wide"))
    return;

  Logger::info("loading builtin fig");
  m_Fig = std::make_unique<Fig>();
}

} // namespace jig
