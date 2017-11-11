all:
	#gcc -o hello -L. -lcontrolcan -lpthread -lusb main.c
	#arm-none-linux-gnueabi-gcc -o hello -L. -L /home/caidunqing/controlcan -lcontrolcan -lpthread main.c
	g++ -o hello_cpp main.cpp logrec.c  /home/pi/Public/linuxCan/controlcan/libcontrolcan.so  -lpthread 

clean:
	rm -f *.o hello
