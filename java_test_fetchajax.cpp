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

int main() 
{
	Cgicc cgi;    // Ajax object
	char *cstr;
	// Create AJAX objects to recieve information from web page.
	form_iterator user_input = cgi.getElement("input");
	
	cout << "user1";
	
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
