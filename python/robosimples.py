import serial
import time
import argparse

from xmlrpc.server import SimpleXMLRPCServer

class ESPRobo(object):

    def __init__(self, dev='/dev/ttyUSB0', mmps=-1975):
        self.mmps = mmps  # mm per step
        self.s = serial.Serial(dev, 9600)
        self.dev = dev
    def close(self):
        self.s.close()
        return None
    def rmove(self, mm):
        nsteps = int(mm * self.mmps)
        cmd = 'R{}\n'.format(nsteps).encode('ASCII')
        
        # Send command and wait until there is a response
        self.s.write(cmd)

        time.sleep(0.5)
        while True:
            if self.s.in_waiting > 0:
                break
            time.sleep(0.5)
            
        time.sleep(0.5)

        b = self.s.readline()
        r = b.decode('ASCII')

        p = r.find('V:')
        if p > -1:
            return True
        else:
            return False
    def move(self, mm):
        nsteps = int(mm * self.mmps)
        cmd = 'M{}\n'.format(nsteps).encode('ASCII')
        
        # Send command and wait until there is a response
        self.s.write(cmd)

        time.sleep(0.5)
        while True:
            if self.s.in_waiting > 0:
                break
            time.sleep(0)
            
        time.sleep(0.5)

        b = self.s.readline()
        r = b.decode('ASCII')
        
        p = r.find('V:')
        if p > -1:
            return True
        else:
            return False
    def position(self):
        cmd = 'P\n'.encode('ASCII')
        self.s.write(cmd)
        time.sleep(0.2)
        r = self.s.readline().decode('ASCII')

        p = r.find('V:')

        ss = int(r[p+2:])
        mm = ss / self.mmps
        return mm
    def set_reference(self, mm=0):

        nsteps = int(mm * self.mmps)
        cmd = 'P{}\n'.format(nsteps).encode('ASCII')
        self.s.write(cmd)
        self.s.flush()
        time.sleep(0.2)
        return self.s.readline()
    

        
def start_server(ip='localhost', port=9543, comport='/dev/ttyUSB0'):
    dev = ESPRobo(comport)
    print("Starting XML-RPC server for Z mover")
    print("IP: {}, port: {}".format(ip, port))
    server = SimpleXMLRPCServer((ip, port), allow_none=True)
    server.register_instance(dev)
    server.serve_forever()


if __name__ == "__main__":
    print("Creating interface ...")
    parser = argparse.ArgumentParser(description="ESPDaq server")
    parser.add_argument("-i", "--ip", help="IP address of the XML-RPC server", default="localhost")
    parser.add_argument("-p", "--port", help="XML-RPC server port", default=9541, type=int)
    parser.add_argument("-s", "--comport", help="Serial port to be used", default="/dev/ttyUSB0")

    args = parser.parse_args()
    start_server(args.ip, args.port, args.comport)
    
