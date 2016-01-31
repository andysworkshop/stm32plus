/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System.Drawing;
using System.Windows.Forms;


namespace LzgFontConv {
  
  /// <summary>
  /// Colour display panel
  /// </summary>
  
  class ColourPanel : Panel {

    /// <summary>
    /// properties
    /// </summary>
    
    public Color Value { get; set; }

    /// <summary>
    /// Constructor
    /// </summary>
    
    public ColourPanel() {
      this.Value=Color.Black;
    }

    /// <summary>
    /// paint the background
    /// </summary>

    protected override void OnPaintBackground(PaintEventArgs e) {
      
      using(SolidBrush brush=new SolidBrush(this.Value))
        e.Graphics.FillRectangle(brush,this.ClientRectangle);
    }
  }
}
