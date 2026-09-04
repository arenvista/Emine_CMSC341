#!/bin/bash 

# Compile the code
g++ bingo.cpp mytest.cpp -g

# Run valgrind 
valgrind -s --leak-check=full ./a.out
