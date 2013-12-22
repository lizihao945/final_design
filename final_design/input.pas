var a, b: integer;
procedure f1(a: integer; var b: integer);
	begin
		write(a);
		write(b)
	end;
procedure f2(c: integer; var d: integer);
	begin
		write(c);
		write(d)
	end;
begin
	a := 10;
	b := 1;
	f1(a, b);
	f2(a, b)
end.