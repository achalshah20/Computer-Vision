#!/bin/sh

g++ a2.cpp -g -o a2 -lX11 -lpng -ljpeg -lpthread -I. -Isiftpp -O3 siftpp/sift.cpp
./a2 part1 images/part1_images/bigben_14.jpg images/part1_images/*
./a2 part1 images/part1_images/colosseum_3.jpg images/part1_images/*
./a2 part1 images/part1_images/eiffel_6.jpg images/part1_images/*
./a2 part1 images/part1_images/empirestate_12.jpg images/part1_images/*
./a2 part1 images/part1_images/londoneye_9.jpg images/part1_images/*
./a2 part1 images/part1_images/louvre_9.jpg images/part1_images/*
./a2 part1 images/part1_images/notredame_8.jpg images/part1_images/*
./a2 part1 images/part1_images/sanmarco_5.jpg images/part1_images/*
./a2 part1 images/part1_images/tatemodern_6.jpg images/part1_images/*
./a2 part1 images/part1_images/trafalgarsquare_6.jpg images/part1_images/*
