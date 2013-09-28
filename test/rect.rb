##
# SDL2::Rect test

SDL2::init
begin
  assert('SDL2::Rect.initialize') do
    r = SDL2::Rect.new
    r.x == 0 && r.y == 0 && r.w == 0 && r.h == 0
  end
  assert('SDL2::Rect.initialize(x)') do
    r = SDL2::Rect.new(1)
    r.x == 1 && r.y == 0 && r.w == 0 && r.h == 0
  end
  assert('SDL2::Rect.initialize(x, y)') do
    r = SDL2::Rect.new(1, 2)
    r.x == 1 && r.y == 2 && r.w == 0 && r.h == 0
  end
  assert('SDL2::Rect.initialize(x, y, w)') do
    r = SDL2::Rect.new(1, 2, 3)
    r.x == 1 && r.y == 2 && r.w == 3 && r.h == 0
  end
  assert('SDL2::Rect.initialize(x, y, w, h)') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.x == 1 && r.y == 2 && r.w == 3 && r.h == 4
  end
  assert('SDL2::Rect.x=') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.x = 10
    r.x == 10 && r.y == 2 && r.w == 3 && r.h == 4
  end
  assert('SDL2::Rect.y=') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.y = 10
    r.x == 1 && r.y == 10 && r.w == 3 && r.h == 4
  end
  assert('SDL2::Rect.w=') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.w = 10
    r.x == 1 && r.y == 2 && r.w == 10 && r.h == 4
  end
  assert('SDL2::Rect.h=') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.h = 10
    r.x == 1 && r.y == 2 && r.w == 3 && r.h == 10
  end
  assert('SDL2::Rect.position') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.position.x == 1 && r.position.y == 2
  end
  assert('SDL2::Rect.position=') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.position = SDL2::Point.new(10, 20)
    r.position.x == 10 && r.position.y == 20 && r.x == 10 && r.y == 20
  end
  assert('SDL2::Rect.size') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.size.w == 3 && r.size.h == 4
  end
  assert('SDL2::Rect.size=') do
    r = SDL2::Rect.new(1, 2, 3, 4)
    r.size = SDL2::Size.new(30, 40)
    r.size.w == 30 && r.size.h == 40 && r.w == 30 && r.h == 40
  end
  assert('SDL2::Rect.has_intersection?') do
    r = SDL2::Rect.new(0, 0, 100, 100)
    r.has_intersection?(SDL2::Rect.new(  10,  10, 20, 20)) &&  # inside
    r.has_intersection?(SDL2::Rect.new( -10, -10, 20, 20)) &&  # across the border
    r.has_intersection?(SDL2::Rect.new(  90,  90, 20, 20)) &&  # across the border
    !r.has_intersection?(SDL2::Rect.new( 100, 100, 20, 20)) && # outside
    r.has_intersection?(SDL2::Rect.new( 99,  99, 20, 20))      # on the border line
  end
  assert('SDL2::Rect.intersection inside') do
    r = SDL2::Rect.new(0, 0, 100, 100)
    i = r.intersection(SDL2::Rect.new(10, 10, 20, 20))
    i.x == 10 && i.y == 10 && i.w == 20 && i.h == 20
  end
  assert('SDL2::Rect.intersection across-the-border') do
    r = SDL2::Rect.new(0, 0, 100, 100)
    i = r.intersection(SDL2::Rect.new(90, 90, 20, 20))
    i.x == 90 && i.y == 90 && i.w == 10 && i.h == 10
  end
  assert('SDL2::Rect.intersection outside') do
    r = SDL2::Rect.new(0, 0, 100, 100)
    i = r.intersection(SDL2::Rect.new(100, 100, 20, 20))
    i.nil?
  end
  assert('SDL2::Rect.intersection_line across-the-border') do
    r = SDL2::Rect.new(0, 0, 100, 100)
    points = r.intersection_line(SDL2::Point.new(-100, -100), SDL2::Point.new(200, 200))
    points.size == 2 && points[0].x == 0 && points[0].y == 0 && points[1].x == 99 && points[1].y == 99
  end
  assert('SDL2::Rect.intersection_line outside') do
    r = SDL2::Rect.new(0, 0, 100, 100)
    points = r.intersection_line(SDL2::Point.new(-100, -100), SDL2::Point.new(-100, 200))
    points.nil?
  end
  assert('SDL2::Rect.empty?') do
    SDL2::Rect.new(0, 0, 0, 0).empty? &&
    SDL2::Rect.new(0, 0, 0, 1).empty? &&
    SDL2::Rect.new(0, 0, 1, 0).empty? &&
    !SDL2::Rect.new(0, 0, 1, 1).empty?
  end
  assert('SDL2::Rect.==') do
    SDL2::Rect.new(0, 0, 0, 0) == SDL2::Rect.new(0, 0, 0, 0) &&
    SDL2::Rect.new(0, 0, 0, 1) == SDL2::Rect.new(0, 0, 0, 1) &&
    SDL2::Rect.new(0, 0, 1, 0) == SDL2::Rect.new(0, 0, 1, 0) &&
    SDL2::Rect.new(0, 0, 1, 1) == SDL2::Rect.new(0, 0, 1, 1) &&
    !(SDL2::Rect.new(1, 2, 3, 4) == SDL2::Rect.new(1, 2, 1, 2))
  end
  assert('SDL2::Rect.!=') do
    !(SDL2::Rect.new(0, 0, 0, 0) != SDL2::Rect.new(0, 0, 0, 0)) &&
    !(SDL2::Rect.new(0, 0, 0, 1) != SDL2::Rect.new(0, 0, 0, 1)) &&
    !(SDL2::Rect.new(0, 0, 1, 0) != SDL2::Rect.new(0, 0, 1, 0)) &&
    !(SDL2::Rect.new(0, 0, 1, 1) != SDL2::Rect.new(0, 0, 1, 1)) &&
    SDL2::Rect.new(1, 2, 3, 4) != SDL2::Rect.new(1, 2, 1, 2)
  end
  assert('SDL2::Rect.union') do
    u = SDL2::Rect.new(0, 0, 100, 100).union(SDL2::Rect.new(-10, -10, 20, 20))
    u.x == -10 && u.y == -10 && u.w == 110 && u.h == 110
  end
ensure
  SDL2::quit
end
