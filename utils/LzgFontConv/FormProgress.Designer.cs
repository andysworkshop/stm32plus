namespace LzgFontConv {
  partial class FormProgress {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing) {
      if (disposing && (components != null)) {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent() {
      this.ProgressBar = new System.Windows.Forms.ProgressBar();
      this.SuspendLayout();
      // 
      // ProgressBar
      // 
      this.ProgressBar.Location = new System.Drawing.Point(12, 12);
      this.ProgressBar.Name = "ProgressBar";
      this.ProgressBar.Size = new System.Drawing.Size(281, 23);
      this.ProgressBar.Step = 1;
      this.ProgressBar.TabIndex = 0;
      // 
      // FormProgress
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(305, 46);
      this.ControlBox = false;
      this.Controls.Add(this.ProgressBar);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "FormProgress";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
      this.Text = "Please Wait...";
      this.ResumeLayout(false);

    }

    #endregion

    public System.Windows.Forms.ProgressBar ProgressBar;

  }
}