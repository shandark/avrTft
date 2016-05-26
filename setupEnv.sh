#/bin/bash

alias load='function _load(){ sudo avrdude -p m328p -c usbasp -U flash:w:$1; };_load'

