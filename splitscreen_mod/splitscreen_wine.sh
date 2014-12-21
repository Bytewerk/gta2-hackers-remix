#!/bin/bash
#
# Sound doesn't work when merging windows with AutoIt3. However,
# wine can create a virtual desktop, which is more or less the
# same thing (all GTA2 windows in one window!) - so we'll just
# do that :)
#
# TODO: check for installed programs (winetricks, wine, xrandr)
# TODO: take resolution as parameters, use desktop resolution
# 	as default
#

export DISPLAY=:0
export WINEPREFIX=~/.wine-g2hr
export WINEARCH=win32
export LANG=C

# Set the desired resolution
# width=800
# height=600

# Desktop resolution
xrandr=$(xrandr --current | grep current)
width=$(echo $xrandr | cut -d ' ' -f 8)
height=$(echo $xrandr | cut -d ' ' -f 10 | cut -d',' -f 1)


# Get the directory of the script file, even if it was symlinked etc.
# Source: http://stackoverflow.com/a/246128
# resolve $src until the file is no longer a symlink
src="${BASH_SOURCE[0]}"
while [ -h "$src" ]; do
	dir="$( cd -P "$( dirname "$src" )" && pwd )"
	src="$(readlink "$src")"

	# if $src was a relative symlink, we need to resolve it relative
	# to the path where the symlink file was located
	[[ $src != /* ]] && src="$dir/$src"
done
src="$( cd -P "$( dirname "$src" )" && pwd )"



# install the required libraries (only if they are not installed yet)
winetricks -q directplay vb6run

# start the mod as virtual desktop
cd "$src"
wine explorer /desktop=G2HR,${width}x${height} splitscreen_mod.exe


# to get it working, you'll need to start a normal network game first:
# cd ~/gta2/GTA2-MP
# wine explorer /desktop=G2HR,${width}x${height} gta2manager.exe



