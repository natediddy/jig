#include "fig.h"

#include <cstdlib>
#include <utility>

#include "logger.h"
#include "strutils.h"

namespace jig {
namespace {

constexpr char COMMENT_CHAR = '#';
constexpr char EQUAL_CHAR = '=';

constexpr char BUILTIN_FIG[] = "WrapLines=false\n"
                               "ShowLineNumbers=false\n"
                               "UseSpacesForTabs=false\n"
                               "TabWidth=4\n";

const std::unordered_map<std::string, Settings::ValueType> VALID_OPTIONS = {
  {"WrapLines", Settings::ValueType::BOOLEAN},
  {"ShowLineNumbers", Settings::ValueType::BOOLEAN},
  {"UseSpacesForTabs", Settings::ValueType::BOOLEAN},
  {"TabWidth", Settings::ValueType::NUMBER},
};

const Path BUILTIN_FIG_PATH = "";

void stripComments(std::string &line) {
  std::size_t p = line.find_first_of(COMMENT_CHAR);
  if (p != std::string::npos)
    line.erase(p);
}

void preprocessLine(std::string &line) {
  stripComments(line);
  if (line.empty())
    return;
  str::stripAllWhitespace(line);
}

std::pair<std::string, std::string> processLine(const std::string &line) {
  if (str::occurs(line, EQUAL_CHAR) != 1) {
    Logger::error("config syntax error in line: `%s'", line.c_str());
    Logger::error("--> Each line can only logically contain one '=' character");
    std::abort();
  }
  std::size_t p = line.find_first_of(EQUAL_CHAR);
  return std::make_pair(line.substr(0, p), line.substr(p + 1));
}

} // namespace

Fig::Fig() : m_File{nullptr}, m_Buffer{BUILTIN_FIG} {
  parseSettings();
}

Fig::Fig(const Path &path)
    : m_File{std::make_unique<File>(path)}, m_Buffer{m_File->readContents()} {
  parseSettings();
}

const Path &Fig::getPath() const {
  if (m_File)
    return m_File->getPath();
  return BUILTIN_FIG_PATH;
}

void Fig::parseSettings() {
  static const auto INVALID_OPTION = VALID_OPTIONS.end();

  for (auto B = m_Buffer.getFirstLineIterator(),
            E = m_Buffer.getLastLineIterator(), I = B;
       I != E; ++I) {
    std::string line{m_Buffer.getLineAt(I - B)};
    preprocessLine(line);
    if (line.empty())
      continue;
    auto kv = processLine(line);
    auto p = VALID_OPTIONS.find(kv.first);
    if (p == INVALID_OPTION) {
      Logger::warn("encountered invalid config option -- `%s'",
                   kv.first.c_str());
      continue;
    }
    if (p->second != Settings::getRealValueTypeForValueAsString(kv.second)) {
      Logger::warn("config option `%s' expects %s values", kv.first.c_str(),
                   Settings::getValueTypeString(p->second));
      continue;
    }
    switch (p->second) {
      case Settings::ValueType::BOOLEAN:
        m_Settings.set<bool>(kv.first,
                             Settings::convertStringToBoolean(kv.second));
        break;
      case Settings::ValueType::NUMBER:
        m_Settings.set<int>(kv.first,
                            Settings::convertStringToNumber(kv.second));
        break;
      case Settings::ValueType::STRING:
        m_Settings.set<const char *>(kv.first, kv.second.c_str());
        break;
      default: // not reached
        break;
    }
  }
}

} // namespace jig
