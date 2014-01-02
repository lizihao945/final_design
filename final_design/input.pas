var x, y, z, a, b, i: integer;
begin
  x := a;
  y := b;
  i := 0;
  while i < 100 do
    begin
      z := a * 10;
      x := x + y;
      if x < z then
        x := x - y;
      y := y + 1;
      i := i + 1;
    end;
  write("a=", a);
end.