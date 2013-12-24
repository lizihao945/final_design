var x, y, z: integer;
begin
  x := 6;
  y := 3;
  z := ((x + y) - ((x + y) * (x - y))) + ((x + y) * (x - y));
  write(z)
end.