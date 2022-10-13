#include "stm32f10x.h"
#include "main_functions.h" 
#include "String2.h"

extern int flag;
extern int bf;
void clear_Buffer(char *Buff){
	for(int i=0;i<511;i++){
		Buff[i] = 0;	
	}
	bf=0;
}

void OK(char *Buff){
	int16_t n =-1;
	while( n==-1){	
		n = pos(Buff, "OK");
		delay_ms(10);
	}
	clear_Buffer(Buff);
}


void otvet_klienty(int ch_id, char *Buff) { 
	char mass_int[6];
	char send[22]="AT+CIPSEND=";
	char Content[20];
	int lc=0;
	

	if(pos(Buff, "/ledon")>=0){
		GPIOC->BRR |= GPIO_BRR_BR13 ;
		flag=1;
		mass_new(Content, "ledon");
	}
	if(pos(Buff, "/ledoff")>=0){
		GPIOC->BSRR |= GPIO_BRR_BR13 ;
		flag=0;
		mass_new(Content, "ledoff");
	}
	
	lc = strlen2(Content);

	//"AT+CIPSEND=0,82" - ответ клиенту: ат-команда, id-клиента, , , длина пакета
	
	int16ToStr(ch_id, &mass_int[0]);
	mass_add(send, mass_int);
	mass_add(send, ",");
	
	int16ToStr(80+lc, &mass_int[0]);
	mass_add(send, mass_int);
	ESP_print(send);
	Tx1('\r'); Tx1('\n');
	delay_ms(50);


	int yes=cmp(Buff, '>' );
	if(yes) {
		// отправка заголовка
		ESP_print("HTTP/1.1 200 OK");
		Tx1('\r'); Tx1('\n');
		ESP_print("Content-Type: text/html");
		Tx1('\r'); Tx1('\n');
		ESP_print("Connection: close");
		ESP_print("Content-Length: ");
		int16ToStr(lc, &mass_int[0]);
		ESP_print(mass_int);
		Tx1('\r'); Tx1('\n');
		Tx1('\r'); Tx1('\n');		

		ESP_print(Content); // отправка содержимого пакета
		delay_ms(10);
	}

} 
