.PHONY: main
ifneq ($V,1)
Q ?= @
endif

GXX = g++
ERRORS = -Wall
CFLAGS = -c $(ERRORS) -fPIC -O0
BUILD_DIR = build
EXE_NAME = pnp
DLL_NAME = pnp
SRC_DIR = src
SOURCES += $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
HEADERS += $(wildcard $(SRC_DIR)/*.hpp)

IMGUI_DIR = src/imgui
# IMGUI_SOURCES = $(wildcard $(IMGUI_DIR)/*.cpp)
IMGUI_SOURCES = $(wildcard $(IMGUI_DIR)/*.cpp)
IMGUI_HEADERS = $(wildcard $(IMGUI_DIR)/*.h)

IMGUI_BACKENDS_DIR = $(IMGUI_DIR)/backends
# IMGUI_BACKENDS_SOURCES = $(wildcard $(IMGUI_BACKENDS_DIR)/*.cpp)
IMGUI_BACKENDS_SOURCES = $(IMGUI_BACKENDS_DIR)/imgui_impl_win32.cpp \
	$(IMGUI_BACKENDS_DIR)/imgui_impl_dx12.cpp
IMGUI_BACKENDS_HEADERS = $(IMGUI_BACKENDS_DIR)/imgui_impl_win32.h \
	$(IMGUI_BACKENDS_DIR)/imgui_impl_dx12.h

IMGUI_MISC_DIR = $(IMGUI_DIR)/misc/cpp
# IMGUI_MISC_SOURCES = $(wildcard $(IMGUI_MISC_DIR)/*.cpp)
IMGUI_MISC_SOURCES = $(IMGUI_MISC_DIR)/imgui_stdlib.cpp
IMGUI_MISC_HEADERS = $(IMGUI_MISC_DIR)/imgui_stdlib.h

SOURCES += $(IMGUI_SOURCES)
SOURCES += $(IMGUI_BACKENDS_SOURCES)
SOURCES += $(IMGUI_MISC_SOURCES)

HEADERS += $(IMGUI_HEADERS)
HEADERS += $(IMGUI_BACKENDS_HEADERS)
HEADERS += $(IMGUI_MISC_HEADERS)

TARGET = 
ifeq ($(OS),Windows_NT)
TARGET = windows
else
TARGET = linux
endif

ifeq ($(TARGET),windows)
RM = cmd /c del /Q /F
COPY = copy
MOVE = move
SYS_FLAGS = -D_WIN32 -DWIN32
EXE_EXTENSION = .exe
DLL_EXTENSION = .dll
ARCHIVE_EXTENSION = .lib
OBJECT_EXTENSION = .obj
ECHO = @echo
SYS_MSG = "Windows_NT detected!"
C_LIBS += -lopengl32 -lgdi32 -luser32 -lkernel32 -ldwmapi
LDFLAGS = -static-libgcc -static-libstdc++ -mwindows
LDFLAGS += -L"$(DXSDK_DIR)Lib/x64" 
LDFLAGS += -ld3d12 -ldxgi -ld3dcompiler -ldxguid
else
RM = rm -rf
COPY = cp
MOVE = mv
SYS_FLAGS = -D  
EXE_EXTENSION =
DLL_EXTENSION = .so
ARCHIVE_EXTENSION = .a
OBJECT_EXTENSION = .o
ECHO = @echo
SYS_MSG = "Linux detected!"
C_LIBS +=
endif

C_INCLUDE_PATH = -I. -I./$(IMGUI_DIR) -I./$(IMGUI_BACKENDS_DIR) -I./$(IMGUI_MISC_DIR) -I"$(DXSDK_DIR)Include"
C_LINKER_PATH = -L./$(BUILD_DIR)

# Object files
OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%$(OBJECT_EXTENSION), $(SOURCES))

# all: $(EXE_NAME)$(EXE_EXTENSION)
main: $(EXE_NAME)$(EXE_EXTENSION)

$(EXE_NAME)$(EXE_EXTENSION): build_dir $(OBJECTS)
	$Q $(ECHO) [linking] $@
	$Q $(GXX) $(C_LINKER_PATH) -o $(BUILD_DIR)/$@ $(OBJECTS) $(C_LIBS) $(LDFLAGS)

# Compile Objects
$(BUILD_DIR)/%$(OBJECT_EXTENSION): %.cpp $(HEADERS) build_dir
	$Q $(ECHO) [compiling] $<
	$Q $(GXX) $(CFLAGS) $(C_INCLUDE_PATH) $(SYS_FLAGS) -c -o $@ $<

build_dir:
ifeq ($(TARGET),windows)
	$Q @if not exist $(BUILD_DIR)\src mkdir $(BUILD_DIR)\src
	$Q @if not exist $(BUILD_DIR)\src\imgui mkdir $(BUILD_DIR)\src\imgui
	$Q @if not exist $(BUILD_DIR)\src\imgui\backends mkdir $(BUILD_DIR)\src\imgui\backends
	$Q @if not exist $(BUILD_DIR)\src\imgui\misc\cpp mkdir $(BUILD_DIR)\src\imgui\misc\cpp
else
	$Q @mkdir -p $(BUILD_DIR)/src
	$Q @mkdir -p $(BUILD_DIR)\src/imgui
	$Q @mkdir -p $(BUILD_DIR)\src/imgui/backends
	$Q @mkdir -p $(BUILD_DIR)\src/imgui/misc/cpp
endif