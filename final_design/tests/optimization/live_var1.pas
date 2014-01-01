var a, b, i: integer;
begin
  a := 0;
  i := 0;
  while i < 10 do
    begin
      a := a * i;
      i := i + 1
    end;
  write("a=", a);
  b := a;
  i := 0;
  while i < 100 do
    begin
      b := b + i;
      i := i + 1;
    end;
  write("b=", b);
end.