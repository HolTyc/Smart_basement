#!/bin/bash
echo "Nazev kodu = $1"
echo "Arduino cislo = $2"
(sh /home/hanzalt/gpio/gpio.sh write 4 1)
sleep 1
if (( $2 == 1  || $2 == "master"));
then
	(sed -i '1269d'	/etc/avrdude.conf)
	(sed -i '1269i  reset = 22;' /etc/avrdude.conf)
	(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:firmware.hex)
elif (($2 == 2  || $2 == "sensor"));
then
	(sed -i '1269d'	/etc/avrdude.conf)
	(sed -i '1269i  reset = 27;' /etc/avrdude.conf)
	(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:firmware.hex)
elif (($2 == 3 || $2 == "interface"));
then
	(sed -i '1269d'	/etc/avrdude.conf)
	(sed -i '1269i  reset = 17;' /etc/avrdude.conf)
	(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -FU flash:w:firmware.hex)
elif (($2 == 4 || $2 == "com"));
then
	(sed -i '1269d'	/etc/avrdude.conf)
	(sed -i '1269i  reset = 23;' /etc/avrdude.conf)
	(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:firmware.hex)
else
	echo "nemenim"
fi
(make -C /home/hanzalt/Arduino_Programming/arduinoProgramming/$1)
(avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:/home/hanzalt/Arduino_Programming/arduinoProgramming/$1/build-uno/"${1##*/}"_.hex -F)
(rm -r /home/hanzalt/Arduino_Programming/arduinoProgramming/$1/build-uno)
(sh /home/hanzalt/gpio/gpio.sh write 4 0)
