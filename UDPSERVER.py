

__author__="OUERGHI FIRAS && ABAZA ANIS"
'''
PROTOCOL 3JEJA [ALL RIGHTS ARE RESERVED]!!!

'''

import socket
import time
import simplejson as json
from _thread import *
import threading 

#the command List that will be sent from my pc['the server'] to the raspberry pi and then the node  
cmd=["<=>0013A20040B4DC64,0,DISC","<=>0013A20040B4DC64,0,ES,dht11,1,ALARME,1,12","<=>0013A20040B4DC64,0,EM,SensoeA,enit garden,est,ouest,3,192.168.8.20,raspberry,wifi","<=>0013A20040B4DC64,0,DC,0","<=>0013A20040B4DC64,0,DM","<=>0013A20040B4DC64,0,DFC","<=>0013A20040B4DC64,0,DAC","<=>0013A20040B4DC64,0,LB","<=>0013A20040B4DC64,1,ST,1","<=>0013A20040B4DC64,1,SV,1","<=>0013A20040B4DC64,1,AV"]

def broadcast(CMD):
    server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.settimeout(5)
    server.bind(("", 2222))
    server.sendto(CMD.encode("ascii"), ('255.255.255.255',3333)) #All nodes listening on port 3333 wil receive the command CMD
    data, addr = server.recvfrom(1024)
    print("message received form : {} => {}".format(addr,data))
    server.sendto(data,('192.168.8.100',4000))#192.168.8.100 is the pc ip address
    #while True :
    #    start_new_thread(receive,(server),) 

def receive(server):
    data, addr = server.recvfrom(1024)
    print("message received form : {} => {}".format(addr,data))
    server.sendto(data,('192.168.8.100',4000))




def RequestHandler(client) :
    while 1:
        command =str((client.recv(1024)).decode("ASCII"))
        print(command)
        if not command:
            continue
        else :   
            broadcast(cmd[int(command)])
            







def main(host,port) :
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)     
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)     
    try:
        s.bind((host, port))
    except socket.error as msg:
        print(msg)
    print("Socket bind complete.")
    s.listen(100)
    while True:
        client, address = s.accept()
        print("#########################################################")
        print("# Connected to: " + address[0] + ":" + str(address[1])+'#')
        print("#########################################################")
        start_new_thread(RequestHandler,(client,))


if __name__=="__main__" :
    host='0.0.0.0'
    port=8282
    main(host,port)