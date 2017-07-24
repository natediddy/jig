//===--- statusbar.h ----------------------------------------------------===//
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

#ifndef __JIG_STATUSBAR_H__
#define __JIG_STATUSBAR_H__

#include <cstdint>
#include <string>

#include "view.h"

namespace jig {

class StatusBar : public View {
public:
  StatusBar() = default;

  virtual void init() final;
  virtual void updateDimensions() final;
  virtual int getKeypress() override;

  void update();

private:
  void initWindow();
  void writeToWindow();

  std::string m_Text;
  unsigned int m_LineNumber;
  unsigned int m_ColumnNumber;
  unsigned int m_Portion;
};

} // namespace jig

#endif // __JIG_STATUSBAR_H__
