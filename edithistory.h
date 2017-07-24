//===--- edithistory.h --------------------------------------------------===//
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

#ifndef __JIG_EDITHISTORY_H__
#define __JIG_EDITHISTORY_H__

#include <vector>

#include "edit.h"

namespace jig {

class Buffer;

class EditHistory {
public:
  EditHistory() = default;
  EditHistory(EditHistory &&) = default;
  EditHistory &operator=(EditHistory &&) = default;

  void addNew(std::unique_ptr<Edit> &&edit);

  void undo(Buffer &buffer) { m_List[m_MostRecentIndex--]->undo(buffer); }
  void redo(Buffer &buffer) { m_List[++m_MostRecentIndex]->apply(buffer); }

  bool canUndo() const { return m_MostRecentIndex > -1; }
  bool canRedo() const { return m_MostRecentIndex != m_List.size() - 1; }

private:
  std::vector<std::unique_ptr<Edit>> m_List;
  ssize_t m_MostRecentIndex = -1;
};

} // namespace jig

#endif // __JIG_EDITHISTORY_H__
