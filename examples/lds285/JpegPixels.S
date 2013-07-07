	.global JpegTest0Pixels
	.global JpegTest0PixelsSize

JpegTest0Pixels:
	.incbin "jpeg/test0.jpg"
	JpegTest0PixelsSize=.-JpegTest0Pixels
