OPTIMIZATION_ON ?= 1
ASAN ?= 0
DEPRECATION_OFF ?= 0
DEBUG ?= 0
COPYCHECK_ARGS ?=
LLD ?= 0
WERROR ?= 0

CC := g++
INC := -I ZAPD -I lib/assimp/include -I lib/elfio -I lib/json/include -I lib/stb -I lib/tinygltf -I lib/libgfxd -I lib/tinyxml2
CFLAGS += -fpic -std=c++17 -rdynamic -Wall -fno-omit-frame-pointer

ifneq ($(DEBUG),0)
  OPTIMIZATION_ON = 0
  DEPRECATION_OFF = 1
  CFLAGS += -g3 -DDEVELOPMENT
  COPYCHECK_ARGS += --devel
  DEPRECATION_ON = 0
endif

ifneq ($(WERROR),0)
  CXXFLAGS += -Werror
endif

ifeq ($(OPTIMIZATION_ON),0)
  CFLAGS += -O0
else
  OPTFLAGS := -O2
endif
ifneq ($(ASAN),0)
  CFLAGS += -fsanitize=address
endif
ifneq ($(DEPRECATION_OFF),0)
  CFLAGS += -DDEPRECATION_OFF
endif
# CFLAGS += -DTEXTURE_DEBUG

LDFLAGS := -ldl -lpng
UNAME := $(shell uname)
UNAMEM := $(shell uname -m)
ifneq ($(UNAME), Darwin)
  LDFLAGS += -Wl,-export-dynamic -lstdc++fs
  EXPORTERS := -Wl,--whole-archive ExporterTest/ExporterTest.a -Wl,--no-whole-archive
else
  EXPORTERS := -Wl,-force_load ExporterTest/ExporterTest.a
  ifeq ($(UNAMEM),arm64)
    ifeq ($(shell brew list libpng > /dev/null 2>&1; echo $$?),0)
      LDFLAGS += -L $(shell brew --prefix)/lib
      INC += -I $(shell brew --prefix)/include
      else
      $(error Please install libpng via Homebrew)
    endif
  endif
endif


ZAPD_SRC_DIRS := $(shell find ZAPD -type d)
SRC_DIRS = $(ZAPD_SRC_DIRS) lib/tinyxml2

ZAPD_CPP_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
ZAPD_H_FILES   := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h))

CPP_FILES += $(ZAPD_CPP_FILES) lib/tinyxml2/tinyxml2.cpp
O_FILES   := $(CPP_FILES:.cpp=.o)

all: ZAPD.out copycheck

genbuildinfo:
	python3 ZAPD/genbuildinfo.py $(COPYCHECK_ARGS)

copycheck: ZAPD.out
	python3 copycheck.py

clean:
	rm -f $(O_FILES) ZAPD.out
	$(MAKE) -C lib/libgfxd clean

rebuild: clean all

format:
	clang-format-11 -i $(ZAPD_CPP_FILES) $(ZAPD_H_FILES)

.PHONY: all genbuildinfo copycheck clean rebuild format

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(LDFLAGS)

ZAPD/Main.o: genbuildinfo ZAPD/Main.cpp
	$(CC) $(CFLAGS) $(INC) -c ZAPD/Main.cpp -o $@ $(LDFLAGS)

lib/libgfxd/libgfxd.a:
	$(MAKE) -C lib/libgfxd

.PHONY: ExporterTest
ExporterTest:
	$(MAKE) -C ExporterTest

.PHONY: ZAPDUtils
ZAPDUtils:
	$(MAKE) -C ZAPDUtils


# Linking
ZAPD.out: $(O_FILES) lib/libgfxd/libgfxd.a ExporterTest ZAPDUtils
	$(CXX) $(CXXFLAGS) $(O_FILES) lib/libgfxd/libgfxd.a ZAPDUtils/ZAPDUtils.a $(EXPORTERS) $(LDFLAGS) $(OUTPUT_OPTION)
