PICO_SDK_PATH=~/pico/pico-sdk
PICO_BUILD_DIR=build/pico
PICO_TARGET=build/pico/spc
LINUX_BUILD_DIR=build/linux
LINUX_TARGET=build/linux/spc

# Path to your source files
SRC_PATH := src

CC=gcc
CXX=g++
LINUX_LD=$(CC) # use c linker
PICO_CC=arm-none-eabi-gcc
PICO_CXX=arm-none-eabi-g++
CFLAGS=-DLINUX -I$(SRC_PATH) -O2 -g
CXXFLAGS=-DLINUX -I$(SRC_PATH) -O2 -g
LINUX_LDFLAGS=-lncurses -ltinfo

PICO_CFLAGS=-O2 -g -I$(SRC_PATH)
PICO_CXXFLAGS=-O2 -g
PICO_LDFLAGS=
PICO_INCLUDES=-I${PICO_SDK_PATH}/include
PICO_LIBS=-L${PICO_SDK_PATH}/lib

CSRCS=$(wildcard $(SRC_PATH)/*.c)
CPPSRCS=$(wildcard $(SRC_PATH)/*.cpp)
COBJS=$(addprefix ${LINUX_BUILD_DIR}/, $(CSRCS:.c=.o))
CPPOBJS=$(addprefix ${LINUX_BUILD_DIR}/, $(CPPSRCS:.cpp=.o))
PICO_COBJS=$(addprefix ${PICO_BUILD_DIR}/, $(CSRCS:.c=.o))
PICO_CPPOBJS=$(addprefix ${PICO_BUILD_DIR}/, $(CPPSRCS:.cpp=.o))

.DEFAULT_GOAL := linux

.PHONY: pico linux clean

pico: $(eval INCLUDES := $(PICO_INCLUDES))
pico: $(eval LIBS := $(PICO_LIBS))
linux: $(eval LDFLAGS := $(PICO_LDFLAGS))
pico: $(PICO_TARGET)

$(PICO_TARGET): ${PICO_COBJS} ${PICO_CPPOBJS}
	${PICO_CXX} ${LDFLAGS} -o $@ $^ ${LIBS}

linux: $(eval INCLUDES := $(LINUX_INCLUDES))
linux: $(eval LIBS := $(LINUX_LIBS))
linux: $(eval LDFLAGS := $(LINUX_LDFLAGS))
linux: $(LINUX_TARGET)

$(LINUX_TARGET): ${COBJS} ${CPPOBJS}
	${LINUX_LD} -o $@ $^ ${LIBS} ${LDFLAGS}

${COBJS}: ${LINUX_BUILD_DIR}/%.o : %.c
	@echo "Compiling $@"
	mkdir -p ${LINUX_BUILD_DIR}/$(SRC_PATH)
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

${CPPOBJS}: ${LINUX_BUILD_DIR}/%.o : %.cpp
	@echo "Compiling $@"
	mkdir -p ${LINUX_BUILD_DIR}/$(SRC_PATH)
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

${PICO_COBJS}: ${PICO_BUILD_DIR}/%.o : %.c
	@echo "Compiling $@"
	mkdir -p ${PICO_BUILD_DIR}/$(SRC_PATH)
	${PICO_CC} ${PICO_CFLAGS} ${INCLUDES} -c $< -o $@

${PICO_CPPOBJS}: ${PICO_BUILD_DIR}/%.o : %.cpp
	@echo "Compiling $@"
	mkdir -p ${PICO_BUILD_DIR}/$(SRC_PATH)
	${PICO_CXX} ${PICO_CXXFLAGS} ${INCLUDES} -c $< -o $@

clean:
	-@rm -rf ${PICO_BUILD_DIR} ${LINUX_BUILD_DIR} $(LINUX_TARGET) $(PICO_TARGET)

compile_flags_linux:
	echo ${CFLAGS} ${LINUX_INCLUDES} > compile_flags.txt

compile_flags_pico:
	echo ${PICO_CFLAGS} ${PICO_INCLUDES} > compile_flags.txt

# # Name of your output file
# OUT_FILE := spc

# # Path to your source files
# SRC_PATH := src

# # Path to the Pico SDK
# PICO_SDK_PATH := path/to/pico-sdk

# # Name of your Pico and Linux compilers
# PICO_CC := arm-none-eabi-gcc
# PICO_CPP := arm-none-eabi-g++
# LINUX_CC := gcc
# LINUX_CPP := g++
# LINUX_LDFLAGS=-lncurses

# # Preprocessor flags
# PICO_C_FLAGS := -DPICO -mcpu=cortex-m0plus -mthumb -O2 -I$(PICO_SDK_PATH)/include
# PICO_CPP_FLAGS := $(PICO_C_FLAGS)
# LINUX_C_FLAGS := -DLINUX -O2 -I$(SRC_PATH)
# LINUX_CPP_FLAGS := $(LINUX_C_FLAGS)

# # Source files
# C_FILES := $(wildcard $(SRC_PATH)/*.c)
# CPP_FILES := $(wildcard $(SRC_PATH)/*.cpp)

# .DEFAULT_GOAL := pico
# .PHONY: all clean pico pico_c pico_cpp compile_flags_pico linux linux_c linux_cpp compile_flags_linux

# all: pico

# pico: pico_c pico_cpp
# 	@mkdir -p build/pico
# 	@$(PICO_CC) build/pico/*.o -o build/pico/$(OUT_FILE).elf

# pico_c:
# 	@if [ "$(C_FILES)" ]; then echo "Compiling C files for Raspberry Pi Pico..."; mkdir -p build/pico; $(PICO_CC) -c $(PICO_C_FLAGS) $(C_FILES); mv *.o build/pico/; fi

# pico_cpp:
# 	@if [ "$(CPP_FILES)" ]; then echo "Compiling C++ files for Raspberry Pi Pico..."; mkdir -p build/pico; $(PICO_CPP) -c $(PICO_CPP_FLAGS) $(CPP_FILES); mv *.o build/pico/; fi

# compile_flags_pico:
# 	@echo $(PICO_C_FLAGS) > compile_flags.txt

# linux: linux_c linux_cpp
# 	@mkdir -p build/linux
# 	@$(LINUX_CC) ${LINUX_LDFLAGS} build/linux/*.o -o build/linux/$(OUT_FILE)

# linux_c:
# 	@if [ "$(C_FILES)" ]; then echo "Compiling C files for Linux..."; mkdir -p build/linux; $(LINUX_CC) -c $(LINUX_C_FLAGS) $(C_FILES); mv *.o build/linux/; fi

# linux_cpp:
# 	@if [ "$(CPP_FILES)" ]; then echo "Compiling C++ files for Linux..."; mkdir -p build/linux; $(LINUX_CPP) -c $(LINUX_CPP_FLAGS) $(CPP_FILES); mv *.o build/linux/; fi

# compile_flags_linux:
# 	@echo $(LINUX_C_FLAGS) > compile_flags.txt

# clean:
# 	-@rm -rf build/
