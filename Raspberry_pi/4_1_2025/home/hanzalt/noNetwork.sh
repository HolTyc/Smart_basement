#!/bin/bash
ping -c 4 192.168.0.1
let a=$?
if [ "$a" = "0" ]; then
  echo "We have connection."
else
  echo "We have lost connection.."
  shutdown -r now
fi
