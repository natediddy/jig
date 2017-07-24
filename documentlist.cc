//===--- documentlist.cc ------------------------------------------------===//
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

#include "documentlist.h"

#include <assert.h>

#include "app.h"

namespace jig {

Document &DocumentList::operator[](std::size_t which) {
  assert(which < m_List.size() && "Index is out of bounds");
  return m_List[which];
}

const Document &DocumentList::operator[](std::size_t which) const {
  assert(which < m_List.size() && "Index is out of bounds");
  return m_List[which];
}

void DocumentList::setNextAsCurrent() {
  if (m_CurrentIndex == m_List.size() - 1)
    m_CurrentIndex = 0;
  else
    ++m_CurrentIndex;
  App::getInstance().getUI().getBufferView().clear();
}

void DocumentList::setPreviousAsCurrent() {
  if (m_CurrentIndex == 0)
    m_CurrentIndex = m_List.size() - 1;
  else
    --m_CurrentIndex;
  App::getInstance().getUI().getBufferView().clear();
}

} // namespace jig
