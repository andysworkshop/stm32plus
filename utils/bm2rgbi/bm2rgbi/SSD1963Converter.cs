using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to SSD1963 internal format
  /// </summary>
  
  abstract public class SSD1963Converter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 16:
          return new SSD1963Converter16();

        case 262:
          return new SSD1963Converter262();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the SSD1963");
      }
    }
  }
}
