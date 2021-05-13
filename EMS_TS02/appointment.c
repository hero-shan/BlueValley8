#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

#include <sys/shm.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/ioctl.h>   
#include <sys/socket.h>   
#include <netdb.h>    
#include <net/if.h> 

#include <pthread.h>
#include <bits/wordsize.h>
#include "mymain.h"
#include <errno.h>


#define PORT                  502

void AppointOutNet_GetCarType()
{
    int i = 0;
    for ( i = 0; i < 20; i++)
    {
        if(Cloud_OutNetLock_flg[i]==1||Cloud_AppointLock_flg[i]==1)
        {
            if(Cloud_OutNetLock_flg[i]==1){
                if((strcmp(Cloud_OutNet_CarNo[i],Cloud_Grab_carNo)==0)){
                    Cloud_BookCarNo_flg = 2;
                    CE_OutNetSelfCe_sour = i;
                    
                    Cloud_OutNetLock_flg[i] = 0;
                    strcpy(Cloud_OutNet_CarNo[i],"");
                    break;
                }
            }
            else if(Cloud_AppointLock_flg[i]==1){
                if((strcmp(Cloud_Appoint_CarNo[i],Cloud_Grab_carNo)==0)){
                    Cloud_BookCarNo_flg = 1;
                    CE_OutNetSelfCe_sour = i;

                    Cloud_AppointLock_flg[i] = 0;
                    strcpy(Cloud_Appoint_CarNo[i],"");
                    break;
                }
            }
            
        }
    }
    
}
void Appoint_GetCarType()
{
	 int i=0,j=0;
        for(i=0;i<20;i++)
        {
            if((strcmp(Cloud_BookcarNo1[i],Cloud_Grab_carNo)==0)||
                (strcmp(Cloud_BookcarNo2[i],Cloud_Grab_carNo)==0)||
                 (strcmp(Cloud_BookcarNo3[i],Cloud_Grab_carNo)==0)){
               Cloud_BookCarNo_flg = 1;

               if(Cloud_carType_Code==1&&Locat_Book_num[0]>0)
                {  
  //                  printf("SysSta456465645=%d,%s \n",Locat_Book_num[0],Cloud_BookcarNo1[Locat_Book_num[0]-1]);     
                    for(i=0;i<Locat_Book_num[0];i++){
                        if((strcmp(Cloud_BookcarNo1[i],Cloud_Grab_carNo)==0)){
                            for(j=i;j<Locat_Book_num[0]-1;j++){
                                strcpy(Cloud_BookcarNo1[j],Cloud_BookcarNo1[j+1]);
                            }      
                            bzero(Cloud_BookcarNo1[Locat_Book_num[0]-1],100);
                            Locat_Book_num[0]=(Locat_Book_num[0]-1);    
  //                          printf("SysSta2131321321=%d,%s \n",Locat_Book_num[0],Cloud_BookcarNo1[Locat_Book_num[0]-1]);
                            break;
                        }
                    }     
                }
                if(Cloud_carType_Code==2&&Locat_Book_num[1]>0)
                {  
                    for(i=0;i<Locat_Book_num[1];i++){
                        if((strcmp(Cloud_BookcarNo2[i],Cloud_Grab_carNo)==0)){
                            for(j=i;j<Locat_Book_num[1]-1;j++){
                                strcpy(Cloud_BookcarNo2[j],Cloud_BookcarNo2[j+1]);
                            }      
                            bzero(Cloud_BookcarNo2[Locat_Book_num[1]-1],100);
                            Locat_Book_num[1]=(Locat_Book_num[1]-1);         
                            break;
                        }
                    }     
                }
                if(Cloud_carType_Code==3&&Locat_Book_num[2]>0)
                {  
                    for(i=0;i<Locat_Book_num[2];i++){
                        if((strcmp(Cloud_BookcarNo3[i],Cloud_Grab_carNo)==0)){
                            for(j=i;j<Locat_Book_num[2]-1;j++){
                                strcpy(Cloud_BookcarNo3[j],Cloud_BookcarNo3[j+1]);
                            }      
                            bzero(Cloud_BookcarNo3[Locat_Book_num[2]-1],100);
                            Locat_Book_num[2]=(Locat_Book_num[2]-1);       
                            break;
                        }
                    }     
                }
               break;
             }
        }
}

