#!/bin/bash

# 첫 번째 client 실행
./client -i 127.0.0.1 -p 9999 -f b -d /root/socket/test/ &

# 두 번째 client 실행
./client -i 127.0.0.1 -p 9999 -f c -d /root/socket/test/ &
