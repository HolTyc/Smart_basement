#!/usr/bin/env python3
import serial
import RPi.GPIO as GPIO
import os
import time
import paho.mqtt.client as mqtt
import json

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)

if __name__ == '__main__':
    SERVER = "192.168.0.5"
    sensor_data = {'t': 0, 'p': 0, 'h': 0, 'co': 0, 'ch': 0, 'co2': 0}
    x = 0
    pinVypinace = 26
    pinRelay = 6
    pinSafe = 4
    stavVypinace = 0
    svetlo = False
    dataSensor = [0,0,0,0,0,0]
    
    GPIO.setmode(GPIO.BCM);
    GPIO.setup(pinVypinace, GPIO.IN)
    GPIO.setup(pinRelay, GPIO.OUT)
    GPIO.setup(pinSafe, GPIO.OUT)
    GPIO.output(pinSafe, 0)
    stavVypinace = GPIO.input(pinVypinace)
    
    ser = serial.Serial('/dev/ttyS0', 9600, timeout=1)
    ser.reset_input_buffer()
    ser.write(1)
    while True:
        if (GPIO.input(pinVypinace)):
            GPIO.output(pinRelay, True)
            time.sleep(0.01)
        else:
            GPIO.output(pinRelay, False)
            time.sleep(0.01)

        #
        #if (stavVypinace != GPIO.input(pinVypinace)):
        #    stavVypinace = GPIO.input(pinVypinace)
        #    #Toggle svetlo
        #    svetlo = not svetlo
        #    GPIO.output(pinRelay, svetlo)
        #    time.sleep(1)
        #    stavVypinace = GPIO.input(pinVypinace)
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            if (line[0].isalpha()):
                print(line)
                #Turn relay
                #Change svetlo - mozna delalo celou dobu paseku
                #if (line[0]=='A'):
                #    svetlo = not svetlo
                #    GPIO.output(pinRelay, svetlo)
#                    print("ZAP")
            else:
                
                x+=1
                #Nekontroloval jsem - nevim jestli funguje
                dataSensor[x-1]=line
        if x>=6:
            client.connect(SERVER, 1883, 60)
            client.loop_start()

            sensor_data['t'] = int(dataSensor[0])
            sensor_data['p'] = int(dataSensor[1])
            sensor_data['h'] = int(dataSensor[2])
            sensor_data['co'] = int(dataSensor[3])
            sensor_data['ch'] = int(dataSensor[4])
            sensor_data['co2'] = int(dataSensor[5])
            
            client.publish('/home/sensors', json.dumps(sensor_data), 1)
            
            for x in range(6):
                print(dataSensor[x])
            #erase array
            x=0
            ser.write(1)
            client.loop_stop()
            client.disconnect()
