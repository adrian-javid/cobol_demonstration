ARTIFACT_DIR := artifact

LIB_GMP_VERSION := 6.3.0
LIB_GMP_NAME    := gmp-${LIB_GMP_VERSION}
LIB_GMP_TARBALL := ${LIB_GMP_NAME}.tar.xz

LIB_COB_VERSION := 3.0
LIB_COB_NAME    := gnucobol-${LIB_COB_VERSION}-rc1
LIB_COB_TARBALL := ${LIB_COB_NAME}.tar.xz

build/binary_objects/libcob.so: build/libraries/lib/libcob.a build/libraries/lib/libgmp.a | build/binary_objects/
	cd build/binary_objects && \
		ar -x $(abspath $(word 1,$^)) && \
		ar -x $(abspath $(word 2,$^)) && \
		${EMSDK}/upstream/bin/wasm-ld *.o -o libcob.so --relocatable --no-entry

build/tools/cobc: build/libraries/lib/libcob.a | build/external_source_code/
	cd build/external_source_code/${LIB_COB_NAME} && \
		${MAKE} INCLUDES=-I$(abspath build/libraries/include) && \
		emmake ${MAKE} install

# Here is how download the dependencies: `sudo apt-get install autopoint gettext`.
build/libraries/lib/libcob.a: build/download_cache/${LIB_COB_TARBALL} build/libraries/lib/libgmp.a | build/external_source_code/
	tar xvf $< -C build/external_source_code
	cd build/external_source_code/${LIB_COB_NAME} && \
		autoreconf -f -i && \
		./configure --with-db=false --disable-assembly --prefix=$(abspath build/libraries) --includedir=$(abspath build/libraries/include) && \
		${MAKE} defaults.h && \
		cd libcob && \
			${MAKE} INCLUDES=-I$(abspath build/libraries/include) && \
			emmake ${MAKE} install

build/download_cache/${LIB_COB_TARBALL}: build/download_cache/${LIB_COB_TARBALL}.sig | build/download_cache/
	curl --location --output $@ https://sourceforge.net/projects/gnucobol/files/gnucobol/${LIB_COB_VERSION}/${LIB_COB_TARBALL}/download

build/download_cache/${LIB_COB_TARBALL}.sig: | build/download_cache/
	curl --location --output $@ https://sourceforge.net/projects/gnucobol/files/gnucobol/${LIB_COB_VERSION}/${LIB_COB_TARBALL}.sig/download

build/libraries/lib/libgmp.a: build/download_cache/${LIB_GMP_TARBALL} | build/external_source_code/
	tar xvf $< -C build/external_source_code
	cd build/external_source_code/${LIB_GMP_NAME} && \
		emconfigure ./configure --disable-assembly --host none --prefix=$(abspath build/libraries) && \
		${MAKE} && \
		${MAKE} install

build/download_cache/${LIB_GMP_TARBALL}: build/download_cache/${LIB_GMP_TARBALL}.sig | build/download_cache/
	curl --location --output $@ https://gmplib.org/download/gmp/${LIB_GMP_TARBALL}

build/download_cache/${LIB_GMP_TARBALL}.sig: | build/download_cache/
	curl --location --output $@ https://gmplib.org/download/gmp/${LIB_GMP_TARBALL}.sig

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
