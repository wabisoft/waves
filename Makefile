clean:
	rm -rf build

debug:
	mkdir -p build
	cd build && cmake ../src -DCMAKE_BUILD_TYPE=Debug && make
	cp build/waves ./
	./waves

release:
	mkdir -p build
	cd build && cmake ../src -DCMAKE_BUILD_TYPE=Debug && make
	cp build/waves ./
	./waves

tags:
	@echo "updating tags"
	ctags -R src

commands:
	ln -s build/compile_commands.json ./compile_commands.json
