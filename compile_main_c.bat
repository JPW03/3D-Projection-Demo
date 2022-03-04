echo @off
gcc main.c polygons.c matrices.c ui.c -o ProjectionDemo.exe -O1 -Wall -Wno-missing-braces -std=c99 -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -Wl,--subsystem,windows
pause