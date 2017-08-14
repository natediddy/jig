//===--- file.cc --------------------------------------------------------===//
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

#include "file.h"

#include <sys/stat.h>

#include <cerrno>
#include <cstring>

#include "logger.h"
#include "util.h"

namespace jig {
namespace {

File::Type getTypeFromMode(mode_t m) {
  if (S_ISBLK(m))
    return File::Type::BLOCK_DEVICE;
  if (S_ISCHR(m))
    return File::Type::CHARACTER_DEVICE;
  if (S_ISDIR(m))
    return File::Type::DIRECTORY;
  if (S_ISFIFO(m))
    return File::Type::FIFO;
  if (S_ISREG(m))
    return File::Type::REGULAR;
  if (S_ISSOCK(m))
    return File::Type::SOCKET;
  if (S_ISLNK(m))
    return File::Type::SYMLINK;
  return File::Type::UNKNOWN;
}

} // namespace

bool File::directoryExists(const Path &path) {
  struct stat statBuf;
  return stat(path.getCString(), &statBuf) == 0 && S_ISDIR(statBuf.st_mode);
}

bool File::fileExists(const Path &path) {
  struct stat statBuf;
  return stat(path.getCString(), &statBuf) == 0 && S_ISREG(statBuf.st_mode);
}

void File::update() {
  if (isOpen())
    close();
  m_Size = UINT64_C(0);
  m_Type = Type::UNKNOWN;
  m_Exists = false;
  initStats();
}

void File::open(const char *mode) {
  m_Error = 0;
  m_FPtr = std::fopen(m_Path.getCString(), mode);
  if (!m_FPtr) {
    m_Error = errno;
    Logger::error("failed to open file `%s' (mode=\"%s\") -- %s",
                  m_Path.getCString(), mode, std::strerror(m_Error));
  }
}

void File::close() {
  m_Error = 0;
  if (std::fclose(m_FPtr) != 0) {
    m_Error = errno;
    Logger::error("failed to properly close file `%s' -- %s",
                  m_Path.getCString(), std::strerror(m_Error));
  }
  m_FPtr = nullptr;
}

int File::readByte() {
  m_Error = 0;
  int byte = std::fgetc(m_FPtr);
  if (byte == EOF && std::ferror(m_FPtr)) {
    m_Error = errno;
    Logger::error("failed to read byte from file `%s' -- %s",
                  m_Path.getCString(), std::strerror(m_Error));
  }
  return byte;
}

void File::writeByte(int byte) {
  m_Error = 0;
  if (std::fputc(byte, m_FPtr) == EOF) {
    m_Error = errno;
    Logger::error("failed to write byte to file `%s' -- %s",
                  m_Path.getCString(), std::strerror(m_Error));
  }
}

std::string File::readContents() {
  open();
  if (hadError())
    return "";

  std::string contents;
  contents.reserve(m_Size);

  int ch;
  while (true) {
    ch = std::fgetc(m_FPtr);
    if (ch == EOF)
      break;
    contents += static_cast<char>(ch);
  }

  if (std::ferror(m_FPtr)) {
    close();
    m_Error = errno;
    Logger::error("failed to read contents from file `%s' -- %s",
                  m_Path.getCString(), std::strerror(m_Error));
    return "";
  }

  close();
  return contents;
}

void File::writeContents(const std::string &contents) {
  open("wb");
  if (hadError())
    return;

  std::fwrite(contents.data(), sizeof(std::string::value_type), contents.size(),
              m_FPtr);
  close();
}

const char *File::errorMessage() const {
  return std::strerror(m_Error);
}

void File::initStats() {
  struct stat statBuf;

  std::memset(&statBuf, 0, sizeof(struct stat));
  m_Error = 0;
  if (stat(m_Path.getCString(), &statBuf) == 0) {
    m_Size = static_cast<uint64_t>(statBuf.st_size);
    m_Type = getTypeFromMode(statBuf.st_mode);
    m_Exists = true;
  } else if (errno != ENOENT)
    m_Error = errno;
}

} // namespace jig
