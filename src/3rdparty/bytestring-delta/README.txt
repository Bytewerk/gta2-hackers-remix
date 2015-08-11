This library gets used to create a patch file of the latest GTA2 version by Vike the Hube ( http://gtamp.com/gta2 )
and apply it to the "freeware" GTA2.exe



Name:                bytestring-delta
Version:             0.1.0.1
Synopsis:            Simple, fast binary diff/patch
Description:
    Compute a patch between two ByteStrings which can later be applied to the
    first to produce the second.  This can be used to save bandwidth and disk
    space when many strings differing by a small number of bytes need to be
    transmitted or stored.
    .
    The underlying implementation is written in C, and can also be found at
    <http://ccodearchive.net/info/bdelta.html>.
    .
    Currently, a patch does not save any space when two strings differ by
    more than 1000 bytes.  This arbitrary limit serves to keep applications
    from spiking in memory and CPU usage, as the algorithm uses quadratic
    space and time with respect to the length of the patch.  A better
    algorithm may be introduced in a future version of bytestring-delta.

Homepage:            http://github.com/joeyadams/haskell-bytestring-delta
License:             MIT
License-file:        LICENSE
Author:              Joey Adams
Maintainer:          joeyadams3.14159@gmail.com
Copyright:           Copyright (c) Joseph Adams 2011
