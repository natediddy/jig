//===--- view.cc --------------------------------------------------------===//
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

#include "view.h"

#include <assert.h>

#include "logger.h"

namespace jig {

void View::draw() {
  assert(m_Window != nullptr && "This View's Window object is unitialized");
  m_Window->refresh();
}

void View::clear() {
  assert(m_Window != nullptr && "This View's Window object is unitialized");
  m_Window->clear();
}

int View::getHeight() const {
  assert(m_Window != nullptr && "This View's Window object is unitialized");
  return m_Window->getHeight();
}

int View::getWidth() const {
  assert(m_Window != nullptr && "This View's Window object is unitialized");
  return m_Window->getWidth();
}

int View::getStartY() const {
  assert(m_Window != nullptr && "This View's Window object is unitialized");
  return m_Window->getStartY();
}

int View::getStartX() const {
  assert(m_Window != nullptr && "This View's Window object is unitialized");
  return m_Window->getStartX();
}

int View::getKeypress() {
  assert(m_Window != nullptr && "This View's Window object is unitialized");
  return m_Window->getKeypress();
}

void View::initWindow(int height, int width, int startY, int startX) {
  m_Window = std::make_unique<Window>(height, width, startY, startX);
}

void View::writeToWindow() {
  assert(m_Window != nullptr && "This View's Window object is unitialized");
}

} // namespace jig
