SDL2::init

if !Object.const_defined?(:GL2) then
  puts "Note: This mruby does not support OpenGL ES!"
  puts "Note: Get mruby-gles from \"https://github.com/crimsonwoods/mruby-gles/tree/fix_for_compile_on_Ubuntu\"."
  raise StandardError, "OpenGL ES is not supported."
end

X = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
Y = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
W = 640
H = 480
FLAGS = SDL2::Video::Window::SDL_WINDOW_OPENGL

VSHADER = <<SHADER
attribute vec4 a_Position;\n
uniform float u_Rotation;\n
void main() {\n
  mat4 m_rot = mat4(cos(u_Rotation), -sin(u_Rotation), 0.0, 0.0,\n
    sin(u_Rotation), cos(u_Rotation), 0.0, 0.0,\n
    0.0, 0.0, 1.0, 0.0,\n
    0.0, 0.0, 0.0, 1.0);\n
  vec4 v_pos = m_rot * a_Position;\n
  gl_Position = v_pos;\n
  gl_FrontColor = vec4(v_pos.x + 1.0 / 2.0, v_pos.y + 1.0 / 2.0, v_pos.z + 1.0 / 2.0, 1.0);\n
}
SHADER
FSHADER = <<SHADER
void main() {\n
  gl_FragColor = gl_Color;\n
}
SHADER

vertex = SDL2::FloatBuffer.new(
  [
     0.0, -1.0, 0.0,
     1.0,  0.5, 0.0,
    -1.0,  1.0, 0.0,
  ]
)

def create_shader(type, source)
  shader = GL2::glCreateShader(type)
  GL2::glShaderSource(shader, 1, [source], [source.length]);
  GL2::glCompileShader(shader)
  if GL2::GL_FALSE == GL2::glGetShaderiv(shader, GL2::GL_COMPILE_STATUS) then
    log = GL2::glGetShaderInfoLog(shader, 100)
    puts log[1]
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
    u_Rotation = GL2::glGetUniformLocation(prog, "u_Rotation");
    if -1 == u_Rotation then
      raise StandardError, "cannot get uniform variable."
    end

    r = 0.0
    until (ev = SDL2::Input::wait).nil?
      if ev.type == SDL2::Input::SDL_MOUSEMOTION then
        x = ev.x - (W / 2.0)
        y = -(ev.y - (H / 2.0))
        if x.abs < 0.000001 then
          r = 0
        else
          r = Math.atan(y/x)
        end
        if x < 0 then
          r = Math::PI / 2.0 + (Math::PI / 2.0 + r)
        end
      end
      if ev.type == SDL2::Input::SDL_QUIT then
        break
      end
      if ev.type == SDL2::Input::SDL_FINGERMOTION then
        next
      end
      r = Math::PI - r
      GL2::glClear(GL2::GL_COLOR_BUFFER_BIT | GL2::GL_DEPTH_BUFFER_BIT)
      GL2::glUniform1f(u_Rotation, r)
      GL2::glEnableVertexAttribArray(a_Position)
      GL2::glVertexAttribPointer(a_Position, 3, GL2::GL_FLOAT, 0, false, vertex.cptr)
      GL2::glDrawArrays(GL2::GL_TRIANGLES, 0, 3)
      GL2::glDisableVertexAttribArray(a_Position)
      w.swap
    end

    gl_context.delete
    w.destroy
  ensure
    SDL2::Video::quit
  end
ensure
  SDL2::quit
end
