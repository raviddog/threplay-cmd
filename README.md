# Readme

**threplay** is a small program to aid in watching downloaded Touhou replays without having to manually rename and copy them into the respective game folders.

## Usage

Set your replay and game paths in the config file. Then copy a replay file into the same directory as the executable, then drag it onto it. Game version detection is based on the replay name so you can't just rename it to anything but default replay names from most services should work fine.

Note: an existing replay with the same name as set in config will be overwritten. This program is designed for watching once. Maybe I can modify it later but for now this is all I need.

## todo

- ~~change winexec to createprocess~~
- ~~possibly move game detection to the config file~~
- add gui interface
- work out a way to grab replays from any directory
