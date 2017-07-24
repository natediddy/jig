//===--- window.cc ------------------------------------------------------===//
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

#include "window.h"

#include <cstdlib>

#include <ncurses.h>

#include "logger.h"

namespace jig {

const int Window::Attr::NORMAL = A_NORMAL;
const int Window::Attr::STANDOUT = A_STANDOUT;
const int Window::Attr::UNDERLINE = A_UNDERLINE;
const int Window::Attr::REVERSE = A_REVERSE;
const int Window::Attr::BLINK = A_BLINK;
const int Window::Attr::DIM = A_DIM;
const int Window::Attr::BOLD = A_BOLD;
const int Window::Attr::PROTECT = A_PROTECT;
const int Window::Attr::INVISIBLE = A_INVIS;
// const int Window::Attr::ITALIC = A_ITALIC;

Window::Window(int h, int w, int y, int x)
  : m_Height{h},
    m_Width{w},
    m_StartY{y},
    m_StartX{x},
    m_BackgroundColor{Color::DEFAULT, Color::DEFAULT} {
  WINDOW **p = ((WINDOW **)&m_WinPtr);
  *p = newwin(m_Height, m_Width, m_StartY, m_StartX);
  if (!*p) {
    Logger::fatal("failed to create new Ncurses window");
    std::exit(EXIT_FAILURE);
  }
  wbkgd(*p, m_BackgroundColor.getPairAttribute());
}

Window::Window(int h, int w, int y, int x, const Color &backgroundColor)
  : m_Height{h},
    m_Width{w},
    m_StartY{y},
    m_StartX{x},
    m_BackgroundColor{backgroundColor} {
  WINDOW **p = ((WINDOW **)&m_WinPtr);
  *p = newwin(m_Height, m_Width, m_StartY, m_StartX);
  if (!*p) {
    Logger::fatal("failed to create new Ncurses window");
    std::exit(EXIT_FAILURE);
  }
  wbkgd(*p, m_BackgroundColor.getPairAttribute());
}

Window::~Window() {
  delwin((WINDOW *)m_WinPtr);
}

void Window::moveCursor(int y, int x) {
  wmove((WINDOW *)m_WinPtr, y, x);
}

void Window::enableKeypad() {
  keypad((WINDOW *)m_WinPtr, true);
}

void Window::disableKeypad() {
  keypad((WINDOW *)m_WinPtr, false);
}

void Window::enableAttrs(int attrs) {
  wattron((WINDOW *)m_WinPtr, attrs);
  m_Attrs |= attrs;
}

void Window::disableAttrs(int attrs) {
  wattroff((WINDOW *)m_WinPtr, attrs);
  m_Attrs &= ~(attrs);
}

void Window::disableAllAttrs() {
  wattroff((WINDOW *)m_WinPtr, m_Attrs);
  m_Attrs = 0;
}

void Window::setBackgroundColor(const Color &color) {
  m_BackgroundColor = color;
  wbkgd((WINDOW *)m_WinPtr, m_BackgroundColor.getPairAttribute());
}

void Window::put(int y, int x, char ch) {
  mvwaddch((WINDOW *)m_WinPtr, y, x, ch);
}

void Window::put(int y, int x, const char *str) {
  mvwaddstr((WINDOW *)m_WinPtr, y, x, str);
}

void Window::put(int y, int x, const char *str, std::size_t count) {
  mvwaddnstr((WINDOW *)m_WinPtr, y, x, str, count);
}

void Window::put(int y, int x, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  wmove((WINDOW *)m_WinPtr, y, x);
  vw_printw((WINDOW *)m_WinPtr, fmt, args);
  va_end(args);
}

void Window::put(int y, int x, const std::string &str) {
  mvwaddnstr((WINDOW *)m_WinPtr, y, x, str.c_str(), str.size());
}

void Window::put(int y, int x, const std::string &str, std::size_t count) {
  mvwaddnstr((WINDOW *)m_WinPtr, y, x, str.c_str(), count);
}

int Window::getKeypress() {
  return wgetch((WINDOW *)m_WinPtr);
}

void Window::clear() {
  wclear((WINDOW *)m_WinPtr);
}

void Window::move(int y, int x) {
  if (mvwin((WINDOW *)m_WinPtr, y, x) != ERR) {
    m_StartY = y;
    m_StartX = x;
  }
}

void Window::resize(int h, int w) {
  WINDOW **p = ((WINDOW **)&m_WinPtr);
  delwin(*p);
  *p = newwin(h, w, m_StartY, m_StartX);
  if (!*p) {
    Logger::fatal("failed to resize Ncurses window");
    std::exit(EXIT_FAILURE);
  }
  wbkgd(*p, m_BackgroundColor.getPairAttribute());
  if (m_Attrs != 0)
    wattron(*p, m_Attrs);
  m_Height = h;
  m_Width = w;
  wrefresh(*p);
}

void Window::refresh() {
  wrefresh((WINDOW *)m_WinPtr);
}

} // namespace jig
