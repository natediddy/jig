//===--- bufferview.h ---------------------------------------------------===//
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

#ifndef __JIG_BUFFERVIEW_H__
#define __JIG_BUFFERVIEW_H__

#include "buffer.h"
#include "view.h"

namespace jig {

class BufferView : public View {
public:
  struct Data {
    std::size_t lineIndex = 0;
    std::size_t pos = 0;
    std::size_t offsetY = 0;
    std::size_t offsetX = 0;
    int cursorY = 0;
    int cursorX = 0;
  };

  BufferView() = default;

  virtual void init() final;
  virtual void updateDimensions() final;

  void update();

private:
  void initWindow();
  void writeToWindow();

  const Buffer *m_Buffer = nullptr;
  const Data *m_Data = nullptr;
};

} // namespace jig

#endif // __JIG_BUFFERVIEW_H__
