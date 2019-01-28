#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include"struct function.h"

int cx,cy;  //横纵坐标 
int page;   //当前页码 
char filename[50];  //文档名称 
FILE *fp;   //文件指针 

void GotoPos(int x, int y)  //设置光标位置
{
    COORD loc = {(short)x, (short)y};
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput,loc);
}

int main(int argc, char const *argv[])
{
    printf("Welcome To My Text\n");

    int choice1;
    int choice2;
    int choice3;   //用于判断 
    int i,k;	   //循环变量 
    ROW *head;		//链表头指针 
    ROW *p;			//链表某节点指针 
    int count;		//计数 
    head = Open_File(filename);
    while(head==NULL){
        printf("Please Enter The Filename Again\n");
        head = Open_File(filename);  //打开一个文件 
    }
    while((choice1 = getch())!='q'){	//q为退出 
        if(choice1>'0'&&choice1<='9')	//判断是否为数字 
        {
            if((choice2 = getch())=='d')
            {
                if((choice3 = getch())=='d')	//  <N>dd功能 
                {
                    for(p=head;p->row_pos!=cy+20*(page-1)-2;p=p->next);
                    count=p->row_pos;
                    while(p->next!=NULL)
                        p=p->next;
                    count=p->row_pos-count+1;	//计算删除几行 
            		for(i=1;i<=choice1-'0';i++)
                    {
                        if(i>count)
                            break;
                        head = Delete_row(head);	//dd功能 
                    }
                }
            }
            else if(choice2=='p')
            {
                for(i=1;i<=choice1-'0';i++)		//p功能 
                    head = Move_abv(head);
            }
            else if(choice2=='P')				//P功能 
            {
                for(i=1;i<=choice1-'0';i++)
                    head = Move_bel(head);
            }
        }
        else if(choice1=='d'){					//dd功能 
            if((choice2 = getch())=='d')
                head = Delete_row(head);
        }
        else if(choice1==':'){					//命令行模式 
            do{
                GotoPos(0,25);
                printf("Command Mode:");
                choice2 = getch();
                if(choice2!=27)
                	printf("%c",choice2);
                if(choice2=='o'){				//打开新文件   :o功能 
                    Free_All(head);				//关闭上一文件 
                    system("cls");
                    head = Open_File(filename);
                    break;
                }
                else if(choice2=='w'){			//保存当前文件  :w功能 
                    Save(head);
                    system("pause");
                    for(i=1;i<=3;i++){
                    GotoPos(0,25+i);
                    for(k=0;k<30;k++)
                        printf(" ");
                    }
                }
                else if(choice2=='m'){			//移动某行到某行  :m<N><N>功能 
                    head = Move_free(head);
                }
                GotoPos(13,25);
                putchar(32);
            } while(choice2!=27);		//ESC退出命令行 
            GotoPos(0,25);
            for(i=0;i<15;i++)
                printf(" ");
            GotoPos(cx,cy);
        }
        else{
            if(choice1=='h'||choice1=='j'||choice1=='k'||choice1=='l')			//移动坐标 
                Direction_move(choice1,head);
            else if(choice1=='n'){												//向后翻页 
                Output(head,1);
                GotoPos(30,25);
    			printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
                cx=4,cy=3;
                GotoPos(4,3);
            }
            else if(choice1=='m'){												//向前翻页 
                Output(head,-1);
                GotoPos(30,25);
    			printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
                cx=4,cy=3;
                GotoPos(4,3);
            }
            else if(choice1=='o')			//o功能  
                head = Newline_bel(head);
            else if(choice1=='O')			//O功能 
                head = Newline_abv(head);
            else if(choice1=='c')			//c功能 
                Change_byte(head);
            else if(choice1=='p')			//p功能 
                Move_abv(head);
            else if(choice1=='P')			//P功能 
                Move_bel(head);
            else if(choice1=='i')			//i功能 
                head = Insert(head, choice1);
            else if(choice1=='a')			//a功能 
                head = Insert(head, choice1);
            else if(choice1=='z')			//打开帮助 
                Guidance(head);
        }
	}
    return 0;
}

