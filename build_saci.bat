@echo off
:: Abre o x64 Native Tools Command Prompt para Visual Studio

:: Caminho do vcvars64.bat (ajuste se seu Visual Studio estiver em outro local)
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Navega até o projeto
cd /d C:\Users\Gabriel\Downloads\Saci-Internal

:: Executa o build em modo Release
msbuild SaciCheat.vcxproj /p:Configuration=Release

pause
