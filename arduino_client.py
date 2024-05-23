# # client.py
# import socketio

# # Create a Socket.IO client
# sio = socketio.Client()

# # Define event handlers
# @sio.event
# def connect():
#     print("Connected to server")
#     sio.send("Hello, server!")

# @sio.event
# def disconnect():
#     print("Disconnected from server")

# # @sio.event
# # def response(data):
# #     print(f"Received response: {data}")

# @sio.event
# def message(data):
#     print(f"Received message: data1={data['data1']}, data2={data['data2']}")


# # Connect to the server
# sio.connect('http://127.0.0.1:8000')

# # Wait for events
# sio.wait()

# install "pip install socketio-client pyserial"
import socketio
import serial
import time


# baud rate and serial port of Arduino
arduino_port = 'COM8'  
baud_rate = 9600

# Create a Socket.IO client
sio = socketio.Client()

# Connect to Arduino
arduino = serial.Serial(arduino_port, baud_rate, timeout=1)

@sio.event
def connect():
    print("Connected to server")

@sio.event
def disconnect():
    print("Disconnected from server")

@sio.event
def message(data):
    # x = int(data['data1'])
    # y = int(data['data2'])
    # if x > 8500 or x < 0 or y > 8500 or y < 0:

    #     print(f"Received data: {data['data1']} {data['data2']} {data['data3']}")
    #     print(f"The sent coordinate is outside the boundary value of 2-axis system! Move to the boundary.")
    #     message_str = f"{data['data1']} {data['data2']} {data['data3']}\n"  #the structure of the sent data is "data1 data2"
    #     arduino.write(message_str.encode())

    # else:
    print(f"Received data: {data['data1']} {data['data2']} {data['data3']}")
    message_str = f"{data['data1']} {data['data2']} {data['data3']}\n"  #the structure of the sent data is "data1 data2"
    arduino.write(message_str.encode())

# Connect to the server
sio.connect('http://127.0.0.1:8000')

# Wait for events
sio.wait()