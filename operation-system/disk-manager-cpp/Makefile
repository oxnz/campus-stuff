CXXFLAGS	:= -Wall -std=c++11 -DDEBUG
LDFLAGS		:=
CXX		:= clang++

TARGET = dm
HEADERS += diskmgt.h block.h utils.h
SOURCES += main.cpp diskmgt.cpp block.cpp
OBJECTS += $(patsubst %.cpp,%.o, $(SOURCES))

all: dm test
test: test.o block.o
	$(CXX) $^ -o $@ $(LDFLAGS)
$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJECTS): $(HEADERS) $(SOURCES)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)
	$(RM) test
