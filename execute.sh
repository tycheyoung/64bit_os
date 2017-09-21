#!/bin/bash
make clean
make
sudo qemu-system-x86_64 -L . -m 64 -fda ./Disk.img -localtime -M pc

