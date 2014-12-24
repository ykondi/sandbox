#include <stdio.h>
#include <stdlib.h>

#include "audio.h"
#include "netconfig.h"

#define BUFSIZE 8192
#define FRAMES 2048

main (int argc, char *argv[])
{
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

        for (;;) {
		len = sizeof(cliaddr);
		// Ping from client
      		recvfrom(sockfd, ping, 1, 0,(struct sockaddr *)&cliaddr,&len);
                if ((err = snd_pcm_readi (record_handle, buf, FRAMES)) != FRAMES) {
			//if( err < 0 )
			//	printf(" Buffer overrun \n");
			//else{
                        fprintf (stderr, "recording from audio interface failed (%s)\n",
                                 snd_strerror (err));
			if (snd_pcm_prepare( record_handle ) < 0){
                		fprintf(stderr, "Error preparing handle.\n");
		        }

                        //exit (1);
			//}
                }
		sendto(sockfd, buf, BUFSIZE, 0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
        }

        snd_pcm_close (record_handle);
        exit (0);
}

