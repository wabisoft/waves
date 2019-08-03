@echo off


if /i [%1] == [clean] (
	rm -rf build
) else if /i [%1] == [build] (
	mkdir build
	
	pushd build\
	
	cmake ../src -DSFML_DIR=C:\Users\Public\Libraries\SFML-2.5.1 
	
	msbuild waves.sln
	
	popd
) else if /i [%1] == [run] (
	call "build/Debug/waves.exe"
)

