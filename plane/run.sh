#!/bin/bash
# My first script

clear
g++ Canvas.cpp Color.cpp Component.cpp Line.cpp Plane.cpp Point.cpp Renderer.cpp -o plane -std=c++11
./plane
