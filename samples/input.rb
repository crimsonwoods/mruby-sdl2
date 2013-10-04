def scancode_to_s(code)
  SDL2::Input::Keyboard::scancode_name(code)
end

def windowevent_to_s(event)
  case event
  when SDL2::Input::SDL_WINDOWEVENT_SHOWN then "shown"
  when SDL2::Input::SDL_WINDOWEVENT_HIDDEN then "hidden"
  when SDL2::Input::SDL_WINDOWEVENT_EXPOSED then "exposed"
  when SDL2::Input::SDL_WINDOWEVENT_MOVED then "moved"
  when SDL2::Input::SDL_WINDOWEVENT_RESIZED then "resized"
  when SDL2::Input::SDL_WINDOWEVENT_SIZE_CHANGED then "size_changed"
  when SDL2::Input::SDL_WINDOWEVENT_MINIMIZED then "minimized"
  when SDL2::Input::SDL_WINDOWEVENT_MAXIMIZED then "maximized"
  when SDL2::Input::SDL_WINDOWEVENT_RESTORED then "restored"
  when SDL2::Input::SDL_WINDOWEVENT_ENTER then "enter"
  when SDL2::Input::SDL_WINDOWEVENT_LEAVE then "leave"
  when SDL2::Input::SDL_WINDOWEVENT_FOCUS_GAINED then "focus_gained"
  when SDL2::Input::SDL_WINDOWEVENT_FOCUS_LOST then "focus_lost"
  when SDL2::Input::SDL_WINDOWEVENT_CLOSE then "close"
  else "unknown"
  end
end

SDL2::init

begin
  SDL2::Video::init
  begin
    w = SDL2::Video::Window.new "sample", 100, 100, 640, 480, SDL2::Video::Window::SDL_WINDOW_SHOWN
    until (ev = SDL2::Input::wait).nil?
      puts case ev.type
      when SDL2::Input::SDL_KEYDOWN then
        "down: #{scancode_to_s(ev.keysym.scancode)}"
      when SDL2::Input::SDL_KEYUP then
        "up: #{scancode_to_s(ev.keysym.scancode)}"
      when SDL2::Input::SDL_WINDOWEVENT then
        "window: #{windowevent_to_s(ev.event)}"
      when SDL2::Input::SDL_MOUSEMOTION then
        "mouse motion: "
      when SDL2::Input::SDL_MOUSEBUTTONDOWN then
        "mouse button down: "
      when SDL2::Input::SDL_MOUSEBUTTONUP then
        "mouse button up: "
      when SDL2::Input::SDL_QUIT then
        "quit"
      else
        "?: #{ev.to_s}"
      end
      if ev.type == SDL2::Input::SDL_QUIT then
        break
      end
    end
    w.destroy
  ensure
    SDL2::Video::quit
  end
ensure
  SDL2::quit
end
