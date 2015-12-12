#!/bin/sh
# Syntax:
#	g2hr.sh
#	g2hr.sh winecfg
#	g2hr.sh width height

function STEP
{
	echo "$(tput bold)::: $1$(tput sgr0)"
}

function UIOPT
{
	cat "$UICONFIG" | grep "$1 = " | cut -d '=' -f 2 \
		| tr -c -d "[:graph:]"
}

# $ROOT is the path that SDL chooses to save local data in
ROOT=~/.local/share/bytewerk/G2HR
export WINEPREFIX=$ROOT/WINEPREFIX
export WINEDEBUG=-all
export WINEARCH=win32
SETUPFLAG="$WINEPREFIX/g2hr_prefix_prepared"
UICONFIG="$WINEPREFIX/drive_c/users/$(whoami)/Application Data/bytewerk/G2HR/ui.ini"


#
# PREPARE THE WINEPERFIX
#

STEP "preparing the G2HR wine prefix..."
if [ ! -e "$SETUPFLAG" ]; then
	if ! command -v winetricks >/dev/null 2>&1 ; then
		echo "ERROR: winetricks is required to set up the wine prefix!"
		exit 1
	fi
	mkdir -p "$ROOT"
	wineboot -u || exit 1

	# set the desktop background color to black
	reg="$(mktemp)"
	echo '[HKEY_CURRENT_USER\Control Panel\Colors]' >> "$reg"
	echo '"Background"="0 0 0"' >> "$reg"
	echo '"ActiveBorder"="0 0 0"' >> "$reg"
	echo '"ActiveTitle"="0 0 0"' >> "$reg"
	echo '"ButtonAlternateFace"="0 0 0"' >> "$reg"
	echo '"ButtonDkShadow"="0 0 0"' >> "$reg"
	echo '"ButtonFace"="0 0 0"' >> "$reg"
	echo '"ButtonHilight"="0 0 0"' >> "$reg"
	echo '"ButtonLight"="0 0 0"' >> "$reg"
	echo '"ButtonShadow"="0 0 0"' >> "$reg"
	echo '"ButtonText"="0 0 0"' >> "$reg"
	echo '"GradientActiveTitle"="0 0 0"' >> "$reg"
	echo '"GradientInactiveTitle"="0 0 0"' >> "$reg"
	echo '"InactiveBorder"="0 0 0"' >> "$reg"
	echo '"InactiveTitle"="0 0 0"' >> "$reg"
	echo '"InactiveTitleText"="0 0 0"' >> "$reg"
	echo '"Window"="0 0 0"' >> "$reg"
	echo '"WindowFrame"="0 0 0"' >> "$reg"
	echo '"WindowText"="0 0 0"' >> "$reg"
	regedit "$reg"
	rm "$reg"

	# winetricks actually verifies the hashes of the downloads :)
	STEP "installing 'directplay' and 'vb6run' with winetricks..."
	winetricks -q directplay vb6run || exit 1
	
	touch "$SETUPFLAG"
else
	# update the wine prefix in case wine was updated
	wine test >/dev/null 2>&1
fi


#
# GET THE DESIRED RESOLUTION AND START G2HR (OR WINECFG)
#

if [ "$1" == "winecfg" ]; then
	STEP "starting winecfg..."
	winecfg
else
	STEP "starting G2HR..."
	w=$1
	h=$2
	if [ -z "$h" ]; then
		# no width, height specified on commandline
		if [ "$(UIOPT fullscreen)" != "false" ]; then
			# fullscreen is enabled
			if command -v xrandr >/dev/null 2>&1 ; then
				xrandr=$(xrandr --current | grep current)
				w=$(echo $xrandr | cut -d ' ' -f 8)
				h=$(echo $xrandr | cut -d ' ' -f 10 | cut -d',' -f 1)
			else
				echo "xrandr not found!"
				echo "please specify your desktop resolution on the" \
					"commandline for fullscreen mode, like this:"
				echo "g2hr.sh width height"
			fi
		else
			echo "You have disabled the fullscreen mode. If you want to"
			echo "set a custom resolution, start g2hr.sh like this:"
			echo "g2hr.sh width height"
		fi
	fi
	
	# defaults
	[ -z "$w" ] && w=640
	[ -z "$h" ] && h=480
	
	echo "resolution: ${w}x${h}"
	wine explorer /desktop=G2HR,${w}x${h} bin/g2hr.exe --from-g2hr.sh &
	bin/g2hr_native.bin
fi


#
# CLEAN UP
#

STEP "cleaning up..."
wineboot -e -f


