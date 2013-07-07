using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to LDS285 internal format
  /// </summary>
  
  abstract public class LDS285Converter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 16:
          return new LDS285Converter16();

        case 262:
          return new LDS285Converter262();
        
        case 64:
          return new LDS285Converter64();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the LDS285");
      }
    }
  }
}
