#!/bin/bash

# Fail on any error.
set -e

# Move to root of project.
cd "$(cd -P "$(dirname "$0")"/.. && pwd)"

export PATH="/usr/lib/llvm-13/bin${PATH:+:${PATH}}"
export LD_LIBRARY_PATH="/usr/lib/llvm-13/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"

find lib include tests -name '*.c' -exec clang-format -i {} \;
find lib include tests -name '*.cpp' -exec clang-format -i {} \;
find lib include tests -name '*.h' -exec clang-format -i {} \;
find lib include tests -name '*.hpp' -exec clang-format -i {} \;
