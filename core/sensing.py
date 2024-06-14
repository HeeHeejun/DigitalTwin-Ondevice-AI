import sys
sys.path.append('/home/rpihee/Desktop/DigitalTwin-Ondevice-AI/build/lib.linux-aarch64-3.8')

import sensor
import camera
import object_detection
import threading
import math
import json
import cv2
import time

def calculate_horizontal_angle_from_bbox(bbox, image_width, FOV_horizontal, center_angle_deg=0):
    xmin, _, xmax, _ = bbox
    
    bbox_center_x = (xmin + xmax) / 2
    
    center_x = image_width / 2
    
    dx = bbox_center_x - center_x
    
    FOV_horizontal_rad = math.radians(FOV_horizontal)
    
    theta_x_rad = math.atan((2 * dx * math.tan(FOV_horizontal_rad / 2)) / image_width)
    theta_x_deg = math.degrees(theta_x_rad)
    
    absolute_theta_x_deg = center_angle_deg + theta_x_deg
    if(absolute_theta_x_deg < 0) :
        return (- absolute_theta_x_deg)
    return 360 - absolute_theta_x_deg

def write_json(data):
    json_data = json.dumps(data, indent=4)

    with open("data.json", "w") as json_file:
        json_file.write(json_data)

class Sensor:
    def __init__(self, model_path, label_path, start_signal = True, capture=True):
        self.camera = camera.Camera()
        self.ridar = sensor.ridar()
        self.ridar.scan_start()
        self.tpu = object_detection.USB_accellerater(model_path, label_path)
        self.start_signal = start_signal
        self.camera_thread = threading.Thread(target=self.camera_start)
        self.ridar_thread = threading.Thread(target=self.ridar_start)
        self.pictures = []
        self.iscapture = True
        self.setting()
        self.start_sensing = threading.Thread(target=self.start_thread)
        self.start_sensing.start()
        #self.end()
    
    def camera_start(self):
        try:
            self.camera.start_capturing()
        except KeyboardInterrupt:
            self.camera.stop_capturing()
            
    def pop_from_cam(self):
        det = self.camera.pop_cap()
        if det:
            self.pictures.append(det)
        
    def ridar_start(self):
        self.ridar.scan_data()
    
    def calculate_distance(self, det):
        data = {}
        for idx, detected in det.items():   
            data[idx] = {}
            num_instance = 0
            center_of_camera = 180 if idx == 'front' else 0
            for *xyxy, conf, cls in reversed(detected):
                c = int(cls)
                if self.tpu.names[c] == "person" :
                    continue
                data[idx][num_instance] = {}
                data[idx][num_instance]['box'] = xyxy
                data[idx][num_instance]['conf'] = conf
                if self.tpu.names[c] == "person":
                    continue
                elif self.tpu.names[c] == "car":
                    data[idx][num_instance]['cls_name'] = self.tpu.names[c]
                else :
                    data[idx][num_instance]['cls_name'] = "car"
                angle = calculate_horizontal_angle_from_bbox(xyxy, self.camera.width , self.camera.fov, center_of_camera)
                data[idx][num_instance]['angle'] = angle
                #print(f'angle %f, num_istance %d' % (angle, num_instance))
                data[idx][num_instance]['distance'] = self.ridar.get_distance_from_anlge(angle)
                num_instance = num_instance + 1 
                
                break
        return data
    
    def start(self):
        self.start_sensing.start()
        
    def setting(self):
        try:
            self.camera.start_capturing()
        except KeyboardInterrupt:
            self.camera.stop_capturing()
        self.ridar_thread.start()
        
    def start_thread(self):
        while self.start_signal:
            start_time = time.perf_counter() 
            self.pop_from_cam()
            if not self.pictures:
                continue
            det = self.tpu.predict(self.pictures.pop(0), self.iscapture, self.iscapture)
            write_json(self.calculate_distance(det))
            end_time = time.perf_counter() 
            capture_duration = (end_time - start_time) * 1000 
            print(f"Time taken : {capture_duration:.2f} ms")
        
    def end(self):
        self.ridar_thread.join()
        self.camera.stop_capturing()
        
    
    


def main():
    model = '/home/rpihee/Desktop/DigitalTwin-Ondevice-AI/model/yolov5s-int8-224_edgetpu.tflite'
    labels = '/home/rpihee/Desktop/DigitalTwin-Ondevice-AI/model/coco.yaml'
    Sensor(model, labels)

if __name__ == '__main__':
    main()

        
        
                