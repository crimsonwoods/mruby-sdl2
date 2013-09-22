SDL2::init

X = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
Y = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
W = 640
H = 480
FLAGS = SDL2::Video::Window::SDL_WINDOW_SHOWN

begin
  SDL2::Video::init
  begin
    w = SDL2::Video::Window.new "sample", X, Y, W, H, FLAGS
    renderer = SDL2::Video::Renderer.new(w)
    1000.times do |n|
      renderer.draw_color = SDL2::RGB.new(0, 0, 0)
      renderer.clear
      FW=20
      FH=20
      for y in 0..23
        for x in 0..31
          rgb = (x * 8) << ((y / 8).to_i * 8)
          r = (((0xff0000 & rgb) >> 16) + n) % 0xff
          g = (((0x00ff00 & rgb) >>  8) + n) % 0xff
          b = (((0x0000ff & rgb) >>  0) + n) % 0xff
          renderer.draw_color = SDL2::RGB.new(r, g, b)
          renderer.fill_rect SDL2::Rect.new(x * FW, y * FH, FW, FH)
        end
      end
      renderer.draw_color = SDL2::RGB.new(0xff, 0xff, 0xff)
      renderer.draw_line(SDL2::Point.new(0, 0), SDL2::Point.new(W, H))
      renderer.draw_line(SDL2::Point.new(W, 0), SDL2::Point.new(0, H))
      renderer.present
    end
    renderer.destroy
    w.destroy
  ensure
    SDL2::Video::quit
  end
ensure
  SDL2::quit
end
