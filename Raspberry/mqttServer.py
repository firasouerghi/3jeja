
import time
import paho.mqtt.client as mqtt #import the client1import sys
import datetime
import smtplib, ssl
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import  json

import sqlite3
from sqlite3 import Error


smtp_server="smtp.gmail.com"
port = 587
sender_email = 'sensoralarmes@gmail.com'
password = 'ben3ar9a'
receiver_email = "firas.ouerghi@etudiant-enit.utm.tn"


enitBroker ='10.110.109.222'

homeBroker ='192.168.8.101'



SensorValues={'VIB':0,'PLUV':0,'TEMP':0,'HUMI':0,'GAZ':0}


def on_message(client, userdata, message):
    mqttmsg=(message.payload.decode('utf-8'))
    try :
        jsonData=json.loads(mqttmsg)
        #print(jsonData )
    except json.decoder.JSONDecodeError as json_error:
        print(json_error)
    if jsonData["Type"][0]=="Alarme" :
        print("Sending alarm mail")
        context = ssl.create_default_context()
        try:
            server = smtplib.SMTP(smtp_server,port)
            server.starttls(context=context) # Secure the connection
            server.login(sender_email, password)
            message = MIMEMultipart("alternative")
            message["Subject"] = "ALARM"
            message["From"] = sender_email
            message["To"] = receiver_email
            body= MIMEText(mqttmsg, "plain")
            message.attach(body)
            server.sendmail(sender_email, receiver_email, message.as_string())
            server.close()
        except Exception as e:
            # Print any error messages to stdout
            print(e)
        finally:
            server.quit() 
    elif jsonData["Type"][0]=="Regular" :    
        print("prossessing data") 
        for x in jsonData:
            if(x=='VIB'):
                if int(SensorValues['VIB'])== (int(jsonData[x][0])):
                    print("not inserting matching values ")
                else :    
                    print(jsonData[x][0])
                    try :
                        Connection = sqlite3.connect("data/vibration.db")
                        Cursor = Connection.cursor()
                        try :
                            Cursor.execute("INSERT INTO vibration VALUES(?,?)",(str(datetime.datetime.now()),jsonData[x][0],))
                        except sqlite3.ProgrammingError as e :
                            print(str(e))
                        Connection.commit()
                        Connection.close()
                    except sqlite3.DatabaseError as e :
                        print(str(e))
                    SensorValues['VIB']=jsonData[x][0]
            elif(x=='PLUV'):
                if int(SensorValues['PLUV'])==(int(jsonData[x][0])) :
                    print("not inserting matching values ")
                else :    
                    print(jsonData[x][0])
                    try :
                        Connection = sqlite3.connect("data/precipitation.db")
                        Cursor = Connection.cursor()
                        try :
                            Cursor.execute("INSERT INTO precipitation VALUES(?,?)",(str(datetime.datetime.now()),jsonData[x][0],))
                        except sqlite3.ProgrammingError as e :
                            print(str(e))
                        Connection.commit()
                        Connection.close()
                    except sqlite3.DatabaseError as e :
                        print(str(e))
                    SensorValues['PLUV']=jsonData[x][0]
            elif(x=='TEMP'):
                if int (SensorValues['TEMP'])==(int(jsonData[x][0])) :
                    print("not inserting matching values ")                
                else :
                    print(jsonData[x][0])
                    try :
                        Connection = sqlite3.connect("data/temperature.db")
                        Cursor = Connection.cursor()
                        try :
                            Cursor.execute("INSERT INTO temperature VALUES(?,?)",(str(datetime.datetime.now()),jsonData[x][0],))
                        except sqlite3.ProgrammingError as e :
                            print(str(e))
                        Connection.commit()
                        Connection.close()
                    except sqlite3.DatabaseError as e :
                        print(str(e))
                    SensorValues['TEMP']=jsonData[x][0]
            elif(x=='HUMI'):
                if int(SensorValues['HUMI'])==(int(jsonData[x][0])) :
                    print("not inserting matching values ")
                else :    
                    print(jsonData[x][0])
                    try :
                        Connection = sqlite3.connect("data/humidity.db")
                        Cursor = Connection.cursor()
                        try :
                            Cursor.execute("INSERT INTO humidity VALUES(?,?)",(str(datetime.datetime.now()),jsonData[x][0],))
                        except sqlite3.ProgrammingError as e :
                            print(str(e))
                        Connection.commit()
                        Connection.close()
                    except sqlite3.DatabaseError as e :
                        print(str(e))
                    SensorValues['HUMI']=jsonData[x][0]
            elif(x=='GAZ') :
                if int(SensorValues['GAZ'])==int(jsonData[x][0]) :
                    print("not inserting matching values ")
                else :    
                    print(jsonData[x][0])
                    try :
                        Connection = sqlite3.connect("data/gas.db")
                        Cursor = Connection.cursor()
                        try :
                            Cursor.execute("INSERT INTO gas VALUES(?,?)",(str(datetime.datetime.now()),jsonData[x][0],))
                        except sqlite3.ProgrammingError as e :
                            print(str(e))
                        Connection.commit()
                        Connection.close()
                    except sqlite3.DatabaseError as e :
                        print(str(e))
                    SensorValues['GAZ']=jsonData[x][0]
            else :
                print('nothing to be done check the code ')   
   
    else :
        pass
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)