void Guidance(ROW* head){    					//帮助栏 
    int choice;
    system("cls");
    system("cls");
	printf("				欢迎使用行编辑器\n");
	printf("-----------------------------------------------------------------------\n");
	printf("				移动命令：\n");
	printf("j    向下移动\n");
	printf("k    向上移动\n");
	printf("h    向左移动\n");
	printf("l    向右移动\n");
	printf("-----------------------------------------------------------------------\n");
	printf("				编辑命令：\n");
	printf("dd   	删除当前行\n");
	printf("<N>dd   删除当前行开始的N行\n");
	printf("o    	在当前行的后面插入一个新行\n");
	printf("O    	在当前行的前面插入一个新行\n");
	printf("i    	输入文本，并插入到当前位置的内容之前，按ESC结束\n");
	printf("a    	输入文本，并添加到当前位置的内容之后，按ESC结束\n");
	printf("c    	开始输入并修改对应的文本，直到按ESC结束\n");
	printf("p    	将当前行往前移动1行\n");
	printf("<N>p    将当前行往前移动N行\n");
	printf("P    	将当前行往后移动1行\n");
	printf("<N>P    将当前行往后移动N行\n");
	printf("n    	向后翻页\n");
	printf("m    	向前翻页\n");
	printf("-----------------------------------------------------------------------\n");
	printf("				命令行指令：\n");
	printf("m <被移动的行号> <目的行号>		移动行\n");
	printf("o <打开的文件名称>			打开文件\n"); 
	printf("w <写入的文件名称>			保存文件\n");
	printf("-----------------------------------------------------------------------\n");		
    printf("Come Back To THE File, Enter 'b'");
    while((choice=getch())!='b');
    Output(head,0);
    GotoPos(30,25);
    printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
    GotoPos(cx,cy);
}

ROW *Open_File(char *filename)			//打开一个文件 
{
    ROW* head;
	system("color 79");					//更改控制台颜色 
	GotoPos(0,1);
    printf("Please Enter The Name Of The File\n");
    scanf("%s", filename);
    while((fp = fopen(filename, "r"))== NULL)
    {
        printf("File Open Error\n");
        printf("Please Check And Enter Again!\n");
        scanf("%s", filename);
    }
    system("cls");
    head = Read_row(fp, filename);			//调用读取每行内容函数 
    if(head==NULL){
        printf("NO FILE !\n");
        return head;
    }
    page = 1;								//确定页码为1 
    Output(head,0);
    cx=4;cy=3;
    GotoPos(30,25);
    printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
    GotoPos(4,3);
    return head;
}

ROW *Read_row(FILE *fp ,char *filename)			//读取每行内容 
{
    int i;
    char row[26];
    char *temp;
    char ch;
    int row_pos;
    ROW *head,*tail,*p;
    head=tail=NULL;
	row_pos=1;
    while(1){
        ch=fgetc(fp);				//用于判断是否读到文件末 
        if(ch==EOF)
            break;
        fseek(fp, -1L, SEEK_CUR);	//回到读入前 

        if(!fgets(row,26,fp))
            break;
        for(i=0;row[i]!='\n'&&row[i]!='\0';i++);
        row[i]='\0';
        temp=(char *)malloc(26);		//申请一块空间 
        strcpy(temp,row);
        if(head==NULL){
            head = (ROW*)malloc(sizeof(ROW));		//建立双向链表 
            head->string = temp;
            head->row_pos = row_pos;
            head->next = NULL;
            head->last = NULL;
            head->flag=1;
            tail=head;
        }
        else {
            p=(ROW*)malloc(sizeof(ROW));
            p->string = temp;
            p->row_pos = row_pos;
            p->next = NULL;
            p->last = tail;
            p->flag=1;
            tail->next = p;
            tail = p;
        }
        row_pos++;					//将行号+1 
    }
    return head;
}

