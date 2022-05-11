cmake_minimum_required(VERSION 3.21)
project(OPPLab3 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER "/bin/mpicc.mpich")

add_executable(OPPLab3 main.c matrix.c lattice.c schedule.c)