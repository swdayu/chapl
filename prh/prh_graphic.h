#include "prh_include.h"

// 字体（Font），验证等宽字体效果，真正等宽字体下面三行会完美对齐：
// | 中文 | 中文 | 中文 |
// | abcd | abcd | abcd |
// | 中ab | 中ab | 中ab |
//
// 字体风格，衬线体（Serif）和无衬线体（Sans-Serif），Serif 源自荷兰语 Schreef 笔画、
// 线条，Sans 源自法语（without）。核心区别在于笔画末端是否有装饰性的"小脚"。衬线体（Serif）
// 或罗马体，在笔画末端有额外的装饰性笔触（衬线），像"小脚" 或"小横杠"。例如常见的西文字体
// Times New Roman、中文宋体（SimSun）、思源宋体。起源：模仿古罗马石刻字母的凿刻痕迹；用
// 途：长文阅读（书籍、报纸、论文）；优势：衬线引导视线水平移动，提高阅读流畅性，笔画对比
// 度强，字内结构清晰（阅读疲劳低，视线引导好）。
//
// 等宽（Monospace）和变宽（Proportional）。等宽字体也可以是衬线体风格，模仿打字机正式感，
// 意营造老式打字机氛围。传统打字机打出的并不是真正的衬线体，但视觉效果上确实有类似之处。
// 打字机"伪衬线"的真正来源，一是机械结构的限制，早期打字机的字模是铸造在金属杆上的，字母
// 主体与字臂连接处自然形成粗重的横线，看起来像衬线，但其实是机械连接的残余。二是等宽需求
// 的副作用，为了机械对齐，所有字母必须占据相同的物理宽度，窄字母（i, l）必须加粗/加宽填
// 充空间，在末端形成"横杠"来占满格子。三是击打清晰度的需求，底部横杠增加了着墨面积，防止
// 细笔在纸张纤维上断裂/模糊，有横杠的粗笔更清晰。为什么编程字体多为等宽无衬线？因为衬线
// 在小字号（10-14px）下难以清晰渲染，Sans 不干扰代码逻辑的视觉判断，代码需要严格的列对
// 齐缩进，终端早期只能显示无衬线点阵字体。Courier（1955年，Howard Kettler 设计）是刻意
// 模仿打字机外观的字体，Courier New 最知名的等宽衬线体，模仿打字机，而 Consolas 彻底摒
// 弃了打字机传统的现代等宽无衬线设计。
//
// 无衬线体或黑体或哥特体（Sans-Serif），笔画末端没有任何装饰，干净利落，粗细均匀。例如
// 常见的西文字体 Arial、中文黑体（SimHei）、微软雅黑、思源黑体。起源：19 世纪工业革命后，
// 为广告牌、海报等需要远距离识别的场景设计；用途：屏幕显示、UI 界面、标题、短文本；优势：
// 简洁现代；低分辨率下仍清晰可读，中性客观，不干扰内容（像素渲染清晰，现代感强）。
//
// 中文的"宋体"虽然原理上属于 Serif，但其"顿笔"是书法性的笔锋，而非西文那种几何化的衬线，
// 这是活字印刷模仿毛笔书法的结果。"黑体"笔画粗细均匀，末端无装饰。当前电子内容几乎清一
// 色地都是黑体，而纸质内容绝大部分采用宋体。在大部分电子产品环境下，黑体确实更容易辨认，
// 但这里有一个重要的前提，只以字体为唯一变量。黑体的特征是笔画基本粗细一致，宋体则是横
// 细竖粗，如果屏幕分辨率低，横画就看起来很糊，笔画看不清。有研究实验测量了黑、宋、楷、
// 隶四种字体的辨认阈值，也就是字缩到多小还能认出来。结果发现，可辨认度由高到低依次是黑、
// 宋、楷、隶，黑体赢得干净利落。要是单论认字这件事，黑体的优势是真实存在的，绝非玄学。
//
// 但为什么有的人觉得黑体字难度？但问题是，我们日常做的任何任务可不是认字，而是阅读。这
// 两件事有本质上的不同。阅读时，大脑是一块一块阅读的，不是一个字一个字读。实际上，眼球
// 在阅读时做的是一连串短促的跳跃和停顿。每次停顿约持续 200 至 250 毫秒，而每次停顿时，
// 眼睛在中央视野里能看清细节的范围其实很小，只有 2 到 3 个汉字，但视野周边区域也在同步
// 提取周围的信息，为下一次跳跃做预判。可辨认度指的是字体能不能被识别，而可读性指的是连
// 续读一大段文字时，整体是否流畅、省力、不容易疲劳。一个字体的可辨认度高，排出来的文章
// 不一定号读。例如行距太窄，字间距过松，每行字数太多，这些排版因素（行间距、字间距、每
// 行字数）的影响权重，在大多数情况下都高于字体是黑体还是宋体。另外，电子阅读材料往往还
// 有一个纯技术性的问题：没有做自适应。很多原本是网页或电脑端的内容，在手机上打开并没有
// 做对应的排版适应。
//
// 另外字号的影响远大于字体。一些学者研究了多种显式变量对中文阅读的影响，结论是字体类型、
// 字号大小和间距都会影响阅读速度和主观偏好，但只有字号大小影响了阅读理解率。也就是说，
// 字体风格影响了读起来有没有感觉，但字号大小决定了都进去多少。在手机场景下的研究也得出
// 过相似结论：字号更大的中文字，识别速度显著更快，错误率更低，而字体风格的影响，在同等
// 字号下反而没有字号本身的影响显著。所以，很多人以为是字体让自己看得难受，其实很可能只
// 是字体太小了。只要字体大到视觉能阅读，用什么字体都行。有人认为宋体更好，因为宋体是衬
// 线体，在英文文本阅读研究里，衬线体阅读体验更好。但问题是，英文的字形于中文并不相同，
// 不可直接套用。英文里，衬线就像字母之间隐含的一条基准线，字母衬线就像半隐形的各自。但
// 中文的衬线是在字的顿笔、折笔处形成的，而且每个字都是独立的方块，阅读单元完全不同，这
// 就导致英文中衬线的作用在中文里基本失效。其实，同是黑体或宋体，在字重、笔画粗细比例等
// 细节上，差异也很大，不同黑体字体之间的可辨认度差异，不一定比黑体和宋体之间的差异小。
// 要想知道选什么字体，首先要问什么场景，以及阅读舒适度，还有主观因素。例如手机字幕需要
// 在 0.5 秒内被瞥见、路牌需要在车速 60km/h 下被识别，这些场景的确需要最高可辨认度，黑
// 体的优势在这里最能发挥。而在大段连续阅读的场景下，排版和字号是更关键的变量，字体是次
// 要的。另外，一项研究探索了字体熟悉程度对阅读速度的影响，发现接触时间对阅读速度又显著
// 影响，也就是说，你对一款字体越熟悉，读得越快，这与字形本身的设计优劣关系不大。这就形
// 成了一个自我强化的循环，熟悉了黑体就会觉得黑体阅读感知更自然、更好读，有一些人习惯阅
// 读宋体，就会觉得宋体更舒服。
//
// 不同场景下排版和字号的推荐参考值，字间距正文设为 0，中文方块字内部已有标准间距，额外
// 加字间距反而破坏词组的视觉连贯性，不利于分块识别。如果你用手机读文章经常感觉眼睛累，
// 先看看自己屏幕是不是太亮，周围是不是太暗，再把字号调大，把行距拉宽，最后再去纠结换不
// 换字体，按照这个优先级来，解决问题的效率会高很多。
//  1.  电子书、长文本阅读器：字号 17 至 20px 之间，行距 1.6 至 1.8 倍，每行 28 至 35
//      字是中文长文本的舒适范围。这里需要注意的是，对比度是容易被忽略的一个变量，纯黑
//      文字配纯白背景在高亮度屏幕下，实际上对比过强，长时间阅读容易疲劳，很多阅读器默
//      认的米黄色背景就很好，可以降低对比度峰值。
//  2.  手机端：正文字号最小 15px，推荐 16px，主流中文应用的正文字号也通常集中在 15 至
//      17px 之间，行距推荐 1.6 至 2.0 倍，在碎片化的手机阅读场景下，稍宽的行距有助于维
//      持节奏，减少视线落错行的概率。这种设置下，每行字数约为 20 至 25 字，基本落在舒
//      适范围内。
//  3.  网页桌面端：正文字号 16px 起步，17 至 18px 更舒适，行距推荐 1.5 至 1.8 倍，每
//      行字数 30 至 40 字是舒适范围。每行字数越多，需要越宽的行距来帮助视线准备回到下
//      一行起点，否则很容易重读同一行。
//
// 字体的 Unhinted 版本，是指未添加字体提示（hints）的版本。字体提示是一组指令或数据，
// 告诉渲染引擎（操作系统、浏览器）如何在低分辨率（尤其是屏幕显示）下调整字体的轮廓，使
// 其清晰可读。TrueType Hinting 使用指令语言，可编程控制每个字号的渲染行为，No hinting
// (Unhinted) 完全依赖渲染引擎的抗锯齿和子像素渲染。去掉 hinting 数据，字体文件体积可
// 减少 20-50%，在高 DPI（200+ PPI）屏幕上，矢量轮廓本身的精度已足够，hinting 的强制对
// 齐反而可能造成字形扭曲，无需为每个字号单独调整 hinting 指令，避免不同渲染引擎（Windows
// ClearType / macOS CoreText / FreeType）对 hinting 解释的差异。但主要的问题是，低分
// 辨率发虚：在 96 DPI 的传统显示器上，小字号（<16px）可能模糊、不均匀；Windows 尤其明显：
// Windows 的 GDI 渲染传统上重度依赖 hinting，无 hinting 时回退到灰度抗锯齿，效果较差；
// 小字号可读性下降：中文笔画复杂，低像素下容易糊成一团。为什么现代字体常提供 Unhinted
// 版本？屏幕 DPI 演进：2000 年代 96 DPI (1x)，Hinting 至关重要；2010 年代 130 DPI
// (Retina)，Hinting 有帮助；2020 年代 200+ DPI (4K/手机)，Hinting 基本不需要。高 DPI
// 时代，Unhinted 成为主流选择。
//
// Can get look-and-feel hints from the underlying operating system, so it never
// seems out of place. Is adaptable to the different form factors and resolution
// ranges. Has good standard support for high contrast and other similar display
// modes. Supports both manual layout and lightweight widget packing mechanisms;
// stays away from heavier constraints models as much as is practical. Supports
// animations, sounds and such, without being over-the-top by default. Supports
// RTL, i18n and l10n out of the box. In other words, I want a modern, GPU accelerated
// version of the Delphi VCL.
//
// Widgets               Containers
//  Label                 Window
//  Button                Panel
//  ToggleButton          Row
//  RangeSlider           Column
//  Scrollbar             Grid
//  Checkbox              Tabs
//  Dropdown              Pills
//  Radio Button          Dock
//  Image                 SlidePane
//  Video                 Modal
//  Text Input            Accordionl
//  Toggle Switch
//  Progress Bar
//  Date Picker
//  Time Picker
//  Slider
//  Color Picker        Utilities
//  Calendar             Translation Provider
//  File Picker          Animation Provider
//  Table                Localization Provider
//  List                 Theme Provider
//  Buttonbar
//  Menubar
//  Menu
//  Menu Item
//  Tooltip
//  Badge
//  PixelCanvas
//
// 风格，审美，启示，观感，真实
//
// https://without.boats/blog/futures-unordered/
// https://without.boats/blog/let-futures-be-futures/
//
// 每个屏幕上的像素，是直接生成的实时流，没有 HTML，没有布局引擎，没有代码，仅仅是你想
// 要看到的东西。打开浏览器，没有代码，美哟 HTML，没有 CSS 布局引擎，屏幕上每一帧画面，
// 都是 AI 模型实时生成的像素视频流。它能瞬间理解你的意图，动态重塑整个界面，从旅行规
// 划到复杂数据可视化，全是手绘级插图般生动，还能随点击无缝变形、交互。视频模型实现真
// 实演示，1080p 24fps 实时流式传输，背后是 Modal GPU 服务器。一切简化为像素流，模型
// 直接决定你看到什么、怎么交互。无需布局引擎，插图随窗口自适应变形，不再被 CSS 框死。
// 全屏互动，任何像素都能响应点击，模型实时判断意图，不再局限于预定义按钮。视觉优先，
// 复杂概念用插图、动画、真实渲染表达，而不是枯燥文字和矩形框。(WebSocket)
//
// https://registry.khronos.org/SPIR-V/specs/unified1/SPIRV.html
//
// 指令（instructions）
//
//  1.  字（Word）：32 位。
//  2.  标识符<id>：一个数值名称，用来表示一个对象、类型、函数、标签等。每个 <id> 总占
//      用一个字，一个模块（module）中定义的<id>遵循静态单赋值（SSA）原则。
//  3.  结果 Result <id>：绝大多数指令都会定义一个结果，并由指令显式给出的 <id> 命名。
//      该 Result <id> 可在其他指令里作为操作数。
//  4.  字面量（Literal）：立即数，不是标识符<id>。超过一个字长的字面量会占用多个操作数，
//      每个占一个字长。指令会说明该字面量将被解释成何种类型。
//      字符串，视为以 NULL 结尾的字符流，所有字符区分大小写，字符集为 UTF-8。UTF-8 的
//      字节按小端每4个字节打包进一个字中，即第一个字节占字的最低8位，最后一个字包含NUL
//      终止符，其后多位全部填零。
//      数值字面量，若数值类型的位宽小于32位，值放在字的低有效位，高有效位对于浮点或符
//      号位为0的整数填0，而对于符号位为1的整数做符号位扩展必须都填1。位宽大于 32 位但
//      非 32 位整数倍时，剩余位也按同样规则处理。
//  5.  操作数（Operand）：一个指令的单字长参数，可以是一个标识符 <id>，或者是一个字面
//      量，或部分是一个字面量。其具体形式由操作码明确指定。
//  6.  字计数（WordCount）：一条指令所占的全部字数，包括字计数本身和操作码，以及所有可
//      选操作数。换言之，指令的 WordCount 即该指令在二进制流里占用的总空间。
//  7.  指令（Instruction）：模块头部之后，模块就是由一个指令列表组成。一条指令包含一个
//      字计数（WordCount），一个操作码（Opcode），一个可选的结果 Result <id>，一个可选
//      的表示指令类型的 <id>，和一个可变长度的操作数（Operand）列表。所有指令的操作码
//      及语义均在后面的指令章节列出。
//  8.  修饰符（Decoration）：表示附加信息，例如内建（built-in）变量、流（stream）编号、
//      不变性（invariance）、插值方式（interpolation type）、宽松精度（relaxed precision）
//      等，通过修饰符指令附加到 <id> 或结构体成员上。所有修饰符枚举见后面的二进制形式
//      章节修饰符部分。
//  9.  对象（Object）：一个非 void 类型的实例，可表现为：某一运算操作的 Result <id>，或
//      通过 OpVariable 创建的变量。
//  10. 内存对象（Memory Object）：一个由 OpVariable 创建的对象。若为函数变量，仅在函数
//      期间存在；否则在整个 invocation 期间存在。
//  11. 内存对象声明（Memory Object Declaration）：一个变量（OpVariable），或者一个指针
//      类型的函数参数（OpFunctionParameter），或者一个变量的内容，它持有一个 PhysicalStorageBuffer
//      存储类型的指针，或此类指针的数组。
//  12. 中间对象/值/结果（Intermediate Object / Value / Result）：由运算指令（而非 OpVariable
//      分配的内存）创建的对象，并在最后一次使用后即消亡。
//  13. 常量指令（Constant Instruction）：特化常量（specialization-constant）指令（以 OpSpec
//      开头）或非特化常量指令（以 OpConstant 开头）。
//  14. 区间记号：[a, b] 表示闭区间（含端点），(a, b] 表示开闭区间（不含 a 但含 b），以此类推。
//  15. 非语义指令（Non-Semantic Instruction）：对程序语义无影响的指令，可安全地从模块中删除。
//  16. 提示（Hint）：向编译器暗示某属性多半成立，或请求编译器执行特定转换。它们不改变程序
//      语义。除非另有说明，编译器不得假定该属性一定成立，也不得假定该转换总是安全可行。
//
// OpenGL 是一种编程接口，可以对图形硬件设备特性进行访问的软件库。OpenGL 官方参考文档
// http://www.opengl-redbook.com/。在应用层面，OpenGL 4.5 最大的更新是引入了直接状态
// 访问（Direct State Access, DSA），这是对 OpenGL 编程模型以及应用程序访问对象方式的
// 全面革新。OpenGL 4.5 引入的新编程模型，允许直接修改对象状态而无需先绑定对象。此外，
// 随着将越来越多的功能转移到图形处理器的趋势持续发展，本书更多地致力于着色器功能和 GPU
// 处理相关内容。OpenGL 本身并不包含任何执行窗口任务或者处理用户输入的函数，于此类似，
// OpenGL 也没有提供任何用于表达三维物体模型、或者读取图像文件的操作。这个时候，我们需
// 要通过一系列的几何图元（geometric primitives）来创建三维空间中的物体，包括点、线、
// 三角形、和 Patch。OpenGL 是使用客户端-服务端的形式实现的，我们编写的应用程序可以看作
// 客户端，而计算机图形硬件厂商所提供的 OpenGL 实现可看作是服务端。OpenGL 的某些实现
// （例如 X 窗口系统的实现）允许服务端和客户端在一个网络内的不同计算机上运行。这种情况
// 下，客户端负责提交 OpenGL 命令，这些命令然后被转换为窗口系统相关的协议，通过共享网络
// 传输到服务器，最终执行并产生图像内容。
//
// 所有 OpenGL 程序的基本结构通常都是类似的，一是初始化物体关联的状态，这些状态决定物体
// 如何渲染，二是设置这些需要渲染的物体。
//
//  1.  数学图元（primitives），点、线、三角形、Patch，各种不同的着色器（shaders）可
//      以对图元进行计算，来确定它们的位置、颜色、和其他渲染（rendering）属性
//  2.  屏幕片元（fragments），将输入图元的数学描述转换为与屏幕位置对应的像素片元，也
//      称为光栅化（rasterization），最后针对光栅化过程产生的每个片元，可以执行片元着
//      色器（fragment shader），来决定这个片元的最终颜色和位置。如果有必要，还可以对
//      每个片元执行一些额外的操作（per-fragment operations），来决定片元对应的对象是
//      否可见，或者将片元的颜色与当前屏幕位置的颜色进行融合（blending）。
//  3.  渲染（rendering），表示计算机从模型（model）创建最终图像（image）的过程。OpenGL
//      只是其中一种渲染系统，它是基于光栅化（rasterization-based）的系统，但是也有别
//      方法用于生成图像，例如光线追踪（ray tracing），而这类技术已经超出了本书的介绍
//      范围。不过，即使一个系统使用了光线追踪技术，同样有可能需要用到 OpenGL 来显式图
//      像，或者计算图像生成所需的信息。此外，OpenGL 最新版本所提供的灵活性已经变得如此
//      之大，以至于诸如光线追踪（ray tracing）、光子映射（photon mapping）、路径追踪
//      （path tracing）以及基于图像的渲染（image-based rendering）等算法，在可编程图
//      形硬件上的实现已变得相对容易。光线追踪，通过追踪光线路径模拟真实光照，生成高质量
//      图像。光子映射，预计算光子传播路径，用于全局光照模拟。路径追踪，光线追踪的蒙特卡
//      洛变体，通过随机采样路径求解渲染方程。基于图像的渲染，利用现有图像（照片、环境贴
//      图）合成新视角图像，而非纯几何计算。可编程图形硬件，支持着色器编程的 GPU，区别于
//      早期固定功能管线。
//  4.  模型（models）或物体、对象（objects），是由图元（点、线、三角形）构建的，而图
//      元通过它们的顶点（vertices）指定。
//  5.  着色器（shaders）是另一个 OpenGL 最本质的概念，它是图形硬件设备所执行的一类特
//      殊函数。理解着色器最好的办法是把它看作专门为图形处理单元（GPU）编译的一种小型
//      程序。OpenGL 在其内部包含了所有的编译器工具，可以直接从着色器源代码创建 GPU 所
//      需的代码并执行。在 OpenGL 中，有六个不同的着色阶段（shader stages）。其中最常
//      用的是顶点着色（vertex shader）用于处理顶点数据，和片元着色（fragment shader）
//      用于处理光栅化后的片元数据。
//  6.  像素（pixel）是显示器上最小的可见单元，最终生成的图像包含了屏幕上绘制的所有像素
//      点。计算机系统将所有的像素保存在一个帧缓存（framebuffer）中，此为图形硬件设备
//      管理的一块独立内存区域，可以直接映射到最终的显式设备上。
//
// 窗口上下文创建、提供输入和事件
// https://github.com/glfw/glfw
// for creating windows, contexts and surfaces, receiving input and events
//
// 加载 OpenGL 核心规范功能，是获取OpenGL核心配置文件规范所提供功能的最简单方法
// https://github.com/skaslev/gl3w
// Simple opengl core profile loading, get your hands on the functionality
// offered by the opengl core profile specification. Its main part is a simple
// gl3w_gen.py Python script that downloads the Khronos supported glcorearb.h
// header and generates gl3w.h and gl3w.c from it. Those files can then be
// added and linked (statically or dynamically) into your project.
//
// 基于官方规格的多语言 Vulkan/GL/GLES/EGL/GLX/WGL 加载生成器
// https://github.com/Dav1dde/glad  https://gen.glad.sh/
// Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official
// specifications for multiple languages.
//
// OpenGL 编程接口完整列表和参考，并包含各版本 API 参考页面集合
// https://github.com/KhronosGroup/OpenGL-Refpages
//  . es1.1 - OpenGL ES 1.1 (fixed function)
//  . es2.0 3.0 3.1 - OpenGL ES 2.0 3.0 3.1
//  . es3 - OpenGL ES 3.x (will always be the latest ES, currently 3.2)
//  . gl2.1 - OpenGL 2.1 (including fixed functionality)
//  . gl4/html/index.php - OpenGL 4.x (always be the latest GL, currently 4.6)
//
// OpenGL 标准头文件和扩展头文件。由于扩展因平台和驱动而异，OpenGL 开发人员不能期望所有
// 扩展的接口都定义在操作系统/显卡驱动提供的标准 gl.h、glx.h 和 wgl.h 头文件中，因此提
// 供了额外的头文件。这些头文件定义的接口包括枚举常量、函数原型，以及对于支持动态运行时扩
// 展查询的平台（如 Linux 和 Microsoft Windows），还包括函数指针类型定义。
//  . <GL/gl.h>
//  . <GL/glx.h>
//  . <GL/wgl.h>
//  . <GL/glcorearb.h> OpenGL 核心配置文件和 ARB 扩展接口，如《OpenGL 4.3 规范》附
//    录 G.2 所述。不包含仅存在于兼容配置文件中的接口。
//  . <GL/glext.h> OpenGL 1.2 及以上版本的兼容配置文件和扩展接口。
//  . <GL/glxext.h> GLX 1.3 及以上版本的 API 和 GLX 扩展接口。
//  . <GL/wglext.h> WGL 扩展接口。
//
// https://registry.khronos.org/OpenGL/index_gl.php
// https://github.com/KhronosGroup/OpenGL-Registry/pull/183
//
// 以上几乎所有的头文件都依赖于一个称为 <KHR/khrplatform.h> 的跨平台头文件，该文件为多
// 个 Khronos API 所共用。Khronos 共享平台头文件<KHR/khrplatform.h>，所有 OpenGL 头
// 文件都依赖于来自 EGL Registry 的共享 <KHR/khrplatform.h> 头文件。这是一个新增的依
// 赖关系，在上面链接所示的 OpenGL-Registry 拉取请求中引入，旨在增强 OpenGL 与 OpenGL
// ES 头文件之间的兼容性。
//
// OpenGL 函数名称都以 gl 开头，常量名称 GL_COLOR，OpenGL 基本数据类型和后缀名称如下，
// glUniform2f 表示传入 2 个浮点类型参数，glUniform2fv表示传入一个向量（vector）参数，
// 其中包含 2 个浮点类型数据。
//  . GLbyte                    b       有符号字节型
//  . GLubyte                   ub      无符号字节型
//  . GLshort                   s       有符号短整型
//  . GLushort                  us      无符号短整型
//  . GLint GLsizei             i       有符号32位整型
//  . GLuint GLenum GLbitfield  ui      无符号32位整型
//  . GLfloat GLclampf          f       float
//  . GLdouble GLclampd         d       double
//
// OpenGL 实现了常说的渲染管线（rendering pipeline），它是一系列数据处理过程，并且将
// 应用程序的数据转换到最终渲染的图像。下图为 OpenGL 4.5 版本的管线，自从 OpenGL 诞生
// 以来，它的渲染管线已经发生了非常大的改变。它包含一系列着色：顶点着色、细分着色（包含
// 两个着色器）、几何着色、片元着色。着色器在 OpenGL 程序中扮演重要的角色，你可以完全控
// 制使用的着色器以及用着色器做什么，并不是所有的着色阶段都是必需的，实际上只有顶点着色
// 和片元着色是必需的，细分着色和几何着色是可选步骤。
//
//  1.  顶点数据（Vertex Data），OpenGL 需要将所有的数据都保存到缓存对象（buffer objects）
//      中，它相当于 OpenGL 服务端维护的一块内存区域。我们可以使用多种方式来创建这样的
//      数据缓存，不过最常用的方法是使用下面的函数，我们还需要对这些缓存做一些额外的设置。
//      当讲缓存初始化完毕之后，我们可以通过 OpenGL 的绘图命令请求渲染几何图元，例如使
//      用函数 glDrawArrays()。OpenGL 的绘制通常就是将顶点数据传输到 OpenGL 服务端，可
//      以将一个顶点（vertex）视为一个需要统一处理的数据包。这个包中的数据可以是我们需要
//      的任何数据（我们自己负责定义构成顶点的所有数据），通常其中始终包含位置信息，其他
//      数据例如决定像素的最终颜色。更详细的绘制命令将在使用 OpenGL 绘制章节中涵盖。
//          glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
//          glNamedBufferStorage(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
//          glDrawArrays(GLenum mode, GLint first, GLsizei count);
//  2.  顶点着色器（Vertex Shader），对于绘制命令传输的每个顶点，OpenGL 都会调用一个
//      顶点着色器来处理顶点相关的数据。根据其他光栅化之前的着色器的活跃状态，顶点着色器
//      可能非常简单，例如只是将数据复制并传递到下一个着色阶段，称为传递着色器（pass-through
//      shader）。也可能非常复杂，例如执行大量的计算来得到顶点在屏幕上的位置，通常会用到
//      变换矩阵（transformation matrices）的概念，或者通过光照的计算来判断顶点的颜色，
//      或者其他一些技法的实现。通常一个复杂的程序可能包含许多各顶点着色器，但是在同一时
//      刻只能有一个顶点着色器起作用（处于激活状态，active）。
//  3.  细分控制（Tessellation 镶嵌 密铺 细分曲面 紧密排列 Control）着色器
//  4.  细分求值（Tessellation Evaluation）着色器，当顶点着色器处理完每个顶点的数据之
//      后，如果同时激活了细分着色器，那么它将进一步处理这些信息。后面可以看到，细分着色
//      器使用 Patch 来描述一个物体的形状（object's shape），并且使用相对简单的 Patch
//      几何体集合来完成细分（tessllated）的工作，其结果是几何图元的数量增加，提供更好
//      看的模型外观。细分着色阶段潜在地使用两个着色器来分别操作 Patch 数据和生成最终的
//      形状（shape）。
//  5.  几何（Geometry）着色器允许在光栅化之前对每个几何图元做更进一步的处理，例如创建
//      新的图元。这个着色阶段是可选的，但后面我们可以体会到它的强大之处。
//  6.  图元装配（Primitive Setup/Assembly），前面的着色阶段所处理的都是顶点数据，此外
//      这些顶点之间如何构成几何图元的所有信息也会被传递到 OpenGL 中。图元装配阶段将这些
//      顶点和相关的几何图元之间组织到一起，准备下一步的剪切和光栅化操作。
//  7.  剔除和剪切（Culling and Clipping），顶点可能会落在视口（viewport）之外，也就是
//      我们可以进行绘制的窗口区域，此时会导致与顶点相关的图元需要裁剪以保证像素不会再视
//      外绘制。这一过程叫作剪切，它是由 OpenGL 自动完成的。
//  8.  光栅化（Rasterization）阶段负责生成剪切区域（clipping region）内所有图元的片元
//      数据。在上一步裁剪更新后的图元会立即传递到光栅化阶段处理，以生成片元，空间坐标数
//      据信息转换为屏幕像素信息。光栅化的工作是确定哪些屏幕位置被特定几何体（点、线、或
//      三角形）覆盖。知道了这些屏幕像素位置，以及输入的顶点数据，光栅化器就会对片元着色
//      器中的每个可变变量/插值变量（varying variable）数据值进行线性插值，并将这些值作
//      为输入送入你的片元着色器。可以把片元（fragment）想象成一个“候选像素”（Candidate
//      pixel），片段的比喻说法强调其尚未确定能否写入帧缓冲，这些像素在帧缓冲区中有固定
//      的位置。然而片元仍可能被拒绝，永远无法更新其关联的屏幕像素位置。片元会在接下来的
//      片元着色和逐片元操作两个阶段进行处理。注意，OpenGL 实现如何进行光栅化和插值是平
//      台相关的，你不应期望不同平台会以完全相同的方式插值。虽然光栅化开启了一个片元的生
//      命，且片元着色器中进行的计算对于计算片元的最终颜色至关重要，但这绝不是可以应用于
//      片元的全部处理。
//  9.  片元（Fragment）着色器，最后一个可以通过编程控制屏幕像素位置上显示颜色的阶段，称
//      为片元着色阶段（fragment shading）。这个阶段，我们使用一个着色器来确定片元的最
//      终颜色（尽管逐片元操作阶段还可能改变依次颜色）和它的深度值（depth value）。片元
//      着色器非常强大，通常这个阶段会应用纹理映射来对顶点处理阶段所计算的颜色进行增强。
//      如果我们觉得不应该继续绘制某个片元，在片元着色器中还可以终止这个片元的处理，这一
//      处理叫做片元丢弃（fragment discard）。如果我们需要更好地理解处理顶点的着色器与
//      片元着色器之间的区别，可以用这种方式理解：顶点着色（包括细分和几何着色）决定了一
//      个图元应该出现在屏幕什么位置，而片元着色使用这些信息决定某个片元的颜色应该是什么。
//      片元 ≠ 像素，一个像素可能被多个片段覆盖（如透明叠加、多重采样），也可能没有片段
//      （被裁剪/深度测试拒绝）。因此称为“候选像素”，强调其尚未确定能否写入帧缓冲。
//  10. 逐片元操作（Per-Fragment Operations），该阶段使用深度测试（depth testing，也
//      通常称为 z-buffering）和模板测试（stencil testing）来决定一个片元是否可见。如
//      果一个片元成功通过了所有启用测试，那么它可能直接写入帧缓存（framebuffer）更新其
//      像素的颜色（以及可能的深度值）；或者如果启用了混合（blending），片段的颜色将与像
//      素的当前颜色相结合，生成一个新的颜色写入帧缓冲区。通常，像素数据来自图像文件，尽
//      管它也可以通过使用 OpenGL 渲染创建的。像素数据通常保存在纹理贴图（texture map）
//      当中，通过纹理映射（texture mapping）的方式调用，在纹理映射阶段中我们可以从一个
//      或多个纹理贴图中查找所需的数据值。
//
// 可以通过第三方库创建串口，例如 GLFW 通过 glfwCreateWindow() 创建窗口并且还创建一个
// OpenGL Context 关联到这个窗口上。要开始使用这个上下文，需要将它设置成当前上下文。一
// 个程序可以有多个上下文和多个窗口，而当前上下文（current context）是当前处理命令的这
// 个上下文。实际上，你程序中的每个线程都有一个对应的当前上下文。
