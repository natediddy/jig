//===--- edit.cc --------------------------------------------------------===//
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

#include "edit.h"

#include "buffer.h"

namespace jig {

void InsertEdit::apply(Buffer &buffer) {
  buffer.insert(m_Pos, m_Inserted);
  m_Applied = true;
}

void InsertEdit::undo(Buffer &buffer) {
  buffer.erase(m_Pos, m_Inserted.size());
  m_Applied = false;
}

void EraseBackEdit::apply(Buffer &buffer) {
  if (m_Count > m_Pos)
    return;
  m_Erased = buffer.getStringAt(m_Pos - m_Count, m_Count);
  buffer.erase(m_Pos - m_Count, m_Count);
  m_Applied = true;
}

void EraseBackEdit::undo(Buffer &buffer) {
  if (!m_Applied)
    return;
  buffer.insert(m_Pos - m_Count, m_Erased);
  m_Applied = false;
}

void EraseFrontEdit::apply(Buffer &buffer) {
  if (m_Count > buffer.size() - m_Pos)
    return;
  m_Erased = buffer.getStringAt(m_Pos, m_Count);
  buffer.erase(m_Pos, m_Count);
  m_Applied = true;
}

void EraseFrontEdit::undo(Buffer &buffer) {
  if (!m_Applied)
    return;
  buffer.insert(m_Pos, m_Erased);
  m_Applied = false;
}

void ReplaceEdit::apply(Buffer &buffer) {
  m_Erased = buffer.getStringAt(m_Pos, m_Count);
  buffer.replace(m_Pos, m_Count, m_Inserted);
  m_Applied = true;
}

void ReplaceEdit::undo(Buffer &buffer) {
  buffer.replace(m_Pos, m_Inserted.size(), m_Erased);
  m_Applied = false;
}

} // namespace jig
