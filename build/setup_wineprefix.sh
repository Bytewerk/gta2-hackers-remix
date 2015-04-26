export WINEPREFIX="${PWD}/wineprefix"
export WINEARCH=win32
export WINEDEBUG=-all

au3setup="${WINEPREFIX}/drive_c/autoit-v3-setup.exe"
au3link="https://www.autoitscript.com/cgi-bin/getfile.pl?autoit3/autoit-v3-setup.exe"
au3path="${WINEPREFIX}/drive_c/Program Files/AutoIt3"



if [ ! -e "$WINEPREFIX" ]; then
	echo "Setting up G2HR wine prefix..."
	wineboot -u
fi

if [ ! -e "$au3setup" ]; then
	echo "Downloading AutoIt3..."
	wget -nv "${au3link}" -O "${au3setup}"
fi

if [ ! -e "$au3path" ]; then
	echo "Installing AutoIt3..."
	wine "$au3setup" /S
fi