void Output(ROW* head, int direct)
{
    int i;
    ROW*p;
    page+=direct;		//更改当前页码 
    if(page<1){
    	page++;
        return;
	}
    for(p=head;p->next!=NULL;p=p->next);
    if((page-1)*20>=p->row_pos){	//若为第一页或最后一页则不翻页 
     	page--;
        return;
	}
    system("cls");
    int flen = strlen(filename);
    GotoPos(0,0);
    for(i=0;i<49;i++){                  //在第一行打印出文件名 
        putchar('-');
        if(i+flen/2==24){
            printf("%s",filename);
            i+=flen;
        }
    }
    printf("\n\n");
    for(i=0;i<49;i++)
        printf("-");
    printf("\n");
    p=head;
    if(page!=1){
        while(p!=NULL){
            if(p->row_pos==20*(page-1)+1)		//重新定位是第几页第一行所对应的链表节点 
                break;
            p=p->next;
        }
    }
    for(;p!=NULL&&p->row_pos!=20*page+1;p=p->next){
    	if(p->row_pos>9)
        	printf("%d| ", p->row_pos);				//将该页内容打印 
        else
        	printf("%d | ", p->row_pos);
            puts(p->string);
    }
    for(i=0;i<49;i++)
        printf("-");
    for(i=0;i<8;i++)
        printf(" ");
    printf("If Need Help Please Enter 'z'");
}

void Direction_move(int choice1, ROW*head)		//坐标移动 
{
    ROW* p;
    ROW* q;
    int endline;		//最后一行 
    int endcol;			//最后一列 
    int count=0;
    int jump_be = -1;	//是否向下直接跳转 
    int jump_ab = -1;	//是否向上直接跳转 
    p=head;
    if(page!=1){
        while(p!=NULL){
            if(p->row_pos==20*(page-1)+1)		//定位当前坐标的链表节点 
                break;
            p=p->next;
        }
    }
    for(q=p;q->row_pos!=cy-2+20*(page-1);q=q->next);
        endcol = strlen(q->string)+3;		//计算本行最后一个字符位置 
    if(q->next!=NULL){
        if(cx-3>strlen(q->next->string))
            jump_be = strlen(q->next->string);//判断是否能直接跳转到下一行 
    }
    if(q->last!=NULL){
        if(cx-3>strlen(q->last->string))
            jump_ab = strlen(q->last->string);//判断是否能直接跳转到上一行 
    }
    for(;p!=NULL&&count<20; p=p->next){
    	count++;
    }
    endline = count+2;

    switch(choice1){
        case 'j':			//向下移动 
            if(cy==endline)
               break;
            else{
                if(jump_be!=-1){
                    cx = jump_be+3;
					if(jump_be==0) cx++;
					}
                cy++;  
            }
            break;
        case 'k':		//向下移动 
            if(cy==3)
                break;
            else{
                if(jump_ab!=-1){
                    cx = jump_ab+3;
                    if(jump_ab==0) cx++;
                }
                cy--;
            }
            break;
        case 'h':		//向左移动 
            if(cx==4)
                break;
            else
                cx--;
            break;
        case 'l':		//向右移动 
        	if(cx==endcol)
        		break;
        	else
                cx++;
            break;
    }
    GotoPos(30,25);
    printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);		//显示当前坐标 
    GotoPos(cx,cy);
}

