// 符号可以分为变量和类型，变量的底层表示其实就是一个内存中地址，包括全局变量、局部变量、
// 函数、跳转标签都是一个地址。类型只是一种语法概念并不占据实际物理世界空间，但这两种符号
// 都可以进行调用，变量的调用其实就是函数，类型的调用相当于模板类型实例化变成具体类型。其
// 实除了变量和类型，还存在一种更概念上的符号称为记号，包括包名、宏名。
//
// 关键字，去掉 default 因为可以用 else 实现，而 fallthrough 可以用 continue 代替。
//  if else elif for in break final return 条件语句支持大括号和缩进对齐两种编写方式
//  struct const void embed def pub let var undefined devel revel
//  continue defer yield range lambda reflex trait cold naked
//  static where it or this import scoped as inf (inferred type 推导的类型)
//  with fer der todo debug trap local global // 全局变量必须使用 global 引用
//  mod mut ref gen priv do abstract final macro tane (typename)
//  alignof type  sizeof type  offsetof type.offset
//
//  defer if error deallocation(ptr)
//
// act all are ago alt any auf aut
// can cat cor con cue des dhu din don dor dyn
// fac far fat fen fer fin fit fou fro fry fur
// gen gre lot off per pat pal phr par
// pre pro rem res rim ron rou rut
// tie via was wow yet as of do use
//
// 语句不会返回值，可以通过使用 with 和 where 关键字一前一后让语句变成语句表达式，从而
// 产生一个结果值。
//
// 名称标识符：
//  e_ 预留给错误码字符串，预定义的错误码，只保存错误数值
//  E_ 预留给错误码字符串，动态分配的错误码，保存完整字符串
//  __func() __name__ 以双下划线开始的名称都是保留关键字
//  标识符可以使用数字开头，此时不能以数字结尾，如果不以数字开头则可以使用数字结尾
//  单下划线有特殊含义，不能作为一般标识符使用，使用单下划线结束的结构体成员是私有成员
//  可以使用 #{name} 定义任意的标识符，例如 #{if} #{int}
//  3rd2.0 3rd2.2p 以数字开头的标识符，访问元组成员可能与浮点冲突
//
// 命名空间：
//  1.  函数和闭包内部，会定义一个完全隔离的独立的局部作用域空间，在进入函数和闭包后，
//      除了捕获参数和函数实参名称外，不定义任何其他局部名称。对于全局名称，函数和闭包
//      只能访问当前文件种引入的全局名称。
//  2.  当前文件作用域，相当于一个除去函数定义的文件作用域函数。
//  3.  闭包和宏定义的内部变量也具有局部性，所有外部引入的参数都需要明确声明。
//  4.  函数内部定义的变量名，可以覆盖全局名称，但不能覆盖局部作用域中的名称。
//  5.  因此函数局部作用域中的变量名，可以与全局类型名或全局变量名同名，同名的类型名和
//      变量名，优先识别为变量，但其后不是操作符（包括分隔符）则识别为类型名。如果覆盖
//      的全局类型与变量的类型是同一个类型，仍然可以通过 typeof(name) 访问到全局类型。
//      如果覆盖的全局类型与变量的类型不是同一个类型，仍然可以通过 global.name 访问到
//      全局的类型名称。如果局部变量覆盖的是全局变量，也仍然可以通过 global.name 访问
//      这个全局变量。当前文件作用域中的不冲突的名称，也仍然可以通过 name 直接访问。而
//      全局作用域中引入的与当前文件全局作用域完全隔离的名称，一定不会与当前函数局部作
//      用域内的名称冲突，也仍然可以通过 #std.io.print 访问。
//  6.  函数内部定义的类型名，也可以覆盖全局名称，但不可以覆盖局部作用域中的变量名，也
//      不可以覆盖局部作用域中定义的类型名，以及其他局部名称。但是使用该类型可以定义一
//      个同名的变量名，该类型可以通过 typeof(name) 访问到。
//
// 函数的实现：
//  1.  函数参数可以设置对齐限制，编译器会强制检查类型的对齐属性是否满足要求
//  2.  函数本身也可以设置对齐属性，编译器会将函数生成对对应对齐的地址处
//  3.  不使用成员函数调用语法，所有函数调用都使用 C 函数调用方法
//  4.  a.field 语法仅表示变量 a 访问其类型的内部成员，或类型 a 访问类型的内部成员
//  5.  支持函数重载，支持可选参数和命名参数，可以通过命名参数不按声明顺序传递参数
//  6.  指针可修改和不可修改性复杂度很大，正确实现的难度很大，只要传指针就认为会修改
//  7.  函数参数的传值和传指针与 C 函数相同，而调用约定可能不同，但仍然兼容 C 调用约定
//  8.  当传值时，表示不修改传入的参数对应的变量，如果传入大于指针大小的类型例如 vec3，
//      如果函数内部确实不会修改参数的内容，可以直接传传入变量的地址，否则需要进行拷贝。
//      默认对于大小不大于指针的参数，其副本是可以修改的，声明为 mut 没有任何效果。对
//      于大小大于指针的参数，其副本默认是不可修改的，如果要修改参数副本，需要声明 mut
//      从而传递实参的一份拷贝，实现对副本的修改不会影响传入的实参。
//
//      只有小于等于两个字长的命名类型才可以传值，其他都只能传地址，传地址的变量如果不想
//      修改其自身，可以使用语法 test(&copyof a)，即使是双字长的结构体也只传一个指针，
//      因为需要修改成员，传递一个成员指针和两个成员指针区别不大。在函数中对传入的参数
//      取地址，可能（通过寄存器而不是通过栈传递的情况下）需要将寄存器中的值重新复制到
//      栈中。
//
// 宏的实现：
//  1.  本质上说，宏是一种编写可以生成源代码的代码的方式，称为元编程。宏具有一些函数所
//      没有的额外能力，函数必须声明函数具有的参数数量和类型，宏则可以接受可变数量的参
//      数。宏在编译器解释代码含义之前就被扩展，因此宏可以例如在给定类型上实现 trait。
//      函数不能这样做，因为它在运行时被调用，而 trait 需要在编译时实现。实现宏而不是
//      函数的缺点是，宏定义比函数定义更复杂，因为你正在编写编写源代码的源代码。由于这
//      种间接性，宏定义通常比函数定义更难阅读、理解和维护。宏和函数之间的另一个重要区
//      别是，你必须在文件中调用宏之前定义宏或将它们引入作用域，而函数你可以在任何地方
//      定义并在任何地方调用。
//  2.  Rust 中最广泛使用的宏形式是声明宏，或简称为"宏"，在其核心，声明宏允许你编写类
//      似于 Rust match 表达式的内容。值是传递给宏的字面 Rust 源代码，模式与该源代码
//      的结构进行比较，当匹配时，与每个模式关联的代码替换传递给宏的代码。这全部发生在
//      编译期间。
//  3.  我们可以使用 vec! 宏创建两个整数的向量或五个字符串切片的向量。我们无法使用函数
//      来做同样的事情，因为我们无法预先知道值的数量或类型。
//          #[macro_export]
//          macro_rules! vec {
//              ($($x:expr), *) => {
//                  {
//                      let mut temp_vec = Vec::new();
//                      $(
//                          temp_vec.push($x);
//                      )*
//                      temp_vec
//                  }
//              };
//          }
//  4.  这里我们有一个带有模式 ( $( $x:expr ),* ) 的目标，宏定义中的有效模式语法与普
//      通模式语法不同，因为宏模式是针对 Rust 代码结构而不是值进行匹配的。首先，我们使
//      用一组括号来包含整个模式。使用 $ 在宏系统中声明一个变量，该变量将包含与模式匹配
//      的 Rust 代码。接下来是一组括号，捕获与括号内模式匹配的值，用于替换代码。在 $()
//      内是 $x:expr，它匹配任何 Rust 表达式，并将表达式命名为 $x。$() 后面的逗号表示，
//      与 $() 中代码匹配的代码的每个实例之间必须出现字面逗号分隔符，* 指定该模式匹配零
//      个或多个前面的任何内容。
//  5.  当我们用 vec![1, 2, 3]; 调用此宏时，$x 模式与三个表达式 1、2 和 3 匹配三次。
//      现在让我们看看与此目标关联的代码主体中的模式：$()* 内的 temp_vec.push() 根据
//      模式匹配的次数生成零次或多次，每次匹配 $() 的一部分，$x 被替换为每个匹配的表达
//      式。
//  6.  宏的第二种形式是过程宏，它更像函数（并且是一种过程）。过程宏接受一些代码作为输
//      入，对这些代码进行操作，并产生一些代码作为输出，而不是像声明宏那样匹配模式并用
//      其他代码替换代码。定义过程宏的函数接受 TokenStream 作为输入，并产生 TokenStream
//      作为输出。
//
//  https://doc.rust-lang.org/reference/macros-by-example.html
//  https://lukaswirth.dev/tlborm/ (https://zjp-cn.github.io/tlborm/)
//
// 程序最基本的元素只有：
//  变量，其中编译时已知的变量称为常量
//  类型，类型是一种特殊的变量，该变量的类型为 anytype，类型可以是编译时已知的，也可以处理一个运行时才已知的类型
//      anytype // 一个具体类型
//      inttype // 一个有符号整数类型
//      unttype // 一个无符号整数类型
//      flotype // 一个浮点类型
//      dectype // 一个定点类型
//      comtype // 一个复数类型
//      integer // 一个整数类型，包括有符号和无符号
//      numeric // 一个数值类型，包括整数类型、浮点类型、定点类型、复数类型
//      settype // 一个集合类型
//      maptype // 一个映射类型
//
//  参数化类型，一个返回类型或参数化类型的函数
//  def func(type a type b return type) { ... }
//  def type $(anytype T anytype U int SIZE const C) { ... } // C 可以时常量，也可以是编译时已知类型
//
// 定义类型参数（generic type parameter）和常量参数（compile time const parameter）
//  $T $U
//  $(anytype T) $(anytype U)
//  $(inttype I)
//  $(flotype F)
//  $(numeric N)
//  const C
//  const int C
//  const string S
//  const point POINT
//
// 下划线保留字：
//  __file__
//  __func__
//  __line__
//  __retp__
// 编译时指令： #label_name 定义标签名称
//  PRH_TCPA_#(OPEN_REQ) #{int} #{if} ${int} ${if} #'operation
//  global.override_name 在局部作用域中访问局部作用域中被局部作用域覆盖的名称
//  global.std 访问当前文件全局作用域中的名称 std，一般名称与名字空间名称是隔离的
//  ::namespace::subspace::name 访问全局作用域中定义的名字空间中的名称
//  std::print 访问全局作用域中名字空间 std 中的名称 print
//  static expr
//  static if
// 符号属性，包括函数、类型、变量的属性名称等
//  alignas(n) forcedalignas(n) // forcedalignas(n) 不会被 packed 属性抑制
//  "fastcall" "cdecl" "stdcall" "strict" "nacked"
//  "nonzero" "nonalls" "zeroinit" "packed"
// 内置函数：
//  abort() panic()
//  assert(expr)
//  resert(expr)
//  devel { stmt ... }
//  devel print
//  devel prine
//  devel prerr
//  alignof(vsym) sizeof(expr) typeof(expr)
//  copyof(vsym) moveof(vsym) zeroof(vsym) fillof(vsym)
// 预定义类型
//  this 当前函数（当前函数的地址）或当前结构体，不提供任何面向对象的特殊含义，但匿名类型需要用
// 特殊操作符
//  (&) (*) (**) (*&) (**&) (&1) (&2) (*&1) (*&2)
//
// 基本类型，定义在 type 代码包中：
//  bool null none byte char rune string errno struct "32-byte" i32 r32 int reg arch_int arch_reg arch_reg struct i struct u struct p
//  i08 i16 i32 i64 i128 int <32>int <64>int <128>int (b w d q x y z p r) byte word double-word quad-word xmm-word ymm-word zmm-word
//  r08 r16 r32 r64 r128 reg <32>reg <64>reg <128>reg ...
//  f08 f16 f32 f64 f128 float <32>float <64>float <128>float
//  d08 d16 d32 d64 d128 decimal <32>decimal <64>decimal ...
//  c08 c16 c32 c64 c128 complex <32>complex <64>complex ...
//
//  bool byte char string none null true false def error
//  i08 i16 i32 i64 i128 i256 i512 int arch_int
//  r08 r16 r32 r64 r128 r256 r512 reg arch_reg
//  f08 f16 f32 f64 f128 f256 f512 float
//  d08 d16 d32 d64 d128 d256 d512 decimal
//  c08 c16 c32 c64 c128 c256 c512 complex
//
//  int // signed pointer size integer
//  reg // unsigned pointer size integer // 使用 ~ 开始的标识符可以转义关键字 ~int ~reg
//  arch_int // real architecture register size signed integer
//  arch_reg // real architecture register size unsigned integer
//  bool byte char error float decimal complex
//  array string slice
//
//  08`     // r08 unsigned integer 单字节
//  16`     // r16 unsigned integer 两字节
//  32`     // r32 unsigned integer 四字节
//  64`     // r64 unsigned integer 八字节
//
//  08`int  // i08 signed integer
//  16`int  // i16 signed integer
//  32`int  // i32 signed integer
//  64`int  // i64 signed integer
//
//  rx`     // rrx r128 signed integer 16字节  128位
//  ry`     // rry r256 signed integer 32字节  256位
//  rz`     // rrz r512 signed integer 64字节  512位
//
//  08`float f08            08`decimal d08              08`complex c08
//  16`float f16            16`decimal d16              16`complex c16
//  32`float f32            32`decimal d32              32`complex c32
//  64`float f64            64`decimal d64              64`complex c64
//  rx`float ffx f128       rx`decimal ddx d128         rx`complex ccx c128
//  ry`float ffy f256       ry`decimal ddy d256         ry`complex ccy c256
//  rz`float ffz f512       rz`decimal ddz d512         rz`complex ccz c512
//
//  .digit 表示一个十进制数
//  ident` 表示是类型名称
//  .ident = expr 表示成员赋值
//  a.ident (expr).ident() 表示成员访问，点号与之前的a之间不能有空格，但点号之后可以有空格
//  ::ident 访问类型成员
//
// 简洁尽量实现使用最少字符
//  布尔类型    布尔常量    空值    字符类型    字符串类型
//  'B          @t  @f      @z      'c          's
//  bool        true false  null    char        string
//  单字节  双字节  四字节  八字节  指针或向量大小
//  '1      '2      'i      '8      'j      arch_int      有符号整数
//  'b      'w      'u      'q      'p      arch_reg    无符号整数
//  f08     f16     'f      'g      'x 'y 'z            单精度/双精度/128-bit xmm/256-bit ymm/512-bit zmm
//  i08     i16     int     i64     'j      arch_int
//  r08     r16     r32     r64     'p      arch_reg
//
// 类型信息：type|flag
//  i08 ~ <128>int      0   ~   15
//  f08 ~ <128>float    16  ~   31
//  d08 ~ <128>decimal  32  ~   47
//  c08 ~ <128>complex  48  ~   63
//  string 64 array 65 slice 66 view 67 struct 68 sumt 69 func 70
//  int/arch_int: ixx|int: 1
//  reg/arch_reg: ixx|reg: 1
//  ptr/arch_reg: uxx|ptr: 1
//  bool: r08|bool:1
//  null: r08|null:1
//  char: r32|char:1
//  errno: int|errno: 1
//  enum: ixx|enum: 1
//
// rax rbx rcx rdx rbp rsp rsi rdi rlr rip rpc
// bax bbx bcx bdx bbp bsp bsi bdi blr bip bpc
//
//                                          arch-32     arch-64 small memory range app  arch-64 large memory range app
//  int - pointer size signed type          32-bit      32-bit                          64-bit
//  reg - pointer size unsigned type        32-bit      32-bit                          64-bit
//  ptr - pointer type                      32-bit      32-bit                          64-bit
//  arch_int - system register size type     32-bit      64-bit                          64-bit
//  arch_reg - system register size type     32-bit      64-bit                          64-bit
//  arch_reg - system register width pointer 32-bit      64-bit                          64-bit
//
// 类型约束：
//  Any Integer Float Unsigned Decimal Complex BasicType AnonyType NamedType GimplType
//
// 类型字面量（TypeLit）包括基本类型（BasicType）、匿名类型（AnonyType)、命名类型（NamedType）、实列化类型（ImpledType）。
//  any          // basic_type + anony_type + named_type + gimpl_type
//  basic_type   // numeric + string
//  numeric      // integer + float + decimal + complex
//  integer      // 枚举类型 bool null byte rune errno strt i08~i512 r08~r512 int reg
//  instant_type
//  generic_type
//
// 函数参数中的类型参数，重载操作符 == 可以用来比较类型：
//  def GetType(anytype T anytype U return anytype) {
//      anytype a = T
//      if typeof U == i64 { a = U }
//      return a
//  }
//  def calc($T a $(float U) b T c U d return T)
//  def calc($T a $(float U) b GetType(T, U) c return GetType(T, U))
//  def calc([const N]$T a) { prine("size % % type %", typeof N, N, T) }
//  def calc([const int N]$T a) { ... }
//  var a = {1, 2, 3, 4}
//  calc(a)
//  def calc($T a T.K key T.V value) // 类型 T 必须包含 K 和 V 类型
//  def calc($T a) static has_vector_fields(T)
//  def has_vector_fields(anytype t return bool) {
//      return t.hasfield(float x) && t.hasfield(float y) && t.hasfield(float z)
//  }
//
// 结构体中的类型参数，类型参数和常量参数仅存在于编译时，不占用结构体变量的运行时内存：
//  def type $(const C) { int a }
//  let type(42) a = undefined
//  var x = 2
//  let type(x) b = undefined // 错误，x 不是常量
//  def y = 3
//  let type(y) c = undefined // 正确，y 是常量
//  def type $(anytype T) { T data }
//  let type(int) a = undefined
//  def type $(anytype T T C) { T data } // 成员名称 data 不能与 T 或 C 重名
//  let type(int, 3) a = undefined
//  prine("type % C %", a.T, a.C)
//
// 递归类型：
//  def FooType $(anytype T (T a return int) F) {}
//  def BarType $(anytype T) { def [FooType(BarType(T), BarFunc)] type }
//  def BarFunc(BarType($T) a return int) { return 42 }
//
// 类型实例化，已经访问类型的成员
//  array(3, int)
//  array(3, array(8, float))
//  def [array(3, int)] int_array
//  def [int_array.T] elem_type
//  offsetof int_array.capacity
//
// 复合类型和匿名类型：
//  int  r16  f64  Point  MyInt
//  *int *r16 *f64 *Point *MyInt
//  [2]int [2]r16 [2]f64
//  *[2]int *[2]r16 *[2]f64
//  *int *r16 *f64 *Point
//  *[2]int *[2]r16 *[2]f64
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
//      {int int}            结构体不能同时定义两个同类型的内嵌字段，等价于结构体 {int ${0} int ${1}}
//      {this int int int}   结构体不能内嵌一个指针类型，等价于结构体 {this ${0} int ${1} int ${2} int ${3}}
//      def data {this int int int} 元组成员的命名，可以延迟到使用时
//      let ptr, a, b, c = data {this, 1, 2, 3}     // 可以使用 ptr a b c
//      let _, a, _, b = data {this, 1, 2, 3}       // 可以使用 a b，第一个成员只能使用 this 初始化，否则报错
//      let data = data {this, a = 1, b = 2, 3}     // 可以实现对元组的修改 data.a = 10  data.b = 20
//      let data = data {this, 1, 2, 3}             // 可以使用 data.0 data.1 data.2 data.3
//      def data = {this, a = 1, b = 2, 3}          // 可以实现对元组的修改 data.a = 10  data.b = 20
//      def data = {this, 1, 2, 3}                  // data.0 不能修改 data.1 = 10  data.2 = 20
//  Enum 枚举类型，只能表示整数常量，枚举是结构体模板的一种特殊形式
//      const i08 {RED = const * 2 YELLOW BLUE} // const 是枚举元素的索引值
//      const int {RED YELLOW BLUE}
//  Interface // 接口不能声明为空，必须包含成员函数声明，也只能包含成员函数声明或内嵌接口，接口是一个没有成员只有静态数据的结构体，接口声明也只是结构体模块的一种特殊形式
//      $p { (*p int size return int) read (*p return int) get } // 允许使用关键字 this 定义 $this，然后参数声明使用 (this int size return int)
//  Struct 表示定义一个类型
//      def empty {}
//      { int a b } {1, 2}
//      { (this int a b int) calc }
//      $t $u const size/int { [size]t a u b }
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
//  Func (return r32 bool) 参数为空，返回u32和bool
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
//  Triple $T $reg size { [size]T a U b }
//  length (Point($T) p T) { return p.x * p.x + p.y * p.y }
//  size (Triple($int N, $T, $U) p int) { return N + sizeof T + sizeof U }
// 模板类型实例化：                                                      代码中仅允许使用这种简化的形式
//  ptr(int) ptr(ptr(int))                                              int* int**              type::int* int.type::ptr
//  array(int, 3)                                                       int[3]                  int.type::array(3)
//  slice(int)                                                          int.slice               int.type::slice
//  map(string, int)                                                    string.map(int)         string.type::map(type::int)
// 指针的指针，指针的数组：
//  ptr(ptr(int))                                                       int** int**
//  array(ptr(int), 3)                                                  int*[3]
//  slice(ptr(int))                                                     int*.slice
//  map(ptr(string), int)                                               string*.map(int)
//  map(string, ptr(int))                                               string.map(int*)
// 数组的指针：
//  ptr(array(int, 3))                                                  int[3]*
//  ptr(slice(int))                                                     int.slice*
//  ptr(map(string, int))                                               string.map(int)*
//  ptr(array(ptr(int), 3))                                             int*[3]*
//  ptr(slice(ptr(int)))                                                int*.slice*
//  ptr(map(ptr(string), int))                                          string*.map(int)*
//  ptr(map(string, ptr(int)))                                          string.map(int*)*
// 数组的映射：
//  map(array(int, 3), int)                                             int[3].map(int)
//  map(slice(int), int)                                                int.slice.map(int)
//  map(map(string, int), int)                                          string.map(int).map(int)
//  map(array(ptr(int), 3), int)                                        int*[3].map(int)
//  map(slice(ptr(int)), int)                                           int*.slice.map(int)
//  map(map(ptr(string), int), int)                                     string*.map(int).map(int)
//  map(map(string, ptr(int)), int)                                     string.map(int*).map(int)
//  map(string, array(int, 3))                                          string.map(int[3])
//  map(string, map(string, int))                                       string.map(string.map(int))
//  map(string, slice(int))                                             string.map(string.map(int))
//  map(string, array(ptr(int), 3))                                     string.map(int*[3])
//  map(string, slice(ptr(int)))                                        string.map(int*.slice)
//  map(string, map(ptr(string), int))                                  string.map(string*.map(int))
//  map(string, map(string, ptr(int)))                                  string.map(string.map(int*))

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

