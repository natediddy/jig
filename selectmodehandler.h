//===--- selectmodehandler.h --------------------------------------------===//
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

#ifndef __JIG_SELECTMODEHANDLER_H__
#define __JIG_SELECTMODEHANDLER_H__

#include <string>

namespace jig {

class Document;

class SelectModeHandler {
public:
  using Selection = std::pair<std::size_t, std::size_t>;

  SelectModeHandler() = default;

  void init(const Document &doc);
  void reset() { m_Head = m_Tail = 0; }

  std::size_t getHead() const { return m_Head; }
  std::size_t getTail() const { return m_Tail; }

  Selection getSelection() const;

  bool isCursorWithinSelection(std::size_t cursorPos) const {
    Selection selection = getSelection();
    return cursorPos >= selection.first && cursorPos <= selection.second;
  }

  bool isCursorWithinSelection(const Selection &selection,
                               std::size_t cursorPos) const {
    return cursorPos >= selection.first && cursorPos <= selection.second;
  }

  std::size_t getLength() const;
  std::string getText(const Document &doc) const;
  void eraseText(Document &doc);

  void moveLeft();
  void moveLeft(int n);

  void moveRight(const Document &doc);
  void moveRight(const Document &doc, int n);

private:
  std::size_t m_Head = 0;
  std::size_t m_Tail = 0;
};

} // namespace jig

#endif // __JIG_SELECTMODEHANDLER_H__
