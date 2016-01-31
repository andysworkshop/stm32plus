using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// 64K colour converter for MC2PA8201. This is a packed format:
  /// BBBBBGGG GGGRRRRR
  /// </summary>
  
  class MC2PA8201Converter64 : MC2PA8201Converter, IBitmapConverter {

    /// <summary>
    /// Do the conversion. 
    /// </summary>

    public void convert(Bitmap bm,FileStream fs) {

      int x,y;
      byte r,g,b,b1,b2;
      Color c;

      for(y=0;y<bm.Height;y++) {

        for(x=0;x<bm.Width;x++) {

          c=bm.GetPixel(x,y);

          // convert to 888

          r=(byte)(c.R & 0xf8);
          g=(byte)(c.G & 0xfc);
          b=(byte)(c.B & 0xf8);

      	  b1=(byte)(r | (g >> 5));
      	  b2=(byte)((g << 3) | (b >> 3));

          fs.WriteByte(b1);
          fs.WriteByte(b2);
        }
      }
    }
  }
}
