namespace FontConv
{
  partial class FontChar
  {
    /* 
     * Required designer variable.
     */

    private System.ComponentModel.IContainer components=null;

    /* 
     * Clean up any resources being used.
     */

    protected override void Dispose(bool disposing)
    {
      if(disposing&&(components!=null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Component Designer generated code

    /* 
     * Required method for Designer support - do not modify 
     * the contents of this method with the code editor.
     */
    private void InitializeComponent()
    {
      this.SuspendLayout();
      // 
      // FontChar
      // 
      this.AutoScaleDimensions=new System.Drawing.SizeF(6F,13F);
      this.AutoScaleMode=System.Windows.Forms.AutoScaleMode.Font;
      this.Name="FontChar";
      this.MouseLeave+=new System.EventHandler(this.FontChar_MouseLeave);
      this.MouseEnter+=new System.EventHandler(this.FontChar_MouseEnter);
      this.ResumeLayout(false);

    }

    #endregion
  }
}
