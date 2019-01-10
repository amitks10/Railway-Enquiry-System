#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct stations
{
    char sName[30];
    char sCode[6];
}stations;
stations s[200];
int l=0;
void printAllStations()
{
    int j;
    for(j=0;j<l;j++)
    {
        printf("%2d: (%4s)  %s \n",j+1,s[j].sCode,s[j].sName);
    }
}
int main()
{
    FILE*fp2=fopen("AllStations.txt","w");
    FILE*fp=fopen("TrainNos.txt","r");
    stations s2;
    int i,j,k;
    l=0;
    i=0;
    int d1;
    char c1;
    char tName[6],tFile[17];
    while(!feof(fp))
    {
        fscanf(fp,"%s",tName);
        strcpy(tFile,"Trains/");
        strcat(tFile,tName);
        strcat(tFile,".txt");
        FILE*fp3=fopen(tFile,"r");
        char a[40],b[8];
        fscanf(fp3,"%s",a);
        fscanf(fp3,"%d",&d1);
        fscanf(fp3,"%s",b);
        while(!feof(fp3))
        {
            fscanf(fp3,"%d",&d1);
            fscanf(fp3,"%s",s2.sCode);
            fscanf(fp3,"%s",s2.sName);
            fscanf(fp3,"%d",&d1);
            fscanf(fp3,"%c",&c1);
            fscanf(fp3,"%d",&d1);
            fscanf(fp3,"%d",&d1);
            fscanf(fp3,"%c",&c1);
            fscanf(fp3,"%d",&d1);
            fscanf(fp3,"%d",&d1);
            k=0;
            for(i=0;i<l;i++)
            {
                if(strcmp(s2.sCode,s[i].sCode)==0)
                {
                    k=1;
                    break;
                }
            }
            if(k==0)
            {
                strcpy(s[l].sCode,s2.sCode);
                strcpy(s[l].sName,s2.sName);
                l++;
            }
        }
        fclose(fp3);
    }
    int m,n;
    stations temp;
    for(m=0;m<l;m++)
    {
        for(n=0;n<l-m-1;n++)
        {
            if(strcmp(s[n].sName,s[n+1].sName)>0)
            {
                strcpy(temp.sCode,s[n].sCode);
                strcpy(temp.sName,s[n].sName);
                strcpy(s[n].sCode,s[n+1].sCode);
                strcpy(s[n].sName,s[n+1].sName);
                strcpy(s[n+1].sCode,temp.sCode);
                strcpy(s[n+1].sName,temp.sName);
            }
        }
    }
    for(j=0;j<l;j++)
    {
        fprintf(fp2,"%4s\t%s\n",s[j].sCode,s[j].sName);
    }
    printAllStations(s,l);
    fclose(fp);
    fclose(fp2);
    return 0;
}