ROW* Delete_row(ROW* head)		//dd删除当前行 
{
    int i;
    int temp_y=cy;		//暂存当前行坐标 
    int save=0;
    ROW* p;
    ROW* q;   
    p=head;
    if(page!=1){
        while(p!=NULL){
            if(p->row_pos==20*(page-1)+1)		//定位当前页码的首节点 
                break;
            p=p->next;
        }
    }
    for(;p!=NULL;p=p->next){					//定位当前坐标对应的节点 
        if(p->row_pos==temp_y+20*(page-1)-2)
            break;
    }
    if(p==NULL)
        return head;
    p->flag=0;									//标记本行为要删除行 
    if(p->row_pos==1)	
        if(p->next==NULL||p->next->flag==0)		//判断是否只剩下最后一行 
            save=1;
    if(p->next==NULL){							//如果本行是最后一行 
    	if(p->row_pos==1)
    		GotoPos(4,temp_y);					//擦除当前行 
    	else
        	GotoPos(0,temp_y);
        for (i = 0; i < 30; ++i)
            printf(" ");
        
        if(cy+20*(page-1)!=3){
        	if(cy+20*(page-1)==3+(page-1)*20){
            	Output(head,-1);
            	GotoPos(30,25);								//非第一页的第一行需要向前翻页			 
    			printf("row:%d  col:%d  ", page*20,cx-3);
    			cy=22;
            	GotoPos(4,22);
        	}
            else{
        		cy=temp_y-1;
        		GotoPos(30,25);
        		printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
				GotoPos(4,temp_y-1);
			}
        }
    	else
    		GotoPos(cx,cy);
        return Free_list(head);		//将删除的行所在节点删除 
    }
    p=p->next;
	if(p==NULL){
        if(save==0)
            GotoPos(0,temp_y);
        else
            GotoPos(4,temp_y);
        for (i = 0; i < 30; ++i)		//删除当前行 ，情况与上相同，但可能为第一页第一行 
            printf(" ");
        if(cy+20*(page-1)!=3){
			if(cy+20*(page-1)==3+(page-1)*20){
            	Output(head,-1);
            	GotoPos(30,25);
    			printf("row:%d  col:%d  ", page*20,cx-3);
    			cy=22;
            	GotoPos(4,22);
        	}
            else{
        		cy=temp_y-1;
        		GotoPos(30,25);
        		printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
				GotoPos(4,temp_y-1);
			}	
    	}
    	else
    		GotoPos(cx,cy);
        return Free_list(head);
	}
    for(q=p;q!=NULL;q=q->next)
        q->row_pos-=1;
    while(p!=NULL&&p->row_pos<=20*page){		//删除普通位置一行 
        GotoPos(0,temp_y);
        for(i=0;i<30;i++)
            printf(" ");
        GotoPos(0,temp_y);
        if(p->row_pos>9)
            printf("%d| ", p->row_pos);			//将本行下面行数全部上移输出 
        else
            printf("%d | ", p->row_pos);
        puts(p->string);
        temp_y++;
        p=p->next;
    }
    if(p==NULL){
        GotoPos(0,temp_y);
        for(i=0;i<30;i++)
            printf(" ");
    }
    GotoPos(30,25);
    printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);	//重新定位行数列数 
    GotoPos(cx,cy);
    return Free_list(head);
}

ROW* Free_list(ROW* head)     		//将被删除的行从链表中删除 
{
    ROW* p;
    ROW* q;
    ROW* r;
    if(head==NULL)
        return NULL;
    p=head;
    q=head->next;
    if(head->flag==0){				//删除头节点 
        head=q;
        if(head!=NULL)
            head->last=NULL;
        free(p);
        return head;
    }
    while(p!=NULL){
        if(p->flag==0)
            break;
        p=p->next;
    }
    if(p!=NULL){					//删除普通节点 
        q=p->next;
        r=p->last;
        r->next=q;
        if(q!=NULL)
            q->last=r;
        free(p);
    }
    return head;
}

