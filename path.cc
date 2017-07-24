//===--- path.cc --------------------------------------------------------===//
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

#include "path.h"

namespace jig {

std::string Path::getSanitized(const std::string &str) {
  std::string result{str};
  sanitize(result);
  return result;
}

void Path::sanitize(std::string &str) {
  if (str.size() == 1 && str.back() == SEPARATOR)
    return;
  while (str.back() == SEPARATOR)
    str.pop_back();
  for (auto I = str.begin(), E = str.end(); I != E; ++I)
    if (*I == SEPARATOR)
      while (*(I + 1) == SEPARATOR)
        str.erase(I + 1);
}

std::string Path::getBasename() const {
  if (m_Str.size() == 1 && (m_Str.back() == SEPARATOR || m_Str.back() == '.'))
    return m_Str;
  return m_Str.substr(m_Str.find_last_of(SEPARATOR) + 1);
}

std::string Path::getParent() const {
  if (m_Str == ".." ||
      (m_Str.size() == 1 && (m_Str.back() == SEPARATOR || m_Str.back() == '.')))
    return m_Str;
  return m_Str.substr(0, m_Str.find_last_of(SEPARATOR));
}

Path &Path::append(const char *str) {
  m_Str.append(1, SEPARATOR);
  m_Str.append(str);
  sanitize(m_Str);
  return *this;
}

Path &Path::append(const std::string &str) {
  m_Str.append(1, SEPARATOR);
  m_Str.append(str);
  sanitize(m_Str);
  return *this;
}

Path &Path::append(const Path &path) {
  m_Str.append(1, SEPARATOR);
  m_Str.append(path.m_Str);
  sanitize(m_Str);
  return *this;
}

Path Path::operator+(const char *str) {
  Path result{m_Str};
  return result.append(str);
}

Path Path::operator+(const std::string &str) {
  Path result{m_Str};
  return result.append(str);
}

Path Path::operator+(const Path &path) {
  Path result{m_Str};
  return result.append(path.m_Str);
}

Path &Path::prepend(const char *str) {
  std::string t{str};
  t.append(1, SEPARATOR);
  m_Str.insert(0, t);
  sanitize(m_Str);
  return *this;
}

Path &Path::prepend(const std::string &str) {
  std::string t{str};
  t.append(1, SEPARATOR);
  m_Str.insert(0, t);
  sanitize(m_Str);
  return *this;
}

Path &Path::prepend(const Path &path) {
  std::string t{path.m_Str};
  t.append(1, SEPARATOR);
  m_Str.insert(0, t);
  sanitize(m_Str);
  return *this;
}

} // namespace jig
