#include<iostream>
#include<fstream>
#include<cstdlib>
#include <string.h>


using namespace std;

ifstream Arduino_Input;
ofstream Arduino_Output;

void port_initialize(char const *portname, char const *baud)
{
	char tty_settings[200];
	strcpy(tty_settings, "stty -F ");
	strcat(tty_settings, portname);
	strcat(tty_settings,  " cs8 ");
	strcat(tty_settings, baud);
	strcat(tty_settings,  " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts");

	cout << "BAUD rate: " << baud << endl;

	system(tty_settings);	//Activates the tty connection with the Arduino
//	system("stty -F /dev/ttyACM0 cs8 57600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts");	//Activates the tty connection with the Arduino
	Arduino_Input.open(portname);
//Opens the tty connection as an ifstream
	Arduino_Output.open(portname);
//Opens the tty connection as an ofstream, not used in this example
	double Voltage;	//The Arduino is reading the voltage from A0
	long int Time = time(NULL);
	int i;

	while(time(NULL)-Time < 5){}	//Wait five seconds for the Arduino to start up
}

int send_via_port(void *message, char const *type, int size)
{

//	for(i = 0; i < 100;)
//	{
//		Time = time(NULL);
//		while(time(NULL)-Time < 1){}	//wait one second to get
//good numbers into the Arduino stream
//		while(!Arduino_Input.eof())	//while the eof flage isn't set
//		{
//			Arduino_Input >> Voltage;	//will set the
//error flag if not ready, will get a number from the Arduino stream if ready
//			cout << Voltage << endl;	//Output it to the cout
//stream
//			i++;	//Increament i, it is not known how many numbers
//I'll get at a time
//		}
//		Arduino_Input.clear();	//eof flag won't clear itself
//	}

//	Arduino_Input.close();	//Close the ifstream

	//char msg[] = "ADIOS!!!";
	int bla = 10;
	
	if (strcmp(type, "double") == 0) {
		Arduino_Output << *(double *)message << std::flush;
	}
	else if (strcmp(type, "int") == 0) {
		Arduino_Output << *(int *)message << std::flush;
	}
	else if (strcmp(type, "char") == 0) {
//		Arduino_Output << *(char *)message << std::flush;
		Arduino_Output.write((char const *)message, size);
	}
	else if (strcmp(type, "float") == 0) {
		Arduino_Output << *(float *)message << std::flush;
	}
	else if (strcmp(type, "string") == 0) {
		Arduino_Output << *(string *)message << std::flush;
	}
//	else {
//		Arduino_Output << endl;
//	}

//	Arduino_Output.close();	//Close the ofstream
	return(0);

} 

void port_destruct()
{
	Arduino_Input.close();	//Close the ifstream
	Arduino_Output.close();	//Close the ofstream
}

