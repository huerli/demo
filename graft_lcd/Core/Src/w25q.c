//
// Created by huerli on 2022/7/1.
//
#include "w25q.h"
#include "spi.h"


u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&hspi2,&TxData,&Rxdata,1, 1000);
    return Rxdata;
}
u16 W25QXX_TYPE=W25Q256;

void W25QXX_Init(void)
{
    u8 temp;
    W25QXX_CS_1;
    W25QXX_TYPE=W25QXX_ReadID();
    if(W25QXX_TYPE==W25Q256)
    {
        temp=W25QXX_ReadSR(3);
        if((temp&0X01)==0)
        {
            W25QXX_CS_0;
            SPI2_ReadWriteByte(W25X_Enable4ByteAddr);
            W25QXX_CS_1;
        }
    }
}

u8 W25QXX_ReadSR(u8 regno)
{
    u8 byte=0,command=0;
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;
            break;
        case 2:
            command=W25X_ReadStatusReg2;
            break;
        case 3:
            command=W25X_ReadStatusReg3;
            break;
        default:
            command=W25X_ReadStatusReg1;
            break;
    }
    W25QXX_CS_0;
    SPI2_ReadWriteByte(command);
    byte=SPI2_ReadWriteByte(0Xff);
    W25QXX_CS_1;
    return byte;
}
//Ð´W25QXX×´Ì¬¼Ä´æÆ÷
void W25QXX_Write_SR(u8 regno,u8 sr)
{
    u8 command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //Ð´×´Ì¬¼Ä´æÆ÷1Ö¸Áî
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //Ð´×´Ì¬¼Ä´æÆ÷2Ö¸Áî
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //Ð´×´Ì¬¼Ä´æÆ÷3Ö¸Áî
            break;
        default:
            command=W25X_WriteStatusReg1;
            break;
    }
    W25QXX_CS_0;                            //Ê¹ÄÜÆ÷¼þ
    SPI2_ReadWriteByte(command);            //·¢ËÍÐ´È¡×´Ì¬¼Ä´æÆ÷ÃüÁî
    SPI2_ReadWriteByte(sr);                 //Ð´ÈëÒ»¸ö×Ö½Ú
    W25QXX_CS_1;                            //È¡ÏûÆ¬Ñ¡
}
//W25QXXÐ´Ê¹ÄÜ
//½«WELÖÃÎ»
void W25QXX_Write_Enable(void)
{
    W25QXX_CS_0;                            //Ê¹ÄÜÆ÷¼þ
    SPI2_ReadWriteByte(W25X_WriteEnable);   //·¢ËÍÐ´Ê¹ÄÜ
    W25QXX_CS_1;                            //È¡ÏûÆ¬Ñ¡
}
//W25QXXÐ´½ûÖ¹
//½«WELÇåÁã
void W25QXX_Write_Disable(void)
{
    W25QXX_CS_0;                            //Ê¹ÄÜÆ÷¼þ
    SPI2_ReadWriteByte(W25X_WriteDisable);  //·¢ËÍÐ´½ûÖ¹Ö¸Áî
    W25QXX_CS_1;                            //È¡ÏûÆ¬Ñ¡
}

//¶ÁÈ¡Ð¾Æ¬ID
//·µ»ØÖµÈçÏÂ:
//0XEF13,±íÊ¾Ð¾Æ¬ÐÍºÅÎªW25Q80
//0XEF14,±íÊ¾Ð¾Æ¬ÐÍºÅÎªW25Q16
//0XEF15,±íÊ¾Ð¾Æ¬ÐÍºÅÎªW25Q32
//0XEF16,±íÊ¾Ð¾Æ¬ÐÍºÅÎªW25Q64
//0XEF17,±íÊ¾Ð¾Æ¬ÐÍºÅÎªW25Q128
//0XEF18,±íÊ¾Ð¾Æ¬ÐÍºÅÎªW25Q256
u16 W25QXX_ReadID(void)
{
    u16 Temp = 0;
    W25QXX_CS_0;
    SPI2_ReadWriteByte(0x90);//·¢ËÍ¶ÁÈ¡IDÃüÁî
    SPI2_ReadWriteByte(0x00);
    SPI2_ReadWriteByte(0x00);
    SPI2_ReadWriteByte(0x00);
    Temp|=SPI2_ReadWriteByte(0xFF)<<8;
    Temp|=SPI2_ReadWriteByte(0xFF);
    W25QXX_CS_1;
    return Temp;
}

