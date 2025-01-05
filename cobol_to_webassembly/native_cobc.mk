
# Here is how download the dependencies: `sudo apt-get install autopoint gettext`.
build/native/libraries/bin/cobc: build/download_cache/${LIB_COB_TARBALL} | build/native/external_source_code/
	tar xvf $< -C build/native/external_source_code
	cd build/native/external_source_code/${LIB_COB_NAME} && \
		autoreconf -f -i && \
		./configure --with-db=false --prefix=$(abspath build/native/libraries) && \
		${MAKE} && \
		${MAKE} install
