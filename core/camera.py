import cv2
import threading
import os
import time

camera_location = ["front", "", "back"]
class Camera:
    def __init__(self):
        self.cameras = [0, 2]  
        self.cap = [cv2.VideoCapture(cam, cv2.CAP_V4L2) for cam in self.cameras]
        self.fov = 30
        self.lock = threading.Lock()
        if not all([cap.isOpened() for cap in self.cap]):
            print("camera error")
        for cap in self.cap:
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)  
            self.width = 1280
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)
            self.width = 720
        self.is_running = True

        os.makedirs('output', exist_ok=True)
        self.camera_test_level = False
        self.frame_dict = {}

    def __del__(self):
        for cap in self.cap:
            cap.release()
            
    def pop_cap(self):
        det = False
        if self.lock.acquire(timeout=0.5):
            try:
                det = self.frame_dict.copy()
            finally:
                self.lock.release()
        return det
    
    def start_capturing(self):
        self.camera_threads = []
        for idx, cap in enumerate(self.cap):
            thread = threading.Thread(target=self.capture_frames, args=(cap, self.cameras[idx]))
            self.camera_threads.append(thread)
            thread.start()


    def stop_capturing(self):
        for thread in self.camera_threads:
            thread.join()
        self.is_running = False

    def capture_frames(self, video, camera_idx):
        while self.is_running:
            #start_time = time.perf_counter() 
            ret, frame = video.read() 
            if ret:
                if self.lock.acquire(timeout=0.5):
                    try:
                        self.frame_dict[camera_location[camera_idx]] = frame
                    finally:
                        self.lock.release()
                   
                #end_time = time.perf_counter() 
                #capture_duration = (end_time - start_time) * 1000 
                #print(f"Time taken to capture and process frame from camera {camera_idx}: {capture_duration:.2f} ms")

                if self.camera_test_level:
                    self.save_frame(frame, camera_idx)

            else:
                print(f"Failed to capture frame from camera {camera_idx}")

            time.sleep(0.5)

    def save_frame(self, frame, camera_idx):
        filename = f"output/frame_{camera_idx}_{int(time.time() * 1000)}.jpg"
        cv2.imwrite(filename, frame)
        #print(f"Saved {filename}")


if __name__ == "__main__":
    cam = Camera()
    try:
        cam.start_capturing()
    except KeyboardInterrupt:
        cam.stop_capturing()
    finally:
        print(cam.frame_dict)
