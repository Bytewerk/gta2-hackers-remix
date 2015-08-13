#!/bin/bash
ROOT=$PWD/../../
TEMP=$PWD/temp_installer/
OBJ=$PWD/obj_installer/
HEADER=$PWD/packed_files.h
OBJCOPY="objcopy --output elf32-i386 --binary-architecture i386"

# prepare
rm -rf $TEMP $HEADER $OBJ 2&> /dev/null
mkdir $TEMP $OBJ

# create a list of files
cd $ROOT
files=$(find data bin -type f)

# header variables
EXTERNS=""
FILENAMES=""
SYMBOLS_START=""
SYMBOLS_END=""

# cd into the temp folder, so objcopy doesn't add the path to the
# symbol names
cd $TEMP
for f in $files
do
	# skip some files
	[[ "$f" == *g2hr.ini ]] && continue
	[[ "$f" == *vike_patch.xz ]] && continue
	
	# create a hash
	hash=$(md5sum $ROOT/$f | cut -f 1 -d " ")
	echo $hash : $f
	
	# create a compressed objects
	xz $ROOT/$f --stdout > $TEMP/$hash
	$OBJCOPY --input binary $hash $OBJ/$hash.o
	
	# save the header variables
	FILENAMES="${FILENAMES}\"${hash}\","
	SYMBOLS_START="${SYMBOLS_START}_binary_${hash}_start,"
	SYMBOLS_END="${SYMBOLS_END}_binary_${hash}_end,"
	EXTERNS="$EXTERNS
extern char _binary_${hash}_start;
extern char _binary_${hash}_end;"


done


# create the header file
cat <<EOF > $HEADER
// this file gets generated automatically by pack.sh
$EXTERNS

const char** PACKED_FILENAMES = { ${FILENAMES} NULL};
const char*  PACKED_START = { ${SYMBOLS_START} '\0'};
const char*  PACKED_END = { ${SYMBOLS_END} '\0'};

EOF

# clean up
rm -r "$TEMP"


