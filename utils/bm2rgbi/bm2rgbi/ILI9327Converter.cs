using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to ILI9327 internal format
  /// </summary>
  
  abstract public class ILI9327Converter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 64:
          return new ILI9327Converter64();

        case 262:
          return new ILI9327Converter262();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the ILI9327");
      }
    }
  }
}
