#!/bin/bash

clear

gcc -g telescopesteering.c -o soft.out

clear

./soft.out

rm ./soft.out

rm -dr ./soft.out.dSYM