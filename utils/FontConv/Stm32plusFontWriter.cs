using System;
using System.IO;
using System.Windows.Forms;
using System.Xml;


namespace FontConv {
  
  /*
   * Class for writing out fonts suitable for the stm32
   */

  public class Stm32plusFontWriter : FontWriter {

    /*
     * Constructor
     */

    public Stm32plusFontWriter(SizedFont sf,StreamWriter headerWriter,StreamWriter sourceWriter,XmlElement parent,Control refControl)
      : base(sf,headerWriter,sourceWriter,parent,refControl) {
    }

  /*
   * write trailer
   */

    override protected void WriteTrailer() {
      _headerWriter.Write("} }\n");
    }

  /*
   * write source trailer
   */

    override protected void WriteSourceTrailer() {
      _sourceWriter.Write("} }\n");
    }

  /*
   * write source header
   */

    override protected void WriteSourceHeader() {
      _sourceWriter.Write("#include \"config/stm32plus.h\"\n");
      _sourceWriter.Write("#include \"config/display/font.h\"\n\n");
      _sourceWriter.Write("namespace stm32plus { namespace display {\n\n");
    }

  /*
   * write header
   */

    override protected void WriteHeader() {
      
      _headerWriter.Write("#pragma once\n\n");
      _headerWriter.Write("#include \"display/graphic/Font.h\"\n\n");
      _headerWriter.Write("namespace stm32plus { namespace display {\n\n");
    }


  /*
   * write font byte declarations
   */

    override protected void WriteFontBytes() {
      
      string str;
      bool[,] values;
      byte b,bitpos;
      int x,y;

      _sourceWriter.Write("  // byte definitions for "+_font.Identifier+"\n\n");

      foreach(char c in _font.Characters()) {

        str=GetBytesName(c);

        _sourceWriter.Write("  uint8_t "+str+"[]={ ");

        values=FontUtil.GetCharacterBitmap(_refControl,_font.GdiFont,c,_font.XOffset,_font.YOffset,_font.ExtraLines);
          
        b=0;
        bitpos=0;

        for(y=0;y<values.GetLength(1);y++)
        {
          for(x=0;x<values.GetLength(0);x++)
          {
            if(values[x,y])
              b|=(byte)(1 << bitpos);

            if(bitpos++==7)
            {
              _sourceWriter.Write(b.ToString()+",");
              bitpos=0;
              b=0;
            }
          }
        }

        if(bitpos>0)
          _sourceWriter.Write(b.ToString()+",");

        _sourceWriter.Write("};\n");
      }
      _sourceWriter.Write("\n");
    }


  /*
   * write font char declarations
   */

    override protected void WriteFontChars() {
      
      _sourceWriter.Write("  // character definitions for "+_font.Identifier+"\n\n");
      _sourceWriter.Write("  extern const struct FontChar "+GetCharName()+"[]={\n");

      foreach(char c in _font.Characters()) {
        _sourceWriter.Write("    { ");

        _sourceWriter.Write(Convert.ToUInt16(c).ToString()+",");
        _sourceWriter.Write(GetCharWidth(c)+",");
        _sourceWriter.Write(GetBytesName(c)+" },\n");
      }

      _sourceWriter.Write("  };\n\n");
    }
  }
}
