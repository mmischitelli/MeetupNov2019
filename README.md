# MeetupNov2019
Mini-example of the main topics discussed at the November 2019 Italian C++ Meetup in Modena.

## Description
This example depicts the classic producer/consumer scenario.

In this particular case, each producer is an independent thread that is able to generate random numbers in chunks of two every second.

On the other side there is a consumer, a simple timer that ticks every 150ms and consumes a random value at each tick. This means that in one second, this consumer can consume more than 6 numbers, thus requiring at least 4 producers to keep up with the demand.

The user is able to increase and decrease the number of producers by pressing **N** (create new) and **K** (kill one).

To show graphically what's happening, there is an actor that acts as an observer, polling the status of the producers constantly: it spawns a new paddock for each producer and a create for each available number.

## Features
Threads, semaphores, events, delegates, timers, subsystems.

## Instructions
Clone UE4 source code to your computer, set it up by running Setup.bat and GenerateProjectFiles.bat.
I'd suggest to compile the engine before compiling the project, just to make sure everything works.

Then clone this repo and right-click on the .uproject -> Generate Project Files. Once finished, double-click on the .sln to open Visual Studio, then hit F5 to launch the project with the debugger attached.