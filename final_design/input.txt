procedure foo;
	var a: integer;
	procedure bar;
		var b: integer;
		procedure test;
			var c: integer;
			begin
				c := a
			end;
		begin
			test
		end;
	begin
		bar
	end;
procedure x;
	var a:integer;
	begin
		a:=1
	end;
begin
	foo
end.