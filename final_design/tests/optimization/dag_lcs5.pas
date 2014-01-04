var a, b, x, y, z: integer;
begin
	b := 10;
	a := (a + b) - (a + b);
	if a < b then
		begin
		  x := 6;
		  y := 3;
		  z := ((x + y) - ((x + y) * (x - y))) + ((x + y) * (x - y));
		  write(z)
		end;
end.