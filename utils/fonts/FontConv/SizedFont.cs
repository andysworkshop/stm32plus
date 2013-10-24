using System;
using System.Collections.Generic;
using System.Xml;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Windows.Forms;


namespace FontConv {

/*
 * Sized font class
 */
  
  public class SizedFont {

  /*
   * members
   */
    
    private SortedDictionary<char,char> _selections=new SortedDictionary<char,char>();
    private PrivateFontCollection _winFontFamilies=new PrivateFontCollection();

    public String Filename { get; set; }
    public String Identifier { get; set; }
    public String Name { get; set; }
    public int Size { get; set; }
    public int ExtraLines { get; set; }
    public int CharSpace { get; set; }
    public int XOffset { get; set; }
    public int YOffset { get; set; }
    public Font GdiFont { get; set; }


  /*
   * constructor
   */

    public SizedFont(String filename,int size) {
      
      this.Filename=filename;
      this.Size=size;
      this.XOffset=0;
      this.YOffset=0;
      this.ExtraLines=0;
      this.CharSpace=0;

      _winFontFamilies.AddFontFile(filename);
      
      CreateFont();
    }


  /*
   * return characters
   */

    public ICollection<char> Characters() {
      return _selections.Keys;
    }


  /*
   * return the selected count
   */
    
    public int SelectionCount() {
      return _selections.Count;
    }


    /*
     * Get first selected char
     */

    public char GetFirstCharacter() {

      IEnumerator<char> it;

      if(Characters().Count==0)
        throw new Exception("No characters are selected");

      it=Characters().GetEnumerator();
      it.MoveNext();

      return it.Current;
    }


  /*
   * check if contains char
   */

    public bool ContainsChar(char c_) {
      return _selections.ContainsKey(c_);
    }


  /*
   * toggle selection state
   */

    public void ToggleSelected(char c) {

      if(ContainsChar(c))
        Remove(c);
      else
        Add(c);
    }


  /*
   * add char
   */

    public void Add(char c_) {
      _selections[c_]=c_;
    }


  /*
   * remove char
   */

    public void Remove(char c_) {
      _selections.Remove(c_);
    }

    
  /*
   * create previews for all selected
   */

    public List<CharPreview> CreatePreviews() {
      
      List<CharPreview> previews;

      previews=new List<CharPreview>();
      
      foreach(char c in this.Characters())
        previews.Add(CreatePreview(c));

      return previews;
    }


    /*
     * Create the preview
     */

    public CharPreview CreatePreview(char c) {
      
      CharPreview cp;

      cp=new CharPreview();
      cp.Font=this.GdiFont;
      cp.Create(c,this.XOffset,this.YOffset,this.ExtraLines);
      cp.Tag=c;

      return cp;
    }


  /*
   * create GDI font
   */

    public void CreateFont() {
      
      this.GdiFont=new Font(
        _winFontFamilies.Families[0],
        this.Size,
        GraphicsUnit.Pixel);

      CreateFontName();
      CreateFontId();
    }


  /*
   * font name from filename
   */

    private void CreateFontName() {
      String str;

      str=Path.GetFileName(this.Filename);
      str=Path.GetFileNameWithoutExtension(str);

      this.Name=str.ToUpperInvariant().Replace(" ","_");
      this.Name=this.Name.Replace("-","_");
    }


  /*
   * font id from filename
   */

    private void CreateFontId() {
      this.Identifier="FDEF_"+this.Name;
    }
  }
}
