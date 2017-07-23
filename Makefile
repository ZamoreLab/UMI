all: do_cmake 

do_cmake: cpplib build_dir 
	cd build && cmake -DSEQAN_BUILD_SYSTEM=Release -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_PIPELINE=OFF -DCMAKE_INSTALL_PREFIX=..  .. && make && make test && make install

cpplib: CppLib/CMakeLists.txt
	git submodule update --init --recursive

build_dir: ./build 
	mkdir -p build

clean:
	rm -rf build 

.PHONY: clean all cmake build CppLib/CMakeLists.txt
