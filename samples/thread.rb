SDL2::init

begin
  thread = SDL2::Thread.new do
    puts "this message is put from worker thread."
    for i in 1..10 do
      SDL2::delay 1000
      puts "passed #{i} sec."
    end
  end
  thread.wait
ensure
  SDL2::quit
end
