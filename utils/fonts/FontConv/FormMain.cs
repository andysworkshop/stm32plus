using System;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;


namespace FontConv
{
/*
 * main form
 */
  
  public partial class FormMain : Form {
  
  /*
   * members
   */
    
    private SizedFont _sizedFont;


  /*
   * constructor
   */
    
    public FormMain() {
      InitializeComponent();
    }


    private void RefillCharsPanel() {
      
      FontChar fc;

      try {
        _charsPanel.SuspendLayout();

      // empty panel

        _charsPanel.Controls.Clear();

      // add new. not particularly fast.

        using(Graphics g=CreateGraphics()) {
          foreach(FontUtil.FontRange fr in FontUtil.GetFontUnicodeRanges(_sizedFont.GdiFont)) {
            for(UInt16 code=fr.Low;code<=fr.High;code++) {
              
              char c;
              int width;

              fc=new FontChar(_tooltip);
              c=Convert.ToChar(code);

              // special case for space which we map to the width of a "-"

              width=(int)g.MeasureString(c==' ' ? "-" : c.ToString(),_sizedFont.GdiFont,PointF.Empty,StringFormat.GenericTypographic).Width;

              fc.Text=c.ToString();
              fc.Font=_sizedFont.GdiFont;
              fc.Size=new Size(width,_sizedFont.Size);
              fc.Selected=_sizedFont.ContainsChar(c);
              fc.Click+=OnClickFontChar;
              fc.Tag=c;

              _charsPanel.Controls.Add(fc);
            }
          }
        }
        _preview.Font=_sizedFont.GdiFont;
        RefillPreviews();
      }
      finally {
        _charsPanel.ResumeLayout();
      }
    }


  /*
   * refill previews panel
   */
    
    private void RefillPreviews() {
    
      // refill the list

      _previewPanel.Controls.Clear();

      foreach(CharPreview cp in _sizedFont.CreatePreviews())
        _previewPanel.Controls.Add(cp);
    }


  /*
   * font char clicked
   */

