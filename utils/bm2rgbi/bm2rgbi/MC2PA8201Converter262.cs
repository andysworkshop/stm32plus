using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// 262K colour converter for MC2PA8201. This is a packed format:
  /// RRRRRR00 BBBBBB00 GGGGGG00
  /// </summary>
  
  class MC2PA8201Converter262 : MC2PA8201Converter, IBitmapConverter {

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

          fs.WriteByte(r);
          fs.WriteByte(g);
          fs.WriteByte(b);
        }
      }
    }
  }
}
