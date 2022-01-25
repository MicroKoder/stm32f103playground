#include "stm32f1xx.h"
#include "main.h"
static void InitUSART(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
	USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	USART1->BRR = 0x1D5C;
	//PA9 - TX
	GPIOA->CRH |= GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1;
	//PA10 - RX
	GPIOA->CRH |= GPIO_CRH_CNF10_1;
	
}
static volatile uint32_t syscnt;
void SysTick_Handler(void)
{
	syscnt++;
}

static void delay_ms(uint32_t ms)
{
	uint32_t await_ms = syscnt + ms;
	while(syscnt < await_ms);
}
int main()
{
	SystemInit();
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;	//ENABLE port B
	
	//pin 12 output, push-pull
	GPIOB->CRH |= GPIO_CRH_MODE12_0;
	
	//SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	//SysTick->LOAD = 9000;	//1 ms sys timer
	SysTick_Config(9000);
	
	InitUSART();
	//static uint32_t i;
	while(1)
	{
		delay_ms(100);
		//set bit 12
		GPIOB->BSRR |= GPIO_BSRR_BS12;	
		
		delay_ms(200);
		//reset bit 12
		GPIOB->BSRR |= GPIO_BSRR_BR12;	
	}

}
