using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// 16M colour converter for R61523. This is a packed format:
  /// BBBBBBBB GGGGGGGG RRRRRRRR
  /// </summary>
  
  class R61523Converter16 : R61523Converter, IBitmapConverter {

    /// <summary>
    /// Do the conversion. 
    /// </summary>

    public void convert(Bitmap bm,FileStream fs, Endianness ByteOrder) {

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

          if (ByteOrder == Endianness.BigEndian)
          {
            // big-endian output
            fs.WriteByte(g);
            fs.WriteByte(r);
            fs.WriteByte(0);
            fs.WriteByte(b);
          }
          else
          {
            // little-endian output
            fs.WriteByte(b);
            fs.WriteByte(0);
            fs.WriteByte(r);
            fs.WriteByte(g);
          }
        }
      }
    }
  }
}
