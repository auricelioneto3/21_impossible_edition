#include "Clic.h"

void Clic_resetColor(){ 
    printf("\e[m"); 
}

void Clic_setFontColor(Color color){ 
    printf("\e[38;5;%dm", color); 
}

void Clic_setBackgroundColor(Color color) {
    printf("\e[48;5;%dm", color);
}


void Clic_move(int line, int column){ 
    printf("\e[%d;%dH", line, column);
}

void Clic_moveUp(int lines){ 
    printf("\e[%dA", lines);
}

void Clic_moveDown(int lines){
    printf("\e[%dB", lines);
}

void Clic_moveRight(int columns){
    printf("\e[%dC", columns);
}

void Clic_moveLeft(int columns){
    printf("\e[%dD", columns);
}

void Clic_moveDownBegin(int lines){
    printf("\e[%dE", lines);
}

void Clic_moveUpBegin(int lines){
    printf("\e[%dF", lines);
}

void Clic_moveToLine(int line) {
    int height = Clic_getScreenHeight();
    Clic_moveUp(height);
    Clic_moveDown(line);
}

void Clic_moveToColumn(int column){
    printf("\e[%dG", column);
}

void Clic_moveToBegin(){
    printf("\e[1G");
}


void Clic_clearLine(){
    printf("\e[2K");
}

void Clic_clearScreen(){
    printf("\e[2J\e[1;1H");
}

void Clic_breakLine(){
    printf("\n");
}


void Clic_printSymbol(char *symbol){
    printf("%s", symbol);
}

void Clic_printHLine(int width){
    for (int i = 0; i < width; i++) 
        Clic_printSymbol(Symbol_HLINE);
}

void Clic_printVLine(int height){
    for (int i = 0; i < height; i++) 
        printf(Symbol_VLINE "\e[1D\e[1B");
}

void Clic_printHBlockLine(int width){ 
    for (int i=0; i<width; i++) 
        printf(" ");
}

void Clic_printVBlockLine(int height){
    for (int i = 0; i < height; i++) 
        printf(" \e[1D\e[1B");
}

void Clic_printUpBorder(int width) {
    /** Desenha a linha horizontal superior â”â”â”â”“ */
	Clic_printSymbol(Symbol_CORNER_TL);
	Clic_printHLine(width - 2);
	Clic_printSymbol(Symbol_CORNER_TR);
}

void Clic_printVerticalBorder(int height) {
    /** Desenha uma linha verticai â”ƒ */
    for (int i = 0; i < height; i++) {
        printf(Symbol_VLINE);
        Clic_moveLeft(1);
        Clic_moveDown(1);
    }
}

void Clic_printBottomBorder(int width) {
    /** Desenha a linha horizontal inferiror â”—â”â”â”› */
    Clic_printSymbol(Symbol_CORNER_BL);
    Clic_printHLine(width - 2);
    Clic_printSymbol(Symbol_CORNER_BR);
}

void Clic_printBox(int width, int height) {
	if (width >= Clic_getScreenWidth())
		width = Clic_getScreenWidth();
    if (height >= Clic_getScreenHeight())
		height = Clic_getScreenHeight() - 1;

    Clic_saveCursorPosition();
	Clic_printUpBorder(width);
    Clic_moveLeft(1);
    Clic_moveDown(1);

    int i;
    for (i = 1; i <= height - 2; i++) {
        Clic_restoreCursorPosition();
        Clic_moveDown(i);
        printf(Symbol_VLINE);
        Clic_printHBlockLine(width - 2);
        printf(Symbol_VLINE);
    }

    Clic_restoreCursorPosition();
    Clic_moveDown(i);
    Clic_printBottomBorder(width);
}

int Clic_printCenter(char *text) {
    int width = 0, offset = 0;
    
    if (text) {
        width = Clic_getScreenWidth();
        offset = (width - strlen(text)) / 2;

        Clic_moveToColumn(offset);
        printf("%s", text);
    }

    return offset;
}

void Clic_printInBox(int boxWidth, char *text) {
    int length = strlen(text);

    Clic_saveCursorPosition();
    int lines = 0;
    for (int i = 0; text[i]; i++) {
        if ((i + 1) % boxWidth == 0) {
            lines++;
            Clic_restoreCursorPosition();
            Clic_moveDown(lines);
        }
        printf("%c", text[i]);
    }
}

void Clic_saveCursorPosition(){
    printf("\e7");
}

void Clic_restoreCursorPosition(){
    printf("\e8");
}

