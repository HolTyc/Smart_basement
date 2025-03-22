#!/bin/bash
echo "Soubor = $1"
echo "Arduino cislo = $2"
filename="${1%.*}"
if (( $2 == 1 ));
then
	(sed -i '1269d'	/etc/avrdude.conf)
	(sed -i '1269i  reset = 22;' /etc/avrdude.conf)
	(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:firmware.hex)
elif (($2 == 2 ));
then
	(sed -i '1269d'	/etc/avrdude.conf)
	(sed -i '1269i  reset = 27;' /etc/avrdude.conf)
	(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:firmware.hex)
else
	echo "nemenim"
fi
(avr-gcc -Os -mmcu=atmega328p -I/usr/lib/avr/include -c $1)
(avr-gcc -mmcu=atmega328p -o $filename.elf $1)
(avr-objcopy -j .text -j .data -O ihex $filename.elf $filename.hex)
(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:$filename.hex)
(rm $filename.o $filename.hex $filename.elf)
