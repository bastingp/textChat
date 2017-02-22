#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "fifo.h"

using namespace std;
string receive_fifo = "userAssignment";
string send_fifo = "userStatus";

int main() {
  string message, name;
  string reply, endMsg;

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);

  message = "page opened";
  cout << "Send:" << message << endl;
  sendfifo.openwrite();
  sendfifo.send(message);

  /* Get a message from a server */
  cout << "Server sent: ";
  recfifo.openread();
  reply = recfifo.recv();
  cout << reply << endl;
  recfifo.fifoclose();
  sendfifo.fifoclose();
  //cout << "Server sent: " << reply << endl;
  
  if(reply != "full"){
  char close = 'n';
  string checkStatus = "StatCheck";
  while (close == 'n'){
  	cout << "Close? (y/n)" << endl;
  	cin >> close;
	if (close == 'n'){
	  sendfifo.openwrite();
	  sendfifo.send(checkStatus);
	  recfifo.openread();
	  string status = recfifo.recv();
	  cout << status << endl;
	  recfifo.fifoclose();
	  sendfifo.fifoclose();
	}
  }
  sendfifo.openwrite();
  endMsg = reply + ": left";
  sendfifo.send(endMsg);
  sendfifo.fifoclose();
}
}
