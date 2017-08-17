//===--- view.h ---------------------------------------------------------===//
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

#ifndef __JIG_VIEW_H__
#define __JIG_VIEW_H__

#include "window.h"

namespace jig {

class View {
public:
  View() = default;
  virtual ~View() {}

  virtual void init() = 0;
  virtual void updateDimensions() = 0;

  void draw();
  void clear();

  int getHeight() const;
  int getWidth() const;

  int getStartY() const;
  int getStartX() const;

  virtual int getKeypress();

protected:
  void initWindow(const char *name, int height, int width, int startY,
                  int startX);
  void writeToWindow();

  std::unique_ptr<Window> m_Window = nullptr;
};

} // namespace jig

#endif // __JIG_VIEW_H__
