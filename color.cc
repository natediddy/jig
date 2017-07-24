//===--- color.cc -------------------------------------------------------===//
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

#include "color.h"

#include <ncurses.h>

namespace jig {

short Color::pairNumber = 0;

const short Color::DEFAULT = -1;
const short Color::BLACK = COLOR_BLACK;
const short Color::RED = COLOR_RED;
const short Color::GREEN = COLOR_GREEN;
const short Color::YELLOW = COLOR_YELLOW;
const short Color::BLUE = COLOR_BLUE;
const short Color::MAGENTA = COLOR_MAGENTA;
const short Color::CYAN = COLOR_CYAN;
const short Color::WHITE = COLOR_WHITE;

void Color::init() {
  if (!has_colors())
    return;
  start_color();
  use_default_colors();
}

Color::Color(short foreground, short background)
  : m_Foreground{foreground},
    m_Background{background},
    m_PairNumber{++pairNumber} {
  init_pair(m_PairNumber, m_Foreground, m_Background);
}

int Color::getPairAttribute() const {
  return COLOR_PAIR(m_PairNumber);
}

} // namespace jig
