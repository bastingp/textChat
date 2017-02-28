var username;
var XMLHttp;
var strangers_messages = [];

// Things to do at page load
function pageInit() 
{
	setXMLHttp();
	//getUsername();
}

function setXMLHttp()
{
	if(navigator.appName == "Microsoft Internet Explorer") 
	{
		XMLHttp = new ActiveXObject("Microsoft.XMLHTTP");
	} else 
	{
		XMLHttp = new XMLHttpRequest();
	}
}

function getUsername()
{
	var loadMessage = "$LOAD";
	callCGI(loadMessage);
}

function checkStrangerMessages()
{
	var updateMessage = "$UPDATE%" + username + "*";
	callCGI(updateMessage);
}
//setInterval(function(){ checkStrangerMessages()}, 1000)


// Respond to send button
function sendMessage() 
{
    // Get the text from the text box
    var inText = document.chatForm.textInput.value;
    
    // Make sure something is there
    if (inText.length < 1) return;
	
	// Clear the input text
    document.chatForm.textInput.value = "";    
	var cgiMessage = "$MESSAGE%" + username + '%' + inText + '*';
	callCGI(cgiMessage);
	updateChatBox(inText, "You");
}

function updateChatBox(messages, user)
{	
	//Check if messages is an array, then add all messages to bottom of chatbox
	if(messages.constructor == Array)
	{
		for(var i = 0; i < messages.length; i++)
		{
			document.getElementById('chatBox').innerHTML += "<font color='red'>" + user + ": </font>" + messages[i] +"<br />";
		}
	}
	else
	{
		document.getElementById('chatBox').innerHTML += "<font color='red'>" + user + ": </font>" + messages +"<br />";
	}
    
    // force to bottom
    updateScroll();
}

// Function to force scrollable window to bottom
function updateScroll()
{
    var element = document.getElementById("chatBox");
    element.scrollTop = element.scrollHeight;
}

function callCGI(CGIMessage)
{	
	XMLHttp.open("GET", "/cgi-bin/schutzj_java_test_fetchajax.cgi?"
						 + "&input=" + CGIMessage
						 ,true);
    XMLHttp.onreadystatechange=function() 
	{
		if(XMLHttp.readyState == 4)
		{
			var response = XMLHttp.responseText;
			parseResponse(response);
		}
	}
	XMLHttp.send(null);
}

function parseResponse(response)
{
	var user = "USER";
	var message = "MESSAGE";
	var uptodate = "UPTODATE";
	var update = "UPDATE";
	
	var i = 0;
	//move index to beginning of message
	while(response[i] != '$')
	{
		i++;
	}
	
	//move index to first character of message (past '$')
	i++;
	var temp = '';
	while(i < response.length)
	{
		if(response[i] == '%' || response[i] == '*')
		{
			break;
		}
		temp += response[i];
		i++;
	}
	
	i++;
	//figure out what to do with message
	if(temp == user)
	{
		assignUsername(response, i);
	}
	else if(temp == message)
	{
		//If message, should already be printed locally
		return;
	}
	else if(temp == uptodate)
	{
		//If up to date, do nothing
		return;
	}
	else if(temp == update)
	{
		updateStrangerMessages(response, i);
	}
}

function assignUsername(response, startingIndex)
{
	var i = startingIndex;
	username = '';
	while(i < response.length)
	{
		if(response[i] == '*')
		{
			break;
		}
		username += response[i];
		i++;
	}
}

function updateStrangerMessages(response, startingIndex)
{
	var i = startingIndex;
	temp = '';
	new_messages = [];
	while(i < response.length)
	{
		if(response[i] == '*')
		{
			strangers_messages.push(temp);
			new_messages.push(temp);
			break;
		}
		else if(response[i] == '%')
		{
			strangers_messages.push(temp);
			new_messages.push(temp);
			temp = "";
		}
		else
		{
			temp += response[i];
		}
		i++;
	}
	
	//Only output new messages
	updateChatBox(new_messages, "Stranger");
}