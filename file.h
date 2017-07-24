//===--- file.h ---------------------------------------------------------===//
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

#ifndef __JIG_FILE_H__
#define __JIG_FILE_H__

#include <cstdint>
#include <cstdio>
#include <string>

#include "path.h"

namespace jig {

class File {
public:
  enum class Type {
    UNKNOWN,
    BLOCK_DEVICE,
    CHARACTER_DEVICE,
    DIRECTORY,
    FIFO,
    REGULAR,
    SOCKET,
    SYMLINK
  };

  static bool directoryExists(const Path &path);
  static bool fileExists(const Path &path);

  File() : m_Path{""} {}
  File(const std::string &path);

  ~File() {
    if (m_FPtr)
      close();
  }

  const Path &getPath() const { return m_Path; }
  uint64_t getSize() const { return m_Size; }
  Type getType() const { return m_Type; }
  bool exists() const { return m_Exists; }

  void update();

  void open(const char *mode = "rb");
  void close();

  int readByte();
  void writeByte(int byte);

  bool isOpen() const { return m_FPtr != nullptr; }

  std::string readContents();
  void writeContents(const std::string &contents);

  bool hadError() const { return m_Error != 0; }
  const char *errorMessage() const;

private:
  void initStats();

  Path m_Path;
  std::FILE *m_FPtr = nullptr;
  uint64_t m_Size = UINT64_C(0);
  Type m_Type = Type::UNKNOWN;
  int m_Error = 0;
  bool m_Exists = false;
};

} // namespace jig

#endif // __JIG_FILE_H__
