#!/bin/zsh

for i in {1..20}; do
    echo "Running make run - Attempt $i"

    make run
    result=$?

    if [[ $result -ne 0 ]]; then
        echo "FAILED: make run stopped at attempt $i"
        exit 1
    fi
done

echo "SUCCESS: All 20 attempts of make run passed!"

