/*
 * This file is shared between the open source stm32plus and
 * Arduino XMEM graphics libraries.
 * 
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

using System;
using System.Windows.Forms;

namespace LzgFontConv {

  /// <summary>
  /// very rough estimate of the memory required for this font
  /// </summary>
  
  public class MemoryEstimator {

    /// <summary>
    /// properties
    /// </summary>
    
    public FontDefinition FontDef { get; set; }


    /// <summary>
    /// estimate the size
    /// </summary>

    public int Estimate(Control refCtrl) {
      
      char[] chars;
      CharacterDefinitions charDefs=new CharacterDefinitions();
      int byteTotal;

      Cursor.Current=Cursors.WaitCursor;
      
      try {

        // the selected characters need to be sorted

        chars=this.FontDef.Characters.ToCharArray();
        Array.Sort(chars);
        this.FontDef.Characters=new string(chars);

        // create the char definitions

        charDefs.ParseCharacters(refCtrl,this.FontDef,this.FontDef.Characters,true);

        // sum up the bytes

        byteTotal=0;
        foreach(CharDef cd in charDefs.Definitions)
          byteTotal+=cd.CompressedBytes.Length;

        // add on the fontchar requirements (4 byte struct on the arduino)

        byteTotal+=this.FontDef.Characters.Length*4;
        return byteTotal;
      }
      finally {
        Cursor.Current=Cursors.Default;
      }
    }
  }
}
