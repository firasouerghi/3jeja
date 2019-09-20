#include "Node.h"
    /*****Default constructor*****/
DHT dht(DHTPIN, DHTTYPE);
Node::Node(){
    this->ID="blank";
    this->MoteName="blank" ;
    this->address="blank" ;
    this->Longitude=0.00 ;
    this->Latitude=0.00;
    this->SensorNumber=0 ;
    this->AddDest="blank" ;
    this->MoteGateway="blank" ;
    this->sensors=(Sensor *)malloc(DEFAULT_SENSOR_NUMBER * sizeof(Sensor)) ;
    this->modComm="wifi" ;
  }
  
    /*****Parameterized Constructor*****/

Node :: Node(char *id,char *name,char * address,float lgt,float lat,char *adddest,char * Ng,char * modComm)
{
    this->ID=id;
    this->MoteName=name ;
    this->address=address ;
    this->Longitude=lgt ;
    this->Latitude=lat;
    this->SensorNumber=DEFAULT_SENSOR_NUMBER ;
    this->AddDest=adddest ;
    this->MoteGateway=Ng ;
    this->sensors=(Sensor *)malloc(DEFAULT_SENSOR_NUMBER * sizeof(Sensor)) ;
    this->modComm=modComm ;
    
}

    /*****Destructor*****/

Node :: ~Node() {
        free(this->sensors);
      }
    
    /*****Getters*****/


char * Node :: getId(){
    return(this->ID);
}
char * Node:: getMoteName() {
    return(this->MoteName) ;
}
char * Node:: getAddress(){
    return(this->address) ; 
}
float Node:: getLongitude() 
{
     return(this->Longitude) ;
}
float Node:: getLatitude(){
    return(this->Latitude) ;   
}
uint8_t Node:: getSensorNumber(){
    return(this->SensorNumber) ;  
}
char * Node:: getAddDest(){
    return(this->AddDest) ;  
}
char * Node:: getMoteGateway(){
    return(this->MoteGateway) ;  
}
Sensor * Node:: getSensors(){
    return(this->sensors) ;
}
char * Node:: getModComm(){
    return(this->modComm) ;
}

    /*****Setters*****/
    
void  Node ::  setId(char *  id ) {
  this->ID=id;
  
}
void  Node ::  setMoteName(char *  name) {
   this->MoteName=name ;
   
}
void  Node ::  setAddress(char *  address) {
 this->address=address ;
  
}
void Node :: setLongitude(float lgt) {
  this->Longitude=lgt ;
  
}
void Node :: setLatitude(float lat){
  this->Latitude=lat;
  
}
void Node :: setSensorNumber(uint8_t sn){
  this->SensorNumber=sn ;
   
}
void  Node ::  setAddDest(char *  adddest){
  this->AddDest=adddest ;
  
}
void  Node ::  setMoteGateway(char * Ng){
  this->MoteGateway=Ng ;
  
}
void Node :: setSensors(Sensor * sensorTab){
  this->sensors=sensorTab ;
  
}
void  Node ::  setModComm(char *  modComm){
  this->modComm=modComm ;
  
}

   /*****initialize sensors vlaue to 0 *****/
    
void Node :: initSensors() {

  for(uint8_t i=0;i<DEFAULT_SENSOR_NUMBER;i++){
    ((this->sensors)[i]).setValue(0);  
    }
}

   /*****Set Sensor Port by Order *****/

void Node :: setSensorPort(uint8_t analog,uint8_t order){
  
  ((this->sensors)[order]).setPort(analog);
}

    /*****Set Sensor Type by Order *****/

void Node :: setSensorType(char * type , uint8_t order){

    ((this->sensors)[order]).setType(type);

}

    /*****Set Sensor Threeshold by Order *****/

void Node :: setSensorThreshold(uint8_t Threshold , uint8_t order){

    ((this->sensors)[order]).setThreshold(Threshold );

}

    /*****Set Sensor Sample by Order *****/

void Node :: setSensorSample(uint8_t samples , uint8_t order){

    ((this->sensors)[order]).setSamples(samples);

}

    /*****Set Sensor Name by Order *****/

void Node :: setSensorName(char*SensName , uint8_t order){

    ((this->sensors)[order]).setSensorName(SensName);

}
    /*****Set Sensor Value by Order *****/

