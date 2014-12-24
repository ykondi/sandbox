#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

int initPlayer( snd_pcm_t **playbackhandle, snd_pcm_hw_params_t **hwparams );
int initRecorder( snd_pcm_t **playbackhandle, snd_pcm_hw_params_t **hwparams );

