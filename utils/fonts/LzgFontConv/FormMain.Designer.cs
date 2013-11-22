namespace LzgFontConv
{
  partial class FormMain
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this._cmbSize = new System.Windows.Forms.ComboBox();
      this._cmbFont = new System.Windows.Forms.ComboBox();
      this.label1 = new System.Windows.Forms.Label();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this._btnAscii = new System.Windows.Forms.Button();
      this._btnSymbols = new System.Windows.Forms.Button();
      this._btnLowerCase = new System.Windows.Forms.Button();
      this._btnUpperCase = new System.Windows.Forms.Button();
      this._btnNumbers = new System.Windows.Forms.Button();
      this.label3 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this._editSelectedCharacters = new System.Windows.Forms.TextBox();
      this.groupBox3 = new System.Windows.Forms.GroupBox();
      this._editPreviewY = new System.Windows.Forms.NumericUpDown();
      this._editPreviewX = new System.Windows.Forms.NumericUpDown();
      this._preview = new LzgFontConv.PreviewPanel();
      this._editPreviewText = new System.Windows.Forms.TextBox();
      this.label7 = new System.Windows.Forms.Label();
      this.label8 = new System.Windows.Forms.Label();
      this.label9 = new System.Windows.Forms.Label();
      this.groupBox4 = new System.Windows.Forms.GroupBox();
      this._btnChooseForeground = new System.Windows.Forms.Button();
      this._editTftHeight = new System.Windows.Forms.NumericUpDown();
      this._editExtraH = new System.Windows.Forms.NumericUpDown();
      this._editExtraW = new System.Windows.Forms.NumericUpDown();
      this._editCharacterSpacing = new System.Windows.Forms.NumericUpDown();
      this._editTftWidth = new System.Windows.Forms.NumericUpDown();
      this._cmbStyles = new System.Windows.Forms.ComboBox();
      this._cmbRenderingHint = new System.Windows.Forms.ComboBox();
      this._btnChooseBackground = new System.Windows.Forms.Button();
      this._editForeground = new System.Windows.Forms.TextBox();
      this._editBackground = new System.Windows.Forms.TextBox();
      this._foregroundPreview = new LzgFontConv.ColourPanel();
      this._backgroundPreview = new LzgFontConv.ColourPanel();
      this.label17 = new System.Windows.Forms.Label();
      this.label6 = new System.Windows.Forms.Label();
      this.label12 = new System.Windows.Forms.Label();
      this.label11 = new System.Windows.Forms.Label();
      this.label10 = new System.Windows.Forms.Label();
      this.label14 = new System.Windows.Forms.Label();
      this.label16 = new System.Windows.Forms.Label();
      this.label13 = new System.Windows.Forms.Label();
      this.label5 = new System.Windows.Forms.Label();
      this.label4 = new System.Windows.Forms.Label();
      this._colorDialog = new System.Windows.Forms.ColorDialog();
      this.groupBox5 = new System.Windows.Forms.GroupBox();
      this._logo = new System.Windows.Forms.PictureBox();
      this._btnLoad = new System.Windows.Forms.Button();
      this._btnSaveStm32plus = new System.Windows.Forms.Button();
      this._btnSaveArduino = new System.Windows.Forms.Button();
      this._btnMemory = new System.Windows.Forms.Button();
      this._editCompressionOptions = new System.Windows.Forms.ComboBox();
      this.label15 = new System.Windows.Forms.Label();
      this._tooltip = new System.Windows.Forms.ToolTip(this.components);
      this.groupBox1.SuspendLayout();
      this.groupBox2.SuspendLayout();
      this.groupBox3.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this._editPreviewY)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this._editPreviewX)).BeginInit();
      this.groupBox4.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this._editTftHeight)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this._editExtraH)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this._editExtraW)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this._editCharacterSpacing)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this._editTftWidth)).BeginInit();
      this.groupBox5.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this._logo)).BeginInit();
      this.SuspendLayout();
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this._cmbSize);
      this.groupBox1.Controls.Add(this._cmbFont);
      this.groupBox1.Controls.Add(this.label1);
      this.groupBox1.Location = new System.Drawing.Point(12, 12);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(533, 51);
      this.groupBox1.TabIndex = 0;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Font";
      // 
      // _cmbSize
      // 
      this._cmbSize.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
      this._cmbSize.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
      this._cmbSize.FormattingEnabled = true;
      this._cmbSize.Items.AddRange(new object[] {
            "6",
            "7",
            "8",
            "9",
            "10",
            "11",
            "12",
            "14",
            "16",
            "18",
            "20",
            "22",
            "24",
            "28",
            "30",
            "32",
            "36",
            "48",
            "72"});
      this._cmbSize.Location = new System.Drawing.Point(447, 19);
      this._cmbSize.MaxDropDownItems = 50;
      this._cmbSize.Name = "_cmbSize";
      this._cmbSize.Size = new System.Drawing.Size(73, 21);
      this._cmbSize.TabIndex = 3;
      this._cmbSize.Text = "12";
      this._cmbSize.SelectedIndexChanged += new System.EventHandler(this.UpdatePreviewEvent);
      this._cmbSize.TextUpdate += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _cmbFont
      // 
      this._cmbFont.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
      this._cmbFont.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
      this._cmbFont.FormattingEnabled = true;
      this._cmbFont.Location = new System.Drawing.Point(123, 19);
      this._cmbFont.MaxDropDownItems = 40;
      this._cmbFont.Name = "_cmbFont";
      this._cmbFont.Size = new System.Drawing.Size(318, 21);
      this._cmbFont.Sorted = true;
      this._cmbFont.TabIndex = 1;
      this._cmbFont.SelectedIndexChanged += new System.EventHandler(this.UpdatePreviewEvent);
      this._cmbFont.TextUpdate += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(12, 23);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(102, 13);
      this.label1.TabIndex = 0;
      this.label1.Text = "&Font family and size:";
      // 
      // groupBox2
      // 
      this.groupBox2.Controls.Add(this._btnAscii);
      this.groupBox2.Controls.Add(this._btnSymbols);
      this.groupBox2.Controls.Add(this._btnLowerCase);
      this.groupBox2.Controls.Add(this._btnUpperCase);
      this.groupBox2.Controls.Add(this._btnNumbers);
      this.groupBox2.Controls.Add(this.label3);
      this.groupBox2.Controls.Add(this.label2);
      this.groupBox2.Controls.Add(this._editSelectedCharacters);
      this.groupBox2.Location = new System.Drawing.Point(12, 69);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(533, 124);
      this.groupBox2.TabIndex = 1;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Character selection";
      // 
      // _btnAscii
      // 
      this._btnAscii.Location = new System.Drawing.Point(447, 90);
      this._btnAscii.Name = "_btnAscii";
      this._btnAscii.Size = new System.Drawing.Size(73, 23);
      this._btnAscii.TabIndex = 7;
      this._btnAscii.Text = "&All ASCII";
      this._btnAscii.UseVisualStyleBackColor = true;
      this._btnAscii.Click += new System.EventHandler(this._btnAscii_Click);
      // 
      // _btnSymbols
      // 
      this._btnSymbols.Location = new System.Drawing.Point(366, 90);
      this._btnSymbols.Name = "_btnSymbols";
      this._btnSymbols.Size = new System.Drawing.Size(75, 23);
      this._btnSymbols.TabIndex = 6;
      this._btnSymbols.Text = "Sy&mbols";
      this._btnSymbols.UseVisualStyleBackColor = true;
      this._btnSymbols.Click += new System.EventHandler(this._btnSymbols_Click);
      // 
      // _btnLowerCase
      // 
      this._btnLowerCase.Location = new System.Drawing.Point(285, 90);
      this._btnLowerCase.Name = "_btnLowerCase";
      this._btnLowerCase.Size = new System.Drawing.Size(75, 23);
      this._btnLowerCase.TabIndex = 5;
      this._btnLowerCase.Text = "&Lower case";
      this._btnLowerCase.UseVisualStyleBackColor = true;
      this._btnLowerCase.Click += new System.EventHandler(this._btnLowerCase_Click);
      // 
      // _btnUpperCase
      // 
      this._btnUpperCase.Location = new System.Drawing.Point(204, 90);
      this._btnUpperCase.Name = "_btnUpperCase";
      this._btnUpperCase.Size = new System.Drawing.Size(75, 23);
      this._btnUpperCase.TabIndex = 4;
      this._btnUpperCase.Text = "&Upper case";
      this._btnUpperCase.UseVisualStyleBackColor = true;
      this._btnUpperCase.Click += new System.EventHandler(this._btnUpperCase_Click);
      // 
      // _btnNumbers
      // 
      this._btnNumbers.Location = new System.Drawing.Point(123, 90);
      this._btnNumbers.Name = "_btnNumbers";
      this._btnNumbers.Size = new System.Drawing.Size(75, 23);
      this._btnNumbers.TabIndex = 3;
      this._btnNumbers.Text = "&Numbers";
      this._btnNumbers.UseVisualStyleBackColor = true;
      this._btnNumbers.Click += new System.EventHandler(this._btnNumbers_Click);
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(50, 95);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(67, 13);
      this.label3.TabIndex = 2;
      this.label3.Text = "Quick Insert:";
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(12, 23);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(105, 13);
      this.label2.TabIndex = 0;
      this.label2.Text = "&Selected characters:";
      // 
      // _editSelectedCharacters
      // 
      this._editSelectedCharacters.Location = new System.Drawing.Point(123, 19);
      this._editSelectedCharacters.Multiline = true;
      this._editSelectedCharacters.Name = "_editSelectedCharacters";
      this._editSelectedCharacters.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
      this._editSelectedCharacters.Size = new System.Drawing.Size(397, 65);
      this._editSelectedCharacters.TabIndex = 1;
      this._editSelectedCharacters.Text = "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
      this._editSelectedCharacters.TextChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // groupBox3
      // 
      this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.groupBox3.Controls.Add(this._editPreviewY);
      this.groupBox3.Controls.Add(this._editPreviewX);
      this.groupBox3.Controls.Add(this._preview);
      this.groupBox3.Controls.Add(this._editPreviewText);
      this.groupBox3.Controls.Add(this.label7);
      this.groupBox3.Controls.Add(this.label8);
      this.groupBox3.Controls.Add(this.label9);
      this.groupBox3.Location = new System.Drawing.Point(560, 12);
      this.groupBox3.Name = "groupBox3";
      this.groupBox3.Size = new System.Drawing.Size(494, 526);
      this.groupBox3.TabIndex = 4;
      this.groupBox3.TabStop = false;
      this.groupBox3.Text = "Preview";
      // 
      // _editPreviewY
      // 
      this._editPreviewY.Location = new System.Drawing.Point(438, 21);
      this._editPreviewY.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
      this._editPreviewY.Name = "_editPreviewY";
      this._editPreviewY.Size = new System.Drawing.Size(41, 20);
      this._editPreviewY.TabIndex = 5;
      this._editPreviewY.ValueChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _editPreviewX
      // 
      this._editPreviewX.Location = new System.Drawing.Point(373, 21);
      this._editPreviewX.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
      this._editPreviewX.Name = "_editPreviewX";
      this._editPreviewX.Size = new System.Drawing.Size(41, 20);
      this._editPreviewX.TabIndex = 3;
      this._editPreviewX.ValueChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _preview
      // 
      this._preview.BackgroundColour = System.Drawing.Color.Empty;
      this._preview.FontDef = null;
      this._preview.ForegroundColour = System.Drawing.Color.Empty;
      this._preview.Location = new System.Drawing.Point(15, 51);
      this._preview.Name = "_preview";
      this._preview.Position = new System.Drawing.Point(0, 0);
      this._preview.RenderingHint = System.Drawing.Text.TextRenderingHint.SystemDefault;
      this._preview.Size = new System.Drawing.Size(464, 362);
      this._preview.TabIndex = 6;
      // 
      // _editPreviewText
      // 
      this._editPreviewText.Location = new System.Drawing.Point(86, 21);
      this._editPreviewText.Name = "_editPreviewText";
      this._editPreviewText.Size = new System.Drawing.Size(259, 20);
      this._editPreviewText.TabIndex = 1;
      this._editPreviewText.Text = "Hello World";
      this._editPreviewText.TextChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // label7
      // 
      this.label7.AutoSize = true;
      this.label7.Location = new System.Drawing.Point(12, 25);
      this.label7.Name = "label7";
      this.label7.Size = new System.Drawing.Size(68, 13);
      this.label7.TabIndex = 0;
      this.label7.Text = "&Preview text:";
      // 
      // label8
      // 
      this.label8.AutoSize = true;
      this.label8.Location = new System.Drawing.Point(351, 24);
      this.label8.Name = "label8";
      this.label8.Size = new System.Drawing.Size(16, 13);
      this.label8.TabIndex = 2;
      this.label8.Text = "at";
      // 
      // label9
      // 
      this.label9.AutoSize = true;
      this.label9.Location = new System.Drawing.Point(420, 24);
      this.label9.Name = "label9";
      this.label9.Size = new System.Drawing.Size(10, 13);
      this.label9.TabIndex = 4;
      this.label9.Text = ",";
      // 
      // groupBox4
      // 
      this.groupBox4.Controls.Add(this._btnChooseForeground);
      this.groupBox4.Controls.Add(this._editTftHeight);
      this.groupBox4.Controls.Add(this._editExtraH);
      this.groupBox4.Controls.Add(this._editExtraW);
      this.groupBox4.Controls.Add(this._editCharacterSpacing);
      this.groupBox4.Controls.Add(this._editTftWidth);
      this.groupBox4.Controls.Add(this._cmbStyles);
      this.groupBox4.Controls.Add(this._cmbRenderingHint);
      this.groupBox4.Controls.Add(this._btnChooseBackground);
      this.groupBox4.Controls.Add(this._editForeground);
      this.groupBox4.Controls.Add(this._editBackground);
      this.groupBox4.Controls.Add(this._foregroundPreview);
      this.groupBox4.Controls.Add(this._backgroundPreview);
      this.groupBox4.Controls.Add(this.label17);
      this.groupBox4.Controls.Add(this.label6);
      this.groupBox4.Controls.Add(this.label12);
      this.groupBox4.Controls.Add(this.label11);
      this.groupBox4.Controls.Add(this.label10);
      this.groupBox4.Controls.Add(this.label14);
      this.groupBox4.Controls.Add(this.label16);
      this.groupBox4.Controls.Add(this.label13);
      this.groupBox4.Controls.Add(this.label5);
      this.groupBox4.Controls.Add(this.label4);
      this.groupBox4.Location = new System.Drawing.Point(12, 199);
      this.groupBox4.Name = "groupBox4";
      this.groupBox4.Size = new System.Drawing.Size(533, 189);
      this.groupBox4.TabIndex = 2;
      this.groupBox4.TabStop = false;
      this.groupBox4.Text = "Rendering Options";
      // 
      // _btnChooseForeground
      // 
      this._btnChooseForeground.Location = new System.Drawing.Point(221, 49);
      this._btnChooseForeground.Name = "_btnChooseForeground";
      this._btnChooseForeground.Size = new System.Drawing.Size(30, 21);
      this._btnChooseForeground.TabIndex = 6;
      this._btnChooseForeground.Text = "...";
      this._btnChooseForeground.UseVisualStyleBackColor = true;
      this._btnChooseForeground.Click += new System.EventHandler(this._btnChooseForeground_Click);
      // 
      // _editTftHeight
      // 
      this._editTftHeight.Location = new System.Drawing.Point(199, 131);
      this._editTftHeight.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
      this._editTftHeight.Name = "_editTftHeight";
      this._editTftHeight.Size = new System.Drawing.Size(52, 20);
      this._editTftHeight.TabIndex = 15;
      this._editTftHeight.Value = new decimal(new int[] {
            240,
            0,
            0,
            0});
      this._editTftHeight.ValueChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _editExtraH
      // 
      this._editExtraH.Location = new System.Drawing.Point(415, 157);
      this._editExtraH.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
      this._editExtraH.Name = "_editExtraH";
      this._editExtraH.Size = new System.Drawing.Size(37, 20);
      this._editExtraH.TabIndex = 22;
      this._editExtraH.ValueChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _editExtraW
      // 
      this._editExtraW.Location = new System.Drawing.Point(354, 157);
      this._editExtraW.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
      this._editExtraW.Name = "_editExtraW";
      this._editExtraW.Size = new System.Drawing.Size(37, 20);
      this._editExtraW.TabIndex = 20;
      this._editExtraW.ValueChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _editCharacterSpacing
      // 
      this._editCharacterSpacing.Location = new System.Drawing.Point(118, 157);
      this._editCharacterSpacing.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
      this._editCharacterSpacing.Name = "_editCharacterSpacing";
      this._editCharacterSpacing.Size = new System.Drawing.Size(52, 20);
      this._editCharacterSpacing.TabIndex = 17;
      this._editCharacterSpacing.ValueChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _editTftWidth
      // 
      this._editTftWidth.Location = new System.Drawing.Point(118, 131);
      this._editTftWidth.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
      this._editTftWidth.Name = "_editTftWidth";
      this._editTftWidth.Size = new System.Drawing.Size(52, 20);
      this._editTftWidth.TabIndex = 13;
      this._editTftWidth.Value = new decimal(new int[] {
            320,
            0,
            0,
            0});
      this._editTftWidth.ValueChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _cmbStyles
      // 
      this._cmbStyles.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
      this._cmbStyles.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
      this._cmbStyles.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this._cmbStyles.FormattingEnabled = true;
      this._cmbStyles.Items.AddRange(new object[] {
            "Regular",
            "Bold",
            "Italic",
            "Bold, Italic"});
      this._cmbStyles.Location = new System.Drawing.Point(120, 103);
      this._cmbStyles.MaxDropDownItems = 50;
      this._cmbStyles.Name = "_cmbStyles";
      this._cmbStyles.Size = new System.Drawing.Size(131, 21);
      this._cmbStyles.TabIndex = 11;
      this._cmbStyles.SelectedIndexChanged += new System.EventHandler(this.UpdatePreviewEvent);
      this._cmbStyles.TextUpdate += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _cmbRenderingHint
      // 
      this._cmbRenderingHint.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
      this._cmbRenderingHint.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
      this._cmbRenderingHint.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this._cmbRenderingHint.FormattingEnabled = true;
      this._cmbRenderingHint.Items.AddRange(new object[] {
            "System default",
            "Single bit per pixel grid fit",
            "Single bit per pixel",
            "Anti alias grid fit",
            "Anti alias",
            "Clear type grid fit"});
      this._cmbRenderingHint.Location = new System.Drawing.Point(120, 76);
      this._cmbRenderingHint.MaxDropDownItems = 50;
      this._cmbRenderingHint.Name = "_cmbRenderingHint";
      this._cmbRenderingHint.Size = new System.Drawing.Size(240, 21);
      this._cmbRenderingHint.TabIndex = 9;
      this._cmbRenderingHint.SelectedIndexChanged += new System.EventHandler(this.UpdatePreviewEvent);
      this._cmbRenderingHint.TextUpdate += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _btnChooseBackground
      // 
      this._btnChooseBackground.Location = new System.Drawing.Point(221, 23);
      this._btnChooseBackground.Name = "_btnChooseBackground";
      this._btnChooseBackground.Size = new System.Drawing.Size(30, 21);
      this._btnChooseBackground.TabIndex = 2;
      this._btnChooseBackground.Text = "...";
      this._btnChooseBackground.UseVisualStyleBackColor = true;
      this._btnChooseBackground.Click += new System.EventHandler(this._btnChooseBackground_Click);
      // 
      // _editForeground
      // 
      this._editForeground.Location = new System.Drawing.Point(120, 50);
      this._editForeground.Name = "_editForeground";
      this._editForeground.Size = new System.Drawing.Size(95, 20);
      this._editForeground.TabIndex = 5;
      this._editForeground.Text = "FFFFFF";
      this._editForeground.TextChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _editBackground
      // 
      this._editBackground.Location = new System.Drawing.Point(120, 24);
      this._editBackground.Name = "_editBackground";
      this._editBackground.Size = new System.Drawing.Size(95, 20);
      this._editBackground.TabIndex = 1;
      this._editBackground.Text = "000000";
      this._editBackground.TextChanged += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // _foregroundPreview
      // 
      this._foregroundPreview.Location = new System.Drawing.Point(257, 50);
      this._foregroundPreview.Name = "_foregroundPreview";
      this._foregroundPreview.Size = new System.Drawing.Size(103, 20);
      this._foregroundPreview.TabIndex = 7;
      this._foregroundPreview.Value = System.Drawing.Color.Black;
      // 
      // _backgroundPreview
      // 
      this._backgroundPreview.Location = new System.Drawing.Point(257, 24);
      this._backgroundPreview.Name = "_backgroundPreview";
      this._backgroundPreview.Size = new System.Drawing.Size(103, 20);
      this._backgroundPreview.TabIndex = 3;
      this._backgroundPreview.Value = System.Drawing.Color.Black;
      // 
      // label17
      // 
      this.label17.AutoSize = true;
      this.label17.Location = new System.Drawing.Point(397, 159);
      this.label17.Name = "label17";
      this.label17.Size = new System.Drawing.Size(12, 13);
      this.label17.TabIndex = 21;
      this.label17.Text = "x";
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Location = new System.Drawing.Point(179, 134);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(12, 13);
      this.label6.TabIndex = 14;
      this.label6.Text = "x";
      // 
      // label12
      // 
      this.label12.AutoSize = true;
      this.label12.Location = new System.Drawing.Point(14, 106);
      this.label12.Name = "label12";
      this.label12.Size = new System.Drawing.Size(38, 13);
      this.label12.TabIndex = 10;
      this.label12.Text = "Styles:";
      // 
      // label11
      // 
      this.label11.AutoSize = true;
      this.label11.Location = new System.Drawing.Point(14, 79);
      this.label11.Name = "label11";
      this.label11.Size = new System.Drawing.Size(79, 13);
      this.label11.TabIndex = 8;
      this.label11.Text = "Rendering hint:";
      // 
      // label10
      // 
      this.label10.AutoSize = true;
      this.label10.Location = new System.Drawing.Point(14, 53);
      this.label10.Name = "label10";
      this.label10.Size = new System.Drawing.Size(96, 13);
      this.label10.TabIndex = 4;
      this.label10.Text = "Foreground colour:";
      // 
      // label14
      // 
      this.label14.AutoSize = true;
      this.label14.Location = new System.Drawing.Point(176, 159);
      this.label14.Name = "label14";
      this.label14.Size = new System.Drawing.Size(33, 13);
      this.label14.TabIndex = 18;
      this.label14.Text = "pixels";
      // 
      // label16
      // 
      this.label16.AutoSize = true;
      this.label16.Location = new System.Drawing.Point(218, 159);
      this.label16.Name = "label16";
      this.label16.Size = new System.Drawing.Size(130, 13);
      this.label16.TabIndex = 19;
      this.label16.Text = "Extra bounding box pixels:";
      // 
      // label13
      // 
      this.label13.AutoSize = true;
      this.label13.Location = new System.Drawing.Point(14, 159);
      this.label13.Name = "label13";
      this.label13.Size = new System.Drawing.Size(96, 13);
      this.label13.TabIndex = 16;
      this.label13.Text = "&Character spacing:";
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(14, 134);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(78, 13);
      this.label5.TabIndex = 12;
      this.label5.Text = "&TFT resolution:";
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(14, 27);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(100, 13);
      this.label4.TabIndex = 0;
      this.label4.Text = "&Background colour:";
      // 
      // groupBox5
      // 
      this.groupBox5.Controls.Add(this._logo);
      this.groupBox5.Controls.Add(this._btnLoad);
      this.groupBox5.Controls.Add(this._btnSaveStm32plus);
      this.groupBox5.Controls.Add(this._btnSaveArduino);
      this.groupBox5.Controls.Add(this._btnMemory);
      this.groupBox5.Controls.Add(this._editCompressionOptions);
      this.groupBox5.Controls.Add(this.label15);
      this.groupBox5.Location = new System.Drawing.Point(12, 394);
      this.groupBox5.Name = "groupBox5";
      this.groupBox5.Size = new System.Drawing.Size(533, 144);
      this.groupBox5.TabIndex = 3;
      this.groupBox5.TabStop = false;
      this.groupBox5.Text = "Control Options";
      // 
      // _logo
      // 
      this._logo.Cursor = System.Windows.Forms.Cursors.Hand;
      this._logo.Image = global::LzgFontConv.Properties.Resources.logo;
      this._logo.Location = new System.Drawing.Point(240, 50);
      this._logo.Name = "_logo";
      this._logo.Size = new System.Drawing.Size(280, 78);
      this._logo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
      this._logo.TabIndex = 3;
      this._logo.TabStop = false;
      this._tooltip.SetToolTip(this._logo, "Click to visit Andy\'s Workshop web page");
      this._logo.Click += new System.EventHandler(this._logo_Click);
      // 
      // _btnLoad
      // 
      this._btnLoad.Location = new System.Drawing.Point(15, 50);
      this._btnLoad.Name = "_btnLoad";
      this._btnLoad.Size = new System.Drawing.Size(200, 23);
      this._btnLoad.TabIndex = 1;
      this._btnLoad.Text = "Load...";
      this._btnLoad.UseVisualStyleBackColor = true;
      this._btnLoad.Click += new System.EventHandler(this._btnLoad_Click);
      // 
      // _btnSaveStm32plus
      // 
      this._btnSaveStm32plus.Location = new System.Drawing.Point(15, 108);
      this._btnSaveStm32plus.Name = "_btnSaveStm32plus";
      this._btnSaveStm32plus.Size = new System.Drawing.Size(200, 23);
      this._btnSaveStm32plus.TabIndex = 3;
      this._btnSaveStm32plus.Text = "Save for stm32plus...";
      this._btnSaveStm32plus.UseVisualStyleBackColor = true;
      this._btnSaveStm32plus.Click += new System.EventHandler(this._btnSaveStm32plus_Click);
      // 
      // _btnSaveArduino
      // 
      this._btnSaveArduino.Location = new System.Drawing.Point(15, 79);
      this._btnSaveArduino.Name = "_btnSaveArduino";
      this._btnSaveArduino.Size = new System.Drawing.Size(200, 23);
      this._btnSaveArduino.TabIndex = 2;
      this._btnSaveArduino.Text = "Save for Arduino...";
      this._btnSaveArduino.UseVisualStyleBackColor = true;
      this._btnSaveArduino.Click += new System.EventHandler(this._btnSaveArduino_Click);
      // 
      // _btnMemory
      // 
      this._btnMemory.Location = new System.Drawing.Point(15, 21);
      this._btnMemory.Name = "_btnMemory";
      this._btnMemory.Size = new System.Drawing.Size(200, 23);
      this._btnMemory.TabIndex = 0;
      this._btnMemory.Text = "Estimate memory requirement";
      this._btnMemory.UseVisualStyleBackColor = true;
      this._btnMemory.Click += new System.EventHandler(this._btnMemory_Click);
      // 
      // _editCompressionOptions
      // 
      this._editCompressionOptions.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
      this._editCompressionOptions.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
      this._editCompressionOptions.FormattingEnabled = true;
      this._editCompressionOptions.Items.AddRange(new object[] {
            "ili9325 64",
            "ili9325 262",
            "ili9325a 64",
            "ili9325a 262",
            "ili9327 64",
            "ili9327 262",
            "ili9481 64",
            "ili9481 262",
            "hx8347a 64",
            "hx8352a 64",
            "hx8352a 262",
            "mc2pa8201 64",
            "mc2pa8201 262",
            "mc2pa8201 16",
            "lds285 64",
            "lds285 262",
            "lds285 16",
            "r61523 64",
            "r61523 262",
            "r61523 16"});
      this._editCompressionOptions.Location = new System.Drawing.Point(350, 18);
      this._editCompressionOptions.MaxDropDownItems = 50;
      this._editCompressionOptions.Name = "_editCompressionOptions";
      this._editCompressionOptions.Size = new System.Drawing.Size(170, 21);
      this._editCompressionOptions.TabIndex = 5;
      this._editCompressionOptions.Text = "mc2pa8201 262";
      this._editCompressionOptions.SelectedIndexChanged += new System.EventHandler(this.UpdatePreviewEvent);
      this._editCompressionOptions.TextUpdate += new System.EventHandler(this.UpdatePreviewEvent);
      // 
      // label15
      // 
      this.label15.AutoSize = true;
      this.label15.Location = new System.Drawing.Point(237, 21);
      this.label15.Name = "label15";
      this.label15.Size = new System.Drawing.Size(107, 13);
      this.label15.TabIndex = 4;
      this.label15.Text = "Compression options:";
      // 
      // FormMain
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(1070, 553);
      this.Controls.Add(this.groupBox5);
      this.Controls.Add(this.groupBox4);
      this.Controls.Add(this.groupBox3);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.groupBox1);
      this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
      this.MinimumSize = new System.Drawing.Size(1086, 591);
      this.Name = "FormMain";
      this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
      this.Text = "LZG Font Converter and Compressor";
      this.Load += new System.EventHandler(this.FormMain_Load);
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      this.groupBox2.ResumeLayout(false);
      this.groupBox2.PerformLayout();
      this.groupBox3.ResumeLayout(false);
      this.groupBox3.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this._editPreviewY)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this._editPreviewX)).EndInit();
      this.groupBox4.ResumeLayout(false);
      this.groupBox4.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this._editTftHeight)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this._editExtraH)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this._editExtraW)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this._editCharacterSpacing)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this._editTftWidth)).EndInit();
      this.groupBox5.ResumeLayout(false);
      this.groupBox5.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this._logo)).EndInit();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.GroupBox groupBox2;
    private System.Windows.Forms.Button _btnAscii;
    private System.Windows.Forms.Button _btnSymbols;
    private System.Windows.Forms.Button _btnLowerCase;
    private System.Windows.Forms.Button _btnUpperCase;
    private System.Windows.Forms.Button _btnNumbers;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.TextBox _editSelectedCharacters;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.ComboBox _cmbSize;
    private System.Windows.Forms.ComboBox _cmbFont;
    private LzgFontConv.PreviewPanel _preview;
    private System.Windows.Forms.GroupBox groupBox3;
    private System.Windows.Forms.TextBox _editPreviewText;
    private System.Windows.Forms.Label label7;
    private System.Windows.Forms.Label label8;
    private System.Windows.Forms.Label label9;
    private System.Windows.Forms.GroupBox groupBox4;
    private System.Windows.Forms.Button _btnChooseBackground;
    private System.Windows.Forms.TextBox _editBackground;
    private LzgFontConv.ColourPanel _backgroundPreview;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.ColorDialog _colorDialog;
    private System.Windows.Forms.GroupBox groupBox5;
    private System.Windows.Forms.Button _btnLoad;
    private System.Windows.Forms.Button _btnSaveStm32plus;
    private System.Windows.Forms.Button _btnSaveArduino;
    private System.Windows.Forms.Button _btnMemory;
    private System.Windows.Forms.PictureBox _logo;
    private System.Windows.Forms.Button _btnChooseForeground;
    private System.Windows.Forms.TextBox _editForeground;
    private ColourPanel _foregroundPreview;
    private System.Windows.Forms.Label label10;
    private System.Windows.Forms.ComboBox _cmbRenderingHint;
    private System.Windows.Forms.Label label11;
    private System.Windows.Forms.ComboBox _cmbStyles;
    private System.Windows.Forms.Label label12;
    private System.Windows.Forms.NumericUpDown _editPreviewY;
    private System.Windows.Forms.NumericUpDown _editPreviewX;
    private System.Windows.Forms.NumericUpDown _editTftHeight;
    private System.Windows.Forms.NumericUpDown _editTftWidth;
    private System.Windows.Forms.NumericUpDown _editCharacterSpacing;
    private System.Windows.Forms.Label label14;
    private System.Windows.Forms.Label label13;
    private System.Windows.Forms.ComboBox _editCompressionOptions;
    private System.Windows.Forms.Label label15;
    private System.Windows.Forms.ToolTip _tooltip;
    private System.Windows.Forms.NumericUpDown _editExtraH;
    private System.Windows.Forms.NumericUpDown _editExtraW;
    private System.Windows.Forms.Label label17;
    private System.Windows.Forms.Label label16;
  }
}

