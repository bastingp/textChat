#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include "fifo.h"

using namespace std;

/* Fifo names */
string receive_fifo = "userStatus";
string send_fifo = "userAssignment";
/*
 * 
 */
int main() {
  bool user1Connected = false;
  bool user2Connected = false;
  Fifo recUser(receive_fifo);
  Fifo sendUser(send_fifo);
  string openMessage = "page opened";
  string user1Close = "User1: left";
  string user2Close = "User2: left";
 
  while (1){ //outer loop
	//finding users loop
    while ((user1Connected == false) || (user2Connected ==false)){
        recUser.openread();
        string user1Message = recUser.recv();
        
	if (user1Message == openMessage){ //user 1 connected
            sendUser.openwrite();
	    if(user1Connected == false){
            	sendUser.send("User1");
           	cout << "User 1 assigned"<<endl;
            	user1Connected = true;
           	sendUser.fifoclose();
            	recUser.fifoclose();

		//user 2 connected
            }else if(user1Connected == true && user2Connected == false){
		sendUser.send("User2");
		cout << "User 2 assigned"<<endl;
		user2Connected = true;
		sendUser.fifoclose();
		recUser.fifoclose();
	    }
		//user checking status
        } else if (user1Message == "StatCheck") {
	    sendUser.openwrite();
	    if (user1Connected == false){
		sendUser.send("User 1 has left");
	    }
	    if (user2Connected == false){
		sendUser.send("User 2 has left");
	    }	
            recUser.fifoclose();
	    sendUser.fifoclose();
        } else if (user1Message == user1Close ||
			 user1Message == user2Close){
	    user1Connected = false;
	    user2Connected = false;
	}        
    }

    //after both users are assigned//
    cout <<"Both Users Assigned" << endl;
    char end = 'n';
    while (end != 'y'){
    	recUser.openread();
    	string userMsg = recUser.recv();
	if (userMsg == user1Close){ //user 1 closes
	  cout << "User 1 left" << endl;
	  user1Connected = false;
	  end = 'y';
	}else if(userMsg == user2Close){ //user 2 closes
	  cout << "User 2 left" << endl;
	  user2Connected = false;
	  end = 'y';
	} else if (userMsg == "StatCheck"){ //status check
	    sendUser.openwrite();
	    sendUser.send("Both users connected");
	}
	if ((user1Connected == false) || (user2Connected == false)){
	  end = 'y';
	
	}
	if (userMsg == openMessage){ //prevents a third connection
	    sendUser.openwrite();
	    sendUser.send("full");
	}
	recUser.fifoclose();
    }
  }
    return 0;
}

