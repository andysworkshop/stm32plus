/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System.IO;
using System.Windows.Forms;

namespace LzgFontConv {
  
  /// <summary>
  /// wrapper for the 3 filenames we need
  /// </summary>

  public class FontFilenames {

    public string Header { get; set; }
    public string Source { get; set; }
    public string Definition { get; set; }


    /// <summary>
    /// Use the system dialog to get the font filenames
    /// </summary>

    public DialogResult GetFilenames() {

      SaveFileDialog sfd;

      sfd=new SaveFileDialog();
      sfd.Filter="Saved Fonts (*.xml)|*.xml|All Files (*.*)|*.*||";
      
      // show the dialog

      if(sfd.ShowDialog()==DialogResult.Cancel)
        return DialogResult.Cancel;

      // get teh XML definition name and calculate the others

      this.Definition=sfd.FileName;
      this.Header=Path.Combine(Path.GetDirectoryName(sfd.FileName),Path.GetFileNameWithoutExtension(sfd.FileName)+".h");
      this.Source=Path.Combine(Path.GetDirectoryName(sfd.FileName),Path.GetFileNameWithoutExtension(sfd.FileName)+".cpp");

      return DialogResult.OK;
    }
  }
}
