playback:	playback.c audio.h
	gcc -c playback.c

#play:	playback.o play.c
#	gcc play.c playback.o -lasound -o play

recorder:	recorder.c audio.h
	gcc -c recorder.c

#record:	record.c recorder.o
#	gcc record.c recorder.o -lasound -o record

netconfig:	netconfig.h netconfig.c
	gcc -c netconfig.c

netstream:	netstream.h netstream.c
	gcc -c netstream.c

nettest:	recordNet.o playNet.o
	gcc recordNet.o netconfig.o recorder.o -lasound -o server
	gcc playNet.o netconfig.o playback.o -lasound -lpthread -o client

master:	master.c netconfig.o playback.o recorder.o netstream.o
	gcc master.c playback.o recorder.o netconfig.o netstream.o -lasound -lpthread -o astream

all:	playback recorder netconfig netstream master
	sudo cp astream /bin
