# loss_monitor

A simple utility to assist in AISG's LLM Training.
It monitors the MLFLow's `/loss/train/total` metric for any loss spike.
It checks the loss against three levels of thresholds and sounds the corresponding alerts.

| Threshold | Alert Sound  |
|:---------:|:------------:|
| 2.50      | warning      |
| 2.75      | uh-oh        |
| 3.00      | scream       |

## Sounds
Make sure the appropriate MP3 files are stored in `sounds/`. 
The `alertMap` data structure in `loss_mon.cpp` has to be 
setup with the correct sound file names.

A good source for free sound effects is https://pixabay.com/sound-effects

## Pre-requisites
    - C++
    - SDL2

## Compilation
    $ make

The `Makefile` is done on macOS 12.
Ensure SDL2 is correctly installed. One good source is via `homebrew`, available at https://brew.sh

Need to customize the makefile for Windows / Linux.

## Testing
When `loss_mon` is run, it reads from the input I/O stream.
The MLFlow metric file line format is `x y z` where

    x = unused number
    y = loss
    z = batch number

You can test it by manually inputting three numbers each time,
as shown in example transcript below:

    $ loss_mon
    3 -> sounds/girl_scream-6465.mp3
    sounds/girl_scream-6465.mp3 loaded into [2]
    2.75 -> sounds/uh-ohh-38886.mp3
    sounds/uh-ohh-38886.mp3 loaded into [1]
    2.5 -> sounds/warning-sound-6686.mp3
    sounds/warning-sound-6686.mp3 loaded into [0]
    1 2 3
    batch: 3, loss: 2
    1 2.5 3
    batch: 3, loss: 2.5
    1 2.6 3
    batch: 3, loss: 2.6 > 2.5  warning
    1 2.8 3
    batch: 3, loss: 2.8 > 2.75  getting serious
    1 3.5 3
    batch: 3, loss: 3.5 > 3  Aarrrgggghhhhh!!
    1 2 3
    batch: 3, loss: 2



## Usage
    $ ssh ohiomlflow "/home/ubuntu/utils/mon.sh" | loss_mon

The `mon.sh` utility simply tails the MLFlow metric file and send the stream back via `ssh` to the `loss_monitor` app.
