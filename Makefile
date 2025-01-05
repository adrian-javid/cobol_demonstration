platform := native

BUILD_DIR := build/${platform}

CPLUSPLUS_SOURCE_LIST := $(wildcard source_code/*.cpp)
COBOL_SOURCE_LIST := $(wildcard source_code/*.cbl)

COBOL_TRANSPILED_C_SOURCE_LIST := $(patsubst source_code/%,${BUILD_DIR}/c_source_code/%.c,${COBOL_SOURCE_LIST})

CPLUSPLUS_BINARY_OBJECT_LIST := $(patsubst source_code/%,${BUILD_DIR}/binary_objects/%.o,${CPLUSPLUS_SOURCE_LIST})
COBOL_BINARY_OBJECT_LIST := $(patsubst ${BUILD_DIR}/c_source_code/%,${BUILD_DIR}/binary_objects/%.o,${COBOL_TRANSPILED_C_SOURCE_LIST})

AGGREGATE_OBJECT_LIST := ${CPLUSPLUS_BINARY_OBJECT_LIST} ${COBOL_BINARY_OBJECT_LIST}

RECIPE_DEPENDENCY_LIST := $(AGGREGATE_OBJECT_LIST:.o=.o.d)

COB_CONFIG := cob-config

COMMON_C_AND_CPLUSPLUS_COMPILE_FLAGS := -MMD -MP $(shell ${COB_CONFIG} --cflags)
CPLUSPLUS_COMPILE_FLAGS := #(empty string)
LINK_FLAGS := #(empty string)

TARGET_DIR := ${BUILD_DIR}/artifact
TARGET := ${TARGET_DIR}/app

COBOL_COMPILER := cobc

GENERATED_HEADERS := #(empty string)

ifeq (${platform},native)
TARGET := ${BUILD_DIR}/artifact/app
CPLUSPLUS_COMPILER := g++
C_COMPILER := gcc
LINK_FLAGS += $(shell ${COB_CONFIG} --libs)
else ifeq (${platform},webassembly)
TARGET_DIR := website/generated
TARGET := ${TARGET_DIR}/app.js ${TARGET_DIR}/app.wasm
CPLUSPLUS_COMPILER := em++
C_COMPILER := emcc
COMMON_C_AND_CPLUSPLUS_COMPILE_FLAGS += -Icobol_to_webassembly/build/webassembly/libraries/include -I${BUILD_DIR}/generated_headers
PATH_TO_STATIC_LIB_COB := cobol_to_webassembly/build/webassembly/libraries/lib/libcob.a
PATH_TO_STATIC_LIB_GMP := cobol_to_webassembly/build/webassembly/libraries/lib/libgmp.a
AGGREGATE_OBJECT_LIST += ${PATH_TO_STATIC_LIB_COB} ${PATH_TO_STATIC_LIB_GMP}
GENERATED_HEADERS += ${BUILD_DIR}/generated_headers/libcob.h
else
$(error Unsupported platform ${platform}.)
endif

# Compile binary object from COBOL and binary objects from C++ to an executable.
${TARGET} &: ${AGGREGATE_OBJECT_LIST} | ${TARGET_DIR}/
	${CPLUSPLUS_COMPILER} $^ ${LINK_FLAGS} -o $(word 1,${TARGET})

# Compile C++ source code to binary objects.
${CPLUSPLUS_BINARY_OBJECT_LIST}: ${BUILD_DIR}/binary_objects/%.o: source_code/% | ${BUILD_DIR}/binary_objects/ ${GENERATED_HEADERS}
	${CPLUSPLUS_COMPILER} ${COMMON_C_AND_CPLUSPLUS_COMPILE_FLAGS} ${CPLUSPLUS_COMPILE_FLAGS} -c $< -o $@

# Compile COBOL transpiled C source code to binary objects.
${COBOL_BINARY_OBJECT_LIST}: ${BUILD_DIR}/binary_objects/%.o: ${BUILD_DIR}/c_source_code/% | ${BUILD_DIR}/binary_objects/ ${GENERATED_HEADERS}
	${C_COMPILER} ${COMMON_C_AND_CPLUSPLUS_COMPILE_FLAGS} -c $< -o $@

# Transpile COBOL source code to C source code.
${COBOL_TRANSPILED_C_SOURCE_LIST}: ${BUILD_DIR}/c_source_code/%.c: source_code/% | ${BUILD_DIR}/c_source_code/
	${COBOL_COMPILER} -fixed -debug -verbose=2 -Wall -C $< -o $@

ifeq (${platform},webassembly)
${BUILD_DIR}/generated_headers/libcob.h: | ${BUILD_DIR}/generated_headers/
	cd cobol_to_webassembly && ${MAKE} put_libcob_header dir=$(abspath ${BUILD_DIR}/generated_headers)
endif

# Make a directory.
%/:
	@mkdir --parents --verbose $@

.PHONY: clean line_numbers show_debug_info

show_debug_info:
	@printf 'BUILD_DIR: %s\n' '${BUILD_DIR}'
	@printf 'CPLUSPLUS_SOURCE_LIST: %s\n' '${CPLUSPLUS_SOURCE_LIST}'
	@printf 'COBOL_SOURCE_LIST: %s\n' '${COBOL_SOURCE_LIST}'
	@printf 'COBOL_TRANSPILED_C_SOURCE_LIST: %s\n' '${COBOL_TRANSPILED_C_SOURCE_LIST}'
	@printf 'CPLUSPLUS_BINARY_OBJECT_LIST: %s\n' '${CPLUSPLUS_BINARY_OBJECT_LIST}'
	@printf 'COBOL_BINARY_OBJECT_LIST: %s\n' '${COBOL_BINARY_OBJECT_LIST}'
	@printf 'AGGREGATE_OBJECT_LIST: %s\n' '${AGGREGATE_OBJECT_LIST}'
	@printf 'RECIPE_DEPENDENCY_LIST: %s\n' '${RECIPE_DEPENDENCY_LIST}'
	@printf 'CPLUSPLUS_COMPILER: %s\n' '${CPLUSPLUS_COMPILER}'

line_numbers:
	awk -i inplace '{printf("%04d00%s\n", NR, substr($$0,7,120)) }' ${COBOL_SOURCE_LIST}

clean:
	@rm --recursive --force --verbose -- build/

-include ${RECIPE_DEPENDENCY_LIST}
