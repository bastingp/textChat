#!/bin/bash

g++ -c testChat.cpp
g++ -o testChat fifo.o testChat.o

g++ -c chatServer.cpp
g++ -o chatServer fifo.o chatServer.o
