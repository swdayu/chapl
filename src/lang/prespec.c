// 符号可以分为变量和类型，变量的底层表示其实就是一个内存中地址，包括全局变量、局部变量、
// 函数、跳转标签都是一个地址。类型只是一种语法概念并不占据实际物理世界空间，但这两种符号
// 都可以进行调用，变量的调用其实就是函数，类型的调用相当于模板类型实例化变成具体类型。其
// 实除了变量和类型，还存在一种更概念上的符号称为记号，包括包名、宏名。
//
// 关键字，去掉 default 因为可以用 else 实现，而 fallthrough 可以用 continue 代替。
//  if else elif for in break return 条件语句支持大括号和缩进对齐两种编写方式
//  const void embed let pub def undefined strict
//  continue defer yield range lambda reflex trait
//  static where it or this type import using scoped
//  with adr der todo debug trap prfer local global
//  mod mut ref gen priv do abstract final macro
//  alignof type  sizeof type  offsetof type.offset
//
//  defer_if_error deallocation(ptr)
//
// act all are ago alt any auf aut
// can cat cor con cue des dhu din don dor dyn
// fac far fat fen fer fin fit fou fro fry fur
// gen gre lot off per pat pal phr par
// pre pro rem res rim ron rou rut
// tie via was wow yet as of do use
//
// 特殊名称：
//  e_ 预留给错误码字符串，预定义的错误码，只保存错误数值
//  E_ 预留给错误码字符串，动态分配的错误码，保存完整字符串
//  __name__ 以双下划线开始和结尾的名称都是保留关键字
//
// 符号属性：
//  alignas(n) "fastcall" "cdecl" "stdcall" "strict" // 函数属性名称为了美观不使用@前缀
//  @maybe(none) @nonzero @nonalls @zeroinit @packed
// 内置函数：
//  abort() panic() assert(expr) debug { stmt ... }
//  real_assert(expr) alignof(vsym) sizeof(expr) typeof(expr)
//  copyof(vsym) moveof(vsym) zeroof(vsym) fillof(vsym)
//
// 语句不会返回值，可以通过语句表达式 ({}) 为语句块返回一个值，或者换成使用 with 和
// where 关键字一前一后让语句变成语句表达式，从而产生一个结果值。
//
// 函数参数或者结构体成员声明
//  Point point p2          struct point point p2
//  *Point point p2         struct *point point p2
//  **Point point p2        struct **point point p2
//  [2]Point point p2       struct [2]point point p2
//  *[2]Point point p2      struct *[2]point point p2
//  [2]*Point point p2      struct [2]*point point p2
//
// 定义泛型类型参数（generic type parameter）compile time const 类型参数
//  $T $U            定义两个类型参数 T 和 U，相当于：
//  ${any} T U       上面简写形式例如 $T 之后词法元素，必须以$或其他标点开头，或是关键字、类型名
//  ${int} Int   整数类型
//  ${flo} Float   浮点类型
//  ${num} Type  数值类型
//  $struct t $U        // 为了清晰，$T ${any} T 定义类型参数只能使用首字母大写形式
//  $T $struct u        // 禁止
//  ${any} struct t U   // 禁止
//  ${any} T struct u   // 禁止
//  struct $t a $U b    // 但使用已定义类型定义常量参数是允许的
//  $T a struct $u b
//  ---
//  $T $U        // 相当于定义了两个类型参数 def t 和 def u
//  $any T U   // 与上一行等价
//  $int Int          // 相当于定义了一个类型参数 def Int，该类型必须是一个整型
//  $int Float
//  $num Type
//  const string a b
//  const int a b
//  const point a b
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
// @name{if} @name{Type}。      @{if}   def @{if}
//
// 以 # 开头的标识符是编译器指令。以 @ 开头的标识符是属性名称，包括函数、类型、变量的
// 属性名称等。以单下划线或双下划线开始的标识符，是保留关键字。
//
// 编译属性：
//  @type{}
//  @name{}
//  @zeroinit
//  @useas
//  @align()
//  @packed
//  @cdecl
//  @stdcall
//  @fastcall
//
// 下划线保留字：
//  __file__
//  __func__
//  __line__
//  __retp__
// 编译时函数：
//  static_assert
//  #if 条件编译
// 预定义类型
//  this 当前函数（当前函数的地址）或当前结构体，不提供任何面向对象的特殊含义，但匿名类型需要用
// 操作符
// (&) (*) (**) (*&) (**&) (&1) (&2) (*&1) (*&2)
//
// 基本类型，定义在 type 代码包中：
//  bool null none byte char rune string errno struct "32-byte" i32 u32 int unt sys_int sys_unt sys_ptr struct i struct u struct p
//  i08 i16 i32 i64 i128 int <32>int <64>int <128>int (b w d q x y z p r) byte word double-word quad-word xmm-word ymm-word zmm-word
//  u08 u16 u32 u64 u128 unt <32>unt <64>unt <128>unt ...
//  f08 f16 f32 f64 f128 float <32>float <64>float <128>float
//  d08 d16 d32 d64 d128 decimal <32>decimal <64>decimal ...
//  c08 c16 c32 c64 c128 complex <32>complex <64>complex ...
//
//  bool byte char string none null true false unsigned
//  i08 i16 i32 i64 i128 i256 i512 int arch_int type error
//  u08 u16 u32 u64 u128 u256 u512 unt arch_ptr type ptr
//  f08 f16 f32 f64 f128 f256 f512 float
//  d08 d16 d32 d64 d128 d256 d512 decimal
//  c08 c16 c32 c64 c128 c256 c512 complex
//
// 简洁尽量实现使用最少字符
//  布尔类型    布尔常量    空值    字符类型    字符串类型
//  'B          @t  @f      @z      'c          's
//  bool        true false  null    char        string
//  单字节  双字节  四字节  八字节  指针或向量大小
//  '1      '2      'i      '8      'j      sys_int      有符号整数
//  'b      'w      'u      'q      'p      sys_ptr    无符号整数
//  f08     f16     'f      'g      'x 'y 'z            单精度/双精度/128-bit xmm/256-bit ymm/512-bit zmm
//  i08     i16     int     i64     'j      sys_int
//  u08     u16     u32     u64     'p      sys_ptr
//
// 类型信息：type|flag
//  i08 ~ <128>int      0   ~   15
//  f08 ~ <128>float    16  ~   31
//  d08 ~ <128>decimal  32  ~   47
//  c08 ~ <128>complex  48  ~   63
//  string 64 array 65 slice 66 view 67 struct 68 sumt 69 func 70
//  int/sys_int: ixx|int: 1
//  unt/sys_unt: ixx|unt: 1
//  ptr/sys_ptr: uxx|ptr: 1
//  bool: u08|bool:1
//  null: u08|null:1
//  char: u32|char:1
//  errno: int|errno: 1
//  enum: ixx|enum: 1
//
// rax rbx rcx rdx rbp rsp rsi rdi rlr rip rpc
// bax bbx bcx bdx bbp bsp bsi bdi blr bip bpc
//                                          arch-32     arch-64 small memory range app  arch-64 large memory range app
//  int - pointer size signed type          32-bit      32-bit                          64-bit
//  unt - pointer size unsigned type        32-bit      32-bit                          64-bit
//  ptr - pointer type                      32-bit      32-bit                          64-bit
//  sys_int - system register size type     32-bit      64-bit                          64-bit
//  sys_unt - system register size type     32-bit      64-bit                          64-bit
//  sys_ptr - system register width pointer 32-bit      64-bit                          64-bit
//
// 类型约束：
// Any Integer Float Unsigned Decimal Complex BasicType AnonyType NamedType GimplType
//
// 类型字面量（TypeLit）包括基本类型（BasicType）、匿名类型（AnonyType)、命名类型（NamedType）、实列化类型（ImpledType）。
// any          // basic_type + anony_type + named_type + gimpl_type
// basic_type   // numeric + string
// numeric      // integer + float + decimal + complex
// integer      // 枚举类型 bool null byte rune errno strt i08~i512 u08~u512 int unt
// instant_type
// generic_type
//
// 复合类型和匿名类型：
//  int  u16  f64  Point  MyInt
//  *int *u16 *f64 *Point *MyInt
//  [2]int [2]u16 [2]f64
//  *[2]int *[2]u16 *[2]f64
//  *int *u16 *f64 *Point
//  *[2]int *[2]u16 *[2]f64
//  (int a b) (int a b int) (void) (void int) 参数必须带名称，返回值是一个类型列表不能带名称，函数如果没有参数必须带void
//  (Point p float factor) (Point p) (Point(N, T, U) p)
//  Function   (void)  (int) (def point float factor return point)
//      (void) 没有参数，没有返回值，空括号 () 表示没有参数的函数调用
//      (return int) 没有参数，单个返回值，参数不能省略，如果没有参数必须声明为void，而返回值可以没有
//      (return int string) 没有参数，多个返回值，return 之后为了清晰只罗列类型名称
//      (int a return int)
//      (int a b)
//      (def point float factor) 结构体默认传递地址，因此不需要显式指定指针类型，如果不希望对象被函数修改，需要在调用前先拷贝
//      (int a b yield int i def ptr return int) 带有协程中间结果的函数
//      (int a b return int) [capture d] 带有捕获参数的闭包函数，闭包函数是在定义时一次性生成的，闭包参数只要名字相同且能完成函数内部的操作就没有问题
//      定义一个指针类型：Calc (int a b int)
//      声明一函数字面量：extern calc(int a b int)
//      声明一个函数指针：extern calc Calc
//      定义一函数字面量：calc (int a b int) { return a + b }
//      定义一个匿名函数：(int a b int) { return a + b }
//      定义一个函数指针；calc_ptr Calc calc
//      定义一个函数指针：calc_ptr Calc (int a b int) { return a + b }
//      给一函数指针赋值：calc_ptr = calc
//      给一函数指针赋值：calc_ptr = (int a b int) { return a + b }
//  Tuple 元组补充结构体表达不了的一些东西（一个类型列表）
//      {int int}            结构体不能同时定义两个同类型的内嵌字段，等价于结构体 {int @{0} int @{1}}
//      {this int int int}   结构体不能内嵌一个指针类型，等价于结构体 {this @{0} int @{1} int @{2} int @{3}}
//      def data {this int int int} 元组成员的命名，可以延迟到使用时
//      let ptr, a, b, c = data {this, 1, 2, 3}     // 可以使用 ptr a b c
//      let _, a, _, b = data {this, 1, 2, 3}       // 可以使用 a b，第一个成员只能使用 this 初始化，否则报错
//      let data = data {this, a = 1, b = 2, 3}     // 可以实现对元组的修改 data.a = 10  data.b = 20
//      let data = data {this, 1, 2, 3}             // 可以使用 data.0 data.1 data.2 data.3
//      def data = {this, a = 1, b = 2, 3}          // 可以实现对元组的修改 data.a = 10  data.b = 20
//      def data = {this, 1, 2, 3}                  // data.0 不能修改 data.1 = 10  data.2 = 20
//  Enum 枚举类型，只能表示整数常量，枚举是结构体模板的一种特殊形式
//      const i08 {RED const * 2, YELLOW, BLUE} // const 是枚举元素的索引值
//      const int {RED, YELLOW, BLUE}
//  Interface // 接口不能声明为空，必须包含成员函数声明，也只能包含成员函数声明或内嵌接口，接口是一个没有成员只有静态数据的结构体，接口声明也只是结构体模块的一种特殊形式
//      $p { (*p int size return int) read (*p return int) get } // 允许使用关键字 this 定义 $this，然后参数声明使用 (this int size return int)
//  Struct 表示定义一个类型
//      def empty {}
//      { int a b } {1, 2}
//      { (this int a b int) calc }
//      $T $U const (int size) { T[size] a U b }
//  常量类型，可以表示任意常量，包括结构体常量，常量类型定义一个对应类型的常量值，是一个值不是类型
//      const f16 PI = 3.14
//      const PI = 3.1415926
//      const { int a float b string c Point p } HI = { a 1 b 3.14 p(10, 30) }
//      const HiData HI = { a(MAX_SIZE) b(3.14) c("hello") p{10, 30} } // 基本类型、结构体、复合类型的初始化怎么统一 ???
//      const (int a b return int) FN = calc_func
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
//  Func (int a b int)
//  Func (void) 参数为空，返回为空
//  Func (return u32 bool) 参数为空，返回u32和bool
//  Method (Point p float factor)
//  Data { int a b }
//  Oper $T $(*T p int a b int) calc {}
//  Color $int {RED YELLOW BLUE}
// 函数包含参数和捕获，函数调用时需要给参数提供实参而捕获不需要：
//  (int a b int) [m] { ... }
//  (Point p float factor float) [m] { ... }
// 定义模板类型，模板类型都是命名的，如果模板是基本类型，则需要使用常量进行实例化：
//  Ptr $T { *T p }
//  Pfp $T { **T pp)
//  Array $T $int size { [size]T a }
//  Slice $T { *T a int len }
//  Map $K $V { []{K key V value} slice }
//  Point ${num} T { T x y } // 只能给结构体定义模板，接口和函数都不能定义模板，但是模板类型可以作为函数的接收类型
//  Triple $T $unt size { [size]T a U b }
//  length (Point($T) p T) { return p.x * p.x + p.y * p.y }
//  size (Triple($int N, $T, $U) p int) { return N + sizeof T + sizeof U }
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

