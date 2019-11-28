#!/bin/bash

echo "Testing for symbolic test command execution: "

cd ..
./rshell << EOF > integration_tests/symbolic_test
[ -e test/file/path ] && echo "path exists"
exit
EOF

if [ $? -eq 0 ]
then
    echo "Test passed"
else
    echo "Test failed"
fi

echo "Output of commands located in symbolic_test"
