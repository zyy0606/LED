typedef struct      
{
   double ulAs;     //1AH = 1*3600As
    
   unsigned char ucSOC;    //ʣ����� 1%/λ            
}TSOCPARA;

extern  TSOCPARA g_SOCPara;

void CalSoc(double iSLA);
