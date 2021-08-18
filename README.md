# PMSIS
PulseRain Microcontroller Software Interface Standard

## Clean

cd build
make clean

## Build
cd build
make

## Load and Run

Run for GRV3000D on ArtyA7-100T board

After connecting the board to a Windows PC, please make sure the COM port for the board is determined by looking it up in the Device Manager.

Under Cygwin, the COM port mapping is like:

COM1 - /dev/ttyS0

COM2 - /dev/ttyS1

COM3 - /dev/ttyS2

COM4 - /dev/ttyS3

COM5 - /dev/ttyS4

And setup the COM port for LOAD and RUN in core_portme.mak
Please also install pyserial under Cygwin pip3 install pyserial

To load/run in Command Prompt

cd build

cd scripts

python GRV3000D_Config.py --port=COM5 -r -R --console_enable -i ../GRV3000D.elf

To load/run in Cygwin

cd build

cd scripts

python3 GRV3000D_Config.py --port=/dev/ttyS4 -r -R --console_enable -i ../GRV3000D.elf
