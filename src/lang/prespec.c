// 符号可以分为变量和类型，变量的底层表示其实就是一个内存中地址，包括全局变量、局部变量、
// 函数、跳转标签都是一个地址。类型只是一种语法概念并不占据实际物理世界空间，但这两种符号
// 都可以进行调用，变量的调用其实就是函数，类型的调用相当于模板类型实例化变成具体类型。其
// 实除了变量和类型，还存在一种更概念上的符号称为记号，包括包名、宏名。
//
// 关键字，去掉 default 因为可以用 else 实现，而 fallthrough 可以用 #casefall 表示。
// if else case for range
// continue break defer return goto
// struct interface enum const extern
//
// 首字母大写，以 _t _it _struct 结束的标识符，或以 type! 开头的标识符，自动识别为类型
// 标识符。以$开头的名称是非类型，使用关键字命令非类型例如 $int $if。使用 # 开头的标识
// 符表示编译时标识，@ 开始的标识符表示运行时标识。
//
// 编译时常量：
//  #file #func #line #retp
// 编译时函数：
//  #equal #static #sizeof #typeof
// 编译时关键字：
//  #import
//  #casefall
//  #if 条件编译
// 预定义类型
//  #self 当前函数（当前函数的地址）或当前结构体的指针类型
// 操作符
// #- #+ #^
// (&) (*) (**) (*&) (**&) (&1) (&2) (*1) (*2)
//
// 类型字面量（TypeLit）包括基本类型、匿名类型（AnonyType)、命名类型（NamedType）、实
// 列化类型（ImpledType）。基本类型定义如下：
//  any void
//  bool null byte rune errt strt string type!thrd type!i type!u int type!b type!w type!d type!q type!x type!y type!z type!p
//  i08 i16 i32 i64 i128 i256 i512 intp int intb intw intd intq intx inty intz intq uinb uinw uind uinq uinx uiny uinz uinq
//  u08 u16 u32 u64 u128 u256 u512 uint (b w d q x y z p) byte word double-word quad-word xmm-word ymm-word zmm-word
//  f08 f16 f32 f64 f128 f256 f512 float 
//  d08 d16 d32 d64 d128 d256 d512 decimal
//  c08 c16 c32 c64 c128 c256 c512 complex
//  a08 a16 a32
//
// 匿名类型定义如下，匿名类型除了函数和方法之外只能用来定义新类型：
//  int  u16  f64  Type  Point  type!myint
//  int* u16* f64*
//  int[2] u16[2] f64[2]
//  int[2]* u16[2]* f64[2]*
//  (int a b) (int a b int) (void) (void int) 参数必须带名称，返回值是一个类型列表不能带名称，函数如果没有参数必须带void
//  (Point p float factor) (Point p) (Point(N, T, U) p)
//  Function
//      (void) // 只有返回值可以省略，这里void表示没有参数，空括号 () 表示没有参数的函数调用
//      (void int) // 只有返回值可以省略，这里void表示没有参数
//      (int a int)
//      (int a b)
//      (Point p float factor) // 结构体默认传递地址，因此不需要显式指定指针类型，如果不希望对象被函数修改，需要在调用前先拷贝
//  Structure
//      Type (struct) // 空结构体
//      (struct int a b)
//      (struct (#self int a b int) calc)
//  Interface // 接口不能声明为空，必须包含成员函数声明，也只能包含成员函数声明或内嵌接口
//      (interface (#self int a b int) calc (#self int c) put)
//  Enum // 枚举类型不能为空
//      (enum ) // 非法
//      (enum RED (const * 2) YELLOW BLUE) // const 是枚举元素的索引值
//      (enum byte RED YELLOW BLUE)
//  const PI f64 3.1415926
// 复合类型，匿名类型也可用于复合类型声明，不必先定义新命名类型再定义复合类型：
//      指针                数组                数组的指针              指针的数组
//      int*                int[2]              int[2]*                 int*[2]
//      f64*                f64[2]              f64[2]*                 f64*[2]
//      int**               int[2][3]           int[2][3]**             int**[2][3]
//      f64**               f64[2][3]           f64[2][3]**             f64**[2][3]
// 命名类型单指用户定义的类型，虽然基本类型也都是有名字的：
//  MyInt int type!myint int
//  ReturnType (struct u32 bool) 相当于定义tuple
//  Func (int a b int)
//  Func (void) 参数为空，返回为空
//  Func (void u32 bool) 参数为空，返回u32和bool
//  Method (Point p float factor)
//  Data (struct int a b)
//  Oper (interface (#self int a b int) calc)
//  Color (enum RED YELLOW BLUE)
// 函数包含参数和捕获，函数调用时需要给参数提供实参而捕获不需要：
//  (int a b int) [m] { ... }
//  (Point p float factor float) [m] { ... }
// 定义模板类型，模板类型都是命名的，如果模板是基本类型，则需要使用常量进行实例化：
//  Ptr [T] (struct T* p)
//  Pfp [T] (struct T** pp)
//  Array [T N|int] (struct T[N] a)
//  Slice [T] (struct T* a int len)
//  Map [K V] (struct (struct K key V value)[] slice)
//  Point [T|numeric] (struct T x y) // 只能给结构体定义模板，接口和函数都不能定义模板，但是模板类型可以作为函数的接收类型
//  Triple [N|uint T U] (struct T[N] a U b)
//  length (Point(T) p T) { return type.x * type.x + type.y * type.y }
//  size (Triple(N, T, U) p typeof(N)) { return N + sizeof(T) + sizeof(U) }
// 模板类型实例化：                                                      代码中仅允许使用这种简化的形式
//  ptr(int) ptr(ptr(int))                                              int* int**              type:int* int.type:ptr
//  array(int, 3)                                                       int[3]                  int.type:array(3)
//  slice(int)                                                          int.slice               int.type:slice
//  map(string, int)                                                    string.map(int)         string.type:map(type:int)
//  Triple(3, int, string)                                              Triple#3(int, string)
// 指针的指针，指针的数组：
//  ptr(ptr(int))                                                       int** int**
//  array(ptr(int), 3)                                                  int*[3]
//  slice(ptr(int))                                                     int*.slice
//  map(ptr(string), int)                                               string*.map(int)
//  map(string, ptr(int))                                               string.map(int*)
//  Triple(3, ptr(int), string)                                         Triple#3(int*, string)
//  Triple(3, int, ptr(string))                                         Triple#3(int, string*)
//  Triple(3, ptr(int), ptr(string))                                    Triple#3(int*, string*)
// 数组的指针：
//  ptr(array(int, 3))                                                  int[3]*
//  ptr(slice(int))                                                     int.slice*
//  ptr(map(string, int))                                               string.map(int)*
//  ptr(array(ptr(int), 3))                                             int*[3]*
//  ptr(slice(ptr(int)))                                                int*.slice*
//  ptr(map(ptr(string), int))                                          string*.map(int)*
//  ptr(map(string, ptr(int)))                                          string.map(int*)*
//  ptr(Triple(3, ptr(int), string))                                    Triple#3(int*, string)*
//  ptr(Triple(3, int, ptr(string)))                                    Triple#3(int, string*)*
//  ptr(Triple(3, ptr(int), ptr(string)))                               Triple#3(int*, string*)*
// 数组的映射：
//  map(array(int, 3), int)                                             int[3].map(int)
//  map(slice(int), int)                                                int.slice.map(int)
//  map(map(string, int), int)                                          string.map(int).map(int)
//  map(array(ptr(int), 3), int)                                        int*[3].map(int)
//  map(slice(ptr(int)), int)                                           int*.slice.map(int)
//  map(map(ptr(string), int), int)                                     string*.map(int).map(int)
//  map(map(string, ptr(int)), int)                                     string.map(int*).map(int)
//  map(Triple(3, ptr(int), string), int)                               Triple#3(int*, string).map(int)
//  map(Triple(3, int, ptr(string)), int)                               Triple#3(int, string*).map(int)
//  map(Triple(3, ptr(int), ptr(string)), int)                          Triple#3(int*, string*).map(int)
//  map(string, array(int, 3))                                          string.map(int[3])
//  map(string, map(string, int))                                       string.map(string.map(int))
//  map(string, slice(int))                                             string.map(string.map(int))
//  map(string, array(ptr(int), 3))                                     string.map(int*[3])
//  map(string, slice(ptr(int)))                                        string.map(int*.slice)
//  map(string, map(ptr(string), int))                                  string.map(string*.map(int))
//  map(string, map(string, ptr(int)))                                  string.map(string.map(int*))
//  map(string, Triple(3, ptr(int), string))                            string.map(Triple#3(int*, string).map(int))
//  map(string, Triple(3, int, ptr(string)))                            string.map(Triple#3(int, string*).map(int))
//  map(string, Triple(3, ptr(int), ptr(string)))                       string.map(Triple#3(int*, string*).map(int))
// 复杂实例化：
//  Triple(3, map(array(int, 3), int), string)                          Triple#3(int[3].map(int), string)
//  Triple(3, map(slice(int), int), string)                             Triple#3(int.slice.map(int), string)
//  Triple(3, map(map(string, int), int), string)                       Triple#3(string.map(int).map(int), string)
//  Triple(3, map(array(ptr(int), 3), int), string)                     Triple#3(int*[3].map(int), string)
//  Triple(3, map(slice(ptr(int)), int), string)                        Triple#3(int*.slice.map(int), string)
//  Triple(3, map(map(ptr(string), int), int), string)                  Triple#3(string*.map(int).map(int), string)
//  Triple(3, map(map(string, ptr(int)), int), string)                  Triple#3(string.map(int*).map(int), string)
//  Triple(3, map(Triple(3, ptr(int), string), int), string)            Triple#3(Triple#3(int*, string).map(int), string)
//  Triple(3, map(Triple(3, int, ptr(string)), int), string)            Triple#3(Triple#3(int, string*).map(int), string)
//  Triple(3, map(Triple(3, ptr(int), ptr(string)), int), string)       Triple#3(Triple#3(int*, string*).map(int), string)
//  Triple(3, map(string, array(int, 3)), string)                       Triple#3(string.map(int[3]), string)
//  Triple(3, map(string, map(string, int)), string)                    Triple#3(string.map(string.map(int)), string)
//  Triple(3, map(string, slice(int)), string)                          Triple#3(string.map(string.map(int)), string), string)
//  Triple(3, map(string, array(ptr(int), 3)), string)                  Triple#3(string.map(int*[3]), string)
//  Triple(3, map(string, slice(ptr(int))), string)                     Triple#3(string.map(int*.slice), string)
//  Triple(3, map(string, map(ptr(string), int)), string)               Triple#3(string.map(string*.map(int)), string)
//  Triple(3, map(string, map(string, ptr(int))), string)               Triple#3(string.map(string.map(int*)), string)
//  Triple(3, map(string, Triple(3, ptr(int), string)), string)         Triple#3(string.map(Triple#3(int*, string).map(int)), string)
//  Triple(3, map(string, Triple(3, int, ptr(string))), string)         Triple#3(string.map(Triple#3(int, string*).map(int)), string)
//  Triple(3, map(string, Triple(3, ptr(int), ptr(string))), string)    Triple#3(string.map(Triple#3(int*, string*).map(int)), string)

