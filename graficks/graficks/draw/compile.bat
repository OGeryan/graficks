SET mypath=%~dp0
echo %mypath:~0,-1%

: : YOU'RE GONNA HAVE TO FILL OUT THE SDL DIRECTORY INCLUDES (-I{your sdl libraries}\include\SDL2) AND (-L{again}\lib)
: : ALSO... YOU KNOW, YOU NEED TO INSTALL EVERYTHING YOU NEED!!!!!!!! (g++;sdl2)

g++ %mypath%/main.cpp -o %mypath%/GRAFICKS.exe -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
pause