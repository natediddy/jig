//===--- ui.cc ----------------------------------------------------------===//
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

#include "ui.h"

#include <csignal>
#include <cstdio>
#include <cstdlib>

#include <ncurses.h>

#include "app.h"
#include "color.h"
#include "logger.h"

namespace jig {
namespace {

constexpr int KEY_BACKSPACE_CUSTOM = 127;
constexpr int KEY_NEWLINE = '\n';
constexpr int KEY_TAB = '\t';

// constexpr int KEY_CTRL_A = 1;
// constexpr int KEY_CTRL_B = 2;
constexpr int KEY_CTRL_C = 3;
// constexpr int KEY_CTRL_D = 4;
// constexpr int KEY_CTRL_E = 5;
// constexpr int KEY_CTRL_F = 6;
// constexpr int KEY_CTRL_G = 7;
// constexpr int KEY_CTRL_H = 8;
// constexpr int KEY_CTRL_I = 9;
// constexpr int KEY_CTRL_J = 10;
// constexpr int KEY_CTRL_K = 11;
// constexpr int KEY_CTRL_L = 12;
// constexpr int KEY_CTRL_M = 13;
// constexpr int KEY_CTRL_N = 14;
// constexpr int KEY_CTRL_O = 15;
constexpr int KEY_CTRL_P = 16;
constexpr int KEY_CTRL_Q = 17;
constexpr int KEY_CTRL_R = 18;
constexpr int KEY_CTRL_S = 19;
// constexpr int KEY_CTRL_T = 20;
constexpr int KEY_CTRL_U = 21;
constexpr int KEY_CTRL_V = 22;
// constexpr int KEY_CTRL_W = 23;
constexpr int KEY_CTRL_X = 24;
// constexpr int KEY_CTRL_Y = 25;
// constexpr int KEY_CTRL_Z = 26;

constexpr int KEY_ALT_LEFT = KEY_MAX + 1;
constexpr int KEY_ALT_RIGHT = KEY_MAX + 2;
constexpr int KEY_ALT_UP = KEY_MAX + 3;
constexpr int KEY_ALT_DOWN = KEY_MAX + 4;
constexpr int KEY_SHIFT_ALT_LEFT = KEY_MAX + 5;
constexpr int KEY_SHIFT_ALT_RIGHT = KEY_MAX + 6;
constexpr int KEY_SHIFT_ALT_UP = KEY_MAX + 7;
constexpr int KEY_SHIFT_ALT_DOWN = KEY_MAX + 8;

void resizeHandler(int sig) {
  endwin();
  refresh();
  clear();

  UI &ui = App::getInstance().getUI();
  ui.updateDimensions();
  ui.draw();
}

} // namespace

const int UI::INVALID_INPUT = ERR;

void UI::start() {
  if (m_Running)
    return;

  if (!initscr()) {
    Logger::fatal("failed to initialize Ncurses");
    std::exit(EXIT_FAILURE);
  }

  noecho();
  cbreak();
  raw();
  set_escdelay(25);
  keypad(stdscr, true);

  define_key("\033\033\[D", KEY_ALT_LEFT);
  define_key("\033\033\[C", KEY_ALT_RIGHT);
  define_key("\033\033\[A", KEY_ALT_UP);
  define_key("\033\033\[B", KEY_ALT_DOWN);
  define_key("\033\[1;10D", KEY_SHIFT_ALT_LEFT);
  define_key("\033\[1;10C", KEY_SHIFT_ALT_RIGHT);
  define_key("\033\[1;10A", KEY_SHIFT_ALT_UP);
  define_key("\033\[1;10B", KEY_SHIFT_ALT_DOWN);

  Color::init();

  getmaxyx(stdscr, m_Height, m_Width);
  std::signal(SIGWINCH, resizeHandler);

  if (App::getInstance().getFig()->get<bool>("ShowLineNumbers"))
    m_LineNumberColumn = std::make_unique<LineNumberColumn>();

  m_TitleBar.init();
  m_StatusBar.init();
  if (m_LineNumberColumn)
    m_LineNumberColumn->init();
  m_BufferView.init();

  m_Running = true;
}

void UI::stop() {
  if (!m_Running)
    return;
  endwin();
}

void UI::updateDimensions() {
  if (!m_Running)
    return;

  getmaxyx(stdscr, m_Height, m_Width);

  m_TitleBar.updateDimensions();
  m_StatusBar.updateDimensions();

  if (m_LineNumberColumn)
    m_LineNumberColumn->updateDimensions();

  m_BufferView.updateDimensions();
}

void UI::draw() {
  ::refresh();

  m_TitleBar.draw();
  m_StatusBar.draw();

  if (m_LineNumberColumn)
    m_LineNumberColumn->draw();

  m_BufferView.draw();
}

void UI::handleInput() {
  int k = m_BufferView.getKeypress();
  auto &app = App::getInstance();
  auto &docList = app.getDocumentList();

  switch (k) {
    case KEY_LEFT:
      docList.getCurrent().moveCursorLeft();
      update(false, true, true);
      break;
    case KEY_RIGHT:
      docList.getCurrent().moveCursorRight();
      update(false, true, true);
      break;
    case KEY_UP:
      docList.getCurrent().moveCursorUp();
      update(false, true, true);
      break;
    case KEY_DOWN:
      docList.getCurrent().moveCursorDown();
      update(false, true, true);
      break;
    case KEY_BTAB:
    case KEY_SHIFT_ALT_LEFT:
      docList.setPreviousAsCurrent();
      update(true, true, true);
      break;
    case KEY_SHIFT_ALT_RIGHT:
      docList.setNextAsCurrent();
      update(true, true, true);
      break;
    case KEY_CTRL_C:
      if (app.getCurrentMode() == App::Mode::SELECT) {
        app.getClipboard().setContent(
          app.getSelectModeHandler().getText(docList.getCurrent()));
        app.setCurrentMode(App::Mode::NORMAL);
        app.getSelectModeHandler().reset();
        update(false, true, true);
      }
      break;
    case KEY_CTRL_P: {
      auto &clipboard = app.getClipboard();
      if (!clipboard.isEmpty()) {
        docList.getCurrent().insert(clipboard.getContent());
        update(true, true, true);
      }
      break;
    }
    case KEY_CTRL_Q:
      app.setKeepRunning(false);
      break;
    case KEY_CTRL_R:
      docList.getCurrent().redo();
      update(true, true, true);
      break;
    case KEY_CTRL_S:
      docList.getCurrent().save();
      update(true, false, false);
      break;
    case KEY_CTRL_U:
      docList.getCurrent().undo();
      update(true, true, true);
      break;
    case KEY_CTRL_V:
      switch (app.getCurrentMode()) {
        case App::Mode::NORMAL:
          app.setCurrentMode(App::Mode::SELECT);
          JIG_DEBUG("Starting SELECT mode...");
          app.getSelectModeHandler().init(docList.getCurrent());
          break;
        case App::Mode::SELECT:
          app.setCurrentMode(App::Mode::NORMAL);
          app.getSelectModeHandler().reset();
          break;
      }
      update(false, true, true);
      break;
    case KEY_CTRL_X:
      if (app.getCurrentMode() == App::Mode::SELECT) {
        auto &doc = docList.getCurrent();
        auto &smh = app.getSelectModeHandler();
        app.getClipboard().setContent(smh.getText(doc));
        smh.eraseText(doc);
        app.setCurrentMode(App::Mode::NORMAL);
        smh.reset();
        update(true, true, true);
      }
      break;
    case KEY_NEWLINE:
      docList.getCurrent().insert('\n');
      update(true, true, true);
      break;
    case KEY_BACKSPACE_CUSTOM:
      docList.getCurrent().eraseBack(1);
      update(true, true, true);
      break;
    case KEY_DC:
      docList.getCurrent().eraseFront(1);
      update(true, true, true);
      break;
    case KEY_TAB:
      docList.getCurrent().insert('\t');
      update(true, true, true);
      break;
    default:
      if (std::isprint(k)) {
        docList.getCurrent().insert(k);
        update(true, true, true);
      }
      break;
  }
}

void UI::update(bool updateTitleBar, bool updateStatusBar,
                bool updateBufferView) {
  if (updateTitleBar)
    m_TitleBar.update();

  if (updateStatusBar)
    m_StatusBar.update();

  if (updateBufferView) {
    if (m_LineNumberColumn)
      m_LineNumberColumn->update();
    m_BufferView.update();
  }
}

} // namespace jig
