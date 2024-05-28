#!/bin/bash
set -e

echo "Running cmake..."
cmake .

echo "Running make..."
make

echo "Running eval.sh..."
./eval.sh
