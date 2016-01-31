// -*- mode: javascript; tab-width: 2; indent-tabs-mode: nil; -*-

//------------------------------------------------------------------------------
// This file is part of liblzg.
//
// Copyright (c) 2010 Marcus Geelnard
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Description
// -----------
// This is a JavaScript implementation of the LZG decoder.
//
// Example usage:
//
//   var lzg = new lzgmini();
//   lzg.decode(compressedStr);
//   var str = lzg.getStringUTF8();
//
// The lzgmini class provides a few different methods for retrieving the
// decoded data:
//
//   getByteArray()    - Get the raw (numeric) byte array.
//   getStringLatin1() - Get the data as a string, assuming it is encoded in
//                       8-bit Latin 1 (ISO-8859-1, also works with ASCII).
//   getStringUTF8()   - Get the data as a string, assuming it is encoded in
//                       UTF-8 format.
//------------------------------------------------------------------------------

function lzgmini() {

  // Constants
  var LZG_HEADER_SIZE = 16;
  var LZG_METHOD_COPY = 0;
  var LZG_METHOD_LZG1 = 1;

  // LUT for decoding the copy length parameter
  var LZG_LENGTH_DECODE_LUT = [2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
                               20,21,22,23,24,25,26,27,28,29,35,48,72,128];

  // Decoded data (produced by the decode() method)
  var outdata = null;

  // Calculate the checksum
  var calcChecksum = function(data) {
    var a = 1;
    var b = 0;
    var i = LZG_HEADER_SIZE;
    while (i < data.length)
    {
      a = (a + (data.charCodeAt(i) & 0xff)) & 0xffff;
      b = (b + a) & 0xffff;
      i++;
    }
    return (b << 16) | a;
  }

  // Decode LZG coded data. The function returns the size of the decoded data.
  // Use any of the get* methods to retrieve the decoded data.
  this.decode = function(data) {
    // Start by clearing the decompressed array in this object
    outdata = null;

    // Check magic ID
    if ((data.length < LZG_HEADER_SIZE) || (data.charCodeAt(0) != 76) ||
         (data.charCodeAt(1) != 90) ||  (data.charCodeAt(2) != 71))
    {
      return 0;
    }

    // Calculate & check the checksum
    var checksum = ((data.charCodeAt(11) & 0xff) << 24) |
                   ((data.charCodeAt(12) & 0xff) << 16) |
                   ((data.charCodeAt(13) & 0xff) << 8) |
                   (data.charCodeAt(14) & 0xff);
    if (calcChecksum(data) != checksum)
    {
      return 0;
    }

    // Check which method to use
    var method = data.charCodeAt(15) & 0xff;
    if (method == LZG_METHOD_LZG1)
    {
      // Get marker symbols
      var m1 = data.charCodeAt(16) & 0xff;
      var m2 = data.charCodeAt(17) & 0xff;
      var m3 = data.charCodeAt(18) & 0xff;
      var m4 = data.charCodeAt(19) & 0xff;

      // Main decompression loop
      var symbol, b, b2, b3, len, offset;
      var dst = new Array();
      var dstlen = 0;
      var k = LZG_HEADER_SIZE + 4;
      var datalen = data.length;
      while (k <= datalen)
      {
        symbol = data.charCodeAt(k++) & 0xff;
        if ((symbol != m1) && (symbol != m2) && (symbol != m3) && (symbol != m4))
        {
          // Literal copy
          dst[dstlen++] = symbol;
        }
        else
        {
          b = data.charCodeAt(k++) & 0xff;
          if (b != 0)
          {
            // Decode offset / length parameters
            if (symbol == m1)
            {
              // marker1 - "Distant copy"
              len = LZG_LENGTH_DECODE_LUT[b & 0x1f];
              b2 = data.charCodeAt(k++) & 0xff;
              b3 = data.charCodeAt(k++) & 0xff;
              offset = (((b & 0xe0) << 11) | (b2 << 8) | b3) + 2056;
            }
            else if (symbol == m2)
            {
              // marker2 - "Medium copy"
              len = LZG_LENGTH_DECODE_LUT[b & 0x1f];
              b2 = data.charCodeAt(k++) & 0xff;
              offset = (((b & 0xe0) << 3) | b2) + 8;
            }
            else if (symbol == m3)
            {
              // marker3 - "Short copy"
              len = (b >> 6) + 3;
              offset = (b & 63) + 8;
            }
            else
            {
              // marker4 - "Near copy (incl. RLE)"
              len = LZG_LENGTH_DECODE_LUT[b & 0x1f];
              offset = (b >> 5) + 1;
            }

            // Copy the corresponding data from the history window
            for (i = 0; i < len; i++)
            {
              dst[dstlen] = dst[dstlen-offset];
              dstlen++;
            }
          }
          else
          {
            // Literal copy (single occurance of a marker symbol)
            dst[dstlen++] = symbol;
          }
        }
      }

      // Store the decompressed data in the lzgmini object for later retrieval
      outdata = dst;
      return dstlen;
    }
    else if (method == LZG_METHOD_COPY)
    {
      // Plain copy
      var dst = new Array();
      var dstlen = 0;
      var datalen = data.length;
      for (var i = LZG_HEADER_SIZE; i < datalen; i++)
      {
        dst[dstlen++] = data.charCodeAt(i) & 0xff;
      }
      outdata = dst;
      return dstlen;
    }
    else
    {
      // Unknown method
      return 0;
    }
  }

  // Get the decoded byte array
  this.getByteArray = function()
  {
    return outdata;
  }

  // Get the decoded string from a Latin 1 (or ASCII) encoded array
  this.getStringLatin1 = function()
  {
    var str = "";
    if (outdata != null)
    {
      var charLUT = new Array();
      for (var i = 0; i < 256; ++i)
        charLUT[i] = String.fromCharCode(i);
      var outlen = outdata.length;
      for (var i = 0; i < outlen; i++)
        str += charLUT[outdata[i]];
    }
    return str;
  }

  // Get the decoded string from an UTF-8 encoded array
  this.getStringUTF8 = function()
  {
    var str = "";
    if (outdata != null)
    {
      var charLUT = new Array();
      for (var i = 0; i < 128; ++i)
        charLUT[i] = String.fromCharCode(i);
      var c;
      var outlen = outdata.length;
      for (var i = 0; i < outlen;)
      {
        c = outdata[i++];
        if (c < 128)
        {
          str += charLUT[c];
        }
        else
        {
          if ((c > 191) && (c < 224))
          {
            c = ((c & 31) << 6) | (outdata[i++] & 63);
          }
          else
          {
            c = ((c & 15) << 12) | ((outdata[i] & 63) << 6) | (outdata[i+1] & 63);
            i += 2;
          }
          str += String.fromCharCode(c);
        }
      }
    }
    return str;
  }
}

