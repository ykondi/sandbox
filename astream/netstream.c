#include "netstream.h"

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
        snd_pcm_prepare( *(a->playback_handle) );
	for(i = 0; i < PACKETS; i++){
        	if ((err = snd_pcm_writei (*(a->playback_handle), &(a->buffer[i*BUFSIZE]), FRAMES)) != FRAMES) {
                        //fprintf (stderr, "write to audio interface failed (%s)\n",
                         //        snd_strerror (err));
			// Recover from xflow, reinitialize
                        snd_pcm_prepare( *(a->playback_handle) );
			break;
                }
	}
	*(a->playFlag) = 0;
	snd_pcm_drain( *(a->playback_handle));
}

int ReadNetStream( char *hostname, int timeout )
{
	time_t init, current, elapsed, oldtime;
	time( &init );
	
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
		return(-1);
	}


	// Netconfig variables:
	int sockfd, n;
	struct sockaddr_in servaddr, cliaddr;
	char ping[1];
	sockfd = UDPClientSetup( hostname, &servaddr);
        while((int)(time(&current) - init) < timeout) {
		elapsed = (time(&current) - init);
		if( elapsed > oldtime )
			printf(" elapsed: %ds\t(%d%)\n", (int)(elapsed), (100*(int)elapsed)/timeout);
		oldtime = elapsed;
		for(j = 0; j < PACKETS; j++)
		{
			// Ping server, then read
			sendto(sockfd, ping, 1, 0, (struct sockaddr *)&servaddr,sizeof(servaddr));
			recvfrom(sockfd, &rcvbuf[j*BUFSIZE], BUFSIZE, 0, NULL, NULL);
		}
		for(j = 0; j < PACKETS * BUFSIZE; j++)
			pTdata.buffer[j] = rcvbuf[j];
		
		pthread_create(&playthread, NULL, &playThread, (void *)&pTdata);
		usleep( DELAY - 1000 );
        }
	printf("Reached timeout\n");
	snd_pcm_drain(playback_handle);

        snd_pcm_close (playback_handle);
	free(pTdata.playFlag);
        return(0);
}


int ServeMicOutput( int timeout )
{
        time_t init, current;
        time( &init );
        // Sound card variables:
        int i;
        int err;
        char buf[BUFSIZE];
        snd_pcm_t *record_handle;
        snd_pcm_hw_params_t *hw_params;

        if( initRecorder( &record_handle, &hw_params ) )
                printf("Soundcard initialized\n");
        else
        {
                printf("Soundcard failed to initialize\n");
                exit(-1);
        }

        // Net config:
        int sockfd,n;
        struct sockaddr_in servaddr,cliaddr;
        socklen_t len;
        char ping[1];
        sockfd = UDPServerSetup( &servaddr);


        while((int)(time(&current) - init) < timeout ){
        	len = sizeof(cliaddr);
        	// Ping from client
        	recvfrom(sockfd, ping, 1, 0,(struct sockaddr *)&cliaddr,&len);
                if ((err = snd_pcm_readi (record_handle, buf, FRAMES)) != FRAMES) {
                        fprintf (stderr, "recording from audio interface failed (%s)\n",
                                 snd_strerror (err));
                        if (snd_pcm_prepare( record_handle ) < 0){
                                fprintf(stderr, "Error preparing handle.\n");
                        }
                }
                sendto(sockfd, buf, BUFSIZE, 0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
        }

       snd_pcm_close(record_handle);
       return(0);
}


int ServeWavFile( char *filename )
{
        FILE *in = fopen(filename, "r");

	char buffer[BUFSIZE];

	// Net config:
        int sockfd,n;
        struct sockaddr_in servaddr,cliaddr;
        socklen_t len;
        char ping[1];
        sockfd = UDPServerSetup( &servaddr);

        // Get metadata
        fread(buffer, 1, 44, in);
        while( fread(buffer, 1, BUFSIZE, in) > 0)
        {
        	len = sizeof(cliaddr);
        	// Ping from client
        	recvfrom(sockfd, ping, 1, 0,(struct sockaddr *)&cliaddr,&len);
                sendto(sockfd, buffer, BUFSIZE, 0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
		usleep( (DELAY)/2 + (DELAY)/4 );
        }
        return (0);
}
