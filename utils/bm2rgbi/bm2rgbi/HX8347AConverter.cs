using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to HX8347A internal format
  /// </summary>

    abstract public class HX8347AConverter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 64:
          return new HX8347AConverter64();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the HX8347A");
      }
    }
  }
}
