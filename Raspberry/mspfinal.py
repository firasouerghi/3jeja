
import serial
import RPi.GPIO as GPIO
import os, time
import array
import sys
from tqdm import tqdm

cmd=[
    "<=>0013A20040B4DC64,0,DISC ","<=>0013A20040B4DC64,0,ES,dht11,1,ALARME,1,12 ",
    "<=>0013A20040B4DC64,0,EM,SensoeA,enit garden,est,ouest,3,192.168.8.20,raspberry,Serial ",
    "<=>0013A20040B4DC64,0,DFC ",
    "<=>0013A20040B4DC64,0,DAC " ,
    "<=>0013A20040B4DC64,0,LB "  ,
    "<=>0013A20040B4DC64,1,ST,1 ",
    "<=>0013A20040B4DC64,1,SV,1 " ,
    "<=>0013A20040B4DC64,1,AV " 
]
def SendFrame() :
    GPIO.setmode(GPIO.BOARD)
    port = serial.Serial()
    port.baudrate = 9600
    port.timeout = 0
    port.port = "/dev/ttyS0"
    try:
        # Ouverture du port serie
        port.open()
    except (serial.SerialException, e):
        # Le port serie ne s'ouvre pas
        sys.stderr.write("Impossible d'ouvrir le port serie %r: %s\n" % (port.port, e))
        sys.exit(1)
    print("###################################")
    command=int(input("#    give your command :          # \n# [0] to send a discovery Frame   #\n# [1] to send a confSensor Frame  # \n# [2] to send a confMote Frame    #\n# [3] to send a discovery Frame   # \n# [4] to send a discovery Frame   # \n# [5] to send a discovery Frame   # \n# [6] to send a discovery Frame   # \n# [7] to send a discovery Frame   # \n# [8] to send a discovery Frame   # \n# [9] to send a discovery Frame   # \n# [10]to send a discovery Frame   #\n###################################\n"))
    port.write(cmd[command].encode('ascii'))    
    print("Frame sent succsessfully ")
    port.close()

def ReadSerial() :
    GPIO.setmode(GPIO.BOARD)
    port = serial.Serial()
    port.baudrate = 9600
    port.timeout = 0
    port.port = "/dev/ttyS0"
    try:
        # Ouverture du port serie
        port.open()
    except (serial.SerialException, e):
        # Le port serie ne s'ouvre pas
        sys.stderr.write("Impossible d'ouvrir le port serie %r: %s\n" % (port.port, e))
        sys.exit(1)
    for i in tqdm(range(100)):
        time.sleep(0.03)
    rcv=str((port.readline()).decode('ascii'))
    print(("received data : {0} ").format(rcv))
    port.close()
    
if __name__=="__main__" :
    while(1) :
        SendFrame()
        ReadSerial()
        time.sleep(2)





