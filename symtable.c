#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NAME_LEN 20

/* 符号表结构体*/
typedef struct SymbolTable
{
    char VarName[NAME_LEN]; //变量名
    char value[NAME_LEN];
    char type[20];              //变量类型
    int size;                   //数据存储长度
    int addr;                   //内存首地址
} SymbolTable_t;

/*顺序表结构体*/
typedef struct SqList
{
    SymbolTable_t *elem; //存储空间的首地址
    int length;          //符号表中当前数据类型的个数
} SqList_t;

SqList_t L; //全局声明，C语言无引用，故不能直接在常熟列表改变 L的值 
int InitList();                                         //初始化顺序表
char* StrSplit(char* ori, char* des, char* separator, int index);//分割字符串
int SymbolTableInfo(char *string,int index);            //解析输入的字符串
void WriteInfo(char *string,int size,int index);        //size表示分配字节大小，index为截取的下标
void Assignment(char *varname);                         //变量的赋值
void SelectVar(char *varname);                          //变量的取值
int Menu(int );                                         //选项菜单
void Display(SqList_t L);                               //打印符号表信息

/*初始化顺序表*/
int InitList()
{
    L.elem = (SymbolTable_t *)malloc(sizeof(SymbolTable_t)*100);
    if(!L.elem)
        exit(0);
    L.length = 0;
    return 0;
}

char *Test_strpbrk(const char *cs, const char *ct)
{
    const char *sc1 = cs, *sc2;

    for (; *sc1 != '\0'; ++sc1) {
        for (sc2 = ct; *sc2 != '\0'; ++sc2) {
            if (*sc1 == *sc2)
                return (char *)sc1;
        }
    }
    return NULL;
}

char *strsep(char **s, const char *ct)
{
    char *sbegin = *s;
    char *end;

    if (sbegin == NULL)
        return NULL;

    end = Test_strpbrk(sbegin, ct);
    if (end)
        *end++ = '\0';
    *s = end;
    return sbegin;
}

//函数功能是不改变源串(ori)的前提下,取出以指定字符分隔的第index个子串,放到目标变量(des)中
char* StrSplit(char* ori, char* des, char* separator, int index) //ori can not change 
{ 
    char *token = NULL;
    char temp[32];
	strcpy(temp, ori);
	//如果用malloc自己申请内存
	//char *pStrTemp = (char*)malloc(strlen(ori) + 1);//一定要加1,最后有个'\0'
	//memcpy(pStrTemp, ori, strlen(ori) + 1);//把最后的'\0'也拷贝过来
	//free(pStrTemp);//使用完了记得释放
	token = strtok(temp, separator);
    while(token != NULL)
    {
        if(index-- <= 0)
            break;
		token = strtok(NULL, separator);
    }

    if(index <= 0 && token != NULL)
        strcpy(des, token);
	else
		strcpy(des, "");
    
    return des;   
}


/*解析输入的字符串*/
int SymbolTableInfo(char *string,int index)
{
    char *split=" ='[](){};";
    char arraysize[10];//接收数组大小 
    char datatype[20];//用来保存数据类型，判断变量声明时，数据类型关键字是否在变量名之前
    int flag=0; //flag为1则表示未添加变量
    StrSplit(string,datatype,split,0);
    //判断关键字及变量声明末尾的分号是否在输入的字符串中,否则报错
    if (!strstr(string,"int")&&!strstr(string,"float")&&!strstr(string,"char")&&!strstr(string,"double")&&!strstr(string,"void"))
    {
        printf("There is no data type !\n");
        printf("Please input with correct form !\n");
    }
    //判断变量声明末尾的分号是否在输入的字符串中,否则报错
    else if(!strstr(string,";"))
    {
        printf("Missed \';\'\n");
    }
    //如果数据类型关键字不在变量名之前出现，则报错
    else if((strcmp(datatype,"int")!=0)&&(strcmp(datatype,"float")!=0)&&(strcmp(datatype,"char")!=0)&&(strcmp(datatype,"double")!=0)
                                 &&(strcmp(datatype,"void")!=0))
    {
        printf("This variable is not define !\n");
        printf("Please input with correct form !\n");
    }
    else if(strstr(string,"int"))
    {
        WriteInfo(string,4,index);
    }
    else if(strstr(string,"float"))
    {
        WriteInfo(string,4,index);
    }
    else if(strstr(string,"char"))
    {
        WriteInfo(string,1,index);
    }
    else if(strstr(string,"double"))
    {
        WriteInfo(string,8,index);
    }
    return flag;
}

