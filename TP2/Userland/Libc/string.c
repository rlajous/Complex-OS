#include <string.h>

size_t strlen(const char *str) {
	size_t length = 0;

	while(*str != '\0') {
		length++;
		str++;
	}
	return length;
}

char *strcpy(char *dest, const char *src) {
	char * current = dest;

	while(*src != '\0') {
		*current = *src;
		current++;
		src++;
	}

	*current = '\0';
	return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
	char * current = dest;

	while(*src != '\0' && n > 0) {
		*current = *src;
		current++;
		src++;
		n--;
	}

	while(n > 0) {
		*current = '\0';
		current++;
		n--;
	}

	*current = '\0';

	return dest;
}

int strcmp(const char *str1, const char *str2) {
	while(*str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

void toUpperCase(char *str) {
	while(*str != '\0') {
		if (*str >= 'a' && *str <= 'z') {
			*str += 'A' - 'a';
		}
		str++;
	}
}