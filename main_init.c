#define    DWT_CYCCNT    *(volatile uint32_t *)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t *)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t *)0xE000EDFC
#define CRLF 0x1F



// ========= ������������� ���������������� =========
static __INLINE void main_init() {
// ����� ����������� ��������, 2 ����� �������� ��� FLASH, ��. PM0075
FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;
RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // HCLK = SYSCLK
RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // PCLK1 = SYSCLK / 2 (APB1)
RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // PCLK2 = SYSCLK (APB2)
RCC->CFGR |= RCC_CFGR_SW_PLL; // �������� ������ �� PLL
RCC->CFGR |= RCC_CFGR_PLLMULL9; // SYSCLK = 72 MHz
RCC->CFGR |= RCC_CFGR_PLLSRC_HSE; // ������������ PLL �� ������
RCC->CR |= RCC_CR_HSEON; //��������� ��������� HSE
while (!(RCC->CR & RCC_CR_HSERDY)) {};
RCC->CR |= RCC_CR_PLLON; // ��������� PLL
while ((RCC->CR & RCC_CR_PLLRDY) == 0) {};
// ������� ����� PLL ������ ���������� ������������
while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) {};
//RCC->CR &= ~RCC_CR_HSION; // ��������� ���������� RC-���������

//====================== DWT ======================
SCB_DEMCR |= 0x01000000; // ��������� ������������ �������
DWT_CYCCNT = 0; // �������� �������� �������� ��������
DWT_CONTROL |= 1; // ��������� �������  

//====================== ����� ======================  
RCC->APB2ENR = RCC_APB2ENR_IOPCEN; // �������� ������������ ����� GPIOC
GPIOC->CRH = GPIO_CRH_MODE13 | GPIO_CRH_MODE8; // ��������� ������ PC13 ��� ������

//====================== USART1 ====================== 
// �������� ������������ ����� GPIOA � ����� �������������� �������
RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN); 
RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // �������� ������������ USART1
// PA9 (TX1) AFIO Push-Pull, 10MHz. PA10 (RX1) HiZ, 10MHz
// ������� ������������� ���� ��� � "00", ��� ������ ����������� !
GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9 | GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
// ����� ������ ���� ������������� � '1'
GPIOA->CRH |= (GPIO_CRH_MODE9_0 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_0); 
USART1->BRR = 625; // PCLK2 / Baud = 72000000 / 9600 ���(7500)  //115200 ��� (625) ��� ����� � esp8266 
// �������� USART, ���������� � ��������
USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
USART1->CR1 |= USART_CR1_RXNEIE; // ��������� ���������� �� ����� RX
// ��������� �� ���������: 8 ���. ���, 1 �������� ���, �������� �������� ���
// ��������� ���������� UART1 � ����������� ����������
NVIC_EnableIRQ(USART1_IRQn);

//====================== USART2 ====================== 
// �������� ������������ ����� GPIOA � ����� �������������� �������
//RCC->APB1ENR |= (RCC_APB1ENR_IOPAEN | RCC_APB1ENR_AFIOEN); 
RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // �������� ������������ USART2
// PA2 (TX2) AFIO Push-Pull, 10MHz. PA3 (RX2) HiZ, 10MHz
// ������� ������������� ���� ��� � "00", ��� ������ ����������� !
GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2 | GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
// ����� ������ ���� ������������� � '1'
GPIOA->CRL |= (GPIO_CRL_MODE2_0 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_0); 
USART2->BRR = 313; // PCLK1 / Baud = 36000000 /  115200 ��� (313)
// �������� USART, ���������� � ��������
USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
USART2->CR1 |= USART_CR1_RXNEIE; // ��������� ���������� �� ����� RX
// ��������� �� ���������: 8 ���. ���, 1 �������� ���, �������� �������� ���
// ��������� ���������� UART1 � ����������� ����������
NVIC_EnableIRQ(USART2_IRQn);

// ======================== TIM2 =======================
// ��������� ������������, PCLK1 = 36 ���, TIM2 ���������� x 2 = 72 ���
RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
TIM2->SMCR &= ~TIM_SMCR_SMS; // �������� ���������� ������������
TIM2->PSC = 720 - 1; // ��� (�����) ����� 10 ���
TIM2->ARR = 50000 - 1; // ������������ ����� 0,5 ���.
TIM2->DIER |= TIM_DIER_UIE; // ��������� ���������� �� ������������ �������
NVIC_EnableIRQ(TIM2_IRQn); // ��������� ���������� � ����������� ����������
TIM2->CR1 = TIM_CR1_CEN; // ��������� ����
}  
// �����