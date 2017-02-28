#!/bin/bash

g++ -c testclient.cpp
g++ -o testclient fifo.o testclient.o

g++ -c textserver.cpp
g++ -o textserver fifo.o textserver.o
