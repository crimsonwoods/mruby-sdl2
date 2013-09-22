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
    surface = w.surface
    FW=20
    FH=20
    for y in 0..23
      for x in 0..31
        surface.fill_rect (x * 8) << ((y / 8).to_i * 8), SDL2::Rect.new(x * FW, y * FH, FW, FH)
      end
    end
    w.update_surface
    if Kernel.respond_to?(:sleep) then
      sleep 3
    end
    w.destroy
  ensure
    SDL2::Video::quit
  end
ensure
  SDL2::quit
end
