var i: integer;
function fab(x: integer): integer;
begin
	if x = 0 then
		fab := 0
	else if x = 1 then
		fab := 1
	else fab := fab(x - 2) + fab(x - 1)
end;
begin
	for i := 0 to 9 do
		begin
			write(fab(i));
			write(" ")
		end
end.
