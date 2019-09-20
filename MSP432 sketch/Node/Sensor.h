
#ifndef SENSOR_H
#define SENSOR_H
#include<Energia.h>




class Sensor{

  private :

  

  
  public :

    /*****Attributs*****/
    
    char * SensorName;
    uint8_t Port;
    char * type;
    uint8_t Threshold;//soit pour stocker var ou pour temps de sleep
    uint8_t Samples;
    float value;
    uint8_t order;

    /*****Default Constructor*****/
    
    Sensor() ;
    
    /*****Parameterized Constructor*****/

    Sensor(char * SensorName, uint8_t Port,char * type,uint8_t Threshold,uint8_t Samples,float value ,    uint8_t order);

    /*****Destructor*****/

    ~Sensor();

    /*****Getters*****/

    char * getSensorName();
    uint8_t getPort();
    char * getType();
    uint8_t getThreshold();
    uint8_t getSamples();
    float getValue();
    uint8_t getOrder() ;

    
    /*****Setters*****/

    void setSensorName(char * SensorName);
    void setPort(uint8_t Port);
    void setType(char * type);
    void setThreshold(uint8_t Threshold);
    void setSamples(uint8_t Samples);
    void setValue(float value);
    void setOrder(uint8_t order);
    
};





#endif
