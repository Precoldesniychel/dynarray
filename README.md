cd C:\Labs\variant17
if (Test-Path build) { Remove-Item -Recurse -Force build }
New-Item -ItemType Directory -Path build | Out-Null
cd build

cmake -S .. -B .
cmake --build . --config Release

.\Release\v17.exe
.\Release\run_tests.exe


cd C:\Labs\variant17\build
cmake --build . --config Release --target run_tests
.\tests\Release\run_tests.exe

cd C:\Labs\variant17\build
cmake --build . --config Release

# Запуск оболочки
.\Release\v17.exe

# Запуск Google Test с подробным выводом
.\tests\Release\run_tests.exe --gtest_print_time=0