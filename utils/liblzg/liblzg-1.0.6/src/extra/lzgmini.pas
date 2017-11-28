(* -*- mode: pascal; tab-width: 2; indent-tabs-mode: nil; -*- *)
unit lzgmini;

(*
* This file is part of liblzg.
*
* Copyright (c) 2010 Marcus Geelnard
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*)

(*
* Description
* -----------
* This is a Pascal implemention of the LZG decoder. It has been tested with the
* Free Pascal compiler (http://www.freepascal.org), and should work with
* Delphi as well (possibly with some modifications?).
*
* This version of the decoder is not quite as optimized as the routine in the
* original LZG library, but may be preferred nevertheless in Pascal
* environments.
*)


interface

type
  PByte = ^Byte;

function LZG_DecodedSize(const data: PByte; size: Cardinal): Cardinal;
function LZG_Decode(const indata: PByte; insize: Cardinal; outdata: PByte; outsize: Cardinal): Cardinal;


implementation

const
  { Internal definitions }
  LZG_HEADER_SIZE = 16;
  LZG_METHOD_COPY = 0;
  LZG_METHOD_LZG1 = 1;

  { LUT for decoding the copy length parameter }
  LZG_LENGTH_DECODE_LUT: array[0..31] of Byte = (
    2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
    18,19,20,21,22,23,24,25,26,27,28,29,35,48,72,128
  );


{ Endian and alignment independent reader for 32-bit integers }
function LZG_GetUINT32(data: PByte; offset: Cardinal): Cardinal;
begin
  LZG_GetUINT32 := (Cardinal(data[offset]) << 24) or
                   (Cardinal(data[offset+1]) << 16) or
                   (Cardinal(data[offset+2]) << 8) or
                   Cardinal(data[offset+3]);
end;

{ Calculate the checksum }
function LZG_CalcChecksum(const data: PByte; size: Cardinal): Cardinal;
var
  a, b: Word;
  i: Cardinal;
begin
  a := 1;
  b := 0;
  for i := 0 to size-1 do
  begin
    a := a + Word(data[i]);
    b := b + a;
  end;
  LZG_CalcChecksum := (Cardinal(b) << 16) or a;
end;

{ Determine the size of the decoded data for a given LZG coded buffer }
function LZG_DecodedSize(const data: PByte; size: Cardinal): Cardinal;
begin
  { Check header }
  if (size < 7) or (data[0] <> Byte('L')) or (data[1] <> Byte('Z')) or
     (data[2] <> Byte('G')) then
  begin
    LZG_DecodedSize := 0;
    Exit;
  end;

  { Get output buffer size }
  LZG_DecodedSize := LZG_GetUINT32(data, 3);
end;

{ Decode LZG coded data }
function LZG_Decode(const indata: PByte; insize: Cardinal; outdata: PByte;
  outsize: Cardinal): Cardinal;
var
  src, inEnd, dst, outEnd, copy: PByte;
  symbol, b, b2, m1, m2, m3, m4, method: Byte;
  i, length, offset, encodedSize, decodedSize, checksum: Cardinal;
begin
  LZG_Decode := 0;

  { Check magic ID }
  if (insize < LZG_HEADER_SIZE) or (indata[0] <> Byte('L')) or
     (indata[1] <> Byte('Z')) or (indata[2] <> Byte('G')) then
    Exit;

  { Get header data }
  decodedSize := LZG_GetUINT32(indata, 3);
  encodedSize := LZG_GetUINT32(indata, 7);
  checksum := LZG_GetUINT32(indata, 11);

  { Check sizes }
  if (outsize < decodedSize) or (encodedSize <> (insize - LZG_HEADER_SIZE)) then
    Exit;

  { Check checksum }
  if LZG_CalcChecksum(@indata[LZG_HEADER_SIZE], encodedSize) <> checksum then
    Exit;

  { Initialize the byte streams }
  src := indata + LZG_HEADER_SIZE;;
  inEnd := indata + insize;
  dst := outdata;
  outEnd := outdata + outsize;

  { Check which method to use }
  method := indata[15];
  if method = LZG_METHOD_LZG1 then
  begin
    { Get marker symbols }
    if not ((src + 4) <= inEnd) then Exit;
    m1 := src[0];
    m2 := src[1];
    m3 := src[2];
    m4 := src[3];
    src := src + 4;

    { Main decompression loop }
    while src < inEnd do
    begin
      symbol := src^;
      Inc(src);

      if (symbol <> m1) and (symbol <> m2) and (symbol <> m3) and (symbol <> m4) then
      begin
        { Literal copy }
        if not (dst < outEnd) then Exit;
        dst^ := symbol;
        Inc(dst);
      end
      else
      begin
        if not (src < inEnd) then Exit;
        b := src^;
        Inc(src);
        if b <> 0 then
        begin
          if symbol = m1 then
          begin
            { marker1 - "Distant copy" }
            if not ((src + 2) <= inEnd) then Exit;
            length := LZG_LENGTH_DECODE_LUT[b and $1f];
            b2 := src^;
            Inc(src);
            offset := ((Cardinal(b and $e0) << 11) or
                       (Cardinal(b2) << 8) or
                       src^) + 2056;
            Inc(src);
          end
          else if symbol = m2 then
          begin
            { marker2 - "Medium copy" }
            if not (src < inEnd) then Exit;
            length := LZG_LENGTH_DECODE_LUT[b and $1f];
            b2 := src^;
            Inc(src);
            offset := ((Cardinal(b and $e0) << 3) or b2) + 8;
          end
          else if symbol = m3 then
          begin
            { marker3 - "Short copy" }
            length := Cardinal(b >> 6) + 3;
            offset := Cardinal(b and $3f) + 8;
          end
          else
          begin
            { marker4 - "Near copy (incl. RLE)" }
            length := LZG_LENGTH_DECODE_LUT[b and $1f];
            offset := Cardinal(b >> 5) + 1;
          end;

          { Copy the corresponding data from the history window }
          copy := dst - offset;
          if not ((copy >= outdata) and ((dst + length) <= outEnd)) then Exit;
          for i := 0 to length-1 do
          begin
            dst^ := copy^;
            Inc(copy);
            Inc(dst);
          end;
        end
        else
        begin
          { Literal copy (single occurance of a marker symbol) }
          if not (dst < outEnd) then Exit;
          dst^ := symbol;
          Inc(dst);
        end;
      end;
    end;
  end
  else if method = LZG_METHOD_COPY then
  begin
    { Plain copy }
    while (src < inEnd) and (dst < outEnd) do
    begin
      dst^ := src^;
      Inc(src);
      Inc(dst);
    end;
  end;

  { All OK? }
  if Cardinal(dst - outdata) = decodedSize then
      LZG_Decode := decodedSize;
end;

end.

