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

# License
----

MIT License

