typedef struct      
{
   double ulAs;     //1AH = 1*3600As
    
   unsigned char ucSOC;    // £”‡µÁ¡ø 1%/Œª            
}TSOCPARA;

extern  TSOCPARA g_SOCPara;

void CalSoc(double iSLA);
