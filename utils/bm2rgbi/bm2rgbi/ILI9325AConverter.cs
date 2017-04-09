using System;

namespace bm2rgbi {
  
  /// <summary>
  /// Convert to ILI9325 internal format specific to the Adafruit Arduino shield
  /// </summary>

    abstract public class ILI9325AConverter {

    /// <summary>
    /// Get an instance of the converter 
    /// </summary>

    static public IBitmapConverter createInstance(int depth) {

      switch(depth) {
        case 64:
          return new ILI9325AConverter64();

        case 262:
          return new ILI9325AConverter262();

        default:
          throw new Exception(depth+" is not a recognised colour depth for the ILI9325 (adafruit shield)");
      }
    }
  }
}
