# Template para proyectos C++/C con SDL2
para construir se utiliza el main.cpp
y el `Makefile`, para construir ir al root

```bash
make
```

si se quiciese incluir un `functions.cpp`:

```bash
all:
	g++ -Isrc/Include -Lsrc/lib -o main main.cpp functions.cpp -lmingw32 -lSDL2main -lSDL2

```