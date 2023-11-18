#RecD - Recoder desktop

RecD is simple tools to fast capture area desktop. Make screenshot draw recentlage and capture this area desktop.

###To compile need###

opencv4 - draw lib
ffmpeg  - lib to recoding
scrot   - screenshot tool

g++ recD.cpp -o recD `pkg-config --cflags --libs opencv4`
