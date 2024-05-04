upload:
	arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old ./lowLevel/lowLevel.ino --upload --port $(PORT) --verbose
