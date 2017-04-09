/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System.Drawing;
using System.Drawing.Text;
using System.Windows.Forms;

namespace LzgFontConv {
  
  /// <summary>
  /// Preview panel class
  /// </summary>
  
  class PreviewPanel : Panel {

    /// <summary>
    /// properties
    /// </summary>
    
    public Color BackgroundColour { get; set; }
    public Color ForegroundColour { get; set; }
    public TextRenderingHint RenderingHint { get; set; }
    public Point Position { get; set; }
    public FontDefinition FontDef { get; set; }


    /// <summary>
    /// paint it
    /// </summary>
    
    protected override void OnPaint(PaintEventArgs e) {

      CharacterDefinitions defs;
      Point pos;

      e.Graphics.TextRenderingHint=this.RenderingHint;

      try {
      
        // create bitmaps for this string

        defs=new CharacterDefinitions();
        defs.ParseCharacters(this,this.FontDef,this.Text,false);
        
        // blit them into the display

        pos=this.Position;

        foreach(CharDef cd in defs.Definitions) {
          
          if(cd.Character!=' ')
            e.Graphics.DrawImage(cd.CharacterBitmap,pos);
          
          pos.X+=cd.Size.Width+this.FontDef.Spacing;
        }
      }
      catch {
        using(SolidBrush brush=new SolidBrush(Color.PaleVioletRed))
          e.Graphics.FillRectangle(brush,ClientRectangle);
      }
    }


    /// <summary>
    /// paint the background
    /// </summary>

    protected override void OnPaintBackground(PaintEventArgs e) {
      using(SolidBrush brush=new SolidBrush(this.BackgroundColour))
        e.Graphics.FillRectangle(brush,this.ClientRectangle);
    }
  }
}
