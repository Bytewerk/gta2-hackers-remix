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
export WINEDEBUG=-all
export WINEARCH=win32


setup_done="false"
[ -e "$WINEPREFIX" ] && setup_done="true"


if [ "$setup_done" == "false" ]; then
	STEP "setting up the G2HR wine prefix..."
	mkdir -p "$ROOT"
	wineboot -u

	STEP "setting the desktop background color..."
	reg="$(mktemp)"
	echo '[HKEY_CURRENT_USER\Control Panel\Colors]' >> "$reg"
	echo '"Background"="0 0 0"' >> "$reg"
	regedit "$reg"
	rm "$reg"


	# FIXME: do this without winetricks and *verify* the downloads!
	STEP "installing 'directplay' and 'vb6run' with winetricks..."
	winetricks -q directplay vb6run
else
	STEP "testing the G2HR wine prefix..."
	wine this_should_display_an_error_after_setting_up_the_prefix
fi

STEP "starting wine virtual desktop (in background)..."
wine explorer /desktop=G2HR,${w}x${h} bin/g2hr.exe --just-work &


STEP "starting the native component..."
bin/g2hr_native.bin

STEP "cleaning up..."
# TODO, add some killalls etc.


