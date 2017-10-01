# imagedit-cli
Command Line Image Editor

A	small	number	of	starter	images	are	included	with	the	code,	but	you	can	find	more
in	your	personal	collection	or	online	(https://www.google.com/imghp).	The	images
you	use	must	be	one	of	the	supported	formats	(.bmp,	.jpg/.jpeg,	.png,	.tga).

## Showcase

### [Mangekyō Sharingan](http://naruto.wikia.com/wiki/Mangeky%C5%8D_Sharingan)

|  Original Mangekyō Sharingan         |  Mangekyō Sharingan++          | 
|:-------------------------:|:-------------------------:|
|![](sample/sharingan2.jpg) | ![](output/kamui2.png)  |

### [Kamui](http://naruto.wikia.com/wiki/Kamui)

|  Original          |  Kamui          | 
|:-------------------------:|:-------------------------:|
|![](sample/sharingan.jpg) | ![](output/kamui.png)  |

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
__________________
```
Usage: image -input <filename> [-option [arg ...] ...] -output <filename>
-help
-input <file>
-output <file>
-noise <factor>
-brightness <factor>
-contrast <factor>
-saturation <factor>
-crop <x> <y> <width> <height>
-extractChannel <channel no>
-quantize <nbits>
-randomDither <nbits>
-blur <maskSize>
-sharpen <maskSize>
-edgeDetect
-orderedDither <nbits>
-FloydSteinbergDither <nbits>
-scale <sx> <sy>
-rotate <angle>
-fun <twistFactor>
-sampling <method no>
```

If	you	specify	more	than	one	option,	the	options	are	processed	in	the	order	that	they
are	encountered.	For	example,

`image -input in.bmp -contrast 0.8 -scale 0.5 0.5 –output out.bmp`

would	first	decrease	the	contrast	of	the	input	image	by	20%,	and	then	scale	down	the
result	by	50%	in	both	x	and	y	directions.	It	is	also	possible	to	specify	-output
multiple	times,	to	save	out	intermediate	results:

`image -input in.bmp -blur 5 -output blurred.bmp -edgeDetect
-output edges.bmp -rotate 30 -output allCombined.bmp`

## Features

### Brightness

|  Original          |  Brigther (1.5)          | Darker (0.5) |
|:-------------------------:|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/bright.png)    | ![](output/dim.png)   |

### Contrast

|  Original          |  High Contrast (1.5)          | Low Contrast (0.5) | Negative Contrast (-1) |
|:-------------------------:|:-------------------------:|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/contrast_hi.png)    | ![](output/contrast_lo.png)   | ![](output/contrast_negative.png) |

### Saturation

|  Original          |  High Saturation (2)          | Low Saturation (0.5) | Negative Saturation (-1) |
|:-------------------------:|:-------------------------:|:-------------------------:|:-------------------------:
|![](sample/fruitsplus.jpg) | ![](output/saturation_high.png)    | ![](output/saturation_low.png)   |  ![](output/saturation_negative.png) |

### Random Noise

|  Original          |  More Noise (0.75)          | Less Noise (0.25) |
|:-------------------------:|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/noisy.png)    | ![](output/noisy2.png)   |

### Extract Channel

|  Original          |  Extract Green (2)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/extract_red.png)  |

### Crop

|  Original          |  Cropped (243 281 252 104)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/crop.png)  |

### Quanitze

|  Original          |  Quantize (2)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/quantized.png)  |

### Random Dither

|  Original          |  Random Dithered (2)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/randomDithered.png)  |

### Ordered Dither

|  Original          |  Ordered Dithered (2)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/orderedDithered.png)  |

### Floyd Steinberg Dither

|  Original          |  Floyd Steinberg Dithered (2)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/fsDithered.png)  |

### Blur

|  Original          |  Blurred (2)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/blurry.png)  |

### Sharpen

|  Original          |  Sharpened (2)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/sharpy.png)  |

### Edge Detect

|  Original          |  Edge Detections          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/edgy.png)  |

### Scale Up

|  Original          |  Scale (2 3)          | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/scaled_up.png)  |

### Nearest Neighbour Sampling with Scale Down

|  Original          |  Sampling (1) | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/scaled_down.png)  | 

> with Scale (0.25 0.40)

### Bilinear Interpolation Sampling with Scale Down

|  Original          |  Sampling (2) | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/scaled_down_bilinear.png)  |

> with Scale (0.25 0.40)

### Gaussian Sampling with Scale Down

|  Original          |  Sampling (3)   | 
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/scaled_down_gaussian.png)  |

> with Scale (0.25 0.40)

### Rotate with Gaussian Sampling

|  Original          |  Rotate (30)   | Rotate (150)   |
|:-------------------------:|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/rotate_30.png)  | ![](output/rotate_150.png) |

### Swirl with Gaussian Sampling

|  Original          |  Swirl (128)   |
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/swirly.png)  |

### Combination Filter

|  Original          |  Combined Filter   |
|:-------------------------:|:-------------------------:|
|![](sample/fruitsplus.jpg) | ![](output/allCombined.png)  |


`image -input fruitsplus.jpg -blur 5 -contrast 1.5 -sampling 3 -rotate 30 -fun 48 -randomDither 2 -output allCombined.png`
