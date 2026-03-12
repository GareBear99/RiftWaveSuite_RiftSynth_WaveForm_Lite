@echo off
set ROOT=%~dp0\..
if "%JUCE_DIR%"=="" set JUCE_DIR=C:\SDKs\JUCE
if "%BUILD_DIR%"=="" set BUILD_DIR=%ROOT%\build-win
if not exist "%JUCE_DIR%" (
  echo Missing JUCE_DIR at %JUCE_DIR%
  exit /b 1
)
cmake -S "%ROOT%" -B "%BUILD_DIR%" -DJUCE_DIR="%JUCE_DIR%" -DCMAKE_BUILD_TYPE=Release
cmake --build "%BUILD_DIR%" --config Release
