#!/usr/bin/env python

import time
import RPi.GPIO as GPIO
from mfrc522 import SimpleMFRC522
import mysql.connector

db = mysql.connector.connect(
  host="localhost",
  user="admin",
  passwd="Robot2345",
  database="vstupy"
)

cursor = db.cursor()
reader = SimpleMFRC522()

try:
  while True:
    print('Place Card to\nregister')
    id, text = reader.read()
    cursor.execute("SELECT id, name FROM users WHERE rfid_uid="+str(id))
    result = cursor.fetchone()

    if cursor.rowcount >=1:
        print("Welcome: " + result[1])
        cursor.execute("INSERT INTO attendance (user_id) VALUES (%s)", (result[0],) )
        db.commit()
    else:
        print("User does not exist")
    time.sleep(2)
    break;
finally:
  GPIO.cleanup()
