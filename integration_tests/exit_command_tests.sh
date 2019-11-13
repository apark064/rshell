#!/bin/bash

cd ..
./rshell <<< "exit"

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi
