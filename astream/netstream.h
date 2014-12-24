#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "audio.h"
#include "netconfig.h"

#define PACKETS 15
#define BUFSIZE 8192
#define FRAMES 2048
//#define DELAY (PACKETS*FRAMES*100000)/48000
#define DELAY 64000

int ReadNetStream( char *hostname, int timeout );
int ServeMicOutput( int time );
int ServeWavFile( char *filename );
