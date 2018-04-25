#include <string.h>

size_t strlen(const char *str) {
	size_t length = 0;

	while(*str != '\0') {
		length++;
		str++;
	}
	return length;
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