int Clic_getCursorRow() {
    struct termios orig_term, raw;
    char buf[32];
    unsigned int i = 0;

	int row, col;

    // Salva o estado original do terminal
    tcgetattr(STDIN_FILENO, &orig_term);
    raw = orig_term;

    // Modo "raw" (sem buffering nem eco)
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    // Envia sequÃªncia ANSI para pedir a posiÃ§Ã£o do cursor
    write(STDOUT_FILENO, "\033[6n", 4);

    // LÃª resposta do terminal: ESC [ linha ; coluna R
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, buf + i, 1) != 1)
            break;
        if (buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0';

    // Restaura o terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);

    // Analisa a resposta
    if (sscanf(buf, "\033[%d;%dR", &row, &col) != 2) {
        row = col = -1;
    }

	return row;
}

int Clic_getCursorColumn() {
    struct termios orig_term, raw;
    char buf[32];
    unsigned int i = 0;

	int row, col;

    // Salva o estado original do terminal
    tcgetattr(STDIN_FILENO, &orig_term);
    raw = orig_term;

    // Modo "raw" (sem buffering nem eco)
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    // Envia sequÃªncia ANSI para pedir a posiÃ§Ã£o do cursor
    write(STDOUT_FILENO, "\033[6n", 4);

    // LÃª resposta do terminal: ESC [ linha ; coluna R
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, buf + i, 1) != 1)
            break;
        if (buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0';

    // Restaura o terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);

    // Analisa a resposta
    if (sscanf(buf, "\033[%d;%dR", &row, &col) != 2) {
        row = col = -1;
    }

	return col;
}




int Clic_getScreenWidth() {
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) return 1;
	return w.ws_col;
}

int Clic_getScreenHeight() {
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) return 1;
	return w.ws_row;
}

int Clic_getScreenCenterLine() {
    int centerLine = (Clic_getScreenHeight() - 1) / 2;
    return centerLine;
}

int Clic_getScreenCenterColumn() {
    int centerColumn = Clic_getScreenWidth() / 2;
    return centerColumn;
}





void Clic_setBufferOn() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON | ECHO;  // Ativa entrada canÃ´nica e eco
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void Clic_setBufferOff() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);  // Desativa entrada canÃ´nica e eco
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void Clic_fixDraw() {
	Clic_move(Clic_getScreenHeight() - 1, Clic_getScreenWidth());
	Clic_breakLine();
}

int Clic_keyCapture() {
    int keyPressed = 0;
    int codeKey = 0;
    
    // Limpa o buffer de entrada antes de capturar
    tcflush(STDIN_FILENO, TCIFLUSH);
    
    Clic_setBufferOff();

    keyPressed = getchar();

    if (keyPressed == '\e') {  // Se a tecla for uma sequencia de escape
        getchar(); // Ignora o caractere '['
        codeKey = getchar();
    }
    else
        codeKey = keyPressed;

    Clic_setBufferOn();

    return codeKey;
}

void Clic_pause(float seconds) {
    fflush(stdout);
    int microSeconds = seconds * 1000000;
    usleep(microSeconds);
}

int Format_extractWidth(char *format) {
    int width = 0;
    char *p = format;

    while (*p != '\0') {
        if (isdigit(*p)) {
            width = atoi(p);
            break;
        }
        p++;
    }

    return width;
}

int Format_isTypeSpecifier(char c) {
    if (strchr("diuoxXfFeEgGaAcspn%", c) != NULL) {
        return 1;
    }
    return 0;
}

Format *Format_formatSplit(char *sourceString) {
    
    char *p = sourceString;
    char *startOfFormat = NULL;
	
    int count = 1;
    Format *formats = calloc(Row_MAX_CELLS, sizeof(Format));

    formats[0] = (Format){"%4d", false, true, 4};

    while (*p != '\0') {
        if (*p == '%' && startOfFormat == NULL) {
            startOfFormat = p;
        }
        else if (startOfFormat != NULL) {

            if (Format_isTypeSpecifier(*p)) {
                int len = (p - startOfFormat) + 1;

                if (count < Row_MAX_CELLS && len <= 30) {
                    strncpy(formats[count].formatString, startOfFormat, len);

                    formats[count].formatString[len] = '\0';

                    if (strchr(formats[count].formatString, '-'))
                        formats[count].isLeftAligned = true;
                    else
                        formats[count].isLeftAligned = false;

                    count++;
                }

                if (startOfFormat[1] == '%') {
                    p++;
                }
                
                startOfFormat = NULL;
            } 
            else if (isdigit(*p)) {
                formats[count].isCustomWidth = true;
                formats[count].width = Format_extractWidth(startOfFormat);
            }
            else if (!isdigit(*p) && strchr("-+ #.*hlLzjt", *p) == NULL) {
                startOfFormat = NULL;
            }
        }

        if (*p == '\0') {
            break;
        }
        
        p++;
    }

	return formats;
}