ROW* Newline_bel(ROW* head)		//P功能，向下创建一行 
{
    ROW* p;
    ROW* q;
    ROW* r;
    char row[26];
    char* temp;
    if(head==NULL)
        return NULL;
    for(p = head; p->row_pos!=20*(page-1)+cy-2; p=p->next);		//定位当前坐标的链表节点 
    q = p->next;
    if(q==NULL){			//若本行为最后一行 
        row[0] = '\0';
        temp = (char*)malloc(26);		//为下一行申请空间链接到上一行 
        strcpy(temp,row);
        q=(ROW*)malloc(sizeof(ROW));	
        q->string = temp;
        q->row_pos = p->row_pos+1;
        q->next = NULL;
        q->last = p;
        p->next = q;
        q->flag = 1;
        r=q;
    }
    else{								//本行非最后一行 
        row[0] = '\0';
        temp = (char*)malloc(26);		//为下一行申请空间 
        strcpy(temp,row);
        r=(ROW*)malloc(sizeof(ROW));
        r->string = temp;
        r->row_pos = p->row_pos+1;		//将其链接在两行之间 
        r->last = p;
        r->next = q;
        p->next = r;
        q->last = r;
        r->flag = 1;
        for(;q!=NULL;q=q->next)
            q->row_pos++;
    }
    if(r->row_pos>20*page){				//如果创建大于本页显示则跳转到下一页 
        Output(head,1);
        cy = 3;
        GotoPos(30,25);
        printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
        GotoPos(4,cy);
    }
    else{								//不跳转 
        Output(head,0);
        cy = r->row_pos+2-20*(page-1);
        cx=4;
        GotoPos(30,25);
        printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
        GotoPos(4,cy);
    }
    return head;
}

ROW* Newline_abv(ROW* head)			//p功能向上创建一行 
{
    ROW* p;
    ROW* q;
    ROW* r;
    char row[26];
    char* temp;
    if(head==NULL)
        return NULL;
    for(p = head; p->row_pos!=20*(page-1)+cy-2; p=p->next);		//定位当前坐标对应的链表节点 
    if(p==head){
        row[0] = '\0';								//若为头节点，则向上创建一行，并将头节点更换至上一行 
        temp = (char*)malloc(26);
        strcpy(temp,row);
        p=(ROW*)malloc(sizeof(ROW));				//其余操作与上一函数相同 
        p->string = temp;
        p->row_pos = 1;
        p->next = head;
        p->last = NULL;
        p->flag = 1;
        head->last=p;
        head = p;
        for(p = head->next;p!=NULL;p = p->next)
            p->row_pos++;
        r=head;
    }
    else{							//非头节点，向上创建一行，并将其链接在两节点之间 
        q = p->last;
        row[0] = '\0';
        temp = (char*)malloc(26);
        strcpy(temp,row);
        r=(ROW*)malloc(sizeof(ROW));
        r->string = temp;
        r->row_pos = p->row_pos;
        r->next = p;
        r->last = q;
        p->last = r;
        q->next = r;
        r->flag = 1;
        for(;p!=NULL;p = p->next)
            p->row_pos++;
    }
    Output(head,0);
    cy = r->row_pos+2-20*(page-1);
    GotoPos(30,25);
    printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);			//输出当前行列 
    cx=4;
    GotoPos(4,cy);
    return head;
}

void Change_byte(ROW* head)		//更换当前坐标下的字符 
{
    char byte;
    ROW* p;
    int i;
    int temp_x = cx;
    int temp_y = cy;
    for(p = head;p->row_pos<20*(page-1)+temp_y-2;p=p->next);		//定位当前坐标下的节点 
    while((byte=getch())!=27){	//读入将要更改的字符 
        i=temp_x-4;
        if(p->string[i]=='\0')
            continue;
        p->string[i] = byte;	//修改当前坐标下的字符 
        Output(head,0);
        GotoPos(30,25);
        printf("row:%d  col:%d  ", temp_y+20*(page-1)-2,cx-3);
        temp_x++;				//移动至下一字符 
        GotoPos(temp_x,temp_y);
        if(temp_x-3>strlen(p->string)){
            temp_x--;
            GotoPos(temp_x,temp_y);
        }
    }
    cx = temp_x;
    cy = temp_y;
}

