#!/bin/bash

cd ..
./rshell <<< "ls # -lR"

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi
