//===--- buffer.h -------------------------------------------------------===//
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

#ifndef __JIG_BUFFER_H__
#define __JIG_BUFFER_H__

#include <vector>

#include "line.h"

namespace jig {

class Buffer {
public:
  using LineIterator = std::vector<Line>::iterator;
  using ConstLineIterator = std::vector<Line>::const_iterator;

  Buffer(std::string str) : m_StrBuf{std::move(str)} { initLineBuf(); }

  const std::string &getStrBuf() const { return m_StrBuf; }
  const std::vector<Line> &getLineBuf() const { return m_LineBuf; }

  std::size_t length() const { return m_StrBuf.length(); }
  std::size_t size() const { return m_StrBuf.size(); }

  std::size_t getTotalLines() const { return m_LineBuf.size(); }

  std::string getStringAt(std::size_t pos, std::size_t count) const;
  std::string getLineAt(std::size_t lineIndex) const;
  std::size_t getLineLengthAtPos(std::size_t pos) const;
  std::size_t getLineIndexAtPos(std::size_t pos) const;
  const Line &getLineAtPos(std::size_t pos) const;

  void insert(std::size_t pos, char ch);
  void insert(std::size_t pos, const char *str);
  void insert(std::size_t pos, const char *str, std::size_t len);
  void insert(std::size_t pos, const std::string &str);

  void erase(std::size_t pos, std::size_t count = 1);

  void replace(std::size_t pos, std::size_t count, char ch);
  void replace(std::size_t pos, std::size_t count, const char *str);
  void replace(std::size_t pos, std::size_t count, const char *str,
               std::size_t len);
  void replace(std::size_t pos, std::size_t count, const std::string &str);

  ConstLineIterator getFirstLineIterator() const { return m_LineBuf.cbegin(); }
  ConstLineIterator getLastLineIterator() const { return m_LineBuf.cend() - 1; }

  LineIterator getLineIterator(std::size_t pos);
  ConstLineIterator getConstLineIterator(std::size_t pos) const;

private:
  void initLineBuf();
  void updateLineBuf();

  std::string m_StrBuf;
  std::vector<Line> m_LineBuf;
};

} // namespace jig

#endif // __JIG_BUFFER_H__
