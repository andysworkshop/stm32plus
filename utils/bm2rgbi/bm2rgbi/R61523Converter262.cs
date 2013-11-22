using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// 262K colour converter for R61523. This is a packed format:
  /// BBBBBB00 GGGGGG00 RRRRRR00
  /// </summary>
  
  class R61523Converter262 : R61523Converter, IBitmapConverter {

    /// <summary>
    /// Do the conversion. 
    /// </summary>

    public void convert(Bitmap bm,FileStream fs) {

      int x,y;
      byte r,g,b;
      Color c;

      for(y=0;y<bm.Height;y++) {

        for(x=0;x<bm.Width;x++) {

          c=bm.GetPixel(x,y);

          // convert to 666

          r=(byte)(c.R & 0xFC);
          g=(byte)(c.G & 0xFC);
          b=(byte)(c.B & 0xFC);

          fs.WriteByte(b);
          fs.WriteByte(0);
          fs.WriteByte(r);
          fs.WriteByte(g);
        }
      }
    }
  }
}
