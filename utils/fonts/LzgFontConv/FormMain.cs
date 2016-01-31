/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System;
using System.Drawing;
using System.Drawing.Text;
using System.Globalization;
using System.Windows.Forms;


namespace LzgFontConv {
  

  /// <summary>
  /// main form class
  /// </summary>
  
  public partial class FormMain : Form {
  
    /// <summary>
    /// font definition class
    /// </summary>
    
    private FontDefinition _fd=new FontDefinition();

  
    /// <summary>
    /// constructor
    /// </summary>
    
    public FormMain() {
      InitializeComponent();
    }


    /// <summary>
    /// form loaded
    /// </summary>

    private void FormMain_Load(object sender, EventArgs e) {
      EnumFonts();
      this._preview.FontDef=_fd;
    }


    /// <summary>
    /// Enumerate fonts and add to dropdown
    /// </summary>
    
    private void EnumFonts() {

      foreach(System.Windows.Media.FontFamily fontFamily in System.Windows.Media.Fonts.SystemFontFamilies)
        _cmbFont.Items.Add(fontFamily.Source);

      _cmbFont.SelectedIndex=0;
      _cmbRenderingHint.SelectedIndex=5;
      _cmbStyles.SelectedIndex=0;
    }


    /// <summary>
    /// Insert character ranges
    /// </summary>

    private void _btnNumbers_Click(object sender, EventArgs e) {
     InsertChars(48,57); 
    }

    private void _btnUpperCase_Click(object sender, EventArgs e) {
      InsertChars(65,90);
    }

    private void _btnLowerCase_Click(object sender, EventArgs e) {
      InsertChars(97,122);
    }

    private void _btnSymbols_Click(object sender, EventArgs e) {
      InsertChars(32,47);
      InsertChars(58,64);
      InsertChars(91,96);
      InsertChars(123,126);
    }

    private void _btnAscii_Click(object sender, EventArgs e) {
      InsertChars(32,126);
    }


    /// <summary>
    /// Insert a range of characters
    /// </summary>

    private void InsertChars(int first,int last) {
      
      String str;
      int i;

      str=_editSelectedCharacters.Text.Substring(0,_editSelectedCharacters.SelectionStart);

      for(i=first;i<=last;i++)
        str+=Convert.ToChar(i);

      str+=_editSelectedCharacters.Text.Substring(_editSelectedCharacters.SelectionStart);

      _editSelectedCharacters.Text=str;
    }


    /// <summary>
    /// Choose background
    /// </summary>

    private void _btnChooseBackground_Click(object sender, EventArgs e) {
      
      _colorDialog.Color=_backgroundPreview.Value;
      if(_colorDialog.ShowDialog()==DialogResult.Cancel)
        return;

      _editBackground.Text=ColourToString(_colorDialog.Color);
    }


    /// <summary>
    /// Choose foreground
    /// </summary>

    private void _btnChooseForeground_Click(object sender, EventArgs e) {

      _colorDialog.Color=_foregroundPreview.Value;
      if(_colorDialog.ShowDialog()==DialogResult.Cancel)
        return;

      _editForeground.Text=ColourToString(_colorDialog.Color);
    }


    /// <summary>
    /// Convert a colour to a RRGGBB string
    /// </summary>

    private String ColourToString(Color cr) {
      return cr.R.ToString("X2")+cr.G.ToString("X2")+cr.B.ToString("X2");
    }


    /// <summary>
    /// Something happened that means the preview is to be updated
    /// </summary>

    private void UpdatePreviewEvent(object sender, EventArgs e) {

      // update colour preview

      try {

        UpdateFont();
        UpdateFontSize();
        UpdateSelectedCharacters();
        UpdateCharacterSpacing();
        UpdateExtraSize();
        UpdateBackground();
        UpdateForeground();
        UpdateCompressionOptions();

        _fd.PreviewPosition=UpdatePosition();
        _fd.TftResolution=new Size(UpdateTftWidth(),UpdateTftHeight());

        switch(_cmbRenderingHint.SelectedIndex) {
          case 0:
            _preview.RenderingHint=TextRenderingHint.SystemDefault;
            break;
          case 1:
            _preview.RenderingHint=TextRenderingHint.SingleBitPerPixelGridFit;
            break;
          case 2:
            _preview.RenderingHint=TextRenderingHint.SingleBitPerPixel;
            break;
          case 3:
            _preview.RenderingHint=TextRenderingHint.AntiAliasGridFit;
            break;
          case 4:
            _preview.RenderingHint=TextRenderingHint.AntiAlias;
            break;
          case 5:
            _preview.RenderingHint=TextRenderingHint.ClearTypeGridFit;
            break;
        }
        _fd.Hint=_preview.RenderingHint;

        _preview.Position=_fd.PreviewPosition;
        _preview.Font=CreateFont();
        _preview.Size=_fd.TftResolution;
        _preview.BackgroundColour=_backgroundPreview.Value;
        _preview.ForegroundColour=_foregroundPreview.Value;
        _preview.Text=_editPreviewText.Text;
        _preview.Invalidate();

        _fd.PreviewText=_preview.Text;
      }
      catch {
      }
    }


