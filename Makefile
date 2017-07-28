CXX = clang++
CXXFLAGS = -g -Wall -Werror -std=c++14 -stdlib=libc++ -fno-exceptions -fno-rtti

ifeq ($(debug),)
	CXXFLAGS += -O2 -march=native -mtune=native -DNDEBUG
endif

LIBS = -lncurses

TARGET = jig

SOURCES = app.cc \
	  buffer.cc \
	  bufferview.cc \
	  color.cc \
	  document.cc \
	  documentlist.cc \
	  edit.cc \
	  edithistory.cc \
	  file.cc \
	  logger.cc \
	  main.cc \
	  path.cc \
	  selectmodehandler.cc \
	  statusbar.cc \
	  system.cc \
	  timeutils.cc \
	  titlebar.cc \
	  ui.cc \
	  view.cc \
	  window.cc

HEADERS = app.h \
	  buffer.h \
	  bufferview.h \
	  clipboard.h \
	  color.h \
	  document.h \
	  documentlist.h \
	  edit.h \
	  edithistory.h \
	  file.h \
	  line.h \
	  logger.h \
	  path.h \
	  selectmodehandler.h \
	  statusbar.h \
	  system.h \
	  timeutils.h \
	  titlebar.h \
	  ui.h \
	  view.h \
	  window.h

OBJECTS = $(SOURCES:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJECTS)

clobber:
	@rm -f $(OBJECTS) $(TARGET)

.PHONY: clean clobber
