#!/bin/bash

sshpass -v -panalog ssh \
    -oStrictHostKeyChecking=no \
    -oUserKnownHostsFile=/dev/null \
    -oCheckHostIP=no \
    -l root \
    192.168.2.1
