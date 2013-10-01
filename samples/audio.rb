SDL2::init

# Please modify the path.
#PATH_TO_SAMPLE_WAV = "/path/to/your/sample.wav"
PATH_TO_SAMPLE_WAV = "../mruby-sdl2/samples/sample.wav"

def format_to_s(fmt)
  case fmt
    when SDL2::Audio::AUDIO_S8 then "S8"
    when SDL2::Audio::AUDIO_U8 then "U8"
    when SDL2::Audio::AUDIO_S16LSB then "S16LSB"
    when SDL2::Audio::AUDIO_S16MSB then "S16MSB"
    when SDL2::Audio::AUDIO_S16SYS then "S16SYS"
    when SDL2::Audio::AUDIO_S16 then "S16"
    when SDL2::Audio::AUDIO_U16LSB then "U16LSB"
    when SDL2::Audio::AUDIO_U16MSB then "U16MSB"
    when SDL2::Audio::AUDIO_U16SYS then "U16SYS"
    when SDL2::Audio::AUDIO_U16 then "U16"
    when SDL2::Audio::AUDIO_S32LSB then "S32LSB"
    when SDL2::Audio::AUDIO_S32MSB then "S32MSB"
    when SDL2::Audio::AUDIO_S32SYS then "S32SYS"
    when SDL2::Audio::AUDIO_S32 then "S32"
    when SDL2::Audio::AUDIO_F32LSB then "F32LSB"
    when SDL2::Audio::AUDIO_F32MSB then "F32MSB"
    when SDL2::Audio::AUDIO_F32SYS then "F32SYS"
    when SDL2::Audio::AUDIO_F32 then "F32"
    else "UNKNOWN"
  end
end

def display_spec(spec)
  puts "  Frequency: #{spec.freq} Hz"
  puts "  Channels:  #{spec.channels}"
  puts "  Format:    #{format_to_s(spec.format)}"
  puts "  Samples:   #{spec.samples}"
end

begin
  drivers = SDL2::Audio::drivers
  i = 0
  puts "Available Audio Drivers:"
  drivers.each do |name|
    puts "  #{name}"
  end
  while i < drivers.length
    begin
      SDL2::Audio::init(drivers[i])
      puts "Audio driver \"#{drivers[i]}\" is opened."
      break
    rescue SDL2::SDL2Error => e
      p e
    end
    i += 1
  end
  begin
    data = SDL2::Audio::AudioData.new(PATH_TO_SAMPLE_WAV)
    puts "Audio spec of loaded wav file:"
    display_spec(data.spec)
    offset = 0
    data.spec.callback = proc {|udata, stream, len|
      if offset > data.length then
        SDL2::Audio::pause(true)
      end
      SDL2::Audio::mix_audio(stream, 0, data, offset, len, 32)
      offset += len
    }
    aspec = SDL2::Audio::open data.spec
    puts "Audio spec of opened device:"
    display_spec(aspec);
    begin
      # Play sound 5 seconds.
      SDL2::Audio::pause(false)
      until SDL2::Audio::status == SDL2::Audio::SDL_AUDIO_PAUSED
        SDL2::delay 100
      end
      SDL2::Audio::pause(true)
    ensure
      SDL2::Audio::close
    end
  ensure
    SDL2::Audio::quit
  end
ensure
  SDL2::quit
end

