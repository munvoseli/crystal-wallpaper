#!/bin/bash

do_loop=1

exit_script() {
    do_loop=0
}

trap exit_script SIGINT SIGTERM

while [ $do_loop -gt 0 ]
do
    ./crystal
    feh --bg-scale bg.ppm
    # if you have imlib2, this gets rid of the blending
    # ./render
    sleep .1
done

echo ""
