const n	= 10;
var a: array[10] of integer;
	sp, left, right, seed: integer;
	stack: array[100] of integer;
procedure printarray;
	var i: integer;
	begin 
		i:= 1; 
		while i <= n do
			begin 
				write(a[i]);  
				i:= i + 1 
			end
	end;
procedure initialize; 
	var i: integer;
	begin 
		i:= 1;
		while i <= n do
			begin 
				a[i] := 10 - i;
				i := i + 1
			end
	end;
procedure quicksort; 
	var i, j, x, w, middle: integer;
	begin
		i := left;
		j:= right; 
		middle := (left + right) / 2;
		x:= a[middle];
		while i < j do 
			begin 
				while a[i] < x do i:= i + 1; 
				while a[j] > x do j:= j - 1; 
				if i <= j then
					begin 
						w:= a[i];
						a[i]:= a[j];
						a[j]:= w; 
						i:= i + 1; 
						j:= j - 1
					end
			end;
		if left < j then
			begin 
				sp := sp + 1; 
                stack[sp] := right; 
                right := j; 
                quicksort; 
                right := stack[sp];
                sp := sp - 1
            end;
		if i < right then
			begin 
				sp := sp + 1; 
	            stack[sp] := left; 
	            left := i; 
				quicksort; 
	            left := stack[sp];
	            sp := sp - 1
            end
	end; 
begin 
	initialize;
	write("before:");
	printarray;
	sp := 0; 
	left := 1;
	right := n;
	quicksort;
	write("after:");
	printarray
end. 