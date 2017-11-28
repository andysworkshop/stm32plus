using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to MC2PA8201 internal format
  /// </summary>
  
  abstract public class MC2PA8201Converter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 16:
          return new MC2PA8201Converter16();

        case 262:
          return new MC2PA8201Converter262();
        
        case 64:
          return new MC2PA8201Converter64();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the MC2PA8201");
      }
    }
  }
}
