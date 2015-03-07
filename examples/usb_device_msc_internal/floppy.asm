/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

 	.global FloppyImage
	.global FloppyImageSize

FloppyImage:
	.incbin "floppy.img"
	FloppyImageSize=.-FloppyImage
