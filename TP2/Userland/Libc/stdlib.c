#include <stdlib.h>


/*void * malloc(unsigned int size) {
	void * address;
	int80(8,(uint64_t)&address,size,0);
	return address;
}

void free(void *ptr) {

}*/

//Adapted from naiveterminal.c
int itoa(int value, char * buffer, int base) {
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	if(value < 0 && base == 10) {
		value = -value;
		*p++ = '-';
		digits++;
	}

	//Calculate characters for each digit
	do
	{
		int remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = *buffer == '-' ? buffer + 1: buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int ftoa(float value, char * buffer, int precision)
{
  char *p = buffer;
  char *p1, *p2;
  int number;
  int digits = 0;
  int decimals = precision;

  while(decimals--) {
    value *= 10;
  }

  number = (int) value;

  if(value < 0) {
    number = -number;
    *p++ = '-';
    digits++;
  }

	decimals = precision;

  //Calculate characters for each digit
  do
  {
    if((value < 0 ? digits-1:digits) == precision && precision != 0) {
      *p++ = '.';
      digits++;
    }

    int remainder = number % 10;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    digits++;
		number /= 10;
  }
  while (decimals-- > 0 || number != 0); 

  // Terminate string in buffer.
  *p = 0;

  //Reverse string in buffer.
  p1 = *buffer == '-' ? buffer + 1: buffer;
  p2 = p - 1;
  while (p1 < p2)
  {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return digits;
}

int isAlpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int isDigit(char c) {
	return (c >= '0' && c <= '9');
}
