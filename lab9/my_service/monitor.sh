#!/bin/bash
cd /home/toxoplasma/my_service

while true;do
    /usr/bin/python3 monitor.py
    sleep 5
done
