import serial
import time
import argparse

from xmlrpc.server import SimpleXMLRPCServer

class ESPRobo(object):
    
    
    def __init__(self, dev='/dev/ttyUSB0', mmps=[-1975, 469, 1405],
                 axes=dict(x=2, y=1, z=0)):
        self.mmps = mmps  # mm per step
        self.s = serial.Serial(dev, 9600)
        self.dev = dev
        self.axes = axes
    def close(self):
        self.s.close()
        return None

    def iaxis(self, ax=0):
        if isinstance(ax, str):
            iax = self.axes[ax.lower()]
        elif isinstance(ax, int):
            iax = ax
        else:
            raise ValueError('{} is an invalid axis specifier'.format(ax))
        if not (0 <= iax <= 2):
            raise ValueError('{} is an invalid axis specifier'.format(ax))

        return iax

        
    def move(self, mm=0, axis=0, r=True):
        npulses = int(mm * self.mmps[axis])
        cmdchar = 'R' if r else 'M'
        cmd = '{}{}!{}'.format(cmdchar, axis, npulses).encode('ASCII')
        self.s.write(cmd)
        time.sleep(0.5)
        while True:
            if self.s.in_waiting > 0:
                break
            time.sleep(0.5)
            
        time.sleep(0.5)

        b = self.s.readline()
        r = b.decode('ASCII')
        return r
    def moveX(self, mm):
        return self.move(mm, self.axes['x'], r=False)
    def moveY(self, mm):
        return self.move(mm, self.axes['y'], r=False)
    def moveZ(self, mm):
        return self.move(mm, self.axes['z'], r=False)

    def rmoveX(self, mm):
        return self.move(mm, self.axes['x'], r=True)
    def rmoveY(self, mm):
        return self.move(mm, self.axes['y'], r=True)
    def rmoveZ(self, mm):
        return self.move(mm, self.axes['z'], r=True)
    
    def position(self, ax=None, pulses=False):
        if ax is None:
            # read each axis
            x = self.position(self.axes['x'], pulses)
            y = self.position(self.axes['y'], pulses)
            z = self.position(self.axes['z'], pulses)
            return(dict(x=x, y=y, z=z))
        else:
            iax = self.iaxis(ax)
            cmd = 'P{}?'.format(iax).encode('ASCII')
            self.s.write(cmd)
            time.sleep(0.1)
            r = self.s.readline().decode('ASCII')

            pos = int(r[3:])

            if pulses:
                return pos
            else:
                return pos / self.mmps[iax]


    def set_reference(self, ax, mm=0, pulses=False):
        iax = self.iaxis(ax)

        if pulses:
            nsteps = mm
        else:
            nsteps = int(mm * self.mmps[iax])

        cmd = 'P{}!{}'.format(iax, nsteps).encode('ASCII')
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
    
