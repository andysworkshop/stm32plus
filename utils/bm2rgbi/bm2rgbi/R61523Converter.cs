using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to R61523 internal format
  /// </summary>
  
  abstract public class R61523Converter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 64:
          return new R61523Converter64();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the R61523");
      }
    }
  }
}
