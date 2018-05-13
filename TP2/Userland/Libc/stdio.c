#include <stdio.h>

void write(int fd, char* buffer, int size) {
	int80(1,fd,(uint64_t)buffer,size,0);
}

void read(int fd, char* buffer, int size) {
	int80(0,fd,(uint64_t)buffer,size,0);
}

void putchar(char c) {
	write(1, &c, 1);
}

void putcharatpos(char ch, int x, int y){
	int80(22, ch, x, y,0);
}

char getchar() {
	char ch;
	do
		read(0, &ch, 1);
	while(ch == '\0');
	return ch;
}

int printf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int num, length, precision;
 	int precFlag = 0;
 	double fractionary;
 	char number[30];
	char ch;
	char* string;

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			if((ch = *(format++)) == '.') {
        format += parseInt(format, &precision);
        precFlag = 1;
        ch = *(format++);
      }
      switch(ch) {
				case 'd':
					num = va_arg(args, int);
					length = itoa(num, number, 10);
					if(precFlag == 1 && length < precision) {
            int diff = precision - length;
            while(diff--)
              putchar('0');
          }
          write(1, number, precFlag == 1 && length > precision ? precision : length);
          break;
        case 'f':
          fractionary = va_arg(args, double);
          length = ftoa(fractionary, number, precFlag == 1? precision : 6);
          write(1, number, length);
					break;
				case 'c':
					putchar((char)va_arg(args, int));
					break;
				case 's':
					string = (char*)va_arg(args,char*);
					write(1,string,strlen(string));
					break;
				default:
				 putchar(ch);
				 break;
			}
			precFlag = 0;
		}
		else
			putchar(ch);
	}
	return 0;
}

int scanf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	int read = 0;
	int index = 0;
	char ch;
	char buffer[BUFFER_SIZE];
  char* character;

  readLine(buffer,BUFFER_SIZE);

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			switch(ch = *(format++)) {
				case 'd':
          index += parseInt(&buffer[index], va_arg(args,int*));
					read++;
					break;
				case 'f':
	        index += parseDouble(&buffer[index], va_arg(args,double*));
	        read++;
	        break;
				case 'c':
        	character = va_arg(args, char*);
          *character = buffer[index++];
          read++;
          break;
				case 's':
          character = va_arg(args,char*);
          char aux;
          while( (aux = buffer[index++]) != '\0')
          	*(character++) = aux;
          *character = '\0';
          read++;
          break;
			}
		}
		if(ch != buffer[index++])
			return read;
  }
	return read;
}

int sscanf(const char* format, const char* str, ...) {
	va_list args;
	va_start(args, str);
	int read = 0;
	int index;
	char ch;
  char* character;

	while((ch = *(format++)) != '\0') {
		if(ch == '%') {
			switch(ch = *(format++)) {
				case 'd':
        	index = parseInt(str, va_arg(args,int*));
          str += index;
					read++;
					break;
				case 'f':
          index = parseDouble(str, va_arg(args,double*));
          str += index;
          read++;
          break;
				case 'c':
          character = va_arg(args, char*);
          *character = *(str++);
          read++;
          break;
				case 's':
          character = va_arg(args,char*);
          char aux;
          while( (aux = *(str++)) != '\0')
          	*(character++) = aux;
          *character = '\0';
          read++;
          break;
			}
		}
		if(ch != *(str++))
			return read;
  }
	return read;
}


int parseInt(const char* string, int * value) {
  *value = 0;
	int sign= 1;
	int digits = 0;
	char c;

  if(*string == '-') {
		if (isDigit(*(string + 1))) {
			sign = -1;
			digits++;
			string++;
		}
		else
			return 0;
	}

  while (isDigit(c = *string)){
		*value = (*value)*10+(c - '0');
    digits++;
    string++;
  }

  *value *= sign;

  return digits;
}

int parseDouble(const char * string, double * value)
{
  *value = 0;
  int sign = 1;
  int dotFlag = 0;
  double decimalPlace = 10;
  int digits = 0;
  char c;

  if(*string == '-') {
    if (isDigit(*(string + 1))) {
      sign = -1;
      digits++;
      string++;
    }
    else
      return 0;
  }

  while (isDigit(c = *string) || (c == '.' && !dotFlag)) {
    if(c != '.' || dotFlag) {
      if(!dotFlag)
        *value = (*value)*10 + (c - '0');
      else {
        *value = *value + (c - '0')/decimalPlace;
        decimalPlace *= 10;
      }
      digits++;
      string++;
    }
    else {
      dotFlag = 1;
      digits++;
      string++;
    }
  }

  *value *= sign;

  return digits;

}

int readLine(char * buffer, int maxSize) {
	int length = 0;
	char c;

	while((c = getchar()) != '\n' && length < maxSize) {
		*(buffer++) = c;
		length++;
	}

	*buffer = '\0';
	return length;
}
