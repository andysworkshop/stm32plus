-- -*- mode: lua; tab-width: 2; indent-tabs-mode: nil; -*-

--------------------------------------------------------------------------------
-- This file is part of liblzg.
--
-- Copyright (c) 2010 Marcus Geelnard
--
-- This software is provided 'as-is', without any express or implied
-- warranty. In no event will the authors be held liable for any damages
-- arising from the use of this software.
--
-- Permission is granted to anyone to use this software for any purpose,
-- including commercial applications, and to alter it and redistribute it
-- freely, subject to the following restrictions:
--
-- 1. The origin of this software must not be misrepresented; you must not
--    claim that you wrote the original software. If you use this software
--    in a product, an acknowledgment in the product documentation would
--    be appreciated but is not required.
--
-- 2. Altered source versions must be plainly marked as such, and must not
--    be misrepresented as being the original software.
--
-- 3. This notice may not be removed or altered from any source
--    distribution.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- Description
-- -----------
-- This is a Lua implementation of the LZG decoder. Here is an example of how
-- to use it in a Lua program:
--
--  require("lzgmini")
--  f = io.open(nameOfFile, "rb")
--  compressedData = f:read("*all")
--  f:close()
--  data = lzgmini.decode(compressedData)
--
-- ...where the resulting data is a string variable (or an empty string, if the
-- decompression failed).
--
-- The performance of this implementation is not as good as the native C version
-- found in the main liblzg library, obviously, but still fairly good. As a
-- rule of thumb, the Lua decoder is about 80 times slower than the optimized C
-- version. Though that may sound very slow, a modern computer (2-3 GHz 64-bit)
-- is able to decompress about 4 MB/s using this Lua implementation.
--
-- For even better performance, you can use LuaJIT. It gives about four times
-- faster deompression compared to traditional Lua.
--------------------------------------------------------------------------------


-- We need string, math and table in this module
local string = string
local math = math
local table = table

module("lzgmini")


-- Constants
local LZG_HEADER_SIZE = 16
local LZG_METHOD_COPY = 0
local LZG_METHOD_LZG1 = 1

-- LUT for decoding the copy length parameter
local LZG_LENGTH_DECODE_LUT = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
                               20,21,22,23,24,25,26,27,28,29,35,48,72,128}

-- Calculate the checksum
local function calcchecksum(data)
  local a = 1
  local b = 0
  local i
  for i = (LZG_HEADER_SIZE + 1),data:len() do
    a = (a + data:byte(i)) % 65536
    b = (b + a) % 65536
  end
  return b * 65536 + a
end

-- Decode LZG coded data
function decode(data)
  -- Check magic ID
  if (data:len() < LZG_HEADER_SIZE) or (data:byte(1) ~= 76) or
     (data:byte(2) ~= 90) or (data:byte(3) ~= 71) then
    return ''
  end

  -- Calculate & check the checksum
  local checksum = data:byte(12) * 16777216 + data:byte(13) * 65536 +
                   data:byte(14) * 256 + data:byte(15)
  if calcchecksum(data) ~= checksum then
    return ''
  end

  -- Check which method to use
  local method = data:byte(16)
  if method == LZG_METHOD_LZG1 then
    -- Get marker symbols
    local m1 = data:byte(17)
    local m2 = data:byte(18)
    local m3 = data:byte(19)
    local m4 = data:byte(20)

    -- Main decompression loop
    local symbol, b, b2, b3, length, offset, copy, i
    local dst = {}
    local dstlen = 0
    local datalen = data:len()
    local k = LZG_HEADER_SIZE + 5
    while k <= datalen do
      symbol = data:byte(k); k = k + 1
      if (symbol ~= m1) and (symbol ~= m2) and (symbol ~= m3) and (symbol ~= m4) then
        -- Literal copy
        dstlen = dstlen + 1
        dst[dstlen] = string.char(symbol)
      else
        b = data:byte(k); k = k + 1
        if b ~= 0 then
          -- Decode offset / length parameters
          if symbol == m1 then
            -- marker1 - "Distant copy"
            length = LZG_LENGTH_DECODE_LUT[b % 32];
            b2 = data:byte(k); k = k + 1
            b3 = data:byte(k); k = k + 1
            offset = math.floor(b / 32) * 65536 + b2 * 256 + b3 + 2056
          elseif symbol == m2 then
            -- marker2 - "Medium copy"
            length = LZG_LENGTH_DECODE_LUT[b % 32];
            b2 = data:byte(k); k = k + 1
            offset = math.floor(b / 32) * 256 + b2 + 8
          elseif symbol == m3 then
            -- marker3 - "Short copy"
            length = math.floor(b / 64) + 3
            offset = (b % 64) + 8
          else
            -- marker4 - "Near copy (incl. RLE)"
            length = LZG_LENGTH_DECODE_LUT[b % 32];
            offset = math.floor(b / 32) + 1
          end

          -- Copy the corresponding data from the history window
          for i = 1,length do
            dstlen = dstlen + 1
            dst[dstlen] = dst[dstlen-offset]
          end
        else
          -- Literal copy (single occurance of a marker symbol)
          dstlen = dstlen + 1
          dst[dstlen] = string.char(symbol)
        end
      end
    end

    -- Collapse the table-of-chars into a string and return it
    return table.concat(dst)
  elseif method == LZG_METHOD_COPY then
    -- Plain copy
    return data:sub(17)
  else
    -- Unknown method
    return ''
  end
end