void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
    u16 i;
    W25QXX_CS_0;                            //Ê¹ÄÜÆ÷¼þ
    SPI2_ReadWriteByte(W25X_ReadData);      //·¢ËÍ¶ÁÈ¡ÃüÁî
    if(W25QXX_TYPE==W25Q256)                //Èç¹ûÊÇW25Q256µÄ»°µØÖ·Îª4×Ö½ÚµÄ£¬Òª·¢ËÍ×î¸ß8Î»
    {
        SPI2_ReadWriteByte((u8)((ReadAddr)>>24));
    }
    SPI2_ReadWriteByte((u8)((ReadAddr)>>16));   //·¢ËÍ24bitµØÖ·
    SPI2_ReadWriteByte((u8)((ReadAddr)>>8));
    SPI2_ReadWriteByte((u8)ReadAddr);
    for(i=0;i<NumByteToRead;i++)
    {
        pBuffer[i]=SPI2_ReadWriteByte(0XFF);    //Ñ­»·¶ÁÊý
    }
    W25QXX_CS_1;
}

void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u16 i;
    W25QXX_Write_Enable();                  //SET WEL
    W25QXX_CS_0;                            //Ê¹ÄÜÆ÷¼þ
    SPI2_ReadWriteByte(W25X_PageProgram);   //·¢ËÍÐ´Ò³ÃüÁî
    if(W25QXX_TYPE==W25Q256)                //Èç¹ûÊÇW25Q256µÄ»°µØÖ·Îª4×Ö½ÚµÄ£¬Òª·¢ËÍ×î¸ß8Î»
    {
        SPI2_ReadWriteByte((u8)((WriteAddr)>>24));
    }
    SPI2_ReadWriteByte((u8)((WriteAddr)>>16)); //·¢ËÍ24bitµØÖ·
    SPI2_ReadWriteByte((u8)((WriteAddr)>>8));
    SPI2_ReadWriteByte((u8)WriteAddr);
    for(i=0;i<NumByteToWrite;i++)SPI2_ReadWriteByte(pBuffer[i]);//Ñ­»·Ð´Êý
    W25QXX_CS_1;                            //È¡ÏûÆ¬Ñ¡
    W25QXX_Wait_Busy();					   //µÈ´ýÐ´Èë½áÊø
}
//ÎÞ¼ìÑéÐ´SPI FLASH
//±ØÐëÈ·±£ËùÐ´µÄµØÖ··¶Î§ÄÚµÄÊý¾ÝÈ«²¿Îª0XFF,·ñÔòÔÚ·Ç0XFF´¦Ð´ÈëµÄÊý¾Ý½«Ê§°Ü!
//¾ßÓÐ×Ô¶¯»»Ò³¹¦ÄÜ
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ð´ÈëÖ¸¶¨³¤¶ÈµÄÊý¾Ý,µ«ÊÇÒªÈ·±£µØÖ·²»Ô½½ç!
//pBuffer:Êý¾Ý´æ´¢Çø
//WriteAddr:¿ªÊ¼Ð´ÈëµÄµØÖ·(24bit)
//NumByteToWrite:ÒªÐ´ÈëµÄ×Ö½ÚÊý(×î´ó65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u16 pageremain;
    pageremain=256-WriteAddr%256; //µ¥Ò³Ê£ÓàµÄ×Ö½ÚÊý
    if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//²»´óÓÚ256¸ö×Ö½Ú
    while(1)
    {
        W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)break;//Ð´Èë½áÊøÁË
        else //NumByteToWrite>pageremain
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;

            NumByteToWrite-=pageremain;			  //¼õÈ¥ÒÑ¾­Ð´ÈëÁËµÄ×Ö½ÚÊý
            if(NumByteToWrite>256)pageremain=256; //Ò»´Î¿ÉÒÔÐ´Èë256¸ö×Ö½Ú
            else pageremain=NumByteToWrite; 	  //²»¹»256¸ö×Ö½ÚÁË
        }
    };
}
//Ð´SPI FLASH
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ð´ÈëÖ¸¶¨³¤¶ÈµÄÊý¾Ý
//¸Ãº¯Êý´ø²Á³ý²Ù×÷!
//pBuffer:Êý¾Ý´æ´¢Çø
//WriteAddr:¿ªÊ¼Ð´ÈëµÄµØÖ·(24bit)
//NumByteToWrite:ÒªÐ´ÈëµÄ×Ö½ÚÊý(×î´ó65535)
u8 W25QXX_BUFFER[4096];
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u8 * W25QXX_BUF;
    W25QXX_BUF=W25QXX_BUFFER;
    secpos=WriteAddr/4096;//ÉÈÇøµØÖ·
    secoff=WriteAddr%4096;//ÔÚÉÈÇøÄÚµÄÆ«ÒÆ
    secremain=4096-secoff;//ÉÈÇøÊ£Óà¿Õ¼ä´óÐ¡
    //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//²âÊÔÓÃ
    if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//²»´óÓÚ4096¸ö×Ö½Ú
    while(1)
    {
        W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//¶Á³öÕû¸öÉÈÇøµÄÄÚÈÝ
        for(i=0;i<secremain;i++)//Ð£ÑéÊý¾Ý
        {
            if(W25QXX_BUF[secoff+i]!=0XFF)break;//ÐèÒª²Á³ý
        }
        if(i<secremain)//ÐèÒª²Á³ý
        {
            W25QXX_Erase_Sector(secpos);//²Á³ýÕâ¸öÉÈÇø
            for(i=0;i<secremain;i++)	   //¸´ÖÆ
            {
                W25QXX_BUF[i+secoff]=pBuffer[i];
            }
            W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//Ð´ÈëÕû¸öÉÈÇø

        }else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//Ð´ÒÑ¾­²Á³ýÁËµÄ,Ö±½ÓÐ´ÈëÉÈÇøÊ£ÓàÇø¼ä.
        if(NumByteToWrite==secremain)break;//Ð´Èë½áÊøÁË
        else//Ð´ÈëÎ´½áÊø
        {
            secpos++;//ÉÈÇøµØÖ·Ôö1
            secoff=0;//Æ«ÒÆÎ»ÖÃÎª0

            pBuffer+=secremain;  //Ö¸ÕëÆ«ÒÆ
            WriteAddr+=secremain;//Ð´µØÖ·Æ«ÒÆ
            NumByteToWrite-=secremain;				//×Ö½ÚÊýµÝ¼õ
            if(NumByteToWrite>4096)secremain=4096;	//ÏÂÒ»¸öÉÈÇø»¹ÊÇÐ´²»Íê
            else secremain=NumByteToWrite;			//ÏÂÒ»¸öÉÈÇø¿ÉÒÔÐ´ÍêÁË
        }
    };
}
//²Á³ýÕû¸öÐ¾Æ¬
//µÈ´ýÊ±¼ä³¬³¤...
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();                  //SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS_0;                            //Ê¹ÄÜÆ÷¼þ
    SPI2_ReadWriteByte(W25X_ChipErase);        //·¢ËÍÆ¬²Á³ýÃüÁî
    W25QXX_CS_1;                            //È¡ÏûÆ¬Ñ¡
    W25QXX_Wait_Busy();   				   //µÈ´ýÐ¾Æ¬²Á³ý½áÊø
}

