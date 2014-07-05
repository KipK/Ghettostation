This directory contains a utility that will convert bitmap image files to 
a header file that can be used in an Arduino sketch with the glcd library.

The utility is written in the Processing language; the glcdMakeBitmap.pde file runs
in the Processing environment on your computer - its not an Arduino sketch.
See: http://processing.org/

Also included are a java runtime (glcdMakeBitmap.jar) and java source (glcdMakeBitmap.java)

Run the utility by loading the pde into Processing (or click the jar file in the Java directory) 
and drop the image file to be converted into the window.
If the image can be converted, a header file is created in the bitmap directory.
For example, if the image file name to be converted is named image.bmp, the header is named image.h

The utility can also update the aggregate header file (named allBitmaps.h).
Including allBitmaps.h in your sketch makes all images in the bitmaps directory available.
Flash memory is only consumed for images that are explicitly used by your code so there
is no penalty using this file instead of explicitly including header files.

To update allBitmaps.h to include all header files in the bitmap directory, click the window.

See the glcd documentation for more informaton on using bitmaps.

----
The following are instructions for updating the .jar and .java files in the java directory
if the Processing sketch is modified.

From the Processing IDE, select File > Export
This creates a directory below glcdMakeBitmap that will contain the .jar and .java files 
These should be moved to the GLCD\bitmap\utlis\java directory to ensure the
correct relative path to the directory containing the bitmap headers.

After the .jar and .java files  are moved, the applet directory can be deleted.