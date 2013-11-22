using System.Drawing;
using System.IO;

namespace bm2rgbi {
 
  /// <summary>
  /// Converter for 64K colours
  /// </summary>
  
  public class ST7783Converter64 : ST7783Converter, IBitmapConverter {
  
    /// <summary>
    /// Use the generic converter
    /// </summary>
    
    private Generic565Converter _converter=new Generic565Converter();

    /// <summary>
    /// Do the conversion
    /// </summary>

    public void convert(Bitmap bm,FileStream fs) {
      _converter.convertRGB(bm,fs);
    }
  }
}
