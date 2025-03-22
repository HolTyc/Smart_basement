#!/bin/bash
echo "Arduino cislo = $1"
if (( $1 == 1 ));
then
	(sed -i '1269d'	/etc/avrdude.conf)
	(sed -i '1269i  reset = 22;' /etc/avrdude.conf)
	(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:firmware.hex)
elif (($1 == 2 ));
then
	(sed -i '1269d'	/etc/avrdude.conf)
	(sed -i '1269i  reset = 27;' /etc/avrdude.conf)
	(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:firmware.hex)
else
	echo "nemenim"
fi
(make)
(avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:build-uno/"${PWD##*/}"_.hex)
(rm -r build-uno)
