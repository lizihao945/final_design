var x,y,g,m,z:integer;
function mymod(fArg1,fArg2:integer): integer; 
	begin 
		fArg1 := fArg1 - fArg1 / fArg2 * fArg2 ; 
		mymod := fArg1
	end; 
begin 
	x := 10;
	y := 3;
	write(mymod(x, y))
end.

