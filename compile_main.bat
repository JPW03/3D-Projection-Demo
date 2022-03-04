echo @off
g++ main.c polygons.c matrices.c ui.c -o projection.exe -O1 -Wall -Wno-missing-braces -std=c++17 -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -Wl,--subsystem,windows
pause