# Append Only File System

this is a simple rough implementation of an "Append Only" File system

This sytem utilizes labelling and savefile cloning when insertion and deletion is required, unless if a data is pushed, the system appends the given data into the savefile

This filesystem do not have a security system, encryption or any other safety features. Although it showcases how older File System are implemented

Usually, the Append-Only filesystem is implemented in a WORM system, or any system that utilizes speed and prevention of data corruption as its first priority since append only filesystem do not manipulate existing data.
