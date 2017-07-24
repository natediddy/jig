//===--- edit.h ---------------------------------------------------------===//
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

#ifndef __JIG_EDIT_H__
#define __JIG_EDIT_H__

#include <string>

namespace jig {

class Buffer;

class Edit {
public:
  Edit(std::size_t pos) : m_Pos{pos} {}
  virtual void apply(Buffer &buffer) = 0;
  virtual void undo(Buffer &buffer) = 0;
  bool isApplied() const { return m_Applied; }

protected:
  std::size_t m_Pos;
  bool m_Applied = false;
};

class InsertEdit : public Edit {
public:
  InsertEdit(std::size_t pos, char ch) : Edit{pos}, m_Inserted{ch} {}
  InsertEdit(std::size_t pos, std::string str)
    : Edit{pos}, m_Inserted{std::move(str)} {}

  void apply(Buffer &buffer) final;
  void undo(Buffer &buffer) final;

private:
  std::string m_Inserted;
};

class EraseBackEdit : public Edit {
public:
  EraseBackEdit(std::size_t pos, std::size_t count)
    : Edit{pos}, m_Count{count} {}

  void apply(Buffer &buffer) final;
  void undo(Buffer &buffer) final;

private:
  std::size_t m_Count;
  std::string m_Erased;
};

class EraseFrontEdit : public Edit {
public:
  EraseFrontEdit(std::size_t pos, std::size_t count)
    : Edit{pos}, m_Count{count} {}

  void apply(Buffer &buffer) final;
  void undo(Buffer &buffer) final;

private:
  std::size_t m_Count;
  std::string m_Erased;
};

class ReplaceEdit : public Edit {
public:
  ReplaceEdit(std::size_t pos, std::size_t count, char ch)
    : Edit{pos}, m_Count{count}, m_Erased{""}, m_Inserted{ch} {}
  ReplaceEdit(std::size_t pos, std::size_t count, std::string str)
    : Edit{pos}, m_Count{count}, m_Erased{""}, m_Inserted{std::move(str)} {}

  void apply(Buffer &buffer) final;
  void undo(Buffer &buffer) final;

private:
  std::size_t m_Count;
  std::string m_Erased;
  std::string m_Inserted;
};

} // namespace jig

#endif // __JIG_EDIT_H__
