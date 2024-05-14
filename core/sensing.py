import sensor
import object_detection
import threading

class Sensor:
    def __init__(self, model_path, label_path, start_signal = True):
        self.camera = sensor.camera()
        self.ridar = sensor.ridar()
        self.tpu = object_detection.USB_accellerater(model_path, label_path)
        self.start_signal = start_signal
        self.camera_thread = threading.Thread(target=self.camera_start)
        self.ridar_thread = threading.Thread(target=self.ridar_start)
        
        self.start()
        
    def camera_start(self):
        self.camera.do_capture()
        
    def ridar_start(self):
        self.ridar.scan_start()
        self.ridar.scan_data()
    
    def start(self):
        while self.start_signal:
            self.camera_thread.start()
            self.ridar_thread.start()
            self.camera_thread.join()
            self.ridar_thread.join()
            pictures = self.camera.pop_cap()
            distance = self.ridar_data = self.ridar.get_data()
            
            self.tpu.predict(pictures)
    
    def sensor_test(self):
        self.camera.do_capture()
        self.ridar.scan_start()
        self.ridar.scan_data()

def main():
    model = '/home/rpi_Hee/Desktop/DigitalTwin/model/yolov5s-int8-224_edgetpu.tflite'
    labels = '/home/rpi_Hee/Desktop/DigitalTwin/model/coco.yaml'
    s = Sensor(model, labels)
    s.sensor_test()

if __name__ == '__main__':
    main()

        
        
                