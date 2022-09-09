# Software audio mixer  
## Summary:
Provides 2D buffered audio playback for multiple sources.  
The source mixing is done in a separate thread with configurable audio block count and size.
 
## Features:
* Supported platforms: Windows(WaveOut)
* Supported file formats: RIFF WAVE (*.wav)
* Supports up to 16 active sources
* Source controlls: position, volume, speed

## Dependencies:
* winmm(Windows)
* pthread