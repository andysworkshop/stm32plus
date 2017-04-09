using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to HX8352A internal format
  /// </summary>

    abstract public class HX8352AConverter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 64:
          return new HX8352AConverter64();

        case 262:
          return new HX8352AConverter262();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the HX8352A");
      }
    }
  }
}
