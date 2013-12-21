var i, a, b: integer;
function mod(x, y: integer): integer;
	begin
		mod := x - x / y * y
	end;
begin
	for i:= 100 to 999 do
		begin
			a := i / 100;
			b := mod(i, 10);
			if a = b then write(" ", i)
		end
end.
