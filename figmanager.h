//===--- figmanager.h ---------------------------------------------------===//
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

#ifndef __JIG_FIGMANAGER_H__
#define __JIG_FIGMANAGER_H__

#include "fig.h"

namespace jig {

class FigManager {
public:
  FigManager() = default;

  void init(Path path = "");

  Fig *getCurrentFig() { return m_Fig.get(); }
  const Fig *getCurrentFig() const { return m_Fig.get(); }

  bool isInitialized() const { return m_Initialized; }

private:
  void load(Path &&path);

  std::unique_ptr<Fig> m_Fig = nullptr;
  bool m_Initialized = false;
};

} // namespace jig

#endif // __JIG_FIGMANAGER_H__
