#!/bin/bash

VERBOSE=0
SEPARATOR="\033[94m-------------------------------------------------------------------------\033[0m"
if [ $VERBOSE -eq 1 ]; then
	set -eux
	OPTS=-v
else
	set -e
	OPTS=
fi

rm -f dummy.bin dummy.json a.bin a.json

../fwumd_tool.py dummy $OPTS
echo -e "$SEPARATOR"
../fwumd_tool.py jsonparse $OPTS dummy.json -b a.bin
echo -e "$SEPARATOR"
../fwumd_tool.py binparse $OPTS dummy.bin -j a.json
echo -e "$SEPARATOR"
../fwumd_tool.py test $OPTS
