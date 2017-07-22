all: do_cmake 

do_cmake: cpplib build_dir 
	cd build && cmake -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_TESTS=ON -DBUILD_PIPELINE=ON -DCMAKE_INSTALL_PREFIX=..  .. && make && make test && make install

cpplib: CppLib/CMakeLists.txt
	git submodule update --init --recursive

build_dir: ./build 
	mkdir -p build

clean:
	rm -rf build 

.PHONY: clean all cmake build CppLib/CMakeLists.txt