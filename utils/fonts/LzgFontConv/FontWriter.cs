/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using System.Xml;

namespace LzgFontConv {

  /// <summary>
  /// base class for font writers
  /// </summary>

  public abstract class FontWriter {

    /// <summary>
    /// members
    /// </summary>
    
    protected FontFilenames _filenames=new FontFilenames(); 
    protected CharacterDefinitions _charDefs=new CharacterDefinitions();


    /// <summary>
    /// properties
    /// </summary>
    
    public FontDefinition FontDef { get; set; }


    /// <summary>
    /// derived class methods
    /// </summary>
    
    protected abstract void WriteHeaderStart(TextWriter writer);
    protected abstract void WriteHeaderEnd(TextWriter writer);
    protected abstract void WriteSourceStart(TextWriter writer);
    protected abstract void WriteSourceEnd(TextWriter writer);
    protected abstract void WriteSourceBody(TextWriter writer);


    /// <summary>
    /// write the font data
    /// </summary>
    
    public void Write(Control refCtrl) {

      SortedSet<char> charset;

      // get the filenames

      if(_filenames.GetFilenames()==DialogResult.Cancel)
        return;

      Cursor.Current=Cursors.WaitCursor;
      
      try {

        // remove duplicate characters and sort while we're at it

        charset=new SortedSet<char>();

        foreach(char c in this.FontDef.Characters) {
          if(!charset.Contains(c)) {
            charset.Add(c);
          }
        }

        this.FontDef.Characters=string.Empty;
        foreach(char c in charset)
          this.FontDef.Characters+=c;

        // create the char definitions

        _charDefs.ParseCharacters(refCtrl,this.FontDef,this.FontDef.Characters,true);

        // write the 3 files

        WriteDefinition();
        WriteHeader();
        WriteSource();
      }
      finally {
        Cursor.Current=Cursors.Default;
      }
    }


    /// <summary>
    /// write the font XML
    /// </summary>
    
    protected void WriteDefinition() {

      XmlDocument doc;
      XmlElement root;

      // create root element

      doc=new XmlDocument();
      root=doc.CreateElement("LzgFontConv");
      doc.AppendChild(root);

      // write out the font data

      this.FontDef.WriteXml(root);

      // save the document

      doc.Save(_filenames.Definition);
    }


    /// <summary>
    /// write the header file 
    /// </summary>

    protected void WriteHeader() {

      using(StreamWriter headerWriter=new StreamWriter(_filenames.Header)) {

        WriteHeaderStart(headerWriter);
        WriteHeaderBody(headerWriter);
        WriteHeaderEnd(headerWriter);
      }
    }


    /// <summary>
    /// write the body of the header file
    /// </summary>

    void WriteHeaderBody(TextWriter writer) {
      
      writer.Write("  // helper so the user can just do 'new fontname' without having to know the parameters\n\n");

      writer.Write("  extern const struct FontChar FDEF_"+GetFontName()+"_CHAR[];\n\n");

      writer.Write("  class Font_"+GetFontNameAndSize()+" : public LzgFont {\n");
      writer.Write("    public:\n");
      writer.Write("      Font_"+GetFontNameAndSize()+"()\n");
      writer.Write("        : LzgFont("+GetFirstCharacter()+","+this.FontDef.Characters.Length+","+GetFontHeight()+","+this.FontDef.Spacing+",FDEF_"+GetFontName()+"_CHAR"+") {\n");
      writer.Write("      }\n");
      writer.Write("  };\n");
    }


    /// <summary>
    /// get just the font name
    /// </summary>

    protected string GetFontName() {
      
      string name;

      // name with space replaced by underscore

      name=this.FontDef.Family.Replace(" ","_").ToUpper();

      // include bold/italic

      if(this.FontDef.Style!=FontStyle.Regular) {
        
        name+="_";
        
        if((this.FontDef.Style & FontStyle.Bold)!=0)
          name+="B";

        if((this.FontDef.Style & FontStyle.Italic)!=0)
          name+="I";
      }

      return name;
    }


    /// <summary>
    /// Get the font pixel height
    /// </summary>

    protected int GetFontHeight() {
      
      return _charDefs.Height;
    }


    /// <summary>
    /// get the name and size
    /// </summary>
    
    protected string GetFontNameAndSize() {
      return GetFontName()+"_"+this.FontDef.Size;
    }


    /// <summary>
    /// get the first character code
    /// </summary>

    protected int GetFirstCharacter() {
      return this.FontDef.Characters[0];
    }


    /// <summary>
    /// get the name of a character
    /// </summary>

    protected string GetCharacterName(char c) {
      return "FDEF_"+GetFontNameAndSize()+"_"+Convert.ToUInt16(c);
    }


    /// <summary>
    /// write the source file
    /// </summary>
    
    protected void WriteSource() {

      using(StreamWriter sourceWriter=new StreamWriter(_filenames.Source)) {

        WriteSourceStart(sourceWriter);
        WriteSourceBody(sourceWriter);
        WriteSourceEnd(sourceWriter);
      }
    }
  }
}
