#!/bin/bash
cd /Users/florianepple/Documents/IT/Papa/TelescopeMount/Bilderschnitte/soft
clear
gcc -g main.c -o soft.o -lfreeimage
clear
./soft.o "./assets/images/IMG_1461.jpeg" 1425 1199
# ./soft.o "./assets/images/IMG_1461.jpeg" 0 0
# ./soft.o "./assets/images/test.jpg" 116 116