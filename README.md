# Pong


## What is it?
see [Wikipedia](https://en.wikipedia.org/wiki/Pong)

> TL;DR, Pong is a video game initially developed by Atari.

## Why build it?
The year 2020 happened, and I was stuck at home due to lockdown.. I had projects to submit, and hence multiplayer pong.

## Steps to run:

    sudo apt-get install libsdl2-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev libsdl2-image-dev cmake

    git clone https://github.com/kalpaj12/Pong

    cd Pong

    git submodule update --init --recursive && git submodule foreach git pull origin master

    cd client && ./build.sh

## What works
* Yes, Multiplayer functionality works!


Note: If you have firewall enabled, allow/add rule: 1337/udp