    /// <summary>
    /// validate the preview text position
    /// </summary>

    private Point UpdatePosition() {
      Point p;
      
      p=new Point(
        ValidateIntRange(_editPreviewX.Text,_editPreviewX,0,9999),
        ValidateIntRange(_editPreviewY.Text,_editPreviewY,0,9999)
      );

      return p;
    }


    /// <summary>
    /// Create the font for the preview
    /// </summary>

    private Font CreateFont() {
      
      FontStyle style;

      switch(_cmbStyles.SelectedIndex) {
        
        case 1:
          style=FontStyle.Bold;
          break;

        case 2:
          style=FontStyle.Italic;
          break;

        case 3:
          style=FontStyle.Bold | FontStyle.Italic;
          break;

        default:
          style=FontStyle.Regular;
          break;
      }
      _fd.Style=style;

      return new Font(_cmbFont.Text,float.Parse(_cmbSize.Text),style,GraphicsUnit.Pixel);
    }


    /// <summary>
    /// update TFT width
    /// </summary>
    
    private int UpdateTftWidth() {
      return ValidateIntRange(_editTftWidth.Text,_editTftWidth,1,9999);
    }


    /// <summary>
    /// update TFT height
    /// </summary>
    
    private int UpdateTftHeight() {
      return ValidateIntRange(_editTftHeight.Text,_editTftHeight,1,9999);
    }


    /// <summary>
    /// Update the character spacing
    /// </summary>

    private void UpdateCharacterSpacing() {
      _fd.Spacing=ValidateIntRange(_editCharacterSpacing.Text,_editCharacterSpacing,0,9999);
    }


    /// <summary>
    /// update the extra size
    /// </summary>
    
    private void UpdateExtraSize() {
      _fd.ExtraSpacing=new Size(ValidateIntRange(_editExtraW.Text,_editExtraW,0,9999),
                                ValidateIntRange(_editExtraH.Text,_editExtraH,0,9999));
    }


    /// <summary>
    /// validate the font
    /// </summary>
    
    private void UpdateFont() {

      if(_cmbFont.Items.Contains(_cmbFont.Text))
        _cmbFont.BackColor=SystemColors.Window;
      else {
        _cmbFont.BackColor=Color.PaleVioletRed;
        throw new Exception("Invalid font name");
      }

      _fd.Family=_cmbFont.Text;
    }


    /// <summary>
    /// validate font size
    /// </summary>
    
    private void UpdateFontSize() {
      _fd.Size=ValidateIntRange(_cmbSize.Text,_cmbSize,1,9999);
    }


    /// <summary>
    /// validate that an integer is not zero
    /// </summary>

    private int ValidateIntRange(String str,Control c,int low,int high) {
      
      int size;

      try {
        size=int.Parse(str);
        
        if(size<low || size>high)
          throw new Exception("Invalid size");

        c.BackColor=SystemColors.Window;
        return size;
      }
      catch(Exception ex) {
        c.BackColor=Color.PaleVioletRed;
        throw ex;
      }
    }


    /// <summary>
    /// Update the selected characters
    /// </summary>
    
    private void UpdateSelectedCharacters() {

      if(_editSelectedCharacters.TextLength==0) {
        _editSelectedCharacters.BackColor=Color.PaleVioletRed;
        throw new Exception("No characters selected");
      }

      _editSelectedCharacters.BackColor=SystemColors.Window;
      _fd.Characters=_editSelectedCharacters.Text;
    }


    /// <summary>
    /// Update the compression options
    /// </summary>
    
    private void UpdateCompressionOptions() {

      if(_editCompressionOptions.Text.Length==0) {
        _editCompressionOptions.BackColor=Color.PaleVioletRed;
        throw new Exception("No compression options entered");
      }

      _editCompressionOptions.BackColor=SystemColors.Window;
      _fd.CompressionOptions=_editCompressionOptions.Text;
    }


    /// <summary>
    /// validate background colour
    /// </summary>
    
    private void UpdateBackground() {
      
      Color cr;
      String str;

      try {
      
        str=_editBackground.Text;

        cr=Color.FromArgb(
          0xff,
          byte.Parse(str.Substring(0,2),NumberStyles.HexNumber),
          byte.Parse(str.Substring(2,2),NumberStyles.HexNumber),
          byte.Parse(str.Substring(4,2),NumberStyles.HexNumber)
        );
        
        _editBackground.BackColor=SystemColors.Window;
        _backgroundPreview.Value=cr;
        _backgroundPreview.Invalidate();
        _fd.Background=cr;
      }
      catch(Exception ex) {
        _editBackground.BackColor=Color.PaleVioletRed;
        throw ex;
      }
    }


    /// <summary>
    /// validate foreground colour
    /// </summary>
    
