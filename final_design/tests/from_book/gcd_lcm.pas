const a = 45, b = 27;
var x, y, g, m: integer;
procedure swap;
	var temp: integer;
	begin
		temp := x;
		x := y;
		y := temp
	end;
procedure mod;
	begin
		x := x - x / y * y
	end;
begin
	x := a;
	y := b;
	mod;
	while x <> 0 do
		begin
			swap;
			mod
		end;
	g := y;
	m := a * b / g;
	write(g, m)
end.
