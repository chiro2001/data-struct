@echo off

cd ..

.\cmake-build-debug\main.exe > .\myres.txt

start .\tools\windiff .\result.txt .\myres.txt
