dest    =build/tisys
dest-bin=$(dest)/usr/bin
dest-debian=$(dest)/DEBIAN

aaaaaaaa


all:
	mkdir -p $(dest-bin)
	mkdir -p $(dest-debian)
	cd src; $(MAKE);
	rsync src/* $(dest-bin)/
	rm $(dest-bin)/*.cpp
	rm $(dest-bin)/Makefile
	cp etc/control $(dest-debian)
	dpkg -b $(dest);

install:
	sudo dpkg -i $(dest).deb;
