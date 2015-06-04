#!/bin/bash

# This is just a stub file for developing. For the next release, there
# should be a seperate script for configuring the chroot and one for
# running. Maybe we'll add a nice progress screen that runs natively
# with the G2HR toolkit? :>

native_port="$1"

if [ "$native_port" == "" ]; then
	echo "This script gets executed by 'native.bin', run that instead!"
	exit 1
fi

export WINEDEBUG=-all
export WINEPREFIX="$PWD/build/wineprefix/"

wine explorer /desktop=G2HR,640x480 bin/menu.exe $native_port
