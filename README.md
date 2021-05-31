# Compositing Display Server with Java and C++ client libraries and a Chromium Demo App
## Display Server
Written in C++11, uses OpenGL for Hardware Accelerated Compositing, ZeroMQ for Networking and Inter Process Communication and SDL for mouse and keyboard events. Chromium based Web Browser: based on CEF, it uses the Display Server’s low-level C API for input event and rendering.
Low-Level C API (libasl): used by client applications for input events and rendering on the Display Server, it’s C ABI helps porting the library to different programming languages.

## C++ Application Framework
Object oriented UI Framework for writing applications that run on top of the Display Server, it uses Cairo vector graphics library for UI elements rendering.

## Java Application Framework
Object oriented UI Framework for writing applications that run on top of the Display Server. It uses the low-level C API (libasl) for basic input events and rendering and Java2D for UI elements rendering on the client side, the plan is to replace the Java2D layer with Skia or Cairo graphics for better performance and portability.

## Build on macOS
Install cmake:
```shell
brew install cmake
```
Install dependencies:
```
brew install sdl2
brew install cairo
brew install zeromq
```
Build with CMake:
```
cmake \
-DZMQ_INCLUDE_DIR=/usr/local/include \
-DZMQ_LIBRARY=/usr/local/lib/libzmq.dylib \
-DCAIRO_INCLUDE_DIR=/usr/local/include/cairo \
-DCAIRO_LIBRARY=/usr/local/lib/libcairo.dylib \
-DSDL2_INCLUDE_DIR=/usr/local/include/SDL2 \
-DSDL2_LIBRARY=/usr/local/lib/libSDL2.dylib \
-B./build
```
Build all targets:
```shell
cmake --build ./build
```
Build appserver only:
```shell
cmake --build ./build --target appserver
```