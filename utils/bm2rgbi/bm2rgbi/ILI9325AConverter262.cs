using System;
using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// 262K colour converter for ILI9325 (Adafruit Arduino shield). This is a packed format:
  /// 00000000000000RR RRRRGGGGGGBBBBBB
  /// </summary>
  
  class ILI9325AConverter262 : ILI9325AConverter, IBitmapConverter {

    /// <summary>
    /// Convert to 666 format
    /// </summary>

    public void convert(Bitmap bm, FileStream fs)
    {

      int x, y, r, g, b;
      Color c;

      for (y = 0; y < bm.Height; y++)
      {

        for (x = 0; x < bm.Width; x++)
        {

          c = bm.GetPixel(x, y);

          // convert to 666

          r = c.R & 0xFC;
          g = c.G & 0xFC;
          b = c.B & 0xFC;

          // the conversion is a tidy stream of bytes

          fs.WriteByte(Convert.ToByte(r));
          fs.WriteByte(Convert.ToByte(g));
          fs.WriteByte(Convert.ToByte(b));
        }
      }
    }
  }
}
