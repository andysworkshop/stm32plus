using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;
using System.Reflection;
using System.Diagnostics;

namespace bm2rgbi {
  class Program {
  /// <summary>
  /// Main entry point
  /// </summary>

    static void Main(string[] args_) {
      new Program().run(args_);
    }

    
  /// <summary>
  /// Run the program
  /// </summary>
    
    public void run(string[] args_) {
      
      ProgramArgs args;
      Bitmap bm;

      try {

        args=new ProgramArgs(args_);
        
        bm=new Bitmap(new FileStream(args.InputBitmapFilename,FileMode.Open,FileAccess.Read,FileShare.Read));
        printBitmapInfo(bm);

        Console.WriteLine("Writing converted bitmap");

        using(FileStream fs=new FileStream(args.OutputBitmapFilename,FileMode.Create,FileAccess.Write,FileShare.None))
          args.TargetDevice.convert(bm,fs);

        if(args.Compress) {
          Console.WriteLine("Compressing converted bitmap");
          compress(args.OutputBitmapFilename);
        }

        Console.WriteLine("Completed OK");
      }
      catch(Exception ex) {
        Console.WriteLine(ex.Message);
      }
    }


    /// <summary>
    /// Compress the target
    /// </summary>

    private void compress(string filename) {
      
      string procpath,tempfile;
      ProcessStartInfo psi;
      Process p;
      FileInfo fi;
      long oldlength,percent;

      // get our executable location

      procpath=Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);

      // get the temp file path

      tempfile=filename+".lzg";

      // get the old length

      fi=new FileInfo(filename);
      oldlength=fi.Length;

      // run the compressor

      psi=new ProcessStartInfo(Path.Combine(procpath,"lzg.exe"),"-1 "+filename+" "+tempfile);
      psi.CreateNoWindow=true;
      psi.WindowStyle=ProcessWindowStyle.Hidden;

      if((p=Process.Start(psi))==null)
        throw new Exception("Failed to start compression process (lzg.exe)");

      // wait for it to finish

      p.WaitForExit();

      // if the temp file is >0 bytes we assume it worked :-;

      fi=new FileInfo(tempfile);
      if(fi.Length==0)
        throw new Exception("The compression process (lzg.exe) failed");

      File.Delete(filename);
      File.Move(tempfile,filename);

      percent=(oldlength-fi.Length)*100/oldlength;
      Console.WriteLine("Compression completed: "+oldlength+" down to "+fi.Length+" ("+percent+"%) bytes");
    }


    /// <summary>
    /// show some bm info
    /// </summary>

    private void printBitmapInfo(Bitmap bm_) {

      Console.WriteLine("Width:  "+bm_.Width);
      Console.WriteLine("Height: "+bm_.Height);
      Console.WriteLine("Format: "+bm_.PixelFormat.ToString());
    }
  }
}
