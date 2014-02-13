g++ -c -I./header -I../opengl ./source/*.cpp ./source/*.c
g++ -o libmodule.so -shared *.o -L../opengl/ -lglew32 -lopengl32 -lglut32 -lglut -lglu32 -lws2_32
pause