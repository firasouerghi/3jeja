__author__="OUERGHI FIRAS && ABAZA ANIS"
'''
PROTOCOL 3JEJA [ALL RIGHTS ARE RESERVED]!!!

'''

import socket
import time
import simplejson as json
from _thread import *
import threading 

import time
import paho.mqtt.client as paho

import sys
import datetime

from twisted.internet import defer  
from twisted.internet.protocol import DatagramProtocol
from twisted.internet import reactor
from twisted.python import log

import txthings.resource as resource
import txthings.coap as coap

PAYLOAD ="<=>0013A20040B4DC64,0,DISC"  

class CounterResource (resource.CoAPResource):
    """
 Example Resource which supports only GET method. Response is a
    simple counter value.

    Name render_<METHOD> is required by convention. Such method should
    return a Deferred. If the result is available immediately it's best
    to use Twisted method defer.succeed(msg).
    """
   #isLeaf = True

    def __init__(self, start=0):
        resource.CoAPResource.__init__(self)    
        self.counter = start
        self.visible = True
        self.addParam(resource.LinkParam("title", "Counter resource"))

    def render_GET(self, request):
        response = coap.Message(code=coap.CONTENT, payload='%d' % (self.counter,))
        self.counter += 1
        return defer.succeed(response)


class BlockResource (resource.CoAPResource):
    """
    Example Resource which supports GET, and PUT methods. It sends large
    responses, which trigger blockwise transfer (>64 bytes for normal
    settings).

    As before name render_<METHOD> is required by convention.
    """
    #isLeaf = True

    def __init__(self):
        resource.CoAPResource.__init__(self)
        self.visible = True
    def render_GET(self, request):
        global PAYLOAD
        #payload="0"
        response = coap.Message(code=coap.CONTENT, payload=PAYLOAD)
        #client= paho.Client("client-001") 
        #client.connect('127.0.0.1')#connect
        #client.publish("/data",str(request.payload))#publish
        PAYLOAD="0"
        return defer.succeed(response)

    def render_PUT(self, request):
        log.msg('PUT payload: %s', request.payload)
        payload = "put"
        response = coap.Message(code=coap.CHANGED, payload=payload)
        client= paho.Client("client-001") 
        client.connect('127.0.0.1')#connect
        client.publish("/data",str(request.payload))#publish
        return defer.succeed(response)


class SeparateLargeResource(resource.CoAPResource):
    """
    Example Resource which supports GET method. It uses callLater
    to force the protocol to send empty ACK first and separate response
    later. Sending empty ACK happens automatically after coap.EMPTY_ACK_DELAY.
    No special instructions are necessary.

    Notice: txThings sends empty ACK automatically if response takes too long.

    Method render_GET returns a deferred. This allows the protocol to
    do other things, while the answer is prepared.

    Method responseReady uses d.callback(response) to "fire" the deferred,
    and send the response.
    """
    def __init__(self):
        resource.CoAPResource.__init__(self)
        self.visible = True
        self.addParam(resource.LinkParam("title", "Large resource."))

    def render_GET(self, request):
        d = defer.Deferred()
        reactor.callLater(3, self.responseReady, d, request)
        return d

    def responseReady(self, d, request):
        log.msg('response ready. sending...')
        payload = "Three rings for the elven kings under the sky, seven rings for dwarven lords in their halls of stone, nine rings for morta"
        response = coap.Message(code=coap.CONTENT, payload=payload)
        d.callback(response)

class TimeResource(resource.CoAPResource):
    def __init__(self):
        resource.CoAPResource.__init__(self)
        self.visible = True
        self.observable = True

        self.notify()

    def notify(self):
        log.msg('TimeResource: trying to send notifications')
        self.updatedState()
        reactor.callLater(60, self.notify)

    def render_GET(self, request):
        response = coap.Message(code=coap.CONTENT, payload=datetime.datetime.now().strftime("%Y-%m-%d %H:%M"))
        return defer.succeed(response)

class CoreResource(resource.CoAPResource):
    """
    Example Resource that provides list of links hosted by a server.
    Normally it should be hosted at /.well-known/core

    Resource should be initialized with "root" resource, which can be used
    to generate the list of links.

    For the response, an option "Content-Format" is set to value 40,
    meaning "application/link-format". Without it most clients won't
    be able to automatically interpret the link format.

    Notice that self.visible is not set - that means that resource won't
    be listed in the link format it hosts.
    """
    def __init__(self, root):
        resource.CoAPResource.__init__(self)
        self.root = root

    def render_GET(self, request):
        data = []
        self.root.generateResourceList(data, "")
        payload = ",".join(data)
        log.msg("%s", payload)
        response = coap.Message(code=coap.CONTENT, payload=payload)
        response.opt.content_format = coap.media_types_rev['application/link-format']
        return defer.succeed(response)

############################################## begining of the server side communcationn  :: 



#the command List that will be sent from my pc['the server'] to the raspberry pi and then the node  
cmd=["<=>0013A20040B4DC64,0,DISC","<=>0013A20040B4DC64,0,ES,dht11,1,ALARME,1,12","<=>0013A20040B4DC64,0,EM,SensoeA,enit garden,est,ouest,3,192.168.8.20,raspberry,wifi","<=>0013A20040B4DC64,0,DC,0","<=>0013A20040B4DC64,0,DM","<=>0013A20040B4DC64,0,DFC","<=>0013A20040B4DC64,0,DAC","<=>0013A20040B4DC64,0,LB","<=>0013A20040B4DC64,1,ST,1","<=>0013A20040B4DC64,1,SV,1","<=>0013A20040B4DC64,1,AV"]





def RequestHandler(client) :
    while 1:
        command =str((client.recv(1024)).decode("ASCII"))
        print(command)
        if not command:
            continue
        else :
            PAYLOAD=cmd[int(command)]
            client.close() 
            client.shutdown(socket.SHUT_RDWR)   




def mainServer(host,port) :
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)     
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)     
    try:
        s.bind((host, port))
    except socket.error as msg:
        print(msg)
    print("Socket bind complete.")
    s.listen(100)
    #while True:
    client, address = s.accept()
    print("#########################################################")
    print("# Connected to: " + address[0] + ":" + str(address[1])+'#')
    print("#########################################################")
    a=threading.Thread(target=RequestHandler,args=(client,))
    a.start()
    a.join()
    print("repeat")

def startServer(host,port) :
    #host='0.0.0.0'
    #port=8282
    mainServer(host,port)

############################################## end of the server side communcationn  :: 



host='0.0.0.0'
port=8282


def main():
    serverThread=threading.Thread(target=startServer,args=(host,port,))
    serverThread.start()
    serverThread.join()
    # Resource tree creation
    log.startLogging(sys.stdout)
    root = resource.CoAPResource()
    well_known = resource.CoAPResource()
    root.putChild('.well-known', well_known)
    core = CoreResource(root)
    well_known.putChild('core', core)
    #counter = CounterResource(5000)
    #root.putChild('counter', counter)
    #time = TimeResource()
    #root.putChild('time', time)
    #other = resource.CoAPResource()
    #root.putChild('other', other)
    data = BlockResource()
    root.putChild('Node', data)
    cmd = BlockResource()
    root.putChild('CMD',cmd )
    #separate = SeparateLargeResource()
    #other.putChild('separate', separate)
    print("between")
    endpoint = resource.Endpoint(root)
    reactor.listenUDP(coap.COAP_PORT, coap.Coap(endpoint)) #, interface="::")
    reactor.run()
    print("at the end ")



if __name__=='__main__' :
    main() 
    
