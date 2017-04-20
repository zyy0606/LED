//  ����LED3�����������
#define LED3ON()      GPIOC->ODR |=  GPIO_Pin_9;
#define LED3OFF()     GPIOC->ODR &= ~GPIO_Pin_9;
#define LED3B()       GPIOC->ODR ^=  GPIO_Pin_9;

//  ����LED4�����������
#define LED4ON()      GPIOC->ODR |=  GPIO_Pin_8;
#define LED4OFF()     GPIOC->ODR &= ~GPIO_Pin_8;
#define LED4B()       GPIOC->ODR ^=  GPIO_Pin_8;

//  ���尴����ӦIO��
#define KEY1      GPIO_Pin_0;

extern unsigned char key_bit;

void gpio_conf(void);
void key_work(void);
