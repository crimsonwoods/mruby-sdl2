mruby-sdl2
====

_mruby-sdl2_ wrapped 'SDL2.0'.

# How to build
----

1. edit your 'build_config.rb'.
2. run make command.

build_config.rb:

    conf.gem :github => 'crimsonwoods/mruby-sdl2', :branch => 'master'

    conf.cc do |cc|
      cc.include_paths << "/path/to/your/SDL2/include"
    end

    conf.linker do |linker|
      linker.libraries << %w(SDL2)
      linker.library_paths << "/path/to/your/SDL2/lib"
    end

# Sample code
----

Sample code is contained into 'samples' directory.

# License
----

MIT License

