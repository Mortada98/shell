#!/bin/bash

# Test script for double free issue
echo "Testing double free issue..."

cd /home/mbouizak/Documents/shell

# Create test commands
cat > test_input.txt << 'EOF'
mkdir test_folder
cd test_folder
rm -rf ../test_folder
unset PWD
pwd
exit
EOF

echo "Running minishell with problematic sequence..."
./minishell < test_input.txt

echo "Exit code: $?"
