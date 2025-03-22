#!/bin/bash
echo "Reset pin = $1"

(sed -i '1269d'	/etc/avrdude.conf)
(sed -i '1269i  reset = '"$1"';' /etc/avrdude.conf)
(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U flash:w:firmware.hex -F)
(sudo avrdude -P /dev/spidev0.0 -c linuxgpio -p m328p -U lfuse:w:0xFF:m -F)