// 让类型字面量和复合常量字面量表示唯一，其他都必须为之让路
// 函数类型字面量，函数复合常量是函数类型字面量 + { stmt ... }
(void)
(return int) // return 和 yield 之后可以放心的都是类型名
(return int string point)
(int a return int)
(type point int a return int) // 纯类型名称必须使用 type 前缀，但是 yield return 后不能使用 type 前缀
(type point type camera) // 共两个参数
(type point point)
(type point type point) // 错误，两个同名参数
(int a b)
(type point float factor) [m]
(int a b yield int point return int)
(int a b return int)
(int a) // 可能是将变量a进行类型转换
(*file @maybe(none) = {stdin} type point string name = {"root"} string mode return int) // 默认值 {} 之后必须跟类型名称
// 元组类型和结构体类型字面量
{ }
{int int type point string} // 纯类型名称必须使用 type 前缀
{this type point int float}
{type point} // 怎么区分是结构体还是元组呢，是元组，因为结构体成员必须声明名称，但这里其实是一样的，因为元组同样可以通过类型名point访问这个成员
{type point point} // 第一个一定是类型名称，point 是 point 类型成员
{int point} // point 是 int 型类型成员
{int a b}
{int a} // 可能是将变量a类型转换后给成员赋值
const { red, green, blue }
const int { red, green = 2, blue }
$p { (*p int size return int) read }
// 大括号初始化列表，或者称为元组字面量表达式
{expr, expr, expr} // expr 绝对不会以类型名称或类型字面量开头
// 字面量数组及相关
*[N]Type // [N] [] [_] * ** 的排列组合
[*]Type // 指向的内容是一个 Type 类型的数组，长度不定，另外 *Type 表示指向单个 Type 值
*[N]Type // 指向的内容是一个 [N]Type 类型
[*][N]Type // 指向的内容是一个 [N]Type 类型的数组
[N][N]Type
[N]*Type
[KeyTypeName]ValueType
[|]int // 集合类型
[flat_set|]int // 自定义集合类型
[string]int // 映射类型
[flat_map|string]int // 自定义映射类型
[string]*int
*[string][N]int
where [m &a &b] { stmt... } // 捕获参数
FuncTypeLit [m &a &b] { stmt... } // 捕获参数
vsym[expr] vsym[expr][expr] // 变量数组元素
func()[expr] // 函数返回值数组元素
func()[expr:expr] vsym[expr:expr] // 数组切片和字符串切片
if [expr] // 条件匹配语句
for [&it] // 迭代元素捕获
= [expr, expr, expr] // 数组字面量，或嵌套在数组字面量或大括号初始化语言 {expr, expr, ...} 内部
= [key: value, key: value] // 映射字面量
= [key; key; key] // 集合字面量
[#global] // 全局顶行出现的配置项
[fruit apple]
[fruit]
[apple.physical]
[[]fruit fruits]
// 数据生成表达式
[yield a + b for a in array for b in 1 .. 100] // 生成一个整数数组，每个元素的值为 a+b
[yield 'int a * 2 for a in array] // 带类型转换的生成数组
[yield {to_string(a), a ^ b, a + b == b} for a in array for b in 1 .. 100] // 生成一个元组数组
[yield {name = to_string(a), xor = a ^ b, a + b} for a in array for b in array] // 生成一个元组数组，并为元组的成员命名
[yield a + b | for a in array for b in 1 .. 100] // 生成一个集合
[yield a + b flat_set | for a in array for b in array] // 生成一个 flat_set 集合
[yield a + b : a * b for a in array for b in array] // 生成一个映射
[yield a + b : a * b flat_map | for a in array for b in array] // 生成一个 flat_map 映射

// 字符串实现，字符串总是一个 string view，程序不能修改其指向的内容
//
//  1.  字符串字面量，位于只读分区，添加 local 关键字表示局部与函数作用域的字面量，只能传递给 view
//      string ---->[byte ...] // 不保存大小，常量大小由编译器记录，参数传递时可传递给 string_view
//      print(local "error code: %d", error)
//      def test(const string fmt int a) // 带有 const 的 string 参数，必须传递字面量，const 参数在实际中是一个常量，不占用传参空间
//
//  2.  只读字符串，肯定是固定大小，只不过该固定大小是变量初始化时固定的，不拥有内存
//      指向内容可能指向只读分区，因此 string_view 一定不能修改字符串的内容
//      string_view 观察的内容只能在初始的视察范围内缩小，例如 data 向前移动或 count 减小
//
//      def string_view { // 只读分区和可写分区的字符串都可以传递给 string_view 只读访问
//          *byte data
//          int count
//      }
//
//  3.  编译时大小固定的字符串，即栈上或全局分配的大小固定字符串，只读时传递给 view，修改时传递给 slice
//      string ---->[byte ...] // 不保存大小，常量大小由编译器记录，参数传递时可传递给 string_view
//
//      let [_]byte s = "世界"
//      let [1+"世界".size()]byte fixed_string = {}
//      fixed_string.push('a') // 最多只能添加 5 个字节，能添加的字符小于等于 5
//      fixed_string.push("世界")
//
//  4.  拥有内存的字符串空间，位于可写分区
//                 [0|int size][byte ...] // size 是整个字节串的最大空间大小
//      [allocator][1|int size][byte ...]
//      alloc_buffer -------->'
//
//      仅有 alloc_buffer 才实际拥有内存，其他都只是存在于栈空间的状态维持对其的引用。
//      alloc_buffer 保存在所在协程的 buffer_table 中维护，仅在字符串初始化时添加，
//      并在容量扩充时更新地址。不能保存在协程中，如果一旦传递给其他协程，已修改就会导致
//      协程间多线程访问导致高速缓存失效问题。
//
//  5.  可变大小字符串，这些栈状态结构体都在块作用域结束后自动释放
//
//      def string { // string 相当于动态容量字符串，string_slice 相当于固定容量字符串
//          *alloc_buffer data
//          int count
//      }
//
//      def "de" string { // 双端字符串
//          *alloc_buffer data
//          int count
//          int start
//      }
//
//      def init_string(int size return string)
//      def init_string(int size return "de" string)
//
//      let string s = init_string(672)
//      let "de" string s = init_string(1024)
//
//      初始化一个 string 给定容量后，传递给 slice 处理，即得到一个以当前视角的固定容量的字符串
//      在该 slice 的处理过程中，编译器需要监控程序禁止调用 string 的改变容量的函数。
//      slice.access_begin() slice.access_end() slice.access_begin(string) slice.access_end()
//      在 slice.access_begin() end() 之间不能有可能改变容量大小的函数，当 slice.access_begin()
//      再次修改时，如果修改了容量，必须给 access_begin() 传入参数来更新指针指向值。
//      string_slice 可同时实现动态字符串和双端字符串的固定容量操作功能，当进行更新时，
//      只需要更新 data 指针和 capacity 值。
//
//      def string_slice { // string_slice 相当于 inplace_string，slice 只能在 [data, end) 范围内操作和移动
//          *alloc_buffer data // 只有指向 string/string_double_ended 底层缓冲区时才需要更新
//          int count
//          int start
//          int capacity
//      }
//
//      函数只读取字符串内容，将参数声明为 string_view
//      函数不修改字符串容量，将参数声明为 string_slice
//      函数会修改字符串容量，将参数声明为 string 或 string_double_ended
//
//  10.  错误字符串
//      e_code 是一个确定的 32 位无符号整数，通用错误码
//      E_CODE 是一个与当前应用程序相关的字符串唯一的哈希值，且程序不会存储字符串
//
// 类型唯一识别号，只要保证在当前的程序中唯一。
// 函数参数重载，函数参数类型必须使用命名类型
// 命名空间或名称 N ... E
// 模板实参列表 I ... E
// array<float,4>           =>  ?N5arrayIfLi4EE
// *Type                    =>  ?NP1*4TypeE
// def point { f32 x y }    =>  ?N5pointE

// 常量没有地址，只有当赋值给变量时才真正保存到只读数据段
const PI = 3.1415926
const 2P = 2 * PI
const point = point {100, 200}
const POINT = point {100, 200}
prfer PI = 3.1415926
prfer 2P = 2 * PI
prfer point = point {100, 200}
prfer POINT = point {100, 200}

act all are do use ago alt any auf aut can cat cor con cue des dhu din don dor
fac far fat fen fer fin fit fou fro fry fur gen gre lot off per pat pal phr par
pre pro rem res rim ron rou rut tie via was wow yet

def point @zeroinit @packed {
    f32 x y
}

def point {
    f32 x y
}

def data {
    int a b
    (int a b return int) f g
}

def get ($*T a return int) // 函数参数只能声明类型模板参数
def read ($*T a unsigned p int n return int) // 函数只有第一个参数才能是泛型类型
def reader $T $get(T) get $read(T) read {} // 在 ${} 表达式中需要省略 def 关键字

def @{get} ($*T a return int)
def @{read} ($*T a def ptr p int n return int)
def @{reader} $T $<T>@{get} get $<T>@{read} read {}

Data { int a b (int a b int) f g }
Get($*T a int) // 函数参数只能声明类型模板参数
Read($*T a Ptr p int n int)
Reader $T
    $Get(T) get
    $Read(T) read
{}

@type{get} ($*T a int)
@type{get} (${any} *T a int)
@type{read} ($*T a Ptr p int n int)
@type{reader} $T $(@type{get}(T) get @type{read}(T) read {}

Point { float x y }
Data { int a b (int a b int) f g }
Reader $T $(*T p int a b int) calc $(*T p []byte a) get {}
Get ($*T p int)
Read ($*T p *byte buf int n int)
Reader $T $Get(T) get $Read(T) read {}
Reader $T <T>Get get <T>Read read {}
Handle (*HciRxDesc rx_desc u32 ca HciDataType type U line)
hci_rx_buffer_handled_and_free(HciRxDesc rxdesc u32 ca HciDataType datatype U line)
extern L2capConn         *L2capConn
extern L2capCallback     *L2capCallback
extern TcpSocket         *TcpSocket
extern BtStatus          *BtStatus
extern HciGlobal         *HciGlobal
extern HciConnType       *HciConnType
extern HciConnItem       *HciConnItem
extern QueFit
extern QueFix
extern ArrFix
extern ArrFit
extern ArrDyn
extern ArrLax

extern def l2cap_conn
extern def l2cap_callback
extern def tcp_socket
extern def bt_status
extern def hci_global
extern def hci_conn_type
extern def hci_conn_item

def 点 { float x y }
def 数 { int a b }
def point { float x y }
def data { int a b (int a b return int) f g }
def reader $t $(def *t int a b return int) calc $(def *t  def []byte a) get { }
def get ($*t p return int)
def read ($*t p def *byte buf int n return int)
def reader $T $get(T) get $read(T) read { }
def color i08 { RED 1 BLUE 2 YELLOW 3 }
def bitvalue int { FLAT_BIT1 1 << const, FLAG_BIT2, FLAG_BIT3 }
def tcpaction int { TCPA_OPEN_ACCEPT, TCPA_TX_DATA, TCPA_RX_DONE }
def 协程 { u32 rspoffset loweraddr }
def coroguard { u32 lower_guard_word def *coro coro (def *coroguard g int a b int) f g }
def handle(def *hcirxdesc rxdesc u32 ca def hcidatatype type def u line)
def main(int argc **byte argv int)
def scale(def point point int a b)
def calc(int a b int)

def array $t const (int size) {
    [size]t a
}

def array $a const (int size) { // $ 定义一个类型参数 a
    [size]a
}

def test $a $b {
    type a // 指定 a 是一个类型
    type b
}

def int_N_array const (int size) = type(size, $t) array
def type_size_array const (int size) $t = type(size, t) array
def int_array = type(size, int) array
def some_array_type = type(8, (10, (20, int_N_array))) array
def int_array_of_array = type(8, int_array) array
def int_array_of_array = type(8, (20, int)) array

def test $t $u {
    type t
    type u
}

def color const int {
    red {const + 1}
    blue
    green
    yellow
}

def test $t $array(size, t) a $u const (int size type point) {
    type t
    type u
}

def test $t $u {
    type t
    type u
}

def test $any t u {
    type t
    type u
}

extern def l2capconn         def *l2capconn        def [2]l2capconn
extern def l2capcallback     def *l2capcallback    def [2]l2capcallback
extern def tcpsocket         def *tcpsocket        def [2]tcpsocket
extern def btstatus          def *btstatus         def [2]btstatus
extern def hciglobal         def *hciglobal        def [2]hciglobal
extern def hciconntype       def *hciconntype      def [2]hciconntype
extern def hciconnitem       def *hciconnitem      def [2]hciconnitem
extern def quefit
extern def quefix
extern def arrfix
extern def arrfit
extern def arrdyn
extern def arrlax

def t1 typeof(def int)
def t2 typeof(def *int)
def t2 typeof(def [3]int)
def t3 typeof(def t0)
def t3 typeof(def [3]t0)
def t4 { (int a b return int) f int a b }
def t5 $p { (*p int a b return int) calc }
def t6(int a b return int)
def t7(def point float factor)

def node $t {
    this next
    type t data
}

def tripple $t $u const (int size) {
    [size]t a
    type u b
}

def main(int argc **byte argv return int)
def scale(type point int a b)

// 分两种错误，错误码非零（errno），返回不能为空值（?*int）
Calc (int a b int errno) // errno is a type with i32
let result calc(1, 2)
if result.errno then panic("%", errno)
let result calc(1, 2) or abort() // let or 语句
let ptr getp() or abort()
let ptr getp()
if ptr.error then abort()

let size f(), g(), 3+2  // let逗号语句
let size if a case 'a' { a } case 'b' { b } else { c } // let 可以让语句产生一个值，默认语句块的值为空
let size a or b         // let or语句
let size a > b then a or b // let then or 语句

def :: std/array // 同一个包名下的源代码，可以分割为多个部分，成员函数例如 push(*std::array *type a) 只能编写在 std 包的 array 源代码部分
import * std/array // 剥去包名，除了有冲突的名称仍然添加包名外，其他名称直接引入，std 包名只能存在于系统标准源代码中
import 3rd lib/array // 文件可提供一个包名，也可以不提供，如果没有提供包名，import 时可以定义一个包名
import std/array // 如果提供了包名，可以使用 using 导入特定包名中的所有符号，或定义符号别名
using 3rd_array = 3rd::array
using std_array = std::array

Coro { // 公开函数会公开所有参数涉及的类型，公开类型的字段都是只读的，写操作必须通过公开函数
    u32 rspoffset // 1st field dont move
    u32 loweraddr
    u32 maxudsize 31 ptr_param 1
    i32 coro_id
    unsigned rspoffset
    unsigned loweraddr
    unsigned maxudsize 31 ptr_param 1
    int coro_id
    unt rspoffset
    unt loweraddr
    unt maxudsize 31 ptr_param 1
    int coro_id
    type ptr address
}

def coro {
    u32 rspoffset
    u32 loweraddr
    i32 maxudsize 31 ptr_param 1
    i32 coro_id
    unsigned rspoffset
    int maxudsize
    int coro_id
}

def coro_guard {
    u32 lower_guard_word
    u32 upper_guard_word
    type coro embed
    *coro coro_ptr
    this embed
    this coro_guard
    (int a b return int) calc
    (*coro) func
}

def 协程 {
    u32 rspoffset
    u32 loweraddr
}

def "std"

def coro_guard {
    u32 lower_guard_word
    u32 upper_guard_word
    type coro embed
    *coro coro
    this embed
    this coro_guard
    (int a b return int) calc
    (*coro p) func
    (*coro_guard int a b return int) f g
}

def verify(*coro_guard)

Color const i08 {RED {const + 1} BLUE YELLOW}
Color const int {red blue { blue_defined_value } yellow}
Color const int [[strict]] {RED {1} BLUE {2} YELLOW {3}}
Color const u08 { red blue { blue_defined_value } yellow }

CoroGuard { // 内嵌只能内嵌结构体类型，不能是指针
    u32 lower_guard_word
    u32 upper_guard_word
    Coro _embed_ // 不能内嵌两个相同类型
    *Coro coro_ptr
    this _embed_ // 错误，指针不能内嵌
    this coroguard
    (int a b int) calc
    (int a) print
    (this int a b int) a
    (CoroGuard g int a b int) f g
    (Coro p) h
    { int a b }
    { int a b } tuple
    $i08 {RED {1} BLUE YELLOW} color
    $int {red {red_enum_value} blue yellow} color
}

Color $i08 {
    RED { const + 1 }
    BLUE
    YELLOW
}

Color $int {
    red
    blue {blue_defined_value}
    yellow
}

Color $i08 [[strict]] { // strict 枚举类型必需为全部枚举手动指定值，并在代码更新时不能修改这些值，以防带来代码版本的不兼容
    RED {1}
    BLUE {2}
    YELLOW {3}
}

BitValue $int {
    FLAG_BIT1 { 1 << const }
    FLAG_BIT2
    FLAG_BIT3
    FLAG_BIT4
}

Main(i32 argc **byte argv i32)
Scale(Point point int a b)
Calc(int a b int)
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
    $(*T p Ptr ptr int n int) write
{}

Color $i08 {} // 因为i08是关键字，不能使用关键字定义新的类型参数，因此这里必须是一个常量参数

TypePtr $sum { // enum 用来定义 sum type/tagged union type
    ptr (Ptr)
    null @occupy Ptr 0 // @occupy 只能用于两个元素的enum
}

Token $sum { // enum 定义的是一个联合体类型
    atom {char id}
    oper {char id}
    eof
}

Expr $sum { // enum 定义的是一个联合体类型
    value { int n }
    ident { int id }
    expr {char op Expr lhs rhs}
}

Oper $int -> {int lpri rpri} { // $int 定义的是一个常量
    ass '=' {200, 201} // a = 2 + b = 3
    add '+' {211, 210}
    sub '-' {211, 210}
    mul '*' {221, 220}
    div '/' {221, 220}
    pow '^' {230, 231}
    dot '.' {251, 250}
    end 0 // 默认值为零
}

// 常量没有地址，只有当赋值给变量时才真正保存到只读数据段
const PI = 3.1415926
const 2P = 2 * PI
const point = point {100, 200}
const POINT = point {100, 200}

pub const PI = 3.1415926
pub const 2P = 2 * PI
pub const point = point {100, 200}
pub const POINT = point {100, 200}

def color const u08 { // private type
    RED, GREEN, BLUE,
}

pub color const u08 { // public type
    RED,
    GREEN = 1 << const,
    BLUE,
}

pub color const u08 "strict" { // strict 枚举类型必需为全部枚举手动指定值，并在代码更新时不能修改这些值，以防带来代码版本的不兼容
    RED = 1,
    BLUE = 2,
    YELLOW = 3,
}

def point {
    float x
    float y
}

pub point {
    float x
    float y
}

pub data { int int float string }

pub coro { // 包外访问，结构体成员只读，以下划线结束的成员不可访问
    u32 rspoffset // 名为 rspoffset 的私有成员
    u32 loweraddr // 名为 loweraddr 的私有成员
    i32 maxudsize 31 ptrparam_ 1
    i32 coro_id
}

def type main_proc = (int argc **char argv return int)
def type eat_proc = (*lexer type expr return *oper)
def type int_ptr = *int
def type point_ptr = *point
def type gfx_point = point

pub type main_proc = (int argc **char argv return int)
pub type eat_proc = (*lexer type expr return *oper)
pub type int_ptr = *int
pub type point_ptr = *point
pub type gfx_point = point

def main(int argc **char argv return int) {
    return 0
}

pub eat(*lexer type expr return *oper) {
    return lexer.op or expr.op
}

def main = (return int) { return 0 }
def *int p = adr **int base + sizeof int
def *point p = der **point base + sizeof point
def *point p = adr point {}
def point = der p

def test const (int size type point) {
    [size]int a
}

def array $t const (int size) static size > 0 {
    [size]t a
}

def color const {
    RED, GREEN, BLUE
}

def color const int { // private type
    RED = 1,
    GREEN,
    BLUE
}

pub color const int { // public type
    RED = 1 << const,
    GREEN,
    BLUE,
}

def read_username_result const byte {
    OK {string},
    ERR {unsigned},
}

def calc(int a b return int int) {
    reflex return x, y
    x = a + b
    y = a * b
}

def calc(int a b return int int or error) {
    if a == 0 return e_invalid
    reflex return x, y
    x = a * b
    y = e_notzero
}

def read_username(return string or error) { // 返回值的大小为 sizeof read_username_result，比 string 类型长一个字节，调用者必须检查错误码
    let f = open("username.txt") or return // 这里 or error 如果成立会直接返回 open 函数的错误码
    let s = string {}
    f.read_to_string(adr s) or return
    if s == "unknown" return e_notfound
    return s
}

let s = read_username() or abort(error)
let s = read_username() where [a] { a.trim() } or "unknown"
let s = read_username() where it.trim() or "unknown"
if s.error abort(s.error)

// Option<T> 仅表示 “有/没有”，不携带错误原因，Result<T, E> 表示 “成功/失败” 并附带错误信息
// 相比传统的空值检查，none 和 error 的统一处理方式让 “忘记检查空值” 直接编译报错，编译器强制要求处理 “空” 情况
// 指针/函数指针/字符串 none 的 niche 值为 null, bool 可以使用 0x02 表示 niche 值
// char 字符 UNICODE 标量的上限 0x10FFFF，有大量高位值可用作 niche
// float 可以使用 N/A 值，int/unsigned 则必须手动指定，或使用 nonzero int，nonfini int，nonnull<T>
def divide(float a b return float or none) { // 空值，有值，返回值的大小为 sizeof float，调用者必须检查 none 值
    if b == 0 return none
    return a / b
}

let a = divide(a, b) or abort(e_divbyzero)
let a = divide(a, b) where [x] { x * 10 } or -1 // 如果有值则捕获其值并乘以10，否则得到-1
let a = divide(a, b) where it * 10 or -1
if a == none
    abort(e_divbyzero)
else
    print("a/b=%", a)

def calc(*file @maybe(none) *expr return int) { // 如果加上了 none 属性表示值可能为空，必须要进行 none 检查
}

// Optional Type 的两个好处：
//  1.  编译器保证调用者必须检查可能为 none 的值
//  2.  编译器保证条件分支中 none 值不会传递赋给不能为空的变量或参数，参数默认都是不能为 none 的，除非明确指定为 noneable
// Optional Type 的一个坏处是：
//  1.  让代码变量繁琐，但是这是一种可选择性的有目的性的繁琐
//  2.  因为你可以有目的的选择在关注的代码上选用 or none
// 关于空值：
//  1.  默认不能将 null 传递给指针，除非它被显式声明为 @maybe(none)
//  2.  默认不能将 0 传递给 @nonzero 整数型变量
//  3.  默认不能将全一的值传递给 @nonalls 整数型变量
//  4.  空值是一个特殊的值，不应该在整个程序中泛滥传播
//  5.  or none 必须可以应用到任何类型，用来全面消除空值的泛滥传播
//  6.  @maybe(none) @nonzero @nonalls 可以修饰结构体成员，使用这些成员必须经过 none 检查和传递性验证
//  7.  a where [x] { print(x) } or print("none") 增加新的语法保证简洁性和提供更高的安全性，原来的非空值只能通过if语句保证
//      新的语句将非空焊死在局部变量 x 中，print 根本访问不到可能为空的 a，因为函数闭包只能访问显式写在捕获参数中的值
//  8.  let x = a where [x] { x * 2 } or none // 变量 x 也将变成可空的值
//  9.  let x = a where [x] { x * 2 } or return + b or return // 表达式中可以在遇到 none 的地方直接返回空值
//  10. print(a where [x] { x * 2 } or -1)
//      print(a where it * 2 or -1)
//      a where it.print() or print("none")
//      a where it * 2 or none
//      a where it * 2 or return + b or return

def sqrt(float x y return float or none) { // 调用者必须检查 none 值，不管通过 or 还是 if [a] none 等形式
    let a = divide(x, y) or return + divide(3, x) or return // 这里 or 如果成立会直接返回 none
    return sqrt(x * a)
}

def oper const u32 (u08 lpri rpri) { // sum type
    ASS '=' {200, 201},
    ADD '+' {211, 210},
    SUB '-' {211, 210},
    MUL '*' {221, 220},
    DIV '/' {221, 220},
    POW '^' {230, 231},
    DOT '.' {251, 250},
    END 0 // 默认值为零
}

def token const byte { // sum type
    ATOM {byte id},
    OPER {byte id},
    TEST {int int},
    EOF
}

let atom = token {ATOM, 1}
let oper = token {OPER, '+'}
let test = token {TEST, 1, 2}
let test = token {TEST, a = 1, b = 2}
let eof = token {EOF}
def token atom = {ATOM, 1}
def token oper = {OPER, '+'}
def token test = {TEST, 1, 2}
def token test = {TEST, a = 1, b = 2}
def token eof = {EOF}

// 泛型代码相当于在目标文件中不能生成具体代码，而是生成一个代码模板
def expr const byte { // 相当于是一种泛型类型
    VALUE {int n}, // 相当于存储 {byte 0 int n}
    IDENT {int id}, // 相当于存储 {byte 1 int n}
    TEST {int int},
    EXPR {int op *expr lhs rhs}, // 相当于存储 {byte 2 int op unsigned lhs rhs}
}

if [expr] VALUE { // 必须穷尽所有情况，否则编译报错
    ret = expr.n
} else IDENT {
    ret = expr.id
} else TEST where [a b] { // 捕获元组的内容
    ret = a + b
} else EXPR {
    ret = expr.op
}

if expr == IDENT {
    print("IDNET expr: %", expr.id)
}

if expr == TEST {
    print("TEST expr: % %", expr.0, expr.1)
}

if expr == TEST where [_ a] { // 捕获元组的内容
    print("TEST expr: % %", expr.0, a)
}

if expr == TEST where [a b] {
    expr.a = 1
    print("TEST expr: % %", a, b)
}

def eat(*lexer type token) {
    return lexer.pop()
}

def peek(*lexer type token) {
    return lexer.top()
}

def eval(type oper type expr lhs rhs return expr) {
    if [oper] '=' {
        expr = .value(rhs.value.n)
        get_symbol(lhs.ident.id).value = rhs.value.n
    } else if '+' {
        expr = .value(lhs.value.n + rhs.value.n)
    } else if '-' {
        expr = .value(lhs.value.n - rhs.value.n)
    } else if '*' {
        expr = .value(lhs.value.n * rhs.value.n)
    } else if '/' {
        expr = .value(lhs.value.n / rhs.value.n)
    } else if '^' {
        expr = .value(pow(lhs.value.n, rhs.value.n))
    } else {
        panic("bad operator %c", oper)
    }
    return expr
}

eval(pub oper def expr lhs rhs return expr) {
    reflex return expr
    if [oper] '=' then
        expr = .value(rhs.value.n)
        get_symbol(lhs.ident.id).value = rhs.value.n
    else if '+' then
        expr = .value(lhs.value.n + rhs.value.n)
    else if '-' then
        expr = .value(lhs.value.n - rhs.value.n)
    else if '*' then
        expr = .value(lhs.value.n * rhs.value.n)
    else if '/' then
        expr = .value(lhs.value.n / rhs.value.n)
    else if '^' then
        expr = .value(pow(lhs.value.n, rhs.value.n))
    else then
        panic("bad operator %c", op)
}

eat(*lexer return token) {
    return lexer.pop()
}

eval(type oper type expr a b return expr) {
}

parse_expression(*lexer int min_prior return expr) {
    let expr lhs = undefined
    if [lexer.eat()] atom(it) {
        if it == '0'..'9' then
            lhs = .value(it - '0')
        else if it == 'a'..'z' || it == 'A'..'Z' then
            lhs = .value(get_symbol(it).value)
        else then
            panic("bad token %d", it)
    } else if oper('(') {
        lhs = eval(parse_expression(lexer, 0)
        assert_eq(lexer.skip(), Token.oper(')'))
    } else {
        panic("bad token %d", it)
    }
    for {
        def expr = undefined
        if [lexer.peek()] eof oper(')')
            break
        else if oper(it) then
            expr = .expr(it)
        else then
            panic("bad token %d", it)
        lexer.skip()
        let prior Oper(expr.expr.op)
        if prior.lpri < min_prior
            break
        rhs parse_expression(lexer, prior.rpri)
        lhs = eval(expr.expr.op, lhs, rhs)
    }
    return lhs
}

eat(*lexer return token) {
    return lexer.pop()
}

peek(*lexer type token) {
    return lexer.top()
}

parse_expression(*lexer int min_prior type expr) {
    def expr = undefined
}

main(return int) {

}

// 因为函数的第一个参数可以重载，因此 tcp_poll(file, sock, wait) 和 file.tcp_poll(sock, wait) 都同样有效
tcp_poll(*file *socket *poll_table return poll) [m] alignas(16) {
    def poll = undefined
    def *socket alignas(CACHE_LINE_SIZE) = socket
    let a = byte undefined
    let b = int undefined

    sock_poll_wait(file, sock, wait)

    state = inet_sk_state_load(sk)
    if state == TCP_LISTEN return inet_csk_listen_poll(sk)

    mask = 0

    shutdown = READ_ONCE(sk.sk_shutdown)
    if shutdown == SHUTDOWN_MASK || state == TCP_CLOSE then
        mask != EPOLLHUP
    if shutdown & RCV_SHUTDOWN then
        mask |= EPOLLIN | EPOLLRDNORM | EPOLLRDHUP

    if state != TCP_SYN_SENT && (state != TCP_SYN_RECV || rcu_access_pointer(tp.fastopen_rsk)) {
        target int sock_rcvlowat(sk, 0, INT_MAX)
        urg_data u16 READ_ONCE(tp.urg_data)
    }

    smp_rmb()
    if READ_ONCE(sk.sk_err) || !skb_queue_empty_lockless(&sk.sk_error_queue) then
        mask |= EPOLLERR

    return mask
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

perform_tcpa_open_accept(*TcpSocket tcp u32 txbuf_size u32 rxbuf_size) {
    let pdata = *TcpAccept cono_malloc_pdata(TCPA_OPEN_ACCEPT, TCPQ_UPPER, true, sizeof TcpAccept)
    pdata.rxbuf_size = rxbuf_size
    pdata.txbuf_size = txbuf_size
    cono_freely_post(tcp.tcp_coro, pdata)
}

def report_tcpe_opened(*TcpSocket tcp) {
    let pdata = *TcpOpened tcpa_post_pdata(tcp, TCPE_OPNED, sizeof TcpOpened)
    let txbuf = *ByteArrfit adr tcp.txbuf
    pdata.tcp = tcp
    pdata.txbuf = arrfit_begin(txbuf)
    pdata.size = txbuf.size
    cono_freely_post(tcp.upp_coro, adr pdata->head)
}

def epoll_proc(*coro) {
    def *epoll = cono_data(coro)
    def *coro_pdata = undefined
    let action = byte undefined
    def point = {100, 200}
    for {
        pdata = cono_pwait(coro)
        action = pdata.action
        if action == COAC_EXIT // 省略大括号，读取一条语句
            break
        if [action] EPAC_DEL_CLOSE
            epac_del_close(epoll, (int)pdata.u.value)
        elif EPAC_POLL_ONCE
            epac_wait(epoll)
        elif EPAC_POLL_ALL
            for epac_wait(epoll) void
        else
            debug prerr(action)
    }
}

def main(void) {
    print("hello")
}

def main(int argc **byte argv return int) { // main 函数默认是 public
    print("hello world\n")
    return 0
}

def main(return int) [m] {
    print("hello world\n")
    return 0
}

def main(int argc **byte argv return int)
def scale(type point int a b)
def calc(int a b return int)
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

perform_tcpa_open_accept(*TcpSocket tcp u32 txbuf_size u32 rxbuf_size) {
    let pdata = cono_malloc_pdata(TCPA_OPEN_ACCEPT, TCPQ_UPPER, true, sizeof TcpAccept)
    pdata.rxbuf_size = rxbuf_size
    pdata.txbuf_size = txbuf_size
    cono_freely_post(tcp.tcp_coro, pdata)
}

report_tcpe_opened(*TcpSocket tcp) {
    let txbuf = adr tcp.txbuf
    let pdata = tcpa_post_pdata(tcp, TCPE_OPNED, sizeof TcpOpened)
    pdata.tcp = tcp
    pdata.txbuf = arrfit_begin(txbuf)
    pdata.size = txbuf.size
    cono_post(adr pdata->head)
}

epoll_proc(*Cono cono) {
    let epoll = cono_data(cono)
    let pdata = *ConoPdata undefined
    let action = byte undefined
    for {
        pdata = cono_pwait(cono)
        action = pdata.action
        if action == COAC_EXIT
            break
        if [action] EPAC_DEL_CLOSE
            epac_del_close(epoll, (int)pdata.u.value)
        elif EPAC_POLL_ONCE
            epac_wait(epoll)
        elif EPAC_POLL_ALL
            for epac_wait(epoll) void
        else
            debug(prerr(action))
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
    @impl Writer(File, file_put, file_write)
    @impl Reader(File, file_get, file_read)
}
generic_write(*Writer writer) { // 实际上参数会传递 file 以及 File.Writer 静态数据的地址
    func writer.write
    func(writer)
} // 如果是编译时实现，对每个实现类，都会生成一个特定的 genric_write 函数，其中的 func 会直接替换为实际函数，这种泛型实现是一种非常灵活的填鸭式的代码实现

Calc(int a b int)

def snode $T {
    this next
    T data
}

for i I 0 .. 9 {
    i int der *I addr
    pos + der adr *I (*byte p + size + f(g))
}

def memcpy(type ptr dest src int count)
def memcpy(type ptr dest src int count) 'intrinsic'
def memcmp(type ptr dest src int count int) 'intrinsic'
def memset(type ptr dest byte value int count) 'intrinsic'
def lock_cmpxchg(*T p T old new T) 'intrinsic'
def coroguard(*coro type coro_guard) 'cdcel inline'

Calc (int a b int)
Snode $T { this next T data }
for [&] i I 0 .. 9 {
    i int der *I addr
    if i%2 continue &
    pos + der adr I (*byte p + size + f(g))
}
memcpy (Ptr dst src int count)
memcmp (Ptr dst src int count int)
lock (*T p T old new T)
coro_guard (*Coro coro CoroGuard)
get_coro_guard(def *coro coro def ptr)

// 类型并不需要提前声明，因为可以通过词法直接分辨，函数类型如果可以通过传递的参数匹配也无需提前声明
extern T1
extern T2
extern T3
extern T4
extern T5
extern T6
extern T7
extern Calc
extern MyInt
extern Coro

// 定义变量，包括函数变量，一个非类型标识符后跟一个类型表示定义该类型的一个变量
CoroProc (*Coro coro)
print $string fmt ($T ... int)
print (string fmt $T ... int)
print"width % height %\n"(width, height)
print("width % height %\n", width, height)

calc $int a b (int c d int)
calc"2,3"(c, d)

def print(reflex $a string fmt) {
}

def reader $p {
    (*p int size return int) read
    (*p return int) get
}

def push(trait reader) {
}

push(a.trait(reader))

pub main(return int) {
    return 0
}

pub main(int argc **char argv return int) {
    return 0
}

pub calc(int a b return int) {
    return a + b
}

def scale(type point float factor) {
    point.x *= factor
    point.y *= factor
}

def scale(type point float factor) "C" alignas(64) {
    p.x *= factor
    p.y *= factor
}

def scale(type point float factor) "fastcall" {
    p.x *= factor
    p.y *= factor
}

def next(*node($t) return *t) {
    return node.next
}

def size(*triple(int size, $t, $u) return int) {
    return triple.a + size + sizeof t
}

data { int a b } {1, 2}
data *{ int a b } adr {1, 2}
data [2]{ int a b } {{1, 2}, {3, 4}}
data Data {1, 2}
data *Data adr data
data Data[2] {data1, data2}

found .. index array_find(<<array, item)
found .. error array_find(<<array, item)

cal2 (int a b int) { return a + b }
cal2 (int a b int) { return a + b } (1, 2)
cal2 *(int a b int) (int a b int) {return a + b } // 函数不需要声明成指针，因为它本身就是指针
cal2 *(int a b int) Calc {return a + b }
cal2 [2](int a b int) {Calc {return a + b}, Calc { return a * b }}
cal2 Calc { return a + b }
cal2 *(int a b int) adr {return a + b }
cal2 [2](int a b int) {Calc {return a + b}, Calc { return a * b }}
cal2 *Calc adr (int a b int) {return a + b }
cal2 *Calc adr {return a + b }
cal2 Calc{return a + b}
cal2 [2]Calc {Calc {return a + b}, Calc { return a * b }}
cal2 [2]Calc {Calc {return a + b}, Calc { return a * b }}

let a = undefined // 错误，undefined 必须明确指定类型
let a = 42
let a = +99
let a = -1
let a = 'a'
let a = \t
let a = \0
let a = \9
let a = \x00
let a = \xff
let a = \{0000}
let a = \{ffff_ffff}
let a = 3.14
let a = "hello"

// "def" type_symb "=" expr {, symb "=" expr}
def *ppb = malloc(size) // 全局类型和变量只能使用 def 和 pub 关键字定义
def *int p = null, q = undefined
def point = undefined, o = {1, 2}
def int a = 0, b = 0
pub *ppb = malloc(size)
pub *int p = null, q = undefined
pub point = undefined, o = {1, 2}
pub int a = 0, b = 0

// 使用 const 和 prfer 定义全局常量
const SIZE = 8
const LEN = 32
const PT = point {100, 200}
const PI = 3.1415926
const 2PI = 2 * PI
const P2 = point {0}
prfer SIZE = 8
prfer LEN = 32
prfer PT = point {100, 200}
prfer PI = 3.1415926
prfer 2PI = 2 * PI
prfer P2 = point {0}

// 使用 const 定义局部变量和类型成员
const PI = 3.14159926
const 2PI = 2 * PI
const 3P2 = 3 * PI + 2
const PT2 = point {100, 200}

// "let" type_symb|symb "=" expr {, symb "=" expr}
let *ppb = malloc(size) // 局部变量只能使用 let 关键字定义，等号左边只能定义一个变量
let *int p = null, q = undefined
let int a = 0, b = 0
let point o = undefined, pos = {1, 2} // 自定义类型变量要么使用该形式，要么使用下一种形式，其他形式都禁止
let'point = undefined, o = {1, 2} // 类型名与变量名同名的形式
let point = point undefined, o = point {1, 2} // 冗余，不允许
let point point = undefined, o = {1, 2} // 冗余，不允许
let type point = undefined, o = {1, 2} // 冗余，不允许
let ppb = *ppb malloc(size)
let p = *int null, q = *int undefined
let a = 0, b = byte 0
let ptr = alloc(1024) or panic()
let data = data {this, a = 1, 2, b = 3} // 元组类型变量定义 data.a data.b data.$2
let data = read_tuple(return _, a) // 元组类型值的返回 data.$1 data.a
let integers = [1, 2, 3], colors = ["红", "黄", "绿"]
let array_ints = {[1,2], [3,4,5]}
let mixed_array = {[1,2], ["a", "b", "c"]}
let int_array = mixed_array.$0 // 3rd2.0 以数字开头的标识符，访问元组成员可能与浮点冲突
let str_array = mixed_array.$1 // 可以将元组成员的访问改成 $0 $1 等等
let a = 'int 0, b = 'float 3.1415926 // 非大括号或undefined形式的类型转换，类型前加转换前缀
let calc = (int a b return int) { return a + b} // 类型字面量可以自动识别，不需要添加转换前缀
let a = point{100, 200}, b = *int undefined // vsym + 大括号/undefined 都是类型的初始化，不需要添加转换前缀
let a = int{0}, b = float{3.1415926}
let a = 't' // 字符 t
let a = 't {a + b} // 类型转换成类型 t
let a = '' // 空字符，非法
let a = ''abcd'' // 将多字符当作整数使用，合法

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

let (int a b int) cal2 { return a + b }
let (int a b int) cal2 { return a + b }
let Calc cal2 { return a + b }
let Calc cal2 calc
let cal2 calc

dat3 *{ int a b } adr {3, 4}
dat3 [2]{ int a b } {{3, 4}, data}
dat3 *{ int a b } adr {3, 4}
dat3 *Data adr data
dat3 [2]Data {{3, 4}, data}

cal3 *(int a b int) adr { return a + b }
cal3 *(int a b int) adr calc
cal3 (int a b int) { return a + b }
cal3 Calc { return a + b }
let cal3 calc

// 基本类型和指针不通过大括号进行初始化而是直接给值，根据语法定义一个变量的同时必须进行初始化
// 一个非类型标识符后跟一个类型表示定义该类型的一个变量
// 一个非类型标识符后跟一个非类型标识符，表示用后面的非类型标识符的值定义一个变量
// 一个非类型标识符后跟一个字面常量，表示用字面常量定义一个变量

cal3 *(int a b int) null
cal3 *(int a b int) adr { a + b }
cal3 *(int a b int) calc

numb errno null
numb float 3.14
numb *int null
numb *int adr data
numb bool false

let data false
let data "Hello World"
let data 3.14
let data 1024 // 可以使用字面量直接赋值
let data calc // 但如果是变量值，没有类型转换的情况下必须使用大括号赋值

calc Calc { a + b }
data Data {1, 2}
data int 1024
numb errno null
numb float 3.14
numb *int adr data
calc Calc { a + b }
data Data {1, 2}
data int 1024
numb errno null
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

aaa Data {3, 4} // 赋值语句因为目标变量只有一个，因此只要将类型不加修饰的放在右表达式开头，相对于对整个表达式类型的转换
ppb *Ppb ppb_alloc(alloc)

let pos = dist + int scale_x(facter)
let len = int pos + adr *byte p + size + f(g)
let len = int pos + der *byte (p + size + f(g))
let len = typeof(pos) 3

pos int dist + int scale_x(facter)
len int pos + der *int *byte (p + size + f(g))
len typeof(pos) 3

for i int 3 .. 10 { /* */ }

for {
    capacity *= 2
} ~ if (capacity < new_capacity)

// 函数和普通变量提前声明，同一个变量声明可以出现多次，定义一个变量时必须有初始化也即
// 推荐仅在使用的地方才进行变量定义不提前定义变量
// 类型不需要提前声明，因为其名称可以由词法分辨
extern calc (int a b int)
extern scale (Point p float factor)
extern data { int a b }
extern calc Calc
extern data Data
extern data int
extern numb errno
extern numb float
extern numb *int

static if DYN_LINK_PROC {
    calc Calc null
} else {
    extern calc Calc
}

static assert(SIZE >= 1024)
assert(sizeof int == sizeof ptr)
real_assert(sizeof(*p) == sizeof point)

// https://squidfunk.github.io/mkdocs-material/reference/admonitions/
// https://github.com/cloudwu/datalist
// https://toml.io/cn/v1.0.0

[user]
name "fido"
member_since Date`{1999-08-04}
age 23

Fruit {
    string name
    []int rates
    { string color shape { int hight width { string a b } desc string info } size } physical
    { string name int id }[@arrfit] varieties
    { bool sweet } taste @optional
    { bool smooth } texture
}

Fruit {
    string name
    []int rates
    type {
        string color shape
        type {
            int height width
            type {
                string a b
            } desc
            string info
            string desc
        } size
    } physical
    { string name int id } [@arrlit] varieties
    { bool sweet } taste @optional
    { bool smooth } texture
}

[fruit apple] // 以另一个[var Type]结束，明确是字面常量赋值不需要使用=等号，否则需要使用调用语法进行初始化
physical color "红" size height 23 width 24 desc a "a" b "b" ||| size info "i"
physical shape "round"
taste sweet true
texture smooth true
rates [80, 75, 95]

[fruit apple] // 定义 apple
physical color "红" size height 23 width 24 desc a "a" b "b"
physical size info "i" desc "d"
physical shape "round"
taste sweet true
texture smooth true

[apple.physical] // 给 apple.physical 赋值
color "黄"
size height 23 width 24
shape "round"
sweet true
smooth true

[fruit apple] // 使用顶格和 ||| 控制缩进，0~3个空格是顶格字段，>=4个空格或>=1个Tab才是子字段
physical
    color "红"
    size
        height 23
        width 24
        desc
            a "a"
            b "b" void
    size
        info "i"
        desc "d"
physical
    shape "round"
taste
    sweet true
texture
    smooth true

[fruit apple]
physical color "红"
    size height 23 width 24 desc a "a" b "b" |||
    size info "i" desc "d"
physical
    shape "round"
taste sweet true
texture smooth true

[fruit apple]
physical color "红"
    size
        height 23
        width 24
        desc a "a" b "b" |||
    size
        info "i"
        desc "d"
physical
    shape "round"
taste
    sweet true
texture
    smooth true
void

[fruit apple]
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

[fruit apple]
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

[fruit apple]
fruit_type name "pug"

[[]fruit fruits]
name "apple"
physical color "red" shape "round"

[[]fruit fruits]
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

[[]fruit fruits]
name "banana"
void

import "src/lang/math" // 加载静态代码
time:Time
math:*

优先级（从高到低） ::

    12 从左到右    a:b 名字空间由代码包和文件内代码分块表示，代码分块的表示形如 :::time::: 代码包由一个文件夹组成
    11 从左到右    a() a[] a.b a->b 函数调用，数组下标，成员访问
    10 从右到左    -a +a ^a !a type a adr a der a sizeof a typeof a ->> <<-  not neg int adr der *int [2]int
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

    交换操作
        a =><= b
    以下两种形式的变量初始化，symb 一定是一个类型名称：
        symb { initializer }
        symb undefined

     逻辑操作符 || && 混用时，强制要求添加括号；
     所有的赋值语句如果不是单独成一个语句，而是出现在语句中，强制要求添加括号；

为了省略大部分的分号并避免歧义，一元操作符使用特殊的语法形式： ::

    小括号包含类型用来定义类型或用作类型转换操作符，小括号包含值表示表达式的一部分。
    大括号只能包含值或由值组成的语句列表，值由变量常量操作符组成。
    取地址 & 改为 (&) 地址标记 &1 &2 adr
    解引用 * 改为 (*) (**) (*&) (**&) 地址引用 *&1 *&2 der

    @negt()     @-          @-3.14      @-c         (-3.14) (-c)
    @posi()     @+          @+6.24      @+c         (+6.24) (+c)
    @comp()     @^          @^1024      @^c         (^1024) (^c)
    @addr()     (&)         @&data                  (&)data (*&)data    adr data    der adr data
    @dref()     (*)         @*p         @**pptr     (*)p    (**&)ptr calc(-3.14, +6.28, ^c, &data, *p, **&ptr) 前面必须有分隔符，包括左括号（( [ {），逗号（,），或（@）

// 结构体中的各类成员

def test {
    int a b c d
    int inplace {MASK_BITS} size {INT_BITS - MASK_BITS} // 位域，位域总是无符号类型，即使使用 int 声明，它都是一个无符号类型
    int inplace {MASK_BITS}
        size {INT_BITS - MASK_BITS}
}

// 条件语句包含传统C的if和switch：
//  if cond { expr }
//  if cond return expr
//  if cond { expr } else if_stm
//  #if cond { expr } else { expr }
//  #if cond { expr } else if cond { expr }

if expr { stmt ... } // 条件语句块有两种大括号，一种是左大括号在表达式 expr 结束的同一行，第二种是表达式结束后是一个换行，第二种语句块以 ||| 结束，并且必须有相同的对齐
if expr break
if expr yield
if expr continue
if expr return stmt
if expr // 条件表达式之后没有跟随跳转语句，或起始大括号，都将使用缩进对齐方式解析语句块，因此大括号如果存在，必须与表达式的结尾在同一行
    stmt
    stmt

if expr
    return stmt
else if expr
    aaa
    bbb
    if ccc
        stmt
        void
    void
else
    stmt
    if expr
        stmt
        void
    stmt
    void

if [color] red
    goto green
elif blue
    goto &
elif green
    void
else &
    void

if [color] RED { // 使用break会跳出外层for循环
    goto GREEN
} elif BLUE {
    goto &
} elif GREEN {

} else & {

}

defer_return @label
    return

// 循环语句

for expr { stmt ... }
for expr then stmt
for { stmt ... }
for { stmt ... } ~ if (expr)

// 函数支持默认参数，但不支持函数名重载，但支持第一个参数重载，但支持操作符重载+ - * / == != << >> & | [] % ^ <<< >>> []= .&，#symmetric
// 禁止函数链式调用 a.getb().bfun()
// 定义展开函数：
// `macro (retrun int) {
//      `x = 32
//      retrun `x * 2
// }
// pop(q, free_func)
// pop(q, =>(x)free(x))

// __jai_runtime_init
// __jai_runtime_fini
// __system_entry_point
(+ 12 24 13)
(* 13 14 15)

(->> theme
     (symbol-name)
     (funcall (-flip #'concat) "-theme")
     (intern))

(intern (funcall (-flip #'concat) "-theme" (symbol-name theme)))

// 默认提供编译时类型信息，运行时如果需要提供类型反射信息，传入变量的同时会同时传入类型信息。
// print 函数的设计：print("a % b %\n", a, b) 会转换成
print(typestring, "a ")
print(typeinfo, a)
print(typestring, " b ")
print(typeinfo, b)
print(typestring, "\n")

//  0.  代码编写原则
//
// 不要过度的进行输入参数检查，也不要让程序用治标不治本的方法自动修正错误，这只会种下
// 更大的隐患。例如下面的例子，因为越界参数的调用，已经说明程序前面的逻辑发生了错误，隐
// 瞒得越久，越不容易找到真正导致错误的位置。很多人对自己的程序没有信心，觉得出错是理所
// 必然的事情，所以他们用尽一切办法在错误发生的时候予以挽救。而实际情况是，好的程序代
// 码，各部分逻辑清晰完备自洽，就像这个功能就只有这一种自然而然的正确写法，没有多余的代
// 码，更不需要检查各种边界条件维持程序的正确性。如果你的代码丑陋不堪，更可能是代码设计
// 的原因。
//      at (*IntArray a int i int) {
//          if i < 0 || i >= a.size then i = 0
//          return a.data[i]
//      }
//
// 相比在局部指标不治本的自动修正错误，在代码开发阶段就及早的将错误暴露出来，才是正确
// 的做法。例如上面的检查，更好的方法是写一个 assert。但是要主要正确使用 assert，
// assert 的使用一定不能影响正常的程序逻辑，也就是说，所有的 assert 语句必须是可以去
// 掉的，在 assert 的表达式中改变变量的值都是不允许的。另外，导致断言失败的原因必须
// 是程序错误，不能用 assert 去充当实际真正执行的代码逻辑的检查。
//      assert(i >= 0 && i < a.size)
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
//      array_find_and_erase(int_array, 32, func)
//
//      yield (i, elem) array_find_and_erase(int_array, 32) { 封装与开放的平衡，既封装又没对回调函数参数进行限制
//          printf("int array elem 32 index %d at %p erased\n", i, elem);
//      }
//
//  2.  代码各种简写形式，限制代码行的最大字节数1024个字节，代码以行为单位进行解析
//
//      port->action.head.opcode[PRH_TCPA_INDEX_OPEN_ACCEPT] = PRH_TCPA_OPEN_ACCEPT
//      port->action.head.opcode[PRH_TCPA_INDEX_OPEN_REQ] = PRH_TCPA_OPEN_REQ
//      port->action.head.opcode[PRH_TCPA_INDEX_TX_END] = PRH_TCPA_TX_END
//      port->action.head.opcode[PRH_TCPA_INDEX_FINISH] = PRH_TCPA_FINISH
//
//      port->action.head.opcode[PRH_TCPA_INDEX_..] = PRH_TCPA_##OPEN_ACCEPT
//      port->action.head.opcode[PRH_TCPA_INDEX_..] = PRH_TCPA_##OPEN_REQ
//      port->action.head.opcode[PRH_TCPA_INDEX_..] = PRH_TCPA_##TX_END
//      port->action.head.opcode[PRH_TCPA_INDEX_..] = PRH_TCPA_##FINISH
//
//  3.  scoped 语句块
//
//      enter_read(def *rwlock) scoped(leave_read) {
//          ...
//      }
//
//      scoped rwlock.enter_read() {
//          ...
//      }
//
//  4.  函数参数的传递，函数参数可以设置对齐限制，编译器可以检查类型的对齐属性看是否满足要求
//
//      基本类型 int unsigned sys_int sys_ptr def ptr float 和枚举类型，可以显式传值或指针，传值(1)表示不修改，传指针表示修改
//      结构体类型总是传指针表示修改，声明为 *point，test(adr point) test(point_ptr)，即使是双字长的结构体也只传一个指针，因为需要修改成员，传递一个成员指针和两个成员指针区别不大
//      如果不需要修改结构体，需要声明为 *imm point，不同的是小于等于双字长的结构体直接传递结构体内容（2），大于双字长的将内容拷贝到栈并传递地址
//      情况(1)在函数中变为传指针，可能（通过寄存器而不是通过栈传递的情况下）需要将寄存器中的值重新复制到栈中
//      结构体类型总是传指针，函数参数只允许 def *type_name 语法，如果不想修改提前复制一份副本，或通过 copyof 修改副本，如果函数本身不进行修改则无所谓
//      如果结构体声明为 def type_name @as int { }，将结构体当作基本类型使用，则可以显式传值或指针
//
//      支持函数重载。
//      支持可选参数和命名参数，可以通过命名参数不按参数声明顺序传递参数。
//      不使用成员函数调用语法，所有函数调用都使用 C 函数调用方法。
//      vsym.field 语法仅用于结构体成员 -> 改成可以用于成员函数，但不能链式调用
//
//      "hello".print()
//
//  5.  函数内部，变量名不能覆盖
//
//  10. 协程的实现
//
// 对于 64 位系统，进程虚拟空间足够大，每个协程可以映射一个足够大的内存来使用，并且按
// 需对内存进行实际提交。因为用户空间有 128T 可用，而每 1T 虚拟空间可以给100万个协程
// 每个协程都预留1MB空间。
//
// 对于 32 位系统，因为自己的编程语言可以知道每个函数需要的最大栈空间是多少，可以实际
// 分配对应的栈大小即可。对于递归调用的情况，每次递归调用再次该该函数分配对应大小的栈
// 空间之后才继续执行，相当于创建了同一个函数的多个实例，然后随着递归的进行而分配和释
// 放。

—— 中文编程语言
—— 块注释 —… 注释多行 …—
—— 乃 及 之 也 厂 巨 叵 上 下 占 卡 不 反 后 曰 云 丫 儿 而 人
—— 已 令 入 如 负 用 元 币 白 贝 北 几 王 半 兰 凹 凸 出 羊 气
—— 未 力 又 幺 去 么 工 右 式 土 士 至 于 老 手 存 舟 马 牛 弓
—— 衣 变 大 小 多 少 勺 天 口 中 火 山 石 田 木 禾 米 主 竹 车
—— 皮 斤 两 夫 失 头 无 勿 甲 申 龙 尖 古 克 另 门 生 的 由 次
—— 步 布 比 产 民 冬 各 个 广 光 闩 字 子 自 当 录 改 巴 符 赋
—— 导 引 因 女 在 考 着 者 书 牙 司 正 有 水 牟 看 戳 此 代 否
—— 我 氕 刀 毛 长 常 片 扯 爪 妥 月 欠 风 点 启 心 必 丘 目 电
—— 地 鸟 岛 归 每 没 类 东 西 南 就 艮 亘 青 黑 了 凡 方 丰 缶
—— 亏 才 寸 川 丸 义 孑 刃 叉 廿 卅 仄 区 匹 权 互 化 花 华 仅
—— 乏 从 仑 今 兄 凶 乌 为 尹 本 可 只 先 并 更 助 具 单 管 过
—— 卬 包 奔 边 表 别 部 参 仓 苍 操 册 差 呈 持 充 丑 揣 创 春
—— 等 第 调 叠 定 丁 丢 斗 度 决 断 觉 对 兑 夺 嗯 尔 发 法 范
—— 尬 该 盖 概 干 感 杠 高 告 搞 稿 格 给 跟 根 共 公 攻 勾 估
—— 故 固 瓜 挂 观 哈 还 含 夯 号 好 核 合 和 很 横 宏 红 换 会
—— 汇 混 或 即 基 集 见 间 建 将 交 解 囧 究 举 卷 绝 扣 控 空
—— 开 肯 扣 叩 库 夸 宽 匡 坤 昆 来 勒 棱 角 立 俩 连 列 留 咯
—— 搂 吕 律 率 乱 掠 寽 论 吗 蒙 迷 幂 密 免 秒 灭 乜 名 命 明
—— 谬 里 莫 末 某 谋 你 内 那 呢 奈 能 逆 拟 念 年 捏 凝 宁 诺
—— 喏 挪 偶 奇 派 排 判 旁 抛 陪 捧 批 辟 品 拼 凭 破 剖 析 普
—— 其 起 恰 且 切 请 穷 求 取 群 然 让 扰 绕 若 认 任 揉 弱 思
—— 散 啥 筛 删 闪 商 设 舍 谁 甚 省 是 使 受 收 守 首 数 术 属
—— 输 双 顺 说 斯 嗖 诉 速 算 选 随 遂 虽 所 它 抬 探 套 讨 特
—— 提 替 填 添 挑 挺 同 通 投 图 团 退 推 屯 囤 托 脱 拖 拓 哇
—— 瓦 外 歪 完 往 望 问 闻 物 系 现 限 项 向 效 些 形 行 型 修
—— 休 需 许 序 续 宣 旋 玄 学 寻 训 压 言 央 样 养 要 页 业 耶
—— 野 以 印 应 哟 拥 与 迂 余 原 预 约 越 允 运 咋 杂 载 哉 暂
—— 攒 藏 造 则 责 怎 曾 扎 摘 栈 展 掌 找 招 折 这 真 假 指 终
—— 周 逐 抓 拽 转 最 专 传 撰 写 装 状 准 捉 兹 总 奏 尊 遵 左
—— 坐 作 分 厘 毫 微 纳 皮 飞 阿 擒 点 幂
—— 零 〇 一 二 三 四 五 六 七 八 九 十 百 千 万 亿 兆 吉 太
—— 阿 波 次 得 衣 夫
—— ≠ ≤ ≥ √ × ± Ω Π ▲ ← → ↑ ↓ ÷ ×
—— ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩
—— ·、，。：；？！￥‘’“”（）【】《》
—— — — …… … … ··
—— +-*=~&|%#@{}
—— 1234567890 3f14159p10 +3f14p-10 -0f1 数曰三点一四一五九幂十二 数曰一〇〇〇万八零零零 数曰负零点一
——
—— 12 从左到右    a:b 名字空间由代码包和文件内代码分块表示，代码分块的表示形如 :::time::: 代码包由一个文件夹组成
——     11 从左到右    a() a[] a.b a->b 函数调用，数组下标，成员访问
——     10 从右到左    -a +a ^a !a type a adr a der a sizeof a typeof a ->> <<-  not neg int adr der *int [2]int
——      9 从左到右    a.&b a->&b 返回成员地址，相当于(&)a.b
——      8 从左到右    a*b a/b a%b a&b a<<b a>>b   mul_op   --> <-- &^
——      7 从左到右    a+b a-b a|b a^b             add_op   |^
——      6 从左到右    a<b a>b a<=b a>=b           rel_op
——      5 从左到右    a==b a!=b
——      4 从左到右    a&&b
——      3 从左到右    a||b
——      2 从左到右    a?:b
——      1 从右到左    a=b a+=b a-=b a*=b a/=b a%=b a<<=b a>>=b a&=b a^=b a|=b
——      0 从左到右    a,b
——
—— 变量地址：取址
—— 指向内容：取值，给指向内容赋值：之指向内容
—— 数组下标：之|1|  之|2·3|
—— 成员访问：之，其 用于，曰 用于变量定义
—— 函数调用：施输出《你好，%，%，%》· 参数一 · 参数二 · 参数三，为了简洁，参数不能出现函数调用
—— 算术操作：加上 减去 乘以 除以 求余 取负 取正 位反 位与 位或 异或 左移 右移 循环左移 循环右移
—— 关系操作：大于 小于 等于 不等于 大于等于 小于等于
—— 逻辑操作：并且 或者 为真 为假
—— 赋值操作：昔{vsym} = expr，使用这个额外前缀，是为了明显查找到变量被赋值的地方
—— 字符常量：&a &b &&c #哈希 #空格 #响铃 #退格 #退出 #垂制 #制表 #回车 #换行 #00 #01 #02 #03 #09 #10 #20 #{f001}
—— 字串整数：立值 = &abcd   ‘长整数曰值 = &ab #换行
——
—— 函数     演算 撰 术 法 施
—— if       如若 若是 如果 若 当
—— else     不然
—— else if  然若
—— for      就此
—— range
—— break    为止
—— continue 继续
—— const    常量
—— void     空类
—— embed    内嵌
—— let      立 令
—— pub      附公开属性
—— def      撰 函数，构 类型
—— return   返回 乃还 终 结
—— yield    生成
—— undefined ？？
—— strict   附严格规则
—— defer
—— lambda
—— reflex
—— trait
—— static
—— or
—— this
—— type
—— import
—— using
—— scoped
—— adr      取址
—— der      取值 之指向内容
—— todo
—— debug
—— alignof type         取对齐属性 类型
—— sizeof type          取类型大小 类型
—— offsetof type.offset 取成员偏移 类型之名称
——
—— E_ e_    错曰非法内存 错曰除零 错曰内存溢出  造错曰指定条件不能满足
—— __name__ ……名称……    语言保留名称
—— name_    栈指针偏移… 私有成员
—— 撰程序入口 返回 ’整数 附公开属性 { 返回 0 }
——
—— alignas(n)           附对齐属性|N|
—— "zeroinit"           附零初始化
—— "fastcall"           附快调约定
—— "cdecl"              附传栈约定
—— "stdcall"            附标准约定
——
—— abort()
—— panic()
—— assert(expr) debug assert(expr)
—— debug { stmt ... }
—— alignof(vsym)        取变量对齐 变量
—— sizeof(expr)         取变量大小 变量
—— typeof(expr)         取变量类型 变量
—— copyof(vsym)         施拷贝 变量
—— moveof(vsym)         施移动 变量
—— zeroof(vsym)         施全零 变量
—— fillof(vsym)         施填充 变量 · 10
——
—— bool byte char string none null true false unsigned
—— i08 i16 i32 i64 i128 i256 i512 int arch_int type error
—— u08 u16 u32 u64 u128 u256 u512 unt arch_ptr type ptr
—— f08 f16 f32 f64 f128 f256 f512 float
—— d08 d16 d32 d64 d128 d256 d512 decimal
—— c08 c16 c32 c64 c128 c256 c512 complex
——
—— 布尔 字节 字符 字串 非值 空值 真 假
—— 整数 单整数 短整数 老整数 长整数 超整数 机器整数
—— 正数 单正数 短正数 老正数 长正数 超正数 机器正数
—— 定点 单定点 短定点 老定点 长定点 超定点
—— 浮点 单浮点 短浮点 老浮点 长浮点 超浮点
—— 复数 单复数 短复数 老复数 长复数 超复数
——
—— 如若 条件 { 返回 表达式 }
—— 如若 条件 { 表达式 } 然若 ……
—— 如若 条件 { 表达式 } 不然 { 表达式 }
——
—— 如若 条件 { 返回 表达式 }
—— 如若 条件 { 表达式 } 然若 ……
—— 如若 条件 { 表达式 } 不然 { 表达式 }
——
—— 撰程序入口 返回 ’整数 附公开属性 { 返回 0 }
——
——「」『』〈〉《》（）‹› « » … ‖／‥≈﹦≤﹤≥≠<﹢﹟﹡^－×±#&%⇥⇤⇵↕↔←↓↖↙↗↑↘∪⊥∠∟∥
—— ≡﹙﹚﹛－#^‰／﹡～±×&%‱﹟﹢∽＋÷*/﹠﹣∫∬∯∞∑∩∶∷∠⌒⊥∵∈∏∧∰∭∮∝∨∪•∴∥⊕
—— ⁹⋛⊰⊿⊙√㏒⊱⋘⋙⋚㏑∟½¼¾▕━◦‥；．，…‖﹒？′～ˉ•﹖﹌ˋ！‵ˆ﹐﹔﹗´＿﹕﹑ˇ‘︰¯¿¡﹍〒
—— ﹉﹎﹊￣﹁﹂¯＿―♭♮♩♪℅⚕℃¥£′㎜㏎㏄¤℉$₩″㎝㎎㏕₠₡○㎏㎞㎡¢€°₢₥₨₭₰￡
——
—— ≠ ≤ ≥ √ ± Ω Π ▲ ← → ↑ ↓ ÷×+-%
—— = ≠ < ≤ > ≥
—— ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩
—— ·、，。：；？！￥‘’**-“”（）【】《》、、、、
—— — — …… … … ·· ‥
—— +-*=~&|%#@{} … … ··
——
—— 造点 { ’浮点 横坐标 竖坐标 }
——
—— 造数组 @元素类型 常量‘整数曰大小 {
——    |大小|元素类型 元素
—— }
——
—— 撰传输协议轮询 *文件 *套接字 *轮询表 返回 ’轮询 捕获|参数·其二·其三| 附对齐属性|十六| {
——     立’轮询 = ？？
——     立*套接曰一 附对齐属性|缓存行其长| = 套接字
——     立’字节曰二 = ？？
——     立’整数曰状态 = ？？
——     立状态 = 0xffff
——     施等待 文件 · 套接字 · 时间
——     昔状态 = 施加载套接状态 套接字
——     如若 状态 等于 传输协议其监听状态 {
——          返回 施监听轮询 套接字
——     }
—— }
——
—— 撰程序入口 ’整数曰长度 **字节曰参数 返回 ’整数 {
——     返回 0
—— }
——
—— 撰弹出 *空闲队列 ’块尾偏移 返回 *队列内存块 附公开属性 {
——     如若 空闲队列之空闲块计数 小于等于 0 返回 空值
——     立*队列内存块曰空闲块 = ？？
——     如若 空闲队列之空闲块计数 等于 1 {
——         昔空闲块 = 施查找内存块起始 空闲队列之头元素 · 块尾偏移
——         昔空闲队列之头元素 = 昔空闲队列之尾元素 = 空值
——     } 不然 {
——         如若 空闲队列之头元素之|0| 等于 队列内存块尾值 {
——             昔空闲块 = 施队尾找队块 空闲队列之头元素 · 块尾偏移
——             昔空闲队列之头元素 = **空类 空闲队列之头元素之|1|
——         } 不然 {
——             昔头内存块 = 取值 空闲队列之头元素
——             昔空闲队列之头元素 = ~ 加上 1
——             施调试断言 空闲块 不等于 空值 并且 空闲块 不等于 队列内存块尾值
——             立’整数 = 1 加上（2 减去 3）乘以（4 除以 5）
——         }
——     }
——     昔空闲队列之空闲计数 = ~ 减去 1
——     返回 空闲块
—— }
