//===--- window.h -------------------------------------------------------===//
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

#ifndef __JIG_WINDOW_H__
#define __JIG_WINDOW_H__

#include <string>

#include "color.h"

namespace jig {

class Window {
public:
  struct Attr {
    static const int NORMAL;
    static const int STANDOUT;
    static const int UNDERLINE;
    static const int REVERSE;
    static const int BLINK;
    static const int DIM;
    static const int BOLD;
    static const int PROTECT;
    static const int INVISIBLE;
    // XXX: Not all terminals support italics. So just ignore this for now.
    // static const int ITALIC;
  };

  Window() = default;
  Window(int h, int w, int y, int x);
  Window(int h, int w, int y, int x, const Color &backgroundColor);
  ~Window();

  int getHeight() const { return m_Height; }
  int getWidth() const { return m_Width; }
  int getStartY() const { return m_StartY; }
  int getStartX() const { return m_StartX; }

  void moveCursor(int y, int x);

  void enableKeypad();
  void disableKeypad();

  void enableAttrs(int attrs);
  void disableAttrs(int attrs);
  void disableAllAttrs();

  void setBackgroundColor(const Color &color);

  void put(int y, int x, char ch);
  void put(int y, int x, const char *str);
  void put(int y, int x, const char *str, std::size_t count);
  void put(int y, int x, const std::string &str);
  void put(int y, int x, const std::string &str, std::size_t count);

  void putf(int y, int x, const char *fmt, ...);

  int getKeypress();

  void clear();
  void move(int y, int x);
  void resize(int h, int w);
  void refresh();

private:
  void *m_WinPtr = nullptr;
  int m_Height = 0;
  int m_Width = 0;
  int m_StartY = 0;
  int m_StartX = 0;
  int m_Attrs = 0;
  Color m_BackgroundColor;
};

} // namespace jig

#endif // __JIG_WINDOW_H__
