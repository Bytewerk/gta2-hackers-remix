#!/bin/bash
ROOT=$PWD/../../
TEMP=$PWD/temp_installer/
OBJ=$PWD/obj_installer/
HEADER=$PWD/c/packed_files.h
OBJCOPY="objcopy --output elf64-x86-64 --binary-architecture i386"

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
UNCOMPRESSED_SIZE=""

# cd into the temp folder, so objcopy doesn't add the path to the
# symbol names
cd $TEMP
for f in $files
do
	# skip some files
	[[ "$f" == *g2hr.ini ]] && continue
	[[ "$f" == *vike_patch.xz ]] && continue
	[[ "$f" == *.bin ]] && continue # linux files
	
	# create a hash and size
	hash=$(md5sum $ROOT/$f | cut -f 1 -d " ")
	size=$(du -b $ROOT/$f | cut -f 1)
	echo "$hash $f ($size)"
	
	# create a compressed objects
	xz --check=crc32 $ROOT/$f --stdout > $TEMP/$hash
	$OBJCOPY --input binary $hash $OBJ/$hash.o
	
	# save the header variables
	FILENAMES="${FILENAMES}
	\"${f}\","
	SYMBOLS_START="${SYMBOLS_START}
	&_binary_${hash}_start,"
	SYMBOLS_END="${SYMBOLS_END}
	&_binary_${hash}_end,"
	UNCOMPRESSED_SIZE="$UNCOMPRESSED_SIZE
	${size},"
	EXTERNS="$EXTERNS
extern char _binary_${hash}_start;
extern char _binary_${hash}_end;"
	

done


# create the header file
cat <<EOF > $HEADER
#pragma once
#include <stddef.h>

// this file gets generated automatically by pack.sh, do not modify.
$EXTERNS

// use this variable to iterate over all files, the last element is ""
char* PACKED_FILENAMES[] =
{${FILENAMES}
	""
};

char* PACKED_START[] =
{${SYMBOLS_START}
};

char* PACKED_END[] =
{${SYMBOLS_END}
};

size_t PACKED_UNCOMPRESSED_SIZE[] =
{${UNCOMPRESSED_SIZE}
};

EOF

# clean up
rm -r "$TEMP"


