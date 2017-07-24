//===--- titlebar.h -----------------------------------------------------===//
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

#ifndef __JIG_TITLEBAR_H__
#define __JIG_TITLEBAR_H__

#include <vector>

#include "view.h"

namespace jig {

class Document;

class TitleBar : public View {
public:
  TitleBar() = default;

  virtual void init() final;
  virtual void updateDimensions() final;

  virtual int getKeypress() override;

  void update();

  void addTitle(const Document &doc);
  void removeTitle(std::size_t index);

private:
  void initWindow();
  void initRepr();
  void writeToWindow();

  struct Title {
    Title(const std::string &s, bool d) : str{s}, dirty{d} {}
    std::string str;
    bool dirty;
  };

  std::vector<Title> m_Titles;
  std::vector<std::vector<const Title *>> m_Repr;
};

} // namespace jig

#endif // __JIG_TITLEBAR_H__
