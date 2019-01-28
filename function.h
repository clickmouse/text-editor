#include<stdio.h>

//表示一行的数据节点
typedef struct row_node
{
    char *string;         //一行中所含内容
    int row_pos;           //该行所在行数
    struct row_node *next;//下一个节点
    struct row_node *last;//上一个节点
    int flag;
}ROW;

ROW *Open_File(char *filename);				//打开一个文件
ROW *Read_row(FILE *fp ,char *filename);	//读取每行内容 
void Output(ROW* head, int direct);			//打印链表字符串内容 
void Guidance(ROW* head);					//帮助栏 
void Direction_move(int choice1,ROW* head); //坐标移动 
ROW* Delete_row(ROW* head);					//dd删除当前行 
ROW* Free_list(ROW* head);					//将被删除的行从链表中删除 
ROW* Newline_bel(ROW* head);				//P功能，向下创建一行 
ROW* Newline_abv(ROW* head);				//p功能向上创建一行 
void Change_byte(ROW* head);				//更换当前坐标下的字符 
ROW* Move_abv(ROW* head);					//向上移动一行
ROW* Move_bel(ROW* head);					//向下移动函数
void Free_All(ROW* head);					//打开新文件，将旧文件所有链表节点释放，并关闭文件
void Save(ROW* head);						//命令行，将当前所有链表节点保存至一文件
ROW* Move_free(ROW* head);					//将某行移动至某行
ROW* Insert(ROW* head, int choice1);		//插入功能 
void Move_next(char lose, ROW*p);			//将溢出字符插入下一行行首 
