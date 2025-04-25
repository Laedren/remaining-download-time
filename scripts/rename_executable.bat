@echo off

cd remaining_time

set output=Download Calculator.exe

if exist "%output%" (
	echo File already exists, replacing it
	del "%output%"
)

:: Compiling file source
g++ download_calculator.cpp -o "%output%"

