//===--- ui.h -----------------------------------------------------------===//
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

#ifndef __JIG_UI_H__
#define __JIG_UI_H__

#include "bufferview.h"
#include "linenumbercolumn.h"
#include "statusbar.h"
#include "titlebar.h"

namespace jig {

class UI {
public:
  static const int INVALID_INPUT;

  UI() = default;
  ~UI() { stop(); }

  void start();
  void stop();
  void updateDimensions();
  void draw();
  void handleInput();

  int getHeight() const { return m_Height; }
  int getWidth() const { return m_Width; }
  bool isCurrentlyRunning() const { return m_Running; }

  TitleBar &getTitleBar() { return m_TitleBar; }
  const TitleBar &getTitleBar() const { return m_TitleBar; }

  StatusBar &getStatusBar() { return m_StatusBar; }
  const StatusBar &getStatusBar() const { return m_StatusBar; }

  BufferView &getBufferView() { return m_BufferView; }
  const BufferView &getBufferView() const { return m_BufferView; }

  LineNumberColumn *getLineNumberColumn() { return m_LineNumberColumn.get(); }
  const LineNumberColumn *getLineNumberColumn() const {
    return m_LineNumberColumn.get();
  }

private:
  void update(bool updateTitleBar, bool updateStatusBar, bool updateBufferView);

  TitleBar m_TitleBar;
  StatusBar m_StatusBar;
  BufferView m_BufferView;
  std::unique_ptr<LineNumberColumn> m_LineNumberColumn = nullptr;
  int m_Height = 0;
  int m_Width = 0;
  bool m_Running = false;
};

} // namespace jig

#endif // __JIG_UI_H__
