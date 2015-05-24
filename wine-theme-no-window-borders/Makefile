winetheme.msstyles: WineSampleTheme.ini winetheme.rc colornames.txt fileresnames.txt origfilenames.txt packthem_version.txt sizenames.txt
	i686-w64-mingw32-windres -o winetheme.o winetheme.rc
	i686-w64-mingw32-gcc -shared -o winetheme.msstyles winetheme.o

clean:
	rm *.o *.msstyles

PHONY: clean
