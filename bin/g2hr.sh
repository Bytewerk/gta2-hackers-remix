#!/bin/sh
# Syntax: g2hr.sh [width height]
# By default it will use the full desktop resolution with xrandr

function STEP
{
	echo "$(tput bold)::: $1$(tput sgr0)"
}

# $ROOT is the path that SDL chooses to save local data in
ROOT=~/.local/share/bytewerk/G2HR
export WINEPREFIX=$ROOT/WINEPREFIX
export WINEDEBUG=-all
export WINEARCH=win32
SETUPFLAG="$WINEPREFIX/g2hr_prefix_prepared"
w=$1
h=$2


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

	# winetricks actually verifies the hashes of the downloads :)
	STEP "installing 'directplay' and 'vb6run' with winetricks..."
	winetricks -q directplay vb6run || exit 1
	
	touch "$SETUPFLAG"
else
	# update the wine prefix in case wine was updated
	STEP "updating the G2HR wine prefix..."
	wine this_should_display_an_error_after_setting_up_the_prefix \
		>/dev/null 2>&1
fi

STEP "starting wine virtual desktop..."
if [ -z "$h" ]; then
	if command -v xrandr1 >/dev/null 2>&1 ; then
		xrandr=$(xrandr --current | grep current)
		w=$(echo $xrandr | cut -d ' ' -f 8)
		h=$(echo $xrandr | cut -d ' ' -f 10 | cut -d',' -f 1)
	else
		echo "xrandr not found, but you could specify the resolution:" \
			"g2hr.sh width height"
		w=640
		h=480
	fi
fi
echo "resolution: ${w}x${h}"
wine explorer /desktop=G2HR,${w}x${h} bin/g2hr.exe --just-work &

STEP "starting the native component..."
bin/g2hr_native.bin

STEP "cleaning up..."
wineboot -e -f


