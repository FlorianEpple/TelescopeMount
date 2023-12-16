#!/bin/bash
cd /Users/florianepple/Documents/IT/Papa/TelescopeMount/Bilderschnitte/soft
clear
gcc -g main.c -o soft.o -lfreeimage
clear
./soft.o "./assets/IMG_1461.jpeg" 1211 1237