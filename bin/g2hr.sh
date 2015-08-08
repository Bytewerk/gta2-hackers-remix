#!/bin/sh

function STEP
{
	echo "$(tput bold)::: $1$(tput sgr0)"
}

# Set the resolution here
w=640
h=480

# $ROOT is the path that SDL chooses to save local data in
export ROOT=~/.local/share/bytewerk/G2HR
export WINEPREFIX=$ROOT/WINEPREFIX
export SETUPFLAG="$WINEPREFIX/g2hr_prefix_prepared"
export WINEDEBUG=-all
export WINEARCH=win32


if [ ! -e "$SETUPFLAG" ]; then
	
	if ! command -v winetricks >/dev/null 2>&1 ; then
		echo "ERROR: winetricks is required to set up the wine prefix!"
		exit 1
	fi

	STEP "setting up the G2HR wine prefix..."
	mkdir -p "$ROOT"
	wineboot -u || exit 1

	STEP "setting the desktop background color..."
	reg="$(mktemp)"
	echo '[HKEY_CURRENT_USER\Control Panel\Colors]' >> "$reg"
	echo '"Background"="0 0 0"' >> "$reg"
	regedit "$reg"
	rm "$reg"

	# Winetricks actually verifies the hashes of the downloads!
	STEP "installing 'directplay' and 'vb6run' with winetricks..."
	winetricks -q directplay vb6run || exit 1
	
	touch "$SETUPFLAG"
else
	# this will update the wine prefix in case wine was updated
	STEP "updating the G2HR wine prefix..."
	wine this_should_display_an_error_after_setting_up_the_prefix \
		>/dev/null 2>&1
fi

STEP "starting wine virtual desktop..."
wine explorer /desktop=G2HR,${w}x${h} bin/g2hr.exe --just-work &


STEP "starting the native component..."
bin/g2hr_native.bin

STEP "cleaning up..."
wineboot -e -f


