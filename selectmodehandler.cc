//===--- selectmodehandler.cc -------------------------------------------===//
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

#include "selectmodehandler.h"

#include <assert.h>

#include "app.h"
#include "document.h"
#include "logger.h"

namespace jig {
namespace {

std::pair<std::size_t, std::size_t> maybeSwap(std::size_t head,
                                              std::size_t tail) {
  if (tail < head)
    return std::make_pair(tail, head);
  return std::make_pair(head, tail);
}

} // namespace

void SelectModeHandler::init(const Document &doc) {
  m_Head = m_Tail = doc.getCursorPosition();
  JIG_DEBUG("m_Head = m_Tail = %zu", m_Head);
}

std::pair<std::size_t, std::size_t> SelectModeHandler::getSlice() const {
  return maybeSwap(m_Head, m_Tail);
}

std::size_t SelectModeHandler::getLength() const {
  auto p = maybeSwap(m_Head, m_Tail);
  return (p.second - p.first) + 1;
}

std::string SelectModeHandler::getText(const Document &doc) const {
  auto p = maybeSwap(m_Head, m_Tail);
  auto text =
    doc.getBuffer()->getStrBuf().substr(p.first, (p.second - p.first) + 1);
  JIG_DEBUG("text=\"%s\"", text.c_str());
  return text;
}

void SelectModeHandler::eraseText(Document &doc) {
  auto p = maybeSwap(m_Head, m_Tail);
  doc.eraseFront(p.first, (p.second - p.first) + 1);
}

void SelectModeHandler::moveLeft() {
  if (m_Tail > 0) {
    --m_Tail;
    JIG_DEBUG("m_Tail=%zu", m_Tail);
  }
}

void SelectModeHandler::moveLeft(int n) {
  for (; n != 0; --n)
    moveLeft();
}

void SelectModeHandler::moveRight(const Document &doc) {
  if (m_Tail < doc.getBuffer()->getLength()) {
    ++m_Tail;
    JIG_DEBUG("m_Tail=%zu", m_Tail);
  }
}

void SelectModeHandler::moveRight(const Document &doc, int n) {
  for (; n != 0; --n)
    moveRight(doc);
}

} // namespace jig
