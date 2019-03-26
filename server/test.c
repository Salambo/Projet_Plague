#include "src/server.h"

int main(void) {
    City* city;

    city = (City*)malloc(sizeof(City));

    generate_citizens(city, 2);

    return EXIT_SUCCESS;
}