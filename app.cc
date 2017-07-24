//===--- app.cc ---------------------------------------------------------===//
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

#include "app.h"

#include <clocale>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <getopt.h>

#include "logger.h"
#include "system.h"
#include "util.h"

namespace jig {
namespace {

void showUsage(bool err = false) {
  fprintf(!err ? stdout : stderr, "Usage: %s FILENAME...\n",
          App::getInstance().getExecName());
  if (!err)
    fputs("Options:\n"
          "  -h, --help     Print this text and exit.\n"
          "  -v, --version  Print version information and exit.\n",
          stdout);
}

void showVersion() {
  printf("%s %d.%d.%d\n"
         "Written by Nathan Forbes (2017)\n",
         App::PROGRAM_NAME, App::VERSION_MAJOR, App::VERSION_MINOR,
         App::VERSION_BUILD);
}

void cleanup() {
  App::getInstance().getUI().stop();
  Logger::terminate();
}

void cleanupOnSignal(int sig) {
  App::getInstance().getUI().stop();
  int exitStatus = EXIT_SUCCESS;
  switch (sig) {
    case SIGABRT:
      Logger::info("Caught SIGABRT signal");
      exitStatus = EXIT_FAILURE;
      break;
    case SIGINT:
      Logger::info("Caught SIGINT signal");
      exitStatus = 130;
      break;
    case SIGKILL:
      Logger::info("Caught SIGKILL signal");
      break;
    case SIGHUP:
      Logger::info("Caught SIGHUP signal");
      exitStatus = EXIT_FAILURE;
      break;
    case SIGQUIT:
      Logger::info("Caught SIGQUIT signal");
      break;
    case SIGTERM:
      Logger::info("Caught SIGTERM signal");
      break;
    case SIGSEGV:
      Logger::info("Caught SIGSEGV signal");
      exitStatus = EXIT_FAILURE;
      break;
  }
  Logger::terminate();
  std::exit(exitStatus);
}

void startup() {
  std::setlocale(LC_ALL, "");
  System::makeDirectory(System::getProgramDirectory());
  Logger::init();
  std::signal(SIGABRT, cleanupOnSignal);
  std::signal(SIGINT, cleanupOnSignal);
  std::signal(SIGKILL, cleanupOnSignal);
  std::signal(SIGHUP, cleanupOnSignal);
  std::signal(SIGQUIT, cleanupOnSignal);
  std::signal(SIGTERM, cleanupOnSignal);
  std::signal(SIGSEGV, cleanupOnSignal);
}

} // namespace

constexpr char App::PROGRAM_NAME[];
constexpr char App::DISPLAY_NAME[];

App &App::getInstance() {
  static App instance;
  return instance;
}

int App::run(int argc, char **argv) {
  const struct option options[] = {
    {"help", no_argument, nullptr, 'h'},
    {"version", no_argument, nullptr, 'v'},
    {nullptr, 0, nullptr, 0},
  };
  int c;

  m_ExecName = argv[0];
  setProgramName();

  startup();
  m_Timer.start();

  while ((c = getopt_long(argc, argv, "hv", options, nullptr)) != -1) {
    switch (c) {
      case 'h':
        showUsage();
        return EXIT_SUCCESS;
      case 'v':
        showVersion();
        return EXIT_SUCCESS;
      default:
        showUsage(true);
        return 127;
    }
  }

  if (argc <= optind)
    m_DocumentList.addNew(Document::createEmpty("<untitled>"));
  else
    for (int i = optind; i < argc; ++i)
      m_DocumentList.addNew(Document::createFromFile(argv[i]));

  m_UI.start();
  while (m_KeepRunning) {
    m_UI.draw();
    m_UI.handleInput();
  }

  cleanup();
  return EXIT_SUCCESS;
}

void App::setProgramName() {
  if (m_ExecName && *m_ExecName) {
    const char *p = strrchr(m_ExecName, '/');
    if (p && *p && *(p + 1))
      m_ProgramName = p + 1;
    else
      m_ProgramName = m_ExecName;
    return;
  }
  m_ProgramName = PROGRAM_NAME;
}

} // namespace jig