def on_connect(client, userdata, flags, rc):
     logging.info("Connected flags"+str(flags)+"result code "\
     +str(rc)+"client1_id ")
     client.connected_flag=True       


def CreateDataBases() :
    db1 = sqlite3.connect('data/temperature.db')
    db1.close()
    db2 = sqlite3.connect('data/humidity.db')
    db2.close()
    db3 = sqlite3.connect('data/vibration.db')
    db3.close()
    db4 = sqlite3.connect('data/precipitation.db')
    db4.close()
    db5 = sqlite3.connect('data/gas.db')
    db5.close()

def createTables() :
    try :
        Connection = sqlite3.connect("data/temperature.db")
        Cursor = Connection.cursor()
        try :
            Cursor.execute("CREATE TABLE IF NOT EXISTS temperature( dataDate TEXT , Valeur float ) ") #SELECT DISTINCT column_list FROM table; (duplicated values)
            #Cursor.execute("INSERT INTO NODES VALUES(?,?,?)",(id,None,None,))
        except sqlite3.ProgrammingError as e :
            print(str(e))
        Connection.commit()
        Connection.close()
    except sqlite3.DatabaseError as e :
        print(str(e))
    time.sleep(0.2)   
    try :
        Connection = sqlite3.connect("data/humidity.db")
        Cursor = Connection.cursor()
        try :
            Cursor.execute("CREATE TABLE IF NOT EXISTS humidity ( dataDate TEXT , Valeur float ) ") #SELECT DISTINCT column_list FROM table; (duplicated values)
        except sqlite3.ProgrammingError as e :
            print(str(e))
        Connection.commit()
        Connection.close()
    except sqlite3.DatabaseError as e :
        print(str(e))
    time.sleep(0.2)    
    try :
        Connection = sqlite3.connect("data/vibration.db")
        Cursor = Connection.cursor()
        try :
            Cursor.execute("CREATE TABLE IF NOT EXISTS vibration( dataDate TEXT , Valeur float ) ") #SELECT DISTINCT column_list FROM table; (duplicated values)
            #Cursor.execute("INSERT INTO NODES VALUES(?,?,?)",(id,None,None,))
        except sqlite3.ProgrammingError as e :
            print(str(e))
        Connection.commit()
        Connection.close()
    except sqlite3.DatabaseError as e :
        print(str(e))
    time.sleep(0.2)   
    try :
        Connection = sqlite3.connect("data/gas.db")
        Cursor = Connection.cursor()
        try :
            Cursor.execute("CREATE TABLE IF NOT EXISTS gas( dataDate TEXT , Valeur float ) ") #SELECT DISTINCT column_list FROM table; (duplicated values)
            #Cursor.execute("INSERT INTO NODES VALUES(?,?,?)",(id,None,None,))
        except sqlite3.ProgrammingError as e :
            print(str(e))
        Connection.commit()
        Connection.close()
    except sqlite3.DatabaseError as e :
        print(str(e))
    time.sleep(0.2)    
    try :
        Connection = sqlite3.connect("data/precipitation.db")
        Cursor = Connection.cursor()
        try :
            Cursor.execute("CREATE TABLE IF NOT EXISTS precipitation ( dataDate TEXT , Valeur float ) ") #SELECT DISTINCT column_list FROM table; (duplicated values)
            #Cursor.execute("INSERT INTO NODES VALUES(?,?,?)",(id,None,None,))
        except sqlite3.ProgrammingError as e :
            print(str(e))
        Connection.commit()
        Connection.close()
    except sqlite3.DatabaseError as e :
        print(str(e))
        

CreateDataBases()
time.sleep(0.2)
createTables()
time.sleep(0.2)

print("creating new instance")
client = mqtt.Client("P1") #create new instance
client.tls_set('/home/heroku/Desktop/mqttServer/homecertification/ca.crt')
client.on_message=on_message #attach function to callback
print("connecting to broker")
client.connect('192.168.8.101',port=8883) #connect to broker
print("Subscribing to topic","/data")
client.subscribe("/data")
#time.sleep(400) # wait
while 1:
    client.loop_start() #stop the loop

#time.sleep(30000)

#client.loop_stop()
