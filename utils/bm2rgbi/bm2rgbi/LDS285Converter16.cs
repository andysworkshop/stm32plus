using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// 16M colour converter for LDS285. This is an unpacked format:
  /// BBBBBBBB GGGGGGGG RRRRRRRR
  /// </summary>
  
  class LDS285Converter16 : LDS285Converter, IBitmapConverter {

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

          // convert to 888

          r=(byte)c.R;
          g=(byte)c.G;
          b=(byte)c.B;

          fs.WriteByte(r);
          fs.WriteByte(g);
          fs.WriteByte(b);
        }
      }
    }
  }
}
