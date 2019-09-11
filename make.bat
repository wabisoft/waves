@echo off


if /i [%1] == [clean] (
	rm -rf build
::)
:: else if /i [%1] == [debug] (
:: 	mkdir build
:: 	pushd build\
:: 	cmake ../src -DSFML_DIR=C:\Users\Public\Libraries\SFML-2.5.1 -DCMAKE_BUILD_TYPE=Debug
:: 	call msbuild waves.sln
:: 	popd
:: 	cp -r assets build\Debug\
:: ) else if /i [%1] == [release] (
:: 	mkdir build
:: 	pushd build\
:: 	cmake ../src -DSFML_DIR=C:\Users\Public\Libraries\SFML-2.5.1 -DCMAKE_BUILD_TYPE=Release
::
:: 	msbuild waves.sln /p:Configuration=Release
:: 	popd
:: 	cp -r assets build\Release\
::
:: 	call "build/Release/waves-game.exe"
) else if /i [%1] == [cmake] (
	mkdir build
	pushd build\
	cmake ../src -DSFML_DIR=C:\Users\Public\Libraries\SFML-2.5.1 -DCMAKE_BUILD_TYPE=Debug
	popd
) else if /i [%1] == [tags] (
	echo "updating tags"
	ctags -R src
) else if /i [%1] == [editor] (
	pushd build\
	call msbuild waves.sln /p:Configuration=Debug /t:editor
	popd
	cp -r assets build\Debug\
	call "build/Debug/editor.exe"
) else if /i [%1] == [game] (
	pushd build\
	call msbuild waves.sln /p:Configuration=Debug /t:game
	popd
	cp -r assets build\Debug\
	call "build/Debug/game.exe"
)

