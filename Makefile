PICO_BUILD_DIR=build/pico
PICO_TARGET=build/pico/spc
LINUX_BUILD_DIR=build/linux
LINUX_TARGET=build/linux/spc
LINUX_TEST_DIR=build/test

.DEFAULT_GOAL := linux

.PHONY: pico linux test compile_commands clean

pico:
	@mkdir -p ${PICO_BUILD_DIR}
	@cd ${PICO_BUILD_DIR} && RASPBERRY_PI_PICO=1 cmake ../.. && RASPBERRY_PI_PICO=1 cmake --build .
	@make --no-print-directory compile_commands

linux:
	@mkdir -p ${LINUX_BUILD_DIR}
	@cd ${LINUX_BUILD_DIR} && cmake ../.. && cmake --build .
	@make --no-print-directory compile_commands

test: linux
	@mkdir -p ${LINUX_BUILD_DIR}
	@cd ${LINUX_BUILD_DIR} && ctest

test2:
	@mkdir -p ${LINUX_TEST_DIR}
	@cd ${LINUX_TEST_DIR} && cmake ../../test && cmake --build . && ctest

clean:
	-@rm -rf ${PICO_BUILD_DIR} ${LINUX_BUILD_DIR} ${LINUX_TEST_DIR} $(LINUX_TARGET) $(PICO_TARGET)

compile_commands:
	@cp -f ${LINUX_BUILD_DIR}/compile_commands.json ./
