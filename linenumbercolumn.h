#ifndef __JIG_LINENUMBERCOLUMN_H__
#define __JIG_LINENUMBERCOLUMN_H__

#include "view.h"

namespace jig {

class LineNumberColumn : public View {
public:
  LineNumberColumn() = default;

  virtual void init() final;
  virtual void updateDimensions() final;
  virtual int getKeypress() override;

  void update();

private:
  void initWindow();
  void writeToWindow();

  std::size_t m_FirstLineIndex = 0;
  std::size_t m_TotalLines = 0;
  int m_MaxDigits = 0;
};

} // namespace jig

#endif // __JIG_LINENUMBERCOLUMN_H__
