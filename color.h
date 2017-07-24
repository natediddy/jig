//===--- color.h --------------------------------------------------------===//
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

#ifndef __JIG_COLOR_H__
#define __JIG_COLOR_H__

namespace jig {

class Color {
public:
  // Basic colors
  static const short DEFAULT;
  static const short BLACK;
  static const short RED;
  static const short GREEN;
  static const short YELLOW;
  static const short BLUE;
  static const short MAGENTA;
  static const short CYAN;
  static const short WHITE;

  static void init();

  explicit Color(short foreground, short background);

  Color(const Color &) = default;
  Color &operator=(const Color &) = default;

  int getPairAttribute() const;

  operator int() const { return getPairAttribute(); }

private:
  static short pairNumber;

  short m_Foreground;
  short m_Background;
  short m_PairNumber;
};

} // namespace jig

#endif // __JIG_COLOR_H__
