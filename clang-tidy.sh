#!/bin/bash

set -e
FILE="build/x64_Debug/compile_commands.json"

if [[ ! -f $FILE ]]; then
    RED='\033[0;31m'
    NC='\033[0m' # No Color

    echo -e "\n${RED}==================================================================${NC}"
    echo -e "${RED} ERROR: The file ./build/x64_Debug/compile_commands.json does not exist!${NC}"
    echo -e "${RED}==================================================================${NC}\n"

    echo "Please run the following command to create it:"
    echo -e "\n  ${RED}./build.sh${NC}\n"

    echo -e "${RED}==================================================================${NC}"
    exit -1
fi

list="include src"  # 只保留 include 和 src
run-clang-tidy -header-filter='/home/lanm/ws/clion/ivshm-v2/.*' -p build/x64_Debug -j $(($(nproc) / 2 + 1)) $list

# run-clang-tidy -j $(($(nproc)/2+1)) -p build/clang -clang-tidy-binary=clang-tidy-10 -header-filter=framework,samples,vulkan_samples -checks=-*,google-*,-google-runtime-references -quiet framework/* samples/* vulkan_samples/* tests/*
