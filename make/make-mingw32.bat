::@echo off

cd ..

if not exist "out" (
	mkdir "out"
) else (
	del /s /q "out*.*"
)

cd out
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
mingw32-make

cd ..
copy "lib\glfw\glfw3.dll" "out\"

pause