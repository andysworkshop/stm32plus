/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System.Drawing;
using System.Drawing.Text;
using System.Xml;

namespace LzgFontConv {

  /// <summary>
  /// holder for all font definition parameters
  /// </summary>

  public class FontDefinition {

    /// <summary>
    /// properties
    /// </summary>
    
    public string Family { get; set; }
    public int Size { get; set; }
    public string Characters { get; set; }
    public Color Background { get; set; }
    public Color Foreground { get; set; }
    public TextRenderingHint Hint { get; set; }
    public FontStyle Style { get; set; }
    public Size TftResolution { get; set; }
    public int Spacing { get; set; }
    public Size ExtraSpacing { get; set; }
    public string PreviewText { get; set; }
    public Point PreviewPosition { get; set; }
    public string CompressionOptions { get; set; }


    /// <summary>
    /// write as XML
    /// </summary>

    public void WriteXml(XmlElement parent) {

      XmlUtil.AppendString(parent,"Family",this.Family);
      XmlUtil.AppendInt(parent,"Size",this.Size);
      XmlUtil.AppendString(parent,"Characters",this.Characters);
      XmlUtil.AppendColor(parent,"Background",this.Background);
      XmlUtil.AppendColor(parent,"Foreground",this.Foreground);
      XmlUtil.AppendInt(parent,"TextRenderingHint",(int)this.Hint);
      XmlUtil.AppendInt(parent,"Style",(int)this.Style);
      XmlUtil.AppendSize(parent,"TftResolution",this.TftResolution);
      XmlUtil.AppendInt(parent,"Spacing",this.Spacing);
      XmlUtil.AppendSize(parent,"ExtraSpacing",this.ExtraSpacing);
      XmlUtil.AppendString(parent,"PreviewText",this.PreviewText);
      XmlUtil.AppendPoint(parent,"PreviewPosition",this.PreviewPosition);
      XmlUtil.AppendString(parent,"CompressionOptions",this.CompressionOptions);
    }


    /// <summary>
    /// Read from XML document
    /// </summary>

    public void ReadXml(string filename) {
      
      XmlDocument doc;
      XmlElement parent;

      doc=new XmlDocument();
      doc.Load(filename);

      parent=(XmlElement)doc.SelectSingleNode("LzgFontConv");

      this.Family=XmlUtil.GetString(parent,"Family",null,true);
      this.Size=XmlUtil.GetInt(parent,"Size",10,true);
      this.Characters=XmlUtil.GetString(parent,"Characters",null,true);
      this.Background=XmlUtil.GetColor(parent,"Background");
      this.Foreground=XmlUtil.GetColor(parent,"Foreground");
      this.Hint=(TextRenderingHint)XmlUtil.GetInt(parent,"TextRenderingHint",0,true);
      this.Style=(FontStyle)XmlUtil.GetInt(parent,"Style",0,true);
      this.TftResolution=XmlUtil.GetSize(parent,"TftResolution");
      this.Spacing=XmlUtil.GetInt(parent,"Spacing",0,true);
      this.ExtraSpacing=XmlUtil.GetSize(parent,"ExtraSpacing");
      this.PreviewText=XmlUtil.GetString(parent,"PreviewText",null,true);
      this.PreviewPosition=XmlUtil.GetPoint(parent,"PreviewPosition");
      this.CompressionOptions=XmlUtil.GetString(parent,"CompressionOptions",null,true);
    }
  }
}
