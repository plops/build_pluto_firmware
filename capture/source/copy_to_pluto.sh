#!/bin/bash

# https://forum.openwrt.org/t/ash-usr-libexec-sftp-server-not-found-when-using-scp/125772/2
# use -O to enforce legacy behaviour in the ssh client
sshpass -v -panalog scp \
    -o StrictHostKeyChecking=no \
    -o UserKnownHostsFile=/dev/null \
    -o CheckHostIP=no \
    -O \
    vis \
    root@192.168.2.1:/dev/shm
