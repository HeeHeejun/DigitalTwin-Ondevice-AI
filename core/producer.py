import socket
import json
from _thread import *

class Producer:
    def __init__(self, host, port, receive_mode = True):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.host = host
        self.port = port
        self.data = []
        
        if receive_mode:
            start_new_thread(self.recv_thread, (self.client_socket, ))
            
    def __del__(self):
        self.client_socket.close()

    def connect(self):
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
        with open(json_path, 'r', encoding='utf-8') as file:
            data = json.load(file)
        self.client_socket.sendall(json.dumps(data).encode('utf-8'))
        response = self.client_socket.recv(1024)
        print('Received', repr(response))
        

def main():
    host = " "
    port = " "
    json = '/home/rpi_Hee/Desktop/DigitalTwin/model/Data_front.json'
    p = Producer(host, port, False)
    p.send_test(json)

if __name__ == '__main__':
    main()