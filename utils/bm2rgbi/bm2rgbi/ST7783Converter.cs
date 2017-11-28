using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to ST7783 internal format
  /// </summary>

    abstract public class ST7783Converter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 64:
          return new ST7783Converter64();

        case 262:
          return new ST7783Converter262();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the ST7783");
      }
    }
  }
}
