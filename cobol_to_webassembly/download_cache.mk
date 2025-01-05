
build/download_cache/${LIB_GMP_TARBALL}: build/download_cache/${LIB_GMP_TARBALL}.sig | build/download_cache/
	curl --location --output $@ https://gmplib.org/download/gmp/${LIB_GMP_TARBALL}

build/download_cache/${LIB_GMP_TARBALL}.sig: | build/download_cache/
	curl --location --output $@ https://gmplib.org/download/gmp/${LIB_GMP_TARBALL}.sig

build/download_cache/${LIB_COB_TARBALL}: build/download_cache/${LIB_COB_TARBALL}.sig | build/download_cache/
	curl --location --output $@ https://sourceforge.net/projects/gnucobol/files/gnucobol/${LIB_COB_VERSION}/${LIB_COB_TARBALL}/download

build/download_cache/${LIB_COB_TARBALL}.sig: | build/download_cache/
	curl --location --output $@ https://sourceforge.net/projects/gnucobol/files/gnucobol/${LIB_COB_VERSION}/${LIB_COB_TARBALL}.sig/download

