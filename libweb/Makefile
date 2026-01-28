.PHONY: all

all: compile_w_ssl

full:
	git clone https://github.com/libbase/base.git
	git clone https://github.com/libbase/lbg-compiler.git
	cd base; sudo make; cd ..
	make compile_w_ssl

compile_w_ssl:
	gcc -ffreestanding \
	-c src/*.c 
# 	/usr/lib/libclibp.a 
# 	-lssl -lcrypto
	ar rcs /usr/lib/libweb.a /usr/lib/libclibp.a /usr/lib/x86_64-linux-gnu/libssl.so /usr/lib/x86_64-linux-gnu/libcrypto.so *.o
	rm -rf base
	rm -rf lbg-compiler

# Bloatware
NOSSL:
	gclibp --output a.out \
	web.c \
	src/webserver.c \
	src/parser.c \
	src/router.c \
	src/utils.c 