.PHONY: cmake clean debug release tags commands

cmake-debug:
	mkdir -p build
	cd build && cmake ../src -DCMAKE_BUILD_TYPE=Debug

cmake-release:
	mkdir -p build
	cd build && cmake ../src -DCMAKE_BUILD_TYPE=Release

clean:
	rm -rf build

game: cmake-debug
	cd build && make game
	cp build/game ./
	./game

game-release: cmake-release
	cd build && make game
	cp build/game ./
	./game


release: cmake-release
	cd build && make
	cp build/waves ./
	./waves

tags:
	@echo "updating tags"
	ctags -R src

commands:
	ln -s build/compile_commands.json ./compile_commands.json
