# Particula
N-body Simulator

1. To make Particula, it requires:

    1. wxWidgets 3.0.x
    2. wxMathPlot source
    3. POSIX thread (pthread) library
    4. Eigen3 Matrix Library
    5. OpenGL and GLUT library

2. How to build.
You can find CMakeFiles.txt in Projects directory. This is a file for 
cmake build system.

 > copy wxmathplot source directory in the same level of Particula
 > cd Projects
 > mkdir BUILD
 > cd BUILD
 > cmake .. 
 > make -f Makefile

