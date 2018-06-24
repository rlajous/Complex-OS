#include <stdio.h>

#define INPUT_SHOW_DAV 1
#define INPUT_BOOK 2
#define INPUT_CANCEL 3
#define INPUT_CREATE 4
#define INPUT_DELETE 5
#define INPUT_EXIT 6

int main(void) {
	displayMenu();
	return 0;
}

void displayMenu(){
	int input = -1;
	while (input != INPUT_EXIT){
		printf("Seleccione una opción del menu:\n"
			"1-Mostrar asientos disponibles para una funcion"
			"2-Reservar un asiento en una funcion"
			"3-Cancelar una reserva de asientos en una funcion"
			"4-Crear nueva funcion"
			"5-Eliminar una funcion"
			"6-Salir");
		input = getnum();
		processInput(input);
	}
}

void processInput(int input){
	switch(input){
		case INPUT_SHOW_DAV:
			displayDAVMenu();
			break;
		case INPUT_BOOK:
			displayBookMenu();
			break;
		case INPUT_CANCEL:
			displayCancelMenu();
			break;
		case INPUT_CREATE:
			displayCreateMenu();
			break;
		case INPUT_DELETE:
			displayDeleteMenu();
			break;
	}
}

void getMovieID(){
	char[100] movieName;
	char[20] screeningTime
	printf("Ingrese el nombre de la pelicula deseada:\n");
	scanf("%s", movieName);
	printf("Ingrese el dia y la hora de la funcion deseada (YYYY-MM-DD HH:MM):\n");
	scanf("%s", screeningTime);

}


