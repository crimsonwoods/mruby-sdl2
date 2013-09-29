SDL2::init

X = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
Y = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
W = 640
H = 480
FLAGS = SDL2::Video::Window::SDL_WINDOW_OPENGL

VSHADER = <<SHADER
attribute vec4 a_Position;\n
void main() {\n
  gl_Position = a_Position;\n
  gl_PointSize = 5.0;\n
}
SHADER
FSHADER = <<SHADER
void main() {\n
  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n
}
SHADER

vertex = SDL2::FloatBuffer.new(
  [
    0.0, -1.0, 0.0,
    0.0, 0.5, 0.0,
    0.0, 1.0, 0.0,
  ]
)

def create_shader(type, source)
  shader = GL2::glCreateShader(type)
  GL2::glShaderSource(shader, 1, [source], [source.length]);
  GL2::glCompileShader(shader)
  if GL2::GL_FALSE == GL2::glGetShaderiv(shader, GL2::GL_COMPILE_STATUS) then
    raise StandardError, "cannot compile shader program."
  end
  shader
end

begin
  SDL2::Video::init
  begin
  w = SDL2::Video::Window.new "sample", X, Y, W, H, FLAGS
  gl_context = SDL2::Video::GLContext.new w

  GL2::glCullFace(GL2::GL_BACK)
  GL2::glFrontFace(GL2::GL_CCW)
  GL2::glEnable(GL2::GL_CULL_FACE)
  GL2::glClearColor(0, 0, 0, 0)
  GL2::glViewport(0, 0, W, H)

  vShader = create_shader(GL2::GL_VERTEX_SHADER,   VSHADER)
  fShader = create_shader(GL2::GL_FRAGMENT_SHADER, FSHADER)

  prog = GL2::glCreateProgram()
  GL2::glAttachShader(prog, vShader)
  GL2::glAttachShader(prog, fShader)
  GL2::glDeleteShader(vShader)
  GL2::glDeleteShader(fShader)

  GL2::glLinkProgram(prog)
  if GL2::GL_FALSE == GL2::glGetProgramiv(prog, GL2::GL_LINK_STATUS) then
    raise StandardError, "cannot link shader program."
  end
  GL2::glUseProgram(prog)

  a_Position = GL2::glGetAttribLocation(prog, "a_Position")
  if -1 == a_Position then
    raise StandardError, "cannot get attribute variable."
  end

  x = 0.0
  while x < 1.0
    vertex[0] = 1.0 - x
    vertex[3] = x
    vertex[6] = -x
    GL2::glClear(GL2::GL_COLOR_BUFFER_BIT | GL2::GL_DEPTH_BUFFER_BIT)
    GL2::glEnableVertexAttribArray(a_Position)
    GL2::glVertexAttribPointer(a_Position, 3, GL2::GL_FLOAT, 0, false, vertex.cptr)
    GL2::glDrawArrays(GL2::GL_TRIANGLES, 0, 3)
    GL2::glDisableVertexAttribArray(a_Position)
    w.swap
    x += 0.01
    SDL2::delay 10
  end

  gl_context.delete
  w.destroy
  ensure
    SDL2::Video::quit
  end
ensure
  SDL2::quit
end
