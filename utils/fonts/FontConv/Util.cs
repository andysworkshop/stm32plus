using System;
using System.Windows.Forms;


namespace FontConv
{
  class Util
  {
    static public void Error(Exception ex_)
    {
      MessageBox.Show(ex_.Message,"Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
    }
  }
}
