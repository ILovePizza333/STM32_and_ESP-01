/* 
Плата: Blue Pill
Микроконтроллер: STM32F103C8T6
Язык программирования: Си + работа с регистрами + CMSIS

============ Входы цифровые ============

============ Выходы цифровые ============
PC13 синий светодиод, 0 = горит

=============== Служебные ===============
USART1: PA9 = TX1, PA10 = RX1
*/                       

#include "stm32f10x.h"
#include "main_init.c"
#include "main_functions.h" 
#include "String2.h"
#include "ESP.h"
// ================== Описание глобальных переменных =============
char Buffin[512];
int bf=0;
int flag=0;
char cut[64];
char mass_xxx[6];
// ========================= Массивы(команды) =========================
//u8 AT[4]="AT"; AT[2]='\r'; AT[3]='\n'; // проверка ответа
//char RST[7]="AT+RST"; //перезагрузка есп
//char CWMODE1[16]="AT+CWMODE_CUR=1"; //режим работы( klient)
//char CWMODE[16]="AT+CWMODE_CUR=2"; //режим работы( станция)
//char CWSAP[40]="AT+CWSAP_CUR=\"ESP8266\",\"1234567890\",5,3"; // свойства точки доступа
//char CIPAP[27]="AT+CIPAP_CUR=\"192.168.4.1\"";//set ip
//char CIPMODE[13]="AT+CIPMODE=0";  // сквозной режим передачи данных.
//char CIPMUX[12]="AT+CIPMUX=1"; // multiple connection.
//char CIPSERVER[18]="AT+CIPSERVER=1,80"; // запускаем ТСР-сервер на 80-ом порту
//char CIPSTO[12]="AT+CIPSTO=1";  // таймаут сервера 2 сек
//char CIFSR[9]="AT+CIFSR"; // узнаём адрес
//char CWLAP[9]="AT+CWLAP";// scan wifi
//char CWLIF[9]="AT+CWLIF";// connected wifi
//char PING[22]="AT+PING=\"192.168.4.2\"";//ping
// ============== Подпрограммы обработки прерывания ==============
// =================== USART1 ===================
// USART1->SR, бит RXNE сбрасывается автоматически при чтении USART1->DR,
// записывать в него ноль нужно только при мультибуферной коммуниуации.


void USART1_IRQHandler() {	
	Buffin[bf++]=USART1->DR;
	Tx2(Buffin[bf-1]);
}  

// =================== TIM2 ===================
void TIM2_IRQHandler() {
	if ((TIM2->SR & TIM_SR_UIF) != 0) { // Прерывание по переполнению
		TIM2->SR &= ~TIM_SR_UIF; 
		GPIOC->ODR ^= GPIO_ODR_ODR8; // Мигаем светодиодом на плате
	}
}

 


// =========================== MAIN =========================
int main(void) {
	main_init(); // Инициализация МК
	// ========================= GPIO off  =========================
	GPIOC->BSRR |= GPIO_BRR_BR13 ;  


	// ========================= Конфигурация esp =========================

	Tx2Strln("START");
	GPIOC->BRR |= GPIO_BRR_BR13 ;
	//ESP_println( &AT[0]);
	ESP_println("AT+RST"); //перезагрузка есп
	delay_ms(1000);
	clear_Buffer(&Buffin[0]);
	ESP_println("AT+CWMODE_CUR=2"); //режим работы 2 (точка доступа/станция)
	OK(&Buffin[0]);
	ESP_println("AT+CWSAP_CUR=\"ESP8266\",\"1234567890\",5,3"); // Имя, пароль, канал, тип шифрования (3-WPA2_PSK)
	OK(&Buffin[0]);
	ESP_println("AT+CIPAP_CUR=\"192.168.4.1\"");  //Устанавливаем IP-адрес точки доступа
	OK(&Buffin[0]);
	ESP_println("AT+CIPMODE=0"); // сквозной режим передачи данных.
	OK(&Buffin[0]);
	ESP_println("AT+CIPMUX=1"); // multiple connection.
	OK(&Buffin[0]);
	ESP_println("AT+CIPSERVER=1,80");  // запускаем ТСР-сервер на 80-ом порту
	OK(&Buffin[0]);
	ESP_println("AT+CIPSTO=1"); // таймаут сервера 1 сек
	OK(&Buffin[0]);
	GPIOC->BSRR |= GPIO_BRR_BR13 ;
	Tx2Strln("success config"); 

	// ========================= ГЛАВНЫЙ ЦИКЛ =========================
	while (1) {
		if (Buffin[0] !=0 && strlen2(&Buffin[0])>=20){ 
			int16_t n =-1;
			int ch_id=0;	
			n = pos(Buffin, "IPD");
			if (n >= 0){
				//int k=0;
				ch_id=Buffin[n+4]-'0' ;
				/*
				while(n<511){
					if(Buffin[n+14]==32 || Buffin[n+14]==0 || Buffin[n+14]=='\r'|| Buffin[n+14]=='\n' || k>=50){
						break;
					}
					cut[k]=Buffin[n+14];
					k++;
					n++;
					//Tx2Strln("h");
				}
				*/
				/*
				Tx2Strln(" ");
				int16ToStr(ch_id, &mass_xxx[0]);
				Tx2Str("hello");
				Tx2Strln(&mass_xxx[0]);
				*/
				otvet_klienty(ch_id, &Buffin[0]); 
			}

			//clear_Buffer(&cut[0]);
			clear_Buffer(&Buffin[0]);
		}
		delay_ms(10);
	}
} 



// КОНЕЦ