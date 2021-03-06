#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "fifo.h"

using namespace std;

/* Fifo names */
string receive_fifo = "chatRequest";
string send_fifo = "chatReply";

const string user_assignment1 = "user1";
const string user_assignment2 = "user2";
/*
 * 
 */
 
 string get_command(string message, int& index);
 void store_message(string message, int starting_index, vector<string>& user1messages, vector<string>& user2messages);
 void output_messages_through_pipes(vector<string> messages, Fifo& sendfifo);
 void handle_update(string message, int starting_index, vector<string> user1, vector<string> user2, Fifo& sendfifo);
 vector<string> get_new_messages(int starting_index, vector<string> stored_messages);
 void assign_user(Fifo& sendfifo, bool& user1connected, bool& user2connected);
 void unassign_user(string message, int starting_index, bool& user1connected, bool& user2connected, vector<string>& user1messages, vector<string>& user2messages);
 void return_status(Fifo& sendfifo, bool user1connected, bool user2connected);
 
int main() 
{
	bool user1connected = false;
	bool user2connected = false;
	vector<string> user1messages;
	vector<string> user2messages;
	const string message_command = "MESSAGE";
	const string update_command = "UPDATE";
	const string load_command = "LOAD";
	const string unload_command = "UNLOAD";
	const string status_command = "STATUS";
	
	string inMessage;
	// create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);

	while (1) 
	{
		/* Get a message from a client */
		recfifo.openread();
		inMessage = recfifo.recv();
		
		int i = 0;
		string command = get_command(inMessage, i);
		if(command == message_command)
		{
			store_message(inMessage, i, user1messages, user2messages);
		}
		else if(command == update_command)
		{
			handle_update(inMessage, i, user1messages, user2messages, sendfifo);
		}
		else if(command == load_command)
		{
			assign_user(sendfifo, user1connected, user2connected);
		}
		else if(command == unload_command)
		{
			unassign_user(inMessage, i, user1connected, user2connected, user1messages, user2messages);
		}
		else if(command == status_command)
		{
			return_status(sendfifo, user1connected, user2connected);
		}
		
		
		recfifo.fifoclose();
	}

	return 0;
}

string get_command(string message, int& index)
{	
	index = 0;
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

void store_message(string message, int starting_index, vector<string>& user1messages, vector<string>& user2messages)
{
	int i = starting_index;
	string username = "";
	
	//get username
	while(message[i] != '%')
	{
		username += message[i];
		i++;
	}
	i++;
	
	//get the message
	string str = "";
	while(message[i] != '*')
	{
		str += message[i];
		i++;
	}
	
	if(username == user_assignment1)
	{
		user1messages.push_back(str);
	}
	else
	{
		user2messages.push_back(str);
	}
}

void handle_update(string message, int starting_index, vector<string> user1, vector<string> user2, Fifo& sendfifo)
{
	int i = starting_index;
	string username = "";
	//get username
	while(message[i] != '%')
	{
		username += message[i];
		i++;
	}
	i++;
	
	string size_as_str = "";
	while(message[i] != '*')
	{
		size_as_str += message[i];
		i++;
	}
	
	//check size of client's messages
	int size = atoi(size_as_str.c_str());
	vector<string> reply;
	if(username == user_assignment1)
	{
		//return other user's data
		if(size < user2.size())
		{
			reply = get_new_messages(size, user2);
			reply.push_back("$END");
		}
		else
		{
			reply.push_back("$UPTODATE*");
		}
	}
	else if(username == user_assignment2)
	{
		//return other user's data
		if(size < user1.size())
		{
			reply = get_new_messages(size, user1);
			reply.push_back("$END");
		}
		else
		{
			reply.push_back("$UPTODATE*");
		}
	}	
	output_messages_through_pipes(reply, sendfifo);
}

vector<string> get_new_messages(int starting_index, vector<string> stored_messages)
{
	vector<string> new_messages;
	for(int i = starting_index; i < stored_messages.size(); i++)
	{
		new_messages.push_back(stored_messages[i]);
	}
	
	return new_messages;
}

void output_messages_through_pipes(vector<string> messages, Fifo& sendfifo)
{
	sendfifo.openwrite();
	for(int i = 0; i < messages.size(); i++)
	{
		cout << messages[i] << endl;
		sendfifo.send(messages[i]);
	}
	sendfifo.fifoclose();
}

void assign_user(Fifo& sendfifo, bool& user1connected, bool& user2connected)
{
	//use a vector because output_messages_through_pipes uses a vector parameter
	vector<string> message;
	string username = "$USER%";
	if(!user1connected)
	{
		username += "user1";
		user1connected = true;
	}
	else if(!user2connected)
	{
		username += "user2";
		user2connected = true;
	}
	else
	{
		username += "FULL";
	}
	username += "*";
	message.push_back(username);
	
	output_messages_through_pipes(message, sendfifo);
}

void unassign_user(string message, int starting_index, bool& user1connected, bool& user2connected, vector<string>& user1messages, vector<string>& user2messages)
{
	int i = starting_index;
	string username = "";
	//get username
	while(message[i] != '*')
	{
		username += message[i];
		i++;
	}
	
	if(username == user_assignment1)
	{
		user1connected = false;
		user1messages.clear();
	}
	else
	{
		user2connected = false;
		user2messages.clear();
	}
}

void return_status(Fifo& sendfifo, bool user1connected, bool user2connected)
{
	string message = "$STATUS%";
	if(user1connected && user2connected)
	{
		message += "FULL";
	}
	else
	{
		message += "WAIT";
	}
	message += "*";
	
	//store in vector so output_messages_through_pipes can handle properly
	vector<string> vstr;
	vstr.push_back(message);
	output_messages_through_pipes(vstr, sendfifo);
}
