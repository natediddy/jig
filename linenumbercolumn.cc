//===--- linenumbercolumn.cc --------------------------------------------===//
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

#include "linenumbercolumn.h"

#include <cstring>

#include "app.h"

namespace jig {
namespace {

// n = number of lines in a single buffer
int getNumberOfDigits(std::size_t n) {
  if (n < 10)
    return 1;
  if (n < 100)
    return 2;
  if (n < 1000)
    return 3;
  if (n < 10000)
    return 4;
  if (n < 100000)
    return 5; // Surely not...
  if (n < 1000000)
    return 6; // This would be crazy...
  if (n < 10000000)
    return 7; // But just in case...
  return 8;   // Whoa...
}

} // namespace

void LineNumberColumn::init() {
  update();
  initWindow();
  writeToWindow();
}

void LineNumberColumn::updateDimensions() {
  UI &ui = App::getInstance().getUI();
  int titleBarHeight = ui.getTitleBar().getHeight();
  int h = ui.getHeight() - ui.getStatusBar().getHeight() - titleBarHeight;
  m_Window->resize(h, m_MaxDigits);
  m_Window->move(titleBarHeight, 0);
  writeToWindow();
}

int LineNumberColumn::getKeypress() {
  return UI::INVALID_INPUT;
}

void LineNumberColumn::update() {
  const Document &doc = App::getInstance().getDocumentList().getCurrent();
  const Buffer *buffer = doc.getBuffer();
  m_FirstLineIndex =
    buffer->getConstLineIterator(doc.getBufferViewData()->offsetY) -
    buffer->getFirstLineIterator();
  m_TotalLines = buffer->getTotalLines();
  m_MaxDigits = getNumberOfDigits(m_TotalLines);
  if (m_Window)
    writeToWindow();
}

void LineNumberColumn::initWindow() {
  UI &ui = App::getInstance().getUI();
  int titleBarHeight = ui.getTitleBar().getHeight();
  int h = ui.getHeight() - ui.getStatusBar().getHeight() - titleBarHeight;
  View::initWindow("LineNumberColumn", h, m_MaxDigits + 1, titleBarHeight, 0);
  m_Window->enableAttrs(Window::Attr::REVERSE | Window::Attr::BOLD);
}

void LineNumberColumn::writeToWindow() {
  std::size_t lineNumber = m_FirstLineIndex + 1;
  int height = getHeight();
  int last = getWidth() - 1;
  int nDigits;
  int nSpaces;
  char space[m_MaxDigits];

  std::memset(space, ' ', m_MaxDigits);
  for (int y = 0; y < height; ++y) {
    if (lineNumber > m_TotalLines) {
      m_Window->put(y, 0, space, m_MaxDigits);
      m_Window->put(y, last, *space);
      continue;
    }
    nDigits = getNumberOfDigits(lineNumber);
    nSpaces = m_MaxDigits - nDigits;
    m_Window->put(y, 0, space, nSpaces);
    m_Window->putf(y, nSpaces, "%zu", lineNumber++);
    m_Window->put(y, last, *space);
  }
}

} // namespace jig
