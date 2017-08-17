//===--- settings.cc ----------------------------------------------------===//
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

#include "settings.h"

#include <cctype>
#include <cstdlib>

#include "logger.h"
#include "strutils.h"

namespace jig {
namespace {

bool isBooleanString(const std::string &value) {
  return str::areEqualIgnoreCase(value, "true") ||
         str::areEqualIgnoreCase(value, "false");
}

bool isNumberString(const std::string &value) {
  for (const auto &c : value)
    if (!std::isdigit(c))
      return false;
  return true;
}

} // namespace

Settings::Value &Settings::Value::operator=(bool value) {
  m_Type = ValueType::BOOLEAN;
  m_Value.b = value;
  return *this;
}

Settings::Value &Settings::Value::operator=(int value) {
  m_Type = ValueType::NUMBER;
  m_Value.n = value;
  return *this;
}

Settings::Value &Settings::Value::operator=(const char *value) {
  m_Type = ValueType::STRING;
  m_Value.s = value;
  return *this;
}

Settings::ValueType
Settings::getRealValueTypeForValueAsString(const std::string &value) {
  if (isBooleanString(value))
    return ValueType::BOOLEAN;
  if (isNumberString(value))
    return ValueType::NUMBER;
  return ValueType::STRING;
}

const char *const Settings::getValueTypeString(ValueType type) {
  switch (type) {
    case Settings::ValueType::BOOLEAN:
      return "boolean";
    case Settings::ValueType::NUMBER:
      return "number";
    case Settings::ValueType::STRING:
      return "string";
    default:
      return "???";
  }
}

bool Settings::convertStringToBoolean(const std::string &str) {
  return str::areEqualIgnoreCase(str, "true");
}

int Settings::convertStringToNumber(const std::string &str) {
  return static_cast<int>(std::strtol(str.c_str(), nullptr, 10));
}

bool Settings::get(const std::string &key, bool &value) const {
  auto it = m_Options.find(key);
  if (it == m_Options.end())
    return false;
  if (it->second.getType() != ValueType::BOOLEAN) {
    Logger::error("incorrect value type for setting");
    Logger::error("--> Option \"%s\" expects %s type values", key.c_str(),
                  getValueTypeString(it->second.getType()));
    return false;
  }
  value = it->second.getBoolean();
  return true;
}

bool Settings::get(const std::string &key, int &value) const {
  auto it = m_Options.find(key);
  if (it == m_Options.end())
    return false;
  if (it->second.getType() != ValueType::NUMBER) {
    Logger::error("incorrect value type for setting");
    Logger::error("--> Option \"%s\" expects %s type values", key.c_str(),
                  getValueTypeString(it->second.getType()));
    return false;
  }
  value = it->second.getNumber();
  return true;
}

bool Settings::get(const std::string &key, std::string &value) const {
  auto it = m_Options.find(key);
  if (it == m_Options.end())
    return false;
  if (it->second.getType() != ValueType::STRING) {
    Logger::error("incorrect value type for setting");
    Logger::error("--> Option \"%s\" expects %s type values", key.c_str(),
                  getValueTypeString(it->second.getType()));
    return false;
  }
  value = it->second.getString();
  return true;
}

} // namespace jig
