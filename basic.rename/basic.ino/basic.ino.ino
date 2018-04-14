#include <Bridge.h>
//#include <Console.h>
//#include <Wire.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
//#include <YunServer.h>
//#include <YunClient.h>
#include <string.h>

// Socket port 1667 to communicate.
//YunServer serverSocket(1667); 
int LEDPIN = 3;
BridgeServer serverSocket(1667);
String TURN_ON = "turn on the li";
String TURN_OFF = "turn off the li";

/**
* Entry point of the program.
* Initialize everything. Called when the Arduino is powered.
*/
void setup() 
{
	// Bridge startup
  	//pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LEDPIN, OUTPUT);
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
  //YunClient client = serverSocket.accept();
  BridgeClient client = serverSocket.accept();

  // There is a new client
  if (client) 
	{
    Serial.println("Client connected");

    // When we get a client, go in the loop and exit only when the client disconnect. This will happens when the android application is killed (the socket must be closed by the app). This will automatically happens from the website for each http request.
    while(client.connected())
  	{	
      process(client);
    }
    
    Serial.println("Client disconnected");
    //digitalWrite(LED_BUILTIN,LOW);
    digitalWrite(LEDPIN,LOW);
    client.stop();
  }
  else 
	{
    Serial.println("No client connected");
  }
  // Delay for the battery, for the debug too. Doesn't affect the response time of the Arduino. (Check if there is another client each 		second)
  delay(750);
}

/**
* Will get the command request from the socket/http/etc. entry point.
* Will parse the request and execute it.
*/
void process(BridgeClient client) 
{
	String command = client.readStringUntil('g');
  Serial.println(command);
  const char* com = command.c_str();
  const char* on = TURN_ON.c_str();
  const char* off = TURN_OFF.c_str();

  if(strcmp(com, on) == 0)
  {
    Serial.println("turn on");
    //digitalWrite(LED_BUILTIN,HIGH);
    digitalWrite(LEDPIN,HIGH);
  }
  else if(strcmp(com, off) == 0)
  {
    Serial.println("turn off");
    //digitalWrite(LED_BUILTIN,LOW);
    digitalWrite(LEDPIN,LOW);
  }
  else
  { 
    
  }
}