// 定义命名类型
//
// 括号内表达式是一个类型或类型列表或者是void：
//      如果前面是一个类型，则表示类型实例化，怎样区分成员函数的接收参数（参数列表中的第一个标识符是变量而不是类型!!!!）
//      如果是一个操作符，报错
//      如果是一个非类型标识符，表示是一个函数参数部分的声明
//      如果是一个字面常量，报错
//
// 括号内表达式如果是一个值或者为空：
//      如果前面的表达式是一个操作符，那么值用于操作符求值，空则报错
//      如果前面的表达式是一个类型，值是常量且类型是接受该常量类型的模块则进行类型实例化，否则报错
//      如果前面的表达式是一个变量，则进行函数调用
//      如果前面的表达式是一个常量，则报错

Point (struct float x y)
Data (struct int a b (int a b int) f g)
Reader (interface (#self int a b int) calc (#self byte[] a) get)

T1 int
T2 string
T3 T2
T4 (struct (int a b int) f g int a b)
T5 (interface (#self int a b int) calc)
T6 (int a b int)
T7 (Point p float factor)

T1 int
T2 string
T3 T2
T4 (struct int a b)
T5 (interface (#self int a b int) calc)
T6 (int a b int)
T7 (Point p float factor)

T1 int[3]
T2 string[3]
T3 T2[3]
T4 (struct int a b)[3]
T5 (#self int a b int)[3]
T6 (int a b int)[3]
T7 (Point p float factor)[3]

T1 Triple#3(int[3], (int a b))
T2 Triple#3(string[3], (int a b)*)
T3 Triple#3(T2[3], ((# int a b int)[3]) calc))
T4 Triple#3((int a b)[3], (int a b int)[])
T5 Triple#3(((# int a b int)[3]) calc, (Point p float factor)[int])
T6 Triple#3((int a b int)[3], int*)
T7 Triple#3((Point p float factor)[3], string[int])

// 定义泛化类型

Node [T] (struct
    #self next // #self 总是表示的是指针
)

Triple [T U N|int] (struct
    T[N] a
    U b
)

// 类型并不需要提前声明，因为可以通过词法直接分辨，函数类型如果可以通过传递的参数匹配也无需提前声明
T1;
T2;
T3;
T4;
T5;
T6;
T7;
Calc;
type!int;
type!calc;
type!coro;

// 定义变量，包括函数变量，一个非类型标识符后跟一个类型表示定义该类型的一个变量

Main (int argc byte** argv int)

Sale (Point p int a b)

print (##string fmt any ... intp)

main (int argc byte** argv int) {
    print("%\n", argc)
    return 0
}

main Main {
    print("%\n", argc)
    return 0
}

calc (int a b int) {
    return a + b
}

calc (int a b int) #cdecl {
    return a + b
}

calc (int a b int) #fastcall {
    return a + b
}

scale (Point p float factor) {
    p.x * factor;
    p.y * factor;
}

scale (Point p float factor) #cdecl {
    p.x * factor;
    p.y * factor;
}

scale (Point p float factor) #fastcall {
    p.x * factor;
    p.y * factor;
}

next (Node(T) p T*) {
    return p.next
}

size (Triple(N, T, U) p #typeof(N)) {
    return p.a + N + #sizeof(T)
}

data (struct int a b) {1, 2}
data (struct int a b)* (&) {1, 2}
data (struct int a b)[2] {{1, 2}, {3, 4}}
data Data {1, 2}
data Data* (&) data
data Data[2] {data1, data2}

<found index> array_find(<<array, item)

cal2 (int a b int) { return a + b }
cal2 (int a b int) {return a + b} (1, 2)
cal2 (int a b int)* (int a b int) {return a + b } // 函数不需要声明成指针，因为它本身就是指针
cal2 (int a b int)* Calc {return a + b }
cal2 (int a b int)[2] {Calc {return a + b}, Calc { return a * b }}
cal2 Calc { return a + b }
cal2 type!calc { return a + b }
cal2 (int a b int) * (&) (int a b int) {return a + b }
cal2 (int a b int)[2] {Calc {return a + b}, Calc { return a * b }}
cal2 Calc* (&) (int a b int) {return a + b }
cal2 Calc* (&) {return a + b }
cal2 (&)Calc{return a + b}
cal2 Calc[2] {Calc {return a + b}, Calc { return a * b }}
cal2 Calc[2] {Calc {return a + b}, Calc { return a * b }}

ppb Ppb* malloc(size)

a int ???
a 42
a #+99
b 0
c #-17
d 'a
d \n
e 's 'r \s \t 'c '/ \n
e 'sr\tc/\n
f \nsr\tc/\n

integers [1, 2, 3]
colors ["红", "黄", "绿"]
nested_array_of_ints [[1,2], [3,4,5]]
nested_mixed_array [[1,2], ["a", "b", "c"]]

// https://squidfunk.github.io/mkdocs-material/reference/admonitions/
// https://github.com/cloudwu/datalist
// https://toml.io/cn/v1.0.0

[user User]
name "fido"
member_since Date(1999-08-04)
age 23

Fruit (struct
    string name
    int[] rates
    (struct string color shape (struct int hight width (struct string a b) desc string info) size) physical
    (struct string name int id)[#arrfit] varieties
    (struct bool sweet) taste #optional
    (struct bool smooth) texture
)

Fruit (struct
    string name
    int[] rates
    (struct
        string color shape
        (struct
            int height width
            (struct string a b) desc
            string info
            string desc
        ) size
    ) physical
    (struct string name int id)[#arrlit] varieties
    (struct bool sweet) taste #optional
    (struct bool smooth) texture
)

[apple Fruit] // 以另一个[var Type]结束
physical color "红" size height 23 width 24 desc a "a" b "b" size:info "i"
physical shape "round"
taste sweet true
texture smooth true
rates [80, 75, 95]

[apple Fruit]
physical color "红" size height 23 width 24 desc a "a" b "b" size:info "i" desc "d"
physical shape "round"
taste sweet true
texture smooth true

[apple Fruit] // 0~3个空格是顶格字段，>=4个空格或>=1个Tab才是子字段，size: 回到 size 层级
physical
    color "红"
    size
        height 23
        width 24
        desc
            a "a"
            b "b"
    size:
        info "i"
        desc "d"
physical
    shape "round"
taste
    sweet true
texture
    smooth true

[apple Fruit]
physical color "红"
    size height 23
    width 24
    desc a "a"
    b "b"
    size:info "i"
    desc "d"
physical shape "round"
taste sweet true
texture smooth true

[apple Fruit] {
.physical
    color "红"
    size
        height 23
        width 24
        desc
            a "a"
            b "b"
    size:
        info "i"
        desc "d"
.physical
    shape "round"
.taste
    sweet true
.texture
    smooth true
}

[apple Fruit]
physical
    color "红"
    size height 23 width 24
    desc a "a" b "b"
physical
    size info "i"
physical
    shape "round"
taste
    sweet true
texture
    smooth true

[apple Fruit]
physical
    shape "round"
    color "红"
    size info "i"
    height 23 width 24
    desc a "a" b "b"
taste
    sweet true
texture
    smooth true

[#global]
a true
b 1024
s "hello"

[animal Fruit]
type name "pug"

[fruits Fruit[]]
name "apple"
physical color "red" shape "round"

[fruits Fruit[]]
varieties
    name "red delicious"
    id 1
name "banana"
varieties
    name "granny smith"
    id 2
varieties
    name "plantain"
    id 3

[fruits Fruit[]]
name "banana"

#import "src/lang/math" // 加载静态代码

time:Time
math:*

dat2 Data {3, 4}
data (struct u32 bool) parse_hex_number(slice(hex, it*2, 2))
data.bool
data.bool.1
data (struct u32 n bool ok) parse_hex_number(slice(hex, it*2, 2))
data.bool
data.ok
data parse_hex_number(slice(hex, it*2, 2))

cal2 (int a b int) { return a + b }
cal2 (int a b int) { return a + b }
cal2 Calc { return a + b }
cal2 Calc calc
cal2 calc

dat3 (struct int a b)* (&){3, 4}
dat3 (struct int a b)[2] {{3, 4}, data}
dat3 (struct int a b)* (&){3, 4}
dat3 Data* (&)data
dat3 Data[2] {{3, 4}, data}

cal3 (int a b int)* (int a b int) { return a + b }
cal3 (int a b int)* (&) { return a + b }
cal3 (int a b int)* (&) calc
cal3 (int a b int) { return a + b }
cal3 Calc { return a + b }
cal3 calc

// 基本类型和指针不通过大括号进行初始化而是直接给值，根据语法定义一个变量的同时必须进行初始化
// 一个非类型标识符后跟一个类型表示定义该类型的一个变量
// 一个非类型标识符后跟一个非类型标识符，表示用后面的非类型标识符的值定义一个变量
// 一个非类型标识符后跟一个字面常量，表示用字面常量定义一个变量

cal3 (int a b int)* null
cal3 (int a b int)* (&){ return a + b }
cal3 (int a b int)* calc

numb errt null
numb float 3.14
numb int* null
numb int* (&)data
numb bool false

data false
data "Hello World"
data 3.14
data 1024
data calc

calc Calc { return a + b }
data Data {1, 2}
data int 1024
numb errt null
numb float 3.14
numb int* (&) data
calc Calc { return a + b }
data Data {1, 2}
data int 1024
numb errt null
numb float 3.14
numb int* (&) data
temp int 1024
temp float 3.14

temp int calc(1, 2)
temp Calc { return a + b } (1, 2)
temp Calc { return a + b } (1, 2)
temp (int a b int) { return a + b } (1, 2)

i int 0 sume 0 bytes .. same_as_prev 0 .. more userdata_bytes != 0

// 数据类型转换

aaa Data{3, 4} // 赋值语句因为目标变量只有一个，因此只要将类型不加修饰的放在右表达式开头，相对于对整个表达式类型的转换
ppb ppb_t* ppb_alloc(alloc)

pos dist + cast(int)scale_x(facter) // 其他地方都使用 cast 表达式进行转换
len int pos + (&)(int*):(byte*)p + size + f(g)

pos int dist + scale_x(facter).(int)
len int pos + (*&)(int*):(byte*)p + size + f(g)
len typeof(pos) 3

for i int 3 .. 10 { /* */ }

// 函数和普通变量提前声明，同一个变量声明可以出现多次，定义一个变量时必须有初始化也即
// 推荐仅在使用的地方才进行变量定义不提前定义变量
// 类型不需要提前声明，因为其名称可以由词法分辨
extern calc (int a b int)
extern scale (Point p float factor)
extern data (struct int a b)
extern calc Calc
extern data Data
extern data int
extern numb errt
extern numb float
extern numb int*

#if DYN_LINK_PROC {
    calc Calc null
} else {
    extern calc Calc
}

优先级（从高到低） ::

    12 从左到右    a:b 名字空间由代码包和文件内代码分块表示，代码分块的表示形如 :::time::: 代码包由一个文件夹组成
    11 从左到右    a() a[] a.b a->b 函数调用，数组下标，成员访问
    10 从右到左    #-a #+a #^a !a (type)a (&)a (*)a (**)a (*&)a (**&)a ->> <<-
     9 从左到右    a.&b a->&b 返回成员地址，相当于(&)a.b
     8 从左到右    a*b a/b a%b a&b a<<b a>>b   mul_op   --> <-- &^
     7 从左到右    a+b a-b a|b a^b             add_op   |^
     6 从左到右    a<b a>b a<=b a>=b           rel_op
     5 从左到右    a==b a!=b
     4 从左到右    a&&b
     3 从左到右    a||b
     2 从右到左    a?b:c a=b a+=b a-=b a*=b a/=b a%=b a<<=b a>>=b a&=b a^=b a|=b
     1 从左到右    a,b

为了省略大部分的分号并避免歧义，一元操作符使用特殊的语法形式： ::

    小括号包含类型用来定义类型或用作类型转换操作符，小括号包含值表示表达式的一部分。
    大括号只能包含值或由值组成的语句列表，值由变量常量操作符组成。
    取地址 & 改为 (&) 地址标记 (&1) (&2)
    解引用 * 改为 (*) (**) (*&) (**&) 地址引用 (*1) (*2)

    #negt()     #-          #-3.14      #-c
    #posi()     #+          #+6.24      #+c
    #addr()     (&)         (&)data
    #dref()     (*)         (*)p        (*)c        (**)pp
    #comp()     #^          #^c
    #sizeof(T)
    #typeof(expr)
    #alignof(a)

// 条件语句包含传统C的if和switch：
//  if cond { expr }
//  if cond return expr
//  if cond { expr } else if_stm
//  #if cond { expr } else { expr }
//  #if cond { expr } else if cond { expr }

if color case .RED { // 使用break会跳出外层for循环
    #casefall
} case .BLUE {

} else {

}

defer_return #label
    return

// 函数支持默认参数和重载，以及支持操作符重载+ - * / == != << >> & | [] % ^ <<< >>> []= .&，#symmetric
// 禁止函数链式调用 a.getb().bfun()
// 定义展开函数：
// func (retrun int) #expand {
//      `x = 32
//      retrun `x * 2
// }
// pop(q, free_func)
// pop(q, =>(x)free(x))

// __jai_runtime_init
// __jai_runtime_fini
// __system_entry_point

(->> theme
     (symbol-name)
     (funcall (-flip #'concat) "-theme")
     (intern))

(intern (funcall (-flip #'concat) "-theme" (symbol-name theme)))
