#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "netstream.h"

int isNumeric( char *str )
{
	int i;
	for(i = 0; i < strlen(str); i++)
		if( (str[i] > 58) || (str[i] < 48))
			return 0;
	return 1;
}

void usage( char *execName)
{
	printf("\nUsage: %s <option> <corresponding sub-option>\n", execName );
	printf("Options:\n");
	printf("\t-l <hostname> <time>\t: Listen to hostname for time seconds\n");
	printf("\t-s <time>\t\t: Stream mic output for time seconds\n");
	printf("\t   <filename>\t\t: Stream wav file 'filename'\n\n");
	printf("\t-d <hostname> <listentime> <servetime>\t: Daisychain from hostname and serve mic output\n\n");
}
	

main( int argc, char *argv[] )
{
	if( argc == 1 ) {
		usage( argv[0] );
		exit(1);
	}
	char handle = argv[1][1];

	if(handle == 'l' && argc == 4 )
	{
			ReadNetStream( argv[2], atoi(argv[3]) );
	}
	else if( handle == 's' && argc == 3)
	{
		if( isNumeric(argv[2]) )
			ServeMicOutput( atoi(argv[2]) );
		else
			ServeWavFile( argv[2] );
	}
	else if( handle == 'd' && argc == 5 )
	{
		if( fork() == 0 )
			ServeMicOutput( atoi(argv[4]) );
		else
			ReadNetStream( argv[2], atoi(argv[3]));
	}
	else
		usage( argv[0] );
}
