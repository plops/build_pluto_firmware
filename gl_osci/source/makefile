all: vis

CXX=clang++
# -Og -g3 -ggdb 
# -Wno-error=inaccessible-base
# -Wshadow -Wdouble-promotion -fno-common -Wconversion 
# -Werror
# gnu++2a
CXXFLAGS=-Og -g3 -ggdb \
-std=gnu++2a  \
-Wall -Wextra \
-ffunction-sections -fdata-sections \
-Wno-error=unused-variable \
-Wno-error=unused-parameter \
-Wno-error=double-promotion \
-pipe \
-march=native -mtune=native \
-I/home/martin/src/imgui

LDFLAGS= \
-lglfw -lGL \
-Wl,--gc-sections \
-Wl,--print-gc-sections 

CFILES:=$(shell find .| grep .cpp$)
OBJ:=$(CFILES:%.cpp=%.o)

IMGUI_FILES=imgui.cpp \
imgui_draw.cpp \
imgui_demo.cpp \
imgui_widgets.cpp \
backends/imgui_impl_glfw.cpp \
backends/imgui_impl_opengl2.cpp

IMGUI_OBJ:=$(IMGUI_FILES:%.cpp=/home/martin/src/imgui/%.o)

all: vis

%.o: %.cpp globals.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)

vis: $(OBJ) $(IMGUI_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm vis $(OBJ) $(IMGUI_OBJ)