    private void UpdateForeground() {
      
      Color cr;
      String str;

      try {
      
        str=_editForeground.Text;

        cr=Color.FromArgb(
          0xff,
          byte.Parse(str.Substring(0,2),NumberStyles.HexNumber),
          byte.Parse(str.Substring(2,2),NumberStyles.HexNumber),
          byte.Parse(str.Substring(4,2),NumberStyles.HexNumber)
        );
        
        _editForeground.BackColor=SystemColors.Window;
        _foregroundPreview.Value=cr;
        _foregroundPreview.Invalidate();
        _fd.Foreground=cr;
      }
      catch(Exception ex) {
        _editForeground.BackColor=Color.PaleVioletRed;
        throw ex;
      }
    }


    /// <summary>
    /// save the selected font for the Arduino
    /// </summary>

    private void _btnSaveArduino_Click(object sender, EventArgs e) {
      
      ArduinoFontWriter fw;

      try {

        UpdatePreviewEvent(null,null);

        fw=new ArduinoFontWriter();
        fw.FontDef=_fd;
        fw.Write(this);
      }
      catch(Exception ex) {
        MessageBox.Show(ex.Message,"Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
      }
    }


    /// <summary>
    /// save the selected font for stm32plus
    /// </summary>

    private void _btnSaveStm32plus_Click(object sender, EventArgs e) {
      
      Stm32plusFontWriter fw;

      try {

        UpdatePreviewEvent(null,null);

        fw=new Stm32plusFontWriter();
        fw.FontDef=_fd;
        fw.Write(this);
      }
      catch(Exception ex) {
        MessageBox.Show(ex.Message,"Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
      }
    }


    /// <summary>
    /// estimate the memory requirement
    /// </summary>

    private void _btnMemory_Click(object sender, EventArgs e) {

      MemoryEstimator me;
      int size;

      try {
        
        UpdatePreviewEvent(null,null);

        me=new MemoryEstimator();
        me.FontDef=_fd;

        size=me.Estimate(this);
        MessageBox.Show("Approximately "+size+" bytes of flash memory are required.","Memory Requirement",MessageBoxButtons.OK,MessageBoxIcon.Information);
      }
      catch(Exception ex) {
        MessageBox.Show(ex.Message,"Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
      }
    }


    /// <summary>
    /// load from disk
    /// </summary>

    private void _btnLoad_Click(object sender, EventArgs e) {

      OpenFileDialog ofd;
      FontDefinition fd;

      try {
        
        // choose file

        ofd=new OpenFileDialog();
        ofd.Filter="Saved Fonts (*.xml)|*.xml|All Files (*.*)|*.*||";

        if(ofd.ShowDialog()==DialogResult.Cancel)
          return;

        // read document to the font definition

        fd=new FontDefinition();
        fd.ReadXml(ofd.FileName);

        // set up the controls

        _cmbFont.Text=fd.Family;
        _cmbSize.Text=fd.Size.ToString();
        _editSelectedCharacters.Text=fd.Characters;
        _editBackground.Text=ColourToString(fd.Background);
        _editForeground.Text=ColourToString(fd.Foreground);
        
        switch(fd.Hint) {
          case TextRenderingHint.SystemDefault:
            _cmbRenderingHint.SelectedIndex=0;
            break;
          case TextRenderingHint.SingleBitPerPixelGridFit:
            _cmbRenderingHint.SelectedIndex=1;
            break;
          case TextRenderingHint.SingleBitPerPixel:
            _cmbRenderingHint.SelectedIndex=2;
            break;
          case TextRenderingHint.AntiAliasGridFit:
            _cmbRenderingHint.SelectedIndex=3;
            break;
          case TextRenderingHint.AntiAlias:
            _cmbRenderingHint.SelectedIndex=4;
            break;
          default:
            _cmbRenderingHint.SelectedIndex=5;
            break;
        }

        switch(fd.Style) {
          case FontStyle.Bold:
            _cmbStyles.SelectedIndex=1;
            break;
          case FontStyle.Italic:
            _cmbStyles.SelectedIndex=2;
            break;
          case FontStyle.Bold | FontStyle.Italic:
            _cmbStyles.SelectedIndex=3;
            break;
          default:
            _cmbStyles.SelectedIndex=0;
            break;
        }

        _editTftWidth.Value=fd.TftResolution.Width;
        _editTftHeight.Value=fd.TftResolution.Height;
        _editCharacterSpacing.Value=fd.Spacing;
        _editPreviewText.Text=fd.PreviewText;
        _editPreviewX.Value=fd.PreviewPosition.X;
        _editPreviewY.Value=fd.PreviewPosition.Y;
        _editCompressionOptions.Text=fd.CompressionOptions;
        _editExtraW.Value=fd.ExtraSpacing.Width;
        _editExtraH.Value=fd.ExtraSpacing.Height;

        // new font def

        _fd=fd;
        _preview.FontDef=_fd;

        // update

        UpdatePreviewEvent(null,null);
      }
      catch(Exception ex) {
        MessageBox.Show(ex.Message,"Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
      }
    }


    /// <summary>
    /// click on logo
    /// </summary>

    private void _logo_Click(object sender, EventArgs e) {
      System.Diagnostics.Process.Start("http://www.andybrown.me.uk");
    }
  }
}
