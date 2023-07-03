PICO_BUILD_DIR=build/pico
LINUX_BUILD_DIR=build/linux

.DEFAULT_GOAL := linux

.PHONY: pico linux host.linux host.pico compile_commands clean

pico:
	@mkdir -p ${PICO_BUILD_DIR}
	@docker build -t pico-builder-image -f pico.dockerfile .
	-@docker rm pico-builder-container
	@docker create --rm --name pico-builder-container pico-builder-image
	docker cp pico-builder-container:/project/src/build/spc.uf2 ${PICO_BUILD_DIR}/spc.uf2

host.pico:
	@mkdir -p ${PICO_BUILD_DIR}
	@cd ${PICO_BUILD_DIR} && RASPBERRY_PI_PICO=1 cmake ../.. && RASPBERRY_PI_PICO=1 cmake --build .
	@make --no-print-directory compile_commands

linux:
	@mkdir -p ${LINUX_BUILD_DIR}
	@docker build -t linux-builder-image -f linux.dockerfile .
	-@docker rm linux-builder-container
	@docker create --rm --name linux-builder-container linux-builder-image
	@docker cp linux-builder-container:/project/src/build/spc ${LINUX_BUILD_DIR}/spc
	@docker cp linux-builder-container:/project/src/build/compile_commands.json compile_commands.json

host.linux:
	@mkdir -p ${LINUX_BUILD_DIR}
	@cd ${LINUX_BUILD_DIR} && cmake ../.. && cmake --build . && ctest
	@make --no-print-directory compile_commands

clean:
	-@rm -rf ${PICO_BUILD_DIR} ${LINUX_BUILD_DIR} ${LINUX_TEST_DIR}
