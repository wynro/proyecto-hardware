/* Random number generator demo program

   Calls assembler function 'randomnumber' defined in random.s
*/

//#include <stdio.h>

/* this function prototype is needed because 'randomnumber' is external */
extern unsigned int randomnumber( void );

int main()
{
  int i;
  int nTemp;
  unsigned int random[10];

  for( i = 0; i < 10; i++ )
  { 
  	nTemp = randomnumber();
  	random[i] = nTemp;
  }

  return( 0 );
}
