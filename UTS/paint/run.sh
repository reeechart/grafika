#!/bin/bash
# My first script

clear
g++ Canvas.cpp Color.cpp Component.cpp Layer.cpp Line.cpp Plane.cpp Point.cpp Renderer.cpp Reader.cpp ClippingPlane.cpp main.cpp -o plane -std=c++11 -lpthread -lncurses
./plane

