g++ -c -I./header -I../opengl ./source/*.cpp ./source/*.c
g++ -o libmodule.so -shared *.o -l../opengl/glew32.dll
pause