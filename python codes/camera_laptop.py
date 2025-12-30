import cv2
from ultralytics import YOLO
import serial
import time

# Connect Arduino
try:
    arduino = serial.Serial('COM3', 9600, timeout=0.01)
    time.sleep(2)
    print("Connected!")
except:
    print("Check Serial Port!")
    exit()

model = YOLO('best.pt')
url = "http://192.168.1.139:8080/video"
cap = cv2.VideoCapture(url)

# Trick 1: Set Buffer Size to 1
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

while cap.isOpened():
    # Trick 2: Clear Buffer
    # Bin-faddi el frames elly m-tkhazzana fe el network buffer 3ashan n-awsal lel "Live"
    for _ in range(5):
        cap.grab()
    
    success, frame = cap.retrieve()
    if not success: break

    # Trick 3: Resize for Speed
    # Ta2leel el-size beykhally el YOLO we OpenCV yersemo asra3 kteer
    frame = cv2.resize(frame, (640, 480))
    
    # Run YOLO
    results = model(frame, conf=0.25, verbose=False)
    annotated_frame = results[0].plot()
    
    cv2.imshow("Robot AI View", annotated_frame)

    # Serial Check (Ultrasonic)
    if arduino.in_waiting > 0:
        line = arduino.readline().decode('utf-8', errors='ignore').strip()
        
        if "DETECT" in line:
            print("Object Detected! Analyzing...")
            if len(results[0].boxes) > 0:
                print("Trash Found! Sending 'P'")
                arduino.write(b'P')
                # Wait loop bedon lag lel video
                start = time.time()
                while time.time() - start < 12:
                    cap.grab() # Keep buffer empty
                    cv2.waitKey(1)
            else:
                print("False alarm. Sending 'S'")
                arduino.write(b'S')

    if cv2.waitKey(1) & 0xFF == ord('q'): break

cap.release()
cv2.destroyAllWindows()