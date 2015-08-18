#!/bin/sh
version=$(cat src/common/headers/common.h | grep G2HR_VERSION | cut -f2 -d '"')
root=$PWD
src_stub="$root/rel/_temp/README_SRC_STUB.txt"
gta2_stub="$root/rel/_temp/README_GTA2_STUB.txt"

[ "$1" == "" ] && echo "Syntax: $(basename $0) -jN" && exit 1

function STEP
{
	echo "$(tput bold)::: $1$(tput sgr0)"
}


STEP "preparing 'rel' folder..."
[ -e rel ] && rm -r rel
mkdir -p rel/_temp
cat <<EOF > "$src_stub"
This folder is just here for the license file. Source code is on github:
https://github.com/Bytewerk/gta2-hackers-remix/
alternatively: http://g2hr.org
EOF

cat <<EOF > "$gta2_stub"
Please copy all files from the latest official GTA2 release in this
folder. The GTAMP-edition is not supported.
See http://git.io/g2hr-get-gta2 for more info and download links.
EOF


STEP "building the full Windows release..."
[ -e bin/g2hr_native.bin ] &&  make -C src/native clean
make $1


STEP "preparing common files..."
common="$root/rel/_temp/g2hr-$version-common"
mkdir -p "$common/GTA2"
cp -r --parents \
	bin/g2hr.exe \
	bin/g2hr_menu.exe \
	bin/proxy.dll \
	\
	data/controller-mappings/default.ini \
	data/frontend \
	data/g2hr.png \
	data/screen_layouts.cfg \
	data/g2hr.sample.ini \
	data/vike_patch/vike_patch.plainbsdiff \
	data/vike_patch/README.md \
	\
	README.txt \
	LICENSE \
	src/LICENSE \
	src-3rdparty/*/LICENSE \
	\
	\
	"$common"

# Fill stub-folders with READMEs etc.
cd "$common"
cp "$src_stub" src/README
cp "$src_stub" src-3rdparty/README
cp "$gta2_stub" GTA2/README.txt


STEP "packing Windows release..."
cd "$root"
target="$root/rel/_temp/g2hr-$version-windows/"
cp -r "$common" "$target"
cp bin/g2hr_native.exe "$target/bin/"
mv "$target/bin/g2hr.exe" "$target/g2hr.exe"

cd "$root/rel/_temp/"
zip -9 -r ../g2hr-$version-windows.zip "g2hr-$version-windows"


STEP "building the native component for Linux..."
cd "$root"
make -C src/native clean
make $1 -C src/native Linux


STEP "packing Linux release..."
target="$root/rel/_temp/g2hr-$version-linux/"
cp -r "$common" "$target"
cp bin/g2hr_native.bin "$target/bin/"
cp bin/g2hr.sh "$target/"

cd "$root/rel/_temp/"
tar vcfJ ../g2hr-$version-linux.tar.xz "g2hr-$version-linux"


STEP "done!"
cd "$root/rel"
rm -r "_temp"
du -h *











