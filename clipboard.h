//===--- clipboard.h ----------------------------------------------------===//
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

#ifndef __JIG_CLIPBOARD_H__
#define __JIG_CLIPBOARD_H__

#include <string>

namespace jig {

class Clipboard {
public:
  Clipboard() = default;

  const std::string &getContent() const { return m_Content; }

  void setContent(const std::string &content) { m_Content = content; }
  void setContent(std::string &&content) { m_Content = std::move(content); }

  bool isEmpty() const { return m_Content.empty(); }

private:
  std::string m_Content;
};

} // namespace jig

#endif // __JIG_CLIPBOARD_H__
