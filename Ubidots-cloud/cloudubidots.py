# ubidots ref : https://github.com/ubidots/ubidots-python 

import serial
import RPi.GPIO as GPIO
import os, time
import array
import sys
#import simplejson as json
import json
from ubidots import ApiClient
import time



api = ApiClient(token='A1E-FN2Qg1o10ILgqMTejPJrHDUSga1HTh') #update token

my_temp = api.get_variable('5cb8fc9fc03f973bee816f37') #update variable ID
my_hum = api.get_variable('5cb90357c03f9740e43fa516')
my_gaz= api.get_variable('5cb90385c03f9741783b6087')
my_vibration=api.get_variable('5cb90400c03f9741f56a696a')
my_pluv=api.get_variable('5cb9042bc03f9741f56a6979')
var=api.get_variable('5cb9432ec03f9773083ed32c')



GPIO.setmode(GPIO.BOARD)
port = serial.Serial()
port.baudrate = 9600
port.timeout = 0.05
port.port = "/dev/ttyUSB0"
try:
   # Ouverture du port serie
    port.open()
    time.sleep(0.2)


except (serial.SerialException, e):
    # Le port serie ne s'ouvre pas
    sys.stderr.write("Impossible d'ouvrir le port serie %r: %s\n" % (port.port, e))
    sys.exit(1)

print("Port Open")


SensorValues={'VIB':0,'PLUV':0,'TEMP':0,'HUMI':0,'GAZ':0}


while(1) :
        rcv=str((port.readline()).decode('utf-8'))
        if(rcv) :
            try :
                jsonData=json.loads(rcv)
            except json.decoder.JSONDecodeError as json_error:
                print(json_error)
            else :
                if (jsonData is not None) :
                    for x in jsonData:
                        if(x=='VIB'):
                            SensorValues['VIB']=(int(jsonData[x][0]))
                        elif(x=='PLUV'):
                            SensorValues['PLUV']=(float(jsonData[x][0]))
                        elif(x=='TEMP'):
                            SensorValues['TEMP']=(float(jsonData[x][0]))
                        elif(x=='HUMI'):
                            SensorValues['HUMI']=(int(jsonData[x][0]))
                        elif(x=='GAZ') :
                            SensorValues['GAZ']=(int(jsonData[x][0]))
                        else :
                            print('nothing to be done check the code ')   
                    try :         
                        my_temp.save_value({'value':float(SensorValues['TEMP'])})
                        time.sleep(0.1)
                    except ValueError:  # includes json.decoder.JSONDecodeError
                        print ('Decoding TEM JSON has failed')
                    try :    
                        my_hum.save_value({'value': int(SensorValues['HUMI'])})
                        time.sleep(0.1)
                    except ValueError: 
                        print ('Decoding hum JSON has failed')
                    try :    
                        my_gaz.save_value({'value': int(SensorValues['GAZ'])})
                        time.sleep(0.1)
                    except ValueError: 
                        print ('Decoding GAZ JSON has failed')    
                    try :
                        my_vibration.save_value({'value': int(SensorValues['VIB'])})
                        time.sleep(0.1)
                    except ValueError: 
                        print ('Decoding VIB JSON has failed')
                    try :    
                        my_pluv.save_value({'value': float(SensorValues['PLUV'])})
                        time.sleep(0.1)
                    except ValueError: 
                        print ('Decoding PLUV JSON has failed')
                    print(SensorValues)




