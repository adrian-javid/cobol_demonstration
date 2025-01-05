
build/webassembly/libraries/lib/libgmp.a: build/download_cache/${LIB_GMP_TARBALL} | build/webassembly/external_source_code/
	tar xvf $< -C build/webassembly/external_source_code
	cd build/webassembly/external_source_code/${LIB_GMP_NAME} && \
		emconfigure ./configure --disable-assembly --host none --prefix=$(abspath build/webassembly/libraries) && \
		${MAKE} && \
		${MAKE} install
