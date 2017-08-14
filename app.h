//===--- app.h ----------------------------------------------------------===//
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

#ifndef __JIG_APP_H__
#define __JIG_APP_H__

#include "clipboard.h"
#include "documentlist.h"
#include "figmanager.h"
#include "selectmodehandler.h"
#include "timeutils.h"
#include "ui.h"

namespace jig {

class App {
public:
  enum class Mode {
    NORMAL,
    SELECT,
  };

  static constexpr char PROGRAM_NAME[] = "jig";
  static constexpr char DISPLAY_NAME[] = "Jig";

  static constexpr int VERSION_MAJOR = 1;
  static constexpr int VERSION_MINOR = 0;
  static constexpr int VERSION_BUILD = 0;

  static App &getInstance();

  DocumentList &getDocumentList() { return m_DocumentList; }
  UI &getUI() { return m_UI; }
  Clipboard &getClipboard() { return m_Clipboard; }
  SelectModeHandler &getSelectModeHandler() { return m_SelectModeHandler; }

  Mode getCurrentMode() const { return m_CurrentMode; }
  void setCurrentMode(Mode mode) { m_CurrentMode = mode; }

  FigManager &getFigManager() { return m_FigManager; }

  const char *getExecName() const { return m_ExecName; }
  const char *getProgramName() const { return m_ProgramName; }

  void setKeepRunning(bool keepRunning) { m_KeepRunning = keepRunning; }

  int run(int argc, char **argv);

private:
  App() = default;

  void setProgramName();

  DocumentList m_DocumentList;
  UI m_UI;
  time::Timer m_Timer;
  Clipboard m_Clipboard;
  SelectModeHandler m_SelectModeHandler;
  Mode m_CurrentMode;
  FigManager m_FigManager;
  const char *m_ExecName;
  const char *m_ProgramName;
  bool m_KeepRunning = true;
};

} // namespace jig

#endif // __JIG_APP_H__
