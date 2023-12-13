
cd..

for /r "resource\shader\" %%F in (*) do (  
    glslc "resource\shader\%%~nxF" -o "resource\spv\%%~nxF.spv"
)  

xcopy /s /i /y "resource\" "out\resource\"

pause