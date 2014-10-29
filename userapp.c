#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE	"/dev/gpscdev"

int main ( void )
{
	int	i, fd;
	char	ch;
	char	write_buf[100], read_buf[100];
	char* 	fmt = "%[^\n]%*c";

	fd = open ( DEVICE, O_RDWR );	// open for read and write

	if ( fd == -1 )
	{
		printf ( "File %s either doesn't exist or is locked\n", DEVICE );
		exit ( -1 );
	}

	printf ( "Use r to read from device\n w to write to device\n to enter command:  " );
	scanf ( "%c%*c", &ch );
	// will read everything up to the newline into the string you pass in, 
	// then will consume a single character (the newline) without assigning it to anything (that '*' is 'assignment suppression').

	// Otherwise,the newline is left in the input stream waiting to immediately terminate the the subsequent "%[^\n]" format directives.

	// The problem with adding a space character to the format directive (" %[^\n]") is that the space will match any white space. 
	// So, it will eat the newline from the end of the previous input, but it will also eat any other whitespace (including multiple newlines).

	switch ( ch )
	{
		case 'w' :
			printf ( " Enter data for writing to device, enter to terminate string: " );
			// scanf ( " [^\n]", write_buf );
			scanf ( "%[^\n]%*c", write_buf ); 
			// scanf ( "%[^\n]", write_buf );
			printf ( "\nwrite_buf is [%s]", write_buf );
			write ( fd, write_buf, sizeof ( write_buf ));
			break;

		case 'r' :
			read ( fd, read_buf, sizeof ( read_buf ));
			printf ( "device %s: [%s]\n", DEVICE, read_buf );
			break;

		default :
			printf ( "Command not recognised\n" );
			break;
	}
	close ( fd );

	return 0;
}
