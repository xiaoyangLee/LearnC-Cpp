define只是简单的文本替换
typedef是给一个类型重命名
当我们定义指针的时候,typedef要比define有优势

define工作在预编译时,进行简单的文本替换
define定义时,如果涉及到运算式,需要用括号括起
for example:
define f(x) ((x)*(x))

typedef工作在编译时,将数据类型重名命
typedef可以命名复杂的数据类型,我们常见的结构体类型,就常常和typedef结合起来使用
for example:
typedef　struct　tagMyStruct
{
 
 
int　iNum;
 
long　lLength;
 
 
}MyStruct;

实际上执行两个操作,一个是定义了结构体tagMyStruct,注意这是一个临时的名字
struct　tagMyStruct
{
 
 
int　iNum;
 
 
long　lLength;
 
};
而typedef为这个结构体起了一个新名字
typedef struct tagMyStruct MyStruct;