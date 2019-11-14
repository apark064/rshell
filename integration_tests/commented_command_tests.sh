#!/bin/bash

echo "Testing commented command: ls # -lR"

cd ..
./rshell << EOF > integration_tests/commented_test
"ls # -lR"
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi
