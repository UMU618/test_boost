@echo off

if "%BOOST_ROOT%"=="" (
  echo "Please set environment variable BOOST_ROOT to the location of Boost."
  goto EXIT
)

if not exist "%BOOST_ROOT%\b2.exe" (
  echo "Please compile Boost first."
  goto EXIT
)

pushd "%BOOST_ROOT%"
.\b2.exe "%~dp0"
popd

:EXIT
pause
