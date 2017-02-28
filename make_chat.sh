#!/bin/bash

g++ -c testChat1.cpp
g++ -o testChat1 fifo.o testChat1.o

g++ -c chatServer1.cpp
g++ -o chatServer1 fifo.o chatServer1.o
