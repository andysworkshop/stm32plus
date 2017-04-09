using System.Drawing;
using System.IO;


namespace bm2rgbi {
  
  public interface IBitmapConverter {

    void convert(Bitmap bm,FileStream fs);
  }
}
