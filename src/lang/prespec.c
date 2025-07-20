// 符号可以分为变量和类型，变量的底层表示其实就是一个内存中地址，包括全局变量、局部变量、
// 函数、跳转标签都是一个地址。类型只是一种语法概念并不占据实际物理世界空间，但这两种符号
// 都可以进行调用，变量的调用其实就是函数，类型的调用相当于模板类型实例化变成具体类型。其
// 实除了变量和类型，还存在一种更概念上的符号称为记号，包括包名、宏名。
//
// 关键字，去掉 default 因为可以用 else 实现，而 fallthrough 可以用 #fall 表示。
// if else case for range
// continue break defer return
// struct trait enum const void
// static extern using
// sizeof typeof alignof offsetof
//
// 首字母大写，包括单个大写字母，并排除包含两个和两个以上字母但都是大写的标识符之后的所
// 有标识符都识别为类型名。另外，以后缀 _t 结束，或以单下划线 _ 开始并至少包含一个字母，
// 或包含在 @{} 之内的标识符，也都是类型名。
// 其他标识符符都是非类型标识符，包括函数名、变量名、标签、包名等等。另外包含在 @() 之
// 内的标识符也都是非类型标识符，例如把类型名和关键字当作变量标识符使用：@(int) @(if)
// @(Type) @(_i)。以 # 开头的标识符是编译器指令，包括函数、类型、变量的属性名称等。
//
// 编译时常量：
//  #file #func #line #retp
// 编译时函数：
//  #equal #static #align(16) #packed
// 编译时关键字：
//  #import #dont_import
//  #fall
//  #if 条件编译
// 编译和运行时
//  #comptime #runtime
// 预定义类型
//  this 当前函数（当前函数的地址）或当前结构体的指针类型，不提供任何面向对象的特殊含义，但匿名类型需要用
// 操作符
// #- #+ #^
// (&) (*) (**) (*&) (**&) (&1) (&2) (*1) (*2)
//
// 基本类型，定义在 type 代码包中：
//  bool null byte rune errt strt string
//  i08 i16 i32 i64 i128 int sys_int (b w d q x y z p r) byte word double-word quad-word xmm-word ymm-word zmm-word
//  u08 u16 u32 u64 u128 unt sys_unt sys_ptr
//  f08 f16 f32 f64 f128 float
//  d08 d16 d32 d64 d128 decimal
//  c08 c16 c32 c64 c128 complex
//  -- vector type --
//  int(16-byte)        int(32-byte)        int(64-byte)
//  float(16-byte)      float(32-byte)      float(64-byte)
//  decimal(16-byte)    decimal(32-byte)    decimal(64-byte)
//  complex(16-byte)    complex(32-byte)    complex(64-byte)
//
// rax rbx rcx rdx rbp rsp rsi rdi rlr rip rpc
// bax bbx bcx bdx bbp bsp bsi bdi blr bip bpc
//                                          arch-32     arch-64 small memory range app  arch-64 large memory range app
//  int - pointer size signed type          32-bit      32-bit                          64-bit
//  unt - pointer size unsigned type        32-bit      32-bit                          64-bit
//  void* - pointer type - unt              32-bit      32-bit                          64-bit
//  sys_int - system register size type     32-bit      64-bit                          64-bit
//  sys_unt - system register size type     32-bit      64-bit                          64-bit
//  sys_ptr - system pointer - sys_unt      32-bit      64-bit                          64-bit
//
// 类型约束：
// Any Integer Float Unsigned Decimal Complex BasicType AnonyType NamedType GimplType
//
// 类型字面量（TypeLit）包括基本类型（BasicType）、匿名类型（AnonyType)、命名类型（NamedType）、实列化类型（ImpledType）。
// any          // basic_type + anony_type + named_type + gimpl_type
// basic_type   // numeric + string
// numeric      // integer + float + decimal + complex
// integer      // bool null byte rune errt strt i08~i512 u08~u512 int unt
//
// 复合类型和匿名类型：
//  int  u16  f64  Point  _myint
//  int* u16* f64* Point* _myint*
//  int[2] u16[2] f64[2]
//  int[2]* u16[2]* f64[2]*
//  (int a b) (int a b int) (void) (void int) 参数必须带名称，返回值是一个类型列表不能带名称，函数如果没有参数必须带void
//  (Point p float factor) (Point p) (Point(N, T, U) p)
//  Function
//      (void) // 没有参数，没有返回值，空括号 () 表示没有参数的函数调用
//      (void int) // 没有参数，单个返回值，参数不能省略，如果没有参数必须声明为void，而返回值可以没有
//      (void int string) // 没有参数，多个返回值
//      (int a int)
//      (int a b)
//      (Point p float factor) // 结构体默认传递地址，因此不需要显式指定指针类型，如果不希望对象被函数修改，需要在调用前先拷贝
//  Tuple 元组补充结构体表达不了的一些东西
//      (int int)            结构体不能同时定义两个同类型的内嵌字段
//      (this int int int)   结构体不能内嵌一个指针类型
//  Struct 表示定义一个类型
//      Type {}
//      { int a b } {1, 2}
//      { (this int a b int) calc }
//  Trait // 接口不能声明为空，必须包含成员函数声明，也只能包含成员函数声明或内嵌接口，接口是一个没有成员只有静态数据的结构体
//      trait { (this int a b int) calc (this int c) get }
//  Enum 枚举类型，只能表示整数常量
//      enum { RED (enum * 2) YELLOW BLUE } // enum 是枚举元素的索引值
//      enum int { RED YELLOW BLUE }
//  常量类型，可以表示任意常量，包括结构体常量，常量类型定义一个对应类型的常量值，是一个值不是类型
//      const PI f16 3.14
//      const PI 3.1415926
//      const HI { int a float b string c Point p } { a 1 b 3.14 p(10, 30) }
//      const HI HiData { a(MAX_SIZE) b(3.14) c("hello") p{10, 30} } // 基本类型、结构体、复合类型的初始化怎么统一 ???
//      const fn (int a b int) calc_func
// 定义一个类型参数列表
//      Test (Any $T int $a string $b) {}
//      Test(Type, 1, "hello")
// 复合类型，匿名类型也可用于复合类型声明，不必先定义新命名类型再定义复合类型：
//      指针                数组                数组的指针              指针的数组
//      int*                int[2]              int[2]*                 int*[2]
//      f64*                f64[2]              f64[2]*                 f64*[2]
//      int**               int[2][3]           int[2][3]**             int**[2][3]
//      f64**               f64[2][3]           f64[2][3]**             f64**[2][3]
// 命名类型单指用户定义的类型，虽然基本类型也都是有名字的：
//  MyInt int  _myint int
//  Func (int a b int)
//  Func (void) 参数为空，返回为空
//  Func (void u32 bool) 参数为空，返回u32和bool
//  Method (Point p float factor)
//  Data { int a b }
//  Oper trait { (this int a b int) calc }
//  Color enum { RED YELLOW BLUE }
// 函数包含参数和捕获，函数调用时需要给参数提供实参而捕获不需要：
//  (int a b int) [m] { ... }
//  (Point p float factor float) [m] { ... }
// 定义模板类型，模板类型都是命名的，如果模板是基本类型，则需要使用常量进行实例化：
//  Ptr ($T) { T* p }
//  Pfp ($T) { T** pp)
//  Array ($T int $size) { T[size] a }
//  Slice ($T) { T* a int len }
//  Map ($K $V) { {K key V value}[] slice }
//  Point (Numeric $T) { T x y } // 只能给结构体定义模板，接口和函数都不能定义模板，但是模板类型可以作为函数的接收类型
//  Triple ($T unt $size) { T[size] a U b }
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

