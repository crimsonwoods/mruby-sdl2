SDL2::init

X = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
Y = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
W = 640
H = 480
FLAGS = SDL2::Video::Window::SDL_WINDOW_OPENGL

begin
  SDL2::Video::init
  begin
  w = SDL2::Video::Window.new "sample", X, Y, W, H, FLAGS
  gl_context = SDL2::Video::GLContext.new w
  if Kernel.respond_to?(:sleep) then
    s = w.size
    p = w.position
    for i in 0..100
      w.position = SDL2::Point.new(p.x + i, p.y + i)
      w.size = SDL2::Size.new(s.w - i, s.h - i)
      usleep 10000
    end
    p = w.position
    for i in 0..100
      w.position = SDL2::Point.new(p.x + i, p.y - i)
      w.size = SDL2::Size.new(s.w + i, s.h + i)
      usleep 10000
    end
    p = w.position
    for i in 0..100
      w.position = SDL2::Point.new(p.x - i, p.y - i)
      w.size = SDL2::Size.new(s.w - i, s.h - i)
      usleep 10000
    end
    p = w.position
    for i in 0..100
      w.position = SDL2::Point.new(p.x - i, p.y + i)
      w.size = SDL2::Size.new(s.w + i, s.h + i)
      usleep 10000
    end
  end
  gl_context.delete
  w.destroy
  ensure
    SDL2::Video::quit
  end
ensure
  SDL2::quit
end
