import serial
import time
import paho.mqtt.client as mqtt

# Serial settings (match your Arduino COM port and baud rate)
SERIAL_PORT = 'COM3'          # Change to your COM port (e.g., COM4 or /dev/ttyUSB0 on Linux)
BAUD_RATE = 9600              # Match your Arduino baud rate

# MQTT Broker settings
broker = "localhost"
port = 1883
topic = "incubator/data"

# Connect to MQTT
client = mqtt.Client()
client.connect(broker, port, 60)

# Open Serial port
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
time.sleep(2)  # Allow time to initialize

print("📡 Listening on", SERIAL_PORT)
while True:
    try:
        line = ser.readline().decode().strip()
        if line:
            print("📤 Publishing:", line)
            client.publish(topic, line)
    except Exception as e:
        print("❌ Error:", e)
