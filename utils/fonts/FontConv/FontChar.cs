using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System;


namespace FontConv
{
/*
 * Font character
 */
  
  public partial class FontChar : UserControl
  {
  /*
   * members
   */
    
    private bool _hover=false;
    private ToolTip _tooltip;
   

  /*
   * properties
   */
    
    public bool Selected { get; set; }


  /*
   * constructor
   */
    
    public FontChar(ToolTip tooltip)
    {
      InitializeComponent();

      _tooltip=tooltip;
      SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer,true);
    }


  /*
   * background paint
   */

    protected override void OnPaintBackground(PaintEventArgs e)
    {
      Brush b;

      if(!this.Selected && _hover)
        b=Brushes.Tan;
      else
        b=this.Selected ? SystemBrushes.Highlight : SystemBrushes.Control;

      e.Graphics.FillRectangle(b,e.ClipRectangle);
    }


  /*
   * paint the character
   */

    protected override void OnPaint(PaintEventArgs e)
    {
      e.Graphics.TextRenderingHint=TextRenderingHint.SingleBitPerPixel;

      e.Graphics.DrawString(
        this.Text,
        this.Font,
        this.Selected ? SystemBrushes.HighlightText : SystemBrushes.WindowText,
        0,0,
        StringFormat.GenericTypographic);
    }


  /*
   * mouse enter
   */

    private void FontChar_MouseEnter(object sender,System.EventArgs e)
    {
      _hover=true;
      _tooltip.SetToolTip(this,Convert.ToInt32(this.Text[0]).ToString());

      Invalidate();
    }


  /*
   * mouse leave
   */

    private void FontChar_MouseLeave(object sender,System.EventArgs e)
    {
      _hover=false;
      Invalidate();
    }
  }
}
