;-------------------------------------------------------------------------------
; This file is part of liblzg.
;
; Copyright (c) 2010 Marcus Geelnard
;
; This software is provided 'as-is',without any express or implied
; warranty. In no event will the authors be held liable for any damages
; arising from the use of this software.
;
; Permission is granted to anyone to use this software for any purpose,
; including commercial applications,and to alter it and redistribute it
; freely,subject to the following restrictions
;
; 1. The origin of this software must not be misrepresented; you must not
;    claim that you wrote the original software. If you use this software
;    in a product,an acknowledgment in the product documentation would
;    be appreciated but is not required.
;
; 2. Altered source versions must be plainly marked as such,and must not
;    be misrepresented as being the original software.
;
; 3. This notice may not be removed or altered from any source
;    distribution.
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; Description
; -----------
; This is an assembly language implemention of the LZG decoder for the NMOS 6502
; processor (and variants). It was tested with the xa65 assembler
; (http://www.floodgap.com/retrotech/xa/).
;
; This implementation is a bit crude, with the following limitations
;  - No "safe mode" (no checking for corrupted data, no checksum calculation).
;  - No data integrity checking.
;  - Does not support buffer sizes or offsets larger than 65535 (which is kind
;    of pointless with a 16-bit address space anyway).
;  - Does not support the COPY mode (just LZG1 compressed mode).
;
; The compiled routine is about 360 bytes, and on a stock Commodore 64 (1 MHz
; 6510), this routine decompresses about 25 KB/s.
;
; Tips
;  - This routine can be tailored in several ways for more optimal operation.
;    One idea is to use a small circular buffer for the encoded data, for
;    instance for decompressing the data while loading it from disk.
;  - With careful planning, you can overlap input and output buffers (put the
;    encoded buffer at the end of the decoded buffer, plus a slight offset to
;    accomodate for potential over runs).
;  - On a C64, you can add an "sta $d020" instruction next to the "sta _symbol"
;    instruction for that funky retro look...
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
; Memory addresses
; NOTE These are all located in the zero page. They were chosen according to
; good practices for the Commodore 64 (unused and temporary memory locations).
; On other machines, other memory locations may be more suitable.
;-------------------------------------------------------------------------------

	; Function arguments (modified by the function)
	_in = 26	; 16 bits (compressed data)
	_out = 28	; 16 bits (decompressed data)

	; Local variables for this routine
	_inEnd = 2
	_offset = 4
	_length = 6
	_symbol = 25
	_marker1 = 30
	_marker2 = 31
	_marker3 = 32
	_marker4 = 33
	_copy = 34


;-------------------------------------------------------------------------------
; LZG_Decode - Decode a compressed memory block
;-------------------------------------------------------------------------------

	.text

LZG_Decode
	; Caclulate the end of the input buffer (encoded size must be <= 65535)
	clc
	ldy	#10
	lda	(_in),y
	adc	_in
	sta	_inEnd
	dey
	lda	(_in),y
	adc	_in+1
	sta	_inEnd+1
	clc
	lda	_inEnd
	adc	#16
	sta	_inEnd
	lda	_inEnd+1
	adc	#0
	sta	_inEnd+1

	; Get the marker symbols
	ldy	#16
	lda	(_in),y
	sta	_marker1
	iny
	lda	(_in),y
	sta	_marker2
	iny
	lda	(_in),y
	sta	_marker3
	iny
	lda	(_in),y
	sta	_marker4

	; Skip header + marker symbols (16 + 4 bytes)
	clc
	lda	_in
	adc	#20
	sta	_in
	lda	_in+1
	adc	#0
	sta	_in+1

	; Main decompression loop
	ldy	#0				; Make sure that Y is zero
mainloop
	lda	_in				; done?
	cmp	_inEnd
	bne	notdone
	lda	_in+1
	cmp	_inEnd+1
	bne	notdone
	rts
notdone
	lda	(_in),y				; A = symbol
	sta	_symbol
	inc	_in
	bne	noinc1
	inc	_in+1
noinc1
	cmp	_marker1			; Marker1?
	beq	domarker1
	cmp	_marker2			; Marker2?
	beq	domarker2
	cmp	_marker3			; Marker3?
	beq	domarker3
	cmp	_marker4			; Marker4?
	beq	domarker4
literal
	lda	_symbol
	sta	(_out),y			; Plain copy
	inc	_out
	bne	mainloop
	inc	_out+1
	bne	mainloop

domarker1
	jmp	domarker1b

	; marker4 - "Near copy (incl. RLE)"
domarker4
	lda	(_in),y
	inc	_in
	bne	noinc3
	inc	_in+1
noinc3
	cmp	#0
	beq	literal				; Single occurance of the marker symbol (rare)
	tax
	lsr
	lsr
	lsr
	lsr
	lsr
	sta	_offset
	inc	_offset
	lda	#0
	sta	_offset+1			; offset = (b >> 5) + 1
	txa
	and	#$1f
	tax
	lda	_LZG_LENGTH_DECODE_LUT,x
	sta	_length				; length = _LZG_LENGTH_DECODE_LUT[b & 0x1f]
	jmp	docopy

	; marker3 - "Short copy"
domarker3
	lda	(_in),y
	inc	_in
	bne	noinc4
	inc	_in+1
noinc4
	cmp	#0
	beq	literal				; Single occurance of the marker symbol (rare)
	tax
	lsr
	lsr
	lsr
	lsr
	lsr
	lsr
	clc
	adc	#3
	sta	_length				; length = (b >> 6) + 3
	txa
	and	#$3f
	adc	#8
	sta	_offset
	lda	#0
	sta	_offset+1			; offset = (b & 0x3f) + 8
	beq	docopy

	; marker2 - "Medium copy"
domarker2
	lda	(_in),y
	inc	_in
	bne	noinc5
	inc	_in+1
noinc5
	cmp	#0
	beq	literal				; Single occurance of the marker symbol (rare)
	tax
	lsr
	lsr
	lsr
	lsr
	lsr
	sta	_offset+1
	lda	(_in),y
	inc	_in
	bne	noinc6
	inc	_in+1
noinc6
	clc
	adc	#8
	sta	_offset
	bcc	noinc7
	inc	_offset+1			; offset = (((b & 0xe0) << 3) | b2) + 8
noinc7
	txa
	and	#$1f
	tax
	lda	_LZG_LENGTH_DECODE_LUT,x
	sta	_length				; length = _LZG_LENGTH_DECODE_LUT[b & 0x1f]
	bne	docopy

literal2
	jmp	literal

	; marker1 - "Distant copy"
domarker1b
	lda	(_in),y
	inc	_in
	bne	noinc8
	inc	_in+1
noinc8
	cmp	#0
	beq	literal2			; Single occurance of the marker symbol (rare)
	and	#$1f
	tax
	lda	_LZG_LENGTH_DECODE_LUT,x
	sta	_length				; length = _LZG_LENGTH_DECODE_LUT[b & 0x1f]
	lda	(_in),y
	inc	_in
	bne	noinc9
	inc	_in+1
noinc9
	sta	_offset+1
	lda	(_in),y
	inc	_in
	bne	noinc10
	inc	_in+1
noinc10
	clc
	adc	#$08
	sta	_offset
	lda	_offset+1
	adc	#$08
	sta	_offset+1			; offset = ((b2 << 8) | (*src++)) + 2056

	; Copy corresponding data from history window
docopy
	sec
	lda	_out
	sbc	_offset
	sta	_copy
	lda	_out+1
	sbc	_offset+1
	sta	_copy+1
loop1	lda	(_copy),y
	sta	(_out),y
	iny
	cpy	_length
	bne	loop1
	ldy	#0				; Make sure that Y is zero

	clc
	lda	_out
	adc	_length
	sta	_out
	bcc	noinc11
	inc	_out+1
noinc11
	jmp	mainloop


;-------------------------------------------------------------------------------
; LUT for decoding the copy length parameter
;-------------------------------------------------------------------------------

_LZG_LENGTH_DECODE_LUT
	.byt	2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
	.byt	18,19,20,21,22,23,24,25,26,27,28,29,35,48,72,128

