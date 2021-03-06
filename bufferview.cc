//===--- bufferview.cc --------------------------------------------------===//
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

#include "bufferview.h"

#include <assert.h>

#include "app.h"
#include "logger.h"
#include "buffer.h"

namespace jig {

void BufferView::init() {
  initWindow();
  update();
}

void BufferView::updateDimensions() {
  UI &ui = App::getInstance().getUI();
  int titleBarHeight = ui.getTitleBar().getHeight();
  int h = ui.getHeight() - ui.getStatusBar().getHeight() - titleBarHeight;
  m_Window->resize(h, ui.getWidth());
  m_Window->move(titleBarHeight, 0);
  writeToWindow();
}

void BufferView::update() {
  const Document &doc = App::getInstance().getDocumentList().getCurrent();
  m_Buffer = doc.getBuffer();
  m_Data = doc.getBufferViewData();
  writeToWindow();
}

void BufferView::initWindow() {
  App &app = App::getInstance();
  UI &ui = app.getUI();
  int titleBarHeight = ui.getTitleBar().getHeight();
  int h = ui.getHeight() - ui.getStatusBar().getHeight() - titleBarHeight;
  int w = ui.getWidth();
  int x = 0;

  if (app.getFig()->get<bool>("ShowLineNumbers")) {
    Logger::info("ShowLineNumbers enabled");
    int lineNumberColumnWidth = ui.getLineNumberColumn()->getWidth();
    w -= lineNumberColumnWidth;
    x += lineNumberColumnWidth;
  }

  View::initWindow("BufferView", h, w, titleBarHeight, x);
  m_Window->enableKeypad();
}

void BufferView::writeToWindow() {
  View::writeToWindow();

  auto first = m_Buffer->getConstLineIterator(m_Data->offsetY);
  auto last = m_Buffer->getLastLineIterator();
  std::size_t n = last - first;
  int height = getHeight();
  int width = getWidth();

  if (n > height)
    last -= n - height;

  std::size_t p = first->begin() - m_Buffer->getStrBuf().begin();
  int y = 0;
  int x;

  auto &app = App::getInstance();
  App::Mode mode = app.getCurrentMode();
  auto smh = app.getSelectModeHandler();
  auto selection = smh.getSelection();

  std::for_each(first, last + 1, [&](const Line &l) {
    auto len = l.size();
    if (m_Data->offsetX >= len) {
      bool selected = (mode == App::Mode::SELECT) &&
                      smh.isCursorWithinSelection(selection, p);
      if (selected)
        m_Window->enableAttrs(Window::Attr::REVERSE);
      m_Window->put(y, 0, ' ');
      if (selected)
        m_Window->disableAttrs(Window::Attr::REVERSE);
      p += len + 1;
      ++y;
      return;
    }
    p += m_Data->offsetX;
    auto b = l.begin() + m_Data->offsetX;
    auto e = l.end();
    n = e - b;
    if (n > width)
      e -= n - width;
    x = 0;
    if (mode == App::Mode::SELECT) {
      std::for_each(b, e, [&](const char &c) {
        bool selected = smh.isCursorWithinSelection(selection, p);
        if (selected)
          m_Window->enableAttrs(Window::Attr::REVERSE);
        m_Window->put(y, x++, c);
        if (selected)
          m_Window->disableAttrs(Window::Attr::REVERSE);
        ++p;
      });
    } else {
      std::for_each(b, e, [&](const char &c) {
        m_Window->put(y, x++, c);
        ++p;
      });
    }
    ++p;
    ++y;
  });

  m_Window->moveCursor(m_Data->cursorY, m_Data->cursorX);
}

} // namespace jig
