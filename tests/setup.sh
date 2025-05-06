#!/bin/bash

mkdir -p test-graphics
for f in test-images/*; do 
    IMG=$(echo $f | grep -oiP '([^/]+)(?=\.jpg$)')
    mkdir -p test-graphics/$IMG
    echo "Created dir $IMG"
done