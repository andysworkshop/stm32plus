using System;
using System.Collections.Generic;
using System.Xml;
using System.IO;
using System.Windows.Forms;

namespace FontConv {
  
  abstract public class FontReader {

  /*
   * load from XML
   */

    static public SizedFont Load(String filename) {

      XmlDocument doc;
      XmlNodeList charsNodes;
      char c;
      String fontfile;
      int fontsize;
      SizedFont sf;

      // read the XML

      doc=new XmlDocument();
      doc.Load(filename);

      // get file and size to create the SizedFont

      fontfile=XmlUtil.GetString(doc.DocumentElement,"Filename",null,true);
      fontsize=XmlUtil.GetInt(doc.DocumentElement,"Size",0,true);
      
      // give the user the chance to find missing fonts

      if(!File.Exists(fontfile)) {

        OpenFileDialog ofn;
        
        if(MessageBox.Show(fontfile+" does not exist. Click OK to browse for the font file or Cancel to abort","Font not found",MessageBoxButtons.OKCancel,MessageBoxIcon.Stop)==DialogResult.Cancel)
          throw new Exception("Font not found");

        ofn=new OpenFileDialog();

        ofn.FileName=fontfile;
        ofn.Filter="Font Files (*.ttf)|*.ttf|All Files (*.*)|*.*||";

        if(ofn.ShowDialog()==DialogResult.Cancel)
          throw new Exception("Font not found");

        fontfile=ofn.FileName;
      }

      sf=new SizedFont(fontfile,fontsize);

      // fill in the adjustments

      sf.XOffset=XmlUtil.GetInt(doc.DocumentElement,"XOffset",0,true);
      sf.YOffset=XmlUtil.GetInt(doc.DocumentElement,"YOffset",0,true);
      sf.ExtraLines=XmlUtil.GetInt(doc.DocumentElement,"ExtraLines",0,true);
      sf.CharSpace=XmlUtil.GetInt(doc.DocumentElement,"CharSpace",0,true);

      if((charsNodes=doc.DocumentElement.SelectNodes("Chars/Char"))!=null) {
        
        foreach(XmlNode charNode in charsNodes) {
          c=Convert.ToChar(UInt16.Parse(charNode.InnerText));
          sf.Add(c);
        }
      }

      return sf;
    }
  }
}
