using System.Drawing;
using System.Windows.Forms;


namespace FontConv
{
/*
 * character preview
 */
  
  public partial class CharPreview : UserControl
  {
  /*
   * bitmap
   */
    
    private Bitmap _bitmap;


  /*
   * constants
   */
    
    public const int PixelSize=8;


  /*
   * constructor
   */
    
    public CharPreview()
    {
      InitializeComponent();
      SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer,true);
    }


  /*
   * create from the given character
   */

    public void Create(char c_,int xoffset_,int yoffset_,int extraLines_)
    {
      bool[,] values;
      int x,y;
      Point pos;
      Point[] line;

      values=FontUtil.GetCharacterBitmap(this,this.Font,c_,xoffset_,yoffset_,extraLines_);
      _bitmap=new Bitmap(values.GetLength(0)*PixelSize,values.GetLength(1)*PixelSize);

      pos=Point.Empty;

      using(Graphics g=Graphics.FromImage(_bitmap))
      {
        g.FillRectangle(Brushes.White,0,0,_bitmap.Width,_bitmap.Height);

        for(y=0;y<values.GetLength(1);y++)
        {
          for(x=0;x<values.GetLength(0);x++)
          {
            if(values[x,y])
              g.FillRectangle(Brushes.Black,pos.X,pos.Y,PixelSize,PixelSize);

            pos.X+=PixelSize;
          }

          pos.X=0;
          pos.Y+=PixelSize;
        }

      // grid

        line=new Point[2];
        line[0].X=line[1].X=0;
        line[0].Y=0;
        line[1].Y=_bitmap.Height;

        for(x=0;x<=values.GetLength(0);x++)
        {
          g.DrawLine(Pens.LightGray,line[0],line[1]);
          line[0].X+=PixelSize;
          line[1].X+=PixelSize;
        }

        line[0].X=0;
        line[1].X=_bitmap.Width;
        line[0].Y=line[1].Y=0;

        for(y=0;y<=values.GetLength(1);y++)
        {
          g.DrawLine(Pens.LightGray,line[0],line[1]);
          line[0].Y+=PixelSize;
          line[1].Y+=PixelSize;
        }
      }

    // set the control size

      this.Size=_bitmap.Size;
    }


  /*
   * paint the background
   */

    protected override void OnPaintBackground(PaintEventArgs e)
    {
    }


  /*
   * paint the control
   */

    protected override void OnPaint(PaintEventArgs e)
    {
      e.Graphics.DrawImageUnscaled(_bitmap,0,0);
    }
  }
}
