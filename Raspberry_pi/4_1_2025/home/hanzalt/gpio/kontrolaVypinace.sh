#!/bin/bash
pinVypinac=26
pinRelay=6
stavVypinac=0
svetlo=0

echo $pinVypinac > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio$pinVypinac/direction
stavVypinac=$(< /sys/class/gpio/gpio$pinVypinac/value)

while :
do
	if [ $stavVypinac != $(< /sys/class/gpio/gpio$pinVypinac/value) ];then
		stavVypinac=$(< /sys/class/gpio/gpio$pinVypinac/value)
		#echo $pinVypinac > /sys/class/gpio/unexport
		echo $pinRelay > /sys/class/gpio/export
		echo out > /sys/class/gpio/gpio6/direction
		echo 
	fi	
done
if [ $action = "read" ];then
  	#assign direction
	echo in > /sys/class/gpio/gpio26/direction
  	#read gpio value
	cat /sys/class/gpio/gpio26/value   	
	
elif [ $action = "write" ];then
  	#assign direction
	echo out > /sys/class/gpio/gpio6/direction
  	#assign value
	echo 1 > /sys/class/gpio/gpio6/value
else
   	echo "Unknown parameter"
fi

#remove gpio instance
echo $pin > /sys/class/gpio/unexport

