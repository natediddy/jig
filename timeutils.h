//===--- timeutils.h ----------------------------------------------------===//
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

#ifndef __JIG_TIMEUTILS_H__
#define __JIG_TIMEUTILS_H__

#include <string>

namespace jig {
namespace time {

class Timer {
public:
  static constexpr long HOURS_PER_DAY = 24L;
  static constexpr long SECS_PER_DAY = 86400L;
  static constexpr long SECS_PER_HOUR = 3600L;
  static constexpr long SECS_PER_MINUTE = 60L;
  static constexpr long NANOS_PER_SEC = 1000000000L;
  static constexpr long NANOS_PER_MICRO = 1000L;
  static constexpr long NANOS_PER_MILLI = 1000000L;
  static constexpr long MICROS_PER_SEC = 1000000L;
  static constexpr long MILLIS_PER_SEC = 1000L;

  Timer() = default;

  void start();
  void stop();

  long getElapsedNanos() const { return m_Stop - m_Start; }

  long getElapsedMicros() const {
    return (m_Stop / NANOS_PER_MICRO) - (m_Start / NANOS_PER_MICRO);
  }

  long getElapsedMillis() const {
    return (m_Stop / NANOS_PER_MILLI) - (m_Start / NANOS_PER_MILLI);
  }

  long getElapsedSecs() const {
    return (m_Stop / NANOS_PER_SEC) - (m_Start / NANOS_PER_SEC);
  }

  enum class FormatStringStyle {
    DEFAULT, // "HH:MM:SS" E.g.: "27:51:26"

    // Note that with the following two styles, if a number is zero then it
    // won't be included in the formatted output. For example if the number of
    // minutes turned out to be zero, the SHORT example would look like
    // "1d3h26s", and the LONG example would look like
    // "1 day, 3 hours and 26 seconds".

    SHORT, // "DdHhMmSs" E.g.: "1d3h51m26s"
    LONG,  // "D day(s), H hour(s), M minute(s) and S second(s)"
           // E.g.: "1 day, 3 hours, 51 minutes and 26 seconds"
  };

  std::string
  getFormatString(FormatStringStyle style = FormatStringStyle::DEFAULT) const;

private:
  long m_Start = 0L;
  long m_Stop = 0L;
};

// The "format" argument is passed straight to std::strftime.
// The default value for "format" gives the full date and time:
//   <day of week> <month> <day of month> <time> <timezone> <year>
// E.g:
//   Wed Jul 19 09:44:42 PM EDT 2017
std::string getDateTimeFormatString(const char *format = "%a %b %e %r %Z %Y");

} // namespace time
} // namespace jig

#endif // __JIG_TIMEUTILS_H__
