#include <unistd.h>
#include "serial_talk.h"
#include "stdlib.h"

#include <unistd.h>
#include <iostream>
#include <ctype.h>
#include "opencv2/core/core_c.h"
#include "stdio.h"
#include "string.h"



using namespace std;


int main(int argc, char *argv[]) {
	
	char default_portname[20] = "/dev/ttyACM0";
	char default_baud[20] = "9600";
	char portname[20];
	char baud[20];
	char buf[20];

	cout << "ENTER PORT WHERE Arduino is mounted (enter 'd' for default: /dev/ttyACM0): ";
	cin >> buf;
	if (strcmp("d", buf) == 0) {
		strcpy(portname, default_portname);
	}
	else {
		strcpy(portname, buf);
	}

	cout << "ENTER BAUD RATE FOR Arduino COMMUNICATION (enter 'd' for default: 9600): ";
	cin >> buf;
	if (strcmp("d", buf) == 0) {
		strcpy(baud, default_baud);
	}
	else {
		strcpy(baud, buf);
	}

	port_initialize(portname, baud);

	while (true) {
	
				
		string Roll;
		Roll = "b";
		cout << "String: " << Roll << endl;
		
		//send_via_port((char *)"Roll: ", "char", 6);  
		send_via_port(&Roll, "string", 0);                     //sending string roll
		//send_via_port((char *)",", "char", 1);  	       //sending comma
			
		sleep(1);
	}


		

	return 0;
}
