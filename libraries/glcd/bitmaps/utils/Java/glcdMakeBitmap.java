import processing.core.*; 
import processing.xml.*; 

import java.awt.dnd.*; 
import java.awt.datatransfer.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class glcdMakeBitmap extends PApplet {

/**
 * glcdBitmap. 
 * 
 * Creates a bitmap definition file that can be included in an Arduino sketch
 * to display a bitmap image on a graphics LCD using the Arduino GLCD library.
 *
 * Created  6 Nov  2008  Copyright Michael Margolis 2008,2010
 * updated  14 Feb 2010  
 */




String destinationOffset  ;
String aggregateHeader = "allBitmaps.h";

PImage bitmap;
PFont aFont;

static class imgData
{
  static int width;
  static int height;
  static int pages;
  static int bytes;
  static int pixels;
  static String sourceFileName; // excludes path
  static String baseName;       // excludes path and extension
}

public void setup() 
{
  size(256, 256);
  background(255); // background to white
  noStroke();  // outline to black

  aFont   = createFont("Arial.bold", 12);
  textFont(aFont) ; 
  clearWindow();
  //destinationOffset =  sketchPath("") + ".." + File.separator ; // up one directory 
  // use the following when the code is run two directories below the bitmaps directory 
  destinationOffset = sketchPath("") + ".." + File.separator + ".." + File.separator ;
}

public void draw()
{
}

public void mousePressed() {  
  listImageHeaderFiles( destinationOffset, aggregateHeader );
}

public void clearWindow()
{
  fill(255);
  rect(0,0, width, height);
  fill(0); // font in black
  text("Drop image file (gif, jpg, bmp, tga, png) here", 10,height - 50);
  text("Click window to refresh " + aggregateHeader, 20,height - 30);
  bitmap = null;
}

public void convert(String sourcePath, String sourceFile)
{
  clearWindow();
  bitmap = loadImage(sourcePath);   // load the image
  if( bitmap != null)
  {
    imgData.width = bitmap.width;
    imgData.height = bitmap.height;
    imgData.pages = (imgData.height + 7)/8; // round up so each page contains 8 pixels    
    imgData.bytes = imgData.width * imgData.pages;
    imgData.pixels =  imgData.width * imgData.height;
    image(bitmap,0,0);
    print("Width = "); 
    println(imgData.width);
    print("Height = "); 
    println(imgData.height);
    print("Pages = "); 
    println(imgData.pages);
    print("Image bytes = "); 
    println(imgData.bytes);     
    print("Pixels ="); 
    println(imgData.pixels);

    bitmap.loadPixels();
    imgData.baseName = getBaseName(sourcePath);
    imgData.sourceFileName = sourceFile;
    writeFile(); // writes a file using arguments defined in the imgData structure   

    //println("created header file for " + sourcePath);    // prints the full path   
    println("\nCreated " + imgData.baseName + ".h" + " using " +  sourceFile); // show on command line  
    text("Created file: " + imgData.baseName + ".h", 20,height - 10); // display text in window
    // now update the headernts.h file so the new image is included
    listImageHeaderFiles(destinationOffset, aggregateHeader);
  }
  else
  {
    println("Unable to load image");  
    text("Unable to load image", 20,height - 10);
  }
}

public void writeFile()
{
  print("Basename = "); 
  println(imgData.baseName);  
  String outFileName = destinationOffset + imgData.baseName + ".h"; 
  print("Output file name = "); 
  println(outFileName);

  PrintWriter output;
  output = createWriter(outFileName);

  output.println("/* " + imgData.baseName + " bitmap file for GLCD library */");
  output.println("/* Bitmap created from " +  imgData.sourceFileName + "      */");
  String[] monthName = { "","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"  };
  output.println("/* Date: " +  day() + " " + monthName[month()] +  " " +  year() + "      */" ); 
  output.println("/* Image Pixels = "  + imgData.pixels + "    */"); 
  output.println("/* Image Bytes  = "   + imgData.bytes  + "     */");   
  output.println();

  output.println("#include <inttypes.h>");
  output.println("#include <avr/pgmspace.h>");
  output.println();
  output.println("#ifndef " +  imgData.baseName + "_H");
  output.println("#define " +  imgData.baseName + "_H");

  output.println();
  output.print("static uint8_t ");
  output.print( imgData.baseName);
  output.println("[] PROGMEM = {");

  output.print("  ");
  output.print(imgData.width);   // note width and height are bytes so 256 will be 0
  output.println(", // width"); 
  output.print("  ");
  output.print(imgData.height);
  output.println(", // height"); 
  stroke(0);
  for(int page=0; page < imgData.pages; page++) {
    output.println("\n  /* page " + page + " (lines "  + page*8 + "-" + (page*8+7) + ") */");
    output.print("  "); 
    for(int x=0; x < imgData.width; x++) {
      output.print( "0x" + Integer.toHexString(getValue(x,page))) ;   
      if( (x == (width-1)) && (page == (((height +7)/8)-1))  )
        println("\n"); // this is the last element so new line instead of comma
      else   
        output.print(",");   // comma on all but last entry
      if( x % 16 == 15)
        output.print("\n  ");
    }
  }  
  output.print("\n};\n");
  output.println("#endif");

  output.flush(); // Write the remaining data
  output.close(); // Finish the file
}

// return the byte representing data a the given page and x offset
public int getValue( int x, int page) {  
  //print("page= ");println(page);
  int val = 0; 
  for( byte bit=0; bit < 8; bit++) {
    int y = page * 8 + bit;
    int pos = y * imgData.width + x; 
    if(pos < imgData.width * imgData.height) // skip padding if at the end of real data
    {       
      int c = bitmap.pixels[pos];    // get the color
      int r = (c >> 16) & 0xFF;      // get the rgb values 
      int g = (c >>  8) & 0xFF;
      int b = c         & 0xFF;    
      if( r < 128 || g < 128 || b < 128) // test if all values are closer to dark than light
      {
        val |=  (1 << bit);   // set the bit if this pixel is more dark than light
        point(x, y);   // draw the point in the window
      }
    }
  }
  return val;
}

public String getBaseName(String fileName) 
{
  File tmpFile = new File(fileName);
  tmpFile.getName();
  int whereDot = tmpFile.getName().lastIndexOf('.');
  if (0 < whereDot && whereDot <= tmpFile.getName().length() - 2 )
  {
    return tmpFile.getName().substring(0, whereDot);
    //extension = filename.substring(whereDot+1);
  }    
  return "";
}

DropTarget dt = new DropTarget(this, new DropTargetListener() {
  public void dragEnter(DropTargetDragEvent event) {
    //System.out.println("dragEnter " + event);
    event.acceptDrag(DnDConstants.ACTION_COPY);
  }   
  public void dragExit(DropTargetEvent event) {
    //System.out.println("dragExit " + event);
  }   
  public void dragOver(DropTargetDragEvent event) {
    //System.out.println("dragOver " + event);
    event.acceptDrag(DnDConstants.ACTION_COPY);
  }   
  public void dropActionChanged(DropTargetDragEvent event) {
    //System.out.println("dropActionChanged " + event);
  }   
  public void drop(DropTargetDropEvent event) {
    //System.out.println("drop " + event);
    event.acceptDrop(DnDConstants.ACTION_COPY);
    Transferable transferable = event.getTransferable();
    DataFlavor flavors[] = transferable.getTransferDataFlavors();
    int successful = 0;
    for (int i = 0; i < flavors.length; i++) { 
      try {   
        Object stuff = transferable.getTransferData(flavors[i]);
        if (!(stuff instanceof java.util.List)) continue;
        java.util.List list = (java.util.List) stuff;
        for (int j = 0; j < list.size(); j++) {     
          Object item = list.get(j);
          if (item instanceof File) {  
            File file = (File) item;
            String filePath = file.getPath();
            String fileName = file.getName();
            convert(filePath, fileName);
          }
        }
      }   
      catch (Exception e) {   
        e.printStackTrace();
      }
    }
  }
}
);


// this routine creates a file that lists all the H file in destination
public void listImageHeaderFiles(String destination, String outFilename)
{  

  String outPath = destination + outFilename;
  PrintWriter output;
  output = createWriter(outPath);
  output.println("/* " + outFilename + " bitmap header for GLCD library */");
  output.println("/* This file is created automatically by the glcdMakeBitmap utility */");
  output.println("/* Any edits to this file will be lost when glcdMakeBitmap is next run */");
  output.println();

  println("\nCreating header file " + outFilename + " that includes : ");
  File dir = new File(destination);
  if (dir.isDirectory()) {
    File[] files = dir.listFiles();
    for (int i = 0; i < files.length; i++) {
      if( files[i].isDirectory() == false) {        
        String name = files[i].getName();
        if(name.endsWith(".h") && ! name.equals(outFilename)) {        
          output.println("#include " + '\"' + name + '\"' );
          println(name);
        }
      }
    }
  }
  else
    println(destination + " is not a directory");

  output.flush(); // Write the remaining data
  output.close(); // Finish the file
}


// this routine creates a file that lists all the H file in destination
public void listImageHeaderFilesX(String destination, String outFileName)
{  

  File dir = new File(destination);
  if (dir.isDirectory()) {
    File[] files = dir.listFiles();
    for (int i = 0; i < files.length; i++) {
      File f = files[i];    
      if( f.isDirectory() == false) {        
        String name = f.getName();
        if(name.endsWith(".h"))        
          println("Name: " + name);
      }
    }
  }
  else
    println(destination + " is not a directory");
}



  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#ece9d8", "glcdMakeBitmap" });
  }
}
