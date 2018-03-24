@echo off
rd /s /q ..\depends
md ..\depends
git clone https://github.com/gabime/spdlog.git ..\depends\spdlog\
