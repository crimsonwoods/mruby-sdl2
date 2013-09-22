module SDL2
  class RGB
    def initialize (r, g, b)
      @r = r & 0xff
      @g = g & 0xff
      @b = b & 0xff
    end

    def r
      @r
    end

    def r=(r)
      @r = r & 0xff
    end

    def g
      @g
    end

    def g=(g)
      @g = g & 0xff
    end

    def b
      @b
    end

    def b=(b)
      @b = b & 0xff
    end

    def to_i
      case SDL2::SDL_BYTEORDER
      when SDL2::SDL_LIL_ENDIAN then
        (b << 16) | (g << 8) | (r)
      when SDL2::SDL_BIG_ENDIAN then
        (r << 16) | (g << 8) | (b)
      end
    end
  end

  class RGB888 < RGB
  end

  class RGBA < RGB
    def initialize (r, g, b, a)
      super(r, g, b)
      @a = a & 0xff
    end

    def a
      @a
    end

    def a=(a)
      @a = a & 0xff
    end

    def to_i
      case SDL2::SDL_BYTEORDER
      when SDL2::SDL_LIL_ENDIAN then
        (a << 24) | (b << 16) | (g << 8) | (r)
      when SDL2::SDL_BIG_ENDIAN then
        (r << 24) | (g << 16) | (b << 8) | (a)
      end
    end
  end

  class RGBA8888 < RGBA
  end

  class BGR < RGB
    def to_i
      case SDL2::SDL_BYTEORDER
      when SDL2::SDL_LIL_ENDIAN then
        (r << 16) | (g << 8) | (b)
      when SDL2::SDL_BIG_ENDIAN then
        (b << 16) | (g << 8) | (r)
      end
    end
  end

  class BGR888 < BGR
  end

  class BGRA < BGR
    def initialize (r, g, b, a)
      super(r, g, b)
      @a = a & 0xff
    end

    def a
      @a
    end

    def a=(a)
      @a = a & 0xff
    end

    def to_i
      case SDL2::SDL_BYTEORDER
      when SDL2::SDL_LIL_ENDIAN then
        (a << 24) | (r << 16) | (g << 8) | (b)
      when SDL2::SDL_BIG_ENDIAN then
        (b << 24) | (g << 16) | (r << 8) | (a)
      end
    end
  end

  class BGRA8888 < BGRA
  end
end

