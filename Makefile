# ---- Project layout -------------------------------------------------------
# headers/                -> all .h files
# src/                    -> all .cpp files (compiled as translation units)
# build/                  -> intermediate .o and .d files (auto-created)
# bin/pohto               -> final executable
#
# Drop new .h files into headers/ and new .cpp files into src/ and they will
# be picked up automatically on the next `make` — no need to edit this file.
#
# EXCEPTION: any file ending in "-functions.cpp" (e.g. window-functions.cpp)
# is treated as an inline-include file, not a standalone translation unit,
# because window.cpp does `#include "window-functions.cpp"` directly.
# If such a file were also compiled on its own, every symbol inside it would
# be defined twice and the linker would fail. If you later refactor away
# from the #include-a-.cpp-file pattern, just rename the file so it no
# longer matches *-functions.cpp and it'll be picked up like any other
# source file automatically.
# -----------------------------------------------------------------------

TARGET      := bin/pohto

HEADER_DIR  := headers
SRC_DIR     := src
BUILD_DIR   := build

# All .cpp files under src/, recursively, EXCLUDING *-functions.cpp
# (those are #include'd directly by another .cpp, not compiled standalone).
ALL_SRCS    := $(shell find $(SRC_DIR) -name '*.cpp')
EXCLUDE_SRCS:= $(shell find $(SRC_DIR) -name '*-functions.cpp')
SRCS        := $(filter-out $(EXCLUDE_SRCS),$(ALL_SRCS))

OBJS        := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS        := $(OBJS:.o=.d)

# ---- Toolchain / flags ------------------------------------------------
CXX         := g++
WX_CXXFLAGS := $(shell wx-config --cxxflags)
WX_LIBS     := $(shell wx-config --libs)

CXXFLAGS    := -std=c++17 -Wall -I$(HEADER_DIR) $(WX_CXXFLAGS) -MMD -MP
LDFLAGS     := $(WX_LIBS)

# ---- Rules --------------------------------------------------------------
.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) bin

-include $(DEPS)
