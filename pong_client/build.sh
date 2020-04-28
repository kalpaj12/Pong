dir="build"

[ ! -d "$dir" ] && mkdir -p "$dir"

cd build && cmake ../ && make && ./pong