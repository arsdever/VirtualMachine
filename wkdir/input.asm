assign		a2, 10h
load		r0, a2
assign		a3, 14h
load		r4, a3
div			r0, r4
assign		a3, 1ch
store		r0, a3
load		r0, a2
mul			r0, r4
assign		a3, 18h
store		r0, a3
load		r0, a2
sub			r0, r4
assign		a3, 14h
store		r0, a3
load		r0, a2
add			r0, r4
store		r0, a2
call		function
halt

function:
assign		a2, 10h
load		r0, a2
assign		a3, 14h
load		r4, a3
add			r0, r4
store		ro, a2
ret