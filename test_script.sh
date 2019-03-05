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
#./test_program
echo "=================================="
echo "View log"
dmesg | tail -10
