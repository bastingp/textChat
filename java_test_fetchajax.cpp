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

bool requires_receive(string message, bool& asked_to_update);
string get_command(string message);

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
		bool asked_to_update;
		bool need_to_listen = requires_receive(message, asked_to_update);
		
		cout << "Send:" << message << endl;
		sendfifo.openwrite();
		sendfifo.send(message);
		if(need_to_listen)
		{
			recfifo.openread();
			//update sends multiple strings, so deal with it differently
			if(asked_to_update)
			{
				string all_replies;
				reply = recfifo.recv();
				while(reply.find("$END") > 0)
				{
					all_replies += "%" + reply;
					if(reply == "$UPTODATE*")
					{
						break;
					}
					reply = recfifo.recv();
				}
				
				reply = all_replies;
			}
			else
			{
				cout << "\nGoing to open read fifo...\n";
				recfifo.openread();
				reply = recfifo.recv();
			}
			
			recfifo.fifoclose();
			cout << reply;
		}
		sendfifo.fifoclose();
		
		cout << "Content-Type: text/plain\n\n";
	}
	
	return 0;
}

bool requires_receive(string message, bool& asked_to_update)
{
	const string load_command = "LOAD";
	const string update_command = "UPDATE";
	const string status_command = "STATUS";
	
	string command = get_command(message);

	if(command == update_command)
		asked_to_update = true;
	else
		asked_to_update = false;
	
	return (command == load_command || command == update_command || command == status_command);
}

string get_command(string message)
{	
	int index = 0;
	string command = "";
	
	//go past $
	while(message[index] != '$')
	{
		index++;
	}
	index++;
	//go past MESSAGE%
	while(message[index] != '%')
	{
		command += message[index];
		index++;
	}
	index++;
	
	return command;
}
