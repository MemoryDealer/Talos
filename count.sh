#!/bin/bash

# Counts number of lines in entire source.

find . -name "*.hpp" -o -name "*.cpp" | xargs wc -l
echo "(-4854 from SkyX)"
