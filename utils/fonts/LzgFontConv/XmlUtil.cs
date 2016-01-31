/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System;
using System.Drawing;
using System.Globalization;
using System.Xml;

namespace LzgFontConv {

  /// <summary>
  /// XML utils
  /// </summary>
	
  public class XmlUtil
	{

    /// <summary>
    /// write a color
    /// </summary>

    public static void AppendColor(XmlElement parent,string childName,Color cr) {
      AppendString(parent,
                   childName,
                   cr.R.ToString("X2")+cr.G.ToString("X2")+cr.B.ToString("X2"));
    }


    /// <summary>
    /// write a Size
    /// </summary>

    public static void AppendSize(XmlElement parent,string childName,Size size) {
      
      XmlElement child;

      child=parent.OwnerDocument.CreateElement(childName);
      
      child.SetAttribute("width",size.Width.ToString());
      child.SetAttribute("height",size.Height.ToString());

      parent.AppendChild(child);
    }


    /// <summary>
    /// write a Point
    /// </summary>

    public static void AppendPoint(XmlElement parent,string childName,Point p) {
      
      XmlElement child;

      child=parent.OwnerDocument.CreateElement(childName);
      
      child.SetAttribute("x",p.X.ToString());
      child.SetAttribute("y",p.Y.ToString());

      parent.AppendChild(child);
    }


	/*
	 * append XML child
	 */

		public static void AppendString(XmlElement parent,string childName,string value) {
      XmlElement child;
      XmlText		 text;

      if(value==null)
        return;

      child=parent.OwnerDocument.CreateElement(childName);
      text=parent.OwnerDocument.CreateTextNode(value);

      parent.AppendChild(child);
      child.AppendChild(text);
    }


	/*
	 * append boolean
	 */

		public static void AppendBool(XmlElement parent,string childName,bool value) {
			AppendString(parent,childName,value ? "true" : "false");
		}


	/*
	 * append XML child CDATA
	 */

		public static void AppendCdata(XmlElement parent,string childName,string value) {
      XmlElement			child;
      XmlCDataSection	text;

      child=parent.OwnerDocument.CreateElement(childName);
      text=parent.OwnerDocument.CreateCDataSection(value);

      parent.AppendChild(child);
      child.AppendChild(text);
    }


	/*
	 * append int
	 */

		public static void AppendInt(XmlElement parent,string childName,int value) {
			AppendString(parent,childName,value.ToString());
		}


	/*
	 * get string
	 */

		public static string GetString(XmlNode parent,string xpath,string defaultValue,bool required) {
			string str;

			try
			{
				str=parent.SelectSingleNode(xpath).FirstChild.InnerText;
				return str;
			}
			catch(Exception)
			{
				if(required)
					throw new Exception("XML value: "+defaultValue+" is required but is not present in this document");

				return defaultValue;
			}
		}


	/*
	 * get an integer
	 */

		public static Int32 GetInt(XmlElement parent,string xpath,Int32 defaultValue,bool required) {
			string str;

			if((str=GetString(parent,xpath,null,required))==null)
				return defaultValue;

			return Int32.Parse(str);
		}


	/*
	 * get a boolean
	 */

		public static bool GetBool(XmlElement parent,string xpath,bool defaultValue,bool required) {
			string str;

			if((str=GetString(parent,xpath,null,required))==null)
				return defaultValue;

			return str.Equals("true");
		}


    /// <summary>
    /// Get a color
    /// </summary>

    public static Color GetColor(XmlElement parent,string xpath) {

      string str;

      str=GetString(parent,xpath,null,true);
      
      return Color.FromArgb(
          0xff,
          byte.Parse(str.Substring(0,2),NumberStyles.HexNumber),
          byte.Parse(str.Substring(2,2),NumberStyles.HexNumber),
          byte.Parse(str.Substring(4,2),NumberStyles.HexNumber)
        );
    }


    /// <summary>
    /// Get a size
    /// </summary>

    public static Size GetSize(XmlElement parent,string xpath) {

      XmlNode node;

      if((node=parent.SelectSingleNode(xpath))==null)
        throw new Exception(xpath+" not found in document");

      return new Size(
        int.Parse(node.Attributes["width"].Value),
        int.Parse(node.Attributes["height"].Value)
      );
    }


    /// <summary>
    /// Get a point
    /// </summary>

    public static Point GetPoint(XmlElement parent,string xpath) {

      XmlNode node;

      if((node=parent.SelectSingleNode(xpath))==null)
        throw new Exception(xpath+" not found in document");

      return new Point(
        int.Parse(node.Attributes["x"].Value),
        int.Parse(node.Attributes["y"].Value)
      );
    }
	}
}
