#!/bin/sh

if [ ! -f /dev/xvdma ]
then
  mknod /dev/xvdma c 10 224
fi

ln -sf /mnt/libdrm/* /usr/lib/
