const z=0;
var head, foot, cock, rabbit, n: integer;
begin
	n := z;
	read(head, foot);
	cock := 1;
	while cock <= head do
	begin
		rabbit := head - cock;
		if cock * 2 + rabbit * 4 = foot then
		begin
			write(cock);
			write(rabbit);
			n := n + 1
		end;
		cock := cock + 1
	end;
	if n = 0 then write("0 0")
end.