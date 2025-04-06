# Learning rescources: 
# https://makefiletutorial.com
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

.PRECIOUS: %.o %.exe.o

SRC_DIR := src
BUILD_DIR := .build
MAIN_FILE := game

CXXFLAGS := -g -I include
FLAGS := $(CXXFLAGS) $(shell pkg-config --static --libs glfw3 gl)
DEPS := include/glad/glad.o $(patsubst %.cpp, %.o, $(shell fdfind -e cpp -e h . $(SRC_DIR)))
CXX := g++
LD := ld

SHADER_DIR := $(SRC_DIR)/render/shaders
SHADER_SYMBOLS_FILE := $(SHADER_DIR)/shader_symbols.h
SHADERS := $(patsubst %.glsl, %.o, $(shell fdfind -e glsl . $(SHADER_DIR)))
DEPS += $(SHADERS) $(SHADER_SYMBOLS_FILE)

%.exe %.exe.o: FLAGS := $(CXXFLAGS) ./include/GLFW/lib-mingw-w64/libglfw3.a -lopengl32 -lgdi32
%.exe %.exe.o: CXX := x86_64-w64-mingw32-g++
%.exe %.exe.o: LD := x86_64-w64-mingw32-ld

define compile =
$(MAKE) $(BUILD_DIR)
$(CXX) $(filter-out %.h, $^) -o $@ $(FLAGS)
endef

$(BUILD_DIR)/%: $(SHADER_SYMBOLS_FILE) $(SRC_DIR)/%.o $(DEPS)
	$(compile)
$(BUILD_DIR)/%.exe: $(SHADER_SYMBOLS_FILE) $(SRC_DIR)/%.exe.o $(DEPS:.o=.exe.o) 
	$(compile)

%.o %.exe.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(SHADER_DIR)/%.o $(SHADER_DIR)/%.exe.o: $(SHADER_DIR)/%.glsl
	$(LD) -r -b binary -o $@ $<
	
$(SHADER_SYMBOLS_FILE): $(SHADERS)
	echo "// THIS FILE IS AUTO-GENERATED, DO NOT EDIT\n" > $@
	nm --defined-only $^ | grep "_end" | gawk 'match($$0, /_binary_.+_glsl/) { \
		print "namespace shader_symbols {"; \
		m = "shader_symbols::" substr($$0, RSTART, RLENGTH); \
		print "\textern char " m "_start[];"; \
		print "\textern char " m "_end[];\n}"; \
		s = gensub(/shader_symbols::_binary_$(strip $(subst /,_, $(SHADER_DIR)))_/, "", "g", m); \
		print "const int " s "_size = " m "_end - " m "_start;"; \
		print "const char *" s " = " m "_start;"; \
		print "#define " toupper(s) " " s ", " s "_size\n"; \
	}' >> $@

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
	$(MAKE) -B $(SHADER_SYMBOLS_FILE)
	rm -rf $(BUILD_DIR) $(shell fdfind -I -e o) 