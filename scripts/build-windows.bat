@ECHO OFF

SETLOCAL

PUSHD %~dp0

cmake --version > NUL || GOTO FAILED

CD ..

IF NOT EXIST build\NUL MKDIR build || GOTO FAILED
CD build

:: Remove cmake files that aren't required.
IF EXIST CMakeCache.txt DEL /S CMakeCache.txt > NUL
FOR /D /R . %%G IN (CMakeFiles) DO @IF EXIST "%%G" rd /s /q "%%G"
FOR /D /R . %%G IN (CMakeScripts) DO @IF EXIST "%%G" rd /s /q "%%G"
IF EXIST cmake_install.cmake DEL /S cmake_install.cmake > NUL

SET CMAKE_GENERATOR_NAME="Visual Studio 16 2019"
SET CMAKE_PLATFORM_NAME="x64"
SET CMAKE_TOOLSET_NAME="v142"

cmake -G %CMAKE_GENERATOR_NAME% -A %CMAKE_PLATFORM_NAME% -T %CMAKE_TOOLSET_NAME% .. || GOTO FAILED

cmake --build . --config Release --clean-first || GOTO FAILED
ctest -C Release --output-on-failure || GOTO FAILED

:SUCCEEDED
  POPD
  ECHO SUCCEEDED
  PAUSE
  ENDLOCAL
  EXIT /B 0

:FAILED
  POPD
  ECHO FAILED
  PAUSE
  ENDLOCAL
  EXIT /B 1
