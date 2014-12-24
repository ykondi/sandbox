
#include "audio.h"

int initRecorder( snd_pcm_t **playback_handle, snd_pcm_hw_params_t **hw_params )
{
	snd_pcm_t *pcm_handle;          
    	snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;
	snd_pcm_hw_params_t *hwparams;
	snd_pcm_hw_params_alloca(&hwparams);
	if (snd_pcm_open(&pcm_handle, "plughw:1,0", stream, 0) < 0) {
	      fprintf(stderr, "Error opening PCM device\n");
	      return(-1);
    	}

    	if (snd_pcm_hw_params_any(pcm_handle, hwparams) < 0) {
      		fprintf(stderr, "Can not configure this PCM device.\n");
      		return(-1);
    	}
    	int rate = 48000; 
    	int exact_rate;  
                     
    	int dir;         
                    
                   
    	int periods = 2;   
    	snd_pcm_uframes_t periodsize = 8192; 
  
    	if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
      		fprintf(stderr, "Error setting access.\n");
      		return(-1);
    	}
  
    	if (snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0) {
      		fprintf(stderr, "Error setting format.\n");
      		return(-1);
    	}

    exact_rate = rate;
    	if (snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, 0) < 0) {
      		fprintf(stderr, "Error setting rate.\n");
      		return(-1);
    	}

    	if (snd_pcm_hw_params_set_channels(pcm_handle, hwparams, 2) < 0) {
      		fprintf(stderr, "Error setting channels.\n");
      		return(-1);
    	}

    	if (snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0) < 0) {
      		fprintf(stderr, "Error setting periods.\n");
      		return(-1);
    	}
    	if (snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, (periodsize * periods)>>2) < 0) {
      		fprintf(stderr, "Error setting buffersize.\n");
      		return(-1);
    	}
    	if (snd_pcm_hw_params(pcm_handle, hwparams) < 0) {
      		fprintf(stderr, "Error setting HW params.\n");
      		return(-1);
    	}

	if (snd_pcm_prepare( pcm_handle ) < 0){
		fprintf(stderr, "Error preparing handle.\n");
		return(-1);
	}
    	*playback_handle = pcm_handle;
    	*hw_params = hwparams;

    	return(1);
}
