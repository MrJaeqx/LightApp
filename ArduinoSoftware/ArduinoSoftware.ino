#include "EtherShield.h"

const int light1 = 2;
const int light2 = 3;
const int light3 = 4;
boolean light1State = LOW;
boolean light2State = LOW;
boolean light3State = LOW;

static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x25}; 

static uint8_t myip[4] = {192,168,0,177};

#define MYWWWPORT 80
#define BUFFER_SIZE 550
static uint8_t buf[BUFFER_SIZE+1];

EtherShield es=EtherShield();

uint16_t print_data(uint8_t *buf)
{
  uint16_t plen;

  plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("ENNE"));

  return(plen);
}

uint16_t print_data1(uint8_t *buf)
{
  uint16_t plen;

  plen=es.ES_fill_tcp_data_p(buf,plen,PSTR("TEST"));

  return(plen);
}


void setup(){
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  
  // Initialise SPI interface
  es.ES_enc28j60SpiInit();
  // initialize enc28j60
  es.ES_enc28j60Init(mymac);
  // init the ethernet/ip layer:
  es.ES_init_ip_arp_udp_tcp(mymac,myip, MYWWWPORT);
}

void loop(){
  uint16_t plen, dat_p;

  while(1) {
    // read packet, handle ping and wait for a tcp packet:
    dat_p=es.ES_packetloop_icmp_tcp(buf,es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf));

    /* dat_p will be unequal to zero if there is a valid 
     * http get */
    if(dat_p==0){
      // no http request
      continue;
    }
    // tcp port 80 begin
    
    if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0){
      dat_p=print_data(buf);
      goto SENDTCP;
    }

    if (strncmp("/10 ",(char *)&(buf[dat_p+4]),4)==0){
      dat_p=print_data1(buf);
      light1State = LOW;
      goto SENDTCP;
    }
    if (strncmp("/11 ",(char *)&(buf[dat_p+4]),4)==0){
      dat_p=print_data1(buf);
      light1State = HIGH;
      goto SENDTCP;
    }
    if (strncmp("/20 ",(char *)&(buf[dat_p+4]),4)==0){
      dat_p=print_data1(buf);
      light2State = LOW;
      goto SENDTCP;
    }
    if (strncmp("/21 ",(char *)&(buf[dat_p+4]),4)==0){
      dat_p=print_data1(buf);
      light2State = HIGH;
      goto SENDTCP;
    }
    if (strncmp("/30 ",(char *)&(buf[dat_p+4]),4)==0){
      dat_p=print_data1(buf);
      light3State = LOW;
      goto SENDTCP;
    }
    if (strncmp("/31 ",(char *)&(buf[dat_p+4]),4)==0){
      dat_p=print_data1(buf);
      light3State = HIGH;
      goto SENDTCP;
    }

    else{
      dat_p=es.ES_fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
      goto SENDTCP;
    }


SENDTCP:
    es.ES_www_server_reply(buf,dat_p); // send web page data
    // tcp port 80 end
    digitalWrite(light1, light1State);
    digitalWrite(light2, light2State);
    digitalWrite(light3, light3State);
  }

}



