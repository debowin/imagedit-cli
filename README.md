# imagedit-cli
Fundamentals of Computer Graphics Project #1

A	small	number	of	starter	images	are	included	with	the	code,	but	you	can	find	more
in	your	personal	collection	or	online	(https://www.google.com/imghp).	The	images
you	use	must	be	one	of	the	supported	formats	(.bmp,	.jpg/.jpeg,	.png,	.tga).

## How	the	Program	Works:

The	program	runs	on	the	command	line	and
performs	operation	in	the	order	that	they	appear	in	the	arguments.	For	example,	to
increase	the	brightness	of	the	image	in.bmp	by	10%,	and	save	the	result	in	the
image	out.bmp,	you	would	type:

`image -input in.bmp -brightness 1.1 -output out.bmp`

Notice	the	input	parameter	must	appear	first.	Remember,	everything	happens	in
the	order	specified.	First	the	input,	then	the	brightness	change,	then	the	writing	to
the	specified	output	file.

For	several	of	the	filters,	there	is	more	than	one	corresponding	argument.	To	see	the
complete	list	of	options,	type:

`image –help`



If	you	specify	more	than	one	option,	the	options	are	processed	in	the	order	that	they
are	encountered.	For	example,

`image -input in.bmp -contrast 0.8 -scale 0.5 0.5 –output out.bmp`

would	first	decrease	the	contrast	of	the	input	image	by	20%,	and	then	scale	down	the
result	by	50%	in	both	x	and	y	directions.	It	is	also	possible	to	specify	-output
multiple	times,	to	save	out	intermediate	results:

`image -input in.bmp -blur 5 -output blurred.bmp -edgeDetect
-output edges.bmp -rotate 30 -output allCombined.bmp`