#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: ./checkstyle FILE"
fi

./checkpatch.pl --terse --file --no-tree $1
