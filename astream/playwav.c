#include <stdio.h>
#include <stdlib.h>
#include "audio.h"

#define BUFSIZE 8192
#define FRAMES	2048

main( int argc, char *argv[] )
{
	FILE *in = fopen(argv[1], "r");
	char buffer[BUFSIZE];
	snd_pcm_t *playback_handle;
        snd_pcm_hw_params_t *hw_params;
	int err;

        if( initPlayer( &playback_handle, &hw_params ) )
                printf("Soundcard initialized\n");
        else
        {
                printf("Soundcard failed to initialize\n");
                return(-1);
        }
	// Get metadata
	fread(buffer, 1, 44, in);
	while( fread(buffer, 1, BUFSIZE, in) >= 0)
	{
		if ((err = snd_pcm_writei (playback_handle, buffer, FRAMES)) != FRAMES) {
                        fprintf (stderr, "write to audio interface failed (%s)\n",
                                 snd_strerror (err));
                        return (1);
                }
	}
	snd_pcm_drain(playback_handle);

        snd_pcm_close (playback_handle);
        return (0);
}
	

