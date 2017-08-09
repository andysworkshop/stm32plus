using System;
using System.Drawing;

namespace bm2rgbi
{

    /// <summary>
    /// program args class
    /// </summary>

    class ProgramArgs
    {

        /// <summary>
        /// properties
        /// </summary>

        public string InputBitmapFilename { get; private set; }
        public string OutputBitmapFilename { get; private set; }
        public IBitmapConverter TargetDevice { get; private set; }
        public bool Compress { get; private set; }
        public Endianness ByteOrder { get; private set; }
        public RotateFlipType Flip { get; private set; }


        /// <summary>
        /// constructor
        /// </summary>

        public ProgramArgs(string[] args)
        {

            int depth;
            string device;

            if (args.Length < 4 || args.Length > 7)
                usage();

            InputBitmapFilename = args[0];
            OutputBitmapFilename = args[1];
            device = args[2];
            depth = int.Parse(args[3]);
            this.Compress = false;
            this.ByteOrder = Endianness.LittleEndian; 
            this.Flip = RotateFlipType.RotateNoneFlipNone;

            for (int i = 4; i < args.Length; i++)
            {
                if (args[i].Equals("-c"))
                    this.Compress = true;
                else if (args[i].Equals("-b"))
                    this.ByteOrder = Endianness.BigEndian;
                else if (args[i].Equals("-fx"))
                    this.Flip = RotateFlipType.RotateNoneFlipX;
                else if (args[i].Equals("-fy"))
                    this.Flip = RotateFlipType.RotateNoneFlipY;
                else if (args[i].Equals("-fxy"))
                    this.Flip = RotateFlipType.RotateNoneFlipXY;
            }



            switch (device.ToLower())
            {

                case "hx8347a":
                    this.TargetDevice = HX8347AConverter.createInstance(depth);
                    break;

                case "hx8352a":
                    this.TargetDevice = HX8352AConverter.createInstance(depth);
                    break;

                case "ili9325":
                    this.TargetDevice = ILI9325Converter.createInstance(depth);
                    break;

                case "ili9327":
                    this.TargetDevice = ILI9327Converter.createInstance(depth);
                    break;

                case "ili9481":
                    this.TargetDevice = ILI9481Converter.createInstance(depth);
                    break;

                case "mc2pa8201":
                    this.TargetDevice = MC2PA8201Converter.createInstance(depth);
                    break;

                case "lds285":
                    this.TargetDevice = LDS285Converter.createInstance(depth);
                    break;

                case "ssd1963":
                    this.TargetDevice = SSD1963Converter.createInstance(depth);
                    break;

                case "st7783":
                    this.TargetDevice = ST7783Converter.createInstance(depth);
                    break;

                case "r61523":
                    this.TargetDevice = R61523Converter.createInstance(depth);
                    break;

                default:
                    throw new Exception(device + " is an unrecognised device");
            }
        }


        /// <summary>
        /// show usage message and exit abruptly
        /// </summary>

        private void usage()
        {

            Console.WriteLine("Usage: bm2rgbi input-image-file output-image-file target-device target-colour-depth [-c] [-b]\n");
            Console.WriteLine("-c         : compress the output in LZG format");
            Console.WriteLine("-b         : changes the byte order");
            Console.WriteLine("-f[x/y/xy] : flips the image by x / y / xy axis/axes");
            Console.WriteLine("Supported devices and colours:");
            Console.WriteLine("  ili9325 64");
            Console.WriteLine("  ili9325 262");
            Console.WriteLine("  ili9327 64");
            Console.WriteLine("  ili9327 262");
            Console.WriteLine("  ili9481 64");
            Console.WriteLine("  ili9481 262");
            Console.WriteLine("  hx8347a 64");
            Console.WriteLine("  hx8352a 64");
            Console.WriteLine("  hx8352a 262");
            Console.WriteLine("  mc2pa8201 64");
            Console.WriteLine("  mc2pa8201 262");
            Console.WriteLine("  mc2pa8201 16");
            Console.WriteLine("  lds285 64");
            Console.WriteLine("  lds285 262");
            Console.WriteLine("  lds285 16");
            Console.WriteLine("  ssd1963 16");
            Console.WriteLine("  ssd1963 262");
            Console.WriteLine("  st7783 64");
            Console.WriteLine("  st7783 262");
            Console.WriteLine("  r61523 64");
            Console.WriteLine("  r61523 262");
            Console.WriteLine("  r61523 16");

            Environment.Exit(0);
        }
    }
}