ROW* Insert(ROW* head, int choice1)		//插入功能 
{
    char byte;
    ROW* p;
    int i;
    int ins;
    int pos_0;
    char lose;
    GotoPos(0,25);
    printf("Insert  Mode");				//显示处于插入模式 
    GotoPos(cx,cy);
    for(p = head;p->row_pos<cy-2+20*(page-1);p=p->next);
    while((byte=getch())!=27)
	{   
        if(byte=='\n'||byte=='\t')		//插入模式下不使用回车及制表 
            continue;   
        if(choice1=='i')
            ins=cx-4;					//i与a判断向前插入还是向后插入 
        else if(choice1=='a')
            ins=cx-3;
        if(byte=='\b'){					//退格键可以删除当前字符 
            for(i=ins;p->string[i]!='\0';i++)
                p->string[i] = p->string[i+1];
            Output(head,0);
            GotoPos(0,25);
            printf("Insert  Mode");
            if(cx!=4)
                GotoPos(--cx,cy);
            else
                GotoPos(4,cy);
            continue;
        }
        pos_0 = strlen(p->string);		//以下为判断是否有溢出 
        if(pos_0<25){
        	if(choice1=='a'&&pos_0==0){
        		p->string[0]=' ';		//无溢出则之间进行插入 
        		pos_0++;
        	}
            p->string[pos_0+1] = '\0';
            for(i = pos_0;i>ins;i--)
                p->string[i] = p->string[i-1];
            p->string[ins] = byte;
        }
        else{
            lose = p->string[pos_0-1];			//有溢出则记录溢出字符 
            Move_next(lose,p);					//调用溢出移动函数，将其移动至下一行 
            for(i=pos_0-1;i>ins;i--)
                p->string[i] = p->string[i-1];
            p->string[ins] = byte;
        }
		Output(head,0);
        GotoPos(0,25);
        printf("Insert  Mode");
        if(choice1=='i')
            i=3;
        else if(choice1=='a')
            i=2;
        if(cx-i<25){
            GotoPos(++cx,cy);		//判断插入到最后一个字符是否跳转 
		}
        else{
        	cx = 4;
            if(cy!=22)
                GotoPos(4,++cy);
            else{
                cy=3;
                Output(head,1);
                GotoPos(30,25);
                printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
                GotoPos(4,3);
            }
            p = p->next;
            choice1='i';
        }
    }
    GotoPos(0,25);
    for(i=0;i<15;i++)
        printf(" ");
    GotoPos(30,25);
    printf("row:%d  col:%d  ", cy+20*(page-1)-2,cx-3);
    GotoPos(cx,cy);
    return head;
}


void Move_next(char lose, ROW* last)		//将溢出字符插入下一行行首 
{
    char temp_out[27];
    ROW* p = last->next;
    char* temp; 
    temp_out[0] = lose;
    temp_out[1] = '\0';
    if(p!=NULL){
        strcat(temp_out,p->string);			//将溢出字符与下一行链接 
        if(strlen(temp_out)>25){
            lose = temp_out[25];
            temp_out[25] = '\0';
            Move_next(lose,p);				//如仍然溢出，则递归调用该函数 
        }
        strcpy(p->string,temp_out);
    }
    else{
        temp = (char*)malloc(26);			//否则在本行插入后结束 
        strcpy(temp,temp_out);
        p=(ROW*)malloc(sizeof(ROW));
        p->string = temp;
        p->row_pos = last->row_pos+1;
        p->last = last;
        p->next = NULL;
        last->next = p;
        p->flag = 1;
    }
    return;
}

