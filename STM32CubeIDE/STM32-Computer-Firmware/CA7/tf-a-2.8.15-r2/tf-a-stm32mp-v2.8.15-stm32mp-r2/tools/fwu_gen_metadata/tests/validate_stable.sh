#!/bin/bash

set -e
rm -f dummy*
../fwumd_tool.py dummy
../fwumd_tool.py binparse dummy.bin -j dummy2.json
../fwumd_tool.py jsonparse dummy2.json -b dummy2.bin
shasum dummy*.bin
