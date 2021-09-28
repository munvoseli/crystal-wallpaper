
This program relies on feh.  It grows a wallpaper like crystals, kind of like https://github.com/TomSmeets/FractalArt.

See demo.png for kind of what it looks like.


make
./loop.sh


If you use X and have some libraries installed, do

make comp; ./composite

This will compile crystal-auto.c , a version of the program which does not rely on feh and does not have feh's blurring.  It was used for demo.png .
