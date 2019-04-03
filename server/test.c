#include "src/server.h"

int main(void) {
    City* city;

    city = (City*)malloc(sizeof(City));

    generate_citizens(city);
	printf("terminé");

    return EXIT_SUCCESS;
}

