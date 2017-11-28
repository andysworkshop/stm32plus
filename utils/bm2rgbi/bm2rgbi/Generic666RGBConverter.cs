using System;
using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// Format is: 00000000RRRRRR00 GGGGGG00BBBBBB00
  /// </summary>
  
  class Generic666RGBConverter {

    /// <summary>
    /// Convert to 666 format
    /// </summary>

    public void convert(Bitmap bm,FileStream fs) {

      int x,y,r,g,b;
      Color c;

      for(y=0;y<bm.Height;y++) {

        for(x=0;x<bm.Width;x++) {

          c=bm.GetPixel(x,y);

          // convert to 666

          r=c.R & 0xFC;
          g=c.G & 0xFC;
          b=c.B & 0xFC;

          // the conversion is a tidy stream of bytes

          fs.WriteByte(Convert.ToByte(r));
          fs.WriteByte(0);
          fs.WriteByte(Convert.ToByte(b));
          fs.WriteByte(Convert.ToByte(g));
        }
      }
    }
  }
}
