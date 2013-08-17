using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// 262K colour converter for HX8352A. This is a packed format:
  /// 00000000000000RR RRRRGGGGGGBBBBBB
  /// </summary>
  
  class HX8352AConverter262 : HX8352AConverter, IBitmapConverter {

    /// <summary>
    /// Do the conversion. 
    /// </summary>

    public void convert(Bitmap bm,FileStream fs) {

      int x,y,r,g,b,value;
      Color c;

      for(y=0;y<bm.Height;y++) {

        for(x=0;x<bm.Width;x++) {

          c=bm.GetPixel(x,y);

          // convert to 666

          r=c.R & 0xFC;
          g=c.G & 0xFC;
          b=c.B & 0xFC;

          value = r<<10;
          value |= g << 4;            // G into bits 6-11
          value |= b >> 2;            // B into bits 0-5

          fs.WriteByte((byte) (value >>16));
          fs.WriteByte(0);

          fs.WriteByte((byte) (value & 0xff));
          fs.WriteByte((byte) ((value >> 8) & 0xff));
        }
      }
    }
  }
}
