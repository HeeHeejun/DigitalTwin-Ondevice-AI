import time
import numpy as np
import os
import cv2
import json
import yaml

from nms import non_max_suppression
from utils import plot_one_box, Colors, get_image_tensor
from pycoral.adapters import common
from pycoral.utils.edgetpu import make_interpreter

class USB_accellerater:
    def __init__(self, model, labels, conf_thresh=0.25, iou_thresh=0.45):
        self.interpreter = make_interpreter(model)
        self.interpreter.allocate_tensors()
        self.conf_thresh = conf_thresh
        self.iou_thresh = iou_thresh
        
        self.input_details = self.interpreter.get_input_details()
        self.output_details = self.interpreter.get_output_details()
        
        self.input_zero = self.input_details[0]['quantization'][1]
        self.input_scale = self.input_details[0]['quantization'][0]
        self.output_zero = self.output_details[0]['quantization'][1]
        self.output_scale = self.output_details[0]['quantization'][0]
        
        if self.input_scale < 1e-9:
            self.input_scale = 1.0
        
        if self.output_scale < 1e-9:
            self.output_scale = 1.0
        
        self.input_size = self.get_image_size()
        self.get_names(labels)
        self.colors = Colors()
        
    def get_names(self, path):
        with open(path, 'r') as f:
          cfg = yaml.load(f, Loader=yaml.SafeLoader)
          
        names = cfg['names']
        
        self.names = names
    
    def get_image_size(self):
        return common.input_size(self.interpreter)
    
    def forward(self, x:np.ndarray, with_nms=True) -> np.ndarray:
        tstart = time.time()
        # Transpose if C, H, W
        if x.shape[0] == 3:
          x = x.transpose((1, 2, 0))
        
        x = x.astype('float32')
        
        # Scale input, conversion is: real = (int_8 - zero)*scale
        x = (x/self.input_scale) + self.input_zero
        x = x[np.newaxis].astype(np.uint8)
        
        self.interpreter.set_tensor(self.input_details[0]['index'], x)
        self.interpreter.invoke()
        
        # Scale output
        result = (common.output_tensor(self.interpreter, 0).astype('float32') - self.output_zero) * self.output_scale
        self.inference_time = time.time() - tstart
        
        if with_nms:
            tstart = time.time()
            nms_result = non_max_suppression(result, self.conf_thresh, self.iou_thresh)
            self.nms_time = time.time() - tstart
            
            return nms_result
            
        else:    
          return result
    
    def predict(self, images):
        det = []
        for image_data in images:
            image_array = np.array(image_data, dtype=np.uint8).reshape(self.input_size, self.input_size, 3)
            self.interpreter.set_tensor(self.input_details['index'], [image_array])
            self.interpreter.invoke()

            out_data = self.interpreter.get_tensor(self.output_details['index'])
            det.append(out_data)
        return det
    
    def predict_from_file(self,image_path, save_img=True, save_txt=True):
        full_image, net_image, pad = get_image_tensor(image_path, self.input_size[0])
        pred = self.forward(net_image)
        
        base, ext = os.path.splitext(image_path)
        
        output_path = base + "_detect" + ext
        det = self.process_predictions(pred[0], full_image, pad, output_path, save_img=save_img, save_txt=save_txt)
        
        return det
    
    def process_predictions(self, det, output_image, pad, output_path="detection.jpg", save_img=True, save_txt=True, hide_labels=False, hide_conf=False):
        if len(det):
            # Rescale boxes from img_size to im0 size
            # x1, y1, x2, y2=
            det[:, :4] = self.get_scaled_coords(det[:, :4], output_image, pad)
            output = {}
            base, ext = os.path.splitext(output_path)
            
            s = ""
            
            # Print results
            for c in np.unique(det[:, -1]):
                n = (det[:, -1] == c).sum()  # detections per class
                s += f"{n} {self.names[int(c)]}{'s' * (n > 1)}, "  # add to string
            
            if s != "":
                s = s.strip()
                s = s[:-1]
                        
            # Write results
            for *xyxy, conf, cls in reversed(det):
                c = int(cls)  # integer class
                if save_img:  # Add bbox to image
                    label = None if hide_labels else (self.names[c] if hide_conf else f'{self.names[c]} {conf:.2f}')
                    output_image = plot_one_box(xyxy, output_image, label=label, color=self.colors(c, True))
                if save_txt:
                    output[base] = {}
                    output[base]['box'] = xyxy
                    output[base]['conf'] = conf
                    output[base]['cls'] = cls
                    output[base]['cls_name'] = self.names[c]
                    
            if save_txt:
                output_txt = base+".txt"
                with open(output_txt, 'w') as f:
                   json.dump(output, f, indent=1)
            if save_img:
                print(output_path)
                cv2.imwrite(output_path, output_image)
            
        return det
    
    def get_scaled_coords(self, xyxy, output_image, pad):
        pad_w, pad_h = pad
        in_h, in_w = self.input_size
        output_image = np.array(output_image)
        out_h, out_w, _ = output_image.shape
                
        ratio_w = out_w/(in_w - pad_w)
        ratio_h = out_h/(in_h - pad_h) 
        
        out = []
        for coord in xyxy:

            x1, y1, x2, y2 = coord
                        
            x1 *= in_w*ratio_w
            x2 *= in_w*ratio_w
            y1 *= in_h*ratio_h
            y2 *= in_h*ratio_h
            
            x1 = max(0, x1)
            x2 = min(out_w, x2)
            
            y1 = max(0, y1)
            y2 = min(out_h, y2)
            
            out.append((x1, y1, x2, y2))
        
        return np.array(out).astype(int)
    
def main():
    print('----start----')
    input = '/home/rpi_Hee/Desktop/DigitalTwin/model/bus.jpg'
    model = '/home/rpi_Hee/Desktop/DigitalTwin/model/yolov5s-int8-224_edgetpu.tflite'
    labels = '/home/rpi_Hee/Desktop/DigitalTwin/model/coco.yaml'
    count = 5
    device = USB_accellerater(model, labels)
    
    print('----INFERENCE TIME----')
    print('Note: The first inference is slow because it includes',
            'loading the model into Edge TPU memory.')
    for _ in range(count):
        start = time.perf_counter()
        device.predict_from_file(input)
        inference_time = time.perf_counter() - start
        print('%.2f ms' % (inference_time * 1000))

if __name__ == '__main__':
    main()
