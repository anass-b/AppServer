# Compositing Display Server with Java and C++ client libraries and a Chromium Demo App
## Display Server
Written in C++11, uses OpenGL for Hardware Accelerated Compositing, ZeroMQ for Networking and Inter Process Communication and SDL for mouse and keyboard events. Chromium based Web Browser: based on CEF, it uses the Display Server’s low-level C API for input event and rendering.
Low-Level C API (libasl): used by client applications for input events and rendering on the Display Server, it’s C ABI helps porting the library to different programming languages.

## C++ Application Framework
Object oriented UI Framework for writing applications that run on top of the Display Server, it uses Cairo vector graphics library for UI elements rendering.

## Java Application Framework
Object oriented UI Framework for writing applications that run on top of the Display Server. It uses the low-level C API (libasl) for basic input events and rendering and Java2D for UI elements rendering on the client side, the plan is to replace the Java2D layer with Skia or Cairo graphics for better performance and portability.

## Build
Install dependencies:
```
brew install sdl2
brew install cairo
brew install zeromq
```
Build with CMake:
```
TODO
```
