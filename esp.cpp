#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

void read_line(int stream, char *response);
void write_line(int stream, const char *cmd);

int open_serial(const char *device){

	int USB = open( device, O_RDWR| O_NOCTTY );

	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);
	
	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 ) {
	   std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
	}
	
	/* Save old tty parameters */
	tty_old = tty;
	
	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B9600);
	cfsetispeed (&tty, (speed_t)B9600);
	
	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;            // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;
	
	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  1;                  // read doesn't block
	tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
	
	/* Make raw */
	cfmakeraw(&tty);
	
	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
	   std::cout << "Error " << errno << " from tcsetattr" << std::endl;
	}

	return USB;
}
	

int main(int argc, char **argv){
	int USB = open_serial("/dev/ttyUSB0");
	int arduino = open_serial("/dev/ttyACM0");
	char response[1024];
	memset(response, '\0', sizeof response);

	write_line(USB, "print(1+1)\r\n");
	read_line(USB, response);
	write_line(USB, "dofile=(\'tcptrie2.lua\')\r\n");
	read_line(USB, response);
	//write_line(USB, "print(2+1)\r\n");
	//read_line(USB, response);
	while(1){
		memset(response, '\0', sizeof response);
		read_line(USB, response);
		write_line(arduino, response);
	}


	
	
	
	/* Whole response*/
	
	return 0;
}

void read_line(int stream, char *response){
	char buf = '\0';
	int n = 0;
	int spot = 0;
	do {
	    n = read( stream, &buf, 1 );
	    sprintf( &response[spot], "%c", buf );
	    spot += n;
	} while( buf != '\n' && n > 0);
	printf("%s", response);
}

void write_line(int stream, const char *cmd){
	int n_written = 0,
	spot = 0;
	do {
	    n_written = write( stream, &cmd[spot], 1 );
	    spot += n_written;
	} while (cmd[spot-1] != '\r' && n_written > 0);
}
