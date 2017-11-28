/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace LzgFontConv {
  
  /// <summary>
  /// class to manage the extraction of the character definitions
  /// </summary>
  
  public class CharacterDefinitions {

    /// <summary>
    /// properties
    /// </summary>
    
    public int Height { get; private set; }
    public List<CharDef> Definitions { get; private set; }


    /// <summary>
    /// parse character definitions. The input string must be sorted.
    /// </summary>
    
    public void ParseCharacters(Control refCtrl,FontDefinition fd,string str,bool doCompression) {

      CharDef cd;
      FormProgress progress;
      Font font;

      progress=new FormProgress();
      progress.ProgressBar.Minimum=0;
      progress.ProgressBar.Maximum=str.Length;

      if(doCompression)
        progress.Show();

      // create a font

      font=new Font(fd.Family,fd.Size,fd.Style,GraphicsUnit.Pixel);
      this.Height=Convert.ToInt32(font.GetHeight());

      try {
        this.Definitions=new List<CharDef>();

        foreach(char c in str) {

          // update progress

          progress.ProgressBar.PerformStep();
          Application.DoEvents();

          // create character

          cd=new CharDef();
          cd.Parse(refCtrl,fd,font,c,doCompression);

          Definitions.Add(cd);
        }
      }
      finally {

        if(doCompression)
          progress.Close();
      }
    }
  }
}
