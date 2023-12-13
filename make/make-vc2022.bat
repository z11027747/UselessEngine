::@echo off

cd ..

if not exist "out" (
	mkdir "out"
) else (
	del /s /q "out\*.*"
)

cd out
cmake -G "Visual Studio 17 2022" ..
:: msbuild UselessEngine.sln

cd ..
copy "lib\glfw-vc2022\glfw3.dll" "out\"

pause