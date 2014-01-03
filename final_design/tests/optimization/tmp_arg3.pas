function foo: integer;
	var x: integer;
	procedure bar;
		begin
			x := 1;
		end;
	begin
		bar;
		foo := x;
	end;

begin
	write(foo);
end.
