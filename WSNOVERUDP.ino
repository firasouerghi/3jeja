#include <ArduinoJson.h>

#include<stdio.h>

#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

char * ipS ;

#define COMMANDE 0
#define DONNEE 1
/*************wifiConfig********************/


unsigned int localPort = 3333;      // local port to listen on
char ssid[] = "TT-MobileWiFi-c3dd";
char password[] = "4LNJLEF8" ;
int keyIndex = 0;
WiFiUDP Udp;


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


void BEGINSERVER(){

 
  Serial.println("\nWaiting for a connection from a client...");
  Udp.begin(localPort);


}


void SENDDATA(String a){

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.println(a) ;
    Udp.endPacket();
}
  
void WIFICONNECTION(){
      Serial.print("Attempting to connect to Network named: ");
      Serial.println(ssid); 
      WiFi.begin(ssid,password);
      while ( WiFi.status() != WL_CONNECTED) {
      Serial.print("*");
      delay(300);
      }
            Serial.println(ssid); 

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
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



/*************wifiConfig********************/

//------------------struct mote-------------


typedef struct Mote
{
char*ID;
char*MoteName;
char*address;
char*Longitude;
char*Latitude; 
uint8_t SensorNumber; 
char*AddDest;
char*MoteGateway;
char*modComm;
};

//----------------------struct sensor-------------------------
typedef struct Sensor
{
char*SensorName;
uint8_t Port;
char*type;
uint8_t Threshold;//soit pour stocker var ou pour temps de sleep
uint8_t Samples;
float value;
//uint8_t ordre;
};


Sensor SENSOR;

unsigned long previous=0;

int Delay=0;

//int nbSens=15;

Sensor ListSensor[7];

void initsEN() 
{
  for(int i=0;i<7;i++)
  {
    ListSensor[i].value=0;
  }
}

Mote MOTE;

//-------------------------functions set **SENSOR***-----------------------------
void setPortSensor(uint8_t analog,uint8_t snb){
  
  ListSensor[snb].Port=analog;
}

void setTypeSensor(char*type,uint8_t snb){
ListSensor[snb].type=type;
}

void SetThreshold(uint8_t Threshold ,uint8_t snb){
ListSensor[snb].Threshold=Threshold;
}

void setSamples(uint8_t samples ,uint8_t snb){
ListSensor[snb].Samples=samples;
}

void setNameSensor(char*SensName,uint8_t snb){
ListSensor[snb].SensorName=SensName;
}

void setValue(float valeur,uint8_t snb){
ListSensor[snb].value=valeur;
}
/*  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!?????????????
void setOrdre(uint8_t ordre,uint8_t n){
ListSensor[n].ordre=ordre;
}*/
//----------------------------ConfSensor--------------------------
void ConfSensor(char*name,char*type,uint8_t EchSamp,uint8_t analogout,uint8_t snb) 
{
setPortSensor(analogout,snb);
setTypeSensor(type,snb);
if(strcmp(type,"ALARME")==0)
{
SetThreshold(EchSamp,snb);
}
else 
{
  setSamples(EchSamp,snb);}
  setNameSensor(name,snb);
/*setsnb(ordre,ordre);*/
}




//--------------------SET mote---------
void setId(char*ID){
MOTE.ID=ID;
}
void setName(char*name){
MOTE.MoteName=name;
}
void setaddress(char*address){
MOTE.address=address;
}
void setLongitude(char*Longitude)
{
MOTE.Longitude=Longitude;
}
void setLatitude(char*Latitude)
{
MOTE.Latitude=Latitude;
}
void setSensNumber(uint8_t SensorNumber)
{
MOTE.SensorNumber=SensorNumber;
} 
void setAddDestMote(char*AddDest)
{
MOTE.AddDest=AddDest;
}
void setMoteGateway(char* MoteGateway)
{
MOTE.MoteGateway=MoteGateway;
}
void setModComm(char*modComm)
{
MOTE.modComm=modComm;
}


//----------DefaultConfigAll------------------
void DefaultConfigAll()
{
//ConfSensor("ANE","ALARME",20,1,0);
ConfSensor("ANE","REGULIER",20,1,0);
ConfSensor("Vane","ALARME",25,2,1);
ConfSensor("PLUV","REGULIER",10,3,2);
ConfSensor("S_TEMP","ALARME",45,4,3);
ConfSensor("HUMI","ALARME",45,5,4);
ConfSensor("S_Rad","ALARME",45,6,5);

//----------Mote---------------------------
setId(getMacAddress());//getMacAddress());
setName("node01");
setaddress("manar");
setLongitude("ouest");
setLatitude("est");
setSensNumber(6);
setAddDestMote(ipS);
setModComm("wifi"); 
setMoteGateway("rasp_pi");

}

/*
*
*Read Sensor values ... 
*/
float readAnemometer()
{ 
  
float anemometer=5.5;
/* Turn on the sensor and wait for stabilization and response time
SensorAgrv20.setSensorMode(SENS_ON, SENS_AGR_ANEMOMETER);
delay(10);
// Read the anemometer sensor 
anemometer = SensorAgrv20.readValue(SENS_AGR_ANEMOMETER);
// Turn off the sensor
anemometer=anemometer + 0.002;
SensorAgrv20.setSensorMode(SENS_OFF, SENS_AGR_ANEMOMETER);
*/
return anemometer;

}
//------------------------------------
float readPluviometrie()
{
float pluviometrie=87.5;
//pluviometrie= SensorAgrv20.readPluviometerCurrent();
return pluviometrie;
}
//--------------------------------------------------
float readVane()
{
  float vane=45.5;
  //SensorAgrv20.ON();
//RTC.ON();
//int vane = SensorAgrv20.readValue(SENS_AGR_VANE);
//return SensorAgrv20.vaneDirection;
return vane;
}
//-------------------------------------------
float readPT100Temperature()
{ 
float value=1.25;
// Read the PT1000 sensor 
//value = SensorAgrv20.readValue(SENS_AGR_PT1000);

// Turn off the sensor
//SensorAgrv20.setSensorMode(SENS_OFF, SENS_AGR_PT1000);
return value;
}
//-------------------------------------------
float readRadiationSolar()
{
float value;
float radiation=2.2;
//SensorAgrv20.setSensorMode(SENS_ON, SENS_AGR_RADIATION);
delay(100);
// Read the solar radiation sensor 
//value = SensorAgrv20.readValue(SENS_AGR_RADIATION);
// Conversion from voltage into umol·m-2·s-1
radiation = value / 0.0002;
// Turn off the sensor
//SensorAgrv20.setSensorMode(SENS_OFF, SENS_AGR_RADIATION);
return radiation;
}
//----------------------------------------
float readSoilHumidity()
{ float value=12.2; 
// SensorAgrv20.setSensorMode(SENS_ON, SENS_AGR_WATERMARK_1);
// delay(100);
// 
// // Read the Watermark 1 sensor 
// value = SensorAgrv20.readValue(SENS_AGR_WATERMARK_1);
// 
// // Turn off the sensor
// SensorAgrv20.setSensorMode(SENS_OFF, SENS_AGR_WATERMARK_1);
return value;
}


/*
*
*Sensor value initialization :
*/
void initSensor()
{
ListSensor[0].value=readAnemometer();
ListSensor[1].value=readVane();
ListSensor[2].value=readPluviometrie();
ListSensor[3].value=readPT100Temperature();
ListSensor[4].value=readSoilHumidity();
ListSensor[5].value=readRadiationSolar();
}

 
char*receiveData(uint8_t mod)
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
    int len = Udp.read(data, 255);
    if (len > 0) data[len] = 0;
    Serial.println("Contents:");
    //Serial.println(packetBuffer);
    
      }

  break ;
  
  default :

  break ;
  
}
return data;
free(data);

}


