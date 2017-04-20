void usart1_conf(void);
void Usart1_Sends(unsigned char data[],unsigned char num);
void usart2_conf(void);
void Usart2_Sends(char *str);
int USART2_Receive(const char *str);
extern unsigned short uart1_trx[],uart1_tx,uart1_rx;