    private void OnClickFontChar(object sender,EventArgs args)
    {
      FontChar fc;

      try
      {
      // get values

        fc=(FontChar)sender;

      // toggle select

        _sizedFont.ToggleSelected(fc.Text[0]);

      // select control

        fc.Selected^=true;
        fc.Invalidate();

      // add/remove from the previews

        if(fc.Selected)
          AddPreview(fc.Text[0]);
        else
          RemovePreview(fc.Text[0]);
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


  /*
   * add a preview
   */

    private void AddPreview(char c) {
      _previewPanel.Controls.Add(_sizedFont.CreatePreview(c));
    }


  /*
   * remove preview
   */

    private void RemovePreview(char c) {
      
      foreach(CharPreview cp in _previewPanel.Controls) {
        if((char)cp.Tag==c) {
          _previewPanel.Controls.Remove(cp);
          return;
        }
      }
    }


  /*
   * offset changed
   */

    private void _xoffset_ValueChanged(object sender,EventArgs e) {
      
      try {
        _sizedFont.XOffset=(int)_xoffset.Value;
        RefillPreviews();
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


    /*
     * Y offset changed
     */

    private void _yoffset_ValueChanged(object sender,EventArgs e) {

      try {
        _sizedFont.YOffset=(int)_yoffset.Value;
        RefillPreviews();
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


  /*
   * extra lines changed
   */

    private void _extraLines_ValueChanged(object sender,EventArgs e) {
      
      try {
        _sizedFont.ExtraLines=(int)_extraLines.Value;
        RefillPreviews();
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


  /*
   * Character space changed
   */

    private void _charSpace_ValueChanged(object sender,EventArgs e)
    {
      try {
        _sizedFont.CharSpace=(int)_charSpace.Value;
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


    /*
     * Browse for font file
     */

    private void _btnBrowseFontFile_Click(object sender,EventArgs e) {
      
      try {
        // browse for font

        if(_openFontFileDialog.ShowDialog(this)==DialogResult.Cancel)
          return;

        // set the filename

        _editFontFile.Text=_openFontFileDialog.FileName;
      
        // create the font
      
        _sizedFont=new SizedFont(_openFontFileDialog.FileName,Convert.ToInt32(_fontSize.Value));
        
        _preview.Font=_sizedFont.GdiFont;
        _preview.Invalidate();

        EnableControls();
        RefillCharsPanel();
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


    /*
     * Enable/disable controls
     */

    private void EnableControls() {

      bool enable;

      enable=_sizedFont!=null;

      _extraLines.Enabled=enable;
      _charSpace.Enabled=enable;
      _xoffset.Enabled=enable;
      _yoffset.Enabled=enable;
      _btnSave.Enabled=enable;
      _btnSelectAlpha.Enabled=enable;
      _btnSelect7Bit.Enabled=enable;
    }


    /*
     * Check if char is valid
     */

    private bool IsValidChar(char c) {
      
      foreach(FontChar fc in _charsPanel.Controls)
        if((char)fc.Tag==c)
          return true;

      return false;
    }


    /*
     * Select all alphanumeric
     */

    private void _btnSelectAlpha_Click(object sender,EventArgs e)
    {
      String alphanum="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890";

      try {
        Cursor.Current=Cursors.WaitCursor;

        foreach(char c in alphanum)
          if(IsValidChar(c))
            _sizedFont.Add(c);

        RefillCharsPanel();
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
      finally {
        Cursor.Current=Cursors.Default;
      }
    }


    /*
     * Select 7 bit chars
     */

    private void _btnSelect7Bit_Click(object sender,EventArgs e) {
      
      char c;

      try {
        Cursor.Current=Cursors.WaitCursor;

        for(c=(char)32;c<=(char)127;c++)
          if(IsValidChar(c))
            _sizedFont.Add(c);

        RefillCharsPanel();
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
      finally {
        Cursor.Current=Cursors.Default;
      }
    }


    /*
     * New font size
     */

    private void _fontSize_ValueChanged(object sender,EventArgs e) {
      
      try {
        _sizedFont.Size=Convert.ToInt32(_fontSize.Value);
        _sizedFont.CreateFont();

        RefillCharsPanel();
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


    /*
     * Save out
     */

    private void _btnSave_Click(object sender,EventArgs e) {

      TargetDevice td;

      try {

        // target device

        if(_btnArduino.Checked)
          td=TargetDevice.ARDUINO;
        else if(_btnStm32plus.Checked)
          td=TargetDevice.STM32PLUS;
        else
          throw new Exception("Please select a target device");

        // get filename

        if(_saveFileDialog.ShowDialog()==DialogResult.Cancel)
          return;

        // save operation

        FontWriter.Save(_sizedFont,_saveFileDialog.FileName,this,td);
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


    /*
     * Load settings
     */

    private void _btnLoad_Click(object sender,EventArgs e)
    {
      try {
      
        // get filename

        if(_openFileDialog.ShowDialog()==DialogResult.Cancel)
          return;

        // load operation

        _sizedFont=FontReader.Load(_openFileDialog.FileName);

        _editFontFile.Text=_sizedFont.Filename;
        _fontSize.Value=_sizedFont.Size;
        _extraLines.Value=_sizedFont.ExtraLines;
        _xoffset.Value=_sizedFont.XOffset;
        _yoffset.Value=_sizedFont.YOffset;
        _charSpace.Value=_sizedFont.CharSpace;

        EnableControls();
        RefillCharsPanel();
      }
      catch(Exception ex) {
        Util.Error(ex);
      }
    }


    /*
     * Logo clicked
     */

    private void _logo_Click(object sender,EventArgs e)
    {
      Process.Start("http://www.andybrown.me.uk");
    }
  }
}
