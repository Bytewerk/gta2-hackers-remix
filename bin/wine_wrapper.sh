#!/bin/bash

# This is just a stub!
# TODO:
# - set up an extra wine prefix
# - install required DLLs in that prefix (like winetricks, but with
#		checksum checking)
# - use that wine prefix!
# - support fullscreen mode
# - (overkill: show a native wine prefix setup progress screen with the
#		g2hr toolkit :>)


native_port="$1"

if [ "$native_port" == "" ]; then
	echo "This script gets executed by 'native.bin', run that instead!"
	exit 1
fi

WINEDEBUG=-all wine explorer /desktop=G2HR,640x480 bin/menu.exe $native_port