void Node :: setSensorValue(float value , uint8_t order){

    ((this->sensors)[order]).setValue(value);

}

      /*****Change Sensor Order *****/
      
void Node :: ChangeOrder(uint8_t newOrder , uint8_t oldOrder){

  
    ((this->sensors)[oldOrder]).setOrder(newOrder);
         
}

      /*****Configure Sensor's Node *****/

void Node :: ConfSensor(char*name,char*type,uint8_t EchSamp,uint8_t analogout,uint8_t order) {
    setSensorPort(analogout,order);
    setSensorType(type,order);
    if(strcmp(type,"ALARME")==0){
        setSensorThreshold(EchSamp,order);
    }
    else {
        setSensorSample(EchSamp,order);
    }
    setSensorName(name,order);
    ChangeOrder(order,order);
}
    
      /***** create a random ID *****/
      
char * Node :: getRandomId(){
  
    String aleas =String(RANDOM_ID) ;
    int l=aleas.length()+1 ;
    char * randomId =(char *)malloc(l*sizeof(char)) ;
    aleas.toCharArray(randomId,l);
    return(randomId) ;
    free(randomId) ;
    
}

      /*****Default Configuration *****/

void Node :: DefaultConfigAll()
{

    dht.begin();
  
  
  /*****Sensors*****/
  ConfSensor("ANE","REGULIER",20,1,0);
  ConfSensor("Vane","ALARME",25,2,1);
  ConfSensor("PLUV","REGULIER",10,3,2);
  ConfSensor("S_TEMP","ALARME",45,4,3);
  ConfSensor("HUMI","ALARME",45,5,4);
  ConfSensor("S_Rad","ALARME",45,6,5);
  
  
  /*****Node*****/

  
  setId(getRandomId()) ;
  setMoteName("Node") ;
  setAddress("address") ;
  setLongitude(random(0,100)) ; 
  setLatitude(random(0,100)) ;
  setSensorNumber(DEFAULT_SENSOR_NUMBER);
  setAddDest("DestAddress");
  setMoteGateway("10.110.108.248");
  //setSensors(Sensor * sensorTab) ;
  setModComm("wifi") ;

}


      /***** Read Sensor's Value *****/
float Node :: readAnemometer()
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

float Node :: readPluviometrie()
{
    float pluviometrie=87.5;
    //pluviometrie= SensorAgrv20.readPluviometerCurrent();
    return pluviometrie;
}


float Node ::  readVane()
{
    float vane=45.5;
    //SensorAgrv20.ON();
    //RTC.ON();
    //int vane = SensorAgrv20.readValue(SENS_AGR_VANE);
    //return SensorAgrv20.vaneDirection;
    return vane;
}
float Node :: readTemperature()
{ 
     float value=0.0;
     value=dht.readTemperature();
     if (isnan(value)) {
   	 Serial.println(F("Failed to read tem from DHT sensor!"));
	return (0.0) ;
     }
     else
     return value;
}

float Node :: readHumidity()
{ 
    float value=0.0;
    value=dht.readHumidity();
    if (isnan(value)) {
   	 Serial.println(F("Failed to read hum from DHT sensor!"));
         return(0.0) ;
     }
    else 
     return value;
}
float Node :: readRadiationSolar()
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
float Node :: readSoilHumidity()
{ 
    float value=12.2; 
    // SensorAgrv20.setSensorMode(SENS_ON, SENS_AGR_WATERMARK_1);
    // delay(100);
    // Read the Watermark 1 sensor 
    // value = SensorAgrv20.readValue(SENS_AGR_WATERMARK_1);
    // Turn off the sensor
    // SensorAgrv20.setSensorMode(SENS_OFF, SENS_AGR_WATERMARK_1);
    return value;
}


        
      /***** Initilizing Sensor's Values *****/      

void Node :: initSensor()
{
    setSensorValue( readAnemometer() , 0) ;
    setSensorValue( readVane() , 1) ;
    setSensorValue( readPluviometrie(), 2) ;
    setSensorValue( readTemperature(), 3) ;
    setSensorValue(readHumidity() ,4);//readSoilHumidity(), 4) ;
    setSensorValue( readRadiationSolar(), 5) ;
   
}

  
   /***** Response from node to a discovery *****/


