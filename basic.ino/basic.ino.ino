#include <Bridge.h>
#include <Console.h>
#include <Wire.h>
#include <YunServer.h>
#include <YunClient.h>
#include <string.h>

const int LEDPIN = 13; 
const String TURN_ON = "turn on the light";
const String TURN_OFF = "turn off the light";


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
  	digitalWrite(LED_BUILTIN, LOW);
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
	String command = client.readString();
  const char* com = command.c_str();
	//String command = client.readStringUntil('\\');
  
	if(command.length() > 0)
	{
		//Serial.println("Query:"+client.readString()); 
		//return;// DEBUG
    Serial.print(command);
    
    if(strcmp(com, "turn on the light") == 0)
		{
      Serial.println("turn on");
			digitalWrite(LED_BUILTIN,HIGH);
		}
		else if (strcmp(com, "turn off the light") == 0)
		{
      Serial.println("turn off");
			digitalWrite(LED_BUILTIN,LOW);
		}
		else
		{	
      Serial.println("cannot process command");
		}
	}
}

void doStop(YunClient client)
{
  client.print(F("stop"));
  Serial.println("stop");
  digitalWrite(LED_BUILTIN,LOW);
}
