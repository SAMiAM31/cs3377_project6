/Hector Garza
//Program 6

#include <iostream>
#include "cdk.h"
#include <stdint.h>
#include <fstream>
#include <string>
#include <sstream>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

const int maxRecordStringLength = 25;

class BinaryFileHeader
{
public:

  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;

};

class BinaryFileRecord
{
public:

  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};

int main ()
{

  WINDOW *window;
  CDKSCREEN *cdkscreen;
  CDKMATRIX *myMatrix;// CDK Screen Matrix
  
  BinaryFileHeader* theHeader = new BinaryFileHeader ();
  BinaryFileRecord* theRecord = new BinaryFileRecord ();


  const char *rowTitles[] = { "0", "a", "b", "c", "d", "e"};
  const char *columnTitles[] = { "0", "a", "b", "c"};
   int boxWidths[] =
    { BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH };
  int boxTypes[] = { vMIXED, vMIXED, vMIXED, vMIXED, vMIXED, vMIXED };

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr ();
  cdkscreen = initCDKScreen (window);

  /* Start CDK Colors */
  initCDKColor ();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
   */
  myMatrix =
    newCDKMatrix (cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH,
		  MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_NAME_STRING,
		  (char **) rowTitles, (char **) columnTitles, boxWidths,
		  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix == NULL)
    {
      printf ("Error creating Matrix\n");
      _exit (1);
    }

  /* Display the Matrix */
  drawCDKMatrix (myMatrix, true);

  /*
   * Dipslay a message
   */
   
  ifstream theFile("cs3377.bin", ios::in | ios::binary);
  if(!theFile.is_open())
    {
      cout<<"Not opening file!\n";
      exit(-1);
    }
  theFile.read ((char *) theHeader, sizeof (BinaryFileHeader));
   
  
  stringstream ss1;
  string s1;
  ss1 << hex << theHeader->magicNumber;
  s1 += ss1.str ();

  stringstream ss2;
  string s2;
  ss2 << theHeader->versionNumber;
  s2 += ss2.str();

  stringstream ss3;
  string s3;
  ss3 << theHeader->numRecords;
  s3 += ss3.str();

  setCDKMatrixCell (myMatrix, 1, 1, ("Magix:0x" + s1).c_str ());
  setCDKMatrixCell (myMatrix, 1, 2, ("Version: " +s2).c_str ());
  setCDKMatrixCell (myMatrix, 1, 3, ("NumRecords: " + s3).c_str ());
  drawCDKMatrix (myMatrix, true);

  int k = 2;

  while (theFile.read ((char *) theRecord, sizeof (BinaryFileRecord))
	 && k <= 5)
    {
      stringstream ss;
      stringstream ss1;
      string s;
      string s1;
      
      ss << (int)theRecord->strLength;
      s += ss.str ();
      
      ss1 << theRecord->stringBuffer;
      s1 += ss1.str ();


      setCDKMatrixCell (myMatrix, k, 1, ("strlen: " + s).c_str ());
      setCDKMatrixCell (myMatrix, k, 2, s1.c_str ());
      k++;
    }



  drawCDKMatrix (myMatrix, true);/* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;
  theFile.close ();
  // Cleanup screen
  endCDK ();
}
