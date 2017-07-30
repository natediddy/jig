//===--- document.cc ----------------------------------------------------===//
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

#include "document.h"

#include <cstdlib>

#include "app.h"
#include "logger.h"

namespace jig {

Document Document::createEmpty(const std::string &title) {
  Document doc;
  doc.setTitle(title);
  doc.setContentsFromString("\n");
  return doc;
}

Document Document::createFromString(const std::string &str,
                                    const std::string &title) {
  Document doc;
  doc.setTitle(title);
  doc.setContentsFromString(str);
  return doc;
}

Document Document::createFromFile(const std::string &path) {
  Document doc;
  doc.setContentsFromFile(path);
  doc.setTitle(doc.getFile()->getPath().getBasename());
  return doc;
}

Document &Document::insert(char ch) {
  std::unique_ptr<Edit> edit =
    std::make_unique<InsertEdit>(getCursorPosition(), ch);
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  if (ch == '\n') {
    moveCursorDown();
    moveCursorToBeginningOfLine();
  } else {
    moveCursorRight();
  }
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::insert(const std::string &str) {
  std::unique_ptr<Edit> edit =
    std::make_unique<InsertEdit>(getCursorPosition(), str);
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::insert(std::size_t pos, char ch) {
  std::unique_ptr<Edit> edit = std::make_unique<InsertEdit>(pos, ch);
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::insert(std::size_t pos, std::string &&str) {
  std::unique_ptr<Edit> edit =
    std::make_unique<InsertEdit>(pos, std::move(str));
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::eraseBack(std::size_t count) {
  std::size_t pos = getCursorPosition();
  if (pos == 0)
    return *this;

  if (m_Buffer->getStrBuf()[pos - 1] == '\n') {
    moveCursorUp();
    moveCursorToEndOfLine();
  } else {
    moveCursorLeft();
  }

  std::unique_ptr<Edit> edit = std::make_unique<EraseBackEdit>(pos, count);
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::eraseBack(std::size_t pos, std::size_t count) {
  std::unique_ptr<Edit> edit = std::make_unique<EraseBackEdit>(pos, count);
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::eraseFront(std::size_t count) {
  std::unique_ptr<Edit> edit =
    std::make_unique<EraseFrontEdit>(getCursorPosition(), count);
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::eraseFront(std::size_t pos, std::size_t count) {
  std::unique_ptr<Edit> edit = std::make_unique<EraseFrontEdit>(pos, count);
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::replace(std::size_t pos, std::size_t count, char ch) {
  std::unique_ptr<Edit> edit = std::make_unique<ReplaceEdit>(pos, count, ch);
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::replace(std::size_t pos, std::size_t count,
                            std::string &&str) {
  std::unique_ptr<Edit> edit =
    std::make_unique<ReplaceEdit>(pos, count, std::move(str));
  edit->apply(*m_Buffer);
  m_EditHistory.addNew(std::move(edit));
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::undo() {
  if (!canUndo())
    return *this;
  m_EditHistory.undo(*m_Buffer);
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

Document &Document::redo() {
  if (!canRedo())
    return *this;
  m_EditHistory.redo(*m_Buffer);
  App::getInstance().getUI().getBufferView().clear();
  m_Dirty = true;
  return *this;
}

void Document::moveCursorLeft() {
  auto &app = App::getInstance();

  if (m_ViewData.cursorX == 0) {
    if (m_ViewData.offsetX == 0)
      return;
    --m_ViewData.offsetX;
    app.getUI().getBufferView().clear();
    goto finish;
  }

  --m_ViewData.cursorX;

finish:
  --m_ViewData.pos;
  if (app.getCurrentMode() == App::Mode::SELECT)
    app.getSelectModeHandler().moveLeft();
}

void Document::moveCursorLeft(int n) {
  for (; n != 0; --n)
    moveCursorLeft();
}

void Document::moveCursorRight() {
  auto line = m_Buffer->getConstLineIterator(m_ViewData.lineIndex);

  if (m_ViewData.pos < line->length()) {
    auto &app = App::getInstance();
    auto &view = app.getUI().getBufferView();
    if (m_ViewData.cursorX < view.getWidth()) {
      ++m_ViewData.cursorX;
    } else {
      ++m_ViewData.offsetX;
      view.clear();
    }
    ++m_ViewData.pos;
    if (app.getCurrentMode() == App::Mode::SELECT)
      app.getSelectModeHandler().moveRight(*this);
  }
}

void Document::moveCursorRight(int n) {
  for (; n != 0; --n)
    moveCursorRight();
}

void Document::moveCursorUp() {
  auto line = m_Buffer->getConstLineIterator(m_ViewData.lineIndex);

  if (line == m_Buffer->getFirstLineIterator())
    return;

  --line;
  --m_ViewData.lineIndex;

  auto &app = App::getInstance();
  App::Mode mode = app.getCurrentMode();

  if (mode == App::Mode::SELECT)
    app.getSelectModeHandler().moveLeft(m_ViewData.pos);

  if (line->length() < m_ViewData.pos) {
    m_ViewData.pos = line->length();
    m_ViewData.cursorX = m_ViewData.pos;
  } else if (mode == App::Mode::SELECT) {
    app.getSelectModeHandler().moveLeft(line->length() - m_ViewData.pos + 1);
  }

  if (m_ViewData.cursorY > 0) {
    --m_ViewData.cursorY;
  } else if (m_ViewData.offsetY > 0) {
    --m_ViewData.offsetY;
    app.getUI().getBufferView().clear();
  }
}

void Document::moveCursorUp(int n) {
  for (; n != 0; --n)
    moveCursorUp();
}

void Document::moveCursorDown() {
  auto line = m_Buffer->getConstLineIterator(m_ViewData.lineIndex);

  if (line == m_Buffer->getLastLineIterator())
    return;

  auto &app = App::getInstance();
  App::Mode mode = app.getCurrentMode();

  if (mode == App::Mode::SELECT)
    app.getSelectModeHandler().moveRight(*this,
                                         line->length() - m_ViewData.pos);

  ++line;
  ++m_ViewData.lineIndex;

  if (m_ViewData.pos > line->length()) {
    m_ViewData.pos = line->length();
    m_ViewData.cursorX = m_ViewData.pos;
  } else if (mode == App::Mode::SELECT) {
    app.getSelectModeHandler().moveRight(*this, m_ViewData.pos + 1);
  }

  auto &view = App::getInstance().getUI().getBufferView();
  if (m_ViewData.cursorY < view.getHeight() - 1) {
    ++m_ViewData.cursorY;
  } else {
    ++m_ViewData.offsetY;
    view.clear();
  }
}

void Document::moveCursorDown(int n) {
  for (; n != 0; --n)
    moveCursorDown();
}

void Document::moveCursorToBeginningOfLine() {
  moveCursorLeft(m_ViewData.pos);
}

void Document::moveCursorToEndOfLine() {
  moveCursorRight(
    m_Buffer->getConstLineIterator(m_ViewData.lineIndex)->length() -
    m_ViewData.pos);
}

std::size_t Document::getCursorPosition() const {
  std::size_t pos = 0;
  std::for_each(m_Buffer->getFirstLineIterator(),
                m_Buffer->getConstLineIterator(m_ViewData.lineIndex),
                [&pos](const auto &line) { pos += line.length() + 1; });
  pos += m_ViewData.pos;
  return pos;
}

unsigned int Document::getViewPortion() const {
  auto &ui = App::getInstance().getUI();
  if (!ui.isCurrentlyRunning() || m_ViewData.offsetY == 0)
    return TOP;

  Buffer::ConstLineIterator lastVisible = m_Buffer->getLastLineIterator();
  int height = ui.getBufferView().getHeight();
  auto n = lastVisible - m_Buffer->getConstLineIterator(m_ViewData.offsetY);

  if (n < height)
    return BOTTOM;

  lastVisible -= n - height;

  double frac =
    static_cast<double>(lastVisible - m_Buffer->getFirstLineIterator()) /
    static_cast<double>(m_Buffer->getTotalLines());
  return static_cast<unsigned int>(frac * 100.0f);
}

void Document::save() {
  m_File->writeContents(m_Buffer->getStrBuf());
  m_Dirty = false;
}

void Document::setContentsFromString(const std::string &str) {
  m_Buffer = std::make_unique<Buffer>(str);
}

void Document::setContentsFromFile(const std::string &path) {
  m_File = std::make_unique<File>(path);
  if (m_File->hadError()) {
    Logger::fatal("failed to initialize File object `%s' -- %s",
                  m_File->getPath().getCString(), m_File->errorMessage());
    std::exit(EXIT_FAILURE);
  }

  if (!m_File->exists()) {
    setContentsFromString("\n");
    return;
  }

  std::string contents{m_File->readContents()};
  if (m_File->hadError()) {
    Logger::fatal("failed to read contents from `%s' -- %s",
                  m_File->getPath().getCString(), m_File->errorMessage());
    std::exit(EXIT_FAILURE);
  }

  m_Buffer = std::make_unique<Buffer>(contents);
}

} // namespace jig
