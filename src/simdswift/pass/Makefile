#SOURCES = simdswift.cpp
#LIB_NAME = simdswift_pass.so
ifndef SOURCES
  SOURCES = avxswift.cpp
endif
LIB_NAME = avxswift_pass.so

include ../../Makefile.local

OBJ = $(addsuffix .o, $(basename ${SOURCES}))

LDFLAGS = $(shell $(LLVM_PATH)/llvm-config --ldflags)
CXXFLAGS = -g -Wall -fno-rtti -fPIC -shared $(shell $(LLVM_PATH)/llvm-config --cxxflags)

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	g++ $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *~
	rm -f $(LIB_NAME)

