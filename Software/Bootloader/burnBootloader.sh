echo '     ##############################'
echo '     ## Pinchy Bootloader Burner ##'
echo '     ##############################'
echo '		for macOS'

avrdude -c usbasp -p m32u4 -U lfuse:w:0xff:m -U  hfuse:w:0xd8:m -U efuse:w:0xcb:m

avrdude -c usbasp -p m32u4 -U flash:w:/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/bootloaders/caterina/Caterina-Micro.hex
