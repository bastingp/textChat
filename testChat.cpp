#include <iostream>
#include <fstream>
#include "fifo.h"

using namespace std;

string receive_fifo = "toUser1";
string send_fifo = "fromUser1";
string rec2_fifo = "toUser2";
string send2_fifo = "fromUser2";

int main(){

	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);

	string connect, recMsg, message, update, disconnect;
	string userNumber;

	connect = "$CONNECT*";
	message = "$MESSAGE*";

	cout << "Send: "<< connect << endl;
	sendfifo.openwrite();
	sendfifo.send(connect);

	recfifo.openread();
	recMsg = recfifo.recv();
	cout << "Server sent: "<<recMsg<< endl;
	recfifo.fifoclose();
	sendfifo.fifoclose();

	if (recMsg == "$USER*USER1%"){
		userNumber = "1";
	}
	if (recMsg == "$USER*USER2%"){
		userNumber = "2";
	}

	cout << "Assigned to: User " << userNumber << endl;
	if (userNumber == "1"){
		update = "$USER"+userNumber+":UPDATE*";
		char choice;
		cout << "update or send message (u/s)" << endl;
		cin >> choice;
		string chat;
		if (choice == 's'){
			while(1){
				sendfifo.openwrite();
				cout << "type message: "<< endl;
				cin >> chat;
				chat = message +"User"+userNumber+":"+chat;
				sendfifo.send(chat);
				sendfifo.send(update);
				string chatRec;

				recfifo.openread();
		
				while (chatRec != "$/MESSAGE*"){
					chatRec = recfifo.recv();
					if(chatRec != "$/MESSAGE*"){
						//chatRec.erase(0,8);
						cout << "chat received: " << chatRec << endl;
					} 
				}
				recfifo.fifoclose();
				sendfifo.fifoclose();
			}
		}
		if (choice == 'u'){
			while(1){
				sendfifo.openwrite();
				cout << "UPDATE"<< endl;
				sendfifo.send(update);
				cout << "SENT" << endl;
				recfifo.openread();
				string chatRec;
				while (chatRec != "$/MESSAGE*"){
					chatRec = recfifo.recv();
					if (chatRec != "$/MESSAGE*" && chatRec != "$UPTODATE*"){
						//chatRec.erase(0,8);
						cout << "chat received: " << chatRec << endl;
					}
				}
				recfifo.fifoclose();
				sendfifo.fifoclose();
				//cout << "update? (y/n)" << endl;
				//cin >> x;
			}
		}
		sendfifo.fifoclose();
		disconnect = "$USER"+userNumber+":LEFT*";
		char quit;
		cout << "Quit?"<< endl;
		cin >> quit;
		sendfifo.openwrite();
		sendfifo.send(disconnect);
		sendfifo.fifoclose();
	}
}

if (userNumber == "2"){
	update = "$USER"+userNumber+":UPDATE*";
		char choice;
		cout << "update or send message (u/s)" << endl;
		cin >> choice;
		string chat;
		if (choice == 's'){
			while(1){
				sendfifo.openwrite();
				cout << "type message: "<< endl;
				cin >> chat;
				chat = message +"User"+userNumber+":"+chat;
				sendfifo.send(chat);
				sendfifo.send(update);
				string chatRec;

				recfifo.openread();
		
				while (chatRec != "$/MESSAGE*"){
					chatRec = recfifo.recv();
					if(chatRec != "$/MESSAGE*"){
						//chatRec.erase(0,8);
						cout << "chat received: " << chatRec << endl;
					} 
				}
				recfifo.fifoclose();
				sendfifo.fifoclose();
			}
		}
		if (choice == 'u'){
			while(1){
				sendfifo.openwrite();
				cout << "UPDATE"<< endl;
				sendfifo.send(update);
				cout << "SENT" << endl;
				recfifo.openread();
				string chatRec;
				while (chatRec != "$/MESSAGE*"){
					chatRec = recfifo.recv();
					if (chatRec != "$/MESSAGE*" && chatRec != "$UPTODATE*"){
						//chatRec.erase(0,8);
						cout << "chat received: " << chatRec << endl;
					}
				}
				recfifo.fifoclose();
				sendfifo.fifoclose();
				//cout << "update? (y/n)" << endl;
				//cin >> x;
			}
		}
		sendfifo.fifoclose();
		disconnect = "$USER"+userNumber+":LEFT*";
		char quit;
		cout << "Quit?"<< endl;
		cin >> quit;
		sendfifo.openwrite();
		sendfifo.send(disconnect);
		sendfifo.fifoclose();
	}
}