Point { float x y }
Data { int a b (int a b int) f g }
Reader trait {
    (this int a b int) calc
    (this byte[] a) get
}
Reader ($T (T* a int) $get (T* a void* p int n int) $read) { // 非类型 $n 表示的是一个常量模板参数（编译时常量），类型 $T 表示的是一个类型模板参数（模板类型）
}

Get ($T* a int) // 函数参数只能声明类型模板参数
Read ($T* a void* p int n int)
Reader ($T Get(T) $get Read(T) $read) {} // 结构体可以声明类型模板参数，以及常量模板参数

@{get} ($T* a int)
@{get} (Any$T* a int)
@{read} ($T* a void* p int n int)
@{reader} ($T @{get}(T) $get @{read}(T) $read) {}

_point {
    float x
    float y
}

_data {
    int a
    int b
    (int a b int) f
    (int a b int) g
}

_reader ($T (T* a int) $get (T* a void* p int n int) $read) {} // 普通类型使用下划线命名，类型参数使用首字母大写命名

_reader trait {
    (this int a b int) calc
    (this byte[] a) get
}

T1 int
T2 string
T3 T2
T4 { (int a b int) f int a b }
T5 trait { (this int a b int) calc }
T6 (int a b int)
T7 (Point p float factor)

_t1 int
_t2 string
_t3 _t2
_t4 { (int a b int) f int a b }
_t5 trait { (this int a b int) calc }
_t6 (int a b int)
_t7 (Point p float factor)

