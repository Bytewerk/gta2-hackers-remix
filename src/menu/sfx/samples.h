/*
        These are the IDs of the GTA2 sounds in fstyle.raw.
        Lots of files sound the same (eg. 0-3), although they
        *are* different (md5'd them) so we'll just pick the first
        one in that case. Maybe they are stereo channels? (We're just
        playing mono here).

        In total, we have 17 sound samples in there. Use the sdt2wav
        tool by Black_Phoenix to convert them to WAV and listen to them
        independently (or alternatively, use the main_test.c in the sfx
        folder).
*/

#define SFX_FSTYLE_UP_DOWN 0
#define SFX_FSTYLE_LEFT_RIGHT 4
#define SFX_FSTYLE_RETURN 8
#define SFX_FSTYLE_BACK 10
#define SFX_FSTYLE_CHEAT 16
