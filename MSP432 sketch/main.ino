
#include <Node.h>
#include <Sensor.h>
#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include "coap_client.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

unsigned long Delay=0;
unsigned long previous=0;

#define COMMANDE 0
#define DONNEE 1

    

    
    /***** wifi credential && setup *****/
    
//char ssid[] = "TT-MobileWiFi-c3dd";
char ssid[] = "ENITWireless";
char password[] = "4LNJLEF8" ;
int keyIndex = 0;

 
void WIFICONNECTION(){
      Serial.print("Attempting to connect to Network named: ");
      Serial.println(ssid); 
      //WiFi.begin(ssid,password);
      WiFi.begin(ssid);
      while ( WiFi.status() != WL_CONNECTED) {
      Serial.print("*");
      delay(300);
      }
      printWifiStatus();
      Serial.println("Waiting for an ip address");
       while (WiFi.localIP() == INADDR_NONE) {
            // print dots while we wait for an ip addresss
          Serial.print(".");
        delay(300);
  }
 
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("Network Name: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  delay(1000);
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



char * getIp(){
    char * buffer=(char *) malloc(8) ;
    IPAddress ip = WiFi.localIP();
    sprintf(buffer,"%u.%u.%u.%u",ip[0],ip[1],ip[2],ip[3]) ;
    return(buffer);
    free(buffer);

}



char *  getMacAddress()
{
    char * buffer=(char *) malloc(18) ; 
    uint8_t macAddressVal[SL_MAC_ADDR_LEN];
    uint8_t macAddressLen = SL_MAC_ADDR_LEN;
    sl_NetCfgGet(SL_MAC_ADDRESS_GET,NULL,&macAddressLen,macAddressVal);
    MACAddress MAC(macAddressVal);
    sprintf(buffer,"%2X:%2X:%2X:%2X:%2X:%2X",MAC[0],MAC[1],MAC[2],MAC[3],MAC[4],MAC[5]) ;
    return(buffer);
    free(buffer);
    
}



    
    /***** coap *****/

    
coapClient coap;
//IPAddress coapIp(10,110,108,248);
IPAddress coapIp(10,110,107,60);
int coapPort =5683;
char * PAYLOAD ;
void callback_response(coapPacket &packet, IPAddress coapIp, int coapPort) {
    //char p[packet.payloadlen + 1];
    //PAYLOAD=(char *)malloc((packet.payloadlen + 1)*sizeof(char)) ;
    //memcpy(PAYLOAD,packet.payload, packet.payloadlen) ;
    //memcpy(p, packet.payload, packet.payloadlen);
    //Serial.println(p) ;
    //Serial.println(PAYLOAD) ;
    //p[packet.payloadlen] = NULL;
    //response from coap server
 if(packet.type==3 && packet.code==0){
      Serial.println("ping ok");
    }

    //Serial.println(p);
    free(PAYLOAD);
}

/*****Initilizing coap clinet *****/

void initCoapClient(){
    coap.response(callback_response);
    // start coap client
    coap.start(coapPort);
}

/***** get method of the coap client *****/

char * getcoap(){
  int msid = coap.get(coapIp,coapPort,"CMD");
  Serial.println(PAYLOAD) ;
  return (PAYLOAD) ;
}

/***** put method of the coap client *****/

void putCoap(String donnee){
  int l=donnee.length() +1 ;
  char * conv=(char *)malloc(l*sizeof(char ));
  donnee.toCharArray(conv,l) ;
  int msgid =coap.put(coapIp,coapPort,"Node",conv,l);
  free(conv) ;  
}



    /***** udp *****/


WiFiUDP comUdp;

unsigned int localPort = 3333;      // local port to listen on


void initUDP(){

  Serial.println("\nWaiting for a connection from a client...");
  comUdp.begin(localPort);

}

/***** send data over udp *****/

void SENDDATA(String a){

    comUdp.beginPacket(comUdp.remoteIP(),comUdp.remotePort());
    comUdp.println(a) ;
    comUdp.endPacket();
}


Node node ;

    

/********oldSensorValue is a buckup for the old sensors values ***********/


Sensor * sensorBackup=(Sensor *)malloc(6*sizeof(Sensor)) ;


  /*****getModCom it returns the Communication Mode type *****/

uint8_t  getModCom(char * modcom){
  
    uint8_t mod=0 ;//for Serial or zigbee communication
    if(strcmp(modcom,"wifi")==0){ //for wifi communication
        mod=1 ;
    }
    return(mod) ;
}

      


   /***** receive data from user *****/

 char * receiveData(uint8_t mod)
{
  
   String a;
   char * data ;

  switch (mod) {
      case 0 :
      {
          a=Serial1.readString();
          data=(char *)malloc(a.length() *sizeof(char)) ;
          a.toCharArray(data,a.length()) ;
          break ;
      }
      case 1 :
      {
          data=(char *)malloc(255 *sizeof(char)) ;
          // read the packet into packetBufffer
          int len = comUdp.read(data, 255);
          break ;
      }
  
      default :

          break ;
  
      }
      return data;
      free(data);

} 



/***** Listening to incoming packets *****/

void EntenteCanal()

{

  uint8_t i=getModCom(node.getModComm());
  String a;  
  char*data=NULL;
  char*dataSend=NULL;
  
  switch(i) {
          case 0 :{
            
              if((Serial1.available()>0)){
              data=receiveData(i);
              if(data!=NULL){ 
              ParserSender(data);
              } 
              }
           break ;
           }

          case 1 :
          {
              int packetSize = comUdp.parsePacket();
              if (packetSize) {
                  Serial.print("Received packet of size ");
                  Serial.println(packetSize);
                  Serial.print("From ");
                  IPAddress remoteIp = comUdp.remoteIP();
                  Serial.print(remoteIp);
                  Serial.print(", port ");
                  Serial.println(comUdp.remotePort());
                  data =receiveData(i) ;
                  ParserSender(data);

              }
              break ;
              }
              default :
              Serial.println("No Communcation Mode Provided !!!!");
   
            }
  
}


      /***** Send Response Frame *****/


void sendFrame(DynamicJsonDocument data)
{

    String a ;
    uint8_t mod=getModCom(node.getModComm());
    switch (mod){
        case 0 :{
            serializeJson(data, Serial1);
            serializeJson(data, Serial);
            Serial.println();
            break; 
        }
          
      case 1 :{
           serializeJson(data,a);
           serializeJson(data,Serial);
           putCoap(a);
           break;
   
       }
         
      
   
      default :
        {
           serializeJson(data, Serial);
           Serial.println();
           break; 
        }
     }

    
}


uint8_t NmbOfString(char*str)
{
    uint8_t nbWords=0;
    for(uint8_t i=0;i<strlen(str);i++) 
    { 
        if(str[i] == ',') ++nbWords; 
    }
    
    return nbWords+1;
}



    /***** parse command frame && send response *****/


void ParserSender (char*str)

{
    String dd;
    uint8_t i=0;
    char * a ,*token;
    char data[100];
    DynamicJsonDocument dataToSend(1024);
    Serial.println(str) ;
    strcpy(data,str);
    uint8_t nbWords=NmbOfString(str);
    char* Array[nbWords];
    a = strtok(data, "<=>");
    token = strtok(a, ",");
    for( uint8_t j = 0; j < nbWords; j++ )
    {
        Array[j] = (char*)malloc(30);
    }

    while(token != NULL)
    {
      strcpy(Array[i], token);
      token = strtok(NULL, ",");
      i++;
    }
    uint8_t mod;
    mod=atoi(Array[1]);
    char*codeFonction=Array[2];
    //Serial.println("*******************");
    //Serial.println(codeFonction);
    //Serial.println(strcmp(codeFonction,"DISC"));
    //Serial.println("*******************");
    /***** Modification de la configuration d'un capteur*****/
    if(mod==COMMANDE)
    {
        if(strcmp(codeFonction,"ES")==0)
        {
            node.EditSensor(Array[3],atoi(Array[4]),Array[5],atoi(Array[6]),atoi(Array[7]));
            dataToSend=node.CreateFrameConfigSensor(atoi(Array[4]));
            serializeJson(dataToSend,dd);
            putCoap(dd);
        }
        else if(strcmp(codeFonction,"DISC")==0)
        {
            dataToSend=node.Discovrey();
            serializeJson(dataToSend,dd);
            serializeJson(dataToSend,Serial);
            Serial.println(dd) ;
            putCoap(dd) ;
        }
        /***** Modification de la configuration d'une mote *****/
        else if(strcmp(codeFonction,"EM")==0)
        {
            node.EditMote(Array[3],Array[4],atof(Array[5]),atof(Array[6]),atoi(Array[7]),Array[8],Array[9],Array[10]);
            dataToSend=node.CreateFrameConfigMote();
            serializeJson(dataToSend,dd);
            putCoap(dd);
        }

/***** Demande de configuration d'un capteur *****/
        else if(strcmp(codeFonction,"DC")==0)
        {
            dataToSend=node.getConfSensor(atoi(Array[3]));
            serializeJson(dataToSend,dd);
            putCoap(dd);
        }

/***** Demande de configuration d'un noeud*****/
        else if(strcmp(codeFonction,"DM")==0)
        {
            dataToSend=node.getMote();
            serializeJson(dataToSend,dd);
            putCoap(dd);
        }
        else if(strcmp(codeFonction,"DFC")==0)
        {
            node.DefaultConfigAll();
        }
        else if(strcmp(codeFonction,"DAC")==0){
            dataToSend=node.getSensorsConf();
            serializeJson(dataToSend,dd);
            putCoap(dd);
        }
/***** Level Battery *****/
        else if(strcmp(codeFonction,"LB")==0)
        { 
            dataToSend=node.levelBattery();
            serializeJson(dataToSend,dd);
            putCoap(dd);
        }
        else if(strcmp(codeFonction,"RESET")==0)
        {
            //USB.OFF();
            //xbee802.OFF();
            //USB.ON();
            //xbee802.ON();
            //dataToSend="RESET";
            //sendFrame(dataToSend);
        }

    }
  else if(mod==DONNEE)
  {
      if(strcmp(codeFonction,"ST")==0)
      {
          dataToSend=node.getValueThreshold(atoi(Array[3]));
          serializeJson(dataToSend,dd);
          putCoap(dd);
      }
      else if(strcmp(codeFonction,"SV")==0)
      {
          dataToSend=node.getSensorValue(atoi(Array[3]));
          serializeJson(dataToSend,dd);
          putCoap(dd);
      }
      else if(strcmp(codeFonction,"AV")==0)
      {
          dataToSend=node.getAllValue();
          serializeJson(dataToSend,dd);
          putCoap(dd);
      }
  }
  delay(3000);
}


char * ipS ;
void setup() {
    
    Serial.begin(9600);
    Serial1.begin(9600);
    WIFICONNECTION() ;
        delay(500);
    ipS=getIp() ;
        delay(500);
    initCoapClient();
    delay(500);
    initUDP() ;
        delay(500);

    node.DefaultConfigAll();
    delay(500);

    node.initSensor();
    delay(500);

    node.setAddDest(ipS);
    delay(500);

    sensorBackup=node.sensors ;
}

void loop() {
    unsigned long current = millis();
    Delay= current - previous;
    EntenteCanal();
    node.initSensor();
    sendFrame(node.RegularFrame(Delay,previous,sensorBackup));
    Serial.println() ;
    sendFrame(node.AlarmFrame());
    delay(2000) ;  
    Serial.println(Delay) ;
}