T1 int[3]
T2 string[3]
T3 T2[3]
T4 { int a b }[3]
T5 (this int a b int)[3]
T6 (int a b int)[3]
T7 (Point p float factor)[3]

T1 Triple#3(int[3], (int a b))
T2 Triple#3(string[3], (int a b)*)
T3 Triple#3(T2[3], ((# int a b int)[3]) calc))
T4 Triple#3((int a b)[3], (int a b int)[])
T5 Triple#3(((# int a b int)[3]) calc, (Point p float factor)[int])
T6 Triple#3((int a b int)[3], int*)
T7 Triple#3((Point p float factor)[3], string[int])

Node ($T) {
    this next // this 总是表示的是指针
    T data
}

Triple ($T $U int $size) {
    T[size] a
    U b
}

Main (i32 argc byte** argv i32)

Scale (Point p int a b)

Calc (int a b int)

_array ($T int $size) {
    T[size] a
}

_array ($_t int $size) {
    _t[size] a
}

_array ($T) {
    T* item
    int capacity
    unt len
}

_coro {
    u32 rspoffset // 1st field dont move
    u32 loweraddr
    u32 maxudsize 31 ptr_param 1
    i32 coro_id
    unt rspoffset
    unt loweraddr
    unt maxudsize 31 ptr_param 1
    int coro_id
    _u rspoffset
    _u loweraddr
    _u maxudsize 31 ptr_param 1
    _i coro_id
    _p address
}

_coro_guard { // 内嵌只能内嵌结构体类型，不能是指针
    u32 lower_guard_word
    u32 upper_guard_word
    Coro // 不能内嵌两个相同类型
    Coro* coro_ptr
    this// 错误，指针不能内嵌
    this coro_guard
    (int a b int) calc
    (int a) print
    (this int a b int) a
    (_coro_guard p int a b int) f g
    (Coro p) h
    { int a b }
    { int a b } tuple
    enum { RED(1) BLUE YELLOW } color
    enum int { red(red_enum_value) blue yellow } color
)

Color enum {
    RED (enum + 1)
    BLUE
    YELLOW
}

Color enum int {
    red
    blue (blue_defined_value)
    yellow
}

Color enum #strict { // strict 枚举类型必需为全部枚举手动指定值，并在代码更新时不能修改这些值，以防带来代码版本的不兼容
    RED     1
    BLUE    2
    YELLOW  3
}