Table *Table_create(char *format) {
	int width = Clic_getScreenWidth();
	
	if (format == NULL) {
		perror("Error: Table_create: format string is null.\n");
		return NULL;
	}

    Table *table = (Table *) malloc(sizeof(Table));

    if (!table) {
        perror("Error: Table_create: Unable to allocate memory for Table.\n");
        return NULL;
    }

    table->highlightedIndex = 1;
    table->highlightColor = Color_YELLOW;
    
    table->formats = Format_formatSplit(format);
    
    int countFormats = 0;
    while (table->formats[countFormats].formatString[0] != '\0') {
        countFormats++;
    }
    
    table->nCols = countFormats;
    table->nRows = 0;
    table->width = width;
    table->defaultCellWidth = (width - 2) / table->nCols;

    table->firstRow = NULL;
    table->lastRow = NULL;
	
	return table;
}

void Table_setWidth(Table *table, int width) {
    if (table == NULL)
        return;
    if (width >= Clic_getScreenWidth())
		width = Clic_getScreenWidth();
    if (width < 3)
        width = 3;
        
    table->width = width;
    table->defaultCellWidth = (width - 2) / table->nCols;
}

int Format_getTotalCustomWidth(Table *table) {
    if (!table) return 0;

    int totalWidth = 0;

    for (int col = 0; col < table->nCols; col++) {
        if (table->formats[col].isCustomWidth) {
            totalWidth += table->formats[col].width;
        }
    }

    return totalWidth;
}

int Format_getCustomCellCount(Table *table) {
    if (!table) return 0;

    int count = 0;

    for (int col = 0; col < table->nCols; col++) {
        if (table->formats[col].isCustomWidth) {
            count++;
        }
    }

    return count;
}

/* 
numColsTerminal =     numColsTable + 2
numRowsTerminal = 2 * numRowsTable + 1

1+------------------+     +------------------+   
2|12345678 112345678|     |                  |     12345678 112345678   
3|------------------|     |                  |     ------------------   
4|12345678 112345678| --> |                  |  +  12345678 112345678   
5|------------------|     |                  |     ------------------
6|12345678 112345678|     |                  |     12345678 112345678
7+------------------+     +------------------+
*/

void Table_adjustVerticalSpace(Table *table) {
    if (!table) return;

    int height          = Clic_getScreenHeight();
    int currenLine      = Clic_getCursorRow();
    int availableHeight = height - (currenLine - 1);
    int requiredHeight  = 2 * (table->nRows) + 1;

    if (requiredHeight > availableHeight) {
        
    }
}

void Table_printHighlight(Table *table, bool highlight) {
	if (!table) return;

    int width            = table->width;
    int availableWidth   = width - (2 + table->nCols);
    int totalCustomWidth = Format_getTotalCustomWidth(table);
    int customCellCount  = Format_getCustomCellCount(table);
    int defaultCellCount = table->nCols - customCellCount;
    int remainingWidth   = availableWidth - totalCustomWidth;
    int defaultCellWidth = remainingWidth / defaultCellCount;
    int extraWidth       = remainingWidth - (defaultCellWidth * defaultCellCount);

    Row *currentRow = table->firstRow;
    char contentBuffer[300];

    Clic_saveCursorPosition();
    Clic_printUpBorder(width);
    
    int linesDown = 1;
    Clic_restoreCursorPosition();
    Clic_moveDown(linesDown);

    while (currentRow != NULL) {
        Clic_printSymbol(Symbol_VLINE);

        for (int col = 0; col < table->nCols; col++) {
            Cell cell = currentRow->cell[col];

            int cellWidth = cell.format.isCustomWidth ? cell.format.width : defaultCellWidth;

            if (col == table->nCols - 1 && extraWidth > 0) {
                cellWidth += extraWidth;
            }

            if (highlight && table->highlightedIndex == currentRow->index) {
                Clic_setBackgroundColor(table->highlightColor);
                Clic_setFontColor(Color_BLACK);
            }

            if (strlen(cell.content) > cellWidth) {
                strncpy(contentBuffer, cell.content, cellWidth - 1);
                contentBuffer[cellWidth - 1] = '\0';
                strcat(contentBuffer, Symbol_TREE_POINTS);
            }
            else {
                strcpy(contentBuffer, cell.content);
            }

            if (cell.format.isLeftAligned) {
                printf("%-*s", cellWidth, contentBuffer);
            } 
            else {
                printf("%*s", cellWidth, contentBuffer);
            }

            printf(" ");

            Clic_resetColor();
        }

        Clic_printSymbol(Symbol_VLINE);
        Clic_restoreCursorPosition();
        Clic_moveDown(++linesDown);

        currentRow = currentRow->next;

        if (currentRow) {
            Clic_printSymbol(Symbol_VLINE);
            Clic_printHLine(width - 2);
            Clic_printSymbol(Symbol_VLINE);
            Clic_restoreCursorPosition();
            Clic_moveDown(++linesDown);
        }

    }

    Clic_printBottomBorder(width);

    Clic_restoreCursorPosition();
}