//分配内存以及赋值等
void WriteInfo(char *string,int size,int index)
{
    char *split=" ='\"[](){};";
    int i = index;
    char arraysize[10];//接收数组大小 
    int num = 0;        //用来接收自字符串，转换为整数
    //判断是否为数组
    if(strstr(string,"[")||strstr(string,"]"))//声明数组
    {
        if(strstr(string,"{")||strstr(string,"}"))//声明数组并赋值
        {
            StrSplit(string,L.elem[i].type,split,0);
            StrSplit(string,L.elem[i].VarName,split,1);
            StrSplit(string,L.elem[i].value,split,3);   
            StrSplit(string,arraysize,split,2);
            num=atoi(arraysize);
            L.elem[i].size = num * size ;
            if(i==0)
                L.elem[i].addr = 0 ;
            else
                L.elem[i].addr = L.elem[i-1].addr + L.elem[i-1].size;
        }
        else if(strstr(string,"\""))//声明字符串数组并赋值
        {
            StrSplit(string,L.elem[i].type,split,0);
            StrSplit(string,L.elem[i].VarName,split,1);
            StrSplit(string,L.elem[i].value,split,3);   
            StrSplit(string,arraysize,split,2);
            num=atoi(arraysize);
            L.elem[i].size = num * size ;
            if(i==0)
                L.elem[i].addr = 0 ;
            else
                L.elem[i].addr = L.elem[i-1].addr + L.elem[i-1].size;
        }
        else
        {
            StrSplit(string,L.elem[i].type,split,0);
            StrSplit(string,L.elem[i].VarName,split,1);  
            StrSplit(string,arraysize,split,2);
            num=atoi(arraysize);
            L.elem[i].size = num * size ;
            if(i==0)
                L.elem[i].addr = 0 ;
            else
                L.elem[i].addr = L.elem[i-1].addr + L.elem[i-1].size;
        }
        //判断数组结束
    }
    //判断是否为函数
    else if(strstr(string,"(")&&strstr(string,")"))
    {
        strcpy(L.elem[i].type,"func");
        //StrSplit(string,L.elem[i].type,split,0);
        StrSplit(string,L.elem[i].VarName,split,1);
        L.elem[i].size = size ;
        if(i==0)
            L.elem[i].addr = 0 ;
        else
            L.elem[i].addr = L.elem[i-1].addr + L.elem[i-1].size;
    }//判断函数结束
    else//普通变量
    {
        StrSplit(string,L.elem[i].type,split,0);
        StrSplit(string,L.elem[i].VarName,split,1);
        StrSplit(string,L.elem[i].value,split,2);
        L.elem[i].size =  size ;
        if(i==0)
            L.elem[i].addr = 0 ;
        else
            L.elem[i].addr = L.elem[i-1].addr + L.elem[i-1].size;
    }
    ++L.length;
}

//变量的赋值
void Assignment(char *varname)
{
    char value[20];
    char string[100];
    int flag=0;
    for(int i=0;i<L.length;i++)
    {
        if(strcmp(L.elem[i].VarName,varname)==0)
        {
            printf("This varable has been done!\n");
            printf("please import its value.\n");
            scanf("%s",value);
            strcpy(L.elem[i].value,value);
            flag = 1;
        }
    }
    if(flag==0)
    {
        printf("There is no this variable.\n");
        printf("please import its information.\n");
        fgets(string,sizeof(string),stdin);
        SymbolTableInfo(string,++L.length);
    }
}

//变量的取值
void SelectVar(char *varname)
{
    char value[20];
    char string[100];
    int flag=0;
    for(int i=0;i<L.length;i++)
    {
        if(strcmp(L.elem[i].VarName,varname)==0)
        {
            printf("The value of %s is %s\n",varname,L.elem[i].value);
            flag = 1;
        }
    }
    if(flag==0)
    {
        printf("There is no this variable.\n");
    }
}

/*打印符号表及内存*/
void Display(SqList_t L)
{
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("\t\t\t\tSymbol Table\n");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("Variable Name\t\tAddress\t\tType\t\tSize\t\tValue\n");
    for(int i=0;i<L.length;i++)
        printf("%s\t\t\t%d\t\t%s\t\t%d\t\t%s\n",L.elem[i].VarName,L.elem[i].addr,L.elem[i].type,L.elem[i].size,L.elem[i].value);
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

int Menu(int num)
{
    printf("请选择功能...\n");
    printf("输入数据：  1\n");
    printf("查看数据：  2\n");
    printf("添加数据：  3\n");
    printf("查看变量值: 4\n");
    scanf("%d",&num);
    getchar();
    return num;
}

int main()
{
    char string[100];
    char varname[100];
    int i=0,num=0;
    int flag;
    InitList();
    while(1)
    {
        flag = 1;
        num=Menu(num);
        switch(num)
        {
            case 1:
                fgets(string,sizeof(string),stdin);
                SymbolTableInfo(string,i++);
                memset(string,'\0',sizeof(string));
                break;
            case 2:
                Display(L);
                break;
            case 3:
            	printf("Please input varname:\n");
            	scanf("%s",varname);
            	getchar();
                Assignment(varname);
                break;
            case 4:
            	printf("Please input varname:\n");
            	scanf("%s",varname);
            	getchar();
                SelectVar(varname);
                break;
            default:
                break;
            
        }
    }
    return 0;
}
