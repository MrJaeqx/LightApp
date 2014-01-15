#include <Ethernet.h>

byte mac[] = { 0xFF, 0xAA, 0x09, 0x02, 0x0D, 0xED };  
byte ip[] = { 192, 168, 0, 224 };    
byte gateway[] = { 192, 168, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };

Server server = Server(2020);

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void loop()
{
  // if an incoming client connects, there will be bytes available to read:
  Client client = server.available();
  if (client == true) {
    // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    server.write(client.read());
  }
}
