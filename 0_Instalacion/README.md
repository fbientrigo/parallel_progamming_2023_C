# Instalación
Utilizo Windows 10, por tanto posee una serie de pasos extras para poder correr limpiamente compiladores de C y otras herramientas, se utiliza:

- MYSYS2 para actualizar repositorios en una linux way
- Mingw-w64: entorno de desarrolo de software que incluye un puerto de los compiladores GCC

____

# 1. (MYSYS2)[https://www.msys2.org/]
Es util ya que incluye el packet manager `pacman` de Arc Linux (más información)[https://wiki.archlinux.org/title/pacman]

para instalarlo ir al (link mysys2)[https://www.msys2.org/]
y seguir la serie de pasos indicada, aqui resumidos:

- 1. descargar el instalador `msys2-x86_64-20230318.exe`
- 3. Correr el instalador para un directorio simple en el root del sistema, como `C:\mysys64`
- 5. tendras en el inicio un launcher para la consola, yo utilizo `MYSYS2 MYSYS`

- 6. aqui dentro procede a instalar `mingw-w64` con GCC para poder compilar codigo, se vera algo así:

```bash
$ pacman -S mingw-w64-ucrt-x86_64-gcc
resolving dependencies...
looking for conflicting packages...

Packages (15) mingw-w64-ucrt-x86_64-binutils-2.39-2
            mingw-w64-ucrt-x86_64-crt-git-10.0.0.r68.g6eb571448-1
            mingw-w64-ucrt-x86_64-gcc-libs-12.2.0-1  mingw-w64-ucrt-x86_64-gmp-6.2.1-3
            mingw-w64-ucrt-x86_64-headers-git-10.0.0.r68.g6eb571448-1
            mingw-w64-ucrt-x86_64-isl-0.25-1  mingw-w64-ucrt-x86_64-libiconv-1.17-1
            mingw-w64-ucrt-x86_64-libwinpthread-git-10.0.0.r68.g6eb571448-1
            mingw-w64-ucrt-x86_64-mpc-1.2.1-1  mingw-w64-ucrt-x86_64-mpfr-4.1.0.p13-1
            mingw-w64-ucrt-x86_64-windows-default-manifest-6.4-4
            mingw-w64-ucrt-x86_64-winpthreads-git-10.0.0.r68.g6eb571448-1
            mingw-w64-ucrt-x86_64-zlib-1.2.12-1  mingw-w64-ucrt-x86_64-zstd-1.5.2-2
            mingw-w64-ucrt-x86_64-gcc-12.2.0-1

Total Installed Size:  397.59 MiB

:: Proceed with installation? [Y/n]
[... downloading and installation continues ...]
```

- 7. ahora comprueba la versión

```bash
$ gcc --version
gcc.exe (Rev1, Built by MSYS2 project) 12.2.0
```

- 8. para hacer update de los paquetes, se usa pacman:

```bash
$ pacman -Suy
:: Synchronizing package databases...
 mingw32 is up to date
 mingw64 is up to date
 ucrt64 is up to date
 clang32 is up to date
 clang64 is up to date
 msys is up to date
:: Starting core system upgrade...
 there is nothing to do
:: Starting full system upgrade...
 there is nothing to do
```

ahora para poder correr el progama fuera de esta terminal ver la siguiente sección

___

# 2. Agregando a PATH
Puedes escribir `environment variables` en el inicio de windows, o su versión en español, alli te llevara a `System Properties` y podras acceder a editar estas variables de entorno,

dirigete al PATH y agrega en donde deberia de estar instalado `mingw64`, especificamente los binarios, que es donde se encontraran los ejecutables de `gcc` y `g++`

en mi caso mi PATH incluye:

`C:\msys64\mingw64\bin`

A este punto abre el terminal que desees y prueba correr el fichero en la clase 1

```bash
g++ c1_1_limite_y_output.cpp -o 1_limites.exe
```

de esa menra sabras si ocurrio un link correcto con las bibliotecas estandar

___

# Links de Interes

- (Uso moderno de CMake como extensión en VSCode)[https://dgvergel.blogspot.com/2019/08/integrando-vscode-mingw-w64-y-cmake_27.html]
- (Bibliotecas de 3ros)[https://dgvergel.blogspot.com/2019/08/integrando-vscode-mingw-w64-y-cmake_15.html]
- (Depuración dentro de VSCode)[https://dgvergel.blogspot.com/2019/08/integrando-vscode-mingw-w64-y-cmake_29.html]

___

# 3. Uso de Powershell

Dentro de la Powershell de windows, utiliza el comando:

```
$PROFILE
```

esto te permitira ver la variable de entorno con ese nombre, dirigete a esa ubicación y si reemplazas `profile.ps1` tendras una powershell que tenga una mejor forma de ver las carpetas y que se conecte a `conda`, si no necesitas `conda` puedes borrar las primeras lineas.

Puedes agregarle otras configuraciones, es alta mente personalizable.

