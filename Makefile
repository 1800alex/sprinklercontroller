PICO_BUILD_DIR=build/pico
PICO_TARGET=build/pico/spc
LINUX_BUILD_DIR=build/linux
LINUX_TARGET=build/linux/spc
HOST_TARGET=build/host/spc

.DEFAULT_GOAL := linux

.PHONY: pico linux host test compile_commands clean

pico:
	@mkdir -p ${PICO_BUILD_DIR}
	@docker build -t pico-builder-image -f pico.dockerfile .
	@docker create --name pico-builder-container pico-builder-image
	#docker cp pico-builder-container:/project/src/build/blink.uf2 ./blink.uf2

pico.raw:
	@mkdir -p ${PICO_BUILD_DIR}
	@cd ${PICO_BUILD_DIR} && PICO_SDK_FETCH_FROM_GIT=1 RASPBERRY_PI_PICO=1 cmake ../.. && PICO_SDK_FETCH_FROM_GIT=1 RASPBERRY_PI_PICO=1 cmake --build .
	@make --no-print-directory compile_commands

linux:
	@mkdir -p ${LINUX_BUILD_DIR}
	@docker build -t linux-builder-image -f linux.dockerfile .
	@docker create --name linux-builder-container linux-builder-image

host:
	@mkdir -p ${LINUX_BUILD_DIR}
	@cd ${LINUX_BUILD_DIR} && cmake ../.. && cmake --build . && ctest
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

pico_dev:
	@cd docker/dev/pico && docker build -t pico-builder-image .