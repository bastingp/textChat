#include <iostream>
// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "fifo.h"

using namespace std;
using namespace cgicc; // Needed for AJAX functions.

// fifo for communication
string receive_fifo = "chatRequest";
string send_fifo = "chatReply";

// string get_dummy_messages()
// {
	// string dummy_messages = "$UPDATE";
	
	// for(int i = 0; i < 5; i++)
	// {
		// dummy_messages += "%this is a dummy message";
	// }
	
	// dummy_messages += "*";
	// return dummy_messages;
// }

string get_parsed_message(string message)
{
	string str = "$MESSAGE%";
	int i = 0;
	//go past $
	while(message[i] != '$')
	{
		i++;
	}
	i++;
	//go past MESSAGE%
	while(message[i] != '%')
	{
		i++;
	}
	i++;
	//go past username%
	while(message[i] != '%')
	{
		i++;
	}
	i++;
	//copy the message
	while(message[i] != '*')
	{
		str += message[i];
		i++;
	}
	str += '*';	
	return str;
}

int main() 
{	
	Cgicc cgi;    // Ajax object
	char *cstr;
	// Create AJAX objects to recieve information from web page.
	form_iterator user_input = cgi.getElement("input");
	
	//Make sure user_input refers to a valid element
	if(user_input != cgi.getElements().end())
	{
		string message = **user_input;
		cout << "Content-Type: text/plain\n\n";
		cout << get_parsed_message(message);
	}
	
	// // create the FIFOs for communication
	// Fifo recfifo(receive_fifo);
	// Fifo sendfifo(send_fifo);
	
	// // // Call server to get results
	// string message = **input;
	// sendfifo.openwrite();
	// sendfifo.send(message);
	
	// /* Get a message from a server */
	// cout << "Content-Type: text/plain\n\n";
	// recfifo.openread();
	// string reply = recfifo.recv();
	// cout << reply;
	
	// recfifo.fifoclose();
	// sendfifo.fifoclose();
	
	return 0;
}
