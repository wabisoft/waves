@echo off


if /i [%1] == [clean] (
	rm -rf build
) else if /i [%1] == [cmake] (
	mkdir build
	pushd build\
	REM cmake .. -DSFML_DIR=D:\libs\SFML-2.5.1 -DCMAKE_BUILD_TYPE=Debug -DGLM_DIR=D:\code\g-truc\glm
	cmake .. -DSFML_DIR=D:\libs\SFML-2.5.1 -DCMAKE_BUILD_TYPE=Debug -DImGui-SFML_DIR=D:\libs\ImGui-SFML -DGLM_DIR=D:\code\g-truc\glm
	popd
) else if /i [%1] == [tags] (
	echo "updating tags"
	ctags -R src
) else if /i [%1] == [editor] (
	pushd build\
	call msbuild waves.sln /p:Configuration=Debug /t:editor
	if ERRORLEVEL 1 (
        @echo Build failed
		popd
        exit /b 1
    )
	popd
	cp -r assets build\Debug\
	call "build/Debug/editor.exe"
) else if /i [%1] == [game] (
	pushd build\
	call msbuild waves.sln /p:Configuration=Debug /t:game
	if ERRORLEVEL 1 (
        @echo Build failed
		popd
        exit /b 1
    )
	popd
	cp -r assets build\Debug\
	call "build/Debug/game.exe"
)

