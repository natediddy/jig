//===--- path.h ---------------------------------------------------------===//
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

#ifndef __JIG_PATH_H__
#define __JIG_PATH_H__

#include <string>

#include <cstdio>

namespace jig {

// Path represents a Unix style path
class Path {
public:
  static constexpr char SEPARATOR = '/';

  static std::string getSanitized(const std::string &str);
  static void sanitize(std::string &str);

  Path() : m_Str{""} {}

  Path(const char *str) : m_Str{str} { sanitize(m_Str); }

  Path(const std::string &str) : m_Str{getSanitized(str)} {}

  Path(std::string &&str) : m_Str{std::move(str)} { sanitize(m_Str); }

  Path &operator=(const char *str) {
    m_Str = str;
    return *this;
  }

  Path &operator=(const std::string &str) {
    m_Str = str;
    return *this;
  }

  Path &operator=(std::string &&str) {
    m_Str = std::move(str);
    return *this;
  }

  const std::string &getString() const { return m_Str; }
  const char *getCString() const { return m_Str.c_str(); }

  std::size_t length() const { return m_Str.length(); }
  std::size_t size() const { return m_Str.size(); }

  bool isEmpty() const { return m_Str.empty(); }
  bool isAbsolute() const { return m_Str.front() == SEPARATOR; }
  bool isRelative() const { return m_Str.front() != SEPARATOR; }
  bool isRoot() const { return m_Str.size() == 1 && m_Str.back() == SEPARATOR; }

  std::string getBasename() const;
  std::string getParent() const;

  Path &append(const char *str);
  Path &append(const std::string &str);
  Path &append(const Path &path);

  Path &operator+=(const char *str) { return append(str); }
  Path &operator+=(const std::string &str) { return append(str); }
  Path &operator+=(const Path &path) { return append(path.m_Str); }

  Path operator+(const char *str);
  Path operator+(const std::string &str);
  Path operator+(const Path &path);

  Path &prepend(const char *str);
  Path &prepend(const std::string &str);
  Path &prepend(const Path &path);

private:
  std::string m_Str;
};

} // namespace jig

#endif // __JIG_PATH_H__
