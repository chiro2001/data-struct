@echo off

.\cmake-build-debug\main.exe > myres.txt

start windiff result.txt myres.txt
