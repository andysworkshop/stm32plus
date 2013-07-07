using System.Drawing;
using System.IO;

namespace bm2rgbi {
  
  /// <summary>
  /// 262K colour converter for SSD1963. This is a packed format:
  /// 0000RRRRRRRRGGGG 0000GGGGBBBBBBBB
  /// </summary>
  
  class SSD1963Converter262 : SSD1963Converter, IBitmapConverter {

    /// <summary>
    /// Do the conversion. 
    /// </summary>

    public void convert(Bitmap bm,FileStream fs) {

      int x,y;
 
      byte red,green,blue;
      Color c;
      ushort first,second;

      for(y=0;y<bm.Height;y++) {

        for(x=0;x<bm.Width;x++) {

          c=bm.GetPixel(x,y);

          // convert to new format

          red=(byte)c.R;
          green=(byte)c.G;
          blue=(byte)c.B;

          first=(ushort)(((ushort)red << 4) | (green >> 4));
			    second=(ushort)((((ushort)green & 0xf) << 8) | blue);

          fs.WriteByte((byte)(first & 0xff));
          fs.WriteByte((byte)(first >> 8));

          fs.WriteByte((byte)(second & 0xff));
          fs.WriteByte((byte)(second >> 8));
        }
      }
    }
  }
}
