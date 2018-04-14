#include <Bridge.h>
#include <Console.h>
#include <Wire.h>
#include <YunServer.h>
#include <YunClient.h>

const int LEDPIN = 13; 

// Socket port 1667 to communicate.
YunServer serverSocket(1667); 

/**
* Entry point of the program.
* Initialize everything. Called when the Arduino is powered.
*/
void setup() 
{
	// Bridge startup
  	pinMode(LED_BUILTIN, OUTPUT);
  	//digitalWrite(LED_BUILTIN, LOW);
  	Bridge.begin();

  	// Listen the entire network (socket communication)
  	serverSocket.noListenOnLocalhost();
  	serverSocket.begin();

  	Serial.begin(9600);
}


/**
* Has to act as an -infinite- loop.
* Contains the program logic.
* Wait for a client then process it when he's found.
*/
void loop() 
{
  	// Get clients coming from server
  	YunClient client = serverSocket.accept();

  	// There is a new client
  	if (client) 
	{
    		// Change the predifined timeout from 2000 to 5. Avoid impressive timeout.
    		client.setTimeout(5);
    		Serial.println("Client connected!");

    		// When we get a client, go in the loop and exit only when the client disconnect. This will happens when the android application 			is killed (the socket must be closed by the app). This will automatically happens from the website for each http request.
    		while(client.connected())
		{	
      			// Process request
      			process(client);
    		}
    		// Stop the car for security reasons.
    		doStop(client);
    
    		// Close connection and free resources.
    		client.stop();
  	}
  	else 
	{
    		Serial.println("no client connected, retrying");
  	}
  	// Delay for the battery, for the debug too. Doesn't affect the response time of the Arduino. (Check if there is another client each 		second)
  	delay(1000);
}

/**
* Will get the command request from the socket/http/etc. entry point.
* Will parse the request and execute it.
*/
void process(YunClient client) 
{
	//String command = client.readString();
	//String command = client.readStringUntil('\\');
  
	// Avoid interferences when there is no request. (Because we are in an infinite loop!)
  	/*if(command.length() > 0)
	{
		//Serial.println("Query:"+client.readString()); 
		//return;// DEBUG
    
		if(strcmp(command, "turn on the light") == 0)
		{
			digitalWrite(LED_BUILTIN,HIGH);
		}
		else if (strcmp(command, "turn off the light") == 0)
		{
			digitalWrite(LED_BUILTIN,LOW);
		}
		else
		{	

		}
  	}
	else
	{

	}*/
	digitalWrite(LED_BUILTIN,HIGH);
	
}

void doStop(YunClient client)
{
	client.print(F("stop"));
      	Serial.println("stop");
}
