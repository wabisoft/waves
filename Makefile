.PHONY: cmake clean debug release tags commands

cmake:
	mkdir -p build
	cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug

clean:
	rm -rf build

game: cmake
	cd build && make game
	./build/game

editor: cmake
	cd build && make editor
	./build/editor

tags:
	@echo "updating tags"
	ctags -R src

commands:
	ln -s build/compile_commands.json ./compile_commands.json
