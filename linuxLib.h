#pragma once

//Header for Linux Libs
#include <sys/socket.h> // für socket, bind, connect, listen, accept, send, recv
#include <arpa/inet.h>  // für inet_pton
#include <unistd.h>     // für close

//run with g++ -std=c++20 ./main-win.cpp ./connect.cpp ./listen.cpp ./DataStorage.cpp ./Socket.cpp -o tmp -pthread && ./tmp