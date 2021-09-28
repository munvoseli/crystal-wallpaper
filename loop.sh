#!/bin/bash

do_loop=1

exit_script() {
    do_loop=0
}

trap exit_script SIGINT SIGTERM

while [ $do_loop -gt 0 ]
do
    ./a.out
    feh --bg-tile bg.ppm
    sleep 1
done

echo ""