void Table_print(Table *table) {
    Table_printHighlight(table, false);
}

Row *Table_getRow(Table *table, int index) {
    Row *row = NULL;
    if (table && index >= 0) {
        row = table->firstRow;
        while (row) {
            if (row->index == index)
                return row;
            row = row->next;
        }
    }
    return NULL;
}

void Table_deleteRow(Table *table, int index) {
    Row *row = NULL;
    if (table && index > 0) {
        row = table->firstRow;
        while (row) {
            if (row->index == index) {
                if (row->prev == NULL) {
                    table->firstRow = row->next;
                    
                    if (table->nRows == 1)
                        table->lastRow = NULL;
                    else
                        table->firstRow->prev = NULL;
                }
                else {
                    row->prev->next = row->next;
                    if (row->next)
                        row->next->prev = row->prev;
                }

                table->nRows--;
                free(row);
                break;
            }
            row = row->next;
        }
    }
}

int Table_select(Table *table) {
    Row *row = NULL, *prev = NULL, *next = NULL;

    int selectedIndex = 0;

	if (!table) return -1;

    int key = 0;
    while (true) {
        Table_printHighlight(table, true);

        key = Clic_keyCapture();

        row = Table_getRow(table, table->highlightedIndex);
        if (row == NULL)
            return 0;

        prev = row->prev;
        next = row->next;

        switch (key) {
            case Key_UP:   
                if(prev) 
                    table->highlightedIndex = prev->index; 
                break;
            case Key_DOWN: 
                if(next) 
                    table->highlightedIndex = next->index;
                break;
            case Key_ENTER:
                selectedIndex = table->highlightedIndex;
                return selectedIndex;
            case Key_DELETE:
                Clic_saveCursorPosition();
                for (int i = 0; i < 2 * table->nRows + 1; i++) {
                    Clic_moveDown(i);
                    Clic_printHBlockLine(table->width);
                    Clic_restoreCursorPosition();
                }

                Table_deleteRow(table, table->highlightedIndex);

                selectedIndex = table->highlightedIndex;
                if(prev) 
                    table->highlightedIndex = prev->index;
                else if (next)
                    table->highlightedIndex = next->index;
                else
                    table->highlightedIndex = 1;

                Table_printHighlight(table, true);

                return -1 * selectedIndex;
        }
    }

    return selectedIndex;
}

void Table_free(Table *table) {
	if (!table) return;

	Row *currentRow = table->firstRow;
    Row *nextRow;

    while (currentRow != NULL) {
        nextRow = currentRow->next;
        free(currentRow);
        currentRow = nextRow;
    }

	free(table);
}

int countDigits(int number) {
    int digits = 1;
    while (number) {
        number = number / 10;
        digits++;
    }
    return digits;
}

Row *Row_create(Table *table, va_list args) {
    if (table == NULL) {
        perror("Error: Row_create: table is NULL.\n");
        return NULL;
    }

    Row *newRow = (Row *) malloc(sizeof(Row));

    if (!newRow) {
        perror("Error: Row_create: Unable to allocate memory for new Row.\n");
        return NULL;
    }

    char *formatString;
    char *buffer;

    for (int col = 0; col < table->nCols; col++) {
        formatString = table->formats[col].formatString;
        buffer = malloc(300 * sizeof(char));

        if (col == 0) {
            snprintf(buffer, 300, formatString, table->nRows + 1);
        }
        else if (strchr(formatString, 's')) {
            char *strArg = va_arg(args, char *);
            snprintf(buffer, 300, formatString, strArg);
        } 
        else if (strchr(formatString, 'd') || strchr(formatString, 'i')) {
            int intArg = va_arg(args, int);
            snprintf(buffer, 300, formatString, intArg);
        } 
        else if (strchr(formatString, 'f') || strchr(formatString, 'F')) {
            double doubleArg = va_arg(args, double);
            snprintf(buffer, 300, formatString, doubleArg);
        } 
        else {
            snprintf(buffer, 300, "N/A");
        }

        newRow->cell[col].content = buffer;
        newRow->cell[col].format = table->formats[col];
    }

    va_end(args);

    return newRow;
}

void Table_addRow(Table *table, ...) {
    if (table == NULL) {
        perror("Error: Table_addRow: table is NULL.\n");
        return;
    }
    
    va_list args;
    va_start(args, table);

    Row *newRow = Row_create(table, args);

    if (!newRow) {
        perror("Error: Table_addRow: Unable to allocate memory for new Row.\n");
        return;
    }

    newRow->index = table->nRows + 1;
    newRow->length = table->nCols;
    newRow->next = NULL;
    newRow->prev = table->lastRow;

    if (table->lastRow) {
        table->lastRow->next = newRow;
    } 
    else {
        table->firstRow = newRow;
    }

    table->lastRow = newRow;
    table->nRows++;
}