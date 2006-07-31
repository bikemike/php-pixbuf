PHPVER=php4
CFLAGS=`pkg-config gdk-pixbuf-2.0 --cflags` -I/usr/include/$(PHPVER)/main -I/usr/include/$(PHPVER)/Zend -I/usr/include/$(PHPVER)/TSRM -I/usr/include/$(PHPVER)/ -I/usr/include/$(PHPVER)/main -DCOMPILE_DL_PIXBUF
LIBS=`pkg-config gdk-pixbuf-2.0 --libs` 

php-pixbuf.so: php-pixbuf.o
	gcc -shared -L -rdynamic -o php-pixbuf.so php-pixbuf.o $(LIBS)

php-pixbuf.o: php-pixbuf.c php-pixbuf.h
	gcc -fpic -DCOMPILE_DL=1 -c -o php-pixbuf.o php-pixbuf.c $(CFLAGS)


#Compiling	cc -fpic -DCOMPILE_DL=1 -I/usr/local/include -I. -I.. -I../Zend -c -o <your_object_file>  <your_c_file>
#Linking	cc -shared -L/usr/local/lib -rdynamic -o <your_module_file> <your_object_file(s)>
