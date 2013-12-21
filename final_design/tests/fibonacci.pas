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
	read(n);
	write(fab(n))
end.
