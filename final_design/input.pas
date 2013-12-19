const val='x';
var x: array[10] of integer;
function foo: char;
	begin
		foo := val
	end;
begin
	x[0] := foo;
	write(x[0])
end.