/**************getModCom it returns the Communication type *****************/

uint8_t getModCom(char * modcom){
  uint8_t mod=0 ;

  if(strcmp(modcom,"serial")==0){
    mod=0 ;
    
  }
  else if(strcmp(modcom,"wifi")==0){
    mod=1 ;
    
  }
  else{
    mod =2 ;//for the zigbee ;
    
  }
  return(mod) ;

  
}

/************here/////////////////////*/
//------Entente&send savoir s'il y a des données à recevoir ou pas
void EntenteCanal()
{
  uint8_t i=getModCom(MOTE.modComm);
  String a;  
  char*data=NULL;
  char*dataSend=NULL;
  switch(i) {
          case 0 :
      if((Serial1.available()>0)){
        data=receiveData(i);
        if(data!=NULL){ 
          ParserSender(data);
        } 
      }

break ;

case 1 :
{
      int packetSize = Udp.parsePacket();
      if (packetSize) {
            Serial.print("Received packet of size ");
            Serial.println(packetSize);
            Serial.print("From ");
            IPAddress remoteIp = Udp.remoteIP();
            Serial.print(remoteIp);
            Serial.print(", port ");
            Serial.println(Udp.remotePort());
            data =receiveData(i) ;
            ParserSender(data);

      }
    

break ;
}
default :
Serial.println("dcds");

  
   
  }
  
}
/*************/
uint8_t NmbOfString(char*str)
{
uint8_t nbWords=0;
for(uint8_t i=0;i<strlen(str);i++) 
{ 
if(str[i] == ',') ++nbWords; 
}
return nbWords+1;/////§§§§§§§§§§§§§§§§§§§§§§§§§§§+2 !!!!!!
}
DynamicJsonDocument Discovrey()
{
  DynamicJsonDocument data(1024);
  JsonArray ack = data.createNestedArray("ack");
  ack.add("acknowledgment from ") ;
  JsonArray mote = data.createNestedArray("Mote");
  mote.add(MOTE.ID) ;
  mote.add(MOTE.MoteName);
  mote.add(MOTE.address);
  mote.add(MOTE.Longitude);
  mote.add(MOTE.Latitude);
return data;
}
void sendFrame(DynamicJsonDocument data)
{

uint8_t error ;//= xbee802.send(MOTE.AddDest,data);
    String a ;

uint8_t mod=getModCom(MOTE.modComm);
switch (mod){
  case 0 :
    serializeJson(data, Serial1);
    serializeJson(data, Serial);
    Serial.println();
  break; 
  case 1 : 
    serializeJson(data,a);
    //SENDDATA(a);//Send over MQTT
    Serial.println(a);
    Serial.println();
  break;
   
  default :
    serializeJson(data, Serial);
    Serial.println();
 
  break; 

    
}
/*
if( error == 0 )
{
//USB.println(F("send ok"));
}
else 
{
//USB.println(F("send error"));
}*/
}

