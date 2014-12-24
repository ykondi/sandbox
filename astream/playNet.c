#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "audio.h"
#include "netconfig.h"

#define BUFSIZE 8192
#define FRAMES 2048
#define PACKETS 50

typedef struct play_this{
	snd_pcm_t **playback_handle;
	char buffer[PACKETS * BUFSIZE];
	int *playFlag;
} pTargs;

void* playThread( void *args )
{
	int i, err;
	pTargs *a = args;
	if( *(a->playFlag) == 1 )
	{
		printf("Packet dropped\n");
		return;
	}
	*(a->playFlag) = 1;
	for(i = 0; i < PACKETS; i++){
        	if ((err = snd_pcm_writei (*(a->playback_handle), &(a->buffer[i*BUFSIZE]), FRAMES)) != FRAMES) {
                        fprintf (stderr, "write to audio interface failed (%s)\n",
                                 snd_strerror (err));
			// Recover from xflow, reinitialize
                        snd_pcm_prepare( *(a->playback_handle) );
                }
	}
	*(a->playFlag) = 0;
}

main (int argc, char *argv[])
{
	// Sound card variables:
        int i, j;
        int err;
	char rcvbuf[PACKETS * BUFSIZE], *playbuf;

        snd_pcm_t *playback_handle;
        snd_pcm_hw_params_t *hw_params;
	pthread_t playthread;
	pTargs pTdata;
        
	pTdata.playback_handle = &playback_handle;
	pTdata.playFlag = malloc(sizeof(int));
	*(pTdata.playFlag) = 0;

	if( initPlayer( &playback_handle, &hw_params ) )
		printf("Soundcard initialized\n");
	else
	{
		printf("Soundcard failed to initialize\n");
		exit(-1);
	}


	// Netconfig variables:
	int sockfd, n;
	struct sockaddr_in servaddr, cliaddr;
	char ping[1];
	sockfd = UDPClientSetup( argv[1], &servaddr);


        for (i=0; i < 10000; i++) {
		// Ping server
		for(j = 0; j < PACKETS; j++)
		{
			sendto(sockfd, ping, 1, 0, (struct sockaddr *)&servaddr,sizeof(servaddr));
			recvfrom(sockfd, &rcvbuf[j*BUFSIZE], BUFSIZE, 0, NULL, NULL);
		}

		for(j = 0; j < PACKETS * BUFSIZE; j++)
			pTdata.buffer[j] = rcvbuf[j];
		
		pthread_create(&playthread, NULL, &playThread, (void *)&pTdata);
        }
	snd_pcm_drain(playback_handle);

        snd_pcm_close (playback_handle);
        exit (0);
}

