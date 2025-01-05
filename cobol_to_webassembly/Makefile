ARTIFACT_DIR := artifact

LIB_GMP_VERSION := 6.3.0
LIB_GMP_NAME    := gmp-${LIB_GMP_VERSION}
LIB_GMP_TARBALL := ${LIB_GMP_NAME}.tar.xz

LIB_COB_VERSION := 3.0
LIB_COB_NAME    := gnucobol-${LIB_COB_VERSION}-rc1
LIB_COB_TARBALL := ${LIB_COB_NAME}.tar.xz

include $(wildcard *.mk)

%/:
	@mkdir --parents --verbose $@

.PHONY: put_libcob_header install delete_download_cache

dir := #(empty string)

CHECK_DIR := \
	@if [ -z '$(strip ${dir})' ]; then \
		printf '`dir` is improperly set as "${dir}". Please specify a valid directory.\n' >&2; \
		false; \
	fi

put_libcob_header: build/webassembly/libraries/lib/libcob.a
	$(call CHECK_DIR)
	@install --verbose --compare build/webassembly/external_source_code/${LIB_COB_NAME}/libcob.h ${dir}

install: build/binary_objects/libcob.so
	@if [ -z '$(strip ${dir})' ]; then \
		printf '`dir` is improperly set as "${dir}". Please specify a valid installation directory.\n' >&2; \
		false; \
	fi
	@install --verbose --compare $(word 1,$^) ${dir}
	@install --verbose --compare --directory ${dir}/include
	@install --verbose --compare build/external_source_code/${LIB_COB_NAME}/libcob.h ${dir}/include
	@cp --verbose --recursive build/libraries/include ${dir}

delete_download_cache:
	@rm --recursive --force --verbose -- build/download_cache
