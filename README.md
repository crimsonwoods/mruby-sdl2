mruby-sdl2
====

_mruby-sdl2_ wrapped 'SDL2.0'.

# How to build
----

1. edit your 'build_config.rb'.
2. run 'make' command.

build_config.rb:

    conf.gem :github => 'crimsonwoods/mruby-sdl2', :branch => 'master'

    conf.cc do |cc|
      cc.include_paths << "/path/to/your/SDL2/include"
    end

    conf.linker do |linker|
      linker.libraries << %w(SDL2)
      linker.library_paths << "/path/to/your/SDL2/lib"
    end

# Hot to use OpenGL
----

## Build

1. edit your 'build_config.rb'.
2. add one mrbgem 'mruby-gles' in your build config script.
3. run 'make' command.
4. run mruby with your script.

## Program

1. create window with 'SDL_WINDOW_OPENGL' flag.
2. create GLContext associated window.
3. draw something
4. swap window


build_config.rb:

    conf.gem :github => 'xxuejie/mruby-gles', :branch => 'master'

# Sample code
----

Sample code is contained into 'samples' directory.


# Status
----

## Basics

|          summary          |    header   |       status      |
|:--------------------------|:------------|:-----------------:|
|Initialization and Shutdown|SDL.h        |     supported     |
|Configuration Variables    |SDL_hints.h  |     supported     |
|Error Handling             |SDL_error.h  |supported partially|
|Log Handling               |SDL_log.h    |   not supported   |
|Assertions                 |SDL_assert.h |   not supported   |
|Querying SDL Version       |SDL_version.h|     supported     |

## Video

|              summary                |    header     |    status   |
|:------------------------------------|:--------------|:-----------:|
|Display and Window Management        |SDL_video.h    |  supported  |
|2D Accelerated Rendering             |SDL_render.h   |  supported  |
|Pixel Formats and Conversion Routines|SDL_pixels.h   |not supported|
|Rectangle Functions                  |SDL_rect.h     |  supported  |
|Surface Creation and Simple Drawing  |SDL_surface.h  |  supported  |
|Platform-specific Window Management  |SDL_syswm.h    |not supported|
|Clipboard Handling                   |SDL_clipboard.h|not supported|

## Input Events

|        summary        |       header       |    status   |
|:----------------------|:-------------------|:-----------:|
|Event Handling         |SDL_events.h        |  supported  |
|Keyboard Support       |SDL_keyboard.h<br/>SDL_keycode.h<br/>SDL_scancode.h|supported partially|
|Mouse Support          |SDL_mouse.h         |  supported  |
|Joystick Support       |SDL_joystick.h      |not supported|
|Game Controller Support|SDL_gamecontroller.h|not supported|

## Force Feedback

|     summary          |   header   |   status    |
|:---------------------|:-----------|:-----------:|
|Force Feedback Support|SDL_haptic.h|not supported|

## Audio

|                   summary                    |  header   | status  |
|:---------------------------------------------|:----------|:-------:|
|Audio Device Management, Playing and Recording|SDL_audio.h|supported|

## Threads

|           summary               |   header   |   status    |
|:--------------------------------|:-----------|:-----------:|
|Thread Management                |SDL_thread.h|  supported  |
|Thread Synchronization Primitives|SDL_mutex.h |  supported  |
|Atomic Operations                |SDL_atomic.h|not supported|

## Timers

|   summary   |   header   |   status    |
|:------------|:-----------|:-----------:|
|Timer Support|SDL_timer.h |  supported  |

## File I/O Abstraction

|      summary       |   header   |   status    |
|:-------------------|:-----------|:-----------:|
|File I/O Abstraction|SDL_rwops.h |not supported|

## Shared Object Support

|                summary                  |   header   |   status    |
|:----------------------------------------|:-----------|:-----------:|
|Shared Object Loading and Function Lookup|SDL_loadso.h|not supported|

## Platform and CPU Information

|          summary           |    header    |   status    |
|:---------------------------|:-------------|:-----------:|
|Platform Detection          |SDL_platform.h|not supported|
|CPU Feature Detection       |SDL_cpuinfo.h |not supported|
|Byte Order and Byte Swapping|SDL_endian.h  |not supported|
|Bit Manipulation            |SDL_bits.h    |not supported|

## Power Management

|        summary        |   header   |   status    |
|:----------------------|:-----------|:-----------:|
|Power Management Status|SDL_power.h |not supported|

## Additional

|summary|   header   |   status    |
|:------|:-----------|:-----------:|
|Other  |SDL_stdinc.h|not supported|

# License
----

MIT License

