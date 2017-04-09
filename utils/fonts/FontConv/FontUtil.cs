using System;
using System.Collections.Generic;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Drawing.Text;


namespace FontConv
{
/*
 * font utils
 */
  
  public class FontUtil
  {
    [DllImport("gdi32.dll")]
    public static extern uint GetFontUnicodeRanges(IntPtr hdc, IntPtr lpgs);

    [DllImport("gdi32.dll")]
    public extern static IntPtr SelectObject(IntPtr hDC, IntPtr hObject);

    public struct FontRange
    {
        public UInt16 Low;
        public UInt16 High;
    }
    
  /*
   * get unicode font ranges
   */

    static public List<FontRange> GetFontUnicodeRanges(Font font)
    {
      Graphics g = Graphics.FromHwnd(IntPtr.Zero);
      IntPtr hdc = g.GetHdc();
      IntPtr hFont = font.ToHfont();
      IntPtr old = SelectObject(hdc, hFont);
      uint size = GetFontUnicodeRanges(hdc, IntPtr.Zero);
      IntPtr glyphSet = Marshal.AllocHGlobal((int)size);
      GetFontUnicodeRanges(hdc, glyphSet);
      List<FontRange> fontRanges = new List<FontRange>();
      int count = Marshal.ReadInt32(glyphSet, 12);
      
      for (int i = 0; i < count; i++)
      {
        FontRange range = new FontRange();
        range.Low = (UInt16)Marshal.ReadInt16(glyphSet, 16 + i * 4);
        range.High = (UInt16)(range.Low + Marshal.ReadInt16(glyphSet, 18 + i * 4) - 1);
        fontRanges.Add(range);
      }

      SelectObject(hdc, old);
      Marshal.FreeHGlobal(glyphSet);
      g.ReleaseHdc(hdc);
      g.Dispose();
      return fontRanges;
    }


  /*
   * get the character bitmap
   */

    static public bool[,] GetCharacterBitmap(Control refControl_,Font f_,char c_,int xoffset_,int yoffset_,int extraLines_)
    {
      bool[,] values;
      int x,y,width;
      Bitmap bm;
      Color cr;

      using(Graphics g=refControl_.CreateGraphics())
      {
        g.TextRenderingHint=TextRenderingHint.SingleBitPerPixel;

        if(c_==' ')
          width=(int)g.MeasureString("-",f_,PointF.Empty,StringFormat.GenericTypographic).Width;
        else
          width=(int)g.MeasureString(c_.ToString(),f_,PointF.Empty,StringFormat.GenericTypographic).Width;
        
        bm=new Bitmap(width,(int)f_.Size+extraLines_,g);

        using(Graphics g2=Graphics.FromImage(bm))
        {
          g2.FillRectangle(Brushes.White,0,0,bm.Width,bm.Height);

          g2.TextRenderingHint=TextRenderingHint.SingleBitPerPixel;
          g2.DrawString(c_.ToString(),f_,Brushes.Black,xoffset_,yoffset_,StringFormat.GenericTypographic);

          values=new bool[bm.Width,bm.Height];

          for(y=0;y<bm.Height;y++)
          {
            for(x=0;x<bm.Width;x++)
            {
              cr=bm.GetPixel(x,y);
              
              values[x,y]=cr.B==0 && cr.G==0 && cr.R==0;
            }
          }
        }
      }

      return values;
    }
  }
}
