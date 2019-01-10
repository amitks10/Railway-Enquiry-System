#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include"GetStations.h"
#include<graphics.h>
#include<conio.h>
typedef struct Time
{
    int hour;
    int min;
}Time;
typedef struct DB
{
    int count;
    struct train *start;
}database;
typedef struct train
{
    struct train *next;
    struct station *start;
    char tNo[6];
    char tName[40];
    int NOS;
    int dist;
    char week[8];
    Time late;
}train;
typedef struct station
{
    struct station *next;
    short sNo;//nth station from source
    char sName[30];
    char sCode[6];
    Time arr;
    Time dep;
    int dist;//distance from source
}station;
database*initDB()
{
    database*h=(database*)malloc(sizeof(database));
    h->start=NULL;
    h->count=0;
    return h;
}
train*initTrain()
{
    train * t=(train*)malloc(sizeof(train));
    t->next=NULL;
    //t->fp=NULL;
    t->dist=0;
    t->NOS=0;
    strcpy(t->tNo,"\0");
    strcpy(t->tName,"\0");
    t->start=NULL;
    strcpy(t->week,"0000000\0");
    return t;
}
station *createStationNode()
{
    station*s=(station*)malloc(sizeof(station));
    s->next=NULL;
    s->arr.hour=0;
    s->arr.min=0;
    s->dep.hour=0;
    s->dep.min=0;
    s->dist=0;
    strcpy(s->sCode,"\0");
    s->sNo=0;
    strcpy(s->sName,"\0");
    return s;
}
train*loadTrain(train*t,char tempNo[])
{
    char tFile[17]="Trains/";
    strcpy(t->tNo,tempNo);
    strcat(tFile,tempNo);
    strcat(tFile,".txt");
    FILE*fp=fopen(tFile,"r");
    fscanf(fp,"%s",t->tName);
    fscanf(fp,"%d",&(t->NOS));
    fscanf(fp,"%s",t->week);
    station *currs,*s;
    while(!feof(fp))
    {
        s=createStationNode();
        fscanf(fp,"%d",&(s->sNo));
        fscanf(fp,"%s",s->sCode);
        fscanf(fp,"%s",s->sName);
        fscanf(fp,"%d",&(s->arr.hour));
        char c;
        fscanf(fp,"%c",&c);
        fscanf(fp,"%d",&(s->arr.min));
        fscanf(fp,"%d",&(s->dep.hour));
        fscanf(fp,"%c",&c);
        fscanf(fp,"%d",&(s->dep.min));
        fscanf(fp,"%d",&(s->dist));
        if(t->start==NULL)
        {
            t->start=s;
            currs=s;
        }
        else
        {
            currs->next=s;
            currs=s;
        }
    }
    fclose(fp);
    return t;
}
database*loadDB(database*h)
{
    FILE*TNOS=fopen("TrainNos.txt","r");
    char tempNo[10];
    train*currt,*t;
    while(!feof(TNOS))
    {
        fscanf(TNOS,"%s",tempNo);
        t=initTrain();
        t=loadTrain(t,tempNo);
        if(h->start==NULL)
        {
            h->start=t;
            currt=t;
        }
        else
        {
            currt->next=t;
            currt=t;
        }
    }
    return h;
}
Time getCurrentTime()
{
    time_t t;
    Time currentTime;
    time(&t);
    char c[20];
    strcpy(c,ctime(&t));
    currentTime.hour=(c[11]-48)*10+(c[12]-48);
    currentTime.min=(c[14]-48)*10+(c[15]-48);
    return currentTime;
}
void display(database *h)
{
    train *t=h->start;
    char out[256];
    int i=0,inp;
    while(t!=NULL)
    {
    dobara:
    cleardevice();
    sprintf(out,"Train info :\n%s     %s",t->tName,t->tNo);
    outtextxy(70,5,out);
    station*s=t->start;
    i=0;
    while(s!=NULL)
    {
        i++;
        line(5,i*30-5,getmaxx()-35,i*30-5);
        if(i*30+30>getmaxy())
        {
            line(5,25,5,480);
            line(280,25,280,480);
            line(27,25,27,480);
            line(460,25,460,480);
            line(getmaxx()-35,25,getmaxx()-35,480);
            inp=getch();
        if(inp==54)
        {
            i=1;
            cleardevice();
            sprintf(out,"Train info :%s     %s",t->tName,t->tNo);
            outtextxy(70,5,out);
        }
        else
        {
            return;
        }
        }
        sprintf(out,"%2d  %20s  %4s ",s->sNo,s->sName,s->sCode);
        outtextxy(10,i*30,out);
        sprintf(out,"ARR-%02d:%02d   DEP-%02d:%02d   ",s->arr.hour,s->arr.min,s->dep.hour,s->dep.min);
        outtextxy(300,i*30,out);
        sprintf(out,"Dist-%4d \n",s->dist);
        outtextxy(500,i*30,out);
        s=s->next;
        line(5,i*30-5,getmaxx()-35,i*30-5);
    }
    line(5,i*30+30,getmaxx()-35,i*30+30);
    line(5,25,5,i*30+30);
    line(280,25,280,i*30+30);
    line(27,25,27,i*30+30);
    line(460,25,460,i*30+30);
    line(getmaxx()-35,25,getmaxx()-35,i*30+30);
    inp=getch();
    if(inp==52)
        goto dobara;
    t=t->next;
    }
}
void printTrains(database*h)
{
    int i=1;
    train*t=h->start;
    char out[256];
    int inp;
    while(t!=NULL)
    {
        if(i*30+30>getmaxy())
        {
            i=1;
            inp=getch();
            if(inp==54)
            {
                cleardevice();
                //outtext("Choose the train to see schedule :");
            }
        }
        sprintf(out,"%25s     %s",t->tName,t->tNo);
        outtextxy(100,i*30,out);
        i++;
        t=t->next;
    }
    getch();

}
void displayTrain(database *h)
{
    cleardevice();
    GT:outtext("\nChoose the train to see schedule :\n\n");
    Sleep(600);
    printTrains(h);
    outtext("\nChoose the train to see schedule :\n\n");
    char tr[6],inp,out[256];
    int in=0,x=0;
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)
            break;
        if(inp==8)
        {
            in--;
            x--;
            outtextxy(250+x*8,0,"     ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(250+x*8,0,out);
        tr[in++]=inp;
    }
    tr[in]='\0';
    train *t=h->start;
    int i=0;
    while(t!=NULL)
    {
        if(strcmp(t->tNo,tr)==0)
        {
            i=1;
            break;
        }
        t=t->next;
    }
    if(i==0)
    {
        outtext("\nNo such train. Please Re-enter your choice.\n");
        getch();
        cleardevice();
        goto GT;
    }
    dobara:
    cleardevice();
    sprintf(out,"Train info :\n%s     %s",t->tName,t->tNo);
    outtextxy(70,5,out);
    station*s=t->start;
    i=0;
    while(s!=NULL)
    {
        i++;
        line(5,i*30-5,getmaxx()-35,i*30-5);
        if(i*30+30>getmaxy())
        {
            line(5,25,5,480);
            line(280,25,280,480);
            line(27,25,27,480);
            line(460,25,460,480);
            line(getmaxx()-35,25,getmaxx()-35,480);
            inp=getch();
        if(inp==54)
        {
            i=1;
            cleardevice();
            sprintf(out,"Train info :%s     %s",t->tName,t->tNo);
            outtextxy(70,5,out);
        }
        else
        {
            return;
        }
        }
        sprintf(out,"%2d  %20s  %4s ",s->sNo,s->sName,s->sCode);
        outtextxy(10,i*30,out);
        sprintf(out,"ARR-%02d:%02d   DEP-%02d:%02d   ",s->arr.hour,s->arr.min,s->dep.hour,s->dep.min);
        outtextxy(300,i*30,out);
        sprintf(out,"Dist-%4d \n",s->dist);
        outtextxy(500,i*30,out);
        s=s->next;
        line(5,i*30-5,getmaxx()-35,i*30-5);
    }
    line(5,i*30+30,getmaxx()-35,i*30+30);
    line(5,25,5,i*30+30);
    line(280,25,280,i*30+30);
    line(27,25,27,i*30+30);
    line(460,25,460,i*30+30);
    line(getmaxx()-35,25,getmaxx()-35,i*30+30);
    inp=getch();
    if(inp==52)
        goto dobara;
}
int timeComp(Time t1,Time t2)
{
    int a=t1.hour*60+t1.min;
    int b=t2.hour*60+t2.min;
    if(a>b)
        return 1;
    else if(a<b)
        return -1;
    else
        return 0;
}
Time timeDifference(Time t1,Time t2)//t1 is larger
{
    Time d;
    if(t1.hour==t2.hour)
    {
        d.hour=0;
        d.min=t1.min-t2.min;
        return d;
    }
    else if(t1.min>=t2.min)
    {
        d.min=t1.min-t2.min;
        d.hour=t1.hour-t2.hour;
        return d;
    }
    else
    {
        d.hour=t1.hour-t2.hour-1;
        d.min=60-(t2.min-t1.min);
        return d;
    }
}
void spotTrain(database *h)
{
    cleardevice();
    outtext("Choose the train number :");
    Sleep(500);
    printTrains(h);
    outtext("Choose the train number :");
    char tr[6],inp,out[256];
    int in=0,x=0;
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)
            break;
        if(inp==8)
        {
            x--;
            in--;
            outtextxy(250+x*8,0,"     ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(250+x*8,0,out);
        tr[in++]=inp;
    }
    tr[in]='\0';
    cleardevice();
    train *t=h->start;
    while(strcmp(t->tNo,tr)!=0)
        t=t->next;
    station *s=t->start,*s2;
    s2=s->next;
    Time currentTime=getCurrentTime();
    int i=0;
    while(s2!=NULL)
    {
        if(timeComp(currentTime,t->start->arr)==1&&timeComp(currentTime,t->start->dep)==-1)
        {
            outtextxy(10,200,"\nThe train is yet to start from source.\n");
            i=1;
            break;
        }
        else if(timeComp(s->dep,currentTime)==-1&&timeComp(s->next->arr,currentTime)==1)
        {
            Time diff=timeDifference(currentTime,s->dep);
            sprintf(out,"\nThe train is between %s and %s.\nThe train departed %s %02d:%02d ago.\n",s->sName,s->next->sName,s->sName,diff.hour,diff.min);
            outtextxy(10,200,out);
            i=1;
            break;
        }
        else if(timeComp(s->arr,currentTime)==-1&&timeComp(s->dep,currentTime)==1)
        {
            sprintf(out,"\nThe train is at %s.\n",s->sName);
            outtextxy(10,200,out);
            i=1;
            break;
        }
        else if(timeComp(s2->arr,currentTime)==-1&&timeComp(s2->dep,currentTime)==1)
        {
            sprintf(out,"\nThe train is at %s.\n",s2->sName);
            outtextxy(10,200,out);
            i=1;
            break;
        }
         else if(timeComp(s->arr,currentTime)==0||timeComp(s->dep,currentTime)==0)
        {
            sprintf(out,"\nThe train is at %s.\n",s->sName);
            outtextxy(10,200,out);
            i=1;
            break;
        }
        else if(timeComp(s2->arr,currentTime)==0||timeComp(s2->dep,currentTime)==0)
        {
            sprintf(out,"\nThe train is at %s.\n",s2->sName);
            outtextxy(10,200,out);
            i=1;
            break;
        }
        s=s->next;
        s2=s->next;
    }
    if(i==0)
        {
        sprintf(out,"\nThe train is yet to start from source.\n");
        outtextxy(10,200,out);
        }
        getch();
}
void liveStation(database*h)
{
    cleardevice();
    outtextxy(10,100,"Choose the station and enter the Station code:");
    //display all stations here
    char a[5],c[6],out[256],inp;
    int in=0,j=0,x=0;
    while(1)
    {
        j++;x++;
        inp=getch();
        if(inp==13)
            break;
        if(inp==8)
        {
            in--;
            j--;
            outtextxy(10*j,150,"     ");
            j--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(10*j,150,out);
        a[in++]=inp;
    }
    a[in]='\0';
    outtextxy(10,200,"Enter the time for which you want to see live status(in HH:MM format):");
    in=0;
    j=0;
    while(1)
    {
        j++;
        inp=getch();
        if(inp==13)
            break;
        if(inp==8)
        {
            in--;
            j--;
            outtextxy(8*j,250,"     ");
            j--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(8*j,250,out);
        c[in++]=inp;
    }
    c[in]='\0';
    cleardevice();
    outtextxy(50,0,"Station:");
    outtextxy(150,0,a);
    outtextxy(200,0,"Time Range:");
    outtextxy(300,0,c);
    Time b,ct;
    ct=getCurrentTime();
    b.hour=(c[0]-48)*10+(c[1]-48);
    b.min=(c[3]-48)*10+(c[4]-48);
    train*t=h->start;
    int i=0;j=0;
    while(t!=NULL)
    {
        station*s=t->start;
        while(s!=NULL)
        {
            if(strcmp(s->sCode,a)==0&&timeComp(s->dep,ct)>=1)
            {
                Time d=timeDifference(s->dep,ct);
                if(timeComp(b,d)>=0)
                {
                    i=1;
                    j++;
                    line(5,30*j-5,getmaxx()-20,30*j-5);
                    line(5,30*j-5,5,30*j+25);
                    sprintf(out,"%s",t->tName);
                    outtextxy(10,30*j,out);
                    sprintf(out,"%s",t->tNo);
                    outtextxy(300,30*j,out);
                    sprintf(out,"Arr- %02d:%02d",s->arr.hour,s->arr.min);
                    outtextxy(400,30*j,out);
                    sprintf(out,"Dep- %2d:%2d",s->dep.hour,s->dep.min);
                    outtextxy(500,30*j,out);
                    line(getmaxx()-20,30*j-5,getmaxx()-20,30*j+25);
                    line(5,30*j+25,getmaxx()-20,30*j+25);
                    break;
                }
            }
            s=s->next;
        }
        t=t->next;
    }
    if(i==0)
    {
        outtextxy(10,300,"No trains in the provided time.\n");
        getch();
    }
    getch();
}
void trainBetweenStations(database *h)
{
    train *t=h->start;
    char from[5],to[5],inp,out[256];
    int i,in=0,j=0,x=0;
    cleardevice();
    outtextxy(0,0,"FROM:");
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)
            break;
        if(inp==8)
        {
            x--;
            in--;
            outtextxy(100+10*x,0,"     ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(100+10*x,0,out);
        from[in++]=inp;
    }
    from[in]='\0';
    x=0;
    outtextxy(170,0,"TO:");
    in=0;
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)
             break;
        if(inp==8)
        {
            x--;
            in--;
            outtextxy(230+10*x,0,"     ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(230+10*x,0,out);
        to[in++]=inp;
    }
    to[in]='\0';
    cleardevice();
    //scanf("%s",to);
    int u=0;
    while(t!=NULL)
    {
        i=0;
        station*s=t->start,*s1;
        while(s!=NULL)
        {
            if(!strcmp(s->sCode,from))
            {
                i++;
                s1=s;
            }
            if(!strcmp(s->sCode,to))
            {
                    if(!i)
                        break;
                    u=1;
                    j++;
                    if(j*90+90>getmaxy())
                    {
                        j=0;
                        in=getch();
                        if(in==54)
                        cleardevice();
                    }
                    line(45,j*90-5,getmaxx()-50,j*90-5);
                    line(45,j*90-5,45,j*90+85);
                    sprintf(out,"Train info :%s",t->tName);
                    outtextxy(50,j*90,out);
                    sprintf(out,"%s",t->tNo);
                    outtextxy(350,j*90,out);
                    sprintf(out,"No of stations in between-%d",((s->sNo)-(s1->sNo)));
                    outtextxy(400,j*90,out);
                    sprintf(out,"%s",s1->sName);
                    outtextxy(50,j*90+30,out);
                    sprintf(out,"Arrival: %02d:%02d",s1->arr.hour,s1->arr.min);
                    outtextxy(250,j*90+30,out);
                    sprintf(out,"Departure: %02d:%02d",s1->dep.hour,s1->dep.min);
                    outtextxy(400,j*90+30,out);
                    sprintf(out,"%s",s->sName);
                    outtextxy(50,j*90+60,out);
                    sprintf(out,"Arrival: %02d:%02d",s->arr.hour,s->arr.min);
                    outtextxy(250,j*90+60,out);
                    sprintf(out,"Departure: %02d:%02d",s->dep.hour,s->dep.min);
                    outtextxy(400,j*90+60,out);
                    line(getmaxx()-50,j*90-5,getmaxx()-50,j*90+85);
                    line(45,j*90+85,getmaxx()-50,j*90+85);
            }
            s=s->next;
        }
        t=t->next;
    }
    if(u==0)
    {
        outtext("          No trains found.");
        getch();
        return;
    }
    outtextxy(50,getmaxy()-20,"Do you want to see schedule ?(Y/N)");
    char choice;
    choice=getch();
    if(choice=='N'||choice=='n')
        return;
    cleardevice();
    GT:outtext("\nChoose the train to see schedule :\n\n");
    Sleep(600);
    x=0;
    char tr[6];
    in=0;
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)break;
        if(inp==8)
        {
            x--;
            in--;
            outtextxy(250+x*10,0,"   ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(250+x*10,0,out);
        tr[in++]=inp;
    }
    tr[in]='\0';
    t=h->start;
    i=0;
    while(t!=NULL)
    {
        if(strcmp(t->tNo,tr)==0)
        {
            i=1;
            break;
        }
        t=t->next;
    }
    if(i==0)
    {
        outtext("\nNo such train. Please Re-enter your choice.\n");
        getch();
        cleardevice();
        goto GT;
    }
    dobara:
    cleardevice();
    sprintf(out,"Train info :\n%s     %s",t->tName,t->tNo);
    outtextxy(70,5,out);
    station*s=t->start;
    i=0;
    while(s!=NULL)
    {
        i++;
        line(5,i*30-5,getmaxx()-35,i*30-5);
        if(i*30+30>getmaxy())
        {
            line(5,25,5,480);
            line(280,25,280,480);
            line(27,25,27,480);
            line(460,25,460,480);
            line(getmaxx()-35,25,getmaxx()-35,480);
            inp=getch();
        if(inp==54)
        {
            i=1;
            cleardevice();
            sprintf(out,"Train info :%s     %s",t->tName,t->tNo);
            outtextxy(70,5,out);
        }
        else
        {
            return;
        }
        }
        sprintf(out,"%2d  %20s  %4s ",s->sNo,s->sName,s->sCode);
        outtextxy(10,i*30,out);
        sprintf(out,"ARR-%02d:%02d   DEP-%02d:%02d   ",s->arr.hour,s->arr.min,s->dep.hour,s->dep.min);
        outtextxy(300,i*30,out);
        sprintf(out,"Dist-%4d \n",s->dist);
        outtextxy(500,i*30,out);
        s=s->next;
        line(5,i*30-5,getmaxx()-35,i*30-5);
    }
    line(5,i*30+30,getmaxx()-35,i*30+30);
    line(5,25,5,i*30+30);
    line(280,25,280,i*30+30);
    line(27,25,27,i*30+30);
    line(460,25,460,i*30+30);
    line(getmaxx()-35,25,getmaxx()-35,i*30+30);
    inp=getch();
    if(inp==52)
        goto dobara;
}
void showSpecialTrains(database*h)
{
    int i=0,j=0,inp;
    char out[256];
    train*t=h->start;
    cleardevice();
    outtextxy(250,0,"Special Trains:");
    while(t!=NULL)
    {
        if(t->tNo[0]=='0')
        {
            j++;
            if(j*30>getmaxy())
            {
                inp=getch();
                if(inp==54)
                {
                    cleardevice();
                    outtextxy(250,0,"Special Trains:");
                    j=1;
                }
            }
            sprintf(out,"%25s",t->tName);
            outtextxy(100,j*30,out);
            sprintf(out,"%s",t->tNo);
            outtextxy(450,j*30,out);
            line(100,j*30-5,500,j*30-5);
            line(100,j*30-5,100,j*30+25);
            line(350,j*30-5,350,j*30+25);
            line(500,j*30-5,500,j*30+25);
            i=1;
        }
        line(100,j*30+25,500,j*30+25);
        t=t->next;
    }
    if(i==0)
        outtext("No special train at the moment.\n\n");
    getch();
}
void admin_lateTrain(database*h)
{
    outtextxy(0,0,"Choose the Train No. to update Late Status :");
    printTrains(h);
    G:
    outtext("Enter Train No. :->");
    int x=0;
    char tr[6],inp,out[256];
    int in=0;
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)break;
        if(inp==8)
        {
            x--;
            in--;
            outtextxy(250+x*10,0,"   ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(250+x*10,0,out);
        tr[in++]=inp;
    }
    tr[in]='\0';
    cleardevice();
    train*t=h->start;
    int i=0;
    while(t!=NULL)
    {
        if(strcmp(t->tNo,tr)==0)
         {
             i=1;
            break;
         }
         t=t->next;
    }
    if(i==0)
    {
        outtext("     Invalid Code. Please Enter again.\n");
        cleardevice();
        goto G;
    }
    station*s=t->start;
    outtextxy(0,100,"Enter the time for which the train is late (in HH:MM format) : ");
    char tm1[6];
    in=0;
    int j=0;
    while(1)
    {
        j++;
        inp=getch();
        if(inp==13)
            break;
        if(inp==8)
        {
            in--;
            j--;
            outtextxy(400+8*j,100,"     ");
            j--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(400+8*j,100,out);
        tm1[in++]=inp;
    }
    tm1[in]='\0';
    Time tm;
    tm.hour=(tm1[0]-48)*10+(tm1[1]-48);
    tm.min=(tm1[3]-48)*10+(tm1[4]-48);
    while(s!=NULL)
    {
        if((s->arr.min+tm.min)>=60)
        {
            s->arr.min=s->arr.min+tm.min-60;
            s->arr.hour=s->arr.hour+tm.hour+1;
        }
        else
        {
            s->arr.min=s->arr.min+tm.min;
            s->arr.hour=s->arr.hour+tm.hour;
        }
        if((s->dep.min+tm.min)>=60)
        {
            s->dep.min=s->dep.min+tm.min-60;
            s->dep.hour=s->dep.hour+tm.hour+1;
        }
        else
        {
            s->dep.min=s->dep.min+tm.min;
            s->dep.hour=s->dep.hour+tm.hour;
        }
        s->arr.hour=s->arr.hour%24;
        s->dep.hour=s->dep.hour%24;
        s=s->next;
    }
    cleardevice();
    outtextxy(0,0,"The updated schedule is :");
    s=t->start;
    i=0;
while(s!=NULL)
    {
        i++;
        line(5,i*30-5,getmaxx()-35,i*30-5);
        if(i*30>getmaxy())
        {
            line(5,25,5,480);
            line(280,25,280,480);
            line(27,25,27,480);
            line(460,25,460,480);
            line(getmaxx()-35,25,getmaxx()-35,480);
            inp=getch();
        if(inp==54)
        {
            i=1;
            cleardevice();
            sprintf(out,"Train info :%s     %s",t->tName,t->tNo);
            outtextxy(70,5,out);
        }
        else
        {
            return;
        }
        }
        sprintf(out,"%2d  %20s  %4s ",s->sNo,s->sName,s->sCode);
        outtextxy(10,i*30,out);
        sprintf(out,"ARR-%02d:%02d   DEP-%02d:%02d   ",s->arr.hour,s->arr.min,s->dep.hour,s->dep.min);
        outtextxy(300,i*30,out);
        sprintf(out,"Dist-%4d \n",s->dist);
        outtextxy(500,i*30,out);
        s=s->next;
        line(5,i*30-5,getmaxx()-35,i*30-5);
    }
    line(5,i*30+30,getmaxx()-35,i*30+30);
    line(5,25,5,i*30+30);
    line(280,25,280,i*30+30);
    line(27,25,27,i*30+30);
    line(460,25,460,i*30+30);
    line(getmaxx()-35,25,getmaxx()-35,i*30+30);
    getch();
}
void admin_cancelTrain(database*h)
{
    cleardevice();
    g:outtext("Choose the train to cancel :\n\n");
    train *t=h->start;
    printTrains(h);
    outtext("\n\nEnter choice :\n->");
    char tr[6],inp,out[256];
        int x=0;
    int in=0;
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)break;
        if(inp==8)
        {
            x--;
            in--;
            outtextxy(250+x*10,0,"   ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(250+x*10,0,out);
        tr[in++]=inp;
    }
    tr[in]='\0';
    int i=0;
    while(t!=NULL)
    {
        if(strcmp(t->tNo,tr)==0)
        {
            i=1;
            break;
        }
        t=t->next;
    }
    if(i==0)
    {
        outtext("Invalid choice. Please re-enter.\n");
        getch();
        goto g;
    }
    train*temp=h->start;
    while(temp!=NULL)
    {
        if(temp->next==t)
        {
            temp->next=t->next;
            free(t);
        }
        temp=temp->next;
    }
    cleardevice();
    outtext("The revised list of trains is :\n");
    Sleep(1000);
    printTrains(h);
    Sleep(1000);
    printf("\n\n");
}
void admin(database*h)
{
    cleardevice();
    int j=1;
    G:
    outtextxy(getmaxx()/2-100,getmaxy()/2-5,"Username : ");
    char uname[30],out[256];
    char pw[20],inp;
    int x=0,in=0;
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)break;
        if(inp==8)
        {
            x--;
            in--;
            outtextxy(getmaxx()/2+x*8,getmaxy()/2-5,"   ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(getmaxx()/2+x*8,getmaxy()/2-5,out);
        uname[in++]=inp;
    }
    uname[in]='\0';
    outtextxy(getmaxx()/2-100,getmaxy()/2+25,"Password : ");
    int i=0;
    x=0,in=0;
    while(1)
    {
        x++;
        inp=getch();
        if(inp==13)break;
        if(inp==8)
        {
            x--;
            in--;
            outtextxy(getmaxx()/2+x*10,getmaxy()/2+25,"   ");
            x--;
            continue;
        }
        sprintf(out,"%c",inp);
        outtextxy(getmaxx()/2+x*10,getmaxy()/2+25,"*");
        pw[in++]=inp;
    }
    pw[in]='\0';
    cleardevice();
    if(strcmp("jiit62",uname)!=0||strcmp("sdflab",pw)!=0)
    {
        if(j==3)
        {
            outtextxy(getmaxx()/2-50,getmaxy()/2-5,"Wrong input 3 times. Login failed.");
            Sleep(100);
            cleardevice();
            return;
        }
        outtextxy(getmaxx()/2-50,getmaxy()/2-5,"Wrong username or password. Please re-enter.");
        Sleep(200);
        cleardevice();
        j++;
        goto G;
    }
    outtextxy(getmaxx()/2-50,getmaxy()/2-5,"Login Successful!!!");
    Sleep(750);
    int choice=49;
    while(choice)
    {
        cleardevice();
        outtextxy(100,30,"Enter your choice");
        outtextxy(100,60,"1: Change train LATE status");
        outtextxy(100,90,"2: Cancel a train");
        outtextxy(100,120,"0: Return to Main Menu");
        choice=getch();
        switch(choice)
        {
        case 49:
            {
                admin_lateTrain(h);
                break;
            }
        case 50:
            {
                admin_cancelTrain(h);
                break;
            }
        case 48:
            {
                return;
            }
        default:
            break;
        }
    }
}
void startprogram()
{
cleardevice();
setcolor(YELLOW);
for(int i=getmaxx(),j=i;i>-800;i-=10,j-=10)
{
    if(j>(-220)){
  outtextxy(190,50,"Welcome to JIIT TRAIN ENQUIRY SYSTEM");}
  else
  {
    outtextxy(190,50,"Welcome to JIIT TRAIN ENQUIRY SYSTEM");}
    line(300+i,150,300+i,300);
    line(390+i,150,390+i,300);
    line(290+i,150,400+i,150);
    line(290+i,145,400+i,145);
    line(290+i,145,290+i,150);
    line(400+i,145,400+i,150);
    line(310+i,160,380+i,160);
    line(310+i,210,380+i,210);
    line(310+i,160,310+i,210);
    line(380+i,160,380+i,210);
    circle(346+i,300,22);
    circle(346+i,300,27);
    line(300+i,300,318+i,300);
    line(390+i,300,375+i,300);
    line(300+i,225,200+i,225);
    line(200+i,225,200+i,300);
    line(200+i,300,215+i,300);
    circle(230+i,300,15);
    circle(230+i,300,10);
    circle(270+i,300,15);
    circle(270+i,300,10);
    line(255+i,300,245+i,300);
    line(285+i,300,305+i,300);
    line(215+i,305,260+i,305);
    line(217+i,309,262+i,309);
    line(215+i,305,217+i,309);
    line(260+i,305,262+i,309);
    line(280+i,225,280+i,205);
    line(265+i,225,265+i,205);
    line(265+i,205,280+i,205);
    ellipse(273+i,205,0,180,7,6);
    line(240+i,225,240+i,210);
    line(230+i,225,230+i,210);
    line(240+i,210,230+i,210);
    ellipse(235+i,210,0,180,5,4);
    line(210+i,225,210+i,185);
    line(215+i,225,215+i,185);
    line(210+i,185,215+i,185);
    line(210+i,185,190+i,150);
    line(215+i,185,235+i,150);
    line(190+i,150,235+i,150);
    line(190+i,150,200+i,140);
    line(235+i,150,225+i,140);
    line(200+i,140,225+i,140);
    line(300+i,150,300+i,300);
    line(390+i,150,390+i,300);
    line(290+i,150,400+i,150);
    line(290+i,145,400+i,145);
    line(290+i,145,290+i,150);
    line(400+i,145,400+i,150);
    line(310+i,160,380+i,160);
    line(310+i,210,380+i,210);
    line(310+i,160,310+i,210);
    line(380+i,160,380+i,210);
    circle(346+i,300,22);
    circle(346+i,300,27);
    line(300+i,300,318+i,300);
    line(390+i,300,375+i,300);
    line(300+i,225,200+i,225);
    line(200+i,225,200+i,300);
    line(200+i,300,215+i,300);
    circle(230+i,300,15);
    circle(230+i,300,10);
    circle(270+i,300,15);
    circle(270+i,300,10);
    line(255+i,300,245+i,300);
    line(285+i,300,305+i,300);
    line(215+i,305,260+i,305);
    line(217+i,309,262+i,309);
    line(215+i,305,217+i,309);
    line(260+i,305,262+i,309);
    line(280+i,225,280+i,205);
    line(265+i,225,265+i,205);
    line(265+i,205,280+i,205);
    ellipse(273+i,205,0,180,7,6);
    line(240+i,225,240+i,210);
    line(230+i,225,230+i,210);
    line(240+i,210,230+i,210);
    ellipse(235+i,210,0,180,5,4);
    line(210+i,225,210+i,185);
    line(215+i,225,215+i,185);
    line(210+i,185,215+i,185);
    line(210+i,185,190+i,150);
    line(215+i,185,235+i,150);
    line(190+i,150,235+i,150);
    line(190+i,150,200+i,140);
    line(235+i,150,225+i,140);
    line(200+i,140,225+i,140);

    line(440+i,150,440+i,300);
    line(700+i,150,700+i,300);
    line(430+i,150,710+i,150);
    line(430+i,145,710+i,145);
    line(430+i,145,430+i,150);
    line(710+i,145,710+i,150);
    circle(518+i,300,27);
    circle(622+i,300,27);
    circle(518+i,300,22);
    circle(622+i,300,22);
    line(440+i,300,491+i,300);
    line(545+i,300,595+i,300);
    line(649+i,300,700+i,300);
    line(453+i,165,513+i,165);
    line(540+i,165,600+i,165);
    line(627+i,165,687+i,165);
    line(453+i,225,513+i,225);
    line(540+i,225,600+i,225);
    line(627+i,225,687+i,225);
    line(453+i,165,453+i,225);
    line(513+i,165,513+i,225);
    line(540+i,165,540+i,225);
    line(600+i,165,600+i,225);
    line(627+i,165,627+i,225);
    line(687+i,165,687+i,225);

    line(390+i,260,440+i,260);
    line(390+i,250,440+i,250);
    if(i<getmaxx()-300){
    outtextxy(200,450,"Loading please wait...");}
    delay(35);
    cleardevice();
}
}
void endprogram()
{
cleardevice();
setcolor(YELLOW);
for(int i=-800;i<getmaxx();i+=10)
{
    outtextxy(250,50,"Please Visit  Again");
    line(300+i,150,300+i,300);
    line(390+i,150,390+i,300);
    line(290+i,150,400+i,150);
    line(290+i,145,400+i,145);
    line(290+i,145,290+i,150);
    line(400+i,145,400+i,150);
    line(310+i,160,380+i,160);
    line(310+i,210,380+i,210);
    line(310+i,160,310+i,210);
    line(380+i,160,380+i,210);
    circle(346+i,300,22);
    circle(346+i,300,27);
    line(300+i,300,318+i,300);
    line(390+i,300,375+i,300);
    line(300+i,225,200+i,225);
    line(200+i,225,200+i,300);
    line(200+i,300,215+i,300);
    circle(230+i,300,15);
    circle(230+i,300,10);
    circle(270+i,300,15);
    circle(270+i,300,10);
    line(255+i,300,245+i,300);
    line(285+i,300,305+i,300);
    line(215+i,305,260+i,305);
    line(217+i,309,262+i,309);
    line(215+i,305,217+i,309);
    line(260+i,305,262+i,309);
    line(280+i,225,280+i,205);
    line(265+i,225,265+i,205);
    line(265+i,205,280+i,205);
    ellipse(273+i,205,0,180,7,6);
    line(240+i,225,240+i,210);
    line(230+i,225,230+i,210);
    line(240+i,210,230+i,210);
    ellipse(235+i,210,0,180,5,4);
    line(210+i,225,210+i,185);
    line(215+i,225,215+i,185);
    line(210+i,185,215+i,185);
    line(210+i,185,190+i,150);
    line(215+i,185,235+i,150);
    line(190+i,150,235+i,150);
    line(190+i,150,200+i,140);
    line(235+i,150,225+i,140);
    line(200+i,140,225+i,140);
    line(300+i,150,300+i,300);
    line(390+i,150,390+i,300);
    line(290+i,150,400+i,150);
    line(290+i,145,400+i,145);
    line(290+i,145,290+i,150);
    line(400+i,145,400+i,150);
    line(310+i,160,380+i,160);
    line(310+i,210,380+i,210);
    line(310+i,160,310+i,210);
    line(380+i,160,380+i,210);
    circle(346+i,300,22);
    circle(346+i,300,27);
    line(300+i,300,318+i,300);
    line(390+i,300,375+i,300);
    line(300+i,225,200+i,225);
    line(200+i,225,200+i,300);
    line(200+i,300,215+i,300);
    circle(230+i,300,15);
    circle(230+i,300,10);
    circle(270+i,300,15);
    circle(270+i,300,10);
    line(255+i,300,245+i,300);
    line(285+i,300,305+i,300);
    line(215+i,305,260+i,305);
    line(217+i,309,262+i,309);
    line(215+i,305,217+i,309);
    line(260+i,305,262+i,309);
    line(280+i,225,280+i,205);
    line(265+i,225,265+i,205);
    line(265+i,205,280+i,205);
    ellipse(273+i,205,0,180,7,6);
    line(240+i,225,240+i,210);
    line(230+i,225,230+i,210);
    line(240+i,210,230+i,210);
    ellipse(235+i,210,0,180,5,4);
    line(210+i,225,210+i,185);
    line(215+i,225,215+i,185);
    line(210+i,185,215+i,185);
    line(210+i,185,190+i,150);
    line(215+i,185,235+i,150);
    line(190+i,150,235+i,150);
    line(190+i,150,200+i,140);
    line(235+i,150,225+i,140);
    line(200+i,140,225+i,140);

    line(440+i,150,440+i,300);
    line(700+i,150,700+i,300);
    line(430+i,150,710+i,150);
    line(430+i,145,710+i,145);
    line(430+i,145,430+i,150);
    line(710+i,145,710+i,150);
    circle(518+i,300,27);
    circle(622+i,300,27);
    circle(518+i,300,22);
    circle(622+i,300,22);
    line(440+i,300,491+i,300);
    line(545+i,300,595+i,300);
    line(649+i,300,700+i,300);
    line(453+i,165,513+i,165);
    line(540+i,165,600+i,165);
    line(627+i,165,687+i,165);
    line(453+i,225,513+i,225);
    line(540+i,225,600+i,225);
    line(627+i,225,687+i,225);
    line(453+i,165,453+i,225);
    line(513+i,165,513+i,225);
    line(540+i,165,540+i,225);
    line(600+i,165,600+i,225);
    line(627+i,165,627+i,225);
    line(687+i,165,687+i,225);

    line(390+i,260,440+i,260);
    line(390+i,250,440+i,250);
    outtextxy(200,450,"Shutting Down Please wait...");
    delay(35);
    cleardevice();
}
}
int main()
{
    int gd = DETECT,gm;
    initgraph(&gd,&gm ,"");
    setbkcolor(BLUE);
    cleardevice();
    startprogram();
    database *h=initDB();
    h=loadDB(h);
    int ch=49;
    while(ch!=48 )
    {
        cleardevice();
        outtextxy(100,30,"\n\n\nEnter your choice\n");
        outtextxy(100,60,"1: Display all trains");
        outtextxy(100,90,"2: Spot your train");
        outtextxy(100,120,"3: Display train schedule");
        outtextxy(100,150,"4: Display everything");
        outtextxy(100,180,"5: Train Between Stations");
        outtextxy(100,210,"6: Live Station");
        outtextxy(100,240,"7: Show all stations");
        outtextxy(100,270,"8: Show Special Trains");
        outtextxy(100,300,"9: Admin Login");
        outtextxy(100,330,"0: Quit the program");
        ch=getch();
        switch(ch)
        {
            case 48:
            {
                endprogram();
                break;
            }
            case 49:
                {
                    printTrains(h);
                    break;
                }
            case 50:
                {
                    spotTrain(h);
                    break;
                }
            case 51:
                {
                    displayTrain(h);
                    break;
                }
            case 52:
                {
                    display(h);
                    break;
                }
            case 53:
                {
                    trainBetweenStations(h);
                    break;
                }
            case 54:
                {
                    liveStation(h);
                    break;
                }
            case 55:
                {
                    printAllStations();
                    break;
                }
            case 56:
                {
                    showSpecialTrains(h);
                    break;
                }
            case 57:
                {
                    admin(h);
                    break;
                }
            default :
                {
                    break;
                }
        }
    }
    closegraph();
    return 0;
}

