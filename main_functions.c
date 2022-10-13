#include "stm32f10x.h"
#include "String2.h"

#define    DWT_CYCCNT    *(volatile uint32_t *)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t *)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t *)0xE000EDFC
#define CRLF 0x1F
// Функции для генерирования программной задержки с использованием DWT
// Не забывайте, что разные прерывания могу мешать точному отсчёту времени

// Функция задержки в мкс (можно от 1 мкс). Получите: задержка + 1 мкс.
void delay_us(uint32_t us) {
	static volatile uint32_t n;
	n =  us * 72; // SystemCoreClock/1000000 = 72
	DWT_CYCCNT = 0; while (DWT_CYCCNT < n) ;
}

// Функция задержки в мс от 1 мс до 59 сек. (дальше переполнение)
void delay_ms(uint32_t ms) {
	delay_us(ms*1000);
}

// Функция преобразования знакового 16 битного числа в строку
void int16ToStr(int Number, char *pStr) {
	clear_mass(pStr);
	//pStr[0] ='-';
	//static int8_t i;
	if(Number<10){pStr[0] =Number+ '0';pStr[1] = 0;pStr[2] = 0;pStr[3] = 0;pStr[4] = 0;}
	else if(Number<100){pStr[0] =Number/10+ '0';pStr[1] = Number%10+ '0';pStr[2] = 0;pStr[3] = 0;pStr[4] = 0;}
	else if(Number<1000){pStr[0] =Number/100+ '0';pStr[1] = Number/10%10+ '0';pStr[2] = Number%100+ '0';pStr[3] = 0;pStr[4] = 0;}
	pStr[5] = 0; // конец строки
	//Tx2('\r'); Tx2('B'); Tx2Buf( pStr ); Tx2('\r');
}

// Функция преобразования беззнакового 16 битного числа в строку
void Uint16ToStr(uint16_t Number, char *pStr) {
	static int8_t i;
	for(i = 4; i != -1; i--) { pStr[i] = (Number % 10) + 0x30; Number /= 10; }
	pStr[5] = 0; // конец строки
}
/*
// Функция преобразования беззнакового 32 битного числа в строку
// 0 .. 4 294 967 295  <CR> = 13, <LF> = 10
void Uint32ToStr(uint32_t Number, uint8_t *pStr, uint8_t ChEnd ) {
	static int8_t i;
	for(i = 9; i != -1; i--) { pStr[i] = (Number % 10) + 0x30; Number /= 10; }
	if (ChEnd == CRLF) { pStr[10] = 13; pStr[11] = 10; pStr[12] = 0; } 
	  else pStr[10] = ChEnd;
}
*/

// ========================= USART1 =========================

// Функция передачи символа по USART1 по готовности
void Tx1(char Symbol) {
	while ((USART1->SR & USART_SR_TXE) == 0) {};
	USART1->DR = Symbol;
}

// Функция ожидания и приёма символа по USART1
char Rx1() {
	while ((USART1->SR & USART_SR_RXNE) == 0) {};
	return USART1->DR;
}

/*
// Функция передачи массива символов (строки) по USART1
// пока не встретится "0" байт
void Tx1Str(char *pStr) {
	static uint8_t i;
	i = 0; do Tx1( pStr[i++] ); while (pStr[i] != 0);
}


// Функция передачи массива символов (строки) по USART1 пока не встретится "0" байт
void Tx1Buf(char *pStr) {
	static uint8_t i;
	int len= 0;    while (pStr[len]) len++;
	i = 0; do Tx1( pStr[i++] ); while (i<len);
}

*/



// ========================= USART2 =========================

// Функция передачи символа по USART2 по готовности
void Tx2(char Symbol) {
	while ((USART2->SR & USART_SR_TXE) == 0) {};
	USART2->DR = Symbol;
}

// Функция ожидания и приёма символа по USART2
char Rx2() {
	while ((USART2->SR & USART_SR_RXNE) == 0) {};
	return USART2->DR;
}

// Функция передачи массива символов (строки) по USART2
// пока не встретится "0" байт
void Tx2Str(char *pStr) {
	static uint8_t i;
	i = 0; do Tx2( pStr[i++] ); while (pStr[i] != 0);
}

// Функция передачи массива символов (строки) по USART2
// пока не встретится "0" байт
void Tx2Strln(char *pStr) {
	static uint8_t i;
	i = 0; do Tx2( pStr[i++] ); while (pStr[i] != 0);
	Tx2('\r');
	Tx2('\n');
}

// Функция передачи массива символов (строки) по USART1
// пока не встретится "0" байт
void Tx2Buf(char *pStr) {
	static uint8_t i;
	int len= 0;    while (pStr[len]) len++;
	i = 0; do Tx2( pStr[i++] ); while (i<len);
}


// ========================= ESP =========================
// Функция передачи массива символов (строки) по USART1 пока не встретится "0" байт
// +задержка после отправки сообщения
void ESP_print(char *pStr) {
	static uint8_t i;
	i = 0; do Tx1( pStr[i++] ); while (pStr[i] != 0);
}
void ESP_println(char *pStr) {
	static uint8_t i;
	//Tx2Strln( pStr);
	i = 0; do Tx1( pStr[i++] ); while (pStr[i] != 0);
	Tx1('\r');
	Tx1('\n');
}
/*
void ESP_print_int(uint8_t *pStr) {
	static uint8_t i;
	i = 0; do Tx1( pStr[i++] ); while (pStr[i] != 0);
}
*/
// КОНЕЦ
