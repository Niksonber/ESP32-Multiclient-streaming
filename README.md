# ESP32-Multiclient-streaming

ESP-32 library for multiclient streaming

#### Usage

in your code, include the library and create a instance of Streamer:

```cpp
#include "streamer.hpp"

Streamer stream;
```
Then, initialize it with default stream provider

```cpp
stream.begin(<DESIRED FPS>);
```
or with a custom provider, for instance camProvider`

```cpp
CamProvider provider;

stream.begin(<DESIRED FPS>, &provider);
```

Then, the ESP-32 will stream to all available clients in `<LOCAL-IP>/stream` or just provide a sample in`<LOCAL-IP>/sample`.
