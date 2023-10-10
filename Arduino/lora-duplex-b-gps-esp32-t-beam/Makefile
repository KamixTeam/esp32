BOARD1=esp32:esp32:t-beam
PORT23=/dev/cu.SLAB_USBtoUART3

.PHONY: default lint all flash clean

default: all flash clean

lint:
	cpplint --extensions=ino --filter=-legal/copyright *.ino

all:
	arduino-cli compile --fqbn $(23) ./

flash:
	arduino-cli upload -p $(23) --fqbn $(23)

clean:
	rm -r build