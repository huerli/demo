include_directories(PICTURE)
file("PICTURE/*.*")
typedef DWORD FSIZE_t;

typedef struct {
    FATFS*	fs;			/* Pointer to the owner file system object */
    WORD	id;			/* Owner file system mount ID */
    BYTE	attr;		/* Object attribute */
    BYTE	stat;		/* Object chain status (b1-0: =0:not contiguous, =2:contiguous (no data on FAT), =3:got flagmented, b2:sub-directory stretched) */
    DWORD	sclust;		/* Object start cluster (0:no cluster or root directory) */
    FSIZE_t	objsize;	/* Object size (valid when sclust != 0) */
#if _FS_EXFAT
    DWORD	n_cont;		/* Size of coutiguous part, clusters - 1 (valid when stat == 3) */
	DWORD	c_scl;		/* Containing directory start cluster (valid when sclust != 0) */
	DWORD	c_size;		/* b31-b8:Size of containing directory, b7-b0: Chain status (valid when c_scl != 0) */
	DWORD	c_ofs;		/* Offset in the containing directory (valid when sclust != 0) */
#endif
#if _FS_LOCK != 0
    UINT	lockid;		/* File lock ID origin from 1 (index of file semaphore table Files[]) */
#endif
} _FDID;

typedef struct {
    _FDID	obj;			/* Object identifier */
    DWORD	dptr;			/* Current read/write offset */
    DWORD	clust;			/* Current cluster */
    DWORD	sect;			/* Current sector */
    BYTE*	dir;			/* Pointer to the directory item in the win[] */
    BYTE*	fn;				/* Pointer to the SFN (in/out) {body[8],ext[3],status[1]} */
#if _USE_LFN != 0
    DWORD	blk_ofs;		/* Offset of current entry block being processed (0xFFFFFFFF:Invalid) */
    WCHAR*	lfn;			/* Pointer to the LFN working buffer */
#endif
#if _USE_FIND
    const TCHAR* pat;		/* Pointer to the name matching pattern */
#endif
} DIR;
//读写W25q实验
while(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==GPIO_PIN_SET);
//    while(1)
//    {
//        id = W25QXX_ReadID();
//        if (id == W25Q128 || id == NM25Q128)
//            break;
//        LCD_ShowString(30,150,200,16,16,"W25Q128 Check Failed!");
//        HAL_Delay(500);
//        LCD_ShowString(30,150,200,16,16,"Please Check!        ");
//        HAL_Delay(500);
//        HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);
//    }

//    while(1)
//    {
//        key=KEY_Scan(0);
//        if(key==KEY1_PRES)
//        {
//            LCD_Fill(0,170,239,319,WHITE);
//            LCD_ShowString(30,170,200,16,16,"Start Write W25Q128....");
//            W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);
//            LCD_ShowString(30,170,200,16,16,"W25Q256 Write Finished!");
//        }
//        if(key==KEY0_PRES)
//        {
//            LCD_ShowString(30,170,200,16,16,"Start Read W25Q128.... ");
//            W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);
//            LCD_ShowString(30,170,200,16,16,"The Data Readed Is:   ");
//            LCD_ShowString(30,190,200,16,16,datatemp);
//        }
//        if(key==WKUP_PRES) break;
//        i++;
//        HAL_Delay(10);
//        if(i==20)
//        {
//            HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);
//            i=0;
//        }
//    }
//内存管理实验
while(1)
    {
        key=KEY_Scan(0);
        switch(key)
        {
            case 0:
                break;
            case KEY0_PRES:
                p=mymalloc(SRAMIN,2048);
                if(p!=NULL)sprintf((char*)p,"Memory Malloc Test%03d",i);
                break;
            case KEY1_PRES:
                myfree(SRAMIN,p);
                p=0;
                break;
            case WKUP_PRES:
                if(p!=NULL)
                {
                    sprintf((char*)p,"Memory Malloc Test%03d",i);
                    LCD_ShowString(30,250,200,16,16,p);
                }
                break;
        }
        if(tp!=p)
        {
            tp=p;
            sprintf((char*)paddr,"P Addr:0X%08X",(u32)tp);
            LCD_ShowString(30,230,200,16,16,paddr);
            if(p)LCD_ShowString(30,250,200,16,16,p);
            else LCD_Fill(30,250,239,266,WHITE);
        }
        HAL_Delay(10);
        i++;
        if((i%20)==0)
        {
            LCD_ShowNum(30+96,190,my_mem_perused(SRAMIN),3,16);
            HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);
        }
    }