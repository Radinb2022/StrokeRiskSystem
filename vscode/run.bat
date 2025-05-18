@echo off
echo Compiling Stroke Risk Prediction System...
javac -d . src/*.java
if errorlevel 1 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo Running application...
java src.Main
pause 