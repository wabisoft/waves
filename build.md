# Building waves.

Waves is built with CMake. In the `src/` directory there is a file called
`CMakeLists.txt`. This file is like a script that tells the tool CMake how to
build the files needed to build waves. The thing to remember here is that CMake is not
the tool that builds our code. It is a tool to configure the tool we actually use to build our code.

### Note on compilers
The compiler is what actually transforms our code into machine code. It reads the code with the expectation
that the code follows the rules of the language it implements (presumably c++ in our case).
There are various different C and C++ compilers. each one implements "standard" c++ at a minimum and
various compilers also support different language features not descibed in the language standard.
On windows you can use `cl(msvc)` which is microsoft's c/c++ compiler  
or  
`clang-cl` which is clang/llvm(an open source c/c++ compiler) for windows.

On mac you should use `clang` or `gcc`.

Back to CMake.
What cmake does is generates files and configuation for whichever tool we choose to use. (It does not compiler code)
Use `cmake --help` to see the different generators you can use.

Cmake will only do what we tell it to do. If you look at `src/CMakeLists.txt` you can kind of get a feel
for what we are telling CMake to do, (CMake is a big boi and there is a lot I don't know about it so don't
feel intimidated by all the random key words and magic in the the CMakeLists.txt, I still look everything up)
You should think of this file as a sort of script that tells CMake what the dependencies of our project are
which files in our source should be used to compile the executable we want to make and which external libarires
we want to "link" against. For our purposes.

### Note on GNUMake
Make is a tool that predates cmake. It is a program that reads a special file named `Makefile` and executes
commands and programs specified in the `Makefile`. It is a monsterous and archaic tool that take a while to
understand and become proficient with. I have grown very partial to it and So I like to make all my build commands
runnable from a top level make command.
You will see in the top level directory there is a `Makefile` this is that top level file. It is for use on `UNIX`
systems like mac and linux only and will not work on Windows. (this is because I can't figure out how to get
make to run on windows). To solve this I've made a windows batch command that I've named make you can find it in
`make.bat` this command takes 1 argument and executeds a set of commands depending on that.
For our purposes we just want to use visual studio most of the time so we only need to run one command from this
batch script.

Before we run our command we need to install SFML.  
https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip  
Use that link to download the sfml redist package.

If you unzip the archive you'll see a fold called `SFML-2.5.1` in there. Copy that to whereever you want
SFML to live on your system (remember this is just the redistributable library and is not code we're ever
going to edit). I like to keep all my libraries in a common parent on my D: drive which is larger than my C: drive.
I use `D:\libs` so my sfml installation is at `D:\libs\SFML-2.5.1`


Once you have SFML you can build the project using CMake. But before you do that you'll need to edit a line in the
`make.bat` file. In `make.bat` there is a line that called cmake with various arguments, one of those arguments
sets a variable for cmake to know where sfml is so that when we ask cmake to find sfml in our `CMakeLists.txt`
it can find it(it doesn't need special help on UNIX systems because libraries are usually in a predicable place
on those systems. Whereas they can be just about anywhere on windows).

You need to tell cmake where you put SFML when you copied it somewhere. You'll see that the file has my location
in there currently(`D:\libs\SFML-2.5.1`) you should change the line that says:
```
	cmake ../src -DSFML_DIR=D:\libs\SFML-2.5.1 -DCMAKE_BUILD_TYPE=Debug
```
to use the corrent path for your system.

Once you've done that you can run the batch script with the command
```
$ make cmake
```

This should produce output like this

```
-- Building for: Visual Studio 16 2019
-- The C compiler identification is MSVC 19.22.27905.0
-- The CXX compiler identification is MSVC 19.22.27905.0
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/VC/Tools/MSVC/14.22.27905/bin/Hostx64/x64/cl.exe
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/VC/Tools/MSVC/14.22.27905/bin/Hostx64/x64/cl.exe -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/VC/Tools/MSVC/14.22.27905/bin/Hostx64/x64/cl.exe
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/VC/Tools/MSVC/14.22.27905/bin/Hostx64/x64/cl.exe -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found SFML 2.5.1 in D:/libs/SFML-2.5.1/lib/cmake/SFML
-- Configuring done
-- Generating done
CMake Warning:
  Manually-specified variables were not used by the project:

    CMAKE_BUILD_TYPE


-- Build files have been written to: D:/code/wabisoft/waves/build
```

what this does is makes a directory called `build` and then runs cmake from inside that directory.
This produces the `waves.sln` file which is what we will open in visual studio.
If you open that `waves.sln` in visual studio you should be able to now build and run the project from within
visual studio. 

# NOTE:
if the game builds and runs but something weird happens. check that it is not a known issue
[here](https://github.com/wabisoft/waves/issues) before you try to fix it. Then decide if you really need
to fix it to do what you want to do today.

REMEMBER: the .sln is a product of the cmake process, so if you make changes to the solution(.sln) and
rerun cmake you'll lose your sln local changes.

If you get stuck call me.
