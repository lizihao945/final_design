var n: integer;
function fab(x: integer): integer;
begin
	if x = 0 then
		fab := 1
	else if x = 1 then
		fab := 1
	else fab := fab(x - 2) + fab(x - 1)
end;
begin
	while 1 = 1 do
		begin
			read(n);
			write("answer:", fab(n));
			write("!")
		end
end.
