
cd..

del /s /q "resource\spv\*.*"

@echo off
for /r "resource\shader\" %%F in (*) do (  
    if /i "%%~xF"==".glsl" (
        echo Ignore .glsl file: "%%~nxF"  
    ) else (
        echo Compiling shader: "%%~nxF" to spv format ... 
        glslc "resource\shader\%%~nxF" -o "resource\spv\%%~nxF.spv"
    )
)  

::xcopy /s /i /y "resource\" "out\resource\"

::pause