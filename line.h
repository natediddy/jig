//===--- line.h ---------------------------------------------------------===//
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

#ifndef __JIG_LINE_H__
#define __JIG_LINE_H__

#include <string>

namespace jig {

class Line {
public:
  Line(std::string::iterator b, std::string::iterator e)
    : m_Begin{b}, m_End{e} {}

  std::size_t length() const { return m_End - m_Begin; }
  std::size_t size() const { return m_End - m_Begin; }

  std::string toString() const {
    std::string ret;
    std::for_each(m_Begin, m_End + 1, [&ret](const char &ch) { ret += ch; });
    return ret;
  }

  auto &begin() { return m_Begin; }
  const auto &begin() const { return m_Begin; }
  auto &end() { return m_End; }
  const auto &end() const { return m_End; }

private:
  std::string::iterator m_Begin;
  std::string::iterator m_End;
};

} // namespace jig

#endif // __JIG_LINE_H__