void Appoint_CallBack_Deal(int count ,char * carType , char SendBookCE_flg)
{
    // printf("ssssssssssssssssss\n\n\n\n\n");
    // printf("aaaaaaaaaaaaaaaaaaa\n\n\n\n\n");
    int i = 0;
    switch (SendBookCE_flg)
    {
    case 0:
    {
        if(Cloud_SendBookCE_flg==5){
            Book_Init_cou++;
            if(Book_Init_cou>2){
                Cloud_SendBookCE_flg = 0;
                Book_Init_flg = 2;
                MLocat_CheckLock_flg = 5;
            }
            }
            else if(Cloud_SendBookCE_flg==6){
  //              printf("ssssssssssssssssss\n\n\n\n\n");
                Book_Init_cou++;
                if(Book_Init_cou>1){
                    Cloud_SendBookCE_flg = 0;
                    Book_Init_flg = 2;
                    MLocat_CheckLock_flg = 6;
                }
            }
            else{
                Cloud_SendBookCE_flg = 0;
                MLocat_CheckLock_flg = 1;
            } 
    }
    break;
    case 1:
    {
        for(i=0;i<count;i++)
        {
            if(strcmp(carType,"EU300")==0)
            {
                strcpy(Cloud_BookcarNo1[Locat_Book_num[0]],plate[i]);
                Locat_Book_num[0]++;
            }
            else if(strcmp(carType,"EU5A")==0)
            {
                strcpy(Cloud_BookcarNo2[Locat_Book_num[1]],plate[i]);
                Locat_Book_num[1]++;
            }
            else if(strcmp(carType,"EU5B")==0)
            {
                strcpy(Cloud_BookcarNo3[Locat_Book_num[2]],plate[i]);
                Locat_Book_num[2]++;
            }
        }   
    }
    break;
    case 2:
    {
        for(i=0;i<count;i++)
        {
            if(strcmp(carType,"EU300")==0)
            {
                strcpy(Clear_BookcarNo1[Clear_Book_num[0]],plate[i]);
                Clear_Book_num[0]++;
            }
            else if(strcmp(carType,"EU5A")==0)
            {
                strcpy(Clear_BookcarNo2[Clear_Book_num[1]],plate[i]);
                Clear_Book_num[1]++;
            }
            else if(strcmp(carType,"EU5B")==0)
            {
                strcpy(Clear_BookcarNo3[Clear_Book_num[2]],plate[i]);
                Clear_Book_num[2]++;
            }
        } 
    }
    break;
    case 4:
    {
        if(Locat_Book_num[0]==Clear_Book_num[0]&&Locat_Book_num[1]==Clear_Book_num[1]&&
						Locat_Book_num[2]==Clear_Book_num[2])
        {
            for(i=0;i<Clear_Book_num[0];i++)
            {
                strcpy(Cloud_BookcarNo1[i],Clear_BookcarNo1[i]) ; 			              
            }
            for(i=0;i<Clear_Book_num[1];i++)
            {
                strcpy(Cloud_BookcarNo2[i],Clear_BookcarNo2[i]) ; 			              
            }
            for(i=0;i<Clear_Book_num[2];i++)
            {
                strcpy(Cloud_BookcarNo3[i],Clear_BookcarNo3[i]) ; 			              
            }
 //           printf("Locat_Book_num:%d,%d,%d\n",Locat_Book_num[0],Locat_Book_num[1],Locat_Book_num[2]);
        }
        else 
        {	
            MLocat_CheckClear_flg = 1;
            if(Locat_Book_num[0]>Clear_Book_num[0])
            {
                for(i=0;i<Locat_Book_num[0];i++)
                {
                    bzero(Cloud_BookcarNo1[i],100) ;                 
                }
                for(i=0;i<Clear_Book_num[0];i++)
                {
                    strcpy(Cloud_BookcarNo1[i],Clear_BookcarNo1[i]) ; 			              
                }
                Locat_Book_num[0] = Clear_Book_num[0];    
            }
            if(Locat_Book_num[1]>Clear_Book_num[1])
            {
                for(i=0;i<Locat_Book_num[1];i++)
                {
                    bzero(Cloud_BookcarNo2[i],100) ;                 
                }
                for(i=0;i<Clear_Book_num[1];i++)
                {
                    strcpy(Cloud_BookcarNo2[i],Clear_BookcarNo2[i]) ; 			              
                }
                Locat_Book_num[1] = Clear_Book_num[1];   
            }
            if(Locat_Book_num[2]>Clear_Book_num[2])
            {
                for(i=0;i<Locat_Book_num[2];i++)
                {
                    bzero(Cloud_BookcarNo3[i],100);                 
                }
                for(i=0;i<Clear_Book_num[2];i++)
                {
                    strcpy(Cloud_BookcarNo3[i],Clear_BookcarNo3[i]) ; 			              
                }
                Locat_Book_num[2] = Clear_Book_num[2]; 
            }
//            printf("Locat_Book_num:%d,%d,%d\n",Locat_Book_num[0],Locat_Book_num[1],Locat_Book_num[2]);
 //           printf("Cloud_BookcarNo1 2:%s,%s\n",Cloud_BookcarNo1[0],Cloud_BookcarNo1[1]);
        }              
    }
    break;
    default:
        break;
    }
}