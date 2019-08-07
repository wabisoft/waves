@echo off


if /i [%1] == [clean] (
	rm -rf build
) else if /i [%1] == [debug] (
	mkdir build
	pushd build\
	cmake ../src -DSFML_DIR=C:\Users\Public\Libraries\SFML-2.5.1
	msbuild waves.sln 
	popd
	cp -r assets build\Debug\
	call "build/Debug/waves.exe"
) else if /i [%1] == [release] (
	mkdir build
	pushd build\
	cmake ../src -DSFML_DIR=C:\Users\Public\Libraries\SFML-2.5.1
	msbuild waves.sln /p:Configuration=Release
	popd
	cp -r assets build\Release\
	call "build/Release/waves.exe"
) else if /i [%1] == [tags] (
	echo "updating tags"
	ctags -R src
)
