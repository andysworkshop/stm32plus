using System.Drawing;
using System.IO;


namespace bm2rgbi {

    /// <summary>
    /// Specifies the endianness of the target device
    /// </summary>
    public enum Endianness
    {
        /// <summary>
        /// Most significant byte first
        /// </summary>
        BigEndian = 0,
        /// <summary>
        /// Least significant byte first
        /// </summary>
        LittleEndian = 1,
    }

    public interface IBitmapConverter {

    void convert(Bitmap bm,FileStream fs, Endianness ByteOrder);
  }
}
