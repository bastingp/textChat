#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "fifo.h"

using namespace std;

/* Fifo names */
string receive_fifo = "fromUser1";
string send_fifo = "toUser1";
string rec2_fifo = "fromUser2";
string send2_fifo = "toUser2";

int main() {

	bool user1Connect = false; // whether user 1 is connected or not
	bool user2Connect = false; // whether user 2 is connected or not
	Fifo recUser(receive_fifo);
	Fifo sendUser(send_fifo);
	Fifo recUser2(rec2_fifo);
	Fifo sendUser2(send2_fifo);

	string openMessage = "$CONNECT*"; //message sent when connection starts
	string user1Assign = "$USER*USER1%"; //assigns connector to user 1
	string user2Assign = "$USER*USER2%"; //assigns connector to user 2
	string user1Close = "$USER1:LEFT*"; //message received when user1 leaves
	string user2Close = "$USER2:LEFT*"; //message received when user2 leaves
	string user1Update = "$USER1:UPDATE*"; //request for user1 message update
	string user2Update = "$USER2:UPDATE*"; //request for user2 message update
	string recMessage = "$MESSAGE*"; //flag to indicate a message for text chat
	string endMsg = "$/MESSAGE*"; //indicates end of messages
	string upToDate = "$UPTODATE*"; //sent when there is nothing to update
	string userMissingMsg = "Waiting on a new user to connect"; 
		//message sent to update user if other user left

	string userMsg; //Message received from user
	vector<string> msgLog; //log of all messages for current users
	vector<string> sentMsg1; //messages sent to user 1
	vector<string> sentMsg2; //messages sent to user 2

	//recUser.openread(); //opens receiving fifo

	while (1){ //keeps server running
		
		//Find Users//
		while ((user1Connect == false) || (user2Connect == false)){
			recUser.openread();
			userMsg = recUser.recv();

			//assigns users//
			if (userMsg == openMessage){
				sendUser.openwrite();
				if (user1Connect == false){
					sendUser.send(user1Assign);
					sendUser.fifoclose();
					cout <<"User 1 assigned" << endl;
					user1Connect = true;
				}else{
					sendUser.send(user2Assign);
					sendUser.fifoclose();
					cout <<"User 2 Assigned" << endl;
					user2Connect = true;
				}

			}

			//updates user if no other user is found//
			if (userMsg == user1Update || userMsg == user2Update){
				sendUser.openwrite();
				sendUser.send(userMissingMsg);
				sendUser.fifoclose();
			}
			
			//resets variables if user disconnects during 'find users'
			if (userMsg == user1Close){
				user1Connect = false;
			}
			if (userMsg == user2Close){
				user2Connect = false;
			}
			recUser.fifoclose();
		}

		sendUser.fifoclose();
		recUser.fifoclose();


		//After both users connect//
		cout <<"Both users connected" << endl;
		char end = 'n'; //used to return to 'find users' loop
		while (end != 'y'){
			//recUser.openread();
			cout << "open" <<endl;
			recUser.openread();
			recUser.openread();
			userMsg = recUser.recv();
			cout << "read" << endl;
			//recUser.openwrite();
			//recUser.openwrite();
			//looks for 'disconnect' messages//
			if(userMsg == user1Close){ //checks if user 1 leaves
				cout << "user 1 left" << endl;
				user1Connect == false;
				msgLog.clear();
				sentMsg1.clear();
				sentMsg2.clear();
				end == 'y';
			}
			if(userMsg == user2Close){ //checks if user 2 leaves
				cout << "user 2 left" << endl;
				user2Connect == false;
				msgLog.clear();
				sentMsg1.clear();
				sentMsg2.clear();
				end == 'y';
			}


			//Receives messages//
			if(userMsg.find(recMessage) != string::npos){
				msgLog.push_back(userMsg);
			}

			//updates users chatroom//
			if(userMsg == user1Update){
				//finds and returns new messages//
				if (msgLog.size() > sentMsg1.size()){
					int dif = (msgLog.size() - sentMsg1.size());
					int index = (msgLog.size() - 1);
					dif--;
					for (dif; dif >= 0; dif--){
						string x = msgLog[index - dif];
						cout <<"Text sent:"<< x << endl;
						sendUser.openwrite();
						sendUser.send(x); 
						sentMsg1.push_back(x);
					}
					sendUser.send(endMsg);
				}else{
					sendUser.openwrite();
					sendUser.send(upToDate); //sends if there are no changes
				}
				sendUser.fifoclose();
			}
			if(userMsg == user2Update){
				sendUser.openwrite();
				if (msgLog.size() > sentMsg2.size()){
					int dif = (msgLog.size() - sentMsg2.size());
					int index = (msgLog.size() - 1);
					dif--;
					for (dif; dif >= 0; dif--){
						string x = msgLog[index - dif];
						cout <<"Text sent:"<< x << endl;
						sendUser.send(x);
						sentMsg2.push_back(x);
					}
					sendUser.send(endMsg);
				}else{
					sendUser.send(upToDate);
				}
			}
			sendUser.fifoclose();
			recUser.fifoclose();
			recUser.fifoclose();
		}



	}



}