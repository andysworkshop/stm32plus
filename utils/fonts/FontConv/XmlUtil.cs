using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;


namespace FontConv
{
	class XmlUtil
	{
	/*
	 * append XML child
	 */

		public static void AppendString(XmlElement parent_,string childName_,string value_)
    {
      XmlElement child;
      XmlText		 text;

      if(value_==null)
        return;

      child=parent_.OwnerDocument.CreateElement(childName_);
      text=parent_.OwnerDocument.CreateTextNode(value_);

      parent_.AppendChild(child);
      child.AppendChild(text);
    }


	/*
	 * append boolean
	 */

		public static void AppendBool(XmlElement parent_,string childName_,bool value_)
		{
			AppendString(parent_,childName_,value_ ? "true" : "false");
		}


	/*
	 * append XML child CDATA
	 */

		public static void AppendCdata(XmlElement parent_,string childName_,string value_)
    {
      XmlElement			child;
      XmlCDataSection	text;

      child=parent_.OwnerDocument.CreateElement(childName_);
      text=parent_.OwnerDocument.CreateCDataSection(value_);

      parent_.AppendChild(child);
      child.AppendChild(text);
    }


	/*
	 * append int
	 */

		public static void AppendInt(XmlElement parent_,string childName_,int value_)
		{
			AppendString(parent_,childName_,value_.ToString());
		}


	/*
	 * get string
	 */

		public static string GetString(XmlNode parent_,string xpath_,string default_,bool required_)
		{
			string str;

			try
			{
				str=parent_.SelectSingleNode(xpath_).FirstChild.InnerText;
				return str;
			}
			catch(Exception)
			{
				if(required_)
					throw new Exception("XML value: "+default_+" is required but is not present in this document");

				return default_;
			}
		}


	/*
	 * get an integer
	 */

		public static Int32 GetInt(XmlElement parent_,string xpath_,Int32 default_,bool required_)
		{
			string str;

			if((str=GetString(parent_,xpath_,null,required_))==null)
				return default_;

			return Int32.Parse(str);
		}


	/*
	 * get a boolean
	 */

		public static bool GetBool(XmlElement parent_,string xpath_,bool default_,bool required_)
		{
			string str;

			if((str=GetString(parent_,xpath_,null,required_))==null)
				return default_;

			return str.Equals("true");
		}
	}
}