ROW* Move_abv(ROW* head)				//向上移动一行 
{
    ROW* p;
    ROW* q;
    char temp[26];
    for(p=head;p->row_pos<cy-2+20*(page-1);p=p->next);		//定位坐标所在节点 
    if(p->last==NULL)		//无上一行 
        return head;
    q = p->last;
    strcpy(temp,p->string);				//将本行与上一行字符串进行交换 
    strcpy(p->string,q->string);
    strcpy(q->string,temp);
    if(cy-2!=1){
        Output(head,0);
        GotoPos(30,25);
    	printf("row:%d  col:%d  ", cy+20*(page-1)-3,cx-3);		//判断向上后是否需要翻页 
        GotoPos(cx,--cy);
    }
    else{
        Output(head,-1);
        GotoPos(30,25);
    	printf("row:%d  col:%d  ", 20*page,cx-3);
    	cy=22;
        GotoPos(cx,22);
    }
    return head;
}

ROW* Move_bel(ROW* head)		//向下移动函数 
{
    ROW* p;
    ROW* q;
    char temp[26];
    for(p=head;p->row_pos<cy-2+20*(page-1);p=p->next);		//定位坐标所在节点 
    if(p->next==NULL)//无下一行
        return head;
    q = p->next;
    strcpy(temp,p->string);
    strcpy(p->string,q->string);				//交换本行与下一行字符串 
    strcpy(q->string,temp);
    if(cy-2!=20){
        Output(head,0);
        GotoPos(30,25);
        printf("row:%d  col:%d  ", cy+20*(page-1)-1,cx-3);
        GotoPos(cx,++cy);
    }
    else{								//判断是否需要翻页 
        Output(head,1);
        GotoPos(30,25);
        printf("row:%d  col:%d  ", 1+20*(page-1),cx-3);
        cy=3;
        GotoPos(cx,3);
    }
    return head;   
}

void Free_All(ROW* head)		//打开新文件，将旧文件所有链表节点释放，并关闭文件 
{
    ROW* p;
    while(head!=NULL){
        p = head;
        head=head->next;
        free(p); 
    }
    fclose(fp);
}

void Save(ROW* head)			//命令行，将当前所有链表节点保存至一函数 
{
    ROW* p;
    FILE* fp;
    char NEWfile[50];
    scanf("%s", NEWfile);
    while((fp = fopen(NEWfile, "w+"))== NULL){
        printf("Error!\n");
        printf("Enter The Filename!\n");
        scanf("%s", NEWfile);
    }
    for(p = head; p!=NULL; p=p->next){
        fputs(p->string,fp);
        fputc('\n',fp);
    }
    printf("FILE SAVE SUCCESSFULLY!\n");
}

ROW* Move_free(ROW* head)		//将某行移动至某行 
{
    int now;
    int des;
    int MAX;
    int i;
    int k;
    int jump;
    ROW* p;
    for(p = head;p->next!=NULL;p=p->next);
    MAX = p->row_pos;			//判断最大行 
    p = head;
    do{
        scanf("%d", &now);
        if(now>MAX||now<1){
            printf("THE Now Position not exists Enter Again");		//判断用户所输行数是否存在 
            GotoPos(0,26);
            for(i=0;i<6;i++)
                printf(" ");
            GotoPos(0,27);
            for(i=0;i<40;i++)
                printf(" ");
            GotoPos(0,26);
        }
    }while(now>MAX||now<1);
    do{
        scanf("%d", &des);
        if(des>MAX||des<1){						
            printf("THE destination not exists");					//判断用户所属行数是否存在 
            GotoPos(0,27);
            for(k=0;k<3;k++)
                printf(" ");
            GotoPos(0,28);
            for(k=0;k<30;k++)
                printf(" ");
            GotoPos(0,27);
        }
    }while(des>MAX||des<1);
    jump = des-now;				//计算需要跳转多少次 
    if(jump==0)
        return head;
	for(p = head;p->row_pos<now;p=p->next);
	cy = p->row_pos+2-20*(page-1);
    if(jump>0){					//向下跳转 
        for(i=1;i<=jump;i++)
            head = Move_bel(head);
    }
    else{
        jump=abs(jump);			//向上跳转 
        for(i=1;i<=jump;i++)
            head = Move_abv(head);
    }
    return head;
}
