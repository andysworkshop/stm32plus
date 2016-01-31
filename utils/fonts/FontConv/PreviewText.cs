using System.Drawing;
using System.Drawing.Text;
using System.Windows.Forms;

namespace FontConv {
  
  public partial class PreviewText : Label {
    
    public PreviewText() {
      InitializeComponent();
    }

    protected override void OnPaint(PaintEventArgs e) {
      e.Graphics.TextRenderingHint=TextRenderingHint.SingleBitPerPixel;
      e.Graphics.DrawString(this.Text,this.Font,SystemBrushes.WindowText,Point.Empty);
    }
  }
}
