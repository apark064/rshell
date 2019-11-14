#!/bin/bash

echo "Testing single command: date"

cd ..
./rshell << EOF > integration_tests/single_test
date
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi
