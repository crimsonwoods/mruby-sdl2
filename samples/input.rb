def scancode_to_s(code)
  case code
  when SDL2::Input::SDL_SCANCODE_0 then "0"
  when SDL2::Input::SDL_SCANCODE_1 then "1"
  when SDL2::Input::SDL_SCANCODE_2 then "2"
  when SDL2::Input::SDL_SCANCODE_3 then "3"
  when SDL2::Input::SDL_SCANCODE_4 then "4"
  when SDL2::Input::SDL_SCANCODE_5 then "5"
  when SDL2::Input::SDL_SCANCODE_6 then "6"
  when SDL2::Input::SDL_SCANCODE_7 then "7"
  when SDL2::Input::SDL_SCANCODE_8 then "8"
  when SDL2::Input::SDL_SCANCODE_9 then "9"
  when SDL2::Input::SDL_SCANCODE_A then "A"
  when SDL2::Input::SDL_SCANCODE_B then "B"
  when SDL2::Input::SDL_SCANCODE_C then "C"
  when SDL2::Input::SDL_SCANCODE_D then "D"
  when SDL2::Input::SDL_SCANCODE_E then "E"
  when SDL2::Input::SDL_SCANCODE_F then "F"
  when SDL2::Input::SDL_SCANCODE_G then "G"
  when SDL2::Input::SDL_SCANCODE_H then "H"
  when SDL2::Input::SDL_SCANCODE_I then "I"
  when SDL2::Input::SDL_SCANCODE_J then "J"
  when SDL2::Input::SDL_SCANCODE_K then "K"
  when SDL2::Input::SDL_SCANCODE_L then "L"
  when SDL2::Input::SDL_SCANCODE_M then "M"
  when SDL2::Input::SDL_SCANCODE_N then "N"
  when SDL2::Input::SDL_SCANCODE_O then "O"
  when SDL2::Input::SDL_SCANCODE_P then "P"
  when SDL2::Input::SDL_SCANCODE_Q then "Q"
  when SDL2::Input::SDL_SCANCODE_R then "R"
  when SDL2::Input::SDL_SCANCODE_S then "S"
  when SDL2::Input::SDL_SCANCODE_T then "T"
  when SDL2::Input::SDL_SCANCODE_U then "U"
  when SDL2::Input::SDL_SCANCODE_V then "V"
  when SDL2::Input::SDL_SCANCODE_W then "W"
  when SDL2::Input::SDL_SCANCODE_X then "X"
  when SDL2::Input::SDL_SCANCODE_Y then "Y"
  when SDL2::Input::SDL_SCANCODE_Z then "Z"
  else "?"
  end
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
