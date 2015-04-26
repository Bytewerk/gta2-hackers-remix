Compile all PKGBUILDs on Arch Linux, so you can do proper crosscompiling. This might also work with MSYS2 on Windows (because it ships with pacman), but this hasn't been tested at all.
```
cd $package
makepkg -si
```
