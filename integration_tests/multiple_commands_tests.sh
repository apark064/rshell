#!/bin/bash

cd ..
./rshell <<< "ls -a; echo hello && mkdir test || echo world; git status"

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi
