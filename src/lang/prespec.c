// 符号可以分为变量和类型，变量的底层表示其实就是一个内存中地址，包括全局变量、局部变量、
// 函数、跳转标签都是一个地址。类型只是一种语法概念并不占据实际物理世界空间，但这两种符号
// 都可以进行调用，变量的调用其实就是函数，类型的调用相当于模板类型实例化变成具体类型。其
// 实除了变量和类型，还存在一种更概念上的符号称为记号，包括包名、宏名。
//
// 关键字，去掉 default 因为可以用 else 实现，而 fallthrough 可以用 #fall 表示。
//  if else case for range
//  continue break defer await yield return
//  const void pub adr der
//  static extern using
//  sizeof typeof alignof offsetof
//
// 函数参数或者结构体成员声明
//  Point point p2
//  *Point point p2
//  **Point point p2
//  [2]Point point p2
//  *[2]Point point p2
//  [2]*Point point p2
//
// 定义泛型类型参数（generic type parameter）compile time const 类型参数
//  $T $U            定义两个类型参数 T 和 U，相当于：
//  ${any} T U       上面简写形式例如 $T 之后词法元素，必须以$或其他标点开头，或是关键字、类型名
//  ${int} Int   整数类型
//  ${flo} Float   浮点类型
//  ${num} Type  数值类型
//
// 定义泛型常量参数（generic value parameter）compile time const 常量参数
//  $T a $U b $int c  定义三个常量参数 a 和 b 和 c，a 的类型是 T，b 的类型是 U，c 的类型是 int
//  $string a $int b  定义两个产量参数 a 和 b，a 的类型是字符串，b 的类型是整数
//
// 首字母大写，包括单个大写字母，并排除包含两个和两个以上字母但都是大写的标识符之后的所
// 有标识符都识别为类型名。包含在 @type{} 之内的标识符，也是类型名。
//
// 其他标识符符都是非类型标识符，包括函数名、变量名、标签、包名等等。另外包含在 @name{}
// 之内的标识符也都是非类型标识符，例如把类型名和关键字当作变量标识符使用：@name{int}
// @name{if} @name{Type}。
//
// 以 # 开头的标识符是编译器指令。以 @ 开头的标识符是属性名称，包括函数、类型、变量的
// 属性名称等。以双下划线开始的标识符，是保留关键字。
//
// 编译时常量：
//  #file #func #line #retp
// 编译时函数：
//  #equal #assert #align(16) #packed
// 编译时关键字：
//  #import #dont_import
//  #fall
//  #if 条件编译
// 编译和运行时
//  #comptime #runtime
// 预定义类型
//  this 当前函数（当前函数的地址）或当前结构体，不提供任何面向对象的特殊含义，但匿名类型需要用
// 操作符
// #- #+ #^
// (&) (*) (**) (*&) (**&) (&1) (&2) (*1) (*2) @noinit
//
// 基本类型，定义在 type 代码包中：
//  bool null byte char string errno
//  i08 i16 i32 i64 i128 int int(32) int(64) int(128) (b w d q x y z p r) byte word double-word quad-word xmm-word ymm-word zmm-word
//  u08 u16 u32 u64 u128 unt unt(32) unt(64) unt(128) ... Ptr SysPtr SysInt SysUnt
//  f08 f16 f32 f64 f128 float float(32) float(64) float(128)
//  d08 d16 d32 d64 d128 decimal decimal(32) decimal(64) ...
//  c08 c16 c32 c64 c128 complex complex(32) complex(64) ...
//
// rax rbx rcx rdx rbp rsp rsi rdi rlr rip rpc
// bax bbx bcx bdx bbp bsp bsi bdi blr bip bpc
//                                          arch-32     arch-64 small memory range app  arch-64 large memory range app
//  int - pointer size signed type          32-bit      32-bit                          64-bit
//  unt - pointer size unsigned type        32-bit      32-bit                          64-bit
//  Ptr - pointer type                      32-bit      32-bit                          64-bit
//  SysInt - system register size type      32-bit      64-bit                          64-bit
//  SysUnt - system register size type      32-bit      64-bit                          64-bit
//  SysPtr - system register width pointer  32-bit      64-bit                          64-bit
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
//  int  u16  f64  Point  MyInt
//  *int *u16 *f64 *Point *MyInt
//  [2]int [2]u16 [2]f64
//  *[2]int *[2]u16 *[2]f64
//  *int *u16 *f64 *Point
//  *[2]int *[2]u16 *[2]f64
//  (int a b) (int a b return int) (void) (return int) 参数必须带名称，返回值是一个类型列表不能带名称，函数如果没有参数必须带void
//  (Point p float factor) (Point p) (Point(N, T, U) p)
//  Function
//      (void) 没有参数，没有返回值，空括号 () 表示没有参数的函数调用
//      (return int) 没有参数，单个返回值，参数不能省略，如果没有参数必须声明为void，而返回值可以没有
//      (return int string) 没有参数，多个返回值
//      (int a return int)
//      (int a b)
//      (Point point float factor)
//      (Point p float factor) 结构体默认传递地址，因此不需要显式指定指针类型，如果不希望对象被函数修改，需要在调用前先拷贝
//      (int a b yield int i Ptr p return int) 带有协程中间结果的函数
//      (int a b [capture_c d] return [Ptr p] int) 带有捕获参数的闭包函数，闭包函数是在定义时一次性生成的，闭包参数只要名字相同且能完成函数内部的操作就没有问题
//      定义一个函数类型：Calc(int a b return int)
//      声明一函数字面量：extern calc(int a b return int)
//      声明一个函数指针：extern calc Calc
//      定义一函数字面量：calc(int a b return int) { return a + b }
//      定义一个匿名函数：(int a b return int) { return a + b }
//      定义一个函数指针；calc_ptr Calc calc
//      定义一个函数指针：calc_ptr Calc (int a b return int) { return a + b }
//      给一函数指针赋值：calc_ptr = calc
//      给一函数指针赋值：calc_ptr = (int a b return int) { return a + b }
//  Tuple 元组补充结构体表达不了的一些东西（一个类型列表）
//      (int int)            结构体不能同时定义两个同类型的内嵌字段
//      (this int int int)   结构体不能内嵌一个指针类型
//  Enum 枚举类型，只能表示整数常量，枚举是结构体模板的一种特殊形式
//      $i08 {RED {const * 2} YELLOW BLUE} // const 是枚举元素的索引值
//      $int {RED YELLOW BLUE}
//  Interface // 接口不能声明为空，必须包含成员函数声明，也只能包含成员函数声明或内嵌接口，接口是一个没有成员只有静态数据的结构体，接口声明也只是结构体模块的一种特殊形式
//      $T $(*T p int a b return int) calc $(*T p int c) get {}
//  Struct 表示定义一个类型
//      Type {}
//      { int a b } {1, 2}
//      { (this p int a b int) calc }
//      $T $U $int size { T[size] a U b }
//  常量类型，可以表示任意常量，包括结构体常量，常量类型定义一个对应类型的常量值，是一个值不是类型
//      const PI f16 3.14
//      const PI 3.1415926
//      const HI { int a float b string c Point p } { a 1 b 3.14 p(10, 30) }
//      const HI HiData { a(MAX_SIZE) b(3.14) c("hello") p{10, 30} } // 基本类型、结构体、复合类型的初始化怎么统一 ???
//      const fn (int a b int) calc_func
// 定义一个类型参数列表
//      Test $T $int a $string b {}
//      Test(Type, 1, "hello")
// 复合类型，匿名类型也可用于复合类型声明，不必先定义新命名类型再定义复合类型：
//      指针                数组                数组的指针          指针的数组
//      *int                [2]int              *[2]int             [2]*int
//      *f64                [2]f64              *[2]f64             [2]*f64
//      **int               [2][3]int           **[2][3]int         [2][3]**int
//      **f64               [2][3]f64           **[2][3]f64         [2][3]**f64
// 命名类型单指用户定义的类型，虽然基本类型也都是有名字的：
//  MyInt int
//  Func (int a b return int)
//  Func (void) 参数为空，返回为空
//  Func (return u32 bool) 参数为空，返回u32和bool
//  Method (Point p float factor)
//  Data { int a b }
//  Oper $T $(*T p int a b return int) calc {}
//  Color $int {RED YELLOW BLUE}
// 函数包含参数和捕获，函数调用时需要给参数提供实参而捕获不需要：
//  (int a b [m] return int) { ... }
//  (Point p float factor [m] return float) { ... }
// 定义模板类型，模板类型都是命名的，如果模板是基本类型，则需要使用常量进行实例化：
//  Ptr $T { *T p }
//  Pfp $T { **T pp)
//  Array $T $int size { [size]T a }
//  Slice $T { *T a int len }
//  Map $K $V { []{K key V value} slice }
//  Point ${num} T { T x y } // 只能给结构体定义模板，接口和函数都不能定义模板，但是模板类型可以作为函数的接收类型
//  Triple $T $unt size { [size]T a U b }
//  length (Point($T) p return T) { return p.x * p.x + p.y * p.y }
//  size (Triple($int N, $T, $U) p return int) { return N + sizeof T + sizeof U }
// 模板类型实例化：                                                      代码中仅允许使用这种简化的形式
//  ptr(int) ptr(ptr(int))                                              int* int**              type::int* int.type::ptr
//  array(int, 3)                                                       int[3]                  int.type::array(3)
//  slice(int)                                                          int.slice               int.type::slice
//  map(string, int)                                                    string.map(int)         string.type::map(type::int)
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
Get(*$T a int) // 函数参数只能声明类型模板参数
Read(*$T a Ptr p int n int)
Reader $T
    $Get(T) get
    $Read(T) read
{}

@type{get} (*$T a return int)
@type{get} (*${any} T a return int)
@type{read} (*$T a Ptr p int n return int)
@type{reader} $T $(@type{get}(T) get @type{read}(T) read {}

hci_rx_buffer_handled_and_free(Hci_Rx_Desc rx_desc u32 ca Hci_Data_Type data_type U line)

L2cap_Conn
L2cap_Callback
Hci_Conn_Type
Bt_Status
Btif_L2cap_Echo_Req_Callback
Hci_Global
Hci_Data_Type
Hci_Conn_Type
Hci_Conn_Item
Hci_Evt_Packet
Hci_Rx_Desc
Hci_Tx_Desc

Point { float x y }
Data { int a b (int a b return int) f g }
Reader $T $(*T p int a b return int) calc $(*T p []byte a) get {}
Get (*$T p return int)
Read (*$T p *byte buf int n return int)
Reader $T $Get(T) get $Read(T) read {}
Handle (*HciRxDesc rx_desc u32 ca HciDataType type U line)
using L2capConn         *L2capConn
using L2capCallback     *L2capCallback
using TcpSocket         *TcpSocket
using BtStatus          *BtStatus
using HciGlobal         *HciGlobal
using HciConnType       *HciConnType
using HciConnItem       *HciConnItem
using QueFit
using QueFix
using ArrFix
using ArrFit
using ArrDyn
using ArrLax

T1 int // 禁止
T3 T2 // 禁止
T4 { (int a b return int) f int a b }
T5 $T $(*T p int a b return int) calc {}
T6 (int a b return int)
T7 (Point p float factor)

T1 int[3] // 禁止
T3 T2[3] // 禁止
T4 { int a b }[3] // 禁止
T5 (this p int a b int)[3] // 禁止
T6 (int a b int)[3] // 禁止
T7 (Point p float factor)[3] // 禁止

T1 Triple#3(int[3], (int a b))
T2 Triple#3(string[3], (int a b)*)
T3 Triple#3(T2[3], ((# int a b int)[3]) calc))
T4 Triple#3((int a b)[3], (int a b int)[])
T5 Triple#3(((# int a b int)[3]) calc, (Point p float factor)[int])
T6 Triple#3((int a b int)[3], int*)
T7 Triple#3((Point p float factor)[3], string[int])

Node $T {
    this next
    T data
}

Triple $T $U $int size {
    T[size] a
    U b
}

Main (i32 argc **byte argv return i32)
Scale (Point p int a b)
Calc (int a b return int errno) // errno is a type with i32
result calc(1, 2)
if errno { // error handle
}
calc(1, 2) #abort(if errno)

using 3rd "array.code" // 文件可提供一个包名，也可以不提供
array 3rd::Array 3rd::array_init()
array 3rd::array_init()
array.append(2)
array += 3
array += 4

:::std::: export { // 必须紧跟在 ::: 之后
    Calc
    Coroutine
    Arrfit
    init(*Arrfit ~)
    free(*Arrlax ~)
}

pub Coro { // pub 赋予字段只读权限
    u32 rspoffset // 1st field dont move
    u32 loweraddr
    u32 maxudsize 31 ptr_param 1
    i32 coro_id
    unt rspoffset
    unt loweraddr
    unt maxudsize 31 ptr_param 1
    int coro_id
    unt rspoffset
    unt loweraddr
    unt maxudsize 31 ptr_param 1
    int coro_id
    Ptr address
}

pub CoroGuard {
    u32 lower_guard_word
    u32 upper_guard_word
    Coro @embed
    *Coro coro
    this @embed
    this coro_guard
    (int a b return int) calc
    (*Coro p) func
}

Color $i08 {RED {const + 1} BLUE YELLOW}
Color $int {red blue { blue_defined_value } yellow}
Color $int [[strict]] {RED {1} BLUE {2} YELLOW {3}}
Color $u08 { red blue { blue_defined_value } yellow }

CoroGuard { // 内嵌只能内嵌结构体类型，不能是指针
    u32 lower_guard_word
    u32 upper_guard_word
    Coro #embed // 不能内嵌两个相同类型
    *Coro coro_ptr
    this #embed // 错误，指针不能内嵌
    this coroguard
    (int a b return int) calc
    (int a) print
    (this p int a b return int) a
    (CoroGuard g int a b return int) f g
    (Coro p) h
    { int a b }
    { int a b } tuple
    $i08 (RED 1 BLUE YELLOW) color
    $int (red {red_enum_value} blue yellow) color
)

Color $i08 {
    RED {const + 1}
    BLUE
    YELLOW
}

Color $int {
    red
    blue {blue_defined_value}
    yellow
}

Color $i08 @strict { // strict 枚举类型必需为全部枚举手动指定值，并在代码更新时不能修改这些值，以防带来代码版本的不兼容
    1 RED
    2 BLUE
    3 YELLOW
}

BitValue $int {
    (1 << const) FLAG_BIT1
    FLAG_BIT2
    FLAG_BIT3
    FLAG_BIT4
}

Main (i32 argc **byte argv return i32)
Scale (Point point int a b)
Calc (int a b return int)
Array $T $int size { [size]T a }
Color $i08 {RED 1 BLUE 2 YELLOW 3}
BitValue $int {FLAG_BIT1 {1 << const} FLAG_BIT2 FLAG_BIT3}

TcpAction $int {
    TCPA_OPEN_ACCEPT
    TCPA_TX_DATA
    TCPA_RX_DONE
    TCPA_CLOSE_REQ
    TCPA_CLOSE_CFM
    TCPA_EPOLL_IND
}

TcpAccept {
    ConoPdata head
    u32 rxbuf_size
    u32 txbuf_size
}

Writer $T
    $(*T p int c) put
    $(*T p Ptr p int n return int) write
{}

Color $i08 {} // 因为i08是关键字，不能使用关键字定义新的类型参数，因此这里必须是一个常量参数

TypePtr enum { // enum 用来定义 sum type/tagged union type
    ptr (Ptr)
    null #occupy Ptr 0 // @occupy 只能用于两个元素的enum
}

Token enum { // enum 定义的是一个联合体类型
    atom {char id}
    oper {char id}
    eof
}

Expr enum { // enum 定义的是一个联合体类型
    value { int n }
    ident { int id }
    expr {char op Expr lhs rhs}
}

Oper $int {int lpri rpri} { // $int 定义的是一个常量
    ass '=' => {200, 201} // a = 2 + b = 3
    add '+' => {211, 210}
    sub '-' => {211, 210}
    mul '*' => {221, 220}
    div '/' => {221, 220}
    pow '^' => {230, 231}
    dot '.' => {251, 250}
    end 0 // 默认值为零
}

eat (*Lexer lexer return Token) {
    return lexer.pop()
}

peek (*Lexer lexer return Token) {
    return lexer.top()
}

eval (Oper op Expr lhs rhs return Expr) {
    res Expr ???
    if op case '=' {
        res = .value(rhs.value.n)
        get_symbol(lhs.ident.id).value = rhs.value.n
    } case '+' {
        res = .value(lhs.value.n + rhs.value.n)
    } case '-' {
        res = .value(lhs.value.n - rhs.value.n)
    } case '*' {
        res = .value(lhs.value.n * rhs.value.n)
    } case '/' {
        res = .value(lhs.value.n / rhs.value.n)
    } case '^' {
        res = .value(pow(lhs.value.n, rhs.value.n))
    } else panic("bad operator %c", op)
    return res
}

parse_expression (*Lexer lexer int min_prior) {
    lhs Expr ???
    if lexer.eat() case .atom(it) {
        if it == '0'..'9' {
            lhs = .value(it - '0')
        } else if it == 'a'..'z' || it == 'A'..'Z' {
            lhs = .value(get_symbol(it).value)
        } else panic("bad token %d", it)
    } case .oper('(') {
        lhs = eval(parse_expression(lexer, 0)
        assert_eq(lexer.skip(), Token.oper(')'))
    } else panic("bad token %d", it)

    for {
        op Expr ???
        if lexer.peek() case .eof .oper(')') {
            break
        } case .oper(it) {
            op = .expr(it)
        } else panic("bad token %d", it)
        lexer.skip()

        prior match(Oper, op.expr.op)
        if prior.lpri < min_prior {
            break
        }

        rhs parse_expression(lexer, prior.rpri)
        lhs = eval(op.expr.op, lhs, rhs)
    }

    return lhs
}

// a * b + c / 3
// =>   Token.atom('a')
//      Token.op('*')
//      Token.atom('b')
//      Token.op('+')
//      Token.atom('c')
//      Token.op('/')
//      Token.atom('3')
//      Token.eof()
// =>   Expr.operation('+', =>){
//          Expr.operation('*', =>) {
//              Expr.atom('a'),
//              Expr.atom('b'),
//          },
//          Expr.operation('/', =>) {
//              Expr.atom('c'),
//              Expr.atom('3'),
//          }
//      }

ptr Ptr alloc(1024)
ptr.unwrap_or_panic()

token Token scan()
if token case .eof {
}

perform_tcpa_open_accept(*TcpSocket tcp u32 txbuf_size u32 rxbuf_size) {
    pdata *TcpAccept cono_malloc_pdata(TCPA_OPEN_ACCEPT, TCPQ_UPPER, true, sizeof TcpAccept)
    pdata.rxbuf_size = rxbuf_size
    pdata.txbuf_size = txbuf_size
    cono_freely_post(tcp.tcp_coro, pdata)
}

report_tcpe_opened(*TcpSocket tcp) {
    pdata *TcpOpened tcpa_post_pdata(tcp, TCPE_OPNED, sizeof TcpOpened)
    txbuf *ByteArrfit addrof tcp.txbuf
    pdata.tcp = tcp
    pdata.txbuf = arrfit_begin(txbuf)
    pdata.size = txbuf.size
    cono_freely_post(tcp.upp_coro, addrof pdata->head)
}

epoll_proc(*Cono cono) {
    epoll *Epoll cono_data(cono)
    pdata *ConoPdata __uninit
    action byte __uninit
    for {
        pdata = cono_pwait(cono)
        action = pdata.action
        if action == COAC_EXIT break
        if action case EPAC_DEL_CLOSE {
            epac_del_close(epoll, (int)pdata.u.value)
        } case EPAC_POLL_ONCE {
            epac_wait(epoll)
        } case EPAC_POLL_ALL {
            for epac_wait(epoll) void
        } else {
            debug(prerr(action));
        }
    }
}

pub main(int argc **byte argv return int) {

}

Main (i32 argc **byte argv return i32)
scale (Point point int a b)
calc (int a b return int)
Array $T $int size { [size]T a }
Color $i08 [[strict]] {RED {1} BLUE {2} YELLOW {3}}
BitValue $int {FLAG_BIT1 {1 << const} FLAG_BIT2 FLAG_BIT3 FLAG_BIT4}

TcpAction $i08 {
    TCPA_OPEN_ACCEPT
    TCPA_TX_DATA
    TCPA_RX_DONE
    TCPA_CLOSE_REQ
    TCPA_CLOSE_CFM
    TCPA_EPOLL_IND
}

TcpAccept {
    ConoPdata head
    u32 rxbuf_size
    u32 txbuf_size
}

perform_tcpa_open_accept (*TcpSocket tcp u32 txbuf_size u32 rxbuf_size) {
    pdata *TcpAccept cono_malloc_pdata(TCPA_OPEN_ACCEPT, TCPQ_UPPER, true, sizeof TcpAccept)
    pdata.rxbuf_size = rxbuf_size
    pdata.txbuf_size = txbuf_size
    cono_freely_post(tcp.tcp_coro, pdata)
}

report_tcpe_opened(*TcpSocket tcp) {
    pdata *TcpOpened tcpa_post_pdata(tcp, TCPE_OPNED, sizeof TcpOpened)
    txbuf *ByteArrfit adr tcp.txbuf
    pdata.tcp = tcp
    pdata.txbuf = arrfit_begin(txbuf)
    pdata.size = txbuf.size
    cono_post(adr pdata->head)
}

epoll_proc(*Cono cono) {
    epoll *Epoll cono_data(cono)
    pdata *ConoPdata __novalue
    action byte __novalue
    for {
        pdata = cono_pwait(cono)
        action = pdata.action
        if action == COAC_EXIT break
        if action case EPAC_DEL_CLOSE {
            epac_del_close(epoll, (int)pdata.u.value)
        } case EPAC_POLL_ONCE {
            epac_wait(epoll)
        } case EPAC_POLL_ALL {
            for epac_wait(epoll) void
        } else {
            debug(prerr(action));
        }
    }
}

// 结构体参数是该结构体定义的所有变量都共享的参数，即静态数据
Writer [[comptime]] $T // 可以有两种实现，一种时编译时实现速度快但代码大，一种时运行时实现速度慢一点但代码小
    $(*T p int c) put
    $(*T p Ptr p int n int) write
{}

// File类型的静态数据布局：
// File.Writer  put(File* self int c)
//              write(File* self Ptr p int n int)
// File.Reader  get(File* self)
//              read(File* self Ptr p int n int)
File {
    Ptr fd
    #impl Writer(File, file_put, file_write)
    #impl Reader(File, file_get, file_read)
}
generic_write (*Writer writer) { // 实际上参数会传递 file 以及 File.Writer 静态数据的地址
    func writer.write
    func(writer)
} // 如果是编译时实现，对每个实现类，都会生成一个特定的 genric_write 函数，其中的 func 会直接替换为实际函数，这种泛型实现是一种非常灵活的填鸭式的代码实现

Calc (int a b return int)

Snode $T {
    this next
    T data
}

for i I 0 .. 9 {
    i int der *I addr
    pos + der adr *I (*byte p + size + f(g))
}
memcpy (Ptr dst src int count) [[intrinsic]]
memcmp (Ptr dst src int count return int) [[intrinsic]]
memset (Ptr dst byte value int count) [[intrinsic]]
lock_cmpxchg (*T p T old new return T) [[intrinsic]]
coroguard (*Coro coro return CoroGuard) [[cdcel inline]]

Calc (int a b return int)
Snode $T { this next T data }
for i I 0 .. 9 {
    i int der *I addr
    pos + der adr I (*byte p + size + f(g))
}
memcpy (Ptr dst src int count)
memcmp (Ptr dst src int count return int)
lock (*T p T old new return T)
coro_guard (*Coro coro return CoroGuard)

// 类型并不需要提前声明，因为可以通过词法直接分辨，函数类型如果可以通过传递的参数匹配也无需提前声明
using T1
using T2
using T3
using T4
using T5
using T6
using T7
using Calc
using MyInt
using Coro

// 定义变量，包括函数变量，一个非类型标识符后跟一个类型表示定义该类型的一个变量
CoroProc(*Coro coro)
print(##string fmt $T ... return int)

main (int argc **byte argv return int) {
    return 0
}

calc (int a b return int) {
    return a + b
}

scale (Point p float factor) {
    p.x *= factor
    p.y *= factor
}

scale (Point p float factor) [[cdecl]] {
    p.x *= factor
    p.y *= factor
}

scale (Point p float factor) [[fastcall]] {
    p.x *= factor
    p.y *= factor
}

next (Node($T) p return *T) {
    return p.next
}

size (Triple($int N, $T, $U) p return int) {
    return p.a + N + sizeof T
}

data { int a b } {1, 2}
data *{ int a b } adr {1, 2}
data [2]{ int a b } {{1, 2}, {3, 4}}
data Data {1, 2}
data *Data adr data
data Data[2] {data1, data2}

found .. index array_find(<<array, item)
found .. error array_find(<<array, item)

cal2 (int a b return int) { return a + b }
cal2 (int a b return int) { return a + b } (1, 2)
cal2 *(int a b return int) (int a b int) {return a + b } // 函数不需要声明成指针，因为它本身就是指针
cal2 *(int a b return int) Calc {return a + b }
cal2 [2](int a b return int) {Calc {return a + b}, Calc { return a * b }}
cal2 Calc { return a + b }
cal2 *(int a b int) adr {return a + b }
cal2 [2](int a b int) {Calc {return a + b}, Calc { return a * b }}
cal2 *Calc adr (int a b int) {return a + b }
cal2 *Calc adr {return a + b }
cal2 adr Calc{return a + b}
cal2 Calc[2] {Calc {return a + b}, Calc { return a * b }}
cal2 Calc[2] {Calc {return a + b}, Calc { return a * b }}

ppb *Ppb malloc(size)

a int ???
a 42
a #+99
a if a==0 ? b : c
b 0
b a // 一个变量赋值给另一个变量的形式必须在同一行，不能换行
c #-17
d 'a
d \n
e 's' 'r' \s \t 'c' '/' \n
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
cal2 calc;

dat3 *{ int a b } adr {3, 4}
dat3 [2]{ int a b } {{3, 4}, data}
dat3 *{ int a b } adr {3, 4}
dat3 *Data adr data
dat3 [2]Data {{3, 4}, data}

cal3 *(int a b int) adr { return a + b }
cal3 *(int a b int) adr calc
cal3 (int a b int) { return a + b }
cal3 Calc { return a + b }
cal3 calc;

// 基本类型和指针不通过大括号进行初始化而是直接给值，根据语法定义一个变量的同时必须进行初始化
// 一个非类型标识符后跟一个类型表示定义该类型的一个变量
// 一个非类型标识符后跟一个非类型标识符，表示用后面的非类型标识符的值定义一个变量
// 一个非类型标识符后跟一个字面常量，表示用字面常量定义一个变量

cal3 *(int a b return int) null
cal3 *(int a b return int) adr { a + b }
cal3 *(int a b return int) calc

numb errno null
numb float 3.14
numb *int null
numb *int adr data
numb bool false

data false
data "Hello World"
data 3.14
data 1024 // 可以使用字面量直接赋值
data calc // 但如果是变量值，没有类型转换的情况下必须使用大括号赋值

calc Calc { a + b }
data Data {1, 2}
data int 1024
numb errt null
numb float 3.14
numb *int adr data
calc Calc { a + b }
data Data {1, 2}
data int 1024
numb errt null
numb float 3.14
numb *int adr data
temp int 1024
temp float 3.14

temp int calc(1, 2)
temp Calc { a + b } (1, 2)
temp Calc { a + b } (1, 2)
temp (int a b int) { a + b } (1, 2)

// 同一行连续的语句必须使用逗号分隔
i int 0, sume 0
bytes {more}, same_as_prev {userdata_bytes != 0}

// 数据类型转换

aaa Data{3, 4} // 赋值语句因为目标变量只有一个，因此只要将类型不加修饰的放在右表达式开头，相对于对整个表达式类型的转换
ppb *Ppb ppb_alloc(alloc)

pos dist + int scale_x(facter)
len int pos + adr *int *byte p + size + f(g)
len int pos + adr (*int *byte p) + size + f(g)

pos int dist + int scale_x(facter)
len int pos + der *int (*byte p + size + f(g))
len typeof(pos) 3
len 3

for i int 3 .. 10 { /* */ }

for ^^(capacity < new_capacity) { // ^^ 第一次条件检查一定为真
    capacity *= 2
}

// 函数和普通变量提前声明，同一个变量声明可以出现多次，定义一个变量时必须有初始化也即
// 推荐仅在使用的地方才进行变量定义不提前定义变量
// 类型不需要提前声明，因为其名称可以由词法分辨
extern calc (int a b return int)
extern scale (Point p float factor)
extern data { int a b }
extern calc Calc
extern data Data
extern data int
extern numb errt
extern numb float
extern numb *int

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
member_since Date''{1999-08-04}
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
    {---string color shape
        {---int height width
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
fruit_type name "pug"

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
    取地址 & 改为 (&) 地址标记 (&1) (&2) adr
    解引用 * 改为 (*) (**) (*&) (**&) 地址引用 (*1) (*2) der

    #negt()     #-          #-3.14      #-c         (-3.14) (-c)
    #posi()     #+          #+6.24      #+c         (+6.24) (+c)
    #comp()     #^          #^1024      #^c         (^1024) (^c)
    #addr()     (&)         #&data                  (&)data (*&)data    adr data    der adr data
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

// 函数支持默认参数，但不支持函数名重载，但支持第一个参数重载，但支持操作符重载+ - * / == != << >> & | [] % ^ <<< >>> []= .&，#symmetric
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
->> (+ 12 24 13)
->> (* 13 14 15)

(->> theme
     (symbol-name)
     (funcall (-flip #'concat) "-theme")
     (intern))

(intern (funcall (-flip #'concat) "-theme" (symbol-name theme)))

// 0. 代码编写原则
//
// 不要过度的进行输入参数检查，也不要让程序用治标不治本的方法自动修正错误，这只会种下
// 更大的隐患。例如下面的例子，因为越界参数的调用，已经说明程序前面的逻辑发生了错误，隐
// 瞒得越久，越不容易找到真正导致错误的位置。很多人对自己的程序没有信心，觉得出错是理所
// 必然的事情，所以他们用尽一切办法在错误发生的时候予以挽救。而实际情况是，好的程序代
// 码，各部分逻辑清晰完备自洽，就像这个功能就只有这一种自然而然的正确写法，没有多余的代
// 码，更不需要检查各种边界条件维持程序的正确性。如果你的代码丑陋不堪，更可能是代码设计
// 的原因。
//      int at(intarray* a, int i) {
//          if (i < 0 || i >= a->size) i = 0;
//          return a->data[i];
//      }
//
// 相比在局部指标不治本的自动修正错误，在代码开发阶段就及早的将错误暴露出来，才是正确
// 的做法。例如上面的检查，更好的方法是写一个 assert。但是要主要正确使用 assert，
// assert 的使用一定不能影响正常的程序逻辑，也就是说，所有的 assert 语句必须是可以去
// 掉的，在 assert 的表达式中改变变量的值都是不允许的。另外，导致断言失败的原因必须
// 是程序错误，不能用 assert 去充当实际真正执行的代码逻辑的检查。
//      assert(i >= 0 && i < a->size);
//
// 在开发阶段，用 assert() 暴露错误直接让程序死掉，对于开发人员是最好的方案了。一个崩
// 溃不能继续执行的程序最好地揭示了程序中存在的问题，并且崩溃点离真正出错的位置越近，
// 越易于修正问题。而用欺骗的手法让程序继续运行，欺骗了机器也欺骗了自己。
//
// 开发阶段尽量多地给必要的代码加上运行时检查，就能够尽可能早的发现隐藏的问题。如果你
// 使用了一个第三方库，它频频弹出 assert 窗口，永远不要抱怨。你应该感谢库的设计人员，
// 是他们尽责指出了你自己代码中的问题。或许在发生 assert 的那一刻，你无法发觉附近的程
// 序哪里写错了，觉得 assert 毫无道理，甚至希望就地做一些判断阻断断言的再次失败。哦那
// 可真是一个危险的念头，真的这样干了，下次发生更为严重的错误时就不会是弹出 assert 窗
// 口，而是程序莫名其妙崩溃了。
//
// 不要滥用条件编译指令，这往往是引出问题的根源之一，滥用这些很容易导致多个版本的行为
// 不一致，错误很难查出。我们应该根据实际想表达的意思分类，把 #ifdef 出现的次数降低，
// 例如可用 FORDEBUG(p = 0;) 替代下面的条件编译：
//      free(p);
//      #ifdef _DEBUG
//      p = 0;
//      #endif
//
//  1.  在一个函数中执行一个回调函数的解耦简化
//
//  第一种情况，调用一个函数，其中一个参数是回调函数，且该函数会立即返回。这是将被调
//  函数实现为一个 soro 协程，被调函数被调用时自动分配一个自己使用的栈，需要回调的时
//  候直接yield让主调函数执行，然后主调函数继续切到被调函数，指定执行完毕。
//
//      for [i, elem] array_find_and_erase(int_array, 32) { 封装与开放的平衡
//          printf("int array elem 32 index %d at %p erased\n", i, elem);
//      }
//
// 2. 函数参数的传递
//
// 10. 协程的实现
//
// 对于 64 位系统，进程虚拟空间足够大，每个协程可以映射一个足够大的内存来使用，并且按
// 需对内存进行实际提交。因为用户空间有 128T 可用，而每 1T 虚拟空间可以给100万个协程
// 每个协程都预留1MB空间。
//
// 对于 32 位系统，因为自己的编程语言可以知道每个函数需要的最大栈空间是多少，可以实际
// 分配对应的栈大小即可。对于递归调用的情况，每次递归调用再次该该函数分配对应大小的栈
// 空间之后才继续执行，相当于创建了同一个函数的多个实例，然后随着递归的进行而分配和释
// 放。