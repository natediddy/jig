//===--- titlebar.cc ----------------------------------------------------===//
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

#include "titlebar.h"

#include <assert.h>

#include "app.h"
#include "documentlist.h"
#include "ui.h"

namespace jig {

void TitleBar::init() {
  const auto &docs = App::getInstance().getDocumentList();

  m_Titles.reserve(docs.getTotal());
  std::for_each(docs.begin(), docs.end(), [&](const auto &doc) {
    m_Titles.emplace_back(doc.getTitle(), doc.isDirty());
  });
  initRepr();
  initWindow();
  writeToWindow();
}

void TitleBar::updateDimensions() {
  m_Repr.clear();
  initRepr();
  m_Window->resize(m_Repr.size(), App::getInstance().getUI().getWidth());
  writeToWindow();
}

int TitleBar::getKeypress() {
  return UI::INVALID_INPUT;
}

void TitleBar::update() {
  const auto &docs = App::getInstance().getDocumentList();
  for (std::size_t i = 0; i < docs.getTotal(); ++i)
    m_Titles[i].dirty = docs[i].isDirty();
  updateDimensions();
}

void TitleBar::addTitle(const Document &doc) {
  m_Titles.emplace_back(doc.getTitle(), doc.isDirty());
  updateDimensions();
}

void TitleBar::removeTitle(std::size_t index) {
  assert(index < m_Titles.size() &&
         "Attempted to erase from TitleBar vector with an invalid position.");
  m_Titles.erase(m_Titles.begin() + index);
  updateDimensions();
}

void TitleBar::initWindow() {
  View::initWindow("TitleBar", m_Repr.size(), App::getInstance().getUI().getWidth(), 0, 0);
  m_Window->enableAttrs(Window::Attr::REVERSE);
}

void TitleBar::initRepr() {
  std::size_t repPos = 0;
  int width = App::getInstance().getUI().getWidth();
  int xPos = 0;
  int titleLen;

  m_Repr.push_back(std::vector<const Title *>{});
  std::for_each(m_Titles.begin(), m_Titles.end(), [&](const auto &title) {
    titleLen = title.str.size();
    if (title.dirty)
      ++titleLen;
    if (xPos + titleLen + 2 >= width) {
      m_Repr.push_back(std::vector<const Title *>{});
      ++repPos;
      xPos = 0;
    }
    m_Repr[repPos].push_back(&title);
    xPos += titleLen + 2;
  });
}

void TitleBar::writeToWindow() {
  View::writeToWindow();

  std::size_t currentDocIndex =
    App::getInstance().getDocumentList().getCurrentIndex();
  std::size_t i = 0;
  int y = 0;
  int x = 0;
  int width = getWidth();

  // The TitleBar looks like this:
  //  _______________________________________
  // |[file1.txt][+file2.txt][file3.txt]     |
  // |                                       |
  // |                                       |
  // |                                       |
  // |_______________________________________|
  //
  // A title with a '+' next to it means it has been modified and hasn't been
  // saved yet.
  std::for_each(m_Repr.begin(), m_Repr.end(), [&](const auto &row) {
    std::for_each(row.begin(), row.end(), [&](const auto &title) {
      m_Window->put(y, x++, '[');
      if (i == currentDocIndex)
        m_Window->enableAttrs(Window::Attr::UNDERLINE | Window::Attr::BOLD);
      if (title->dirty)
        m_Window->put(y, x++, '+');
      m_Window->put(y, x, title->str);
      x += title->str.size();
      if (i++ == currentDocIndex)
        m_Window->disableAttrs(Window::Attr::UNDERLINE | Window::Attr::BOLD);
      m_Window->put(y, x++, ']');
    });
    while (x < width)
      m_Window->put(y, x++, ' ');
    ++y;
    x = 0;
  });
}

} // namespace jig