DynamicJsonDocument Node :: Discovrey()
{

    DynamicJsonDocument data(1024);
    JsonArray ack = data.createNestedArray("discovery");
    ack.add("acknowledgment from ") ;
    JsonArray mote = data.createNestedArray("Mote");
    mote.add(this->ID) ;
    mote.add(this->MoteName);
    mote.add(this->address);
    mote.add(this->Longitude);
    mote.add(this->Latitude);
    
    return data;

}




        /*****    Edit Sensor *****/


void  Node :: EditSensor(char*name,uint8_t nbrs,char*type,uint8_t port,uint8_t EchSamp)
{ 
    setSensorName(name,nbrs);
    setSensorPort(port,nbrs);
    setSensorType(type,nbrs);
    if(strcmp(type,"ALARME")==0){
        setSensorThreshold(EchSamp,nbrs); 
        setSensorSample(0 ,nbrs);
    }
else if (strcmp(type,"REGULIER")==0){
        setSensorThreshold(0 ,nbrs);
        setSensorSample(EchSamp ,nbrs);
    }
}



      /***** Create Frame ConfigSensor *****/

DynamicJsonDocument Node :: CreateFrameConfigSensor(uint8_t n)
{ 
    
    DynamicJsonDocument doc(1024) ;
    char * typ=((this->sensors)[n]).getType();
 
    if(strcmp(typ,"ALARME")==0)
    { 
        doc["Sensor Name"]= ((this->sensors)[n]).getSensorName();
        doc["Type"]= "ALARME";
        doc["Port"]= ((this->sensors)[n]).getPort() ;
        doc["Threshold"]= ((this->sensors)[n]).getThreshold();
    }
else if(strcmp(typ,"REGULIER")==0){
  
      doc["Sensor Name"]=  ((this->sensors)[n]).getSensorName();
      doc["Type"]= "REGULIER";
      doc["Port"]= ((this->sensors)[n]).getPort();
      doc["Sample"]= ((this->sensors)[n]).getSamples();
}
      return doc; 
} 



          /***** Edit Mote *****/


void  Node :: EditMote(char*name,char*address,float Longitude,float Latitude,uint8_t numSens,char*AddDest,char*moteGateway,char*modComm)
{
    setMoteName(name);
    setAddress(address);
    setLongitude(Longitude);
    setLatitude(Latitude);
    setSensorNumber(numSens);
    setAddDest(AddDest);
    setMoteGateway(moteGateway);
    setModComm(modComm);
}


    /***** Create Configuration Frame *****/


DynamicJsonDocument Node :: CreateFrameConfigMote()
{

    DynamicJsonDocument data(1024);
    JsonArray mote = data.createNestedArray("Mote");
    mote.add(this->ID) ;
    mote.add(this->MoteName);
    mote.add(this->address);
    mote.add(this->Longitude);
    mote.add(this->Latitude);
    mote.add(this->SensorNumber);
    mote.add(this->AddDest);
    mote.add(this->MoteGateway);
    mote.add(this->modComm);
    
    return data;
}





        /***** Get Sensors Configuration *****/


DynamicJsonDocument Node :: getConfSensor(uint8_t n)
{
    DynamicJsonDocument data(1024);
    data=CreateFrameConfigSensor(n);
    return data;
}


      /***** Get Node Configuration *****/


DynamicJsonDocument Node :: getMote()
{
    DynamicJsonDocument data(1024);
    data=CreateFrameConfigMote();
    return data;
}


      /*****  Get Sensors Values *****/



DynamicJsonDocument Node :: getSensorValue(uint8_t ordre)
{
    DynamicJsonDocument data(1024);
    data=CreateDataFrameSensor(ordre);
    return data;
}
  
      /*****Create Sensors Configuration  Frame *****/


DynamicJsonDocument Node :: CreateFrameConfigSensors()
{   
    DynamicJsonDocument data(1024); 
    for(int n=0;n< getSensorNumber() ;n++){
      
        char * typ=((this->sensors)[n]).getType(); 
        JsonArray sensor = data.createNestedArray("sensor"+String(n));
   
    if(strcmp(typ,"ALARME")==0)
    { 
        sensor.add(((this->sensors)[n]).getSensorName()) ;
        sensor.add("ALARME") ;
        sensor.add(((this->sensors)[n]).getPort()) ;
        sensor.add(((this->sensors)[n]).getThreshold()) ;
    }
    else if(strcmp(typ,"REGULIER")==0)
    {
        sensor.add(((this->sensors)[n]).getSensorName()) ;
        sensor.add("REGULIER") ;
        sensor.add(((this->sensors)[n]).getPort()) ;
        sensor.add(((this->sensors)[n]).getSamples()) ;
    }
}
    return data;
}


      /***** Get Sensor Configuration *****/


