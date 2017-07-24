//===--- edithistory.cc -------------------------------------------------===//
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

#include "edithistory.h"

namespace jig {

void EditHistory::addNew(std::unique_ptr<Edit> &&edit) {
  std::size_t n = m_List.size();
  if (n > 0 && m_MostRecentIndex < n - 1)
    m_List.erase(m_List.begin() + (m_MostRecentIndex + 1), m_List.end());
  m_List.push_back(std::move(edit));
  ++m_MostRecentIndex;
}

} // namespace jig
