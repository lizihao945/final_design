var x,y,g,m,z:integer;
    i:integer; 
    a,b:integer;
    c:array[3] of integer; 
procedure swap; 
	var temp:integer; 
	begin 
		temp:=x; 
		x:=y; 
		y:=temp 
	end; 
function mymod(var fArg1,fArg2:integer): integer; 
	begin 
		fArg1 := fArg1 - fArg1 / fArg2 * fArg2 ; 
		mymod := fArg1
	end; 
begin 
	for i := 3 downto 1 do 
		begin 
			if i = 3 then 
				begin 
					x := 45; 
					y := 27 
				end;
			if i = 2 then 
				begin 
					x := 0; 
					y := 5 
				end; 
			if i = 1 then 
				begin 
					x := 1; 
					y := 6 
				end; 
			a := x; 
			b := y ; 
			c[i-1] := x + y; 
			z := mymod (x,y); 
			if x <> 0 then 
				begin 
					while x <> 0 do
						begin 
							swap; 
							z := mymod (x,y) 
						end 
				end;
			g:=y; 
			m:= a * b / g; 
			write("GCD: ",g);
			write("LCM: ",m) 
		end; 
	for i := 0 to 2 do 
		begin 
			write(" ",i); 
			write("d",c[i])
		end
end.