//-------------------------EditSensor---------------------------------
void*EditSensor(char*name,uint8_t nbrs,char*type,uint8_t port,uint8_t EchSamp)
{ 
setNameSensor(name,nbrs);
setPortSensor(port,nbrs);
setTypeSensor(type,nbrs);
if(strcmp(type,"ALARME")==0)
{SetThreshold(EchSamp,nbrs);
setSamples(0 ,nbrs);
}
else if (strcmp(type,"REGULIER")==0)
{
SetThreshold(0 ,nbrs);
setSamples(EchSamp ,nbrs);
}
}
//--------------------ReadFrameConfigSensor---------
 DynamicJsonDocument CreateFrameConfigSensor(uint8_t n)
{ 
  DynamicJsonDocument doc(1024) ;

char*typ=ListSensor[n].type;
if(strcmp(typ,"ALARME")==0)
{ 
  doc["Sensor Name"]= ListSensor[n].SensorName;
  doc["Type"]= "ALARME";
  doc["Port"]= ListSensor[n].Port;
  doc["Threshold"]= ListSensor[n].Threshold;
  
}
else if(strcmp(typ,"REGULIER")==0)
{
  doc["Sensor Name"]= ListSensor[n].SensorName;
  doc["Type"]= "REGULIER";
  doc["Port"]= ListSensor[n].Port;
  doc["Sample"]= ListSensor[n].Samples;
}
return doc; 
} 


//-------------------------EditMote-----------------------
void*EditMote(char*name,char*address,char*Longitude,char*Latitude,uint8_t numSens,char*AddDest,char*moteGateway,char*modComm)
{
setName(name);
setaddress(address);
setLongitude(Longitude);
setLatitude(Latitude);
setSensNumber(numSens);
setAddDestMote(AddDest);
setMoteGateway(moteGateway);
setModComm(modComm);
}

