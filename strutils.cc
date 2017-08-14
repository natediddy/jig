//===--- strutils.cc ----------------------------------------------------===//
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

#include "strutils.h"

#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace jig {
namespace str {
namespace {

std::string convertAllToLowercase(const std::string &str) {
  std::string r;
  r.reserve(str.size());
  std::for_each(str.begin(), str.end(),
                [&r](const char &c) { r += std::tolower(c); });
  return r;
}

} // namespace

std::size_t occurs(const std::string &str, char ch) {
  std::size_t count = 0;
  std::for_each(str.begin(), str.end(), [ch, &count](const char &c) {
    if (c == ch)
      ++count;
  });
  return count;
}

std::size_t occurs(const std::string &str, const std::string &s) {
  std::size_t p = str.find(s);
  if (p == std::string::npos)
    return 0;

  std::size_t n = s.size();
  std::size_t count = 0;

  do {
    ++count;
    p = str.find(s, p + n);
  } while (p != std::string::npos);
  return count;
}

std::vector<std::size_t> findAllPositions(const std::string &str, char ch) {
  std::vector<std::size_t> r;
  std::size_t len = str.size();
  for (std::size_t p = 0; p != len; ++p)
    if (str[p] == ch)
      r.emplace_back(p);
  return r;
}

std::vector<std::size_t> findAllPositions(const std::string &str,
                                          const std::string &s) {
  std::vector<std::size_t> r;

  std::size_t p = str.find(s);
  if (p == std::string::npos)
    return r;

  std::size_t n = s.size();

  do {
    r.emplace_back(p);
    p = str.find(s, p + n);
  } while (p != std::string::npos);
  return r;
}

void stripAllWhitespace(std::string &str) {
  // Do not store str.end() here. It might change before the loop is done.
  for (auto I = str.begin(); I != str.end(); ++I)
    while (std::isspace(*I))
      str.erase(I);
}

bool areEqualIgnoreCase(const std::string &str1, const std::string &str2) {
  return convertAllToLowercase(str1) == convertAllToLowercase(str2);
}

std::string format(const char *fmt, ...) {
  va_list args;
  std::size_t n;

  va_start(args, fmt);
  n = vsnprintf(nullptr, 0, fmt, args);
  va_end(args);

  char arr[n + 1];
  va_start(args, fmt);
  vsnprintf(arr, n + 1, fmt, args);
  va_end(args);
  return arr;
}

} // namespace str
} // namespace jig
