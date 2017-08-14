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
