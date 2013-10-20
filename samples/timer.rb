SDL2::init

begin
  n = 2000
  timer = SDL2::Timer.add(n) do |interval|
    puts "Timer - interval = #{interval}/#{n}"
    n = n - 500
  end
  SDL2::Timer::delay 6000
ensure
  SDL2::quit
end