DynamicJsonDocument CreateFrameConfigMote()
{
DynamicJsonDocument data(1024);
JsonArray mote = data.createNestedArray("Mote");
mote.add(MOTE.ID) ;
mote.add(MOTE.MoteName);
mote.add(MOTE.address);
mote.add(MOTE.Longitude);
mote.add(MOTE.Latitude);
mote.add(MOTE.SensorNumber);
mote.add(MOTE.AddDest);
mote.add(MOTE.MoteGateway);
mote.add(MOTE.modComm);
return data;
}
//-------------------------GetConfSensor-------------------------------
DynamicJsonDocument getConfSensor(uint8_t n)
{
  DynamicJsonDocument data(1024);
data=CreateFrameConfigSensor(n);
return data;
}//----------------------------GETConfMOTE--------------------
DynamicJsonDocument getMote()
{
DynamicJsonDocument data(1024);
data=CreateFrameConfigMote();
return data;
}
//-----------------------------GetValueData---------------------------------
DynamicJsonDocument getSensorValue(uint8_t ordre)
{
  DynamicJsonDocument data(1024);

data=CreateDataFrameSensor(ordre);
return data;
}

//--------------------CreateFrameConfigSensorS-----------------------------------
DynamicJsonDocument CreateFrameConfigSensors()
{   DynamicJsonDocument data(1024);
  
for(int n=0;n<6;n++){
  char*typ=ListSensor[n].type;
  JsonArray sensor = data.createNestedArray("sensor"+String(n));
if(strcmp(typ,"ALARME")==0)
{ 
  sensor.add(ListSensor[n].SensorName) ;
  sensor.add("ALARME") ;
  sensor.add(ListSensor[n].Port) ;
  sensor.add(ListSensor[n].Threshold) ;
  }
else if(strcmp(typ,"REGULIER")==0)
{
   sensor.add(ListSensor[n].SensorName) ;
  sensor.add("REGULIER") ;
  sensor.add(ListSensor[n].Port) ;
  sensor.add(ListSensor[n].Samples) ;
 
}
}
  return data;
}
//-------------------------------GetConfSensors-----------------------------------------
DynamicJsonDocument getSensors()
{
  DynamicJsonDocument data(1024);
  data=CreateFrameConfigSensors();
  return data;
}
DynamicJsonDocument levelBattery()
{
  DynamicJsonDocument doc(1024) ;
uint8_t levBatt=10;//(uint8_t) PWR.getBatteryLevel();
if(levBatt<25)
{
   doc["battery Lavel"]=levBatt;
   doc["ack"]="Etat critique veuillez alimenter votre RCSF le plus tot possible";
}
else { 
 doc["battery Lavel"]=levBatt;
 doc["ack"]="still working";
}
return doc;
}

//----------------------------------------------
DynamicJsonDocument CreateDataFrameSensor(uint8_t ordre)
{
    DynamicJsonDocument data(1024);
    data["Sensor Name"]=ListSensor[ordre].SensorName ;
    data["value"]=ListSensor[ordre].value ;
return data;
}

//---------------------GetVALUETHRESHOLD---------------------------------------------
DynamicJsonDocument getValueThreshold(uint8_t ordre)
{ 
  DynamicJsonDocument data(1024);

  //char*data="nothing";
if(strcmp(ListSensor[ordre].type,"ALARME")==0)
{
if(ListSensor[ordre].value > ListSensor[ordre].Threshold){
data=CreateDataFrameSensor(ordre);
}
}
return data;
}

DynamicJsonDocument CreateDataFrameSensors()
{ 
   DynamicJsonDocument data(1024);
   for(int i=0;i<6;i++){
      JsonArray sensor = data.createNestedArray("sensor"+String(i));
      sensor.add(ListSensor[i].SensorName);
      sensor.add(ListSensor[i].value);
   }
   return data;
}

//----------------------------getValueofAllSensors
DynamicJsonDocument getAllValue()
{
   DynamicJsonDocument data(1024);
   data=CreateDataFrameSensors();
   return data;
}

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
Serial.println("*******************");

Serial.println(codeFonction);
Serial.println(strcmp(codeFonction,"DISC"));
Serial.println("*******************");


