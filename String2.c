//delete[] dinamich_array;  // удаляем динамический массив
#include "stm32f10x.h"
#include "main_functions.h" 
/////возвращает длину строки///////////////////
int strlen2(char* p) {
    int i = 0;
    while (*p++) i++;
    return i;//отлажено
}

void clear_mass(char *Buff){
	int l=strlen2(&Buff[0]);
	for(int i=0;i<l;i++){
		Buff[i] = 0;	
	}
}

void mass_add(char *Buffer, char *Buff){
	int l=strlen2(&Buffer[0]);
	int k=strlen2(&Buff[0]);
	int j=0;
	for(int i=l;i<l+k;i++){
		Buffer[i] = Buff[j];
		j++;		
	}
}

void mass_new(char *Buffer, char *Buff){
	int l=strlen2(&Buffer[0]);
	for(int i=0;i<l;i++){
		Buffer[i] = 0;		
	}
	int k=strlen2(&Buff[0]);
	for(int i=0;i<k;i++){
		Buffer[i] = Buff[i];		
	}
}


/////ищет в стороке символ ///////////////////
int cmp(char* str1, char p){
	int i =0;
	int n=strlen2(str1);
	    while (i<n){
        if (str1[i++] == p){
           return 1;
		}
    }
    return 0;
}



// Функция для поиска подстроки в строке
// + поиск позиции, с которой начинается подстрока
int pos(char *s, char *c){
	//Tx2('\r');
	int i, j; // Счетчики для циклов
	int lenC=strlen2(c);  int lenS=strlen2(s); // Длины строк
	 
	//Tx2('\r'); Tx2('B'); Tx2Buf( s ); Tx2('\r');// Tx2('I'); Tx2Str( c );  Tx2('\r');
	//char mass2[6]; Tx2('\r');   Tx2('S'); Uint16ToStr(lenS, &mass2[0]); Tx2Str( &mass2[0] );   Tx2('\r'); //  Tx2('C'); Uint16ToStr(lenC, &mass2[0]); Tx2Str( &mass2[0] );   Tx2('\r');
	 
	//x2('M');Tx2('\r');
	if(lenS !=0){
		for (i = 0; i <= lenS - lenC; i++){ // Пока есть возможность поиска
			// Tx2(s[i]);
			 
			for (j = 0; j<=lenC; j++){ // Проверяем совпадение посимвольно Если посимвольно совпадает по длине искомого Вернем из функции номер ячейки, откуда начинается совпадение Учитывать 0-терминатор  ( '\0' )
				if (s[i + j] != c[j]){
					 break;
				}
			}
			if (j == lenC){return i;}
		}
	}
	//Tx2('K');Tx2('\r');
	//Иначе вернем -1 как результат отсутствия подстроки
	return -1;

}

/*
/////сравнивает две стороки одинаковой длины ///////////////////
int mstrcmp(const char* str1, const char* str2){
    while (*str1 && *str2){
        if (*str1 != *str2){
            if (*str1 > *str2)
                return 1;
            else
                return -1;
        }
        *str1++;
        *str2++;
    }
    return 0;
}
*/


/*
/////соединение двух строк///////////////////
char* strstr2(char* a, char* b) {
    int ia = strlen2(a);
    int ib = strlen2(b);
    int n = strlen2(a) + strlen2(b);

    char* dinamich_array = new char[n];    // создаем динамический массив
    for (int i = 0; i <= n; i++) {
        dinamich_array[i] = 0;  // считываем числа в ячейки массива
    }
    for (int i = 0; i <= ia; i++) {
        dinamich_array[i] = a[i];  // считываем числа в ячейки массива
    }
    for (int i = ia; i < n; i++) {
        dinamich_array[i] = b[i - ia];  // считываем числа в ячейки массива
    }
       
    return dinamich_array;
}

/////создание строки///////////////////
char* str2(char* a) {
    int ia = strlen2(a);

    int n = strlen2(a);

    char* dinamich_array = new char[n];    // создаем динамический массив
    for (int i = 0; i <= ia; i++) {
        dinamich_array[i] = a[i];  // считываем числа в ячейки массива
    }
    return dinamich_array;
}
*/
