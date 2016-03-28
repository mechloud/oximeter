# oximeter
Open Source DIY Pulse Oximeter Using Arduino

Instructions:

Plug the device into any usb port (computer, power bank, cellphone charger).

When first turning on the device, an Adafruit logo should appear on the screen for a second or so followed by an SpO2 value screen. 

The value of the first 10 points taken by the device will vary but and after the first 10 points, an average of these points will be taken. From that point on, the device will read a point and take the average of the last 10 points. The average will start to stabilize close to the real value. 

There is a reset button in the middle of the controller if ever you switch users or the average is not stabilizing.