// 类型转换，当表达式中的类型转换需要添加括号时，需要使用 ('type expr) 的形式，避免与函数类型冲突
a + int b + c * d
a + ('int b + c) * d
// 逗号只能出现在 {} 或 [] 或 let 表达式中，不能出现在 () 中，避免与函数类型冲突
(point, camera) // 括号 () 中出现逗号必然是函数声明，只有函数类型声明中才能包含逗号
// 让类型字面量和复合常量字面量表示唯一，其他都必须为之让路
// 函数类型字面量，“开始小括号 + 结果为类型的表达式” 表示函数类型的开始，函数复合常量是函数类型字面量 + { stmt ... }
(void) // void 表示没有参数，也没有返回值
(return int) // 返回 int，当有返回值时才需要 return 关键字
(return int string point) // 返回 int string point
(return int float yield int)
(yield int point return int)
(yield int point)
(int a return int) // 返回 int
(int a) // 没有返回值，不需要 return 关键字
(int argc **char argv return int or none)
(point p int a return int) // 返回 int
(point point int a return int) // 命名与类型同名的参数，不能写成 point point，两个类型名将触发返回值的声明的开始
(point, int a return int) // 简写形式
(camera camera point point)
(point point)
(camera, point) (camera = expr, point) // 简写形式
(point) (point`) (point .= expr) // 简写形式，使用 point` 和 .= 避免与表达式语法冲突
(*camera camera *point point)
(*point point)
(*camera, *point) // 简写形式
(*point) // 简写形式
(int a)
(int a int b int c)
(int a..b..c) // 简写形式
(int a int b return int yield int point)
(int a int b yield int point return int)
(int a int b yield int point)
(int a..b yield int point)
(int a return int point float (count point scale))
(*file? file = stdin, point point string name = "root", string mode)
(*file? = stdin, point, string name = "root", string mode) // 简写形式
(*file? = stdin, point point point origin string name string mode int a int b int c)
(*file? = stdin, point, point origin string name..mode int a..b..c)
(*file? = stdin, point point..origin string name..mode int a..b..c)
// 元组类型，以一元操作符 < << <<< ... 开始的表示元组类型的开始
[int] // 特殊情况外不是一个元组，元组必须至少包含两个元素，但仍然可以通过 [int $] 来表示
[int] (x $) // int 成员命名为 x
[int point int]
[int point int] (a b c)
[int point int] (_ b _)
[int int string]
[point point]
// 结构体类型字面量，“起始大括号 + 结果为类型的表达式” 称表示结构体的开始
struct {} // 空结构体
{int a} // 如果不使用特殊语法表示类型转换，这里可以解析成将变量 a 转换成 int 类型，然后将其值作为语句块的值
{int a int b}
{int a..b}
{int a int b point o string s}
{int a..b point o string s}
{point point} // 怎么区分是结构体还是元组呢，是元组，因为结构体成员必须声明名称，但这里其实是一样的，因为元组同样可以通过类型名point访问这个成员
{int point} // point 是 int 型类型成员
const { red green blue }
const int { red green = 2 blue }
$(anytype T) { (*T p int size return int) read }
// 大括号初始化列表，数组/元组/结构体/集合/映射都通过大括号进行初始化
{expr, expr, expr} // expr 绝对不会以类型名称或类型字面量开头
{:1 :2 :3 :4}
|flat_set|{:1 :2 :3}
{"a":1, "b":2, "c":3}
// 模板类型的实例化，不会与元组的 let 赋值冲突，因为元组的赋值必须包含两个元素且用空格分隔 let data(result error) = read_tuple()
array(int, float)
*array(int, 24)
[3]array(int)
*[5]array(int)
[5]*array(int)
// 字面量数组及相关，使用前缀方括号操作符表示数组/集合/映射等类型
*[N]Type // [N] [] [_] * ** 的排列组合，N 是一个常量表达式，常量表达式会进行编译时即时计算
[*]Type // 指向的内容是一个 Type 类型的数组，长度不定，另外 *Type 表示指向单个 Type 值
*[N]Type // 指向的内容是一个 [N]Type 类型
[*][N]Type // 指向的内容是一个 [N]Type 类型的数组
[N][N]Type
[N]*Type
[:int] // 集合类型
|flat_set|[:int] // 自定义集合类型
[string:int] // 映射类型
|flat_map|[string:int] // 自定义映射类型
[string:*int]
*[string:[N]int]
where [m &a &b] { stmt... } // 捕获参数
FuncTypeLit [m &a &b] { stmt... } // 捕获参数
vsym[expr] vsym[expr][expr] // 变量数组元素
func()[expr] // 函数返回值数组元素
func()[expr:expr] vsym[expr:expr] // 数组切片和字符串切片
if [expr] // 条件匹配语句
for [&it] // 迭代元素捕获
[global] // 全局顶行出现的配置项
[apple]fruit
[apple.physical]
[fruits][]fruit
// 数据生成表达式
[yield a + b for a in array for b in 1 .. 100] // 生成一个整数数组，每个元素的值为 a+b
[yield int a * 2 for a in array] // 带类型转换的生成数组
[yield {to_string(a), a ^ b, a + b == b} for a in array for b in 1 .. 100] // 生成一个元组数组
[yield {name = to_string(a), xor = a ^ b, a + b} for a in array for b in array] // 生成一个元组数组，并为元组的成员命名
[yield |set| a + b for a in array for b in 1 .. 100] // 生成一个集合
[yield |flat_set| a + b for a in array for b in array] // 生成一个 flat_set 集合
[yield |map| a + b : a * b for a in array for b in array] // 生成一个映射
[yield |flat_map| a + b : a * b for a in array for b in array] // 生成一个 flat_map 映射

// 内存分配的类别
//
//  1.  固定大小栈分配
//  2.  程序初始化时的全局分配
//  3.  即用的生存期非常明确的分配，其生存期编译器可以静态推断
//  4.  存在一个周期的可以重用的周期性释放和分配
//  5.  不能通过静态分析推断生存期的分配
//  6.  分配的释放必须早于内存池的释放
//  7.  注意以上类型的相互赋值
//
// 数组实现
//
//  1. 数组字面量，位于只读分区，添加 local 关键字表示局部与函数作用域的字面量，只能传递给 view
//      array ---->[item][...] // 不保存大小，大小和元素大小由编译器记录，参数传递时可传递给 array_view
//
//  2.  编译时大小固定的数组，即栈上或全局分配的大小固定数组，只读时传递给 view，修改时传递给 slice
//      array ---->[item][...] // 不保存大小，大小和元素大小由编译器记录，参数传递时可传递给 array_view
//
//  3.  拥有内存的数组空间，位于可写分区
//                 [0|int size][byte ...] // size 是整个字节串的最大空间大小
//      [allocator][1|int size][byte ...]
//      alloc_buffer -------->'
//
//  4.  可变大小字符串，这些栈状态结构体都在块作用域结束后自动释放
//
//      def array $type {
//          *type data @from(alloc_buffer);
//          int count;
//      } // array 可以传值
//
//      def d_array { // 双端数组
//          *type data @from(alloc_buffer);
//          int count;
//          int start;
//      }
//
//  5. 视图和分片
//
//      def array_view $type {
//          *type data;
//          int count;
//      }
//
//      def array_slice $type {
//          *type data;
//          int count;
//      }
//
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
//      def slice { // 可写的字符串切片
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
//      } // string 可以传值
//
//      def d_string { // 双端字符串
//          *alloc_buffer data
//          int count
//          int start
//      }
//
//      def init_string(int size return string)
//      def init_string(int size return d_string)
//
//      let string s = init_string(672)
//      let d_string s = init_string(1024)
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

// 表示法（Notation）。词法分析和语法的描述使用了一种混合了 EBNF（Extended Backus-Naur
// Form）和 PEG（Parsing Expression Grammar） 的语法表示法。例如：
//      name:   letter (letter | digit | "_")*
//      letter: "a"..."z" | "A"..."Z"
//      digit:  "0"..."9"
//
//  rule_name: expression
//      可选地，可以在规则名后包含返回类型，指定对应规则的C或Python函数的返回类型：
//  rule_name[return_type]: expression
//      如果不提供返回值，则对应规则的C函数返回void*，Python函数返回Any
//  "invalid_" 开头的规则名，用于专门的语法错误处理
//      . 这些规则不会在解析器的第一次遍历中使用
//      . 只有当第一次遍历解析失败时，才会执行包含无效规则的第二次遍历
//      . 如果第二阶段以通用语法错误失败，将使用第一次遍历中通用失败的位置（这避免了由于无效规则而报告错误位置）
//      . 涉及无效规则的替代项（alternatives）的顺序很重要（就像PEG中的任何规则一样）
//
// 第一行表示名称是一个字母后跟零个或多个字母、数字和下划线的序列。字母依次是单个字符
// 'a' 到 'z' 和 'A' 到 'Z' 中的任何一个，数字是 0 到 9 的单个字符。每条规则以名称
// （标识正在定义的规则）开头，后跟冒号，冒号右侧的定义使用以下语法元素：                *** https://docs.python.org/3/reference/grammar.html#full-grammar-specification
//  name
//      名称引用另一条规则，在可能的情况下，它是规则定义的链接
//  TOKEN
//      大写名称表示一个词法元素标记，就语法定义而言，标记与规则相同
//  "text" 'text'
//      单引号或双引号中的文本必须按字面匹配（不带引号），引号类型根据文本含义选择：
//      "case"：双引号中的名称表示软关键字（SOFT KEYWORDS）
//      'if'：单引号中的名称表示关键字（KEYWORDS）
//      '@'：单引号中的非字母符号表示 OP 标记，即分隔符或运算符
//  e1 e2
//      仅用空白分隔的项表示序列，表示匹配 e1 然后匹配 e2
//  e1 | e2
//      竖线用于分隔替代项（alternatives），匹配 e1 或 e2，它表示 PEG 的 "有序选择"，
//      如果 e1 匹配，则不考虑 e2，在传统 PEG 语法中，这写作斜杠 / 而不是竖线，更多背   *** https://peps.python.org/pep-0617/ New PEG parser for CPython
//      景和细节参见 PEP 617
//  ( e )
//      括号用于分组，匹配 e，也允许在分组中使用其他运算符，如 (e)*
//  [ e ] 或 e?
//      方括号括起的短语表示零次或一次出现，换句话说，括起的短语是可选的。问号与方括号
//      含义完全相同，该项是可选的
//  e*
//      星号表示前一项的零次或多次重复，匹配零次或多次 e
//  e+
//      加号表示前一项的一次或多次重复，匹配一次或多次 e
//  s.e+
//      匹配一次或多次 e，以 s 分隔，生成的解析树不包含分隔符，不然等同于 (e (s e)*)
//  &e
//      如果 e 可以被解析，则成功，但不消耗任何输入
//  !e
//      如果 e 可以被解析，则失败，但不消耗任何输入
//  ~
//      一旦遇到 ~，解析器就承诺选择当前这个替换项，不再考虑其他替换项，如果当前替换项
//      解析失败，整个规则直接失败，不会回溯去尝试其他替换项。这与普通的 PEG 解析行为
//      不同，通常一个替换项失败后会自动尝试下一个替换项，而 ~ 表示切割或提交点，其作用
//      是禁止这种回溯，用于优化和生成更好的错误消息。Python主要使用切割来进行优化或改
//      进错误消息。在语法列表中，它们通常看起来毫无用处。切割目前不会出现在括号、方括
//      号、向前查看等类似结构中，它们在这些上下文中的行为被故意未作规定。
//  &&e
//      急切解析 e，解析器不会回溯，如果 e 无法被解析，将立即以 SyntaxError 失败
//
// 序列（e1 e2）的优先级高于替代项（e1 e2 | e3），一元运算符（*、+、?）尽可能紧密绑定，
// 竖线（|）绑定最松散。空白仅在分隔标记时有意义。一些定义还使用向前查看（lookahead），
// 指示元素必须（或必须不）在给定位置匹配，但不消耗任何输入：
//  1.  &e：正向前查看，即要求e匹配
//  2.  !e：负向前查看，即要求e不匹配
//
// 以下表示法仅用于词法定义：
//  1.  "a"..."z"：由三个点分隔的两个字面字符表示给定（包含）ASCII 字符范围内的任何单
//      个字符的选择
//  2.  <...>：尖括号之间的短语给出匹配符号的非正式描述，例如 <除""外的任何ASCII字符>，
//      或在附近文本中定义的缩写，例如 <Lu>
//
// 规则通常包含在单行上，但过长的规则可以换行：
//      literal: stringliteral | bytesliteral
//          | integer | floatnumber | imagnumber
//
// 或者，规则可以格式化为第一行以冒号结尾，每个替代项以竖线开头在新行上，这并不意味着存
// 在空的第一个替代项。
//      literal:
//          | stringliteral
//          | bytesliteral
//          | integer
//          | floatnumber
//          | imagnumber
//
// 词法和语法定义。词法和语法分析之间存在一些差异：词法分析器对输入源的单个字符进行操作，
// 而解析器（语法分析器）对词法分析生成的标记流进行操作。然而，在某些情况下，两个阶段之
// 间的确切边界是 CPython 实现细节。两者的实际区别在于，在词法定义中，所有空白都是重要
// 的。词法分析器丢弃所有未转换为 token.INDENT 或 NEWLINE 等标记的空白。然后语法定义
// 使用这些标记，而不是源字符。本文档对两种定义风格使用相同的 BNF 语法。下一章（词法分
// 析）中 BNF 的所有使用都是词法定义，后续章节中的使用是语法定义。
//
// https://peps.python.org/pep-0617/ New PEG parser for CPython
//
// 本 PEP 提议用一个新的基于 PEG 的解析器替换 CPython 当前基于 LL(1) 的解析器。这个新   *** LL(1) 只能向前查看1个token的自顶向下解析器
// 解析器将允许消除当前语法中为规避 LL(1) 限制而存在的多种"hack（技巧/变通方法）"。它将
// 大幅降低与编译管道相关某些领域的维护成本，例如语法、解析器和 AST 生成。新的 PEG 解析   *** PEG 解析表达式语法解析器，支持无限向前查看
// 器还将解除当前 Python 语法的 LL(1) 限制。
//
// 背景：LL(1) 解析器。当前的 Python 语法是基于 LL(1) 的语法。如果一个语法可以被 LL(1)
// 解析器解析，那么它就可以被称为 LL(1) 语法。而 LL(1) 解析器被定义为一种自顶向下的解
// 析器，它从左到右解析输入，对句子执行最左推导，并且只使用一个 token 的前瞻。构建或生
// 成 LL(1) 解析器的传统方法是生成一个解析表，该表编码了解析器所有可能状态之间的可能转
// 换。这些表通常根据语法的 first 集和 follow 集构建：
//  1.  给定一个规则，first 集是在该规则的完整推导中可以作为第一个出现的所有终结符的集
//      合。直观地说，这帮助解析器决定规则中的备选方案。例如，给定规则：
//          rule: A | B
//      如果只有 A 可以以终结符 a 开头，只有 B 可以以终结符 b 开头，当解析器在解析此规
//      则时看到 token b，它就知道需要遵循非终结符 B。
//  2.  当规则可能展开为空字符串时，需要对这个简单想法进行扩展。给定一个规则，follow 集
//      是在部分推导中可以立即出现在该规则右侧的所有终结符的集合。直观地说，这解决了空备
//      选方案的问题。例如，给定规则：
//          rule: A 'b'
//      如果解析器拥有 token b，而非终结符 A 只能以 token a 开头，那么解析器可以判断
//      这是一个无效的程序。但如果 A 可以展开为空字符串（称为 ε-产生式），那么解析器会
//      识别出一个有效的空 A，因为下一个 token b 在 A 的 follow 集中。
//
// 当前的 Python 语法不包含 ε-产生式，因此在创建解析表时不需要 follow 集。目前，在 CPython
// 中，一个解析器生成器程序读取语法并生成一个解析表，该表表示一组确定性有限自动机（DFA），
// 可以包含在 C 程序（即解析器）中。该解析器是一个下推自动机，使用这些数据生成具体语法树
// （CST），有时直接称为"解析树"。在此过程中，first 集在生成 DFA 时被间接使用。
//
// LL(1) 解析器和语法通常高效且易于实现和生成。然而，在 LL(1) 限制下，无法以自然于语言
// 设计者和读者的方式表达某些常见结构，Python 语言中就包含一些这样的例子。由于 LL(1) 解
// 析器只能向前查看一个 token 来区分可能性，语法中的某些规则可能是模糊的。例如规则：
//      rule: A | B
//
// 如果 A 和 B 的 first 集有一些共同元素，那么这个规则就是模糊的。当解析器在输入程序中
// 看到一个 A 和 B 都可以开始的 token 时，它无法推断应该展开哪个选项，因为无法检查程序
// 的更多 token 来消除歧义。该规则可以转换为等价的 LL(1) 规则，但这样人类读者可能更难理
// 解其含义。本文档后面的示例表明，当前基于 LL(1) 的语法深受这种情况的困扰。
//
// 另一类被 LL(1) 排除的广泛规则是左递归规则。如果一个规则可以推导出一个以自身作为最左
// 符号的句型，那么它就是左递归的。例如下面这个规则是左递归的，因为该规则可以展开为一个
// 以自身开头的表达式。正如后面将要描述的，左递归是直接在语法中表达某些期望语言特性的自
// 然方式。
//      rule: rule 'a'
//
// 背景：PEG 解析器。PEG（解析表达式语法）与上下文无关语法（如当前的语法）的不同之处在
// 于，它的书写方式更紧密地反映了解析器在解析时的操作方式。根本的技术差异在于选择操作符
// 是有序的。这意味着当编写下面的规则时，一个上下文无关语法解析器（如 LL(1) 解析器）生
// 成的结构，给定一个输入字符串，其推断必须展开哪个备选方案（A、B 或 C），而 PEG 解析器
// 将检查第一个备选方案是否成功，只有失败时才会继续按照书写顺序检查第二个或第三个。这使
// 得选择操作符不满足交换律。
//      rule: A | B | C
//
// 与 LL(1) 解析器不同，基于 PEG 的解析器不会产生歧义：如果一个字符串可以解析，它恰好有
// 一个有效的解析树。这意味着基于 PEG 的解析器不会遭受上一节描述的歧义问题。PEG 解析器
// 通常被构造为递归下降解析器，其中语法中的每个规则对应于实现解析器的程序中的一个函数，
// 而解析种的表达式（该规则的展开或定义）代表的是该函数中的代码。每个解析函数在概念上接
// 受一个输入字符串作为参数，并产生以下结果之一：
//  1.  "成功"结果：此结果表明该表达式可以被该规则解析，函数可以选择性地向前移动或消耗
//      提供给它的输入字符串中的一个或多个字符。
//  2.  "失败"结果：在这种情况下，不消耗任何输入。
//
// 注意，"失败"结果并不意味着程序不正确或解析失败，因为由于选择运算符是有序的，"失败"结
// 果仅仅表示"尝试后续选项"。将 PEG 解析器直接实现为递归下降解析器，在最坏情况下会呈现
// 指数级时间性能，与 LL(1) 解析器相比，因为 PEG 解析器具有无限前瞻（这意味着它们可以在
// 决定规则之前考虑任意数量的 token）。通常，PEG 解析器通过一种称为 "packrat 解析" 的    *** https://pdos.csail.mit.edu/~baford/packrat/thesis/
// 技术避免这种指数级时间复杂度，该技术不仅将整个程序加载到内存中再进行解析，还允许解析
// 器任意回溯。这通过对每个位置已匹配的规则进行记忆化（memoization）来实现高效。记忆化
// 缓存的成本是解析器自然会比简单的基于表的 LL(1) 解析器使用更多内存。我们将在本文档后
// 面解释为什么我们认为这个成本是可以接受的。
//
// 扩展巴科斯范式（EBNF，Extended Backus-Naur Form）
//  1.  小写名称表示终结符
//  2.  首字母大写形式表示语法结构（非终结符）
//  3.  " " 或 ` ` 表示词法字面量
//  4.  { } 表示0次或多次
//  5.  [ ] 表示0次或1次
//  6.  ( ) 用来分组
//  7.   |  表示多种选择
//  8.   -  表示字符范围，例如 0-9
//  9.   ~  排除指定字符集合之外的任何字符，例如 ~ (b | B)
//  10.  =  定义语法结构
//  11.  .  表示语句的结束
//  12.  // 表示注释
//  13. < > 不能明确定义的规则说明
//  14. U+XXXX 单个统一编码字符
//  15. \r \n \0 \9 \xXX 单个转义字符
//  16. a z A Z 0 9 单个可打印字母和数字表示字符自己
//  17. ENDMARK 表示输入流结束
//  18. ANYCHAR 表示全体合法字符
//  19. NEWLINE 换行符
//
//  unicode_letter = Lu | Ll | Lt | Lm | Lo .
//  unicode_digit = Nd .
//  letter = unicode_letter | "_" .     // 下划线被当作小写字母
//  bin_digit = 0 … 1 .
//  dec_digit = 0 … 9 .
//  hex_digit = 0 … 9 | a … f | A … F .
//
//  词法元素
//  WHITESPACE = U+0020 | U+0009 | comment .
//  NEWLINE = U+000A | U+000D | U+000D U+000A | U+000A U+000D | <endmark> .
//  unicode_char = <unicode> - newline .  // 除换行外的其他任何字符码点
//
// 语言内置 EBNF 语法：
//
//      ``` bnf {
//      }
//
//  0.  标识符
//
//      源代码使用 UTF-8 编码，编码文本是没有规范化的，单个变音字符与对应的音调和字母是
//      不同的，后者被当作两个字符对待。为了与其他工具兼容，编译器不允许源代码中包含NUL
//      字符（U+0000），会忽略 UTF-8 的编码记号字节（U+FEFF），但它必须是源文件的第一
//      个字符，记号字节不允许在源文件的其他地方出现。
//
//      词法元素（token）组成语言的词汇表，分成四种类别：标识符、关键字、操作符和标点、
//      字面量。空白字符（whitespace），空格（U+0020，' '）、垂直制表（U+0009，'\t'）、
//      回车（U+000D，'\r'）、换行（U+000A，'\n'），除非它分隔了词法元素否则会被忽略。
//      另外，行注释和块注释会被解析成一个空格，一个换行符或者文件结束符或者输入流结束
//      都会被解析成代码换行。当将输入流拆解为词法元素时，会找下一个最长的合法字符序列。
//
//      程序由解析器（parser）读取，解析器的输入是一串词法元素（a stream of tokens），
//      词法元素由词法分析器（lexical analyzer）产生，本章将描述词法分析器是如何将文件
//      拆解为词法元素的。
//          1.  一个程序被分成多个逻辑行（logical line）
//          2.  一个逻辑行的结束通过词法元素 NEWLINE 表示
//          3.  一个逻辑行由一个或多个物理行组成（physical line），使用显式或隐式的行
//              合并规则（line joining rules），显式行合并即使用反斜杠分行
//          4.  一个物理行通过行结束符（end-of-line）结束，可以是 UNIX 形式的 \n 字
//              符，Windows 形式的 \r\n 双字符，或老式 Macintosh 形式的 \r 字符，另
//              外最后一个为物理行可以随输入流的结束而结束
//          5.  但是在程序的字符串类型中，仅使用 \n 表示换行
//          6.  逻辑行开头的前导空白字符（空格和制表符）用于计算该行的缩进级别，进而用
//              于确定语句的分组。在一个需要计算缩进的上下文中，逻辑行开头的前导空白要
//              么都是空格字符、要么都是制表符，不能混用，否则会引发 e_indent_error，
//              也不能包含其他空白字符，否则会引发 e_indent_invalid_char 错误。第一个
//              非空白字符前的空格总数或制表符总数决定了该行的缩进，当使用制表符时一个
//              制表符表示一个缩进，当使用空格时四个空格表示一个缩进，如果空格不足四的
//              倍数，会引发 e_indent_space_error 错误。
//          7.  连续行的缩进级别使用栈来生成 INDENT 和 DEDENT 标记，具体规则如下：
//              在读取文件第一行之前，单个零被压入栈中，这个零永远不会被弹出。压入栈中
//              的数字将始终从底到顶严格递增。在每个逻辑行开始时，该行的缩进级别与栈顶
//              进行比较。如果相等则不发生任何操作，如果更大则将其压入栈，并生成一个
//              INDENT 标记。如果更小则它必须是栈中出现的数字之一，栈中所有更大的数字
//              都被弹出，每弹出一个数字就生成一个 DEDENT 标记。在文件末尾，为栈中每个
//              大于零的剩余数字生成一个 DEDENT 标记。
//                  if expr     // 如果 if 开头的起始空格不是四的倍数，也会报错
//                  stmt_1      // 错误，没有缩进
//                   stmt_2     // 错误，缩进空格数量不够
//                      stmt_3  // 正确，第一级缩进
//                      s = l[:i] + l[i+1]
//                          p = perm(l[:i] + l[i+1:]) // 错误，不需要的缩进
//                      for x in p
//                              r.append(l[i:i+1] + x) // 错误，跳过了一个缩进级别（e_indent_level_skipped）
//                          r.append(l[i:i+1] + x)
//                  return r // 错误，没有对应的缩进级别可弹出
//          8.  缩进语句块可以写在同一行，允许 \t 前后出现任意空格，但整个语句块必须在同一物理行中
//                  if expr \n\t p = perm(l[:i] + l[i+1:]) \t for x in p \t\t r.append(l[i:i+1] + x) \t\t return r
//                  if expr { p = perm(l[:i] + l[i+1:]) for x in p { r.append(l[i:i+1] + x) return r } }
//
//      标识符（也称为名称）由以下词法定义描述。标识符的语法基于统一编码标准附件UAX-31，   *** https://www.unicode.org/reports/tr31/ Unicode Identifiers and Syntax
//      并进行以下说明和修改，更多细节参见 PEP 3131。在ASCII范围（U+0001 ~ U+007F）
//      内，标识符的有效字符包括大写和小写字母A到Z、下划线_，以及除首字符外的数字0到9。
//      对于引入的ASCII范围之外的额外字符，有效名称只能包含 “类字母（letter-like）”
//      和 “类数字（digit-like）” 字符。标识符长度无限制，大小写敏感。
//          name = name_start { name_continue } .
//          name_start = a … z | A … Z | _ | <non-ascii character> .
//          name_continue = name_start | 0 … 9 .
//          identifier = <name, except keywords>
//
//      包含非 ASCII 字符的名称需要额外的规范化和验证，超出上述规则和语法所解释的范围。
//      例如，ř_1、蛇 或 साँप 是有效的名称，但 r〰2、€ 或 🐍 则不是。本节解释确切的规
//      则。所有名称在解析时都会被转换为 NFKC 规范化形式。这意味着，如某些字符的排版变
//      体会被转换为它们的"基本"形式。例如，ﬁⁿₐˡᵢᶻₐᵗᵢᵒₙ 规范化为 finalization，因此将
//      它们视为相同的名称：
//          > ﬁⁿₐˡᵢᶻₐᵗᵢᵒₙ = 3
//          > finalization
//          3
//
//      注意，规范化仅在词法层面进行，运行时接受名称字符串作为参数的函数通常不会规范化
//      其参数。例如，上面定义的变量在运行时的 globals() 字典中可以通过 globals()["finalization"]
//      访问，但不能通过 globals()["ﬁⁿₐˡᵢᶻₐᵗᵢᵒₙ"] 访问。
//
//      类似于纯 ASCII 名称只能包含字母、数字和下划线，且不能以数字开头，有效的名称必须
//      以 "类字母" 集合 xid_start 中的字符开头，其余字符必须属于 "类字母和数字" 集合
//      xid_continue。这些集合基于统一标准附件 UAX-31 中定义的 XID_Start 和 XID_Continue
//      集合。Python 的 xid_start 额外包含下划线（_），请注意 Python 不一定完全符合
//      UAX-31。
//
//      统一编码字符数据库中的 DerivedCoreProperties.txt 文件提供了统一编码定义的 XID_Start
//      和 XID_Continue 集合中字符的非规范性列表。作为参考，下面给出了 xid_* 集合的构   *** https://www.unicode.org/Public/15.1.0/ucd/DerivedCoreProperties.txt
//      造规则。
//
//      id_start:
//          | <Lu>  // 大写字母，包括 A 到 Z（uppercase letters）
//          | <Ll>  // 小写字母，包括 a 到 z（lowercase letters）
//          | <Lt>  // 首字母大写（titlecase letters）
//          | <Lm>  // 修饰符字母（modifier letters）
//          | <Lo>  // 其他字母（other letters）
//          | <Nl>  // 字母数字（letter numbers）
//          | "_"                                                                  *** https://www.unicode.org/Public/16.0.0/ucd/PropList.txt
//          | <Other_ID_Start>  // 具有Other_ID_Start属性的字符，PropList.txt中支持向后兼容性的显式字符列表
//      xid_start:
//          <id_start中的所有字符，但移除所有 NFKC 规范化后不符合 "id_start xid_continue*" 形式的字符>
//      id_continue:
//          | id_start
//          | <Nd>  // 十进制数字，包括 0 到 9（decimal numbers）
//          | <Pc>  //  连接标点（connector punctuations）
//          | <Mn>  //  非间距标记（nonspacing marks）
//          | <Mc>  //  间距组合标记（spacing combining marks）
//          | <Other_ID_Continue> // 具有Other_ID_Continue属性的字符，PropList.txt中支持向后兼容性的显式字符列表
//      xid_continue:
//          <id_continue中的所有字符，但移除所有 NFKC 规范化后不符合 "id_continue*" 形式的字符>
//      identifier:
//          xid_start xid_continue*
//
//      所有标识符在解析时转换为NFKC规范化形式，标识符的比较基于NFKC。统一编码字符类别   *** https://docs.python.org/3/library/unicodedata.html#module-unicodedata
//      使用 unicodedata 模块中包含的统一编码字符数据库版本。另请参考 PEP 3131 支持非   *** https://peps.python.org/pep-3131/
//      ASCII 标识符，PEP 672 统一编码相关安全注意事项。                              *** https://peps.python.org/pep-0672/
//
//  1.  字符，要么是字节 byte，要么四字节 char
//      \e  退出
//      \d  删除
//      \b  退格
//      \s  空格
//      \t  制表
//      \r  回车
//      \n  换行
//      \'
//      \"
//      \0 ~ \9         数值 0 ~ 9
//      \x00 ~ \xff     数值 0x00 ~ 0xff
//      \{3000}         统一编码字符码点或名称
//      \{ABCDEF12}     统一编码字符码点或名称
//      \{name}         统一编码字符码点或名称
//      ' ' 空格        只能是 0x20 否则报错
//      'c'             整个 'c' 必须在同一行，必须只有三个字符，因此不能写 '\n' 而是直接用 \n
//      'type           类型转换操作前缀
//      ''              空字符，无效语法，报错
//
//  2.  字符串
//      ""  空字符串
//      "abcd\n"
//      "abcd\0"
//      `r"abcd\n"   原始字符串
//      `r8"END
//      原始多行字符串，END 不能非空，否则是单行字符串
//      "END
//      `8"END
//      可包含转义字符的多行字符串，多行字符串的内容不包含 `8"end "end 这两行
//      "END // 最大缩进为 8 个空格，最多去除行首的 8 个空格
//
//  3. 格式化打印
//
//      f-string 格式化字符串
//      "string here {variable here} here {variable here}"
//      variable = "awesome" // f-string 会立即产生字符串
//      print `f"string here {variable} here {variable}"
//
//      t-string 是一个模板对象，会保存需要打印的变量的信息
//      literal part: string here % here %
//      dynamic part: variable variable
//      在最终的字符串输出前，可以对动态部分的变量进行任何处理，例如 sanitize/escape validate transfor
//
//      let user_input = "<script>alert('yo')</script>"
//      let msg = `f"<p>{user_input}</p>"
//      let msg = 't"<p>{user_input | escape}</p>" // 可以做特殊字符转义
//
//  4. 不可恢复错误，无条件语句高效处理
//
//      错误发生时，通过栈展开传递错误，依次检查返回地址将错误传递给第一个感兴趣的主调
//      函数，stack_track_func[retp & 0x02](frame, error);
//      void stack_track_continue(frame *f, prh_error error);
//      void stack_track_error(frame *f, prh_error error);
//
//      主调函处理错误，然后继续传递旧错误或一个新错误
//      call_func() or { /* 处理错误 */ panic(error) }
//
//      错误分为两种，一种普通错误通过函数返回值传递，另一种是不可恢复错误，通过调用链
//      向上退出程序。
//
//  5.  实现标签数组跳转，实现标签传递给函数，让函数返回时跳转
//

act all are do use ago alt any auf aut can cat cor con cue des dhu din don dor
fac far fat fen fer fin fit fou fro fry fur gen gre lot off per pat pal phr par
pre pro rem res rim ron rou rut tie via was wow yet

def point "zeroinit packed" {
    float x
    float y
}

def data {
    int a
    int b
    (int a int b return int) f
    (int a int b return int) g
}

def get ($*T a return int) // 函数参数只能声明类型模板参数
def read ($*T a reg p int n return int) // 函数只有第一个参数才能是泛型类型
def reader $T $get(T) get $read(T) read {} // 在 ${} 表达式中需要省略 def 关键字

def (*$T a return int) get
def (*$T a reg p int n return int) read
def reader $(anytype T get(T) get read(T) read) {}

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
Handle (*HciRxDesc rx_desc r32 ca HciDataType type U line)
hci_rx_buffer_handled_and_free(HciRxDesc rxdesc r32 ca HciDataType datatype U line)
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
def color const i08 { RED = 1 BLUE = 2 YELLOW = 3 }
def bitvalue const int { FLAT_BIT1 = 1 << const FLAG_BIT2 FLAG_BIT3 }
def tcpaction const int { TCPA_OPEN_ACCEPT TCPA_TX_DATA TCPA_RX_DONE }
def 协程 { r32 rspoffset loweraddr }
def coroguard { r32 lower_guard_word def *coro coro (def *coroguard g int a b int) f g }
def handle(def *hcirxdesc rxdesc r32 ca def hcidatatype type def u line)
def main(int argc **byte argv int)
def scale(def point point int a b)
def calc(int a b int)

def array $(anytype t int size) {
    [size]t a
}

def array $(anytype a int size) { // $ 定义一个类型参数 a
    [size]a a
}

def test $(anytype a anytype b) {
    a a // 指定 a 是一个类型
    b b
}

def test $(anytype t anytype u) {
    t t
    u u
}

def color const int {
    red = const + 1
    blue
    green
    yellow
}

def test $(anytype T anytype U int SIZE array(SIZE, T) A point POINT) {
    T t
    U u
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

def node $(anytype t) {
    this next
    t data
}

def tripple $(anytype t anytype u int size) {
    [size]t a
    u b
}

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

def ::std/array // 同一个包名下的源代码，可以分割为多个部分，成员函数例如 push(*std::array *type a) 只能编写在 std 包的 array 源代码部分
import * std/array // 剥去包名，除了有冲突的名称仍然添加包名外，其他名称直接引入，std 包名只能存在于系统标准源代码中
import 3rd lib/array // 文件可提供一个包名，也可以不提供，如果没有提供包名，import 时可以定义一个包名
import std/array // 如果提供了包名，可以使用 def 定义符号别名
def <3rd::array> 3rd_array
def <std::array> std_array

Coro { // 公开函数会公开所有参数涉及的类型，公开类型的字段都是只读的，写操作必须通过公开函数
    r32 rspoffset // 1st field dont move
    r32 loweraddr
    r32 maxudsize 31 ptr_param 1
    i32 coro_id
    reg rspoffset
    reg loweraddr
    reg maxudsize 31 ptr_param 1
    int coro_id
    reg rspoffset
    reg loweraddr
    reg maxudsize 31 ptr_param 1
    int coro_id
    reg address
}

def coro {
    r32 rspoffset
    r32 loweraddr
    i32 {31} maxudsize {1} ptr_param
    i32 coro_id
    r32 rspoffset
    int maxudsize
    int coro_id
}

def coro_guard {
    r32 lower_guard_word
    r32 upper_guard_word
    coro embed
    *coro coro_ptr
    this embed
    this coro_guard
    (int a int b return int) calc
    (*coro) func
}

def 协程 {
    r32 rspoffset
    r32 loweraddr
}

def "std"

def coro_guard {
    r32 lower_guard_word
    r32 upper_guard_word
    coro embed
    *coro coro
    this embed
    this coro_guard
    (int a int b return int) calc
    (*coro p) func
    (*coro_guard int a b return int) f
    (*coro_guard int a b return int) g
}

def verify(*coro_guard)

CoroGuard { // 内嵌只能内嵌结构体类型，不能是指针
    r32 lower_guard_word
    r32 upper_guard_word
    Coro _embed_ // 不能内嵌两个相同类型
    *Coro coro_ptr
    this _embed_ // 错误，指针不能内嵌
    this coroguard
    (int a b int) calc
    (int a) print
    (this int a b int) a
    (CoroGuard g int a b int) f, g
    (Coro p) h
    { int a b }
    { int a b } tuple
    $i08 {RED {1} BLUE YELLOW} color
    $int {red {red_enum_value} blue yellow} color
}

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

Color $i08 "strict" { // strict 枚举类型必需为全部枚举手动指定值，并在代码更新时不能修改这些值，以防带来代码版本的不兼容
    RED {1}
    BLUE {2}
    YELLOW {3}
}

BitValue $int {
    FLAG_BIT1 {1 << const}
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
    r32 rxbuf_size
    r32 txbuf_size
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
    ass {'=', 200, 201} // a = 2 + b = 3
    add {'+', 211, 210}
    sub {'-', 211, 210}
    mul {'*', 221, 220}
    div {'/', 221, 220}
    pow {'^', 230, 231}
    dot {'.', 251, 250}
    end {0} // 默认值为零
}

def color const r08 { // private type
    RED GREEN BLUE
}

pub color const r08 { // public type
    RED
    GREEN = 1 << const
    BLUE
}

pub color const r08 "strict" { // strict 枚举类型必需为全部枚举手动指定值，并在代码更新时不能修改这些值，以防带来代码版本的不兼容
    RED = 1
    BLUE = 2
    YELLOW = 3
}

def point {
    float x
    float y
}

pub coro { // 包外访问，结构体成员只读，以下划线结束的成员不可访问
    r32 rspoffset // 名为 rspoffset 的私有成员
    r32 loweraddr // 名为 loweraddr 的私有成员
    i32 {31} maxudsize {1} ptrparam_
    i32 coro_id
}

// 定义类型别名，结构体和元组使用上面的方式定义，禁止使用该方法
def (int argc **char argv return int) func_type
def |flat_map|[string:int] type_of_map
def [int int float string] tuple_type
def [*int] int_ptr
def [*point] point_ptr
def [point] type_point

pub (int argc **char argv return int) func_type
pub |flat_map|[string:int] type_of_map
pub [int int float string] tuple_type
pub [*int] int_ptr
pub [*point] point_ptr
pub [point] type_point

def main(int argc **char argv return int) { // 相当于定义一个函数类型的常量，函数代码其实就是只读的代码数据，会放到只读分区
    return 0
}

def eat(*lexer l expr e return *oper) { // 编译器可以访问到完整代码的函数就是一个常量，而动态加载的函数相当于时一个函数变量（函数指针变量）
    return l.op or e.op
}

def color const r08 { // private type
    red green blue
}

pub color const r08 { // public type
    red
    green = 1 << const
    blue
}

def color const r08 {
    red = 1 green = 2 blue
}

def color const r08 {
    red = global.blue_defined_value green blue
}

def color const {
    red green blue
}

def color const int {
    red green blue
}

def test $(const C) {
    int data
}

def name (int a return int) { ... }
def (int a return int) func_type
def [int float] tuple_type
def name = 3.1415926
def name const { red blue green }
def name const int { red bule green }
def name const int with {r08 lpri r08 rpri} { ... }
def name const with {r08 lpri r08 rpri} { ... }
def name { int a int b }
def name $(anytype T anytype U const SIZE int N T VALUE) { ... }
def name $(anytype T) { ... }
def name $(anytype T anytype U) { ... }
def name $(int SIZE) { ... }
def name $(int SIZE anytype T anytype U) { ... }

def test $(anytype T anytype U const C int SIZE) {
    int data
    T t
}

def test $(int SIZE point POINT) {
    [SIZE]int a
}

def array $(anytype T int SIZE) SIZE > 0 {
    [SIZE]T a
}

def array $(anytype T const SIZE) (typeof SIZE == Integer && SIZE > 0) {
    [SIZE]T a
}

// 定义常量，常量没有地址，只有当赋值给变量时才真正保存到只读数据段（等号左边总是变量）, *** 逗号只能包含在括号内，或 let 与 = 之间 ***
// def name = expr
def SZ = 1024 // 类型为 const int
def PI = 3.1415926 // 类型为 const float
def 2P = 2 * PI // 类型为 const float
def PI = f64 3.1415926 // 类型为 const f64
def PT = point {100, 200} // 类型为 const point
def P3 = [_]int {100, 200} // 类型为 const [2]int
def P4 = |int int| {100, 200} // 类型为 const <int int>
def P5 = {int a int b} {100, 200} // 类型为 const {int a int b}
def P6 = (int a int b return int) { return a + b } // 相当于 def P6(int a b return int) { return a + b }

pub PI = 3.1415926
pub 2P = 2 * PI
pub PI = f64 3.1415926
pub PT = point {100, 200}
pub P2 = point {100, 200}
pub P3 = [_]int {100, 200}
pub P4 = [int int] {100, 200}
pub P5 = {int a int b} {100, 200}
pub P6 = (int a int b return int) { return a + b } // 相当于 pub P6(int a b return int) { return a + b }

// 定义全局变量，函数常量使用上面的方式定义，禁止使用该方法（等号左边总是变量）
// def var type name = expr
// def var type = expr
// def let name = expr
def var int a = 10
def var int b = 20
def var *int int_ptr = &a
def var *point point_ptr = &point
def var point point = {100, 200}
def var (int a int b return int) calc = { return a + b } // 定义一个函数指针变量，可以随时修改 calc
def var {int a int b point point} data = {10, 20, {100, 200}}
def var [int int point] data = {10, 20, {100, 200}}
def let tuple = {500, 6.4, 1}
def let integers = {1, 2, 3}
def let colors = {"红", "黄", "绿"}
def let set = {:1 :2 :3 :4 :5 :6}
def let map = {"a":1, "b":2, "c":3}
def let tup (a b c) = {500, 6.4, 1}
def let (a _) = read_tuple() // 赋值右边必须是一个元组类型
def let (_ a _ b) = data // 赋值右边必须是一个元组类型
def let (a b c) = [i32 f64 r08] {500, 6.4, 1}

pub var int a = 10
pub var int b = 20
pub var *int int_ptr = &a
pub var *point point_ptr = &point
pub var point point = {100, 200}
pub var (int a int b return int) calc = { return a + b } // 定义一个函数指针变量，可以随时修改 calc
pub var {int a int b point point} data = {10, 20, {100, 200}}
pub var [int int point] data = {10, 20, {100, 200}}
pub let tuple = {500, 6.4, 1}
pub let integers = {1, 2, 3}
pub let colors = {"红", "黄", "绿"}
pub let set = {:1 :2 :3 :4 :5 :6}
pub let map = {"a":1, "b":2, "c":3}
pub let tup (a b c) = {500, 6.4, 1}
pub let (a _) = read_tuple() // 赋值右边必须是一个元组类型
pub let (_ a _ b) = data // 赋值右边必须是一个元组类型
pub let (a b c) = [i32 f64 r08] {500, 6.4, 1}

// 定义局部变量，类型转换，考虑二元操作符当作一元操作符时的情况（- + * &）
//  1.  类型转换时，类型字面量不需要添加 'type 转换前缀
//  2.  named_type {initialize_list} 形式也不需要添加 'type 转换前缀
//  3.  named_type undefined 形式也不需要添加 'type 转换前缀
//  4.  符号 - 正号 + 可以正常使用，当出现分歧时，添加括号就行 (-3.14) (+10)
//  5.  取地址操作符 fer（fetch reference）
//  6.  解引用操作符 der（dereference）
//  var type name = expr
//  var type = expr // 定义与类型名同名的变量
//  let name = expr
var (int argc **char argv return int) main = { return 0 } //（等号左边总是变量）
var *int p = fer **int base + sizeof int
var *point p = der **point base + sizeof point
var point point = {100, 200} // 第一个 point 是类型
var point = {100, 200}
var *point = fer copyof point
var *point = fer {0}
var *int p = null
var *int q = undefined
var int a = 0
var int b = 0
var point o = undefined
var point pos = {1, 2}
var point = undefined
var point point = undefined
var point o = {1, 2}
var *ppb = malloc(size)
var *int p = undefined
var point a = {100, 200}
var [_]int a = {20, 30, 50}
var [8]int a = {1, 2, 3, 4}
var [i32 f64 r08] tup = {500, 6.4, 1} // tup[0] tup[1] tup[2]
var [i32 f64 r08] tup (a b c) = {500, 6.4, 1} // tup.a tup.b tup.c
var [i32 f64 r08] (a b c) = {500, 6.4, 1} // a b c
let tup (a b c) = {500, 6.4, 1} // tup.a tup.b tup.c
let data (value error) = read_tuple() // 元组类型值的返回 data[0] data[1] data.value data.error
let (a _) = read_tuple() // 赋值右边必须是一个元组类型
let (_ a _ b) = data // 赋值右边必须是一个元组类型
let (a b c) = [i32 f64 r08] {500, 6.4, 1}
let tup = {500, 6.4, 1}
let tup = [i32 f64 r08] {500, 6.4, 1}
let integers = {1, 2, 3}
let colors = {"红", "黄", "绿"} // 相同类型是数组，不同类型是元组，但两者都可以通过下标来访问
let set = {:1 :2 :3 :4 :5 :6}
let map = {"a":1, "b":2, "c":3}
let array_ints = {{1,2}, {3,4}, {5,6}} // 数组
let array_ints = {{1,2}, {3,4,5}} // 元组
let mixed_array = {{1,2}, {"a", "b", "c"}} // 元组
let int_array = mixed_array[0] // 3rd2.0 以数字开头的标识符，访问元组成员可能与浮点冲突
let str_array = mixed_array[1]
let o = der p
let p = fer a
let o = point {1, 2}
let ppb = *ppb malloc(size)
let p = *int null
let q = *int undefined
let a = 0
let b = byte 0
let ptr = alloc(1024) or panic()
let data = data {this, a = 1, 2, b = 3} // 元组类型变量定义 data.a data.b data[2]
let data = data {this, a = 1, b = 2, 3} // 可以实现对元组的修改 data.a = 10  data.b = 20
let a = int 0
let b = float 3.1415926 // 非大括号或undefined形式的类型转换，类型前加转换前缀
let calc = (int a b return int) { return a + b} // 类型字面量可以自动识别，不需要添加转换前缀
let a = point{100, 200}
let b = *int undefined // vsym + 大括号/undefined 都是类型的初始化，不需要添加转换前缀

// 局部变量的简化定义语法
if $u prh_lexer_next_utf8(l) (u == '\'' || u == prh_char_invalid)
    return TOKERR
if $c prh_lexer_next_char(l) (c != '\'')
    return TOKERR
l->c = prh_lexer_next_char(l)
l->u.cvalue = u
return CHARLIT
l->parse = prh_utf8_to_unicode(l->parse, fer $unicode);
return unicode;

def calc(int a int b return int int (x y)) {
    x = a + b
    y = a * b
}

def calc(int a int b return int int (x y) or error) {
    if a == 0 return e_invalid
    x = a * b
    y = e_notzero
}

def read_username(return string or error) { // 返回值的大小为 sizeof read_username_result，比 string 类型长一个字节，调用者必须检查错误码
    let f = open("username.txt") or return // 这里 or error 如果成立会直接返回 open 函数的错误码
    let s = string {}
    f.read_to_string(fer s) or return
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
// float 可以使用 N/A 值，int/reg 则必须手动指定，或使用 nonzero int，nonfini int，nonnull<T>
def divide(float a float b return float or none) { // 空值，有值，返回值的大小为 sizeof float，调用者必须检查 none 值
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

def calc(*file? file *expr expr return int) { // 如果加上了 none 属性表示值可能为空，必须要进行 none 检查
}

// Optional Type 的两个好处：
//  1.  编译器保证调用者必须检查可能为 none 的值
//  2.  编译器保证条件分支中 none 值不会传递赋给不能为空的变量或参数，参数默认都是不能为 none 的，除非明确指定为 noneable
// Optional Type 的一个坏处是：
//  1.  让代码变量繁琐，但是这是一种可选择性的有目的性的繁琐
//  2.  因为你可以有目的的选择在关注的代码上选用 or none
// 关于空值：
//  1.  默认不能将 null 传递给指针，除非它被显式声明为 *type?
//  2.  默认不能将 0 传递给 @nonzero 整数型变量
//  3.  默认不能将全一的值传递给 @nonalls 整数型变量
//  4.  空值是一个特殊的值，不应该在整个程序中泛滥传播
//  5.  or none 必须可以应用到任何类型，用来全面消除空值的泛滥传播
//  6.  @nonzero @nonalls 可以修饰结构体成员，使用这些成员必须经过 none 检查和传递性验证
//  7.  a where [x] { print(x) } or print("none") 增加新的语法保证简洁性和提供更高的安全性，原来的非空值只能通过if语句保证
//      新的语句将非空焊死在局部变量 x 中，print 根本访问不到可能为空的 a，因为函数闭包只能访问显式写在捕获参数中的值
//  8.  let x = a where [x] { x * 2 } or none // 变量 x 也将变成可空的值
//  9.  let x = a where [x] { x * 2 } or return + b or return // 表达式中可以在遇到 none 的地方直接返回空值
//  10. print(a where [x] { x * 2 } or -1)
//      print(a where it * 2 or -1)
//      a where it.print() or print("none")
//      a where it * 2 or none
//      a where it * 2 or return + b or return

def sqrt(float x float y return float or none) { // 调用者必须检查 none 值，不管通过 or 还是 if [a] none 等形式
    let a = divide(x, y) or return + divide(3, x) or return // 这里 or 如果成立会直接返回 none
    return sqrt(x * a)
}

def test const size(int) p(point) {
    [size]int a
}

def array $t const size(int) static size > 0 {
    [size]t a
}

def color const { // 默认是 byte 或 r16 或 r32 或 u64，根据最大值的大小而定
    RED GREEN BLUE
}

def color const int {
    RED = 1
    GREEN
    BLUE
}

def oper const r32 with {r08 lpri r08 rpri} { // sum type
    ASS {'=', 200, 201}
    ADD {'+', 211, 210}
    SUB {'-', 211, 210}
    MUL {'*', 221, 220}
    DIV {'/', 221, 220}
    POW {'^', 230, 231}
    DOT {'.', 251, 250}
    END {0} // 默认值为零
}

def read_username_result const {
    OK [string]
    ERR [unsigned]
}

def token const { // sum type
    ATOM {byte id}
    OPER {byte id}
    TEST [int int]
    EOF
}

let atom = token {.ATOM, 1}
let oper = token {.OPER, '+'}
let test = token {.TEST, 1, 2}
let test = token {.TEST, a = 1, b = 2}
let eof = token {.EOF}
def token atom = {.ATOM, 1}
def token oper = {.OPER, '+'}
def token test = {.TEST, 1, 2}
def token test = {.TEST, a = 1, b = 2}
def token eof = {.EOF}

// 泛型代码相当于在目标文件中不能生成具体代码，而是生成一个代码模板
def expr const byte { // 相当于是一种泛型类型
    VALUE {int n} // 相当于存储 {byte 0 int n}
    IDENT {int id} // 相当于存储 {byte 1 int n}
    TEST [int int]
    EXPR {int op *expr lhs *expr rhs} // 相当于存储 {byte 2 int op reg lhs rhs}
}

if [expr] .VALUE { // 必须穷尽所有情况，否则编译报错
    ret = expr.n
} if == .IDENT {
    ret = expr.id
} if == .TEST(a b) { // 捕获元组的内容
    ret = a + b
} if == .EXPR {
    ret = expr.op
}

if [expr] .value
    ret = expr.n
if == .ident
    ret = expr.id
if == .expr
    ret = expr.op
else
    return error

if expr == .IDENT {
    print("IDNET expr: %", expr.id)
}

if expr == .TEST {
    print("TEST expr: % %", expr.0, expr.1)
}

if expr == .TEST(_ a) { // 捕获元组的内容
    print("TEST expr: % %", expr.0, a)
}

if expr == .TEST(a b) {
    expr.a = 1
    print("TEST expr: % %", a, b)
}

def eat(*lexer, token) {
    return lexer.pop()
}

def peek(*lexer, token) {
    return lexer.top()
}

def eval(oper o expr l expr r return expr) {
    let expr = undefined
    if [o] '=' {
        expr = .value(r.value.n)
        get_symbol(l.ident.id).value = r.value.n
    } else if '+' {
        expr = .value(l.value.n + r.value.n)
    } else if '-' {
        expr = .value(l.value.n - r.value.n)
    } else if '*' {
        expr = .value(l.value.n * r.value.n)
    } else if '/' {
        expr = .value(l.value.n / r.value.n)
    } else if '^' {
        expr = .value(pow(l.value.n, r.value.n))
    } else {
        panic("bad operator %c", o)
    }
    return expr
}

eat(*lexer lexer return token) {
    return lexer.pop()
}

parse_expression(*lexer lexer int min_prior return expr) {
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

eat(*lexer lexer return token) {
    return lexer.pop()
}

parse_expression(*lexer lexer int min_prior return expr) {
    def expr = undefined
}

// 因为函数的第一个参数可以重载，因此 tcp_poll(file, sock, wait) 和 file.tcp_poll(sock, wait) 都同样有效
tcp_poll(*file file *socket socket *poll_table poll_table return poll) [m] alignas(16) {
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
        urg_data r16 READ_ONCE(tp.urg_data)
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

perform_tcpa_open_accept(*TcpSocket tcp r32 txbuf_size r32 rxbuf_size) {
    let pdata = *TcpAccept cono_malloc_pdata(TCPA_OPEN_ACCEPT, TCPQ_UPPER, true, sizeof TcpAccept)
    pdata.rxbuf_size = rxbuf_size
    pdata.txbuf_size = txbuf_size
    cono_freely_post(tcp.tcp_coro, pdata)
}

def report_tcpe_opened(*TcpSocket tcp) {
    let pdata = *TcpOpened tcpa_post_pdata(tcp, TCPE_OPNED, sizeof TcpOpened)
    let txbuf = *ByteArrfit fer tcp.txbuf
    pdata.tcp = tcp
    pdata.txbuf = arrfit_begin(txbuf)
    pdata.size = txbuf.size
    cono_freely_post(tcp.upp_coro, fer pdata->head)
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
    r32 rxbuf_size
    r32 txbuf_size
}

perform_tcpa_open_accept(*TcpSocket tcp r32 txbuf_size r32 rxbuf_size) {
    let pdata = cono_malloc_pdata(TCPA_OPEN_ACCEPT, TCPQ_UPPER, true, sizeof TcpAccept)
    pdata.rxbuf_size = rxbuf_size
    pdata.txbuf_size = txbuf_size
    cono_freely_post(tcp.tcp_coro, pdata)
}

report_tcpe_opened(*TcpSocket tcp) {
    let txbuf = fer tcp.txbuf
    let pdata = tcpa_post_pdata(tcp, TCPE_OPNED, sizeof TcpOpened)
    pdata.tcp = tcp
    pdata.txbuf = arrfit_begin(txbuf)
    pdata.size = txbuf.size
    cono_post(fer pdata->head)
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
    pos + der fer *I (*byte p + size + f(g))
}

def memcpy(reg dest unsigned src int count)
def memcpy(reg dest unsigned src int count) 'intrinsic'
def memcmp(reg dest unsigned src int count return int) 'intrinsic'
def memset(reg dest byte value int count return) 'intrinsic'
def lock_cmpxchg(*T p T old T new return T) 'intrinsic'
def coroguard(*coro p return coro_guard) 'cdcel inline'

Calc (int a b int)
Snode $T { this next T data }
for [&] i I 0 .. 9 {
    i int der *I addr
    if i%2 continue &
    pos + der fer I (*byte p + size + f(g))
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

push(trait a.reader)

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
data *{ int a b } fer {1, 2}
data [2]{ int a b } {{1, 2}, {3, 4}}
data Data {1, 2}
data *Data fer data
data Data[2] {data1, data2}

found .. index array_find(<<array, item)
found .. error array_find(<<array, item)

cal2 (int a b int) { return a + b }
cal2 (int a b int) { return a + b } (1, 2)
cal2 *(int a b int) (int a b int) {return a + b } // 函数不需要声明成指针，因为它本身就是指针
cal2 *(int a b int) Calc {return a + b }
cal2 [2](int a b int) {Calc {return a + b}, Calc { return a * b }}
cal2 Calc { return a + b }
cal2 *(int a b int) fer {return a + b }
cal2 [2](int a b int) {Calc {return a + b}, Calc { return a * b }}
cal2 *Calc fer (int a b int) {return a + b }
cal2 *Calc fer {return a + b }
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
let data = data {this, a = 1, 2, b = 3} // 元组类型变量定义 data.a data.b data[2]
let data = read_tuple(return _, a) // 元组类型值的返回 data[1] data.a
let integers = {1, 2, 3}, colors = {"红", "黄", "绿"}
let array_ints = {{1,2}, {3,4,5}}
let mixed_array = {{1,2}, {"a", "b", "c"}}
let int_array = mixed_array[0] // 3rd2.0 以数字开头的标识符，访问元组成员可能与浮点冲突
let str_array = mixed_array[1]
let a = 'int 0, b = 'float 3.1415926 // 非大括号或undefined形式的类型转换，类型前加转换前缀
let calc = (int a b return int) { return a + b} // 类型字面量可以自动识别，不需要添加转换前缀
let a = point{100, 200}, b = *int undefined // vsym + 大括号/undefined 都是类型的初始化，不需要添加转换前缀
let a = int{0}, b = float{3.1415926}
let a = 't' // 字符 t
let a = 't {a + b} // 类型转换成类型 t
let a = '' // 空字符，非法
let a = ''abcd'' // 将多字符当作整数使用，合法

dat2 Data {3, 4}
data (r32 bool) parse_hex_number(slice(hex, it*2, 2))
data.r32
data.bool
data {r32 a bool b} parse_hex_number(slice(hex, it*2, 2))
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

dat3 *{ int a b } fer {3, 4}
dat3 [2]{ int a b } {{3, 4}, data}
dat3 *{ int a b } fer {3, 4}
dat3 *Data fer data
dat3 [2]Data {{3, 4}, data}

cal3 *(int a b int) fer { return a + b }
cal3 *(int a b int) fer calc
cal3 (int a b int) { return a + b }
cal3 Calc { return a + b }
let cal3 calc

// 基本类型和指针不通过大括号进行初始化而是直接给值，根据语法定义一个变量的同时必须进行初始化
// 一个非类型标识符后跟一个类型表示定义该类型的一个变量
// 一个非类型标识符后跟一个非类型标识符，表示用后面的非类型标识符的值定义一个变量
// 一个非类型标识符后跟一个字面常量，表示用字面常量定义一个变量

cal3 *(int a b int) null
cal3 *(int a b int) fer { a + b }
cal3 *(int a b int) calc

numb errno null
numb float 3.14
numb *int null
numb *int fer data
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
numb *int fer data
calc Calc { a + b }
data Data {1, 2}
data int 1024
numb errno null
numb float 3.14
numb *int fer data
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
let len = int pos + fer *byte p + size + f(g)
let len = int pos + der *byte (p + size + f(g))
let len = typeof(pos) 3

pos int dist + int scale_x(facter)
len int pos + der *int *byte (p + size + f(g))
len typeof(pos) 3

for i int 3 .. 10 { /* */ }

for {
    capacity *= 2
} ~ if capacity < new_capacity

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
assert(sizeof int == sizeof reg)
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
    struct {
        string color shape;
        struct {
            int height width;
            struct {
                string a b;
            } desc;
            string info;
            string desc;
        } size;
    } physical;
    { string name; int id } [@arrlit] varieties
    { bool sweet } taste @optional
    { bool smooth } texture
}

[apple] fruit // 以另一个[var Type]结束，明确是字面常量赋值不需要使用=等号，否则需要使用调用语法进行初始化
physical color "红" size height 23 width 24 desc a "a" b "b" ||| size info "i"
physical shape "round"
taste sweet true
texture smooth true
rates [80, 75, 95]

[apple] fruit // 定义 apple
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

[apple] fruit // 使用顶格和 ||| 控制缩进，0~3个空格是顶格字段，>=4个空格或>=1个Tab才是子字段
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

[apple] fruit
physical color "红"
    size height 23 width 24 desc a "a" b "b" |||
    size info "i" desc "d"
physical
    shape "round"
taste sweet true
texture smooth true

[apple] fruit
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

[apple] fruit
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

[apple] fruit
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

[global]
a true
b 1024
s "hello"

[apple] fruit
fruit_type name "pug"

[fruits] []fruit
name "apple"
physical color "red" shape "round"

[fruits] []fruit
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

[fruits] []fruit
name "banana"
void

import "src/lang/math" // 加载静态代码
time:Time
math:*

优先级（从高到低） ::

    12 从左到右    a:b 名字空间由代码包和文件内代码分块表示，代码分块的表示形如 :::time::: 代码包由一个文件夹组成
    11 从左到右    a() a[] a.b a->b 函数调用，数组下标，成员访问
    10 从右到左    -a +a ^a !a type a fer a der a sizeof a typeof a ->> <<-  not neg int fer der *int [2]int
     9 从左到右    a.&b a->&b 返回成员地址，相当于(&)a.b
     8 从左到右    a*b a/b a%b a&b a<<b a>>b a<<<b a>>>b  mul_op   --> <-- &^
     7 从左到右    a+b a-b a|b a^b             add_op   |^
     6 从左到右    a<b a>b a<=b a>=b           rel_op
     5 从左到右    a==b a!=b
     4 从左到右    a&&b
     3 从左到右    a||b
     2 从左到右    a?:b  ?<  ?=  ?>
     1 从右到左    a=b a+=b a-=b a*=b a/=b a%=b a<<=b a>>=b a<<<=b a>>>=b a&=b a^=b a|=b
     0 从左到右    a,b

    -- ++ .. ** %% ?? :: ==
    ... --*-- --.-- ## @@ $$

    交换操作
        a <=> b
    一元操作符 ~ 后跟 if 有特殊含义
        for { ... } ~ if expr
    以下两种形式的变量初始化，symb 一定是一个类型名称：
        symb { initializer }
        symb undefined

     逻辑操作符 || && 混用时，强制要求添加括号；
     所有的赋值语句如果不是单独成一个语句，而是出现在语句中，强制要求添加括号；

为了省略大部分的分号并避免歧义，一元操作符使用特殊的语法形式： ::

    小括号包含类型用来定义类型或用作类型转换操作符，小括号包含值表示表达式的一部分。
    大括号只能包含值或由值组成的语句列表，值由变量常量操作符组成。
    取地址 & 改为 (&) 地址标记 &1 &2 fer
    解引用 * 改为 (*) (**) (*&) (**&) 地址引用 *&1 *&2 der

    @negt()     @-          @-3.14      @-c         (-3.14) (-c)
    @posi()     @+          @+6.24      @+c         (+6.24) (+c)
    @comp()     @^          @^1024      @^c         (^1024) (^c)
    @fer()     (&)         @&data                  (&)data (*&)data    fer data    der fer data
    @der()     (*)         @*p         @**pptr     (*)p    (**&)ptr calc(-3.14, +6.28, ^c, &data, *p, **&ptr) 前面必须有分隔符，包括左括号（( [ {），逗号（,），或（@）

// 结构体中的各类成员
//  1. 成员 type field_name field_name
//  2. 位域 type {bits} name {bits} name ...
//  3. 成员别名 type (a | b | c | ...)
//  4. 在大的成员类型内部定义小的联合类型 type name { | type name | type name type name ... | ...}

def test {
    int a int b int c
    int {MASK_BITS} inplace {INT_BITS - MASK_BITS} size // 位域，位域总是无符号类型，即使使用 int 声明，它都是一个无符号类型
    int {1} inplace {31} size // 位域
    int (size | bytes | count) // 成员别名
    double d { // 最大类型必须是第一个
        | int i
        | float f float g
        | byte b r32 u
        | byte b r32 u
        | char c
    }
}

// 条件语句包含传统C的if和switch：
//  if cond { expr }
//  if cond return expr
//  if cond { expr } else if_stm
//  static if cond { expr } else { expr }
//  static if cond { expr } else if cond { expr }
//
// 逗号操作符，一路否定、两路、三路操作符
//  let {expr, expr, epxr}
//  expr or expr
//
//  let [boolexpr] statement else statement
//  let [a] let [b] stmt else stmt else stmt
//  let [a] stmt else let [b] stmt else stmt
//
//  let [a <> b] ?< statement ?= statement ?> statement
//  let [a] ?< stmt ?= stmt ?> stmt
//  let [a] ?< let [b] ?< stmt ?= stmt ?> stmt ?= stmt ?> stmt
//  let [a] ?< stmt ?= let [b] ?< stmt ?= stmt ?> stmt ?> stmt
//
//  0. 以下then是以下关键字之一：
//      goto
//      break
//      final
//      continue
//      return
//      yield
//
//  在 then 所代表的语法位置，只能出现上面的关键字，或者
//      在同一行的单条语句
//      开始大括号 {
//      一个物理换行 <\n>
//      任意个数标签标识符的定义
//
//  1.  一般条件语句
//  if expr { stmt }
//  if expr { stmt } else { stmt }
//  if expr { stmt } else if expr { stmt } else { stmt }
//
//  2.  简单条件语句
//  if expr then statement
//  if expr then statement else then statement
//  if expr then statement
//  else if expr then statement
//  else then statement
//
//  if expr statement
//  if expr statement else statement
//  if expr statement
//  else if expr statement
//  else statement
//
//  3.  缩进条件语句
//  if expr <\n> // 如果 expr 要换行，请用操作符作为行的结束
//      stmt
//      ...
//  if expr <\n>
//      stmt
//      ...
//  else <\n>
//      stmt
//      ...
//  if expr <\n>
//      stmt
//      ...
//  else if expr <\n>
//      stmt
//      ...
//  else <\n>
//      stmt
//      ...
//
//  4.  标签条件语句，if == item 相当于定义了一个标签，可以用在任何标签可以使用的地方
//  if [expr] == item { statement }
//  if == item { statement }
//  if == item { statement }
//  if == else { statement }
//
//  if [expr] == item then statement
//  if == item then statement
//  if == item then statement
//  if == else then statement
//
//  if [expr] == item statement
//  if == item statement
//  if == item statement
//  if == else statement
//
//  if [expr] == item <\n>
//      statement
//      ...
//  if == item <\n>
//      statement
//      ...
//  if == item <\n>
//      statement
//      ...
//  if == else <\n>
//      statement
//      ...
//  void // 可选，仅标记语句块结束
//
//  if [expr] == item {
//      ...
//  } if == item {
//      ...
//  } if == else { // 不简单使用 else 的原因是，当作为标签用作其他条件语句块中时，可能意外终止最近的一个if
//      ...
//  }
//
//  if == .value
//      if l.escape_code == false
//          statement
//  if == else
//          string_push(s, c)
//          final
//
//  if == .value
//      if l.escape_code == false
//          statement
//  else // 导致语法错误
//          string_push(s, c)
//          final
//
//  5.  静态条件编译
//  static if expr { stmt }
//  static if expr { stmt } static else { stmt }
//  static if expr { stmt } static elif expr { stmt } static else { stmt }
//
//  static if expr statement
//  static if expr statement static else statement
//  static if expr statement
//  static elif expr statement
//  static else statement
//
//  static if expr <\n>
//      statement
//      ...
//  static elif expr <\n>
//      statement
//      ...
//  static else <\n>
//      statement
//      ...
//
//  static if [expr] item <\n>
//      statement
//      ...
//  static if == item <\n>
//      statement
//      ...
//  static else <\n>
//      statement
//      ...
//
//  6. 条件表达式
//  let a = let if expr { stmt }
//  let a = let if expr { stmt } else { stmt }
//  let a = let if expr { stmt } else if expr { stmt } else { stmt }
//
//  let a = let if expr statement // let 条件表达式不能使用 then 系列关键字，包括在语句内部
//  let a = let if expr statement else statement
//  let a = let if expr statement
//      else if expr statement
//      else statement
//
//  let a = let if expr <\n>
//      statement
//      ...
//  else if expr <\n>
//      statement
//      ...
//  else <\n>
//      statement
//      ...
//
//  7. 两值逻辑（true 和 false，或任意两个值，包括两个值的枚举值）
//  if [boolexpr] == true then statement
//  if == else then statement
//
//  if [boolexpr] == true then statement
//  if == false then statement
//  if == else then statement // 报错
//
//  if [boolexpr] == true { statement } if == else { statement }
//  if [boolexpr] == true then statement if == else then statement
//  if [boolexpr] == true statement if == else statement
//
//  if [boolexpr] == true <\n>
//      statement
//      ...
//  if == else <\n>
//      statement
//      ...
//  void
//
//  let [boolexpr] statement else statement
//
//  8. 三值逻辑（小于、等于、大于，或任意三个值，包括三个值的枚举值）
//  if [a <> b] ?< statement // 该分支 this 值小于零
//  if ?= statement // 该分支 this 值为零
//  if ?> return value + this // 该分支 this 值大于零
//
//  if [a <> b] ?> statement
//  if == else statement
//
//  let [a <> b] ?< statement ?= statement ?> statement
//  let [a <> b] ?= statement else statement
//
//  ?< ?= ?> 可以看作是预定义的三个枚举常量值：
//  const i08 {
//      ?<  =   -1
//      ?=  =   0
//      ?>  =   1
//  }
//

if [impl_bstr[impl_b256[c]]] == .dquote
    goto finish
if == .bslash
    l.escape_code = false
    if !lexer_escape(l) {
if == .invalid { return TOK_ERROR }
    }
    c = l.cvalue
    if l.escape_code == false
if == else {
        string_push(s, c)
        final
    }
    let n = unicode_to_utf8(c, string_end(s))
    if n == 0 return TOK_ERROR
    string_increase_size(s, n)
    final
void

if [impl_bstr[impl_b256[c]]] == .dquote
    goto finish
if == .bslash
    l.escape_code = false
    if !lexer_escape(l) {
if == .invalid return TOK_ERROR
    }
    c = l.cvalue
    if l.escape_code == false {
if == else {
        string_push(s, c)
        final
    }} // 这里任何一个结束大括号都不能往后移，因为单条件块 final break continue return 之后不能再有语句
    let n = unicode_to_utf8(c, string_end(s))
    if n == 0 return TOK_ERROR
    string_increase_size(s, n)
    final
void

if [impl_bstr[impl_b256[c]]] == .dquote {
    goto finish
} if == .bslash {
    l.escape_code = false
    if !lexer_escape(l) {
if == .invalid
        return TOK_ERROR
    }
    c = l.cvalue
    if l.escape_code == false {
if == else {
        string_push(s, c)
        final
    }}
    let n = unicode_to_utf8(c, string_end(s))
    if n == 0 return TOK_ERROR
    string_increase_size(s, n)
    final
}

def lexer_dquote(prh_lexer *l) {
    let s = l.svalue
    string_clear(s)
    for {
        let c = lexer_next_char(l)
        if [impl_bstr[impl_b256[c]]] == .dquote
            goto label_finish
        if == .bslash
            l.escape_code = false
            if !lexer_escape(l)
        if == .invalid #label_failed #something_wrong
                return TOK_ERROR
            c = l.cvalue
            if l.escape_code == false
        if == else
                string_push(s, c) // 因为有没有结束的if分支，且 if == 仅仅是一个标签，缩进以未结束的if为准
                final
            let n = unicode_to_utf8(c, string_end(s))
            if n == 0 return TOK_ERROR
            string_increase_size(s, n)
            final
        void
    }
#label_finish
    l.c = lexer_next_char(l)
    l.svalue.size = s.data - l.svalue.data
    return TOK_STRING
}

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
elif blue, yellow
    goto &
elif green
    void
else &
    void

if [color] .RED { // 使用break会跳出外层for循环
    goto .GREEN
} elif .BLUE .YELLOW {
    goto &
} elif .GREEN {

} else & {

}

defer_return @label
    return

// 循环语句

for expr { stmt ... }
for expr then stmt
for { stmt ... }
for { stmt ... } ~ if expr

// 函数支持默认参数，但不支持函数名重载，但支持第一个参数重载，但支持操作符重载+ - * / == != << >> & | [] % ^ <<< >>> []= .&，symmetric
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
//      port->action.head.opcode[PRH_TCPA_INDEX_..] = PRH_TCPA_#(OPEN_ACCEPT)
//      port->action.head.opcode[PRH_TCPA_INDEX_..] = PRH_TCPA_#(OPEN_REQ)
//      port->action.head.opcode[PRH_TCPA_INDEX_..] = PRH_TCPA_#(TX_END)
//      port->action.head.opcode[PRH_TCPA_INDEX_..] = PRH_TCPA_#(FINISH)
//
//  3.  scoped 语句块
//
//      def enter_read(*rwlock lock) scoped leave_read(lock) {
//          ...
//      }
//
//      scoped enter_read(rwlock) {
//          ... // 退出当前作用域后，会自动调用 leave_read(lock)
//      }
//
//  4.  函数参数的传递，函数参数可以设置对齐限制，编译器可以检查类型的对齐属性看是否满足要求
//
//      只有小于等于两个字长的命名类型才可以传值，其他都只能传指针，传指针的变量如果不想
//      修改其自身，可以使用语法 test(&copyof a)
//
//      基本类型 int reg arch_int arch_reg def ptr float 和枚举类型，可以显式传值或指针，传值(1)表示不修改，传指针表示修改，传指针需要声明为 *int
//      结构体类型总是传指针表示修改，声明为 *point，test(fer point) test(point_ptr)，即使是双字长的结构体也只传一个指针，因为需要修改成员，传递一个成员指针和两个成员指针区别不大
//      如果不需要修改结构体，需要声明为 *imm point，不同的是小于等于双字长的结构体直接传递结构体内容（2），大于双字长的将内容拷贝到栈并传递地址
//      情况(1)在函数中变为传指针，可能（通过寄存器而不是通过栈传递的情况下）需要将寄存器中的值重新复制到栈中
//      结构体类型总是传指针，函数参数只允许 def *type_name 语法，如果不想修改提前复制一份副本，或通过 copyof 修改副本，如果函数本身不进行修改则无所谓
//      如果结构体声明为 def type_name @as int { }，将结构体当作基本类型使用，则可以显式传值或指针
//
//      def test(int a float b *int c **point p)
//      def test(*int p) // 不能修改 <<p，整数不能修改
//      def test(*point p) // 不能修改 <<p，结构体 point 不能修改
//      def test(**point p) // 不能修改 <<p，也不能修改 <<*p，结构体 point 不能修改，指针 *point 也不能修改
//      def test(*mut int p) // 可以修改 <<p
//      def test(*mut point p) // 可以修改 <<p
//      def test(*mut *point p) // 指针 *point 可以修改，结构体 point 不可修改
//      def test(*mut *mut point p) // 指针 *point 和结构体 point 都可以修改
//
//      不可修改指针不可以传给可修改指针；
//      复杂度很大，正确实现的难度很大，去掉不可修改和可修改指针的设计，只要传指针就认为会修改。
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
—— 坐 作 分 厘 毫 微 纳 皮 飞 阿 擒 点 幂 阿 波 次 得 衣 夫
—— 零 壹 贰 叁 肆 伍 陆 柒 捌 玖 拾 佰 仟 萬 億
—— 洞 幺 两 三 四 五 六 拐 八 勾
—— 〇 一 二 三 四 五 六 七 八 九 十 百 千 万 亿 兆（万亿）京（亿亿）10^4+4+4+4
—— 千 兆 吉 太 拍 艾 泽 尧 10^3+3+3+3+3+3+3+3  k M G T P E Z Y
—— 毫 微 纳 皮 飞 阿 仄 幺 10^-3-3-3-3-3-3-3-3 m u n p f a z y
—— 分米 dm 厘米 cm 毫米 mm 微米 um 纳米 nm 皮米 pm 飞米 fm 阿米 am 仄米 zm 幺米 ym
—— 十米dam 百米 hm 千米 km 兆米 Mm 吉米 Gm 太米 Tm 拍米 Pm 艾米 Em 泽米 Zm 尧米 Ym
—— ≠ ≤ ≥ √ × ± Ω Π ▲ ← → ↑ ↓ ÷ ×
—— ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩
—— ·、，。：；？！￥‘’“”（）【】《》
—— — — …… … … ··
—— +-*=~&|%#@{}
—— 1234567890 3f14159p10 +3f14p-10 -0f1 数曰三点一四一五九幂十二 数曰一〇〇〇万八零零零 数曰负零点一
——
—— 12 从左到右    a:b 名字空间由代码包和文件内代码分块表示，代码分块的表示形如 :::time::: 代码包由一个文件夹组成
——     11 从左到右    a() a[] a.b a->b 函数调用，数组下标，成员访问
——     10 从右到左    -a +a ^a !a type a fer a der a sizeof a typeof a ->> <<-  not neg int fer der *int [2]int
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
—— scoped
—— fer      取址
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
—— r08 r16 r32 r64 r128 r256 r512 reg arch_reg
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
