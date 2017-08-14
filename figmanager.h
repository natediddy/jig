#ifndef __JIG_FIGMANAGER_H__
#define __JIG_FIGMANAGER_H__

#include "fig.h"

namespace jig {

class FigManager {
public:
  FigManager() = default;

  void init(Path path = "");
  Fig *getCurrentFig() { return m_Fig.get(); }

  bool isInitialized() const { return m_Initialized; }

private:
  void load(Path &&path);

  std::unique_ptr<Fig> m_Fig = nullptr;
  bool m_Initialized = false;
};

} // namespace jig

#endif // __JIG_FIGMANAGER_H__