BitValue enum int {
    FLAG_BIT1   (1 << enum)
    FLAG_BIT2
    FLAG_BIT3
    FLAG_BIT4
}

// 结构体参数是该结构体定义的所有变量都共享的参数，即静态数据
Writer trait #comptime { // 可以有两种实现，一种时编译时实现速度快但代码大，一种时运行时实现速度慢一点但代码小
    (this int c) put
    (this void* p int n int) write
}

// File类型的静态数据布局：
// File.Writer  put(File* self int c)
//              write(File* self void* p int n int)
// File.Reader  get(File* self)
//              read(File* self void* p int n int)
File {
    prh_ptr fd
    trait Writer(file_put, file_write)
    trait Reader(file_get, file_read)
}
generic_write(_writer* writer) { // 实际上参数会传递 file 以及 File.Writer 静态数据的地址
    func writer.write
    func(writer)
} // 如果是编译时实现，对每个实现类，都会生成一个特定的 genric_write 函数，其中的 func 会直接替换为实际函数，这种泛型实现是一种非常灵活的填鸭式的代码实现

_calc (int a b int)

_snode ($T) {
    this next
    T data
)

for i _i 0 .. 9 {
    i int #* _i* addr
    pos + (*&) _i (byte* p + size + f(g))
}
memcpy (void* dst src int count) #intrinsic
memcmp (void* dst src int count int) #intrinsic
memset (void* dst byte value int count) #intrinsic
lock_cmpxchg ($_t* p _t old new _t) #intrinsic
coro_guard (coro_t coro coro_guard_t*) #cdcel #inline

// 类型并不需要提前声明，因为可以通过词法直接分辨，函数类型如果可以通过传递的参数匹配也无需提前声明
using T1
using T2
using T3
using T4
using T5
using T6
using T7
using Calc
using myint_t
using calc_t
using coro_t
using _myint
using _calc
using _coro

// 定义变量，包括函数变量，一个非类型标识符后跟一个类型表示定义该类型的一个变量

print (##string fmt any ... int)

main (i32 argc byte** argv i32) {
    print("%\n", argc)
    return 0
}

main Main {
    print("%\n", argc)
    return 0
}

main _main {
    return 0
}

calc (int a b int) {
    return a + b
}

calc Calc #cdecl {
    return a + b
}

calc _calc #fastcall {
    return a + b
}

scale (Point p float factor) {
    p.x * factor
    p.y * factor
}

scale (Point p float factor) #cdecl {
    p.x * factor
    p.y * factor
}

scale (Point p float factor) #fastcall {
    p.x * factor
    p.y * factor
}

next (Node(T) p T*) {
    return p.next
}

size (Triple(N, T, U) p typeof(N)) {
    return p.a + N + sizeof(T)
}

data { int a b } {1, 2}
data { int a b }* (&){1, 2}
data { int a b }[2] {{1, 2}, {3, 4}}
data Data {1, 2}
data Data* (&) data
data Data[2] {data1, data2}

found .. index array_find(<<array, item)
found .. error array_find(<<array, item)

cal2 (int a b int) { return a + b }
cal2 (int a b int) {return a + b} (1, 2)
cal2 (int a b int)* (int a b int) {return a + b } // 函数不需要声明成指针，因为它本身就是指针
cal2 (int a b int)* Calc {return a + b }
cal2 (int a b int)[2] {Calc {return a + b}, Calc { return a * b }}
cal2 Calc { return a + b }
cal2 _calc { return a + b }
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
a if a==0 ? b : c
b 0
b a // 一个变量赋值给另一个变量的形式必须在同一行，不能换行
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

dat2 Data {3, 4}
data (u32 bool) parse_hex_number(slice(hex, it*2, 2))
data.u32
data.bool
data {u32 a bool b} parse_hex_number(slice(hex, it*2, 2))
data.a
data.b
data parse_hex_number(slice(hex, it*2, 2))
data.0
data.1

