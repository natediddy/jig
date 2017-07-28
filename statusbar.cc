//===--- statusbar.cc ---------------------------------------------------===//
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

#include "statusbar.h"

#include "app.h"
#include "util.h"

namespace jig {
namespace {

constexpr char TOP_STRING[] = "Top";
constexpr char BOTTOM_STRING[] = "Bottom";

} // namespace

void StatusBar::init() {
  initWindow();
  update();
  writeToWindow();
}

void StatusBar::updateDimensions() {
  UI &ui = App::getInstance().getUI();
  m_Window->resize(1, ui.getWidth());
  m_Window->move(ui.getHeight() - 1, 0);
  writeToWindow();
}

int StatusBar::getKeypress() {
  return UI::INVALID_INPUT;
}

void StatusBar::update() {
  const Document &doc = App::getInstance().getDocumentList().getCurrent();
  m_Text = doc.getTitle();
#ifndef NDEBUG
  m_BufferPos = doc.getCursorPosition();
#endif
  m_LineNumber = doc.getCursorLineNumber();
  m_ColumnNumber = doc.getCursorColumnNumber();
  m_Portion = doc.getViewPortion();
  writeToWindow();
}

void StatusBar::initWindow() {
  UI &ui = App::getInstance().getUI();
  View::initWindow(1, ui.getWidth(), ui.getHeight() - 1, 0);
  m_Window->enableAttrs(Window::Attr::BOLD | Window::Attr::REVERSE);
}

void StatusBar::writeToWindow() {
  static constexpr std::size_t DATA_STR_SIZE = 256;
  static char dataStr[DATA_STR_SIZE];

  View::writeToWindow();
  m_Window->put(0, 0, m_Text);

#ifndef NDEBUG
  switch (m_Portion) {
    case Document::TOP:
      snprintf(dataStr, DATA_STR_SIZE, "%u,%u (%u) %s", m_LineNumber,
               m_ColumnNumber, m_BufferPos, TOP_STRING);
      break;
    case Document::BOTTOM:
      snprintf(dataStr, DATA_STR_SIZE, "%u,%u (%u) %s", m_LineNumber,
               m_ColumnNumber, m_BufferPos, BOTTOM_STRING);
      break;
    default:
      snprintf(dataStr, DATA_STR_SIZE, "%u,%u (%u) %%%u", m_LineNumber,
               m_ColumnNumber, m_BufferPos, m_Portion);
      break;
  }
#else
  switch (m_Portion) {
    case Document::TOP:
      snprintf(dataStr, DATA_STR_SIZE, "%u,%u %s", m_LineNumber, m_ColumnNumber,
               TOP_STRING);
      break;
    case Document::BOTTOM:
      snprintf(dataStr, DATA_STR_SIZE, "%u,%u %s", m_LineNumber, m_ColumnNumber,
               BOTTOM_STRING);
      break;
    default:
      snprintf(dataStr, DATA_STR_SIZE, "%u,%u %%%u", m_LineNumber,
               m_ColumnNumber, m_Portion);
      break;
  }
#endif

  auto n = std::strlen(dataStr);
  int stop = getWidth() - n;
  int x;
  for (x = m_Text.size(); x < stop; ++x)
    m_Window->put(0, x, ' ');
  m_Window->put(0, x, dataStr, n);
}

} // namespace jig
