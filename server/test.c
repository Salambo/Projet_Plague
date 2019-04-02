void building_type_display(Building ** city){
    int length;
    int width;
    
    for(length=0; length<7; length++){
        for(width=0; width<7; width++){
            printf("%i ", city[length][width].type);
        }
        printf("\n");
    }
}