cal2 (int a b int) { return a + b }
cal2 (int a b int) { return a + b }
cal2 Calc { return a + b }
cal2 Calc calc
cal2 calc

dat3 { int a b }* (&){3, 4}
dat3 { int a b }[2] {{3, 4}, data}
dat3 { int a b }* (&){3, 4}
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

i int 0 sume 0
bytes .. same_as_prev 0 .. more userdata_bytes != 0

// 数据类型转换

aaa Data{3, 4} // 赋值语句因为目标变量只有一个，因此只要将类型不加修饰的放在右表达式开头，相对于对整个表达式类型的转换
ppb ppb_t* ppb_alloc(alloc)

pos dist + int scale_x(facter)
len int pos + (&) int* byte* p + size + f(g)
len int pos + (& int* byte* p) + size + f(g)

pos int dist + int scale_x(facter)
len int pos + (*) int* (byte* p + size + f(g))
len typeof(pos) 3

for i int 3 .. 10 { /* */ }

// 函数和普通变量提前声明，同一个变量声明可以出现多次，定义一个变量时必须有初始化也即
// 推荐仅在使用的地方才进行变量定义不提前定义变量
// 类型不需要提前声明，因为其名称可以由词法分辨
extern calc (int a b int)
extern scale (Point p float factor)
extern data { int a b }
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

// https://squidfunk.github.io/mkdocs-material/reference/admonitions/
// https://github.com/cloudwu/datalist
// https://toml.io/cn/v1.0.0

[user User]
name "fido"
member_since Date(1999-08-04)
age 23

Fruit {
    string name
    int[] rates
    { string color shape { int hight width { string a b } desc string info } size } physical
    { string name int id }[#arrfit] varieties
    { bool sweet } taste #optional
    { bool smooth } texture
}

Fruit {
    string name
    int[] rates
    {...
        string color shape
        {...
            int height width
            { string a b } desc
            string info
            string desc
        } size
    } physical
    { string name int id } [#arrlit] varieties
    { bool sweet } taste #optional
    { bool smooth } texture
}

[apple Fruit] // 以另一个[var Type]结束，明确是字面常量赋值不需要使用=等号，否则需要使用调用语法进行初始化
physical color "红" size height 23 width 24 desc a "a" b "b" size:info "i"
physical shape "round"
taste sweet true
texture smooth true
rates [80, 75, 95]

[apple _fruit]
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

[##global]
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
void

#import "src/lang/math" // 加载静态代码
time:Time
math:*

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
     2 从左到右    a?:b
     1 从右到左    a=b a+=b a-=b a*=b a/=b a%=b a<<=b a>>=b a&=b a^=b a|=b
     0 从左到右    a,b

为了省略大部分的分号并避免歧义，一元操作符使用特殊的语法形式： ::

    小括号包含类型用来定义类型或用作类型转换操作符，小括号包含值表示表达式的一部分。
    大括号只能包含值或由值组成的语句列表，值由变量常量操作符组成。
    取地址 & 改为 (&) 地址标记 (&1) (&2)
    解引用 * 改为 (*) (**) (*&) (**&) 地址引用 (*1) (*2)

    #negt()     #-          #-3.14      #-c         (-3.14) (-c)
    #posi()     #+          #+6.24      #+c         (+6.24) (+c)
    #comp()     #^          #^1024      #^c         (^1024) (^c)
    #addr()     (&)         #&data                  (&)data (*&)data
    #dref()     (*)         #*p         #**pptr     (*)p    (**&)ptr calc(-3.14, +6.28, ^c, &data, *p, **&ptr) 前面必须有分隔符，包括左括号（( [ {），逗号（,），或（#）

// 条件语句包含传统C的if和switch：
//  if cond { expr }
//  if cond return expr
//  if cond { expr } else if_stm
//  #if cond { expr } else { expr }
//  #if cond { expr } else if cond { expr }

if color case .RED { // 使用break会跳出外层for循环
    #fall
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
