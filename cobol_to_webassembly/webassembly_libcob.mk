build/webassembly/binary_objects/libcob.so: build/webassembly/libraries/lib/libcob.a build/webassembly/libraries/lib/libgmp.a | build/webassembly/binary_objects/
	cd build/webassembly/binary_objects && \
		ar -x $(abspath $(word 1,$^)) && \
		ar -x $(abspath $(word 2,$^)) && \
		${EMSDK}/upstream/bin/wasm-ld *.o -o libcob.so --relocatable --no-entry

# Here is how download the dependencies: `sudo apt-get install autopoint gettext`.
build/webassembly/libraries/lib/libcob.a: build/download_cache/${LIB_COB_TARBALL} build/webassembly/libraries/lib/libgmp.a | build/webassembly/external_source_code/
	tar xvf $< -C build/webassembly/external_source_code
	cd build/webassembly/external_source_code/${LIB_COB_NAME} && \
		sed -i '14680,14868d' configure && \
		sed -i '515,582d' configure.ac && \
		autoreconf -f -i && \
		emconfigure ./configure --with-db=false --disable-assembly --prefix=$(abspath build/webassembly/libraries) --includedir=$(abspath build/webassembly/libraries/include) && \
		${MAKE} defaults.h && \
		cd libcob && \
			emmake ${MAKE} INCLUDES=-I$(abspath build/webassembly/libraries/include) && \
			${MAKE} install
