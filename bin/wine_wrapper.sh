#!/bin/bash

# This is just a stub file for developing. For the next release, there
# should be a seperate script for configuring the chroot and one for
# running. Maybe we'll add a nice progress screen that runs natively
# with the G2HR toolkit? :>

native_port="$1"
gdb="/usr/i686-w64-mingw32/bin/gdb.exe"

if [ "$native_port" == "" ]; then
	echo "This script gets executed by 'native.bin', run that instead!"
	exit 1
fi

export WINEDEBUG=-all+reg
export WINEPREFIX="$PWD/build/wineprefix/"


# Attach to the meta logfile
# TODO: also attach to the injected logfile
METALOG=meta_errors.log
[ -e $METALOG ] && rm $METALOG
touch $METALOG
tail -f $METALOG &
TAIL_META=$!

# Actually start wine in a virtual desktop
cmd="bin/menu.exe $native_port"

if [ "$2" == "--gdb" ]; then
	cmd="$gdb -batch -ex run -ex bt --args $cmd"
fi

cmd="wine explorer /desktop=G2HR,640x480 $cmd"
echo "> $cmd"
$cmd

# Clean up the logfiles
rm $METALOG
kill $TAIL_META



