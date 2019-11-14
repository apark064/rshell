#!/bin/bash

echo "Testing multiple commands: ls -a; echo hello && date || echo world; git status"

cd ..
./rshell << EOF > integration_tests/mult_test
ls -a; echo hello && date || echo world; git status; 
exit 
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi
