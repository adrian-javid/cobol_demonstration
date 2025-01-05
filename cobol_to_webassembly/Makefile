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

.PHONY: install delete_download_cache

INSTALL_DIR := #(empty string)

install: build/binary_objects/libcob.so
	@if [ -z '$(strip ${INSTALL_DIR})' ]; then \
		printf '`INSTALL_DIR` is improperly set as "${INSTALL_DIR}". Please specify a valid installation directory.\n' >&2; \
		false; \
	fi
	@install --verbose --compare $(word 1,$^) ${INSTALL_DIR}
	@install --verbose --compare --directory ${INSTALL_DIR}/include
	@install --verbose --compare build/external_source_code/${LIB_COB_NAME}/libcob.h ${INSTALL_DIR}/include
	@cp --verbose --recursive build/libraries/include ${INSTALL_DIR}

delete_download_cache:
	@rm --recursive --force --verbose -- build/download_cache
