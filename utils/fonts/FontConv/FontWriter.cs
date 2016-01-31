using System;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Windows.Forms;
using System.Xml;


namespace FontConv {
  
  /*
   * Helper for writing fonts
   */

  abstract public class FontWriter {

    /*
     * Members
     */

    protected SizedFont _font;
    protected StreamWriter _headerWriter;
    protected StreamWriter _sourceWriter;
    protected XmlElement _parent;
    protected Control _refControl;


    /*
     * For the derivations
     */

    abstract protected void WriteSourceHeader();
    abstract protected void WriteHeader();
    abstract protected void WriteTrailer();
    abstract protected void WriteSourceTrailer();
    abstract protected void WriteFontBytes();
    abstract protected void WriteFontChars();


    /*
     * Constructor
     */

    protected FontWriter(SizedFont sf,StreamWriter headerWriter,StreamWriter sourceWriter,XmlElement parent,Control refControl) {
      _font=sf;
      _headerWriter=headerWriter;
      _sourceWriter=sourceWriter;
      _parent=parent;
      _refControl=refControl;
    }


    /*
     * Save font
     */

    static public void Save(SizedFont sf,String xmlFilename,Control refControl,TargetDevice targetDevice) {
      
      String headerFilename,sourceFilename;
      XmlDocument doc;
      XmlElement root;
      FontWriter fw;

      // filename has .xml extension. calculate same name with .h and .cpp

      headerFilename=Path.GetFileNameWithoutExtension(xmlFilename)+".h";
      headerFilename=Path.Combine(Path.GetDirectoryName(xmlFilename),headerFilename);

      sourceFilename=Path.GetFileNameWithoutExtension(xmlFilename)+".cpp";
      sourceFilename=Path.Combine(Path.GetDirectoryName(xmlFilename),sourceFilename);

      // create header stream

      using(StreamWriter headerWriter=new StreamWriter(headerFilename)) {
        using(StreamWriter sourceWriter=new StreamWriter(sourceFilename)) {

          // create XML parent

          doc=new XmlDocument();
          root=doc.CreateElement("FontConv");
          doc.AppendChild(root);

          switch(targetDevice) {

            case TargetDevice.ARDUINO:
              fw=new ArduinoFontWriter(sf,headerWriter,sourceWriter,root,refControl);
              break;

            case TargetDevice.STM32PLUS:
              fw=new Stm32plusFontWriter(sf,headerWriter,sourceWriter,root,refControl);
              break;

            default:
              throw new Exception("Unknown device");
          }

          fw.Save();
          doc.Save(xmlFilename);
        }
      }
    }


    /*
     * Save header and XML
     */

    public void Save() {
    
      SaveXml();

      // header 

      WriteHeader();
      WriteFont();
      WriteTrailer();

      // source

      WriteSourceHeader();
      WriteFontBytes();
      WriteFontChars();
      WriteSourceTrailer();
    }


    /*
     * Write XML
     */

    private void SaveXml() {
      
      XmlElement chars;

      XmlUtil.AppendString(_parent,"Filename",_font.Filename);
      XmlUtil.AppendInt(_parent,"Size",_font.Size);
      XmlUtil.AppendInt(_parent,"XOffset",_font.XOffset);
      XmlUtil.AppendInt(_parent,"YOffset",_font.YOffset);
      XmlUtil.AppendInt(_parent,"ExtraLines",_font.ExtraLines);
      XmlUtil.AppendInt(_parent,"CharSpace",_font.CharSpace);

      chars=_parent.OwnerDocument.CreateElement("Chars");
        
      foreach(char c in _font.Characters())
        XmlUtil.AppendInt(chars,"Char",Convert.ToUInt16(c));
        
      _parent.AppendChild(chars);
    }


    /*
     * get bytes name
     */

    protected string GetBytesName(char c) {
      return _font.Identifier+Convert.ToUInt16(c).ToString()+"_BYTES";
    }


  /*
   * get char name
   */

    protected string GetCharName() {
      return _font.Identifier+"_CHAR";
    }


  /*
   * get the width of this character
   */

    protected int GetCharWidth(char c) {
      
      using(Graphics g=_refControl.CreateGraphics()) {
          
        g.TextRenderingHint=TextRenderingHint.SingleBitPerPixel;
        return (int)g.MeasureString(c==' ' ? "-" : c.ToString(),_font.GdiFont,0,StringFormat.GenericTypographic).Width;
      }
    }


    /*
     * Write the font declaration
     */

    protected void WriteFont() {

      int firstChar,charCount,height,spacing;

      firstChar=_font.GetFirstCharacter();
      charCount=_font.Characters().Count;
      height=_font.Size+_font.ExtraLines;
      spacing=_font.CharSpace;

      _headerWriter.Write("  // helper so the user can just do 'new fontname' without having to know the parameters\n\n");

      _headerWriter.Write("  extern const struct FontChar "+GetCharName()+"[];\n\n");

      _headerWriter.Write("  class Font_"+_font.Name+_font.Size+" : public Font {\n");
      _headerWriter.Write("    public:\n");
      _headerWriter.Write("      Font_"+_font.Name+_font.Size+"()\n");
      _headerWriter.Write("        : Font("+firstChar+","+charCount+","+height+","+spacing+","+GetCharName()+") {\n");
      _headerWriter.Write("      }\n");
      _headerWriter.Write("  };\n");
    }
  }
}
