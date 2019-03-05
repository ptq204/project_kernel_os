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
echo "List in proc"
sudo cat /proc/devices | grep random_generator
echo "List in dev"
ls /dev/ | grep random_generator
echo "=================================="
#run test program here
echo "Reading from device file"
sudo cat /dev/random_generator && echo ""
echo "=================================="
echo "Exit LKM"
sudo rmmod rgenerator
echo "=================================="
echo "View log"
dmesg | tail -10
