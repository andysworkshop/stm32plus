using System;
using System.Drawing;
using System.IO;

namespace bm2rgbi {
 
  /// <summary>
  /// Converter for 64K colours
  /// </summary>
  
  public class ILI9325AConverter64 : ILI9325AConverter, IBitmapConverter {

    /// <summary>
    /// Convert to 565 format
    /// </summary>

    public void convert(Bitmap bm, FileStream fs)
    {

      int x, y, value, r, g, b;
      Color c;

      for (y = 0; y < bm.Height; y++)
      {

        for (x = 0; x < bm.Width; x++)
        {

          c = bm.GetPixel(x, y);

          // convert to 565

          r = c.R >> 3;
          g = c.G >> 2;
          b = c.B >> 3;

          value = r << 11 | g << 5 | b;

          // little-endian output

          fs.WriteByte(Convert.ToByte(value >> 8));
          fs.WriteByte(Convert.ToByte(value & 0xFF));
        }
      }
    }
  }
}
