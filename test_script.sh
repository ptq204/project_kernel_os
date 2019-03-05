#!/usr/bin/env bash

echo "Running make"
make clean
make all || exit $?
echo "=================================="
echo "List of KO file"
ls -l *.ko
echo "=================================="
echo "Init LKM"
sudo insmod rgenerator.ko
lsmod | grep rgenerator
echo "=================================="
#run test program here
sudo cat /dev/random_generator
echo "Exit LKM"
sudo rmmod rgenerator
echo "=================================="
echo "View log"
dmesg | tail -10
