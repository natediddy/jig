//===--- document.h -----------------------------------------------------===//
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

#ifndef __JIG_DOCUMENT_H__
#define __JIG_DOCUMENT_H__

#include "buffer.h"
#include "bufferview.h"
#include "edit.h"
#include "edithistory.h"
#include "file.h"
#include "statusbar.h"

namespace jig {

class Document {
public:
  static constexpr unsigned int TOP = 0U;
  static constexpr unsigned int BOTTOM = 100U;

  static Document createEmpty(const std::string &title);
  static Document createFromString(const std::string &str,
                                   const std::string &title);
  static Document createFromFile(const std::string &path);

  Document(Document &&) = default;
  Document &operator=(Document &&) = default;

  Buffer *getBuffer() { return m_Buffer.get(); }
  const Buffer *getBuffer() const { return m_Buffer.get(); }

  const File *getFile() const { return m_File.get(); }

  const BufferView::Data *getBufferViewData() const { return &m_ViewData; }

  const std::string &getTitle() const { return m_Title; }
  void setTitle(const std::string &title) { m_Title = title; }
  void setTitle(std::string &&title) { m_Title = std::move(title); }

  Document &insert(char ch);
  Document &insert(const std::string &str);
  Document &insert(std::size_t pos, char ch);
  Document &insert(std::size_t pos, std::string &&str);

  Document &eraseBack(std::size_t count);
  Document &eraseBack(std::size_t pos, std::size_t count);

  Document &eraseFront(std::size_t count);
  Document &eraseFront(std::size_t pos, std::size_t count);

  Document &replace(std::size_t pos, std::size_t count, char ch);
  Document &replace(std::size_t pos, std::size_t count, std::string &&str);

  Document &undo();
  Document &redo();

  bool canUndo() const { return m_EditHistory.canUndo(); }
  bool canRedo() const { return m_EditHistory.canRedo(); }

  void moveCursorLeft();
  void moveCursorLeft(int n);

  void moveCursorRight();
  void moveCursorRight(int n);

  void moveCursorUp();
  void moveCursorUp(int n);

  void moveCursorDown();
  void moveCursorDown(int n);

  void moveCursorToBeginningOfLine();
  void moveCursorToEndOfLine();

  unsigned int getCursorLineNumber() const {
    return m_Buffer->getConstLineIterator(m_ViewData.lineIndex) -
           m_Buffer->getFirstLineIterator() + 1;
  }

  unsigned int getCursorColumnNumber() const { return m_ViewData.pos + 1; }

  std::size_t getCursorPosition() const;

  unsigned int getViewPortion() const;

  bool isDirty() const { return m_Dirty; }

  void save();

private:
  Document() = default;

  void setContentsFromString(const std::string &str);
  void setContentsFromFile(const std::string &path);

  std::unique_ptr<Buffer> m_Buffer = nullptr;
  std::unique_ptr<File> m_File = nullptr;

  // A list of all edits to this Document.
  EditHistory m_EditHistory;

  // The title shown in the TitleBar.
  std::string m_Title;

  // Each Document should have it's own instance of a BufferView::Data object.
  // There is only one BufferView instance in the entire program, but when it's
  // time to draw a Document's contents to the screen, the BufferView will grab
  // a const pointer of this member variable from the current Document.
  BufferView::Data m_ViewData;

  // Has this Document been modified since the last save?
  bool m_Dirty = false;
};

} // namespace jig

#endif // __JIG_DOCUMENT_H__
