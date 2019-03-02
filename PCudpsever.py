



Reference="https://docs.python.org/2/library/json.html"

import socket
import time
import simplejson as json
from tqdm import tqdm

cmd=["<=>0013A20040B4DC64,0,DISC ","<=>0013A20040B4DC64,0,ES,dht11,1,ALARME,1,12 ","<=>0013A20040B4DC64,0,EM,SensoeA,enit garden,est,ouest,3,192.168.8.20,raspberry,Serial ","<=>0013A20040B4DC64,0,DC,0 ","<=>0013A20040B4DC64,0,DM ","<=>0013A20040B4DC64,0,DFC ","<=>0013A20040B4DC64,0,DAC ","<=>0013A20040B4DC64,0,LB ","<=>0013A20040B4DC64,1,ST,1 ","<=>0013A20040B4DC64,1,SV,1 ","<=>0013A20040B4DC64,1,AV "]

host='192.168.8.104'
port=8282

client= socket.socket(socket.AF_INET, socket.SOCK_STREAM)     
try:
    client.connect((host, port))
except socket.error as msg:
    print(msg)
print("Socket bind complete.")
  
while True:

  
    print("###################################")
    command=int(input("#    give your command :          # \n# [0] to send a discovery Frame   #\n# [1] to send a confSensor Frame  # \n# [2] to send a confMote Frame    #\n# [3] to send a discovery Frame   # \n# [4] to send a discovery Frame   # \n# [5] to send a discovery Frame   # \n# [6] to send a discovery Frame   # \n# [7] to send a discovery Frame   # \n# [8] to send a discovery Frame   # \n# [9] to send a discovery Frame   # \n# [10]to send a discovery Frame   #\n###################################\n->"))
    if(command==0) :
        client.send(str(command).encode('ASCII'))
    elif(command==1) :
        client.send(str(command).encode('ASCII'))
    elif(command==2) :
        client.send(str(command).encode('ASCII'))
    elif(command==3) :
        client.send(str(command).encode('ASCII'))
    elif(command==4) :
        client.send(str(command).encode('ASCII'))
    elif(command==5) :
        client.send(str(command).encode('ASCII'))
    elif(command==6) :
        client.send(str(command).encode('ASCII'))
    elif(command==7) :
        client.send(str(command).encode('ASCII'))
    elif(command==8) :
        client.send(str(command).encode('ASCII'))
    elif(command==9) :
        client.send(str(command).encode('ASCII'))
    else:
        client.send(str(command).encode('ASCII'))
    '''
    for i in tqdm(range(100)):
        time.sleep(0.1)  
    '''
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    # Set a timeout so the socket does not block
    # indefinitely when trying to receive data.
    server.settimeout(15)
    server.bind(("", 4000))  
    content, addr = server.recvfrom(1024)
    if not content:
        #break
        continue
    else:
        #parsed_json = json.loads(content)
        #print (json.dumps(parsed_json, sort_keys=True, indent=4, separators=(',', ': ')))
        #print(parsed_json['battery Lavel'])
        #print(parsed_json['ack'])
        print("from connected user: \n" )
        print(content)
        
    time.sleep(4)
