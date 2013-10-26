namespace FontConv
{
  partial class FormMain
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

    #region Windows Form Designer generated code

    /*
     * Required method for Designer support - do not modify
     * the contents of this method with the code editor.
     */
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
      this._charsPanel = new System.Windows.Forms.FlowLayoutPanel();
      this._previewPanel = new System.Windows.Forms.FlowLayoutPanel();
      this.label3 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.label1 = new System.Windows.Forms.Label();
      this._charSpace = new System.Windows.Forms.NumericUpDown();
      this._extraLines = new System.Windows.Forms.NumericUpDown();
      this._xoffset = new System.Windows.Forms.NumericUpDown();
      this._yoffset = new System.Windows.Forms.NumericUpDown();
      this.BottomToolStripPanel = new System.Windows.Forms.ToolStripPanel();
      this.TopToolStripPanel = new System.Windows.Forms.ToolStripPanel();
      this.RightToolStripPanel = new System.Windows.Forms.ToolStripPanel();
      this.LeftToolStripPanel = new System.Windows.Forms.ToolStripPanel();
      this.ContentPanel = new System.Windows.Forms.ToolStripContentPanel();
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this._fontSize = new System.Windows.Forms.NumericUpDown();
      this._btnBrowseFontFile = new System.Windows.Forms.Button();
      this._editFontFile = new System.Windows.Forms.TextBox();
      this.label5 = new System.Windows.Forms.Label();
      this.label4 = new System.Windows.Forms.Label();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this._btnSelect7Bit = new System.Windows.Forms.Button();
      this._btnSelectAlpha = new System.Windows.Forms.Button();
      this.groupBox3 = new System.Windows.Forms.GroupBox();
      this._preview = new FontConv.PreviewText();
      this._btnSave = new System.Windows.Forms.Button();
      this._btnLoad = new System.Windows.Forms.Button();
      this._fontDialog = new System.Windows.Forms.FontDialog();
      this._openFileDialog = new System.Windows.Forms.OpenFileDialog();
      this._saveFileDialog = new System.Windows.Forms.SaveFileDialog();
      this._btnStm32plus = new System.Windows.Forms.RadioButton();
      this._btnArduino = new System.Windows.Forms.RadioButton();
      this.groupBox4 = new System.Windows.Forms.GroupBox();
      this._openFontFileDialog = new System.Windows.Forms.OpenFileDialog();
      this._logo = new System.Windows.Forms.PictureBox();
      this._tooltip = new System.Windows.Forms.ToolTip(this.components);
      ((System.ComponentModel.ISupportInitialize)(this._charSpace)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this._extraLines)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this._xoffset)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this._yoffset)).BeginInit();
      this.groupBox1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this._fontSize)).BeginInit();
      this.groupBox2.SuspendLayout();
      this.groupBox3.SuspendLayout();
      this.groupBox4.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this._logo)).BeginInit();
      this.SuspendLayout();
      // 
      // _charsPanel
      // 
      this._charsPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this._charsPanel.AutoScroll = true;
      this._charsPanel.Location = new System.Drawing.Point(10, 20);
      this._charsPanel.Name = "_charsPanel";
      this._charsPanel.Size = new System.Drawing.Size(901, 162);
      this._charsPanel.TabIndex = 0;
      // 
      // _previewPanel
      // 
      this._previewPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this._previewPanel.AutoScroll = true;
      this._previewPanel.Location = new System.Drawing.Point(10, 19);
      this._previewPanel.Name = "_previewPanel";
      this._previewPanel.Size = new System.Drawing.Size(901, 401);
      this._previewPanel.TabIndex = 0;
      // 
      // label3
      // 
      this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(268, 55);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(96, 13);
      this.label3.TabIndex = 8;
      this.label3.Text = "Character spacing:";
      // 
      // label2
      // 
      this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(139, 55);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(58, 13);
      this.label2.TabIndex = 6;
      this.label2.Text = "Extra lines:";
      // 
      // label1
      // 
      this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(422, 55);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(60, 13);
      this.label1.TabIndex = 10;
      this.label1.Text = "Offset (x,y):";
      // 
      // _charSpace
      // 
      this._charSpace.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._charSpace.Enabled = false;
      this._charSpace.Location = new System.Drawing.Point(370, 53);
      this._charSpace.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
      this._charSpace.Name = "_charSpace";
      this._charSpace.Size = new System.Drawing.Size(46, 20);
      this._charSpace.TabIndex = 9;
      this._charSpace.ValueChanged += new System.EventHandler(this._charSpace_ValueChanged);
      // 
      // _extraLines
      // 
      this._extraLines.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._extraLines.Enabled = false;
      this._extraLines.Location = new System.Drawing.Point(203, 53);
      this._extraLines.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
      this._extraLines.Name = "_extraLines";
      this._extraLines.Size = new System.Drawing.Size(59, 20);
      this._extraLines.TabIndex = 7;
      this._extraLines.ValueChanged += new System.EventHandler(this._extraLines_ValueChanged);
      // 
      // _xoffset
      // 
      this._xoffset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._xoffset.Enabled = false;
      this._xoffset.Location = new System.Drawing.Point(486, 53);
      this._xoffset.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
      this._xoffset.Name = "_xoffset";
      this._xoffset.Size = new System.Drawing.Size(45, 20);
      this._xoffset.TabIndex = 11;
      this._xoffset.ValueChanged += new System.EventHandler(this._xoffset_ValueChanged);
      // 
      // _yoffset
      // 
      this._yoffset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._yoffset.Enabled = false;
      this._yoffset.Location = new System.Drawing.Point(537, 53);
      this._yoffset.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
      this._yoffset.Name = "_yoffset";
      this._yoffset.Size = new System.Drawing.Size(45, 20);
      this._yoffset.TabIndex = 12;
      this._yoffset.ValueChanged += new System.EventHandler(this._yoffset_ValueChanged);
      // 
      // BottomToolStripPanel
      // 
      this.BottomToolStripPanel.Location = new System.Drawing.Point(0, 0);
      this.BottomToolStripPanel.Name = "BottomToolStripPanel";
      this.BottomToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
      this.BottomToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
      this.BottomToolStripPanel.Size = new System.Drawing.Size(0, 0);
      // 
      // TopToolStripPanel
      // 
      this.TopToolStripPanel.Location = new System.Drawing.Point(0, 0);
      this.TopToolStripPanel.Name = "TopToolStripPanel";
      this.TopToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
      this.TopToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
      this.TopToolStripPanel.Size = new System.Drawing.Size(0, 0);
      // 
      // RightToolStripPanel
      // 
      this.RightToolStripPanel.Location = new System.Drawing.Point(0, 0);
      this.RightToolStripPanel.Name = "RightToolStripPanel";
      this.RightToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
      this.RightToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
      this.RightToolStripPanel.Size = new System.Drawing.Size(0, 0);
      // 
      // LeftToolStripPanel
      // 
      this.LeftToolStripPanel.Location = new System.Drawing.Point(0, 0);
      this.LeftToolStripPanel.Name = "LeftToolStripPanel";
      this.LeftToolStripPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
      this.LeftToolStripPanel.RowMargin = new System.Windows.Forms.Padding(3, 0, 0, 0);
      this.LeftToolStripPanel.Size = new System.Drawing.Size(0, 0);
      // 
      // ContentPanel
      // 
      this.ContentPanel.AutoScroll = true;
      this.ContentPanel.Size = new System.Drawing.Size(773, 544);
      // 
      // groupBox1
      // 
      this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.groupBox1.Controls.Add(this._fontSize);
      this.groupBox1.Controls.Add(this._btnBrowseFontFile);
      this.groupBox1.Controls.Add(this.label1);
      this.groupBox1.Controls.Add(this.label3);
      this.groupBox1.Controls.Add(this._yoffset);
      this.groupBox1.Controls.Add(this._editFontFile);
      this.groupBox1.Controls.Add(this._xoffset);
      this.groupBox1.Controls.Add(this._charSpace);
      this.groupBox1.Controls.Add(this.label2);
      this.groupBox1.Controls.Add(this.label5);
      this.groupBox1.Controls.Add(this.label4);
      this.groupBox1.Controls.Add(this._extraLines);
      this.groupBox1.Location = new System.Drawing.Point(12, 12);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(595, 87);
      this.groupBox1.TabIndex = 0;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Choose a font and size";
      // 
      // _fontSize
      // 
      this._fontSize.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this._fontSize.Location = new System.Drawing.Point(64, 53);
      this._fontSize.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
      this._fontSize.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
      this._fontSize.Name = "_fontSize";
      this._fontSize.Size = new System.Drawing.Size(69, 20);
      this._fontSize.TabIndex = 5;
      this._fontSize.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
      this._fontSize.ValueChanged += new System.EventHandler(this._fontSize_ValueChanged);
      // 
      // _btnBrowseFontFile
      // 
      this._btnBrowseFontFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this._btnBrowseFontFile.Location = new System.Drawing.Point(447, 21);
      this._btnBrowseFontFile.Name = "_btnBrowseFontFile";
      this._btnBrowseFontFile.Size = new System.Drawing.Size(135, 23);
      this._btnBrowseFontFile.TabIndex = 3;
      this._btnBrowseFontFile.Text = "Browse font file...";
      this._btnBrowseFontFile.UseVisualStyleBackColor = true;
      this._btnBrowseFontFile.Click += new System.EventHandler(this._btnBrowseFontFile_Click);
      // 
      // _editFontFile
      // 
      this._editFontFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this._editFontFile.Location = new System.Drawing.Point(64, 24);
      this._editFontFile.Name = "_editFontFile";
      this._editFontFile.ReadOnly = true;
      this._editFontFile.Size = new System.Drawing.Size(377, 20);
      this._editFontFile.TabIndex = 1;
      // 
      // label5
      // 
      this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(6, 55);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(52, 13);
      this.label5.TabIndex = 4;
      this.label5.Text = "Font size:";
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(6, 27);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(47, 13);
      this.label4.TabIndex = 0;
      this.label4.Text = "Font file:";
      // 
      // groupBox2
      // 
      this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.groupBox2.Controls.Add(this._charsPanel);
      this.groupBox2.Controls.Add(this._btnSelect7Bit);
      this.groupBox2.Controls.Add(this._btnSelectAlpha);
      this.groupBox2.Location = new System.Drawing.Point(12, 105);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(923, 217);
      this.groupBox2.TabIndex = 1;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Character selection";
      // 
      // _btnSelect7Bit
      // 
      this._btnSelect7Bit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
      this._btnSelect7Bit.Enabled = false;
      this._btnSelect7Bit.Location = new System.Drawing.Point(655, 188);
      this._btnSelect7Bit.Name = "_btnSelect7Bit";
      this._btnSelect7Bit.Size = new System.Drawing.Size(104, 23);
      this._btnSelect7Bit.TabIndex = 1;
      this._btnSelect7Bit.Text = "Select all 7-bit";
      this._btnSelect7Bit.UseVisualStyleBackColor = true;
      this._btnSelect7Bit.Click += new System.EventHandler(this._btnSelect7Bit_Click);
      // 
      // _btnSelectAlpha
      // 
      this._btnSelectAlpha.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
      this._btnSelectAlpha.Enabled = false;
      this._btnSelectAlpha.Location = new System.Drawing.Point(765, 188);
      this._btnSelectAlpha.Name = "_btnSelectAlpha";
      this._btnSelectAlpha.Size = new System.Drawing.Size(146, 23);
      this._btnSelectAlpha.TabIndex = 2;
      this._btnSelectAlpha.Text = "Select all alphanumeric";
      this._btnSelectAlpha.UseVisualStyleBackColor = true;
      this._btnSelectAlpha.Click += new System.EventHandler(this._btnSelectAlpha_Click);
      // 
      // groupBox3
      // 
      this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.groupBox3.Controls.Add(this._previewPanel);
      this.groupBox3.Controls.Add(this._preview);
      this.groupBox3.Location = new System.Drawing.Point(12, 328);
      this.groupBox3.Name = "groupBox3";
      this.groupBox3.Size = new System.Drawing.Size(923, 456);
      this.groupBox3.TabIndex = 2;
      this.groupBox3.TabStop = false;
      this.groupBox3.Text = "Preview";
      // 
      // _preview
      // 
      this._preview.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this._preview.Location = new System.Drawing.Point(7, 426);
      this._preview.Name = "_preview";
      this._preview.Size = new System.Drawing.Size(904, 24);
      this._preview.TabIndex = 1;
      this._preview.Text = "The quick brown fox jumped over the lazy dogs.";
      this._preview.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // _btnSave
      // 
      this._btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this._btnSave.Enabled = false;
      this._btnSave.Location = new System.Drawing.Point(954, 45);
      this._btnSave.Name = "_btnSave";
      this._btnSave.Size = new System.Drawing.Size(97, 23);
      this._btnSave.TabIndex = 4;
      this._btnSave.Text = "Save...";
      this._btnSave.UseVisualStyleBackColor = true;
      this._btnSave.Click += new System.EventHandler(this._btnSave_Click);
      // 
      // _btnLoad
      // 
      this._btnLoad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this._btnLoad.Location = new System.Drawing.Point(954, 16);
      this._btnLoad.Name = "_btnLoad";
      this._btnLoad.Size = new System.Drawing.Size(97, 23);
      this._btnLoad.TabIndex = 3;
      this._btnLoad.Text = "Load...";
      this._btnLoad.UseVisualStyleBackColor = true;
      this._btnLoad.Click += new System.EventHandler(this._btnLoad_Click);
      // 
      // _openFileDialog
      // 
      this._openFileDialog.Filter = "XML Files(*.xml)|*.xml|All Files|*.*||";
      // 
      // _saveFileDialog
      // 
      this._saveFileDialog.Filter = "XML Files(*.xml)|*.xml|All Files|*.*||";
      // 
      // _btnStm32plus
      // 
      this._btnStm32plus.AutoSize = true;
      this._btnStm32plus.Location = new System.Drawing.Point(10, 19);
      this._btnStm32plus.Name = "_btnStm32plus";
      this._btnStm32plus.Size = new System.Drawing.Size(72, 17);
      this._btnStm32plus.TabIndex = 0;
      this._btnStm32plus.TabStop = true;
      this._btnStm32plus.Text = "stm32plus";
      this._btnStm32plus.UseVisualStyleBackColor = true;
      // 
      // _btnArduino
      // 
      this._btnArduino.AutoSize = true;
      this._btnArduino.Location = new System.Drawing.Point(10, 40);
      this._btnArduino.Name = "_btnArduino";
      this._btnArduino.Size = new System.Drawing.Size(61, 17);
      this._btnArduino.TabIndex = 1;
      this._btnArduino.TabStop = true;
      this._btnArduino.Text = "Arduino";
      this._btnArduino.UseVisualStyleBackColor = true;
      // 
      // groupBox4
      // 
      this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this.groupBox4.Controls.Add(this._btnStm32plus);
      this.groupBox4.Controls.Add(this._btnArduino);
      this.groupBox4.Location = new System.Drawing.Point(954, 74);
      this.groupBox4.Name = "groupBox4";
      this.groupBox4.Size = new System.Drawing.Size(97, 68);
      this.groupBox4.TabIndex = 5;
      this.groupBox4.TabStop = false;
      this.groupBox4.Text = "Target";
      // 
      // _openFontFileDialog
      // 
      this._openFontFileDialog.Filter = "Font Files(*.ttf)|*.ttf|All Files(*.*)|*.*||";
      // 
      // _logo
      // 
      this._logo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
      this._logo.Cursor = System.Windows.Forms.Cursors.Hand;
      this._logo.Image = ((System.Drawing.Image)(resources.GetObject("_logo.Image")));
      this._logo.Location = new System.Drawing.Point(632, 21);
      this._logo.Name = "_logo";
      this._logo.Size = new System.Drawing.Size(280, 78);
      this._logo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
      this._logo.TabIndex = 6;
      this._logo.TabStop = false;
      this._logo.Click += new System.EventHandler(this._logo_Click);
      // 
      // FormMain
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.BackColor = System.Drawing.SystemColors.Control;
      this.ClientSize = new System.Drawing.Size(1068, 795);
      this.Controls.Add(this._logo);
      this.Controls.Add(this.groupBox4);
      this.Controls.Add(this._btnLoad);
      this.Controls.Add(this._btnSave);
      this.Controls.Add(this.groupBox3);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.groupBox1);
      this.MinimumSize = new System.Drawing.Size(1084, 833);
      this.Name = "FormMain";
      this.Text = "Font Converter";
      ((System.ComponentModel.ISupportInitialize)(this._charSpace)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this._extraLines)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this._xoffset)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this._yoffset)).EndInit();
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this._fontSize)).EndInit();
      this.groupBox2.ResumeLayout(false);
      this.groupBox3.ResumeLayout(false);
      this.groupBox4.ResumeLayout(false);
      this.groupBox4.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this._logo)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.FlowLayoutPanel _charsPanel;
    private System.Windows.Forms.FlowLayoutPanel _previewPanel;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.NumericUpDown _xoffset;
    private System.Windows.Forms.NumericUpDown _yoffset;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.NumericUpDown _extraLines;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.NumericUpDown _charSpace;
    private System.Windows.Forms.ToolStripPanel BottomToolStripPanel;
    private System.Windows.Forms.ToolStripPanel TopToolStripPanel;
    private System.Windows.Forms.ToolStripPanel RightToolStripPanel;
    private System.Windows.Forms.ToolStripPanel LeftToolStripPanel;
    private System.Windows.Forms.ToolStripContentPanel ContentPanel;
    private PreviewText _preview;
    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.TextBox _editFontFile;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.Button _btnBrowseFontFile;
    private System.Windows.Forms.NumericUpDown _fontSize;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.GroupBox groupBox3;
    private System.Windows.Forms.Button _btnSave;
    private System.Windows.Forms.Button _btnLoad;
    private System.Windows.Forms.FontDialog _fontDialog;
    private System.Windows.Forms.OpenFileDialog _openFileDialog;
    private System.Windows.Forms.SaveFileDialog _saveFileDialog;
    private System.Windows.Forms.RadioButton _btnStm32plus;
    private System.Windows.Forms.RadioButton _btnArduino;
    private System.Windows.Forms.GroupBox groupBox4;
    private System.Windows.Forms.OpenFileDialog _openFontFileDialog;
    private System.Windows.Forms.Button _btnSelectAlpha;
    private System.Windows.Forms.Button _btnSelect7Bit;
    private System.Windows.Forms.PictureBox _logo;
    private System.Windows.Forms.ToolTip _tooltip;
  }
}

