//===--- timeutils.cc ---------------------------------------------------===//
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

#include "timeutils.h"

#include <cstdio>
#include <cstring>
#include <ctime>

namespace jig {
namespace time {
namespace {

constexpr std::size_t TEMP_BUFFER_SIZE = 1024;

class ElapsedTimeString {
public:
  ElapsedTimeString(Timer::FormatStringStyle style, long elapsedSecs)
    : m_Days{elapsedSecs / Timer::SECS_PER_DAY},
      m_Hours{(elapsedSecs % Timer::SECS_PER_DAY) / Timer::SECS_PER_HOUR},
      m_Mins{(elapsedSecs % Timer::SECS_PER_HOUR) / Timer::SECS_PER_MINUTE},
      m_Secs{(elapsedSecs % Timer::SECS_PER_HOUR) % Timer::SECS_PER_MINUTE} {
    switch (style) {
      case Timer::FormatStringStyle::SHORT:
        formatShort();
        break;
      case Timer::FormatStringStyle::LONG:
        formatLong();
        break;
      default:
        formatDefault();
        break;
    }
  }

  std::string getString() const { return m_Buffer; }

private:
  static constexpr char AND_STRING[] = " and ";
  static constexpr std::size_t AND_STRING_LEN = 5;

  static constexpr char COMMA_STRING[] = ", ";
  static constexpr std::size_t COMMA_STRING_LEN = 2;

  void formatShort() {
    std::size_t n = 0;

    if (m_Days) {
      snprintf(m_Buffer, TEMP_BUFFER_SIZE, "%ldd", m_Days);
      n = std::strlen(m_Buffer);
    }

    if (m_Hours) {
      snprintf(m_Buffer + n, TEMP_BUFFER_SIZE - n, "%ldh", m_Hours);
      n = std::strlen(m_Buffer);
    }

    if (m_Mins) {
      snprintf(m_Buffer + n, TEMP_BUFFER_SIZE - n, "%ldm", m_Mins);
      n = std::strlen(m_Buffer);
    }

    if (m_Secs)
      snprintf(m_Buffer + n, TEMP_BUFFER_SIZE - n, "%lds", m_Secs);
  }

  void formatLong() {
    std::size_t n = 0;

    if (m_Days) {
      snprintf(m_Buffer, TEMP_BUFFER_SIZE, "%ld day", m_Days);
      n = std::strlen(m_Buffer);
      if (m_Days > 1)
        m_Buffer[++n] = 's';
    }

    auto appendAnd = [&]() {
      std::memcpy(m_Buffer + n, AND_STRING, AND_STRING_LEN);
      n += AND_STRING_LEN;
    };

    auto appendComma = [&]() {
      std::memcpy(m_Buffer + n, COMMA_STRING, COMMA_STRING_LEN);
      n += COMMA_STRING_LEN;
    };

    if (m_Hours) {
      if (m_Days) {
        if (!m_Mins && !m_Secs)
          appendAnd();
        else
          appendComma();
      }
      snprintf(m_Buffer + n, TEMP_BUFFER_SIZE - n, "%ld hour", m_Hours);
      n = std::strlen(m_Buffer);
      if (m_Hours > 1)
        m_Buffer[++n] = 's';
    }

    if (m_Mins) {
      if (m_Days || m_Hours) {
        if (!m_Secs)
          appendAnd();
        else
          appendComma();
      }
      snprintf(m_Buffer + n, TEMP_BUFFER_SIZE - n, "%ld minute", m_Mins);
      n = std::strlen(m_Buffer);
      if (m_Mins > 1)
        m_Buffer[++n] = 's';
    }

    if (m_Secs) {
      if (m_Days || m_Hours || m_Mins)
        appendAnd();
      snprintf(m_Buffer + n, TEMP_BUFFER_SIZE - n, "%ld second", m_Secs);
      n = std::strlen(m_Buffer);
      if (m_Secs > 1) {
        m_Buffer[n] = 's';
        m_Buffer[++n] = '\0';
      }
    }
  }

  void formatDefault() {
    if (m_Days > 0)
      m_Hours += m_Days * Timer::HOURS_PER_DAY;
    snprintf(m_Buffer, TEMP_BUFFER_SIZE, "%02ld:%02ld:%02ld", m_Hours, m_Mins,
             m_Secs);
  }

  long m_Days;
  long m_Hours;
  long m_Mins;
  long m_Secs;
  char m_Buffer[TEMP_BUFFER_SIZE];
};

constexpr char ElapsedTimeString::AND_STRING[];
constexpr char ElapsedTimeString::COMMA_STRING[];

} // namespace

void Timer::start() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  m_Start = ts.tv_sec * NANOS_PER_SEC + ts.tv_nsec;
}

void Timer::stop() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  m_Stop = ts.tv_sec * NANOS_PER_SEC + ts.tv_nsec;
}

std::string Timer::getFormatString(FormatStringStyle style) const {
  ElapsedTimeString ets{style, getElapsedSecs()};
  return ets.getString();
}

std::string getDateTimeFormatString(const char *format) {
  std::time_t now = std::time(nullptr);
  std::tm *t = std::localtime(&now);
  char buffer[TEMP_BUFFER_SIZE];
  std::strftime(buffer, TEMP_BUFFER_SIZE, format, t);
  return buffer;
}

} // namespace time
} // namespace jig
