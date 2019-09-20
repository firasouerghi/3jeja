
#ifndef NODE_H
#define NODE_H
#include <ArduinoJson.h>


#include<Energia.h>
#include "Sensor.h"
#include<stdio.h>
#include "DHT.h"
/****dht11 conf ****/
#define DHTPIN 38

#define DHTTYPE DHT11


/****dht11 conf ****/

#define DEFAULT_SENSOR_NUMBER 6  // each Node has 6  sensors by default 
#define RANDOM_ID random(1000000000,9999999999)


class Node {

  public :

    //Attributes      
    char * ID;
    char * MoteName;
    char * address;
    float Longitude;
    float Latitude; 
    uint8_t SensorNumber=DEFAULT_SENSOR_NUMBER; 
    char * AddDest;
    char * MoteGateway;
    Sensor * sensors;
    char * modComm;



    /*****Default Constructor*****/
    
    Node();
    
    /*****Parameterized Constructor*****/
    
    Node(char *id,char *name,char * address,float lgt,float lat,char *adddest,char * Ng,char * modComm);

    /*****Destructor*****/

    ~Node() ;

    /*****Getters*****/
    char * getId() ;
    char * getMoteName() ;
    char * getAddress() ;
    float getLongitude() ;
    float getLatitude(); 
    uint8_t getSensorNumber(); 
    char * getAddDest();
    char * getMoteGateway();
    Sensor * getSensors();
    char * getModComm();


    /*****Setters*****/
    
    void setId(char * id ) ;
    void setMoteName(char * name) ;
    void setAddress(char * address) ;
    void setLongitude(float lgt) ;
    void setLatitude(float lat); 
    void setSensorNumber(uint8_t sn); 
    void setAddDest(char * adddest);
    void setMoteGateway(char *Ng);
    void setSensors(Sensor * sensorTab);
    void setModComm(char * modComm);

       
       /*****initialize sensors vlaue to 0 *****/
    
    void initSensors() ;


      /*****Set Sensor Port by Order *****/

    void setSensorPort(uint8_t analog,uint8_t order);

      /*****Set Sensor Type by Order *****/

    void setSensorType(char * type , uint8_t order) ;


    /*****Set Sensor Threeshold by Order *****/

    void  setSensorThreshold(uint8_t Threshold , uint8_t order);

    /*****Set Sensor Sample by Order *****/

    void setSensorSample(uint8_t samples , uint8_t order);

    /*****Set Sensor Name by Order *****/

    void setSensorName(char*SensName , uint8_t order);

    /*****Set Sensor Value by Order *****/

    void setSensorValue(float value , uint8_t order);


      /*****Change Sensor Order *****/
      
    void ChangeOrder(uint8_t newOrder , uint8_t oldOrder) ;



      /*****Configure Sensor's Node *****/

    void  ConfSensor(char*name,char*type,uint8_t EchSamp,uint8_t analogout,uint8_t order) ;
     
      
      /*****Default Configuration *****/

    void DefaultConfigAll() ;

      /***** create a random ID *****/

    char *  getRandomId();

      /***** Read Sensor's Value *****/


    float readAnemometer() ;

    float readPluviometrie() ;

    float readVane() ;

    float readTemperature() ;  

    float readHumidity() ;

    float readRadiationSolar() ;

    float readSoilHumidity() ;

        
       /***** Initilizing Sensor's Values *****/      
    
    
    void initSensor() ;
      
   /***** Response from node to a discovery *****/


    DynamicJsonDocument Discovrey() ;


   /*****    Edit Sensor *****/
   

    void  EditSensor(char*name,uint8_t nbrs,char*type,uint8_t port,uint8_t EchSamp) ;

     
  /***** Create Frame ConfigSensor *****/

      
   DynamicJsonDocument CreateFrameConfigSensor(uint8_t n) ;

  
  /***** Edit Mote *****/


   void  EditMote(char*name,char*address,float Longitude,float Latitude,uint8_t numSens,char*AddDest,char*moteGateway,char*modComm) ;


   
    /***** Create Configuration Frame *****/


    DynamicJsonDocument CreateFrameConfigMote() ;

    

    /***** Get Sensors Configuration *****/


    DynamicJsonDocument getConfSensor(uint8_t n) ;
    
    
    /***** Get Node Configuration *****/


    DynamicJsonDocument getMote() ;


      /*****  Get Sensors Values *****/



    DynamicJsonDocument getSensorValue(uint8_t ordre) ;

      /*****Create Sensors Configuration  Frame *****/


    DynamicJsonDocument CreateFrameConfigSensors() ;



      /***** Get Sensor Configuration *****/


    DynamicJsonDocument getSensorsConf() ;

      /***** get Battery level *****/

    DynamicJsonDocument levelBattery() ;

    
    /***** Create  Sensor data frame *****/

    
      DynamicJsonDocument CreateDataFrameSensor(uint8_t order) ;
      
      /*****  Get Threshold Value *****/


      DynamicJsonDocument getValueThreshold(uint8_t ordre) ;

      
      /***** create data frame *****/

 
      DynamicJsonDocument CreateDataFrameSensors() ;

            /*****get Value of All Sensors *****/
            

      DynamicJsonDocument getAllValue() ;



      
        /***** Create regular data frame *****/


      DynamicJsonDocument   RegularFrame(unsigned long Delay,unsigned long previous,Sensor * sensorBackup);


      
        /***** Create alarm data frame *****/



    DynamicJsonDocument  AlarmFrame() ;






};

 
#endif
