SDL2::init

begin
  mutex = SDL2::Mutex.new
  thread = SDL2::Thread.new do
    mutex.lock
    puts "this message is put from worker thread."
    mutex.unlock
    for i in 1..10 do
      mutex.lock
      SDL2::delay 1000
      puts "passed #{i} sec."
      mutex.unlock
    end
  end
  SDL2::delay 500
  for i in 1..10 do
    mutex.lock
    puts "waiting..."
    mutex.unlock
    SDL2::delay 100
  end
  thread.wait
  mutex.destroy
ensure
  SDL2::quit
end
