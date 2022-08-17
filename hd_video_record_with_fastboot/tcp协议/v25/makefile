OBJM = BS_MainActivity.o BS_FunInterface.o BS_Client.o
OBJB = BS_Client_Test.o BS_FunInterface.o BS_Client.o
CFLAGS += -c -g
CLFAGST += -lrt -lpthread

all: MainActivity BS_Client

MainActivity: $(OBJM)
	$(CC) $^  -o $@ $(CLFAGST)

BS_Client: $(OBJB)
	$(CC) $^ -o $@ $(CLFAGST)

BS_MainActivity.o: BS_MainActivity.c
	$(CC) $^ $(CFLAGS) -o $@ $(CLFAGST)

BS_FunInterface.o: BS_FunInterface.c
	$(CC) $^ $(CFLAGS) -o $@ $(CLFAGST)

BS_Client.o: BS_Client.c
	$(CC) $^ $(CFLAGS) -o $@ $(CLFAGST)

BS_Client_Test.o: BS_Client_Test.c
	$(CC) $^ $(CFLAGS) -o $@ $(CLFAGST)

clean:
	$(RM) *.o MainActivity BS_Client -rf