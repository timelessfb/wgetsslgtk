all:
	gcc `pkg-config --cflags --libs libcurl gtk+-2.0` -g wgetssl.c wgetssl.h wgetsslgtk.c -o WGETSSLGTK

clean:
		@echo "cleaning"
		rm -rf WGETSSLGTK
		rm -rf `find . -name "*.dSYM" -print`
		@echo "cleaning"
