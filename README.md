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

## Pre-requisites
    - C++
    - SDL2

## Compilation
    $ make

The `Makefile` is done on macOS 12. Need to customize this to build on Windows / Linux.

## Usage:
    $ ssh ohiomlflow "/home/ubuntu/utils/mon.sh" | loss_mon

The `mon.sh` utility simply tails the MLFlow metric file and send the stream back via `ssh` to the `loss_monitor` app.