//------------Modification de la configuration d'un capteur-----------------
if(mod==COMMANDE)
{
if(strcmp(codeFonction,"ES")==0)
{

EditSensor(Array[3],atoi(Array[4]),Array[5],atoi(Array[6]),atoi(Array[7]));
dataToSend=CreateFrameConfigSensor(atoi(Array[4]));
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
}
else if(strcmp(codeFonction,"DISC")==0)
{
dataToSend=Discovrey();
serializeJson(dataToSend,dd);
SENDDATA(dd);
sendFrame(dataToSend);
}
//-----------------Modification de la configuration d'une mote-----------------
else if(strcmp(codeFonction,"EM")==0)
{
EditMote(Array[3],Array[4],Array[5],Array[6],atoi(Array[7]),Array[8],Array[9],Array[10]);
dataToSend=CreateFrameConfigMote();
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
}

//------------Demande de configuration d'un capteur-----------------
else if(strcmp(codeFonction,"DC")==0)
{
dataToSend=getConfSensor(atoi(Array[3]));
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
}

//----------------------Demande de configuration d'une mote-----------------
else if(strcmp(codeFonction,"DM")==0)
{
dataToSend=getMote();
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
}
else if(strcmp(codeFonction,"DFC")==0)
{
DefaultConfigAll();
}
else if(strcmp(codeFonction,"DAC")==0){
dataToSend=getSensors();
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
}
//----------------------------Level Battery------------------------
else if(strcmp(codeFonction,"LB")==0)
{
  
dataToSend=levelBattery();
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
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
dataToSend=getValueThreshold(atoi(Array[3]));
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
}
else if(strcmp(codeFonction,"SV")==0)
{
dataToSend=getSensorValue(atoi(Array[3]));
//delay(atoi(Array[4]));
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
}

else if(strcmp(codeFonction,"AV")==0)
{
dataToSend=getAllValue();
//delay(atoi(Array[4]));
serializeJson(dataToSend,dd);
SENDDATA(dd);

sendFrame(dataToSend);
}
}
delay(3000);


}

void AlarmFrame()
{ 
DynamicJsonDocument data(1024);
JsonArray type = data.createNestedArray("Type");
type.add("Alarme");
JsonArray mote = data.createNestedArray("Mote");
mote.add(MOTE.ID) ;
mote.add(MOTE.MoteName);
mote.add(MOTE.address);
mote.add(MOTE.Longitude);
mote.add(MOTE.Latitude);
mote.add(MOTE.SensorNumber);
mote.add(MOTE.AddDest);
mote.add(MOTE.MoteGateway);
mote.add(MOTE.modComm);

for(uint8_t i=0;i<6;i++){

if(strcmp(ListSensor[i].type,"ALARME")==0)
{
if(ListSensor[i].value > ListSensor[i].Threshold){
  JsonArray sensor = data.createNestedArray("Sensor"+String(i));
  sensor.add(ListSensor[i].SensorName);
  sensor.add(ListSensor[i].value);
}
}
}  
sendFrame(data); 
}

void RegularFrame(){
DynamicJsonDocument data(1024);
JsonArray type = data.createNestedArray("Type");
type.add("Regular");

JsonArray mote = data.createNestedArray("Mote");
mote.add(MOTE.ID) ;
mote.add(MOTE.MoteName);
mote.add(MOTE.address);
mote.add(MOTE.Longitude);
mote.add(MOTE.Latitude);
mote.add(MOTE.SensorNumber);
mote.add(MOTE.AddDest);
mote.add(MOTE.MoteGateway);
mote.add(MOTE.modComm);
for(uint8_t i=0;i<6;i++){
if(strcmp(ListSensor[i].type,"REGULIER")==0)
{
if(ListSensor[i].Samples <= Delay ){
 JsonArray sensor = data.createNestedArray("Sensor"+String(i));
  sensor.add(ListSensor[i].SensorName);
  sensor.add(ListSensor[i].value);
previous = Delay+previous;
}
}
}

sendFrame(data); 
} 



void setup() {

Serial.begin(9600);
Serial1.begin(9600);
WIFICONNECTION() ;
ipS=getIp() ;

BEGINSERVER() ; 

delay(500);


DefaultConfigAll();
initSensor();

}


void loop() {
   
unsigned long current = millis();
Delay= current - previous;
EntenteCanal();

//sendFrame(s);
//Serial.println(s) ;
//char* alarmData;
//AlarmFrame();
//sendFrame(alarmData);

//delay(3000) ;
//AlarmFrame();
//char* regularData;

//regularData=
//RegularFrame();
delay(2000);

//Serial1.println() ;
//sendFrame(regularData);
//Serial.println("current :"+current ) ;
//Serial.println("previous :" + previous) ;

/*Serial.println(regularData) ;

////////////////////////////////////////////////
// Entering Deep Sleep mode
////////////////////////////////////////////////
//USB.println(F("Going to sleep..."));
//USB.println();
//PWR.deepSleep(sleepTime, RTC_OFFSET, RTC_ALM1_MODE1, ALL_OFF);

//USB.println(F("wake"));
*/
}
