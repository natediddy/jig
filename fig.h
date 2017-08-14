//===--- fig.h ----------------------------------------------------------===//
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

#ifndef __JIG_FIG_H__
#define __JIG_FIG_H__

#include "buffer.h"
#include "file.h"
#include "path.h"
#include "settings.h"

namespace jig {

class Fig {
public:
  Fig();
  Fig(const Path &path);

  const Path &getPath() const;

  template <typename T>
  T get(const std::string &key) const {
    return m_Settings.get<T>(key);
  }

  template <typename T>
  void set(const std::string &key, T value) {
    m_Settings.set<T>(key, value);
  }

private:
  void parseSettings();

  std::unique_ptr<File> m_File;
  Buffer m_Buffer;
  Settings m_Settings;
};

} // namespace jig

#endif // __JIG_FIG_H__
