!pip install opencv-python-headless numpy 
!wget -nc https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov3.cfg 
!wget -nc https://pjreddie.com/media/files/yolov3.weights 
!wget -nc https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names 
import os 
video_filename = "/content/Input.mp4" 
print("Using video file:", video_filename) 
import cv2 
import numpy as np 
from collections import deque 
from IPython.display import Video, display 
# ----- PARAMETERS ----- 
CONFIDENCE_THRESHOLD = 0.5  # Minimum confidence for YOLO detections 
NMS_THRESHOLD = 0.4         
# Non-maxima suppression threshold 
EXPECTED_DIRECTION = "LTR"  # Expected direction: "LTR" (Left to Right) or "RTL" (Right to Left) 
MAX_TRACK_LENGTH = 20       # How many centroid positions to store for tracking 
# YOLO file paths 
YOLO_CONFIG_PATH = 'yolov3.cfg' 
YOLO_WEIGHTS_PATH = 'yolov3.weights' 
COCO_NAMES_PATH   = 'coco.names' 
# Load COCO class labels from file 
with open(COCO_NAMES_PATH, 'r') as f: 
    LABELS = [line.strip() for line in f.readlines()] 
 
# Function to get the output layer names for YOLO 
def get_output_layers(net): 
    layer_names = net.getLayerNames() 
    try: 
        # Works for older OpenCV (e.g., 4.1.x) 
        output_layers = [layer_names[i[0] - 1] for i in net.getUnconnectedOutLayers()] 
    except IndexError: 
        # Works for newer OpenCV (e.g., 4.5.x and later) 
        output_layers = [layer_names[i - 1] for i in net.getUnconnectedOutLayers()] 
    return output_layers 
 
# Initialize the YOLO network 
net = cv2.dnn.readNetFromDarknet(YOLO_CONFIG_PATH, YOLO_WEIGHTS_PATH) 
output_layer_names = get_output_layers(net) 
 
# A deque to store car centroid positions across frames 
centroid_history = deque(maxlen=MAX_TRACK_LENGTH) 
 
def detect_car_yolo(frame): 
    """ 
    Uses YOLO to detect objects in the frame and returns the bounding box 
    and centroid for the 'car' with the largest detected area. 
    """ 
    (H, W) = frame.shape[:2] 
    # Create a blob from the image and perform a forward pass through YOLO 
    blob = cv2.dnn.blobFromImage(frame, 1/255.0, (416, 416), swapRB=True, crop=False) 
    net.setInput(blob) 
    layer_outputs = net.forward(output_layer_names) 
 
    boxes = [] 
    confidences = [] 
    classIDs = [] 
 
    # Loop over each output layer's detections 
    for output in layer_outputs: 
        for detection in output: 
            scores = detection[5:] 
            classID = np.argmax(scores) 
            confidence = scores[classID] 
            # Only consider "car" detections above a threshold 
            if confidence > CONFIDENCE_THRESHOLD and LABELS[classID] == "car": 
                box = detection[0:4] * np.array([W, H, W, H]) 
                (centerX, centerY, width, height) = box.astype("int") 
                x = int(centerX - (width / 2)) 
                y = int(centerY - (height / 2)) 
                boxes.append([x, y, int(width), int(height)]) 
                confidences.append(float(confidence)) 
                classIDs.append(classID) 
 
    # Apply non-maxima suppression to filter out weak and overlapping detections 
    idxs = cv2.dnn.NMSBoxes(boxes, confidences, CONFIDENCE_THRESHOLD, NMS_THRESHOLD) 
 
    if len(idxs) > 0: 
        best_box = None 
        best_area = 0 
        for i in idxs.flatten(): 
            (x, y, w, h) = boxes[i] 
            area = w * h 
            if area > best_area: 
                best_area = area 
                best_box = boxes[i] 
        if best_box is not None: 
            (x, y, w, h) = best_box 
            centroid = (int(x + w / 2), int(y + h / 2)) 
            return (x, y, w, h), centroid 
    return None, None 
 
def check_wrong_way(centroid_history, expected_direction): 
    """ 
    Determines whether the car is moving in the wrong direction based on the 
    history of centroids. 
    """ 
    if len(centroid_history) < 2: 
        return False 
 
    # Use the first and last positions to compute displacement along x-axis 
    start = centroid_history[0] 
    end = centroid_history[-1] 
    dx = end[0] - start[0] 
 
    if expected_direction == "LTR" and dx < 0: 
        return True 
    elif expected_direction == "RTL" and dx > 0: 
        return True 
    return False 
 
def main(video_path): 
    cap = cv2.VideoCapture(video_path) 
    if not cap.isOpened(): 
        print("Error opening video file:", video_path) 
        return 
 
    # Define the codec and create VideoWriter object to save output 
    frame_width = 640 
    frame_height = 480 
    fps = cap.get(cv2.CAP_PROP_FPS) 
    if fps == 0: 
        fps = 30  # default to 30 if couldn't fetch fps 
    fourcc = cv2.VideoWriter_fourcc(*'XVID') 
    out = cv2.VideoWriter('output.avi', fourcc, fps, (frame_width, frame_height)) 
 
    while True: 
        ret, frame = cap.read() 
        if not ret: 
            break 
 
        # Resize frame for consistency 
        frame = cv2.resize(frame, (frame_width, frame_height)) 
 
        bbox, centroid = detect_car_yolo(frame) 
        if centroid is not None: 
            centroid_history.append(centroid) 
            # Draw the bounding box and centroid on the frame 
            (x, y, w, h) = bbox 
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2) 
            cv2.circle(frame, centroid, 5, (0, 0, 255), -1) 
 
            # Draw the trajectory using the stored centroids 
            for i in range(1, len(centroid_history)): 
                cv2.line(frame, centroid_history[i - 1], centroid_history[i], (255, 0, 0), 2) 
 
            # Check if the car is moving in the wrong direction 
            if check_wrong_way(centroid_history, EXPECTED_DIRECTION): 
                cv2.putText(frame, "Wrong Way", (50, 50), 
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2) 
        else: 
            # Reset the centroid history if no car is detected 
            centroid_history.clear() 
 
        # Write the processed frame to the output video file 
        out.write(frame) 
 
    cap.release() 
    out.release() 
    print("Processing complete. Output video saved as output.avi") 
 
# Run the main function on the uploaded video 
main(video_filename) 
 
# Display the output video within Colab 
display(Video("output.avi", embed=True))
