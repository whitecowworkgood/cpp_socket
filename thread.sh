#!/bin/bash

# ù ��° client ����
./client -i 127.0.0.1 -p 9999 -f b -d /root/socket/test/ &

# �� ��° client ����
./client -i 127.0.0.1 -p 9999 -f c -d /root/socket/test/ &
