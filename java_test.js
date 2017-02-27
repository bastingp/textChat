var username;
var XMLHttp;
var strangers_messages = [];

// Things to do at page load
function pageInit() 
{
	setXMLHttp();
	getUsername();
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
	username = callCGI(loadMessage);
}

// Respond to send button
function addText() 
{
    // Get the text from the text box
    var inText = document.chatForm.textInput.value;
    
    // Make sure something is there
    if (inText.length < 1) return;
    
	updateChatBox(inText, username);
}


function updateStrangerMessages()
{
	var updateMessage = "*" + username + "$UPDATE";
	// var serverMessages = callCGI(updateMessage);
	
	updateChatBox(serverMessages, "Stranger");
}

function updateChatBox(messages, user)
{
	// Clear the input text
    document.chatForm.textInput.value = "";
	
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
						 + "&message=" + CGIMessage
						 ,true);
    XMLHttp.onreadystatechange=function() 
	{
		if(XMLHTttp.readyState == 4)
		{
			return XMLHttp.responseText;
		}
	}
}