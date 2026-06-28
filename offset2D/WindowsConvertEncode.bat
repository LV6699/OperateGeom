@echo off
echo utf8 to ansi
chcp 65001 >nul
for /r %%i in (*.cpp *.h) do (
 echo Converting file: %%i
    powershell -Command "(Get-Content -Encoding UTF8 '%%i') | Set-Content -Encoding UTF8 '%%i.tmp'"
    move /y "%%i.tmp" "%%i" >nul
)