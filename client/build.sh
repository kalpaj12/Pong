dir="build"

[ ! -d "$dir" ] && mkdir -p "$dir"

cd build && cmake ../ -DMULTIPLAYER=$1 && make && ./pong