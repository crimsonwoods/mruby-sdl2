SDL2::init

begin
  timer = SDL2::Timer.add(3000) do |interval|
    puts "Timer - interval = #{interval}"
    0
  end
  SDL2::Timer::delay 5000
ensure
  SDL2::quit
end
