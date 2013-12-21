const
  m =  7,
  n = 85;
 
var
  x, y, z, q, r: integer;
 
function odd(x: integer): integer;
begin
  if x / 2 * x = x then
    odd := 1
  else
    odd := 0
end;

procedure multiply;
var a, b: integer;
 
begin
  a := x;
  b := y;
  z := 0;
  while b > 0 do begin
    if odd(b) = 1 then z := z + a;
    a := 2 * a;
    b := b / 2
  end;
  write("multiply");
  write("x=", x);
  write("y=", y);
  write("z=", z)
end;
 
procedure divide;
var w: integer;
begin
  r := x;
  q := 0;
  w := y;
  while w <= r do w := 2 * w;
  while w > y do begin
    q := 2 * q;
    w := w / 2;
    if w <= r then begin
      r := r - w;
      q := q + 1
    end
  end;
  write("divide");
  write("x=", x);
  write("y=", y);
  write("q=", q);
  write("r=", r)
end;
 
begin
  x := m;
  y := n;
  multiply;
  x := 25;
  y :=  3;
  divide
end.