#include "Sensor.h"


    /*****Default Constructor*****/

Sensor :: Sensor(){
  this->SensorName="blank" ;
  this->Port=Port=0;
  this->type=type="blank" ;
  this->Threshold=0 ;
  this->Samples=0 ;
  this->value=0.00 ;
  this->order=random(0,10);
  }

    /*****Parameterized Constructor*****/

Sensor :: Sensor(char * SensorName, uint8_t Port,char * type,uint8_t Threshold,uint8_t Samples,float value,    uint8_t order ){
  this->SensorName=SensorName ;
  this->Port=Port ;
  this->type=type ;
  this->Threshold=Threshold ;
  this->Samples=Samples ;
  this->value=value ;
  this->order=order;
}

    /*****Destructor*****/

    Sensor :: ~Sensor(){}




   /*****Getters*****/

    char * Sensor ::  getSensorName(){
      return(this->SensorName) ;
    }
    uint8_t Sensor ::  getPort(){
      return(this->Port) ;
    }
    char * Sensor ::  getType(){
      return(this->type) ;
    }
    uint8_t Sensor ::  getThreshold(){
      return(this->Threshold) ;
    }
    uint8_t Sensor ::  getSamples(){
      return(this->Samples) ;
    }
    float Sensor ::  getValue(){
      return(this->value) ;
    }
    uint8_t Sensor :: getOrder(){
      return(this->order) ;
    }

   
 /*****Setters*****/

    void Sensor ::  setSensorName(char * SensorName){
      this->SensorName=SensorName ;
  
      
    }
    void Sensor ::  setPort(uint8_t Port){
      this->Port=Port ;
  
    }
    void Sensor ::  setType(char * type){
      this->type=type ;
      
    }
    void Sensor ::  setThreshold(uint8_t Threshold){
      this->Threshold=Threshold ;
      
    }
    void Sensor ::  setSamples(uint8_t Samples){
      this->Samples=Samples ;    
    }
    void Sensor ::  setValue(float value){
      this->value=value ;
  
    }
    void Sensor :: setOrder(uint8_t order){
      this->order=order ;
    }
