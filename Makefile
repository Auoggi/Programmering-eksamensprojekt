# Learning rescources: 
# https://makefiletutorial.com
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

.PRECIOUS: %.o %.exe.o

SRC_DIR := src
BUILD_DIR := .build
MAIN_FILE := game

CXXFLAGS := -g -I include
FLAGS := $(CXXFLAGS) $(shell pkg-config --static --libs glfw3 gl)
DEPS := include/glad/glad.o $(patsubst %.cpp, %.o, $(shell fdfind -e cpp . $(SRC_DIR)))
CXX := g++
LD := ld

SHADER_DIR := $(SRC_DIR)/render/shaders
SHADER_SYMBOLS_FILE := $(SHADER_DIR)/shader_symbols.h
SHADERS := $(patsubst %.glsl, %.o, $(shell fdfind -e glsl . $(SHADER_DIR)))
DEPS += $(SHADERS)

%.exe %.exe.o: FLAGS := $(CXXFLAGS) ./include/GLFW/lib-mingw-w64/libglfw3.a -lopengl32 -lgdi32
%.exe %.exe.o: CXX := x86_64-w64-mingw32-g++
%.exe %.exe.o: LD := x86_64-w64-mingw32-ld

define compile =
$(MAKE) $(BUILD_DIR) $(SHADER_SYMBOLS_FILE)
$(CXX) $^ -o $@ $(FLAGS)
endef

$(BUILD_DIR)/%: $(SRC_DIR)/%.o $(DEPS)
	$(compile)
$(BUILD_DIR)/%.exe: $(SRC_DIR)/%.exe.o $(DEPS:.o=.exe.o) 
	$(compile)

%.o %.exe.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(SHADER_DIR)/%.o $(SHADER_DIR)/%.exe.o: $(SHADER_DIR)/%.glsl
	$(LD) -r -b binary -o $@ $<
	
$(SHADER_SYMBOLS_FILE): $(SHADERS)
	nm --defined-only $^ | awk '/_binary_.+_start/ {print "extern char " $$3 "[];";}' > $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: run
run: $(BUILD_DIR)/$(MAIN_FILE)
	$<

.PHONY: build
build:
	$(MAKE) CXXFLAGS="$(filter-out -g, $(CXXFLAGS))" $(BUILD_DIR)/$(MAIN_FILE)
	$(MAKE) CXXFLAGS="$(filter-out -g, $(CXXFLAGS))" $(BUILD_DIR)/$(MAIN_FILE).exe

.PHONY: clean
clean:
	rm -f $(shell fdfind -I -e o)
	rm -rf $(BUILD_DIR)