DynamicJsonDocument Node :: getSensorsConf() // !!!!!!!!!!!!!!!
{
    DynamicJsonDocument data(1024);
    data=CreateFrameConfigSensors();
    return data;
}

      /***** get Battery level *****/

DynamicJsonDocument Node :: levelBattery()
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

    /***** Create  Sensor data frame *****/

    
DynamicJsonDocument Node :: CreateDataFrameSensor(uint8_t order)
{
      DynamicJsonDocument data(1024);
      data["Sensor Name"]=((this->sensors)[order]).getSensorName() ;
      data["value"]=((this->sensors)[order]).getValue();
      return data;
}

      /*****  Get Threshold Value *****/


DynamicJsonDocument Node :: getValueThreshold(uint8_t order)
{ 
    DynamicJsonDocument data(1024);
    if(strcmp(((this->sensors)[order]).getType(),"ALARME")==0)
    {
      if( ((this->sensors)[order]).getValue() >((this->sensors)[order]).getThreshold() ){
          data=CreateDataFrameSensor(order);
      } 
    }
    return data;
}

      /***** create data frame *****/

 
DynamicJsonDocument Node :: CreateDataFrameSensors()
{ 
    DynamicJsonDocument data(1024);
    for(int i=0;i<getSensorNumber();i++){
        JsonArray sensor = data.createNestedArray("sensor"+String(i));
        sensor.add(((this->sensors)[i]).getSensorName());
        sensor.add(((this->sensors)[i]).getValue());
    }
    return data;
}

      /*****get Value of All Sensors *****/



DynamicJsonDocument Node :: getAllValue()
{
   DynamicJsonDocument data(1024);
   data=CreateDataFrameSensors();
   return data;
}

        /***** Create regular data frame *****/


DynamicJsonDocument Node ::  RegularFrame(unsigned long Delay,unsigned long previous,Sensor * sensorBackup){

  String tosend ;
  DynamicJsonDocument data(1024);
  JsonArray type = data.createNestedArray("Type");
  type.add("Regular");
  JsonArray mote = data.createNestedArray("Mote");
  mote.add(getId()) ;
  mote.add(getMoteName());
  mote.add(getAddress());
  mote.add(getLongitude());
  mote.add(getLatitude());
  mote.add(getSensorNumber());
  mote.add(getAddDest());
  mote.add(getMoteGateway());
  mote.add(getModComm());
  for(uint8_t i=0;i<getSensorNumber();i++){
      if(strcmp(((getSensors())[i]).getType(),"REGULIER")==0)
      {
		if (((getSensors())[i]).getSamples() <= Delay){
			if((sensorBackup[i]).getValue() != ((getSensors())[i]).getValue()){
				JsonArray sensor = data.createNestedArray("Sensor"+String(i));
          			sensor.add(((getSensors())[i]).getSensorName());
          			sensor.add(((getSensors())[i]).getValue());
				(sensorBackup[i]).setValue(((getSensors())[i]).getValue());
				previous = Delay+previous;
                                
                         }
		}
      }
  }
  return(data) ;
} 


        /***** Create alarm data frame *****/



DynamicJsonDocument Node :: AlarmFrame()
{ 
    String tosend ;
    DynamicJsonDocument data(1024);
    JsonArray type = data.createNestedArray("Type");
    type.add("Alarme");
    JsonArray mote = data.createNestedArray("Mote");
    mote.add(getId()) ;
    mote.add(getMoteName());
    mote.add(getAddress());
    mote.add(getLongitude());
    mote.add(getLatitude());
    mote.add(getSensorNumber());
    mote.add(getAddDest());
    mote.add(getMoteGateway());
    mote.add(getModComm());
    for(uint8_t i=0;i<getSensorNumber();i++){
    if(strcmp(((getSensors())[i]).getType(),"ALARME")==0)
    {
        if(((getSensors())[i]).getValue() >((getSensors())[i]).getThreshold()){
        JsonArray sensor = data.createNestedArray("Sensor"+String(i));
        sensor.add(((getSensors())[i]).getSensorName());
        sensor.add(((getSensors())[i]).getValue());
        }
    }
    }  
    return(data) ;
}
