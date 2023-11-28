import RPI.GPIO as GPIO
import time

PIN_BUTTON = 4
PIN_LED = 18
PIN_SERVE = 23
PIN_BUZZER = 24

#Set up GPIO pins
GPIO.setmode(GPIO.BCM)
GPIO.setup(PIN_BUZZER, GPIO.OUT)
GPIO.setup(PIN_LED, GPIO.OUT)
GPIO.setup(PIN_SERVO, GPIO.OUT)
GPIO.setup(PIN_BUTTON, GPIO.IN)

#servo
servo = GPIO.PWN(PIN_SERVO, 50)

#50 Hz PWM frequency
servo.start(0)
#start at 0 degrees
time.sleep(1)
servo.ChangeDutyCycle(10)
#90 degrees
time.sleep(1)
servo.ChangeDutyCycle(1)
time.sleep(1)

#led
GPIO.output(PIN_LED, GPIO.HIGH)
time.sleep(1)
GPIO.output(PIN_LED, GPIO.LOW)
time.sleep(1)

#buzzer
GPIO.output(PIN_BUZZER, GPIO.HIGH)
time.sleep(1)
GPIO.output(PIN_BUZZER, GPIO.LOW)
time.sleep(1)

#button
while True:
    if GPIO.intput(PIN_BUTTON):
        print("pressed")
        time.sleep(1)
