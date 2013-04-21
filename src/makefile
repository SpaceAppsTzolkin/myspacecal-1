testSend.exe : testSend.o myspacecal_send.o
	cc -o testSend.exe testSend.o myspacecal_send.o -L/usr/lib/i386-linux-gnu -lcurl -Wl,-Bsymbolic-functions -Wl,-z,relro \
		

testSend.o : testSend.c myspacecal_send.h
	cc -c testSend.c

myspacecal_send.o : myspacecal_send.c myspacecal_send.h
	cc -c myspacecal_send.c