void W25QXX_Erase_Sector(u32 Dst_Addr)
{
    //¼àÊÓfalsh²Á³ýÇé¿ö,²âÊÔÓÃ
    //printf("fe:%x\r\n",Dst_Addr);
    Dst_Addr*=4096;
    W25QXX_Write_Enable();
    W25QXX_Wait_Busy();
    W25QXX_CS_0;
    SPI2_ReadWriteByte(W25X_SectorErase);
    if(W25QXX_TYPE==W25Q256)
    {
        SPI2_ReadWriteByte((u8)((Dst_Addr)>>24));
    }
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));
    SPI2_ReadWriteByte((u8)Dst_Addr);
    W25QXX_CS_1;
    W25QXX_Wait_Busy();
}

void W25QXX_Wait_Busy(void)
{
    while((W25QXX_ReadSR(1)&0x01)==0x01);
}

void W25QXX_PowerDown(void)
{
    W25QXX_CS_0;
    SPI2_ReadWriteByte(W25X_PowerDown);
    W25QXX_CS_1;
    HAL_Delay(1);
}

void W25QXX_WAKEUP(void)
{
    W25QXX_CS_0;
    SPI2_ReadWriteByte(W25X_ReleasePowerDown);
    W25QXX_CS_1;
    HAL_Delay(1);
}
