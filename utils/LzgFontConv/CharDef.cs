/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Windows.Forms;

namespace LzgFontConv {
  
  /// <summary>
  /// character definition class
  /// </summary>
  
  public class CharDef {

    /// <summary>
    /// properties
    /// </summary>
    
    public Size Size { get; private set; }
    public Bitmap CharacterBitmap;
    public byte[] CompressedBytes { get; private set; }
    public char Character { get; private set; }


    /// <summary>
    /// parse the values for the character
    /// </summary>

    public void Parse(Control refCtrl,FontDefinition fd,Font font,char c,bool doCompression) {

      SizeF size;

      this.Character=c;

      // space is a special case

      if(c==' ') {
        ParseSpace(refCtrl,font);
        return;
      }

      using(Graphics g=refCtrl.CreateGraphics()) {

        g.TextRenderingHint=fd.Hint;

        // get the size that the system thinks the character occupies

        size=TextRenderer.MeasureText(g,c.ToString(),font,new Size(int.MaxValue,int.MaxValue),TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);

        this.CharacterBitmap=new Bitmap(Convert.ToInt32(size.Width)+fd.ExtraSpacing.Width,Convert.ToInt32(font.GetHeight())+fd.ExtraSpacing.Height);
        this.Size=this.CharacterBitmap.Size;

        using(Graphics charGraphics=Graphics.FromImage(this.CharacterBitmap)) {
         
          charGraphics.TextRenderingHint=fd.Hint;
        
          using(SolidBrush backBrush=new SolidBrush(fd.Background))
            charGraphics.FillRectangle(backBrush,0,0,this.CharacterBitmap.Width,this.CharacterBitmap.Height);

          // draw the character into the center of the bitmap

          TextRenderer.DrawText(charGraphics,c.ToString(),font,new Point(0,0),fd.Foreground,fd.Background,TextFormatFlags.NoPadding | TextFormatFlags.NoPrefix);
        }

        // scan the bitmap

        if(doCompression)
          ScanBitmap(this.CharacterBitmap,fd);
      } 
    }


    /// <summary>
    /// scan the bitmap for its values
    /// </summary>

    protected void ScanBitmap(Bitmap bm,FontDefinition fd) {
     
      string tempFile,compressedName=null;
      byte[] bytes;

      // get a temporary filename

      tempFile=Path.GetTempFileName()+".png";

      try {

        // write the bitmap as a PNG to the temporary file

        bm.Save(tempFile,ImageFormat.Png);

        // compress the bitmap

        compressedName=CompressFile(fd,tempFile);

        // read back the bytes

        bytes=File.ReadAllBytes(compressedName);
        
        // encode the bytes with a length prefix in LSB order

        this.CompressedBytes=new byte[bytes.Length+2];
        
        this.CompressedBytes[0]=(byte)(bytes.Length & 0xff);
        this.CompressedBytes[1]=(byte)(bytes.Length >> 8);

        bytes.CopyTo(this.CompressedBytes,2);
      }
      finally {
        File.Delete(tempFile);
        
        if(compressedName!=null)
          File.Delete(compressedName);
      }
    }


    /// <summary>
    /// run bm2rgbi on the temp file
    /// </summary>

    private string CompressFile(FontDefinition fd,string tempFile) {
      
      ProcessStartInfo pi;
      Process p;
      FileInfo fi;
      string compressedName,args;

      // run bm2rgbi on the file

      compressedName=tempFile+".lzg";

      args="\""+tempFile+"\" \""+compressedName+"\" "+fd.CompressionOptions+" -c";
      pi=new ProcessStartInfo("bm2rgbi.exe",args);
      pi.CreateNoWindow=true;
      pi.UseShellExecute=false;
      pi.WindowStyle=ProcessWindowStyle.Hidden;

      p=Process.Start(pi);
      p.WaitForExit();

      // make sure that it did something
      
      fi=new FileInfo(compressedName);
      if(fi.Length==0)
        throw new Exception("Failed to compress the font character. Verify that bm2rgbi.exe and lzg.exe are both present in the same directory as LzgFontConv.exe");

      return compressedName;
    }


    /// <summary>
    /// Copy a bitmap section
    /// </summary>

    private Bitmap CopyBitmap(Bitmap srcBitmap,Rectangle section) {
        
      // Create the new bitmap and associated graphics object
     
      Bitmap bmp=new Bitmap(section.Width, section.Height);
    
      using(Graphics g=Graphics.FromImage(bmp))
        g.DrawImage(srcBitmap,0,0,section,GraphicsUnit.Pixel);
      
      return bmp;
    }


    /// <summary>
    /// parse the space character
    /// </summary>

    private void ParseSpace(Control refCtrl,Font font) {

      StringFormat sf;
      Size size;

      // measure the size of a space

      sf=StringFormat.GenericTypographic;
      sf.FormatFlags|=StringFormatFlags.MeasureTrailingSpaces;

      using(Graphics g=refCtrl.CreateGraphics())
        size=g.MeasureString(" ",font,new PointF(0,0),sf).ToSize();

      // a space has no height

      this.Size=new Size(size.Width,0);
      this.CompressedBytes=new byte[0];
    }
  }
}
