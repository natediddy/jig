//===--- buffer.cc ------------------------------------------------------===//
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

#include "buffer.h"

#include <assert.h>
#include <cstdio>

#include "strutils.h"

namespace jig {

std::string Buffer::getStringAt(std::size_t pos, std::size_t count) const {
  return m_StrBuf.substr(pos, count);
}

std::string Buffer::getLineAt(std::size_t lineIndex) const {
  assert(lineIndex < m_LineBuf.size() &&
         "Cannot access line - Index out of bounds");
  const Line &line{m_LineBuf[lineIndex]};
  return m_StrBuf.substr(line.begin() - m_StrBuf.begin(), line.size());
}

std::size_t Buffer::getLineLengthAtPos(std::size_t pos) const {
  assert(pos < m_StrBuf.size() && "Position is out of bounds");
  auto SB = m_StrBuf.begin();
  for (auto I = m_LineBuf.begin(), E = m_LineBuf.end(); I != E; ++I)
    if (pos >= I->begin() - SB && pos <= I->end() - SB)
      return I->length();
  return 0; // This should never be reached.
}

std::size_t Buffer::getLineIndexAtPos(std::size_t pos) const {
  assert(pos < m_StrBuf.size() && "Position is out of bounds");
  auto SB = m_StrBuf.begin();
  auto LB = m_LineBuf.begin();
  for (auto I = LB, E = m_LineBuf.end(); I != E; ++I)
    if (pos >= I->begin() - SB && pos <= I->end() - SB)
      return I - LB;
  return 0; // This should never be reached.
}

const Line &Buffer::getLineAtPos(std::size_t pos) const {
  assert(pos < m_StrBuf.size() && "Position is out of bounds");
  auto SB = m_StrBuf.begin();
  for (auto I = m_LineBuf.begin(), E = m_LineBuf.end(); I != E; ++I)
    if (pos >= I->begin() - SB && pos <= I->end() - SB)
      return *I;
  return m_LineBuf.back(); // This should never be reached.
}

void Buffer::insert(std::size_t pos, char ch) {
  m_StrBuf.insert(pos, 1, ch);
  updateLineBuf();
}

void Buffer::insert(std::size_t pos, const char *str) {
  m_StrBuf.insert(pos, str);
  updateLineBuf();
}

void Buffer::insert(std::size_t pos, const char *str, std::size_t len) {
  m_StrBuf.insert(pos, str, len);
  updateLineBuf();
}

void Buffer::insert(std::size_t pos, const std::string &str) {
  m_StrBuf.insert(pos, str);
  updateLineBuf();
}

void Buffer::erase(std::size_t pos, std::size_t count /*=1*/) {
  std::size_t n = m_StrBuf.size() - pos;
  if (n == 1)
    return;
  if (count >= n)
    count = n - 1;
  m_StrBuf.erase(pos, count);
  updateLineBuf();
}

void Buffer::replace(std::size_t pos, std::size_t count, char ch) {
  m_StrBuf.erase(pos, count);
  m_StrBuf.insert(pos, 1, ch);
  updateLineBuf();
}

void Buffer::replace(std::size_t pos, std::size_t count, const char *str) {
  m_StrBuf.erase(pos, count);
  m_StrBuf.insert(pos, str);
  updateLineBuf();
}

void Buffer::replace(std::size_t pos, std::size_t count, const char *str,
                     std::size_t len) {
  m_StrBuf.erase(pos, count);
  m_StrBuf.insert(pos, str, len);
  updateLineBuf();
}

void Buffer::replace(std::size_t pos, std::size_t count,
                     const std::string &str) {
  m_StrBuf.erase(pos, count);
  m_StrBuf.insert(pos, str);
  updateLineBuf();
}

Buffer::LineIterator Buffer::getLineIterator(std::size_t pos) {
  assert(pos < m_LineBuf.size() && "Line position is out of bounds");
  return m_LineBuf.begin() + pos;
}

Buffer::ConstLineIterator Buffer::getConstLineIterator(std::size_t pos) const {
  assert(pos < m_LineBuf.size() && "Line position is out of bounds");
  return m_LineBuf.cbegin() + pos;
}

void Buffer::initLineBuf() {
  if (m_StrBuf.empty() || m_StrBuf.back() != '\n')
    m_StrBuf.append(1, '\n');
  m_LineBuf.reserve(str::occurs(m_StrBuf, '\n'));
  updateLineBuf();
}

void Buffer::updateLineBuf() {
  auto B = m_StrBuf.begin();
  auto E = m_StrBuf.end();
  m_LineBuf.clear();
  for (auto I = B; I != E; ++I) {
    if (*I == '\n') {
      if (m_LineBuf.empty()) {
        m_LineBuf.emplace_back(Line{B, I});
        continue;
      }
      if (I == B || (*(I - 1) == '\n')) {
        m_LineBuf.emplace_back(Line{I, I});
        continue;
      }
      for (auto P = I - 1; P != B; --P) {
        if (*P == '\n' || P == B) {
          m_LineBuf.emplace_back(Line{P == B ? P : P + 1, I});
          break;
        }
      }
    }
  }
}

} // namespace jig
