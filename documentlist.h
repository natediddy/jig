//===--- documentlist.h -------------------------------------------------===//
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

#ifndef __JIG_DOCUMENTLIST_H__
#define __JIG_DOCUMENTLIST_H__

#include <vector>

#include "document.h"

namespace jig {

class DocumentList {
public:
  DocumentList() = default;

  Document &getCurrent() { return m_List[m_CurrentIndex]; }
  const Document &getCurrent() const { return m_List[m_CurrentIndex]; }

  std::size_t getCurrentIndex() const { return m_CurrentIndex; }

  Document &operator[](std::size_t which);
  const Document &operator[](std::size_t which) const;

  std::size_t getTotal() const { return m_List.size(); }
  bool isEmpty() const { return m_List.empty(); }

  void addNew(Document doc) { m_List.push_back(std::move(doc)); }

  void setNextAsCurrent();
  void setPreviousAsCurrent();

  auto begin() { return m_List.begin(); }
  const auto begin() const { return m_List.begin(); }

  auto end() { return m_List.end(); }
  const auto end() const { return m_List.end(); }

private:
  std::vector<Document> m_List;
  std::size_t m_CurrentIndex = 0;
};

} // namespace jig

#endif // __JIG_DOCUMENTLIST_H__
