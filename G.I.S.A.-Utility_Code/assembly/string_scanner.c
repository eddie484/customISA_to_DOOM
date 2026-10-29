int string_scanner(char * data_place) {
    int i = 0;
    while(1) {
        char input = scan_uart();
        if (input == 13) {
            data_place[i] = 0;
            return 0;
        } else if (input == 8 || input == 127) {
            if (i != 0) {
                i -= 1;
                data_place[i] = 0;
            }
        } else {
            data_place[i] = input;
            i += 1;
        }
    }
    
    return 1;
}