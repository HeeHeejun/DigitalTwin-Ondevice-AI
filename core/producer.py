import socket
import json
import sensing
import time
import os
from _thread import *

class Producer:
    def __init__(self, host, port, receive_mode = True):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.setblocking(False)
        self.host = host
        self.port = port
        self.connect()

        self.data = []
        
        if receive_mode:
            start_new_thread(self.recv_thread, (self.client_socket, ))
            
    def __del__(self):
        self.client_socket.close()

    def connect(self):
        print(f"Connecting to {self.host}:{self.port}")
        self.client_socket.settimeout(10) 

        try:
            self.client_socket.connect((self.host, self.port))
        except socket.timeout:
            print("Connection timed out")
        except BlockingIOError:
            print("Operation already in progress, waiting for connection to complete...")
            self.wait_for_connection()
        except socket.error as e:
            print(f"Socket error: {e}")
        self.client_socket.connect((self.host, self.port))
        
    def send(self, data):
        self.client_socket.send(data.encode())
    
    def read(self):
        return self.data.pop(0)
    
    def recv_thread(self):
        while True:
            data = self.client_socket.recv(1024)
            self.data.append(data)
            
    def send_test(self, json_path):
        if os.path.getsize(json_path) != 0:
            with open(json_path, 'r', encoding='utf-8') as file:
                data = json.load(file)
            self.client_socket.sendall(json.dumps(data).encode('utf-8'))
        #response = self.client_socket.recv(1024)
        #print('Received', repr(response))
        
def main():
    host = "192.168.0.5"
    port = 11994
    json = '/home/rpihee/Desktop/DigitalTwin-Ondevice-AI/data.json'
    model = '/home/rpihee/Desktop/DigitalTwin-Ondevice-AI/model/yolov5s-int8-224_edgetpu.tflite'
    labels = '/home/rpihee/Desktop/DigitalTwin-Ondevice-AI/model/coco.yaml'

    p = Producer(host, port, False)
    print("connect!!")
    sensing.Sensor(model, labels, True, True)
    print("sensor on!!")
    while(1):
        p.send_test(json)
        time.sleep(0.5)

if __name__ == '__main__':
    main()
