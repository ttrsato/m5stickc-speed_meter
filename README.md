# m5stickc-speed_meter
Reed switch type speed meter
# Hardware compornents
- M5StickC 1x
- Reed switch 1x
- Magnet 1x
# Assemble
## Reed switch
Connect between G26 and GND (Very simple!)<br>
<img src="https://user-images.githubusercontent.com/17703236/71455545-d2b94d80-27d8-11ea-82cb-f4504d2dc26c.jpeg" width=300>
## Manget
Put the magnet on the spoke<br>
Reed switch should be close to the magnet<br>
<img src="https://user-images.githubusercontent.com/17703236/71455591-08f6cd00-27d9-11ea-9dfb-88ba351523a9.jpeg" width=300>
# Comment
This program is compiled in Arduino IDE environment.
You need to set your bicycle circumference length to variable 'circumference'.
The content of the program is simple. Just use the GPIO interrupt to measure the time the magnet passes and calculate the speed. In addition, there is code to detect a stop condition.
Sending speed information to another device via WiFi or Bluetooth might be useful. Compare the speed between this method and calculation using accelerometer results must be interesting. Also, extend the wire between M5StickC and reed switch, then put M5StickC on the handle might be practical.
