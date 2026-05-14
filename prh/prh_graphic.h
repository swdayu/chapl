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
// https://wikis.khronos.org/opengl/Main_Page
// https://wikis.khronos.org/opengl/Getting_Started
// https://www.realtech-vr.com/glview-download/
//
// 基于官方规格的多语言 Vulkan/GL/GLES/EGL/GLX/WGL 加载生成器
// 生成方式：通过 在线生成器 或本地 Python 脚本生成
// 功能丰富：支持 OpenGL、OpenGL ES、EGL、GLX、WGL 等多种 API
// https://github.com/Dav1dde/glad  https://gen.glad.sh/
// Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official
// specifications for multiple languages.
//
// 加载 OpenGL 核心规范功能，是获取OpenGL核心配置文件规范所提供功能的最简单方法
// 生成方式：基于 Python 脚本，从 Khronos 官方 gl.xml 生成 C 代码
// 代码体积：极简，只生成你需要的部分，体积很小
// https://github.com/skaslev/gl3w
// Simple opengl core profile loading, get your hands on the functionality
// offered by the opengl core profile specification. Its main part is a simple
// gl3w_gen.py Python script that downloads the Khronos supported glcorearb.h
// header and generates gl3w.h and gl3w.c from it. Those files can then be
// added and linked (statically or dynamically) into your project.
//
// 窗口上下文创建、提供输入和事件
// https://github.com/glfw/glfw
// for creating windows, contexts and surfaces, receiving input and events
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
// void glEnable(GLenum capability); // 2.0+
// void glEnablei(GLenum capability, GLuint index); // 3.0+
// void glDisable(GLenum capability); // 2.0+
// void glDisablei(GLenum capability, GLuint index); // 3.0+
// GLboolean glIsEnabled(GLenum capability); // 2.0+
// GLboolean glIsEnabledi(GLenum capability, GLuint index); // 3.0+
//
// 启用或禁用服务器端 GL 功能。glEnable 和 glDisable 用于启用和禁用各种功能。使用 glIsEnabled
// 或 glGet 来确定任何功能的当前设置。除 GL_DITHER 和 GL_MULTISAMPLE 外，每个功能的初
// 始值均为 GL_FALSE。GL_DITHER 和 GL_MULTISAMPLE 的初始值为 GL_TRUE。glEnable 和
// glDisable 均接受单个参数 cap，该参数可假定为以下值之一。部分 GL 功能是索引化的，
// glEnablei 和 glDisablei 用于启用和禁用索引化的功能。如果 cap 不是上述列出的值之一，
// 则生成 GL_INVALID_ENUM。如果 index 大于或等于 cap 的索引化功能数量，则 glEnablei
// 和 glDisablei 生成 GL_INVALID_VALUE。glEnable 或 glDisable 接受的任何标记也都被
// glEnablei 和 glDisablei 接受，但如果该功能不是索引化的，则 index 可取的最大值为零。
// 一般来说，将索引化功能传递给 glEnable 或 glDisable 将分别启用或禁用该功能的所有索引。
//
// GL_BLEND
//      如果启用，将计算出的片段颜色值与颜色缓冲区中的值进行混合。参见 glBlendFunc。
// GL_CLIP_DISTANCE i
//      如果启用，根据用户定义的半空间 i 裁剪几何体。
// GL_COLOR_LOGIC_OP
//      如果启用，将当前选定的逻辑运算应用于计算出的片段颜色和颜色缓冲区值。参见 glLogicOp。
// GL_CULL_FACE
//      如果启用，根据多边形在窗口坐标中的绕向来剔除多边形。参见 glCullFace。
// GL_DEBUG_OUTPUT（4.3+）
//      如果启用，调试上下文会产生调试消息。禁用时，调试消息日志将被静默。注意，在非调试
//      上下文中，即使启用了 GL_DEBUG_OUTPUT，也可能很少或根本不产生消息。
// GL_DEBUG_OUTPUT_SYNCHRONOUS（4.3+）
//      如果启用，调试上下文会同步产生调试消息。如果禁用，调试消息可能异步产生。特别是，
//      它们可能相对于 GL 命令的执行有所延迟，且调试回调函数可能从执行命令的线程以外的线
//      程被调用。参见 glDebugMessageCallback。
// GL_DEPTH_CLAMP
//      如果启用，视图体裁剪将忽略 −wc≤zc≤wc 平面方程（实际上，不存在近或远平面裁剪）。
//      参见 glDepthRange。
// GL_DEPTH_TEST
//      如果启用，执行深度比较并更新深度缓冲区。注意，即使深度缓冲区存在且深度掩码非零，
//      如果禁用了深度测试，深度缓冲区也不会更新。参见 glDepthFunc 和 glDepthRange。
// GL_DITHER
//      如果启用，在将颜色分量或索引写入颜色缓冲区之前对其进行抖动处理。
// GL_FRAMEBUFFER_SRGB
//      如果启用，且对应目标缓冲区的帧缓冲区附件的 GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING
//      值为 GL_SRGB，则 R、G 和 B 目标颜色值（从定点转换为浮点后）被视为已编码为 sRGB
//      颜色空间，因此在使用于混合之前会先进行线性化处理。
// GL_LINE_SMOOTH
//      如果启用，使用正确的过滤绘制线条。否则，绘制锯齿线。参见 glLineWidth。
// GL_MULTISAMPLE
//      如果启用，在计算像素的最终颜色时使用多个片段样本。参见 glSampleCoverage。
// GL_POLYGON_OFFSET_FILL
//      如果启用，且多边形以 GL_FILL 模式渲染，则在执行深度比较之前向多边形片段的深度值
//      添加偏移量。参见 glPolygonOffset。
// GL_POLYGON_OFFSET_LINE
//      如果启用，且多边形以 GL_LINE 模式渲染，则在执行深度比较之前向多边形片段的深度值
//      添加偏移量。参见 glPolygonOffset。
// GL_POLYGON_OFFSET_POINT
//      如果启用，且多边形以 GL_POINT 模式渲染，则在执行深度比较之前向多边形片段的深度
//      值添加偏移量。参见 glPolygonOffset。
// GL_POLYGON_SMOOTH
//      如果启用，使用适当的过滤绘制多边形。否则，绘制锯齿多边形。为了正确实现抗锯齿多边
//      形，需要 Alpha 缓冲区，且多边形必须按从前到后的顺序排序。
// GL_PRIMITIVE_RESTART（3.1+）
//      启用图元重启。如果启用，任何将一组通用属性数组元素传输到 GL 的绘制命令，当顶点索
//      引等于图元重启索引时，将重启图元。参见 glPrimitiveRestartIndex。
// GL_PRIMITIVE_RESTART_FIXED_INDEX（4.3+）
//      使用固定索引启用图元重启。如果启用，任何将一组通用属性数组元素传输到 GL 的绘制命
//      令，当顶点索引等于指定索引类型的固定图元索引时，将重启图元。固定索引等于 2^n−1，
//      其中 n 对于 GL_UNSIGNED_BYTE 等于 8，对于 GL_UNSIGNED_SHORT 等于 16，对于
//      GL_UNSIGNED_INT 等于 32。
// GL_RASTERIZER_DISCARD
//      如果启用，图元在可选的变换反馈阶段之后、光栅化之前被丢弃。此外，启用时，glClear、
//      glClearBufferData、glClearBufferSubData、glClearTexImage 和 glClearTexSubImage
//      将被忽略。
// GL_SAMPLE_ALPHA_TO_COVERAGE
//      如果启用，计算一个临时覆盖值，其中每个位由对应样本位置的 Alpha 值决定。然后将临
//      时覆盖值与片段覆盖值进行按位与（AND）运算。
// GL_SAMPLE_ALPHA_TO_ONE
//      如果启用，每个样本的 Alpha 值被替换为可表示的最大 Alpha 值。
// GL_SAMPLE_COVERAGE
//      如果启用，片段的覆盖值与临时覆盖值进行按位与（AND）运算。如果 GL_SAMPLE_COVERAGE_INVERT
//      设置为 GL_TRUE，则反转覆盖值。参见 glSampleCoverage。
// GL_SAMPLE_SHADING
//      如果启用，活动片段着色器对每个被覆盖的样本运行一次，或按 GL_MIN_SAMPLE_SHADING_VALUE
//      当前值确定的该速率的一部分运行。参见 glMinSampleShading。
// GL_SAMPLE_MASK
//      如果启用，光栅化期间为片段生成的样本覆盖掩码将在着色发生之前与 GL_SAMPLE_MASK_VALUE
//      的值进行按位与（AND）运算。参见 glSampleMaski。
// GL_SCISSOR_TEST
//      如果启用，丢弃裁剪矩形之外的片段。参见 glScissor。
// GL_STENCIL_TEST
//      如果启用，执行模板测试并更新模板缓冲区。参见 glStencilFunc 和 glStencilOp。
// GL_TEXTURE_CUBE_MAP_SEAMLESS（3.2+）
//      如果启用，立方体贴图纹理的采样方式使得从两个相邻面之间的边界进行线性采样时，使用
//      来自两个面的纹理元素来生成最终采样值。禁用时，仅使用单个面的纹理元素来构造最终采
//      样值。
// GL_PROGRAM_POINT_SIZE
//      如果启用且顶点或几何着色器处于活动状态，则派生点大小取自（可能被裁剪的）着色器内
//      置变量 gl_PointSize，并钳制到实现相关的点大小范围。
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
//      中，它相当于 OpenGL 服务端维护的一块内存区域。我们有多种方式用数据填充这些缓冲区，
//      不过最常用的方法是使用下面的函数，我们还需要对这些缓存做一些额外的设置。当将缓存
//      初始化完毕之后，我们可以通过 OpenGL 的绘图命令请求渲染几何图元，例如使用函数
//      glDrawArrays()。OpenGL 的绘制通常就是将顶点数据传输到 OpenGL 服务端，可以将一
//      个顶点（vertex）视为一个需要统一处理的数据包。这个包中的数据可以是我们需要的任何
//      数据（我们自己负责定义构成顶点的所有数据），通常其中始终包含位置信息，其他数据例
//      如决定像素的最终颜色。更详细的绘制命令将在使用 OpenGL 绘制章节中涵盖。
//          glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
//          glNamedBufferStorage(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
//          glDrawArrays(GLenum mode, GLint first, GLsizei count);
//  2.  顶点着色器（Vertex Shader），对于绘制命令传输的每个顶点，OpenGL 都会调用一个     *** 一、顶点着色（M）：单独处理顶点缓存对象中的每个顶点数据
//      顶点着色器来处理顶点相关的数据。根据其他光栅化之前的着色器的活跃状态，顶点着色器
//      可能非常简单，例如只是将数据复制并传递到下一个着色阶段，称为传递着色器（pass-through
//      shader）。也可能非常复杂，例如执行大量的计算来得到顶点在屏幕上的位置，通常会用到
//      变换矩阵（transformation matrices）的概念，或者通过光照的计算来判断顶点的颜色，
//      或者其他一些技法的实现。通常一个复杂的程序可能包含许多各顶点着色器，但是在同一时
//      刻只能有一个顶点着色器起作用（处于激活状态，active）。
//  3.  细分控制（Tessellation 镶嵌 密铺 细分曲面 紧密排列 Control）着色器             *** 二、细分着色（O）：接收顶点着色后的数据进一步处理，通过
//  4.  细分求值（Tessellation Evaluation）着色器，当顶点着色器处理完每个顶点的数据之       Patch 细分形态生成额外的几何图元
//      后，如果同时激活了细分着色器，那么它将进一步处理这些信息。后面可以看到，细分着色
//      器使用 Patch 来描述一个物体的形状（object's shape），并且使用相对简单的 Patch
//      几何体集合来完成细分（tessllated）的工作，其结果是几何图元的数量增加，提供更好
//      看的模型外观。细分着色阶段潜在地使用两个着色器来分别操作 Patch 数据和生成最终的
//      形状（shape）。
//  5.  几何（Geometry）着色器允许在光栅化之前对每个几何图元做更进一步的处理，例如创建   *** 三、几何着色（O）：接着处理顶点着色或细分着色后的每个几何
//      新的图元。这个着色阶段是可选的，但后面我们可以体会到它的强大之处。                   图元，可以基于输入图元生成更多几何体，或者例如将三角形
//  6.  图元装配（Primitive Setup/Assembly），前面的着色阶段所处理的都是顶点数据，此外      变为线段改变几何图元类型，或者放弃几何体。
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
//  9.  片元（Fragment）着色器，最后一个可以通过编程控制屏幕像素位置上显示颜色的阶段，称   *** 四、片元着色（M）：处理光栅化后的每个独立片元（fragments）
//      为片元着色阶段（fragment shading）。这个阶段，我们使用一个着色器来确定片元的最        或采样（samples，如果采用着色开启，sample-shading），这个
//      终颜色（尽管逐片元操作阶段还可能改变依次颜色）和它的深度值（depth value）。片元        阶段计算每个片元的颜色和深度值，然后传递到管线的片元测试和
//      着色器非常强大，通常这个阶段会应用纹理映射来对顶点处理阶段所计算的颜色进行增强。        混合模块。
//      如果我们觉得不应该继续绘制某个片元，在片元着色器中还可以终止这个片元的处理，这一
//      处理叫做片元丢弃（fragment discard）。如果我们需要更好地理解处理顶点的着色器与
//      片元着色器之间的区别，可以用这种方式理解：顶点着色（包括细分和几何着色）决定了一    *** 五、计算着色（Compute Shading）：与上述着色阶段不同，计算
//      个图元应该出现在屏幕什么位置，而片元着色使用这些信息决定某个片元的颜色应该是什么。      着色不是图形渲染着色的一部分，二是在程序中相对独立的一个阶段。
//      片元 ≠ 像素，一个像素可能被多个片段覆盖（如透明叠加、多重采样），也可能没有片段         计算着色处理的并不是顶点和片元这类图形数据，而是计算应用程序
//      （被裁剪/深度测试拒绝）。因此称为“候选像素”，强调其尚未确定能否写入帧缓冲。            选定范围内的通用工作项。计算着色可以处理其他着色程序创建和使用
//  10. 逐片元操作（Per-Fragment Operations），该阶段使用深度测试（depth testing，也        的缓存数据，以及包括帧缓存后处理效果，或者所期望的任何任务。
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
//
// 顶点数组对象（VAO，Vertex Array Object），创建函数会返回顶点数组对象的名字，该名字    *** 顶点数组对象（VAO）
// 相当于 C 语言中的指针，指向分配在内存中的顶点数组对象。调用 glGetError() 可以判断调
// 用 OpenGL 函数后是否报错，OpenGL 的多个错误可能累积，需循环查询知道无错误（GL_NO_ERROR）
// 为止。错误独立存在于每个 OpenGL 线程上下文中。
//
// void glCreateVertexArrays(GLsizei n, GLuint *arrays); // 4.5+
// void glGenVertexArrays(GLsizei n, GLuint *arrays); // 3.0+
// GLboolean glIsVertexArray(GLuint array); // 3.0+
//
// 参数 n 表示创建顶点数组对象的个数，参数 arrays 指向 uint 数组，该数组的每个元素接收
// 数组的新顶点数组对象的名字。返回 n 个未使用的顶点数组对象的名称，每个都表示一个初始化
// 为默认状态的顶点数组对象。如果 n 为负数，则返回错误 GL_INVALID_VALUE。通过调用函数
// glGenVertexArrays() 创建的顶点数组对象，必须 glBindVertexArray 进行绑定，才是一个
// glIsVertexArray 对象。另外，如果 array 为零，或者 array 不是一个顶点对象的名字，或
// 者有错误发生，glIsVertexArray 都会返回 GL_FALSE。
//
// void glBindVertexArray(GLuint array); // 3.0+
// void glDeleteVertexArrays(GLsizei n, const GLuint *arrays); // 3.0+
//
// glDeleteVertexArrays 删除 n 个顶点数组对象，置为无内容未使用状态。如果 n 为负数，则
// 返回错误 GL_INVALID_VALUE。一旦顶点数组对象被删除，它便不再包含任何内容，其名称也重
// 新变为未使用状态。如果当前绑定的顶点数组对象被删除，该类对象的绑定将恢复为零，即默认顶
// 点数组变为当前数组。默认顶点数组是绑定为 0 时的隐式 VAO（兼容旧代码）。arrays 中未使
// 用的名称（无效的未生成的或已删除的）将被静默忽略，零值同样如此。
//
// glBindVertexArray 绑定名为 array 的顶点数组对象。array 是先前通过调用 glGenVertexArrays
// 返回的顶点数组对象名称，如果不存在名为 array 的顶点数组对象，即仅仅生成但还没有调用过
// glBindVertexArray 则在首次绑定时会创建一个，并将新创建的对象与提供的 array 名称进行
// 关联，如果顶点数组对象已经创建，那么会激活这个顶点数组对象（即进行绑定）。或者为零以解
// 除现有顶点数组对象的绑定，那么 OpenGL 将不再使用程序所分配的任何顶点数组对象，并且将渲
// 染状态重设为顶点数组的默认状态。如果绑定成功，顶点数组对象的状态不会发生任何改变，并且
// 任何先前的顶点数组对象绑定将被解除。如果 array 不为零，且不是先前通过调用 glGenVertexArrays
// 返回的顶点数组对象名称，则产生 GL_INVALID_OPERATION 错误。绑定对象的过程有点类似设置
// 铁路的道岔开关，一旦设置了开关，从这条线路通过的所有列车都会驶向对应的轨道，如果设置到
// 另一状态，那么所有之后经过的列车都会驶向另一条轨道。总统来讲，在两种情况下我们需要绑定
// 一个对象：创建对象并初始化它所对应的数据时，以及我们准备使用不是当前绑定的对象时。
//  1.  array 为零，解除绑定
//  2.  array 为 glGenVertexArrays 生成但为创建，首次绑定时自动创建
//  3.  array 为 glGenVertexArrays 生成但重复绑定，合法无操作状态不变
//  4.  array 为任意随机整数或已被 glDeleteVertexArrays 删除，GL_INVALID_OPERATION
//
// 与 OpenGL 4.5+ 引入的直接状态访问（DSA）相比，许多操作都无限绑定，直接通过对象作为函
// 数的第一个参数进行访问修改。但 glBindVertexArray 本身仍是必要的，它决定了当前渲染使
// 用哪个顶点数组对象（VAO）。对 glCreateVertexArrays 调用 glBindVertecArray 相当于
// 对已经创建好的顶点数组对象进行绑定。
//
// 缓存对象（Buffer Object）。OpenGL 需要将几乎所有传入的数据都保存到缓存对象中，它相当
// 于 OpenGL 服务端分配和管理的一块内存区域。缓存区域可以存储各种各样的对象，这些对象称为
// 缓存对象目标（buffer object target）。顶点数组对象就是其中一种需要存储在缓存对象中
// 的对象，顶点数组对象相当于仅仅是一个用于区分其他对象类型的包装器，是对缓存对象区域的引
// 用或描述符，它实际的顶点属性数据都存储在缓存对象中。
//
// void glGenBuffers(GLsizei n, GLuint *buffers); // 2.0+
// void glCreateBuffers(GLsizei n, GLuint *buffers); // 4.5+
// void glDeleteBuffers(GLsizei n, const GLuint *buffers); // 2.0+
// GLboolean glIsBuffer(GLuint buffer); // 2.0+
//
// 调用 glGenBuffers 之后，没有缓存对象与名字相关联，直到首次调用 glBindBuffer。如果
// n 为负数，返回 GL_INVALID_VALUE 错误。glCreateBuffers 会之间创建与名字关联的缓存
// 对象。如果删除已经绑定了缓存对象，绑定重新设置为 0（没有绑定任何缓存对象）。如果当前
// 的名字是缓存对象，glIsBuffer 返回 GL_TRUE，如果 buffer 为零，或者非零的非缓存对象
// 名字，或者有错误发生，都返回 GL_FALSE。如果是一个 glGenBuffers 名字，如果还没有调
// 用 glBindBuffer 绑定，也不是一个缓存对象。
//
// void glBindBuffer(GLenum target, GLuint buffer); // 2.0+
// void glGetBooleanv(GLenum type, GLboolean *data); // 2.0+
// void glGetBooleani_v(GLenum target, GLuint index, GLboolean *data); // 3.0+
// void glGetDoublev(GLenum type, GLdouble *data); // 2.0+
// void glGetDoublei_v(GLenum target, GLuint index, GLdouble *data); // 4.1+
// void glGetFloatv(GLenum type, GLfloat *data); // 2.0+
// void glGetFloati_v(GLenum target, GLuint index, GLfloat *data); // 4.1+
// void glGetIntegerv(GLenum type, GLint *data); // 2.0+
// void glGetIntegeri_v(GLenum target, GLuint index, GLint *data); // 3.0+
// void glGetInteger64v(GLenum type, GLint64 *data); // 3.2+
// void glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data); // 3.2+
//
// glBindBuffer 绑定一个 target 类型的缓存对象，glBindBuffer 将缓冲区对象绑定到指定的
// 缓冲区绑定点。调用 glBindBuffer 时，target 设置为某个可接受的常量，buffer 设置为缓
// 冲区对象的名称，则该缓冲区对象名称被绑定到该目标。如果不存在名为 buffer 的缓冲区对象，
// 则会创建一个具有该名称的缓冲区对象。当缓冲区对象被绑定到某个目标时，该目标先前的绑定将
// 自动解除。缓冲区对象名称是无符号整数，零是保留值，但每个缓冲区对象目标没有默认绑定的缓
// 冲区对象。相反，将 buffer 设置为零实际上会解除先前绑定的任何缓冲区对象，并恢复该缓冲区
// 对象目标的客户端内存使用（如果该目标支持的话）。缓冲区对象名称及其对应的缓冲区对象内容
// 对于当前 GL 渲染上下文的共享对象空间是局部的；两个渲染上下文只有在通过适当的 GL 窗口
// 接口函数显式启用上下文间共享时，才会共享缓冲区对象名称。需要调用 glGenBuffers 来生成
// 一组未使用的缓冲区对象名称。如果 target 不是允许的值之一，则产生 GL_INVALID_ENUM 错
// 误。如果 buffer 不是先前通过调用 glGenBuffers 返回的名称，则产生 GL_INVALID_VALUE
// 错误。
//
// 缓冲区对象首次绑定后的状态是一个未映射的、大小为零的内存缓冲区，具有 GL_READ_WRITE
// 访问权限和 GL_STATIC_DRAW 使用方式。当绑定了非零缓冲区对象名称时，对其所绑定目标的
// GL 操作会影响该绑定的缓冲区对象，对该目标的查询会返回该绑定缓冲区对象的状态。当绑定了
// 缓冲区对象名称零时，如在初始状态下，尝试修改或查询其所绑定目标的状态会产生 GL_INVALID_OPERATION
// 错误。通过 glBindBuffer 创建的缓冲区对象绑定将保持活动状态，直到不同的缓冲区对象名称
// 被绑定到同一目标，或者直到绑定的缓冲区对象被 glDeleteBuffers 删除。一旦创建，命名的
// 缓冲区对象可以根据需要多次重新绑定到任何目标。但是，GL 实现可能会根据其初始绑定目标对
// 如何优化缓冲区对象的存储做出选择。
//
// 当非零缓冲区对象绑定到 GL_ARRAY_BUFFER 目标时，顶点数组指针参数（the vertex array
// pointer parameter）被解释为缓冲区对象中以基本机器单位度量的偏移量。
//
// 当非零缓冲区对象绑定到 GL_DRAW_INDIRECT_BUFFER 目标时，通过 glDrawArraysIndirect
// 和 glDrawElementsIndirect 发出的绘制参数从该缓冲区对象数据存储中的指定偏移量获取。
//
// 当非零缓冲区对象绑定到 GL_DISPATCH_INDIRECT_BUFFER 目标时，通过 glDispatchComputeIndirect
// 发出的计算调度参数从该缓冲区对象数据存储中的指定偏移量获取。
//
// 当非零缓冲区对象绑定到 GL_ELEMENT_ARRAY_BUFFER 目标时，glDrawElements、glDrawElementsInstanced、
// glDrawElementsBaseVertex、glDrawRangeElements、glDrawRangeElementsBaseVertex、
// glMultiDrawElements 或 glMultiDrawElementsBaseVertex 的 indices 参数被解释为缓
// 冲区对象中以基本机器单位度量的偏移量。
//
// 当非零缓冲区对象绑定到 GL_PIXEL_PACK_BUFFER 目标时，以下命令会受到影响：glGetCompressedTexImage、
// glGetTexImage 和 glReadPixels。pointer 参数被解释为缓冲区对象中以基本机器单位度量
// 的偏移量。
//
// 当非零缓冲区对象绑定到 GL_PIXEL_UNPACK_BUFFER 目标时，以下命令会受到影响：glCompressedTexImage1D、
// glCompressedTexImage2D、glCompressedTexImage3D、glCompressedTexSubImage1D、
// glCompressedTexSubImage2D、glCompressedTexSubImage3D、glTexImage1D、glTexImage2D、
// glTexImage3D、glTexSubImage1D、glTexSubImage2D 和 glTexSubImage3D。pointer 参数
// 被解释为缓冲区对象中以基本机器单位度量的偏移量。
//
// 提供 GL_COPY_READ_BUFFER（3.1+）和 GL_COPY_WRITE_BUFFER 缓冲区目标，是为了允许使用 glCopyBufferSubData
// 而不干扰其他绑定的状态。但是，glCopyBufferSubData 可以与任意一对缓冲区绑定点一起使
// 用。GL_TRANSFORM_FEEDBACK_BUFFER 缓冲区绑定点可以传递给 glBindBuffer，但不会直接
// 影响变换反馈状态。相反，必须通过调用 glBindBufferBase 或 glBindBufferRange 来使用
// 索引化的 GL_TRANSFORM_FEEDBACK_BUFFER 绑定。这将影响通用的 GL_TRANSFORM_FEEDBACK_BUFFER
// 绑定。同样，GL_UNIFORM_BUFFER（3.1+）、GL_ATOMIC_COUNTER_BUFFER（4.2+） 和 GL_SHADER_STORAGE_BUFFER（4.3+）
// 缓冲区绑定点也可以使用，但分别不会直接影响 uniform 缓冲区、原子计数器缓冲区或着色器存
// 储缓冲区的状态。必须使用 glBindBufferBase 或 glBindBufferRange 将缓冲区绑定到索引
// 化的 uniform 缓冲区、原子计数器缓冲区或着色器存储缓冲区绑定点。GL_QUERY_BUFFER（4.4+） 绑定
// 点用于指定一个缓冲区对象，该对象将通过 glGetQueryObject 系列命令接收查询对象的结果。
//
// 可用的缓存绑定目标类型，以及关联的 glGet 参数：
//      常量                        glGet 参数                              用途和目的
//      GL_ARRAY_BUFFER             GL_ARRAY_BUFFER_BINDING                顶点属性数据，位置、颜色、法线、纹理坐标等（Vertex Attributes，2.0+）
//      GL_ELEMENT_ARRAY_BUFFER     GL_ELEMENT_ARRAY_BUFFER_BINDING        顶点数组索引，图元装配用（Vertex Array Indices，2.0+）
//      GL_ATOMIC_COUNTER_BUFFER    GL_ATOMIC_COUNTER_BUFFER_BINDING       原子计数器存储，用于着色器中的原子操作（Atomic Counter Storage，4.2+）
//      GL_COPY_READ_BUFFER         GL_COPY_READ_BUFFER_BINDING            缓冲区复制源（Buffer Copy Source，3.1+）
//      GL_COPY_WRITE_BUFFER        GL_COPY_WRITE_BUFFER_BINDING           缓冲区复制目标（Buffer Copy Destination，3.1+）
//      GL_DISPATCH_INDIRECT_BUFFER GL_DISPATCH_INDIRECT_BUFFER_BINDING    间接计算调度命令（4.3+）
//      GL_DRAW_INDIRECT_BUFFER     GL_DRAW_INDIRECT_BUFFER_BINDING        间接绘制命令参数（4.0+）
//      GL_PIXEL_PACK_BUFFER        GL_PIXEL_PACK_BUFFER_BINDING           像素打包缓冲区，像素读取目标（Pixel Read Target，2.1+）
//      GL_PIXEL_UNPACK_BUFFER      GL_PIXEL_UNPACK_BUFFER_BINDING         像素解包缓冲区，纹理数据源（Texture Data Source，2.1+）
//      GL_SHADER_STORAGE_BUFFER    GL_SHADER_STORAGE_BUFFER_BINDING       着色器存储（Shaders Read-Write Storage，4.3+）
//      GL_TEXTURE_BUFFER           GL_TEXTURE_BINDING_BUFFER              纹理缓冲区（Texture Data Buffer，3.1+）
//      GL_TRANSFORM_FEEDBACK_BUFFER GL_TRANSFORM_FEEDBACK_BUFFER_BINDING  变换反馈缓冲区，变换反馈捕获的顶点数据
//      GL_UNIFORM_BUFFER           GL_UNIFORM_BUFFER_BINDING              Uniform 块数据（UBO，Uniform Block Storage，3.1+）
//      GL_QUERY_BUFFER             ---                                    查询结果存储（Query Result Buffer，4.4+）
//
// void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage); // 2.0+
// void glNamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage); // 4.5+
// void glBufferStorage(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags); // 4.4+
// void glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags); // 4.5+
//
// 创建缓存对象后，需要将数据载入缓存对象中。调用 glBufferData 分配数据所需的存储空间，
// 然后将数据拷贝到 OpenGL 服务端的内存中。参数 size 表示缓存对像的新数据存储的字节大
// 小，data 表示需要拷贝到数据存储区的数据，如果为 NULL 表示没有数据需要拷贝。如果 data
// 为 NULL，仍会创建指定大小的数据存储，但其内容保持未初始化状态，因此是未定义的。客户端
// 必须按照客户端平台的要求一致地对齐数据元素，并额外满足一个基本要求：缓冲区中到包含 N
// 字节的数据的偏移量必须是 N 的倍数。错误和触发条件：
//      GL_INVALID_ENUM         glBufferData 的 target 不是可接受的缓冲区目标
//      GL_INVALID_ENUM         usage 不是 GL_STREAM_DRAW/READ/COPY、GL_STATIC_DRAW/READ/COPY、GL_DYNAMIC_DRAW/READ/COPY
//      GL_INVALID_VALUE        size 为负数
//      GL_INVALID_OPERATION    glBufferData 绑定的目标缓冲区对象名称为保留的 0
//      GL_INVALID_OPERATION    glNamedBufferData 的 buffer 不是现有缓冲区对象的名称
//      GL_INVALID_OPERATION    缓冲区对象的 GL_BUFFER_IMMUTABLE_STORAGE 标志为 GL_TRUE（即不可变存储）
//      GL_OUT_OF_MEMORY        GL 无法创建指定大小的数据存储
//
// glBufferData 和 glNamedBufferData 为缓冲区对象创建新的数据存储。对于 glBufferData，
// 使用当前绑定到 target 的缓冲区对象。对于 glNamedBufferData，则使用调用者在 buffer
// 中指定的 ID 关联的缓冲区对象。创建新存储时，任何预先存在的数据存储都会被删除，新数据存
// 储以指定的字节大小和用途提示创建。如果 data 不为 NULL，则数据存储将使用此指针的数据进
// 行初始化。在其初始状态下，新数据存储未映射（not mapped），其映射指针（mapped pointer）
// 为 NULL，映射访问权限（mapped access）为 GL_READ_WRITE。usage 是对 GL 实现的一个提
// 示，说明缓冲区对象的数据存储将如何被访问。这使 GL 实现能够做出更智能的决策，从而显著影
// 响缓冲区对象的性能。然而，它并不限制数据存储的实际使用方式。usage 可分为两部分：首先是
// 访问频率（修改和使用），其次是访问性质。
//      访问频率  STREAM  数据存储内容将修改一次，最多使用几次
//               STATIC  数据存储内容将修改一次，使用多次
//               DYNAMIC 数据存储内容将反复修改，使用多次
//      访问性质  DRAW    数据存储内容由应用程序修改，并作为 GL 绘制和图像规范命令的源
//               READ    数据存储内容通过从 GL 读取数据来修改，并在应用程序查询时返回该数据
//               COPY    数据存储内容通过从 GL 读取数据来修改，并作为 GL 绘制和图像规范命令的源
//      典型场景  GL_STREAM_DRAW  每帧更新的动态顶点数据（如粒子系统）
//               GL_STREAM_READ  偶尔读取一次的 GPU 计算结果
//               GL_STREAM_COPY  每帧从纹理读取并用于绘制的数据
//               GL_STATIC_DRAW  加载一次的几何体、纹理坐标
//               GL_STATIC_READ  从不修改的查询结果缓存
//               GL_STATIC_COPY  预烘焙的 GPU 生成数据
//               GL_DYNAMIC_DRAW 频繁更新的 uniform/实例化数据
//               GL_DYNAMIC_READ 每帧读取的 GPU 计算结果
//               GL_DYNAMIC_COPY 每帧在 GPU 上生成并复用的数据
//
// glBufferStorage 和 glNamedBufferStorage 创建一个新的不可变数据存储。对于 glBufferStorage，
// 当前绑定到 target 的缓冲区对象将被初始化。对于 glNamedBufferStorage，buffer 是要配
// 置的缓冲区对象的名称。数据存储的大小由 size 指定。如果初始数据可用，其地址可以通过 data
// 提供。否则，若要创建未初始化的数据存储，data 应为 NULL。如果 data 为 NULL，仍会创建
// 指定大小的数据存储，但其内容保持未初始化状态，因此是未定义的。错误和触发条件：
//      GL_INVALID_ENUM         glBufferStorage 的 target 不是可接受的缓冲区目标之一
//      GL_INVALID_OPERATION    glNamedBufferStorage 的 buffer 不是现有缓冲区对象的名称
//      GL_INVALID_VALUE        size 小于或等于零
//      GL_INVALID_OPERATION    glBufferStorage 绑定的目标缓冲区对象名称为保留的 0
//      GL_OUT_OF_MEMORY        GL 无法创建具有 flags 中请求属性的数据存储
//      GL_INVALID_VALUE        flags 设置了除合法定义之外的任何位
//      GL_INVALID_VALUE        flags 包含 GL_MAP_PERSISTENT_BIT，但不包含 GL_MAP_READ_BIT 或 GL_MAP_WRITE_BIT 中的至少一个
//      GL_INVALID_VALUE        flags 包含 GL_MAP_COHERENT_BIT，但不同时包含 GL_MAP_PERSISTENT_BIT
//      GL_INVALID_OPERATION    glBufferStorage 绑定到 target 的缓冲区的 GL_BUFFER_IMMUTABLE_STORAGE 标志已为 GL_TRUE（即已不可变）
//
// flags 参数指定缓冲区数据存储的预期用途。它必须是以下标志的子集的按位组合，标志允许的
// 组合受到以下限制：如果 flags 包含 GL_MAP_PERSISTENT_BIT，则它还必须包含 GL_MAP_READ_BIT
// 或 GL_MAP_WRITE_BIT 中的至少一个；如果 flags 包含 GL_MAP_COHERENT_BIT，则它还必须
// 包含 GL_MAP_PERSISTENT_BIT。
//      GL_DYNAMIC_STORAGE_BIT 数据存储的内容可以在创建后通过调用 glBufferSubData 更新。如果未设置此位，缓
//          冲区内容可能无法由客户端直接更新。无论是否设置 GL_DYNAMIC_STORAGE_BIT，data 参数都可用于指定缓
//          冲区数据存储的初始内容。无论此位是否存在，缓冲区始终可以通过服务器端调用（如 glCopyBufferSubData
//          和 glClearBufferSubData）进行更新。
//      GL_MAP_READ_BIT 数据存储可以由客户端映射以进行读取访问，并获得客户端地址空间中的指针，可以从中读取。
//      GL_MAP_WRITE_BIT 数据存储可以由客户端映射以进行写入访问，并获得客户端地址空间中的指针，可以通过其写入。
//      GL_MAP_PERSISTENT_BIT 客户端可以请求服务器在缓冲区映射时对其进行读取或写入。只要数据存储处于映射状态，
//          客户端指向数据存储的指针就保持有效，即使在执行绘制或调度命令期间也是如此。
//      GL_MAP_COHERENT_BIT 只要使用 glMapBufferRange 执行映射，客户端访问映射且服务器同时使用的缓冲区的共
//          享访问将是一致的。也就是说，客户端或服务器写入存储的数据将立即对另一方可见，应用程序无需采取进一步
//          操作。具体而言：
//          • 如果未设置 GL_MAP_COHERENT_BIT 且客户端执行写入，随后调用 glMemoryBarrier 命令并设置
//            GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT，则在后续命令中服务器将看到写入。
//          • 如果设置了 GL_MAP_COHERENT_BIT 且客户端执行写入，则在后续命令中服务器将看到写入。
//          • 如果未设置 GL_MAP_COHERENT_BIT 且服务器执行写入，应用程序必须调用 glMemoryBarrier 并设置
//            GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT，然后调用 glFenceSync 并设置 GL_SYNC_GPU_COMMANDS_COMPLETE
//            （或 glFinish）。同步完成后，CPU 将看到写入。
//          • 如果设置了 GL_MAP_COHERENT_BIT 且服务器执行写入，应用程序必须调用 glFenceSync 并设置
//            GL_SYNC_GPU_COMMANDS_COMPLETE（或 glFinish）。同步完成后，CPU 将看到写入。
//      GL_CLIENT_STORAGE_BIT 当满足缓冲区存储分配的所有其他条件时，实现可以使用此位来确定是使用服务器本地还
//          是客户端本地的存储作为缓冲区的后备存储（the backing store for the buffer）。
//
// 可变存储（glBufferData）和不可变存储（glBufferStorage）的对比：
//      大小可变性  可重新分配（再次调用即删除旧存储）     大小固定，不可重新分配
//      持久映射    不支持                              支持（GL_MAP_PERSISTENT_BIT）
//      一致性映射  不支持                              支持（GL_MAP_COHERENT_BIT）
//      客户端存储  不支持                              可选（GL_CLIENT_STORAGE_BIT）
//      直接更新    始终允许                            需显式设置 GL_DYNAMIC_STORAGE_BIT
//      性能        良好                               更优（驱动可做出更强优化假设）
//      适用场景    通用、简单场景                       高性能、多线程、CPU-GPU 协作、持久映射场景
//
// 传统映射（glBufferData）
//      每帧循环：
//        glMapBufferRange → 写入 → glUnmapBuffer
//             ↑___________________________↑
//                  高频率 CPU-GPU 同步开销
//
// 持久映射（glBufferStorage）
//      初始化一次：
//        glNamedBufferStorage(..., PERSISTENT)
//        ptr = glMapNamedBufferRange(..., PERSISTENT)
//      每帧循环：
//        直接写入 ptr
//        glMemoryBarrier(CLIENT_MAPPED_BUFFER)
//             ↑ 可选，非相干模式需要
//      清理时：
//        glUnmapNamedBuffer // 仅程序结束时
//
// 初始化顶点和片元着色器。对于每一个 OpenGL 程序，当它所使用的 OpenGL 版本高于或等于
// 3.1 时，需要绘制任何东西的程序都需要指定至少两个着色器：一个顶点着色器和一个片元着色
// 器。我们通过一个辅助函数 LoadShaders() 来实现这个要求，它需要传入一个 ShaderInfo 的
// 结构体数组，其中提供了着色器的类别和对于的着色器代码文件名称，数组的最后一个结构体为
// 空表示数组结束。对于 OpenGL 程序而言，目前而言着色器是使用 OpenGL 着色语言 GLSL 编
// 写的小型程序。我们可以以字符串的形式将 GLSL 着色器代码传给 OpenGL，但为了简化示例，
// 并且让读者更容易地使用着色器进行开发，我们选择将着色器字符串的内容保存到文件中，并且使
// 用 LoadShaders() 读取文件和创建 OpenGL 着色器程序。
//
// void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer); // 2.0+
// void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer); // 3.0+
// void glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, cosnt void *pointer); // 4.1+
//
// 我们基本完成了初始化工作，init() 剩最后两个函数用于指定顶点着色器的变量与我们存储在缓
// 存对象中的关系，这也就是所谓的着色器管线工程（shader plumbing），即将应用程序与着色
// 器之间，以及不同着色阶段之间的数据通道连接起来。为了将数据关联到我们的顶点着色器，也就
// 是 OpenGL 将要处理的所有顶点数据，需要在着色器中声明一个 in 变量，然后使用 glVertexAttribPointer
// 将它关联到一个顶点属性数组（vertex attribute array）。错误和触发条件：
//      GL_INVALID_VALUE        index 大于或等于 GL_MAX_VERTEX_ATTRIBS
//      GL_INVALID_VALUE        size 不是 1、2、3、4 或（对于 glVertexAttribPointer）GL_BGRA
//      GL_INVALID_ENUM         type 不是可接受的值
//      GL_INVALID_VALUE        stride 为负数
//      GL_INVALID_OPERATION    size 为 GL_BGRA 且 type 不是 GL_UNSIGNED_BYTE、GL_INT_2_10_10_10_REV 或 GL_UNSIGNED_INT_2_10_10_10_REV
//      GL_INVALID_OPERATION    type 为 GL_INT_2_10_10_10_REV 或 GL_UNSIGNED_INT_2_10_10_10_REV 且 size 不是 4 或 GL_BGRA
//      GL_INVALID_OPERATION    type 为 GL_UNSIGNED_INT_10F_11F_11F_REV 且 size 不是 3
//      GL_INVALID_OPERATION    glVertexAttribPointer 的 size 为 GL_BGRA 且 normalized 为 GL_FALSE
//      GL_INVALID_OPERATION    零绑定到 GL_ARRAY_BUFFER 缓冲区对象绑定点且 pointer 参数不为 NULL
//
// glVertexAttribPointer 定义通用顶点属性数据数组（generic vertex attribute data
// array）。参数 index 指定要修改的通用顶点属性的索引。size 指定每个通用顶点属性的分量
// 数（初始值为 4），必须是 1、2、3、4。此外，glVertexAttribPointer 接受符号常量 GL_BGRA。
// type 指定数组中每个分量的数据类型（初始值为 GL_FLOAT），glVertexAttribPointer 和
// glVertexAttribIPointer 接受 GL_BYTE、GL_UNSIGNED_BYTE、GL_SHORT、GL_UNSIGNED_SHORT、
// GL_INT 和 GL_UNSIGNED_INT。此外，glVertexAttribPointer 还接受 GL_HALF_FLOAT、GL_FLOAT、
// GL_DOUBLE、GL_FIXED、GL_INT_2_10_10_10_REV、GL_UNSIGNED_INT_2_10_10_10_REV 和
// GL_UNSIGNED_INT_10F_11F_11F_REV（4.4+）。glVertexAttribLPointer 也接受 GL_DOUBLE，
// 且这是该函数 type 参数唯一接受的标记。
//
// 参数 normalized，对于 glVertexAttribPointer，指定定点数据值在访问时是否应被归一化
// （GL_TRUE）或直接转换作为定点值不进行归一化（GL_FALSE）。参数 stride 指定连续通用顶
// 点属性之间的字节偏移量（初始值为 0）。如果 stride 为 0，则通用顶点属性被理解为在数组
// 中紧密排列。参数 pointer 指定当前绑定到 GL_ARRAY_BUFFER 目标的缓冲区的数据存储中，
// 第一个通用顶点属性的第一个分量的偏移量（初始值为 0）。可以定义一个宏 BUFFER_OFFSET(offset)
// ((void *)offset) 来指定偏移量。在以往版本的 OpenGL 中并不需要这个宏，不过显著我们希
// 望使用它来设置数据在缓存对象中的偏移，而不是像 pointer 原型那样直接设置一个指向内存块
// 的指针。在 OpenGL 的早期版本中（3.1 版本之前），顶点属性数据可以直接保存在应用程序内
// 存中，而不一定是 GPU 的缓存对象，因此旧版本使用指针的形式也是合理的。
//
// 例如以下代码示例，vPosition 顶点着色器中输入变量的 location 值，size 是 2 表示数组
// 中每个顶点的元素数目（即 x 和 y 坐标），type 是 GL_FLOAT 表示元素类型是浮点，归一化
// 为 GL_FALSE 表示坐标值不限定在 [-1.0, 1.0] 归一化范围内，stride 为 0 表示紧密排列，
// pointer 为 0 因为数据是从缓存对象的第一个字节（地址为 0）开始的。
//      glBindVertexArray(vertex_array_object);
//      glBindBuffer(GL_ARRAY_BUFFER, vertex_array_buffer_object);
//      glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
//      glEnableVertexAttribArray(vPosition);
//
// 函数的三种变体对比
//      函数                    数据类型                归一化             着色器中使用
//      glVertexAttribPointer   整数或浮点              可选归一化          float、vec2/3/4
//      glVertexAttribIPointer  仅限整数                不归一化（强制）    int、ivec2/3/4、uint、uvec2/3/4
//      glVertexAttribLPointer  仅限双精度 (GL_DOUBLE)  不适用             double、dvec2/3/4
//
// glVertexAttribPointer、glVertexAttribIPointer 和 glVertexAttribLPointer 指定渲
// 染时使用的、位于索引 index 处的通用顶点属性数组的位置和数据格式。size 指定每个属性的
// 分量数，必须是 1、2、3、4 或 GL_BGRA。type 指定每个分量的数据类型，stride 指定从一
// 个属性到下一个属性的字节跨度，允许顶点和属性被打包到单个数组中或存储在单独的数组中。
// 对于 glVertexAttribPointer，如果 normalized 设置为 GL_TRUE，则表示以整数格式存储
// 的值在访问并转换为浮点数时，将被映射到范围 [-1,1]（对于有符号值）或 [0,1]（对于无符
// 号值）。否则，值将直接转换为浮点数而不进行归一化。
//
// 对于 glVertexAttribIPointer，仅接受整数类型 GL_BYTE、GL_UNSIGNED_BYTE、GL_SHORT、
// GL_UNSIGNED_SHORT、GL_INT、GL_UNSIGNED_INT。值始终保持为整数值。glVertexAttribLPointer
// 为与声明为 64 位双精度分量的着色器属性变量相关联的通用顶点属性数组指定状态。type 必须
// 是 GL_DOUBLE。index、size 和 stride 的行为与 glVertexAttribPointer 和 glVertexAttribIPointer
// 的描述相同。
//
// 如果 pointer 不为 NULL，则必须有一个非零命名缓冲区对象绑定到 GL_ARRAY_BUFFER 目标
// （参见 glBindBuffer），否则会生成错误。pointer 被视为缓冲区对象数据存储中的字节偏移
// 量。缓冲区对象绑定（GL_ARRAY_BUFFER_BINDING）被保存为索引 index 的通用顶点属性数组
// 状态（GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING）。
//
// 当指定通用顶点属性数组时，size、type、normalized、stride 和 pointer 以及当前顶点数
// 组缓冲区对象绑定都被作为顶点数组状态保存。要启用和禁用通用顶点属性数组，请使用 index
// 调用 glEnableVertexAttribArray 和 glDisableVertexAttribArray。如果启用，则在调用
// glDrawArrays、glMultiDrawArrays、glDrawElements、glMultiDrawElements 或 glDrawRangeElements
// 时使用该通用顶点属性数组。每个通用顶点属性数组初始处于禁用状态，在调用 glDrawElements、
// glDrawRangeElements、glDrawArrays、glMultiDrawArrays 或 glMultiDrawElements 时
// 不会被访问。
//
// 获取对应的顶点属性信息：
//      glGet(GL_MAX_VERTEX_ATTRIBS)
//      glGet(GL_ARRAY_BUFFER_BINDING)
//      glGetVertexAttrib(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED)
//      glGetVertexAttrib(index, GL_VERTEX_ATTRIB_ARRAY_SIZE)
//      glGetVertexAttrib(index, GL_VERTEX_ATTRIB_ARRAY_TYPE)
//      glGetVertexAttrib(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED)
//      glGetVertexAttrib(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE)
//      glGetVertexAttrib(index, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING)
//      glGetVertexAttribPointerv(index, GL_VERTEX_ATTRIB_ARRAY_POINTER)
//
// void glEnableVertexAttribArray(GLuint index); // 2.0+
// void glEnableVertexArrayAttrib(GLuint vaobj, GLuint index); // 4.5+
// void glDisableVertexAttribArray(GLuint index); // 2.0+
// void glDisableVertexArrayAttrib(GLuint vaobj, GLuint index); // 4.5+
//
// 启用或禁用通用顶点属性数组。glEnableVertexAttribArray 和 glEnableVertexArrayAttrib
// 启用由 index 指定的通用顶点属性数组。glEnableVertexAttribArray 使用当前绑定的顶点数
// 组对象进行操作，而 glEnableVertexArrayAttrib 更新 ID 为 vaobj 的顶点数组对象的状态。
// glDisableVertexAttribArray 和 glDisableVertexArrayAttrib 禁用由 index 指定的通用
// 顶点属性数组。glDisableVertexAttribArray 使用当前绑定的顶点数组对象进行操作，而
// glDisableVertexArrayAttrib 更新 ID 为 vaobj 的顶点数组对象的状态。默认情况下，所有
// 客户端功能都被禁用，包括所有通用顶点属性数组。如果启用，则在调用顶点数组命令（如 glDrawArrays、
// glDrawElements、glDrawRangeElements、glMultiDrawElements 或 glMultiDrawArrays）
// 时，将访问并使用通用顶点属性数组中的值进行渲染。
//
// 错误和触发条件：
//      GL_INVALID_OPERATION glEnableVertexAttribArray 和 glDisableVertexAttribArray
//          在没有绑定顶点数组对象时调用。
//      GL_INVALID_OPERATION glEnableVertexArrayAttrib 和 glDisableVertexArrayAttrib
//          的 vaobj 不是现有顶点数组对象的名称。
//      GL_INVALID_VALUE，index 大于或等于 GL_MAX_VERTEX_ATTRIBS，最大属性数由 GL_MAX_VERTEX_ATTRIBS
//          决定，通常至少 16。因此 index 必须是一个 0 到 GL_MAX_VERTEX_ATTRIBS-1 之
//          间的值。
//
// void glClear(GLbitfield mask); // 2.0+
// void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha); // 2.0+
// void glClearDepth(GLdouble depth); // 2.0+
// void glClearDepthf(GLfloat depth); // 4.1+
// void glClearStencil(GLint s); // 2.0+
// void glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLfloat stencil); // 3.0+
// void glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value); // 3.0+
// void glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value); // 3.0+
// void glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value); // 3.0+
// void glClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLfloat stencil); // 4.5+
// void glClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value); // 4.5+
// void glClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value); // 4.5+
// void glClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value); // 4.5+
//
// glClear 将缓冲区清除为预设值，掩码表示要清除的缓冲区 GL_COLOR_BUFFER_BIT、
// GL_DEPTH_BUFFER_BIT 和 GL_STENCIL_BUFFER_BIT。glClear 将窗口的位平面区域设置为之
// 前由 glClearColor、glClearDepth 和 glClearStencil 选定的值。通过调用 glDrawBuffer
// 可以同时清除选多个颜色缓冲区（multiple color buffers）。像素所有权测试（pixel ownership
// test）、裁剪测试（scissor test）、抖动（dithering）和缓冲区写入掩码（buffer writemasks）
// 会影响 glClear 的操作。裁剪框（scissor box）限定清除区域。Alpha 函数、混合（blend）
// 函数、逻辑运算、模板测试（stenciling）、纹理映射（texture mapping）和深度缓冲（depth
// buffering）都会被 glClear 忽略。glClear 接受一个参数，该参数是多个值的按位或，指示
// 要清除哪个缓冲区，每个缓冲区清除到的值取决于该缓冲区的清除值设置。如果某个缓冲区不存在，
// 则针对该缓冲区的 glClear 调用不会产生任何效果。如果 mask 中设置了除上述三个定义位之外
// 的任何位，则生成 GL_INVALID_VALUE 错误。
//      GL_COLOR_BUFFER_BIT 当前已启用用于颜色写入的缓冲区
//      GL_DEPTH_BUFFER_BIT 深度缓冲区
//      GL_STENCIL_BUFFER_BIT 模板缓冲区
//
// 获取相关清除值值：
//      glGet(GL_DEPTH_CLEAR_VALUE)
//      glGet(GL_COLOR_CLEAR_VALUE)
//      glGet(GL_STENCIL_CLEAR_VALUE)
//
// glClearColor 设置颜色缓冲区的清除值，指定清除颜色缓冲区时使用的红、绿、蓝和 Alpha
// 值。初始值为 0 表示黑色。glClearColor 指定 glClear 清除颜色缓冲区时使用的红、绿、蓝
// 和 Alpha 值。glClearColor 指定的值会被钳制到 [0,1] 范围内。注意，red、green、blue
// 和 alpha 参数的类型已从 GLclampf 更改为 GLfloat。此更改对用户代码是透明的，详细说明
// 见 removedTypes 页面。清除颜色本身是 OpenGL 状态机制的一个例子，它的数值会一直保留
// 在当前 OpenGL 环境中。OpenGL 有一个庞大的状态列表（见附录），当创建一个新的 OpenGL
// 环境时，所有状态都会被初始化为默认值。因为 OpenGL 会保留所有更改的状态，所有我们可以
// 减少设置状态值的次数。
//
// glClearDepth 设置深度缓冲区的清除值，参数 depth 指定清除深度缓冲区时使用的深度值，
// 初始值为 1。glClearDepth 指定 glClear 清除深度缓冲区时使用的深度值。glClearDepth
// 指定的值会被钳制到 [0,1] 范围内。注意，glClearDepthf 的 depth 参数类型已从 GLclampf
// 更改为 GLfloat，glClearDepth 的 depth 参数类型已从 GLclampd 更改为 GLdouble。此更
// 改对用户代码是透明的，详细说明见 removedTypes 页面。
//
// glClearStencil 设置模板缓冲区的清除值，参数 s 指定清除模板缓冲区时使用的索引值，初始
// 值为 0。glClearStencil 指定 glClear 清除模板缓冲区时使用的索引值。s 会与 2^m − 1
// 进行掩码运算，其中 m 是模板缓冲区的位数。
//
// glClearBuffer 清除帧缓冲中的单个缓存，参数 framebuffer 指定 glClearNamedFramebuffer*
// 的帧缓冲区对象名称，buffer 指定要清除的缓存，drawbuffer 指定要清除的特定绘制缓冲区，
// value 指向用于清除缓冲区的值或值组的指针，depth 清除深度缓冲区所使用的值，stencil
// 清除模板缓冲区所使用的值。
//
// 这些命令将帧缓冲的指定缓存清除为指定值，对于 glClearBuffer*，帧缓冲区为当前绑定的绘
// 制帧缓冲区对象。对于 glClearNamedFramebuffer*，framebuffer 为 0（表示默认绘制帧缓
// 冲区）或帧缓冲区对象的名称。buffer 和 drawbuffer 标识要清除的缓冲区：
//
// 如果 buffer 为 GL_COLOR，则通过将 i 作为 drawbuffer 传递来指定特定的绘制缓冲区 GL_DRAW_BUFFERi，
// value 指向一个四元素向量，指定用于清除该绘制缓冲区的 R、G、B 和 A 颜色值。如果 GL_DRAW_BUFFERi
// 的值为 GL_NONE，则该命令不产生任何效果。否则，GL_DRAW_BUFFERi 的值标识一个或多个颜
// 色缓冲区，每个缓冲区都被清除为相同的值。定点颜色缓冲区的钳制和类型转换与 glClearColor
// 执行方式相同。应使用这些命令的 *fv、*iv 和 *uiv 形式分别清除定点和浮点、有符号整数和
// 无符号整数颜色缓冲区。
//
// 如果 buffer 为 GL_DEPTH，则 drawbuffer 必须为 0，且 value 指向用于清除深度缓冲区的
// 单个值。定点深度缓冲区的钳制和类型转换与 glClearDepth 执行方式相同。应仅使用这些命令
// 的 *fv 形式清除深度缓冲区；其他形式不接受 GL_DEPTH 缓冲区。
//
// 如果 buffer 为 GL_STENCIL，则 drawbuffer 必须为 0，且 value 指向用于清除模板缓冲区
// 的单个值。掩码运算与 glClearStencil 执行方式相同。应仅使用这些命令的 *iv 形式清除模
// 板缓冲区；其他形式不接受 GL_STENCIL 缓冲区。
//
// glClearBufferfi 和 glClearNamedFramebufferfi 用于同时清除深度和模板缓冲区。buffer
// 必须为 GL_DEPTH_STENCIL，且 drawbuffer 必须为 0。depth 和 stencil 分别为清除深度
// 和模板缓冲区所使用的值。定点深度缓冲区的深度钳制和类型转换与 glClearDepth 执行方式相
// 同。模板缓冲区的模板掩码运算与 glClearStencil 执行方式相同。这些命令等效于分别清除深
// 度和模板缓冲区，但在清除内部格式为 GL_DEPTH_STENCIL 的缓冲区时可能更快。glClear 定义
// 的相同逐片段和掩码操作均适用。
//
// 如果指定的值类型与要清除的缓冲区类型之间未定义转换（例如，对定点或浮点缓冲区调用 glClearBufferiv，
// 或对有符号或无符号整数缓冲区调用 glClearBufferfv），则这些命令的结果未定义。这不是一
// 个错误。错误触发条件：
//      如果 framebuffer 不为 0 且不是现有帧缓冲区对象的名称，则 glClearNamedFramebuffer* 生成 GL_INVALID_OPERATION。
//      如果 buffer 不是 GL_COLOR 或 GL_STENCIL，则 glClearBufferiv 和 glClearNamedFramebufferiv 生成 GL_INVALID_ENUM。
//      如果 buffer 不是 GL_COLOR，则 glClearBufferuiv 和 glClearNamedFramebufferuiv 生成 GL_INVALID_ENUM。
//      如果 buffer 不是 GL_COLOR 或 GL_DEPTH，则 glClearBufferfv 和 glClearNamedFramebufferfv 生成 GL_INVALID_ENUM。
//      如果 buffer 不是 GL_DEPTH_STENCIL，则 glClearBufferfi 和 glClearNamedFramebufferfi 生成 GL_INVALID_ENUM。
//      如果 buffer 为 GL_COLOR 且 drawbuffer 为负数，或大于 GL_MAX_DRAW_BUFFERS 减 1 的值，则生成 GL_INVALID_VALUE。
//      如果 buffer 为 GL_DEPTH、GL_STENCIL 或 GL_DEPTH_STENCIL 且 drawbuffer 不为 0，则生成 GL_INVALID_VALUE。
//
// void glDrawArrays(GLenum mode, GLint first, GLsizei count); // 2.0+
//
// 最后我们选择准备要绘制的顶点数组（通过 glBindVertexArray）作为顶点数据，然后调用
// glDrawArrays 将顶点数据实际发送给 OpenGL 渲染管线。绘制最后调用 glFlush() 即强制
// 所有进行中的 OpenGL 命令立即传输到 OpenGL 服务端处理，在后文我们会将 glFlush 替换
// 为另一个更为平滑的命令，但需要进行更多的设置。在你的 OpenGL 编程生涯的某个时刻，你
// 可能会被问及或自问“这需要多少时间”？“这”可能是渲染一个物体，绘制一整个场景，或者 OpenGL
// 能够实现的其他操作。为了能够精确度量和执行自己的任务，我们有必要了解 OpenGL 是什么时
// 候完成这些操作的。这里的 glFlush 命令看起来像是一个正确的答案，但是它不是。事实上
// glFlush 只是强制所有运行中的命令送入 OpenGL 服务端而已，并且它会立即返回，它并不会
// 等待所有的命令完成，而等待却是我们所需要的。此时，我们可以调用 glFinish 命令，它会一
// 直等待所有当前的 OpenGL 操作完成后再返回。你最好只是在开发阶段使用 glFinish()，如果
// 你已经完成了开发的工作，那么最好去掉对这个命令的调用。虽然它对于判断 OpenGL 命令的运
// 行有帮助，但是对于程序的整体性能却有着相当的拖累。
//
// glDrawArrays 从数组数据渲染图元，参数 mode 指定要渲染的图元类型，可接受的符号常量包
// 括 GL_POINTS、GL_LINE_STRIP、GL_LINE_LOOP、GL_LINES、GL_LINE_STRIP_ADJACENCY（3.2+）、
// GL_LINES_ADJACENCY（3.2+）、GL_TRIANGLE_STRIP、GL_TRIANGLE_FAN、GL_TRIANGLES、
// GL_TRIANGLE_STRIP_ADJACENCY（3.2+）、GL_TRIANGLES_ADJACENCY（3.2+） 和 GL_PATCHES。
// 参数 first 指定已启用数组中的起始索引，参数 count 指定要渲染的索引数量。错误值：
//      如果 mode 不是可接受的值，则生成 GL_INVALID_ENUM。
//      如果 count 为负数，则生成 GL_INVALID_VALUE。
//      如果非零缓冲区对象名称绑定到已启用的数组，且该缓冲区对象的数据存储当前已被映射，则生成 GL_INVALID_OPERATION。
//      如果几何着色器处于活动状态，且 mode 与当前安装的程序对象中几何着色器的输入图元类型不兼容，则生成 GL_INVALID_OPERATION。
//
// glDrawArrays 仅需极少量的子程序调用即可指定多个几何图元。无需为传递每个单独的顶点、
// 法线、纹理坐标、边标志或颜色而调用 GL 过程，你可以预先指定独立的顶点、法线和颜色数组，
// 并通过单次调用 glDrawArrays 来使用它们构建一系列图元。调用 glDrawArrays 时，它从每
// 个已启用的数组中使用 count 个连续元素来构建一系列几何图元，从元素 first 开始。mode
// 指定构建何种图元以及数组元素如何构建这些图元。被 glDrawArrays 修改的顶点属性在
// glDrawArrays 返回后具有未指定的值，而未被修改的属性保持明确定义。
//
// void glFlush(void); // 2.0+
// void glFinish(void); // 2.0+
//
// glFlush 强制在有限时间内执行 GL 命令，不同的 GL 实现在多个不同位置缓冲命令，包括网络
// 缓存和图形缓存本身。glFlush 清空所有这些缓冲区，使所有已发出的命令在实际渲染引擎接受
// 后尽快执行。尽管此执行可能不在任何特定时间段内完成，但它确实在有限时间内完成。由于任何
// GL 程序可能通过网络执行，或在缓冲命令的加速器上执行，所有程序在依赖所有先前发出的命令
// 已完成时都应调用 glFlush。例如，在等待依赖于生成图像的用户输入之前调用 glFlush。注意，
// glFlush 可以随时返回，它不会等待所有先前发出的 GL 命令执行完毕。
//
// glFinish 阻塞直到所有 GL 执行完成，glFinish 直到所有先前调用的 GL 命令的效果完全完
// 成后才返回。这些效果包括所有 GL 状态的改变、所有连接状态的改变以及帧缓冲区内容的所有
// 改变。注意，glFinish 需要一次与服务器的往返通信。
//
// 着色器编程语言
//
// 现代 OpenGL 渲染管线严重依赖着色器处理传入的数据，如果不使用着色器，那么 OpenGL 可以
// 做的可能只有清除窗口内容了。在 OpenGL 3.0 及以前，如果用到兼容模式（compatibility
// profile）环境，OpenGL 还包含一个固定功能管线（fixed function pipeline），可以在不
// 使用着色器的情况下处理几何与像素数据。但从 3.1 版本开始，固定功能管线从核心模式中去掉
// 了，此时必须使用着色器来完成工作。
//
// 无论是 OpenGL 还是其他图形接口的着色器，通常都是通过一种特殊的编程语言来编写。对于
// OpenGL 就是 GLSL（OpenGL Shading Language），虽然 GLSL 是一种专门为图形开发设计的
// 编程语言，但是它与 C 语言非常类似。这里我们将介绍 GLSL 并讨论如何编译着色器将其集成
// 到你的应用程序中，以及如何将应用程序中的数据在不同着色器之间传递。一个重要概念是，着色
// 器之间数据传输的方式。着色器类似一个函数调用的方式，数据传输进来，经过处理，然后再传输
// 出去。例如 C 语言中，这一过程可以通过全局变量或者函数参数来完成。GLSL 与之稍有差异，每
// 个着色器都像是一个完整的 C 程序，它的输入点就是主函数，GLSL 的主函数没有任何参数，在
// 某个着色阶段中输入和输出的所有数据都是通过着色器中的特殊全局变量来传递的。请不要将它们
// 与应用中的全局变量相混淆，着色器变量与你在应用程序代码中声明的变量是完全不相干的。
//
// 一个简单的顶点着色器如下，每个变量除了有类型（vec4、mat4），还有 in 表示将数据拷贝到
// 着色器，out 表示将着色器的内容拷贝出去，uniform 直接从 OpenGL 应用程序中接收数据。
// 输入输出变量的值会在 OpenGL 每次执行着色器的过程中更新，而 uniform 变量不会随着顶点
// 或片元的变化而变化，它对于所有的几何体图元都是一样的，除非应用程序对它进行了更新。
//
//      #version 430 core
//
//      in vec4 vertex_position;
//      in vec4 vertex_color;
//      out vec4 color;
//      uniform mat4 model_view_projection_matrix;
//
//      void main() {
//          color = vertex_color;
//          gl_Position = model_view_projection_matrix * vertex_position;
//      }
//
// GLSL 行注释 // 或块注释 /* */，主函数没有参数也没有返回值，每条语句都以分号（;）结束。
// GLSL 是一种强类型语言，所有变量都必须事先声明，并且给出变量的类型。变量命名规范与 C
// 语言相同，由字母、数字、下划线组成，但是数字和下划线不能作为变量名称的第一个字符，另外
// 变量名称不能包含连续的下划线（这些名称被 GLSL 保留使用）。GLSL 支持的基本数据类型，
// 布尔型（bool），32 位有符号（int），32 位无符号（uint），单精度浮点（float），双精度
// 浮点（double），这些类型以及它们的聚合类型，都称为透明类型。与之对应的不透明类型，它们
// 的内部形式是不透明的，例如采样器（sampler）、图像（image）、以及原子计数器（atomic
// counter）。它们所声明的变量相当于一个不透明的句柄，可以用来读取纹理贴图、图像、以及原
// 子计数器数据。
//
// 变量的作用域。在任何函数定义之外的变量拥有全局作用域，对着色器程序中的所有函数都是可见
// 的。在一组大括号内声明的变量，只能在大括号范围内存在。循环的迭代自变量，只在循环体内起
// 作用。所有变量都必须在声明的同时进行初始化。整型字面量可以是八进制、十进制、或十六进制
// 的值，可以在数字之前加一个符号表示负数，或者在末尾添加 u 或者 U 表示一个无符号整数。浮
// 点字面量必须包含一个小数点，除非使用科学记数法。此外，浮点数也可以选择在末尾添加 f 或
// F，要表达双精度浮点数，必须在末尾添加 lF 或者 LF。布尔值可以是 true 或者 false。GLSL
// 中的隐式转换，int 可以隐式转换为 uint 类型，int uint 可以隐式转换为 float，int uint
// float 可以隐式转换为 double。这些类型转换适用于这些类型的标量、向量、以及矩阵，类型转
// 换不能应用于数组或结构体之上。所有其他的数值转换都需要提供显式的转换构造函数，这里构造
// 函数的意义与 C++ 等语言类似，它是一个名字与类型名称相同的函数，返回值就是对应类型的值。
// 例如 float f = 10.0; int ten = int(f); 此外其他一些类型也有转换构造函数，包括 float
// double uint bool 以及这些类型的向量和矩阵。每种构造函数都可以传入多个其他类型的并且进
// 行显式转换，这也体现了 GLSL 的另一个特性，函数可以重载，即每个函数都可以接受不同类型的
// 输入，但是它们都使用相同的一个函数名称。
//
// 聚合类型。GLSL 的基本类型可以进行合并，从而与核心 OpenGL 的数据类型相匹配，以及简化计
// 算过程。首先 GLSL 支持 2、3、4 分量的向量，每个分量都可以使用 bool int uint float
// double 这些基本类型。此外，GLSL 也支持 float 和 double 类型的矩阵，下表给出了所有可
// 用的向量和矩阵类型。矩阵类型需要给出两个维度信息，例如 mat4x3 四列三行矩阵，其中第一个
// 值表示列数，第二个值表示行数。
//      基本类型    2D向量      3D向量      4D向量      矩阵类型
//      float       vec2        vec3        vec4        mat2    mat3    mat4    mat2x2  mat2x3  mat2x4
//                                                      mat3x2  mat3x3  mat3x4  mat4x2  mat4x3  mat4x4
//      double      dvec2       dvec3       dvec4       dmat2   dmat3   dmat4   dmat2x2 dmat2x3 dmat2x4
//                                                      dmat3x2 dmat3x3 dmat3x4 dmat4x2 dmat4x3 dmat4x4
//      int         ivec2       ivec3       ivec4
//      uint        uvec2       uvec3       uvec4
//      bool        bvec2       bvec3       bvec4
//
// 使用这些类型声明的变量的初始化过程与它们的标量部分是类似的，类型之间也可以进行等价转换，
// 向量的构造函数还可以用来截短或者加长一个向量。矩阵的构建方式与此相同，并且可以将它初始
// 为一个对角矩阵或者完全填充的矩阵。对于对角矩阵，只需要向构造函数传递一个值，矩阵的对角
// 线元素就设置为这个值，其他元素全部设置为 0。矩阵也可以通过在构造函数中指定每一个元素的
// 值来构建，传入元素可以是标量和向量的集合，只要给定足够数量的数据即可，每一列的设置方式
// 也遵循这样的原则。此外，矩阵的指定需要遵循列的主序原则，也就是，传入的数据将首先填充列，
// 然后填充行。这一点与 C 语言的多维数组刚好相反，C 语言的多维数组是以行优先。
//      vec3 velocity = vec3(0.0, 2.0, 3.0);
//      ivec3 steps = ivec3(velocity);
//      vec4 color;
//      vec3 rgb = vec3(color); // 保留前三个分量
//      vec3 white = vec3(1.0); // 三个分量都是 1.0
//      vec4 trans = vec4(white, 0.5);
//      mat3 m = mat3(4.0); // 3x3 矩阵，对角线都为 4.0，其余元素都为 0
//      mat3 m = mat3(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
//      vec3 column1 = vec3(1.0, 2.0, 3.0);
//      vec3 column2 = vec3(4.0, 5.0, 6.0);
//      vec3 column3 = vec3(7.0, 8.0, 9.0);
//      mat3 m = mat3(column1, column2, column3)
//      vec2 column1 = vec2(1.0, 2.0);
//      vec2 column2 = vec2(4.0, 5.0);
//      vec2 column3 = vec2(7.0, 8.0);
//      mat3 m = mat3(column1, 3.0, column2 6.0, column3, 9.0);
//
// 访问向量和矩阵中的元素。向量与矩阵中的元素可以单独访问和设置，支持名称和下标两种访问
// 形式。与位置相关的向量分量 (x, y, z, w)，与颜色相关的向量分量 (r, g, b, a)，与纹理
// 坐标相关的向量分量 (s, t, p, q)。还可以重复一个向量分量多次来形成一个多分量的值，这
// 种方式称为调换重排（swizzle）。
//      float red = color.r;
//      float v_y = velocity.y;
//      float red = color[0];
//      float v_y = velocity[1];
//      vec3 luminance = color.rrr;
//      color = color.abgr; // 反转分量
//      mat4 m = mat4(2.0);
//      vec4 zvec = m[2]; // 第三列
//      float yscale = m[1][1] 第二列第二行
//      float yscale = m[1].y
//
// 结构体可以简化多组数据传入函数的过程，如果定义一个结构体，那么会自动创建一个新类型，并
// 且隐式定义了一个构造函数，将各种类型的结构体元素作为输入参数。GLSL 还支持任意类型的数
// 组，包括结构体数组。GLSL 4.3 种，数组的组成元素也可以是另一个数组，因此可以处理多维数
// 据。数组可以定义为有大小的，或者没有大小的。我们可以使用没有大小的数组作为一个数组变量
// 的前置声明，然后重新用一个合适的大小来声明它。数组属于 GLSL 种的第一等类型，也就是说它
// 有构造函数，并且可以用作函数的参数和返回类型。此外，GLSL 的数组与 Java 类型，它有一个
// 隐式方法返回元素的个数 length()。向量和矩阵类型也可以使用 length() 方法，向量的长度就
// 是它包含的分量个数，矩阵的长度是它包含的列的个数。因为长度值在编译时是已知的，length()
// 返回一个编译时常量，可以在需要使用常量的场合直接使用它。对于所有向量和矩阵，以及大部分
// 的数组来说，length() 都是一个编译时已知的常量。但是对于某些数组来说，length() 的值在
// 连接之前可能都是未知的，如果使用链接器来减少同一阶段种多个着色器的大小，那么可能发生这
// 种情况。对于着色器种保存的缓存对象（使用 buffer 进行声明），length() 的值直到渲染时才
// 可能得到。如果我们需要 length() 返回一个编译时常量，那么我们需要保证着色器的数组大小在
// 使用它的 length() 方法之前就以及确定了。多维数组相当于从数组种再创建数组，它的语言与C
// 语言当中类似，多维数组可以使用任何类型或者形式来构成。如果需要与应用程序共享，那么最内层
// （即最右侧）维度的数据在内存布局中的变化是最快的，GLSL（以及 C/C++）使用行优先（Row-Major）
// 存储，即最右侧的索引变化最快。
//      struct particle { float lifetime; vec3 position; vec3 velocity; };
//      particle p = particle(10.0, pos, vel);
//      float coeff[3];
//      float[3] coeff; // 与上面定义等价
//      int indices[]; // 长度未定义，稍后可以重新声明它的长度
//      float coeff[3] = float[3](2.38, 3.14, 42.0);
//      float coeff[3] = float(2.38, 3,14, 42.0); // 这里构造函数的长度值可以不填
//      for (int i = 0; i < coeff.length(); ++i) { coeff[i] *= 2.0; }
//      mat3x4 m;
//      int c = m.length(); // 3
//      int r = m[0].length(); // 4
//      mat4 m;
//      float diagonal[m.length()]; // 4
//      float x[gl_in.length()];
//      float coeff[3][5]; // 有3个数组，每个数组有5个元素
//      coeff[2][1] *= 2.0;
//      coeff.length(); // 3
//      coeff[2]; // 长度为5的数组
//      coeff[2].length(); // 5
//
// 存储限定符。数据类型还可以通过一些修饰符来改变自己的行为，GLSL 定义以下多种全局范围内
// 的修饰符。如果需要在应用程序中共享一大块缓存给着色器，那么最好的方式是使用 buffer 变量。
// 它与 uniform 变量非常类似，不过也可以用着色器对它的内容进行修改。通常来说，需要在一个
// buffer 块中使用 buffer 变量，后面将对“块”的概念进行介绍。buffer修饰符指定随后作为着
// 色器与应用程序共享的一块内存缓存，这块内存缓存对于着色器来说是可读也是可写的，缓存的大
// 小可以在着色器编译和程序连接完成后设置。shared 修饰符只能用于计算着色，它可以建立本地
// 工作组内共享内存。
//      const   将一个变量定义为只读，如果用常量初始化，那么它本身也会变成编译时常量
//      in      设置变量为着色阶段的输入变量，可以是输入的顶点属性，或前一个着色器的输出变量
//      out     设置变量为着色阶段的输出变量，如顶点着色输出变换后的齐次坐标，片元着色输出的片元颜色
//      uniform 用户应用程序传递给着色器的数据，它对于给定的图元而言是一个常量
//      buffer  与应用程序共享的一块可读写内存，该内存也作为着色器的存储缓存使用（shader storage buffer）
//      shared  设置变量在本地工作组（local work group）中共享，它仅用在计算着色器中（compute shader）
//
// GLSL 提供了大量的操作符，下表从高优先级到低优先级列出了操作符表。总体上来说，操作符对应
// 的类型必须时相同的，并且对于向量和矩阵而言，操作符的操作对象也必须是同一维度的。下表注明
// 的整型包括 int uint 和对应的向量，浮点类型包括 float double 以及对应的向量和矩阵，算术
// 类型包含所有的整数和浮点类型、以及所有相关的结构体和数组。GLSL 中的大部分操作符都是经过
// 重载的，也就是说它们可以应用于多种类型的数据操作。特别是，矩阵和向量的算术操作符，包括前置
// 和后置的递增递减操作符，在 GLSL 中都是经过严格定义的。例如，如果我们需要进行向量和矩阵之
// 间的乘法（注意矩阵乘法不遵循交换律），可以使用下面的操作。基本的限制条件是要求矩阵和向量
// 的维度是匹配的。此外，也可以对向量或矩阵执行标量乘法。一个必须要提交的例外是，两个向量相
// 乘得到的是一个逐分量相乘的新向量，但是两个矩阵相乘得到的是通常矩阵相乘的结果。我们还可以
// 函数调用的方式实现场景的一些向量操作（如点乘和叉乘），以及各种逐分量执行的向量和矩阵操作。
//      vec3 v; mat3 m; vec3 result = v * m;
//      vec2 a, b, c; mat m, u, v;
//      c = a * b; // a.x * b.x, a.y * b.y
//      m = u * v;
//
// 操作符       可用类型            描述
//  ()          括号               分组
// -------------------------------------------------
//  []          数组、矩阵、向量    数组下标
//  f()         函数               函数调用，构造函数
//  .           结构体             成员变量或方法
//  ++ --       算术类型           后置递增和递减
// -------------------------------------------------
//  ++ --       算术类型           前置递增和递减
//  + -         算术类型           一元正和负
//  ~           整型              一元按位反
//  !           布尔型             一元逻辑非
// --------------------------------------------------
//  * / %       算术类型           乘法运算
//  + -         算术类型           加法运算
//  << >>       整型               位移
//  < <= > >=   算术类型           大小比较
//  == !=       任意               相等比较
//  &           整型               按位与
//  ^           整型               按位异或
//  |           整型               按位或
//  &&          布尔型             逻辑与
//  ^^          布尔型             逻辑异或
//  ||          布尔型             逻辑或
//  -------------------------------------------------
//  a ? b : c   布尔型?任意:任意    三元选择
//  -------------------------------------------------
//  =           任意                赋值
//  += -=       算术类型
//  *= /=       算术类型
//  %= <<= >>=  整型
//  &= ^= |=    整型
//  -------------------------------------------------
//  ,           任意
//
// GLSL 控制流 if else switch case 与 C 语言一致，循环语句 for while do while 也与
// C 语言一致。GLSL 有一个特殊的流控制语句 discard，它丢弃当前的片元，终止着色器执行。
// discard 语句只适用于片元着色器中，片元着色器的运行会在 discard 语句的位置上立即终止，
// 不过这也取决于具体的硬件实现。GLSL 支持用户自定义函数，同时也定义了一些内置函数，用户
// 自定义函数可以在单个着色器对象中定义，然后在多个着色器程序中复用。函数声明语法与 C 语
// 言非常类似，只是变量名需要添加访问修饰符。函数名称可以是任何字符、数字、和下划线字符的
// 组合，但是不能以数字、连续下划线、或者 gl_ 作为函数名称的开始。返回值可以是任何内置的
// GLSL 类型，或者用户定义的结构体和数组类型，当为数组时，必须显式指定其大小。函数的参数
// 也可以是任何类型，包括数组（但也必须设置数组的大小）。在使用一个函数之前，必须声明它的
// 原型或者直接给出函数体。GLSL 没有指针和引用的概念，不过与之对应的，函数参数可以指定一个
// 参数限定符，来表明它是否需要在函数运行时将数据拷贝到函数中，或者从函数返回修改的数据。
// 如果需要在编译时验证函数是否修改某个输入变量，可以设置 const in 修饰符来阻止函数对变量
// 进行修改。如果不这么做，那么在函数中写入一个 in 类型的变量，相当于对变量的局部拷贝进行
// 修改，因此只在函数自身范围内产生作用。
//      访问修饰符      描述
//      in              将数据拷贝到函数中，如果没有指定修饰符，默认为 in
//      const in        将只读数据拷贝到函数中
//      out             从函数中获取数据，因此输入函数的值是未定义的
//      inout           将数据拷贝到函数中，并且返回函数中修改的数据
//
// 计算的不变性。GLSL 无法保证在不同的着色器中，两个完全相同的计算式得到完全一样的结果。
// 这一情形与 CPU 端应用程序进行计算时的问题相同，即不同的优化方式可能会导致结果非常细微
// 的差异。这些细微的差异对于多通道的算法会产生问题，因为各个着色器阶段可能需要计算得到完
// 全一致的结果。GLSL 有两种方法来确保着色器之间的计算不变性，即 invariant 或者 precise
// 关键字。这两种方法都需要在图形设备上完成计算过程，来确保同一表达式的结果可以保证重复性
// （不变性）。但是，对于宿主计算机和图形硬件各自的计算，这两种方法都无法保证结果是完全一
// 致的。着色器编译时的常量表达式是由编译器的宿主计算机计算的，因此我们无法保证宿主计算
// 的结果与图形硬件计算的结果完全相同。如以下示例，无论对任何一个变量设置 invariant 还是
// precise 限制符，结果都不会有任何改变，因为这两个关键字都只能影响图形设备的计算结果。
//      uniform float ten; // 假设应用程序设置这个值为 10.0
//      const float f = sin(10.0); // 宿主计算机编译器负责计算
//      float g = sin(ten); // 图形计算机负责计算
//      void main() { if (f == g); } // f 和 g 不一定相等
//
// invariant 限制符可以设置任何着色器的输出变量，它可以确保如果两个着色器的输出变量使用
// 了同样的表达式，并且表达式中的变量也是相同值，那么计算产生的结果也是相同的。可以将一个
// 内置的输出变量声明为 invariant，也可以声明一个用户自定义的变量为 invariant。输出变量
// 的作用是将一个着色器的数据从一个阶段传递到下一个，可以在着色器中用到某个变量（包括内置
// 变量）之前的任何位置对该变量设置关键字 invariant。标准的做法是只使用 invariant 来声
// 明这个变量，如下面对 gl_Position 的设置。在调试过程中，可能需要将着色器的所有可变量都
// 设置为 invariant，可以通过顶点着色器的预编译命令 #pragma STDGL invariant(all) 来实
// 现。全局都设置为 invariant 可以帮助我们解决调试问题，但是这对着色器的性能会有所影响，
// 而为了保证不变性，通常也会导致 GLSL 编译器所执行的一些优化工作被迫停止。
//      invariant gl_Position;
//      invariant centriod out vec3 color;
//
// precise 限制符可以设置任何计算中的变量或者函数的返回值，它的名字优点望文生义，它的用途
// 并不是增加数据精度，而是增加计算的可复用性。我们通常在细分着色器中用它来避免造成几何体
// 形状的裂缝。总体上说，如果必须保证某个表达式产生的结果是一致的，即使表达式中的数据发生
// 了变化（但数学上并不影响结果），那么此时应该使用 precise 而非 invariant。例如，下面的
// 表达式中，即使 a 和 b 的值发生了交换，得到的结果也是不变的。此外，即使 c 和 d 的值发生
// 了交换，或者 a 和 c 同时与 b 和 d 发生了交换等，都应该得到同样的计算结果。precise 可
// 以设置内置变量、用户变量、和函数返回值。在着色器中，关键字 precise 可以在使用某个变量
// 之前的任何位置上设置这个变量，并且可以修改之前已经声明过的变量。编译器使用 precise 的
// 一个实际影响是，类似下面的表达式不能再使用两种不同的乘法命令来同时参与计算。例如，第一
// 个乘法使用普通的乘法，而第二个乘法使用呼和乘加（fused multiply-and-add, fma）。这是
// 因为这两个命令对于同一组值的计算结果可能会存在微小的差异。而这种差异是 precise 所不允
// 许的，因此编译器会直接阻止。由于混合乘加运算对于性能的提升非常重要，因此不可能完全禁止
// 用户使用它们，所有 GLSL 提供了一个内置函数 fma() 让用户可以直接使用这个函数。当然，如
// 果不需要考虑交换 a 和 c 的值，那么没有必要使用这种写法，因为那个时候也没有必要使用
// precise 了。
//      location = a * b + c * d;
//      precise gl_Position;
//      precise out vec3 location;
//      precise vec3 subdivide(vec3 p1, vec3 p2) { ... }
//      precise out float result;
//      float f = c * d;
//      float result = fma(a, b, f);
//
// 着色器的预处理器。编译一个 GLSL 着色器的第一步是解析预处理器，这一点与 C 语言中的预处
// 理类似，并且 GLSL 同样提供了一些列命令来有条件地生成编译代码块，或者定义数值。不过，与
// C 语言的预处理不同的是，GLSL 中没有文件包含命令 #include。下表给出了 GLSL 所支持的
// 预处理命令。GLSL 预处理与 C 语言类似，不过它不支持字符串替换以及预编译连接。字符串替
// 换（String Substitution）是指预处理器将宏参数或宏体中的内容原样转换为用双引号括起的
// 字符串字面量，即字符串化操作符（#s），而预编译连接即连接运算符（##）。
//      #define #undef
//      #if #ifdef #ifndef #else #elif #endif
//      #error text
//      #pragma options 控制编译器的特定选项
//      #extension options 设置编译器支持特定 GLSL 扩展功能
//      #version number 设置当前使用的 GLSL 版本
//      #line options 设置诊断行号
//      __LINE__ 当前源代码行号
//      __FILE__ 当前源代码文件名称
//      __VERSION__ 当前 GLSL 版本的整数表示形式
//
// #pragma 命令可以向编译器传递附加信息，并在着色器代码编译时设置一些额外属性。优化选项
// 用于启用或者禁用着色器的优化，它会直接影响该命令所在的着色器源代码。这类选项必须在函数
// 定义的代码之外设置，一般默认所有着色器都开启了优化选项。调试选项可以启用或禁用着色器的
// 额外诊断信息的输出。另一个可用的 #pragma 命令选项就是 STDGL，这个选项目前用于启用所有
// 输出变量值的不变性检查。
//      #pragma optimize(on)
//      #pragma optimize(off)
//      #pragma debug(on)
//      #pragma debug(off)
//
// GLSL 与 OpenGL 类似，都可以通过扩展的方式来增加功能。设备生产商也可以在自己的 OpenGL
// 实现中加入特殊的扩展，因此很有必要对着色器中可能用到的扩展功能进行编译级别的控制。GLSL
// 预处理器提供了 #extension 命令，用户提示着色器的编译器在编译时如何处理可用的扩展内容。
// 对于任何扩展，或者全部扩展，我们都可以在编译器编译过程中设置它们的处理方式。如下所示，
// 这里的 extension_name 与调用 glGetString(GL_EXTENSIONS) 时获取的扩展功能名称是一致
// 的，或者也可以使用下一行的方式影响所有扩展的行为，directive 可用的选项如下表。
//      #extension extension_name: <directive>
//      #extension all: <directive>
//      require     如果对应的扩展无法支持或设置为 all，则提示错误
//      enable      如果对应的扩展无法支持则提示警告，如果设置为 all 则提示错误
//      warn        如果对应的扩展无法支持，或者编译过程使用了任何扩展，则提示警告
//      disable     强制编译器不提供对该扩展功能的支持，如果设置为all则禁止所有扩展
//
// 着色器与应用程序之间，或者着色器各阶段之间共享的变量可以组织为变量块的形式，并且有时候
// 必须采用这种形式。uniform 变量可以使用 uniform 块，输入和输出变量可以使用 in 和 out
// 块，着色器的存储缓存可以使用 buffer 块。它们的形式都是类似的，看下面的 uniform 块的
// 写法。块（block）开始部分的名称对应于外部访问时的接口名称，而尾部名称用于在着色器代码
// 中访问具体成员变量。如果着色器变得比较复杂，那么其中用到的 uniform 变量的数量大增，通
// 常会在多个着色器程序中用到同一个 uniform 变量。由于 uniform 变量的位置是着色器链接时
// 产生的，也就是调用 glLinkProgram 的时候，因此它在应用程序中获得的索引可能会有变化，即
// 使我们给 uniform 变量设置的值可能是完全相同的。而 uniform 缓存对象（uniform buffer
// object）就是一种优化 uniform 变量访问，以及在不同着色器之间共享 uniform 数据的方法。
// 正如你知道的，uniform 变量是同时存在于用户应用程序和着色器当中的，因此需要同时修改着色
// 器的内容并调用 OpenGL 函数来设置 uniform 缓存对象。
//      uniform a {
//          vec4 v1;
//          bool v2;
//      }; // 着色器访问块成员使用 v1 v2
//      uniform a {
//          vec4 v1;
//          bool v2;
//      } name; // 着色器访问块成员使用 name.v1 name.v2
//      uniform matrices {
//          mat4 model_view;
//          mat4 projection;
//          mat4 color;
//      };
//
// 在着色器中指定 uniform 块。访问一组 uniform 变量的方法是使用如 glMapBuffer 函数，而
// 在着色器中对它们的声明，不再分别声明每个 uniform 变量，而是直接成组声明成 uniform 块。
// 着色器中的数据类型有两种，不透明和透明的，一个 uniform 块中只可以包含透明类型的变量，
// 此外 uniform 块必须在全局作用中声明。可以使用限定符来指定 uniform 块中变量的布局方式，
// 这些限制符可以用来设置单个 uniform 块，也可以使用布局声明来设置所有后继 uniform 块的
// 排列方式。
//      shared          设置 uniform 块是多个程序间共享（这是默认布局方式，注意还有一个名为 shared 变量存储限定符）
//      packed          设置 uniform 块占用最小的内存空间，但是这样会禁止程序间共享这个块
//      std140          使用标准布局来设置 uniform 块或者 buffer 块
//      std430          使用标准布局来设置 buffer 块
//      binding=N       指定缓存的绑定点，被 OpenGL 函数使用
//      offset=N        显式强制一个成员偏移位于缓存偏移 N 的位置
//      align=N         显式强制一个成员偏移是 N 的倍数
//      row_major       使用行主序来存储 uniform 块中的矩阵
//      column_major    使用列主序来存储 uniform 块中的矩阵（这是默认顺序）
//
// 多个限制符可以通过圆括号中的逗号来分隔，如果需要对所有后继的 uniform 块设置同一种布局，
// 单纯声明一个 uniform 关键字。这样一来，当前行之后的所有 uniform 块都会使用这种布局方
// 式，除非再次改变全局布局，或者多某个块单独声明设置专属的布局方式。当你在一个着色器和应
// 用程序之间共享缓冲区时，双方需要就哪些内存偏移量存放哪些成员达成一致。因此，需要一个显
// 式布局，这正是 std140 和 std430 所提供的。虽然 std140 和 std430 提供了定义良好的显
// 式缓冲区布局，但你可能希望对缓冲区的布局方式进行更精细的控制。你可以使用 offset 精确
// 控制成员的位置，或者使用 align 在更粗粒度上对齐成员。你只需要对部分成员使用这些限定符，
// 以保持应用程序和着色器之间的布局同步。随后未加限定符的成员会自动分配偏移量，这与 std140
// 或 std430 的标准做法一致。在应用程序中，设置缓冲区的结构以匹配上述布局，可以使用 C/C++
// struct 的语言工具进行修饰，或者直接以正确的偏移量写入缓冲区。唯一需要注意的是，偏移量
// 对齐方式都必须是合理的。成员仍然按照偏移量递增的顺序排列，并且仍然必须按照 std140 和
// std430 规则的要求进行对齐。一般来说，对于包含浮点数和双精度数的任何类型，这是自然对齐
// 的，其中 std140 还有一个额外约束：小于 vec4 的类型需要 16 字节对齐。关于 N 的说明：任
// 何时候 GLSL 布局限定符采用 layout(ID = N) 的形式，值 N 必须是非负整数。在 #version
// 430 或更早版本中，N 必须是字面量整数。然而，从 #version 440 开始，N 可以是常量整数表
// 达式。
//      layout (shared, row_major) uniform { ... }; // 专属布局
//      layout (packed, column_major) uniform; // 全局布局
//
//      #version 440
//      layout (std140) uniform b {
//          float size; // 偏移从 0 开始
//          layout (offset=32) vec4 color; // 偏移从 32 字节开始
//          layout (align=1024) vec4 a[12]; // 偏移从 1024 倍数处开始
//          vec4 b[12]; // 偏移紧跟在 a[12] 之后
//      } buf;
//
// 访问在 uniform 块中声明的 uniform 变量。虽然 uniform 块是有名称的，但在其中声明的
// uniform 变量并不由该名称限定。也就是说，uniform 块不会对 uniform 变量的名称进行作用
// 域限定，因此在两个不同名称的 uniform 块中声明两个同名变量会导致错误。不过，访问 uniform
// 变量时并不需要使用块名称。
//
// 从应用程序种访问 uniform 块。niform 变量是着色器与应用程序之间数据的桥梁，因此如果着
// 色器种的 uniform 变量是定义在命名 uniform 块中，那么就必要找到不同变量的偏移值。如果
// 获取了这些变量的具体位置，那么就可以使用数据对它们进行初始化，这一过程与处理缓存对象是
// 一致的（使用 glBufferData 函数）。调用 glGetUniformBlockIndex() 可以找到块在着色器
// 程序种的索引位置，然后在应用程序的地址空间中完成 uniform 变量的映射。如果要初始化 uniform
// 块对应的缓存对象，需要使用 glBindBuffer() 将缓存对象绑定到目标 GL_UNIFORM_BUFFER 之
// 上。当对缓存对象进行初始化之后，我们需要判断命名的 uniform 块中的变量总共占据多大的空间，
// 可以调用函数 glGetActiveUniformBlockiv() 并设置参数为 GL_UNIFORM_BLOCK_DATA_SIZE
// 得到编译器分配的大小（根据 uniform 块的布局，编译器可能会自动排除着色器中没有用到的变
// 量）。glGetActiveUniformBlockiv() 还可用来获取命名 uniform 块的其他相关参数。在获取
// uniform 块的索引之后，我们需要将一个缓存对象与这个块相关联，最常见的方法是调用 glBindBufferRange()，
// 或者如果整个缓存都是来存储 uniform 块的，那么可以使用 glBindBufferBase()。当建立了命
// 名 uniform 块和缓存对象之间的关联之后，只要使用缓存相关命令即可对块内的数据进行初始化
// 或者修改。
//
// GLuint glGetUniformBlockIndex(GLuint program, const char *uniformBlockName);
// glGetActiveUniformBlockiv()
// glBindBufferRange()
// glBindBufferBase()
// GLint glUniformBlockBinding(GLuint program, GLuint uniform_block_index, GLuint uniform_block_binding);
// void glGetUniformIndices(GLuint program, GLsizei uniform_count, const char **uniform_names, GLuint *uniform_indices);
// glGetActiveUniformsiv()
//
// 我们也可以直接设置某个命令 uniform 块和缓存对象之间的绑定关系，也就是说不使用链接器内
// 部自动绑定块对象并且查询关联结果的方式。如果多个着色器程序需要共享同一个 uniform 块，
// 那么你可能需要用到这种方法。这样可以避免对不同的着色器程序同一个块有不同的索引号。如果
// 需要显式地控制一个 uniform 块的绑定方式，可以在调用 glLinkProgram() 之前调用 glUniformBlockBinding()
// 函数。在一个命名的 uniform 块中，uniform 变量的布局是通过各种布局限制符在编译和链接时
// 控制的。如果使用了默认的布局方式，那么需要判断每个变量在 uniform 块中的偏移量和数据存
// 储大小。为此，需要调用两个命令，glGetUniformIndices() 负责获取指定名称 uniform 变量
// 的索引位置，而 glGetActiveUniformsiv() 可以获得指定索引位置的编译量和大小。
//
// 在着色器运行之前，uniform 修饰符可以指定一个在应用程序中设置好的变量，它不会在图元处理
// 的过程中发生变化。uniform 变量在所有可用的着色阶段之间都是共享的，它必须定义为全局变量。
// 任何类型的变量（包括结构体和数组）都可以设置为 uniform 变量，着色器无法写入到 uniform
// 变量，也无法改变它的值。举例来说，我们可能需要设置一个给图元着色的颜色值，此时可以声明
// 一个 uniform 变量，将颜色值信息传递到着色器，而在着色器中会进行以下声明。在着色器中，
// 可以根据名字 base_color 来引用这个变量，但是如果需要在用户应用程序中设置它的值，还需
// 要多做一些工作。GLSL 编译器会在链接着色器程序时创建一个 uniform 变量列表，如果需要设置
// 应用程序中 base_color 的值，我们需要首先获得 base_color 在列表中的索引，这一步可以通
// 过 glGetUniformLocation() 函数完成。当得到 uniform 变量对应的索引值之后，可以通过调
// 用 glUniform*() 或者 glUniformMatrix*() 系列函数来设置 uniform 变量的值了。
//      uniform vec4 base_color;
//      GLint time_location;
//      GLfloat time_value;
//      time_location = glGetUniformLocation(program, "time");
//      glUniformf(time_location, time_value);
//
// GLint glGetUniformLocation(GLuint program, const char *name);
// void glUniform{1234}{fdi ui}(GLint location, TYPE value);
// void glUniform{1234}{fdi ui}v(GLint location, GLsizei count, const TYPE *values);
// void glUniformMatrix{234}{fd}v(GLint location, GLsizei count, GLboolean transpose, const GLfloat *values);
// void glUniformMatrix{2x3,2x4,3x2,3x4,4x2,4x3}{fd}v(GLint location, GLsizei count, GLboolean transpose, const GLfloat *values);
//
// GLSL 中的 buffer 块，或着色器的存储缓存对象（shader storage buffer object），它的
// 行为类似 uniform 块。不过两者之间有两个决定性的区别，使得 buffer 块的功能更加强大。
// 首先，着色器可以写入 buffer 块，修改其中的内容并呈现给其他着色器或应用程序本身。其次，
// 可以在渲染之前再决定它的大小，而不是编译和链接时决定。例如下面的 buffer 块声明，如果
// 没有给出对应数组的大小，那么可以在应用程序中编译和链接后，在渲染之前设置它的大小。着色
// 器可以通过 length() 方法来获取渲染时的数组大小。着色器可以对 buffer 块中的成员执行读
// 或者写，写入操作对着色器存储缓存对象的修改对于其他着色器都是可见的。这种特性对于计算着
// 色器非常有意义，尤其是对非图像的内存区域进行处理的时候。有关 buffer 块的内存限制符，如
// coherent 以及原子操作的相关讨论见后面计算着色器部分。设置着色器存储缓存对象的方式与设
// 置 uniform 缓存的方式类似，不过 glBindBuffer() 和 glBufferData() 需要使用 GL_SHADER_STORGAGE_BUFFER
// 作为目标参数。如果你不许在着色器中写入缓存，那么可以直接使用 uniform 块，并且硬件设备
// 本身可能也没有足够的资源空间来支持 buffer 块，但是 uniform 块通常是足够的。
//      buffer buffer_object { // 可被着色器读写的缓存块
//          int mode; // preamble members
//          vec4 points[]; // 最后一个成员可以是未定义大小的数组
//      };
//
// 输入输出块。着色器变量从一个阶段输出，然后再输入到下一个阶段，这一过程可以使用块接口来
// 表示。使用逻辑上成组的方式来进行组织也更有利于判断两个阶段的数据接口是否一致，同样对单
// 独程序的链接也会变得更加简单。例如，一个顶点着色器的输出可能如下，它必须与片元着色器的
// 输入匹配。顶点着色器可以输出材质和光照信息，并且都分成独立的数据块。OpenGL 着色语言中
// 内置的接口同样也是以块的方式存在的。例如 gl_PerVertex 其中包含了内置变量 gl_Position
// 等信息。
//      out lighting {
//          vec3 normal;
//          vec2 bump_coord;
//      };
//      in lighting {
//          vec3 normal;
//          vec2 bump_coord;
//      };
//
// 着色器的编译。OpenGL 着色器程序的编写与 C 语言等基于编译器的语言非常类似。我们使用编
// 译器来解析程序，检查是否存在错误，然后将它翻译为目标代码。然后，在链接过程中将一些列目
// 标代码合并，并产生最终的可执行程序。在程序中使用 GLSL 着色器的过程与之类似，只不过编
// 译器和链接器都是 OpenGL 接口的一部分而已。以下给出了创建 GLSL 着色器对象并且通过链接
// 生成可执行着色器程序的过程。对于每个着色器程序，我们都需要在应用程序中通过下面的步骤进
// 行设置。对每个着色器对象，执行以下步骤。为什么要创建多个着色器对象？这是因为我们有可能
// 在不同的程序中复用同一个函数，我们创建的通用函数可以在多个着色器中得到复用。
//  1.  创建一个着色器对象并关联源代码，glCreateShader() glShaderSource()
//  2.  将着色器源代码编译为对象，glCompileShader() glGetShaderInfoLog()
//  3.  验证着色器的编译是否成功
//
// 然后需要将多个着色器对象链接为一个着色器程序，包括：
//  1.  创建一个着色器程序，glCreateProgram()
//  2.  将着色器对象关联到着色器程序，glAttachShader() glDetachShader()
//  3.  链接着色器程序，glLinkProgram() glGetProgramInfoLog()
//  4.  判断着色器的链接过程是否成功完成
//  5.  使用着色器来处理顶点和片元，glUseProgram()
//
// 如果成功完成着色器程序的链接，那么就可以调用 glUseProgram() 启用着色器程序。当这些任
// 务完成后，就可以调用 glDeleteShader() 删除着色器对象，并不需要关心它是否关联到某个活
// 动程序上。这一点与 C 语言程序的链接是相同的，当我们得到可执行程序之后，就不再需要对象
// 文件了，直到我们再次进行编译为止。与此类似，如果我们不再使用某个着色器程序，可以调用
// glDeleteProgram() 删除。最后可以通过 glIsShader() 和 glIsProgram() 判断一个对象是
// 不是着色器对象或者程序。
//
// GLuint glCreateShader(GLenum type);
// void glDeleteShader(GLuint shader);
// GLboolean glIsShader(GLuint shader);
// void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint length);
// void glCompileShader(GLuint shader);
// void glGetShaderInfoLog(GLuint shader, GLsizei buffer_size, GLsizei *length, char *info_log);
// GLuint glCreateProgram(void);
// void glDeleteProgram(GLuint program);
// GLboolean glIsProgram(GLuint program);
// void glAttachShader(GLuint program, GLuint shader);
// void glDetachShader(GLuint program, GLuint shader);
// void glLinkProgram(GLuint program);
// void glGetProgramInfoLog(GLuint program, GLsizei buffer_size, GLsizei *length, char *info_log);
// void glUseProgram(GLuint program);
//
// 着色器子程序。GLSL 允许我们在着色器中定义函数，而这些函数的调用过程总是静态的，如果需
// 要动态地选择调用不同的函数，那么可以创建两个不同的着色器，或者使用 if 语句来进行运行时
// 选择。而着色器子程序在概念上类似于 C 语言的函数指针，它可以实现动态子程序选择。在着色
// 器中，可以预先声明一个可用子程序的集合，然后动态地指定子程序的类型，然后通过设置一个子
// 程序的 uniform 变量，从预设的子程序中选择一个并加以执行。当我们需要在着色器中进行子程
// 序选择时，通过需要三个步骤来设置一个子程序池。首先，通过关键字 subroutine 定义子程序的
// 类型，它相当于定义一个函数原型；然后，使用刚才定义的 subroutine_type 来定义这个子程序
// 集合的内容，以便稍后进行动态选择；最后，指定一个子程序 uniform 变量，其中保存了相当于
// 函数指针，在应该程序中更改进行选择。函数并不一定只属于一个子程序类型，如果定义了多种类
// 型的子程序，只要能够适配我们可以设置一个函数属于多个类型，方法是在定义函数时把类型添加
// 到列表中。例如下面的示例，可以将函数 func_1 赋值给函数指针 name_1 和 name_2。
//      void func1() { ... }
//      void func2() { ... }
//      uniform int func; // 静态着色器控制
//      void main() { if (func == 1) func1() else func2(); }
//
//      subroutine return_type subroutine_type(type param, ...);
//      subroutine (subroutine_type) return_type func_name(type param, ...);
//      subroutine uniform subroutine_type variable_name;
//
//      subroutine void type_1();
//      subroutine void type_2();
//      subroutine void type_3();
//      subroutine (type_1, type_2) func_1();
//      subroutine (type_1, type_3) func_2();
//      subroutine uniform type_1 name_1;
//      subroutine uniform type_2 name_2;
//      subroutine uniform type_3 name_3;
//
// 选择着色器函数。如果我们已经在着色器中定义了所有的子程序类型和函数，那么只需要在链接后
// 的着色器程序中查询这些值，然后设置合适的函数即可。与其他 uniform 变量不同，子程序的
// uniform 变量需要使用 glGetSubroutineUniformLocation() 来获取自身的位置。然后调用
// glGetSubroutineIndex() 获取着色器函数的索引，最后调用 glUniformSubroutinesuiv()
// 设置选择的函数。某个着色阶段中，所有的子程序 uniform 变量都必须先进行初始化。注意，调
// 用 glUseProgram() 时会重置所有子程序 uniform 变量的值，具体的顺序与硬件实现相关。
//
// GLint glGetSubroutineUniformLocation(GLuint program, GLenum shadertype, const char *name);
// GLuint glGetSubroutineIndex(GLuint program, GLenum shadertype, const char* name);
// GLuint glUniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint * indices);
//
// 可以看下面的一个实现环境光照和漫反射光照动态选择的例子：
//      // GLSL 着色器程序
//      subroutine vec4 LightFunc(vec3);
//      subroutine (LightFunc) vec4 ambient(vec3 n) {
//          return Materials.ambient;
//      }
//      subroutine (LightFunc) vec4 diffuse(vec3 n) {
//      return Materials.diffuse * max(dot(normalize(n), LightVec.xyz), 0.0);
//      }
//      subroutine uniform LightFunc material_shader;
//
//      // OpenGL 程序 C 代码
//      GLint materialShaderLoc;
//      GLuint ambientIndex;
//      GLuint diffuseIndex;
//      glUseProgram(program);
//      materialShaderLoc = glGetSubroutineUniformLocation(program, GL_VERTEX_SHADER, "material_shader");
//      if (materialShaderLoc < 0) ; // Error: materialShader is not an active subroutine uniform in the shader.
//      ambientIndex = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "ambient");
//      diffuseIndex = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "diffuse");
//      if (ambientIndex == GL_INVALID_INDEX || diffuseIndex == GL_INVALID_INDEX) {
//          // Error: the specified subroutines are not active in the currently bound program for the GL_VERTEX_SHADER stage.
//      } else {
//          GLsizei n;
//          glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &n);
//          GLuint *indices = new GLuint[n];
//          indices[materialShaderLoc] = ambientIndex;
//          glUniformSubroutinesuiv(GL_VERTEX_SHADER, n, indices);
//          delete[] indices;
//      }
//
// 独立的着色器对象。在 OpenGL 4.1 版本之前（不考虑扩展功能），在应用程序中，同一时间只能
// 绑定一个着色器程序。如果你的程序需要使用多个片元着色器来处理来自同一个顶点着色器的几何体
// 变换数据，那么这样很不方便。此时只能将同一个顶点着色器复制多份，并且多次绑定到不同的着色
// 器程序，从而造成资源的浪费和代码的重复。独立的着色器对象可以将不同程序的着色阶段（如顶点
// 着色）合并到同一个程序管线中。首先，我们需要创建用于着色器管线的着色器程序。我们可以调用
// glProgramParameteri() 设置参数 GL_PROGRAM_SEPARABLE，然后再链接着色器程序。这样该程
// 序就被标记为在着色器程序管线中使用。如果想要简化这个过程，还可以直接使用新增的 glCreateShaderProgramv()
// 来封装着色器编译过程，并且将程序标识为可共享，然后链接到最终的对象。着色器管线的创建可以
// 调用 glGenProgramPipelines() 或 glCreateProgramPipelines() 然后 glBindProgramPipeline()。
// 之后，可以调用 glUesProgramStages() 将之前标记为独立的程序对象关联到管线上，它通过位
// 域的方式来描述管线处理几何体和着色片元时程序所处的着色阶段。而之前的 glUseProgram() 只
// 能直接调用一个程序并且替换当前绑定的程序管线。
//
// 为了确保管线可以使用，着色器阶段之间的接口（in 和 out 变量）必须匹配。非独立的着色器
// 对象在链接时就可以检查这些接口的匹配情况，与之相比，使用独立程序对象的着色器管线只能在
// 绘制调用过程中进行检查。如果接口没有正确匹配，那么所有的可变变量（out 变量）都未定义。
// 内置的 gl_PerVertex 块必须重新声明，以便显式地指定固定管线接口中的哪些部分可以使用。
// 如果管线用到了多个程序，那么这一步是必需的。例如：
//      out gl_PerVertex {
//          vec4 gl_Position; // 设置 gl_Position 在接口中可用
//          float gl_PointSize; // 设置 gl_PointSize 在接口中可用，不再使用 gl_PerVertex 其他成员
//      };
//
// 这样我们就建立了着色器的输出接口，它将用户后续的管线阶段当中。这里必须使用 gl_PerVertex
// 自己的内置成员。如果不同的着色器程序都用到了同一个内置的块接口，那么所有的着色器都必须
// 使用相同的方式重新声明这个内置的块。因为独立的着色器对象可以有各自独立的程序 uniform
// 集合，所有我们可以使用两种方法来设置 uniform 变量的值。第一种方法是通过 glActiveShaderProgram()
// 来选择一个活动的着色器程序，然后调用 glUniform*() 和 glUniformMatrix*() 来设置某个
// 着色器程序的 unifrom 变量的值。另一种方法，也是我们推荐的方法，是调用 glProgramUniform*()
// 和 glProgramUnifromMatrix*() 函数，它们有一个显式的 program 对象参数，这样可以独立
// 地设置某个程序的 unifrom 变量的值。
//
// Khronos SPIR-V Registry https://registry.khronos.org/SPIR-V/#spec
//
// SPIR-V 是 Khronos 标准化的中间语言，为分发着色器提供了另一种选择。OpenGL 接受 SPIR-V
// 形式的着色器，就像它接受 GLSL 形式的着色器一样。通常，对于 SPIR-V 形式，离线工具链会
// 从高级着色语言（如 GLSL）生成 SPIR-V，而不是随应用程序分发 GLSL 源代码，你会分发生成
// 的 SPIR-V。SPIR-V 以称为模块（modules）的二进制单元形式创建、分发和消费。SPIR-V 模块
// 可以作为 32 位字的序列存在于内存中，也可以作为文件存储，同样作为 32 位字的序列。然而，
// 与 GLSL 一样，OpenGL 不处理文件，因此 SPIR-V 必须以指向内存中 32 位字序列的指针形式
// 交给 OpenGL。
//
// 每个 SPIR-V 模块包含一个或多个入口点，作为着色器执行的起始位置，每个入口点都知道它属于
// 哪个 OpenGL 管线阶段。这些入口点中的每一个必须形成一个单一的、完整的 OpenGL 管线阶段。
// 也就是说，与桌面版 GLSL 不同，SPIR-V 着色器不包含多个编译单元，以便后续链接在一起形成
// 单个阶段。对于 SPIR-V，这种链接将由前端在将高级语言形式翻译为 SPIR-V 时离线完成，产生
// 的结果是一个完全链接的阶段。单个 SPIR-V 模块可以包含多个入口点，甚至可以是同一阶段的多
// 个入口点。SPIR-V 模块可以被特化，这意味着在运行时最终编译之前，更改模块内部某些特殊标
// 识的常量的值。这样做是为了减少表示着色器多个微小变体所需的 SPIR-V 模块的数量（或大小）。
//
// 选择 SPIR-V 的理由。与 GLSL 相比，分发 SPIR-V 形式的着色器有几个潜在的理由。有些可能
// 适用于你的情况，有些可能不适用：
//  1.  更好的可移植性。可移植性的一个问题是，每个平台的驱动程序对 GLSL 的高级规则可能有
//      略微不同的解释。高级语言之所以部分是高级的，是因为它们允许编码者拥有表达自由。然而，
//      这种自由的界限有时很难完全确定，导致解释上的差异。SPIR-V 在构造表达方式上要严格得
//      多、规范得多，留下的解释空间更少。这反过来导致各平台对 SPIR-V 的解释差异更小，从
//      而提高了可移植性。当然，你不是用 SPIR-V 编写代码，所以你仍然需要处理 GLSL（例如）。
//      然而，为了生成 SPIR-V，你可以为所有目标平台选择单一的前端。也就是说，你可以通过坚
//      持使用单一的 GLSL 前端，来消除源于不同 GLSL 解释的可移植性问题。其他人可能会为他
//      们的着色器选择不同的前端，那也没关系。重要的是，一个应用程序的 GLSL 着色器在 SPIR-V
//      最终运行的所有平台上获得相同的 GLSL 解释。
//  2.  其他源语言。SPIR-V 使得使用 GLSL 以外的高级语言成为可能。只要分发的 SPIR-V 格式
//      正确，它是如何生成的不重要。
//  3.  减小分发体积。SPIR-V 具有多种特性，可以显著减小分发时着色器集合的大小。单个着色器
//      本身在 SPIR-V 中通常比在 GLSL 中大，但单个着色器本身在任何情况下都很小。然而，相
//      关着色器的集合可能相当大，而 SPIR-V 有两个特性专门用于解决这个问题：特化和每模块多
//      个入口点。特化允许后期更改某些常量值，同一 SPIR-V 模块中的多个入口点允许交付单个函
//      数体的单一实例，该函数体可能被多个入口点使用。GLSL 分发可能需要为集合中的每个着色器
//      分发该函数的副本，而 SPIR-V 分发只需交付一个副本。
//  4.  保护源代码。这有时被称为混淆，因为有时你不想以易于利用的形式分发着色器源代码。着色
//      器源代码可以代表新颖的想法或知识产权，你不想以透明、易于修改的格式将其分发给其他方。
//      你可以通过将源代码离线编译为 SPIR-V 并分发 SPIR-V 来避免分发源代码。这使得查看着
//      色器如何实现某种效果变得更加困难。是的，反向编译仍然可以重新创建 GLSL 或其他高级
//      着色语言，这些语言会编译为你分发的 SPIR-V。然而，接收者需要进行这种逆向工程活动的
//      需求，为你的知识产权提供了真正的保护。
//
// 运行时编译器性能通常被认为是选择中间语言而非高级语言的另一个理由，但这里需要谨慎。高性
// 能着色器可执行文件通常需要在运行时执行调度和寄存器分配算法，这些算法本身就很耗时。这些
// 后续步骤无法通过使用可移植的中间语言来消除。然而，运行时编译器性能在多个方面得到了改善。
// 首先，解析高级语言需要一些时间。虽然解析通常是编译栈的一小部分，但对于包含大量未使用代
// 码的着色器，或多个着色器编译为相同中间结果的情况，解析变得更加重要。在这些情况下，使用
// SPIR-V 可以显著消除解析时间。此外，一些高级优化可以离线执行，但要注意不要执行可能损害
// 某些目标平台性能的平台特定优化。例如，是否应在所有调用点内联所有函数可能是平台相关的。
//
// 在 OpenGL 中使用 SPIR-V。在 OpenGL 中使用 SPIR-V 着色器与使用 GLSL 着色器非常相似。
// 按照我们之前描述的方式创建着色器对象后，需要两个步骤将 SPIR-V 入口点与每个着色器对象
// 关联。第一步是通过调用 glShaderBinary() 将 SPIR-V 模块与每个着色器对象关联。由于 SPIR-V
// 通常以 32 位字流的形式指定和操作，请确保将 SPIR-V 的大小转换为字节以用于 glShaderBinary()。
// 此 glShaderBinary() 函数可用于其他非源代码形式的着色器，因此这是一个通用函数，除非指定
// 了 SHADER_BINARY_FORMAT_SPIR_V_ARB，否则不特定于 SPIR-V。将 SPIR-V 入口点与着色器对
// 象关联所需的第二步是 glSpecializeShader()，如果成功，它将把由 glShaderBinary() 设置
// 的编译状态从 GL_FALSE 更改为 GL_TRUE。我们将稍后讨论使用 GLSL 进行特化，之后你使用
// glAttachShader() 和 glLinkProgram()，就像我们之前使用 glShaderSource() 处理 GLSL
// 时一样，其他一切工作方式相同。
//
// 使用 GLSL 为 OpenGL 生成 SPIR-V。如何生成 SPIR-V 没有任何要求，只要求 SPIR-V 本身是
// 格式良好的。虽然这对于支持广泛的高级语言和创建 SPIR-V 的新颖工具非常有利，但拥有用于编
// 写和交换着色器的标准高级语言也很方便。为此，Khronos 标准化了一种用于创建 SPIR-V 的 GLSL
// 形式。有两种用于制作 SPIR-V 着色器的 GLSL 风格：一种创建适合 Vulkan 的 SPIR-V（通过
// KHR_glsl_vulkan 扩展），一种创建适合 OpenGL 的 SPIR-V（通过 ARB_gl_spirv 扩展）。
// 这里，当然，我们将讨论用于为 OpenGL 生成 SPIR-V 的 GLSL。用于此目的的 GLSL 与标准 GLSL
// 相同，有少量添加、少量删除和一些小的更改。通常，所有输入和输出都需要指定位置，I/O 类似
// 于使用 SSO 模型。除此之外，它与本章前面介绍的 GLSL 完全相同。
//
// 验证 SPIR-V。OpenGL 驱动程序不会在运行时完全验证 SPIR-V，因为创建有效的离线 SPIR-V
// 是一种性能优势。OpenGL 只需要在被给予完全有效的 SPIR-V 时表现正确。也就是说，无效的
// SPIR-V 可能导致意外行为。Khronos 提供了 SPIR-V 验证器以及其他工具，可在以下网页地址
// https://github.com/KhronosGroup/SPIRV-Tools 获取，帮助你离线验证要分发的 SPIR-V
// 是否有效。此工具应集成到你的离线 SPIR-V 生成工具链中，以使着色器获得最大的可移植性。
//
// 为了生成 SPIR-V，GLSL 添加的关键特性是特化。特化常量可以大大减少你分发的着色器变体数
// 量。它们允许后期更改着色器常量，而无需手动生成新着色器。通常，知道哪些值在编译时是常量
// 有助于优化器生成执行更快的代码，相比访问始终具有相同值的变量。循环可以获得已知计数，计
// 算可以简化。由于使用常量的这些积极影响，GLSL 着色器通常用预处理器宏或某种形式的计算机
// 生成代码进行参数化。然后为参数的不同值创建多个不同的着色器。使用特化常量，此类参数被显
// 式标识，赋予默认值，并允许被视为常量，即使其值可以在最终运行时编译之前更改。因此，可以
// 创建和分发带有特化常量的单个着色器，这些常量稍后获取其正确的最终值。在 GLSL 中，这看起
// 来像下面这样。它声明 param 是特化常量（因为 constant_id），默认值为 8。值 17 是如果
// 应用程序想通过 OpenGL API 更改默认值时，稍后引用 param 的方式，就像之前使用 glSpecializeShader()
// 所做的那样。当编译为 SPIR-V 时，SPIR-V 着色器将此 param 跟踪为特化常量。当需要使用着
// 色器创建渲染管线时，提供正确的常量值与 SPIR-V 着色器，然后针对该值进行优化。因此，这就
// 是避免分发同一着色器多个变体的原因。
//      layout (constant_id = 17) const int param = 8;
//
// 为了生成 SPIR-V，GLSL 有一些删除，SPIR-V 不支持一些传统的 GLSL 特性。我们在此列出并
// 给出替代的建议。
//  1.  子程序：OpenGL GLSL 子程序特性在 SPIR-V 中不可用。可以使用 GLSL 中的其他构造表
//      达类似功能，包括 switch 语句和函数调用。例如：
//      switch (material) {
//          case 1: result = material1(...); break;
//          case 2: result = material2(...); break;
//          case 3: result = material3(...); break;
//      }
//  2.  已弃用特性：应始终避免使用已弃用特性，但某些特性在生成 SPIR-V 时完全缺失。这包括
//      旧的已弃用纹理函数，如 texture2D()，它们不再被允许，因为 texture2D 现在保留为从
//      单独的采样器和 2D 纹理创建 sampler2D 的类型。相反，只需使用现代版本 texture 及
//      其纹理查找内置函数家族。
//  3.  兼容配置文件（compatibility profile）：通常，仅属于兼容配置文件的特性不受 SPIR-V
//      支持，从 GLSL 生成 SPIR-V 时不允许使用 GLSL 兼容配置文件。你需要使用核心配置文件
//      的特性来表达着色器，包括前面为 SPIR-V 添加到 GLSL 的特性。
//  4.  gl_DepthRangeParameters：SPIR-V 没有用于深度范围参数的内置变量。任何你想与着色
//      器共享的此类信息，你可以改为通过声明自己的 uniform 变量并通过 API 显式设置它们
//      来共享。
//
// 为了生成 SPIR-V，对 GLSL 的更改。gl_FragColor 广播：当直接使用 GLSL（而非通过 SPIR-V）
// 时，写入 gl_FragColor 可以生成对所有颜色输出附件的广播写入。然而，SPIR-V 不支持此特
// 性。理想情况下，你将声明要写入的输出变量并显式写入它们。如果你确实使用 gl_FragColor，
// 写入它将只写入附加在位置 0 的单个颜色输出。
//
// Khronos Group 提供了一个 GLSL 参考前端，能够从 GLSL 为 OpenGL 或 Vulkan 生成 SPIR-V。
// 注意，你必须指定为哪个 API 生成 SPIR-V，因为它们具有不同的特性，因此具有不同的 GLSL 语
// 义。虽然它是用于验证正确 GLSL 的 Khronos 参考前端，但它只是 SPIR-V 编译器的一个示例，
// 不应被视为唯一的方式。Glslang 作为 GitHub 上的开源项目维护，地址为 https://github.com/KhronosGroup/glslang。
// 注意，glslang 是 Khronos 用于直接 OpenGL 消费的有效 GLSL，或 OpenGL ES 消费的有效
// ESSL 的合法语义检查的参考。这种高地位尚未赋予它用于 SPIR-V 生成，SPIR-V 生成应被视为
// 示例实现，而非 Khronos 认可的参考。
//
// SPIR-V 内部是什么？SPIR-V 是一种简单的纯二进制形式，表示高级中间语言。它以此形式存储为
// 简单的线性 32 位字序列。当你从离线编译器获得结果或设置到 API 时，它将是这样的 32 位字
// 流（但你确实需要乘以 4 以获得 glShaderBinary() 期望的字节数）。它是自包含的；字序列周
// 围没有包装器；只需从文件或 API 入口点获取或设置原始字序列。在此序列中，前几个字提供关于
// 其余部分的完整性检查，包括第一个字是 SPIR-V 魔数，你可以验证它是 0x07230203。如果你有
// 该值，但字节顺序相反，你要么没有一次查看一个 32 位字，要么某些步骤反转了字节序。
//
// SPIR-V 从高级语言编写的着色器中丢失的信息非常少。它可以保留嵌套控制和其他高级构造、GLSL
// 原生类型以及关于内置变量语义的装饰，以便没有目标平台缺少执行高性能优化所需的信息。关于
// SPIR-V 的进一步内部细节超出了本书的范围，本书旨在向你展示如何使用 GLSL 生成 SPIR-V，
// 然后你可以将其随应用程序分发，但不是如何自己制作 SPIR-V。
//
// void glShaderBinary(GLsizei count, const GLuint *shaders, enum binaryformat, const void *binary, GLsizei length);
// 当 binaryformat 为 GL_SHADER_BINARY_FORMAT_SPIR_V_ARB 时，将一组着色器对象与 binary 中给定的 SPIR-V 模块关联。
// shaders 包含 count 个着色器对象句柄的列表。每个着色器对象句柄引用唯一的着色器类型，即 GL_VERTEX_SHADER、GL_FRAGMENT_SHADER、GL_TESS_CONTROL_SHADER、GL_TESS_EVALUATION_SHADER、GL_GEOMETRY_SHADER 或 GL_COMPUTE_SHADER 之一。
// binary 指向有效 SPIR-V 模块的第一个字节，length 包含 SPIR-V 模块的长度（以字节为单位）。
// 成功消费 SPIR-V 模块后，shaders 的每个条目将知道从此 SPIR-V 模块获取其入口点。这些着色器中每个的编译状态被设置为 GL_FALSE。
//
// void glSpecializeShader(GLuint shader, const char* pEntryPoint, GLuint numSpecializationConstants, const uint* pConstantIndex, const uint* pConstantValue);
// 设置 SPIR-V 模块中入口点的名称，并设置 SPIR-V 模块中任何特化常量的值。
// shader 是之前通过 glShaderBinary() 与 SPIR-V 模块关联的着色器对象的名称。
// pEntryPoint 是指向以 NUL 结尾的 UTF-8 字符串的指针，指定用于着色器的 SPIR-V 模块中的入口点名称。如果 pEntryPoint 为 NULL，将使用字符串 "main"。
// numSpecializationConstants 是此调用设置值的特化常量数量。
// pConstantIndex 是指向 numSpecializationConstants 个无符号整数数组的指针，每个整数保存 SPIR-V 模块中特化常量的索引。pConstantValue 中的对应条目用于设置由 pConstantIndex 中条目索引的特化常量的值。虽然此数组是无符号整数，但每个条目会按位转换为模块的适当类型，因此可以通过在 pConstantValue 数组中包含其 IEEE-754 位表示来设置浮点常量。
// pConstantIndex 未引用的特化常量保留其在 SPIR-V 模块中的默认值。
// 着色器特化成功后，着色器的编译状态设置为 GL_TRUE。失败时，着色器的编译状态设置为 GL_FALSE，有关失败原因的附加信息可能在着色器编译日志中可用。
//
// 视图变换（Viewing Transformations）
//
// 通常我们会拥有很多物体对象，它们有独立指定的几何坐标，此时需要通过空间变换（移动、缩放
// 和旋转oriented）将其置入现场场景（scene）中。然后，我们还需要一个特定的位置（location）、
// 方向（direction）、缩放比例（scaling）、和朝向角度（orientation）观察这个场景（scene）。
//
// 观察视图（Viewing）。如果直接在显式设备上显式标准几何模型的坐标，我们可能什么都看不到。
// 模型坐标的范围（可能从 -100 到 100米）是不会与显式设备的坐标范围（可能 0 ~ 1919 像素）
// 相对应，如果要强制坐标必须对应，那无疑将是一件麻烦事。此外，我们需要从不同的位置、方向
// 和透视角度（perspective）来观察物体，我们该如何处理这种情况呢？基本上来说，显示器本身
// 是一个平面的、固定的、二维矩形区域，但模型却是一个三维空间的几何体。这里我们将要学习如
// 何将模型的三维坐标投影到固定的二维屏幕坐标上。将三维空间的模型投影（projecting）到二
// 维的关键工具是一个视图模型（a viewing model）、使用齐次坐标（homogeneous coordinate）、
// 通过矩阵乘法应用线性变换（linear transformations）、以及视口映射（viewport mapping）。
//
// 相机模型（Camera Model）。产生期望视图（desired view）的常见变换过程，可类比为使用
// 相机拍摄照片的过程。以下是使用相机（或计算机）的注意步骤：
//  1.  将准备拍摄的对象移动到场景（scene）中期望位置（模型变换，modeling transformation）
//  2.  将相机移动到准备拍摄的位置，将它对准某个方向（视图变换，viewing transformation）
//  3.  设置相机的焦距，或者调整缩放比例（投影变换，projection transformation）
//  4.  拍摄照片（应用变换结果）
//  5.  对结果图像进行拉伸或者挤压，将它变换到需要的图片大小（视口变换，viewport transformation）。
//      对于 3D 图形来说，这里同样需要对深度信息进行拉伸或者挤压（深度范围缩放，depth-range scaling）。
//      不要与第 3 步混淆，第 3 步只是选择捕捉场景的范围大小（how much of the scene），
//      并不是对结果进行拉伸（not how much to streth the result）。
//
// 注意第 1 步和第 2 步可以认为做的是同一件事，只不过方向相反而已。我们可以把相机固定在原
// 地，然后把感兴趣的物品拿过来拍摄，或者也可以保持物体位置不动然后把相机移过来。将相机移
// 动到左边的操作相当于将物体移动到右边，而沿着顺时针转动相机也就相当于沿着逆时针旋转物体。
// 因此第 1 步和第 2 步各自有多少操作，完全由用户自己决定。正因为如此，通常可以将这两个步
// 骤合并为一个模型-视图变换（model-view transformation）。不过它始终由一系列移动（平移，
// translation）、旋转和缩放组成。这种组合定义的特征在于，为组合到一个场景中观察的所有物体
// 创建一个单一的（single）统一（unified）空间，或称为眼空间（eye space）。
//
// OpenGL 中我们可以直接在着色器中完成上面的第 1 步到第 3 步。也就是说，我们传递给 OpenGL
// 的坐标应该是已经完成模型视图变换和投影变换的。我们还需要告诉 OpenGL 如果完成第 5 步所
// 述的视口变换，当然固定渲染管线中会自动完成这个变换。以下所示 OpenGL 的整个处理过程中所
// 用到的坐标系统。OpenGL 得到的最终坐标是归一化之后的齐次坐标，并且将进行剪切和光栅化的
// 操作。也就是说，最后要绘制的坐标总是在 [-1.0, 1.0] 范围内，直到 OpenGL 对它们进行缩放
// 以匹配视口大小为止。在 OpenGL 不可见的用户或着色器变换内部，首先模型有自己的齐次模型    *** 模型坐标，局部坐标（model/local coordinates）
// （model）坐标 (x, y, z, 1.0)，经过模型变换（物体在场景中缩放、旋转、平移）后，变成世界     世界坐标（world coordinates）
// （world）坐标 (x, y, z, 1.0)，经过视图变换（观察者从某一位置和角度观察眼前场景）后，       相机坐标，观察坐标，人眼坐标（view/eye coordinates）
// 变成人眼（eye）坐标 (x, y, z, 1.0)，经过投影变换（应用视锥体的透视和大小设置）后，变       投影坐标，裁剪坐标（project/clip coordinates）
// 成齐次的剪切（clip）坐标 (x, y, z, w)，这是最终 OpenGL 所需要的输入坐标。而前面的坐       归一化设备坐标（NDC），经投影视口变换后，Z 轴方向反转指向屏幕内
// 标都不是 OpenGL 所使用的，但是对于光照和其他着色器计算非常重要，尤其是着色器中的大部分      屏幕坐标，窗口坐标
// 光照计算都是在人眼空间坐标完成的。
//
//      起始坐标      物体或模型的坐标 | 物体单位，可以是米、英尺等
//                         (x, y, z) |
//                          [添加坐标分量 w=1.0]
//      需要齐次坐标来  模型的齐次坐标 | 单位相同
//      完成平移和投影  (x, y, z, 1.0)|
//                [用户或着色器变换：缩放、旋转、平移、投影]
//      OpenGL所需的     剪切齐次坐标 | 单位规范化（除以 w）让可见坐标点位于 -1.0 到 1.0 之间
//      输入齐次坐标    (x, y, z, w) |
//                      [OpenGL 执行除以 w 的操作]
//      OpenGL负责缩放 归一化设备坐标 | x 和 y 映射到 -1.0 到 1.0 之间
//      到视口和深度范围   (x, y, z) | z 映射到 0 到 1 之间
//                  [OpenGL 执行剪切和视口/深度范围变换]
//                  窗口坐标 (x, y) | x 和 y 的单位是像素（带小数）
//                  深度坐标 z      | z 是深度值
//                              [光栅化]
//
// OpenGL 的屏幕坐标和纹理坐标，都是坐下角为原点，x 轴向右 y 轴向上 z 轴向内，属于左手
// 坐标系。但是顶点数据一旦存储到帧缓存（framebuffer），或内存图像，或大多数图像文件格
// 式（PNG、BMP 等），都设定左上角为原点，x 轴向右 y 轴向下 z 轴向内，属于右手坐标系。
// 因此 glViewport 设置和顶点着色器输出 gl_Position 经透视和视口变换后原点都是左下角。
// 但如果从缓存或图像格式中读取，或将屏幕坐标写入缓存或图像格式，需要进行一次翻转。
//
// OpenGL 模型坐标右手系，世界坐标右手系，从 Blender（右手系）导出的模型无需坐标转换，从
// 3ds Max（左手系）导出的模型需要调整，矩阵库 GLM 默认按 OpenGL 右手系约定设计。OpenGL
// 观察/人眼坐标右手系，相机默认位于原点，看向 -Z 方向（Z 轴指向屏幕外）。OpenGL 模型视
// 图矩阵和投影矩阵默认按右手系设计（顶点着色器的输出 gl_Position 右手系），经过投影视口
// 变换后，Z 轴方向反转变成左手系的屏幕坐标。无论是透视投影还是正交投影，都会将右手系的观
// 察坐标变成左手系的屏幕坐标。总之一句话，模型坐标和世界坐标右手系，投影变换后经裁剪归一
// 化变成左手系。
//
// 视锥体（viewing frustum）。我们操作相机的第 3 步设置了焦距和缩放（投影变换），这实际
// 上是在选择相机拾取场景的矩形锥体宽窄度。只有落在这个锥体之内的几何体才会出现在最终的图
// 像上。同时该步骤还计算拥有透视投影的近大远小效果的相关参数，齐次坐标的第四个坐标值 w。
// OpenGL 还会排除太近或太远的几何体，也就是说，位于近裁剪平面（near plane）之前或远裁剪
// 平面（far plane）之后的几何体都会被剔除（culled）。这在相机类比中没有对应物（除了清理
// 镜头内部的异物），但在多种方面都非常有用。最重要的是，接近视锥体顶点的物体会显得无限大，
// 这会造成问题，尤其是当它们到达顶点时。另一方面，对于场景中太远而无需绘制的物体，出于性
// 能原因最好将其排除，如果深度必须跨越过大的距离，出于深度精度原因也应如此。因此我们需要
// 增加两个平面，并且与已有的矩形视锥体（rectanglar viewing cone）的四个平面相交，这样的
// 六个平面就定义了一个截锥体形状的观察视景空间（viewing frustum）。名词 cone 标识圆锥体，
// 而 frustum 表示截锥体。
//
// 视锥体的剪切（frustum clipping）。如果某个图元落在视锥体的平面之外，那么就不会被绘制
// （被剔除，culled）。但是，如果有一个图元正好穿过某个平面呢？此时 OpenGL 将会对此图元
// 进行剪切（clipping）。也就是说，它会负责计算几何体与平面的交集，然后将落入视锥体范围内
// 的形状进行计算好生成新的几何体。因为 OpenGL 需要通过剪切操作来保证绘制的正确性，所以
// 应用程序就必须告诉 OpenGL 当前视锥体的参数。有时候，我们并不需要透视视图，而需要采用
// 正交视图模型（orthographic viewing model）。这种投影方式常见于建筑设计图和计算机辅
// 助设计的领域，它的主要作用是在投影之后依然保持物体的真实大小以及相互之间的角度。我们可
// 以简单第通过忽略 x y z 三个坐标轴中的一个来实现这一效果，也就是用其余两个构成二维坐标。
// 当然，在这之前我们需要通过模型视图变换，将物体和相机都放置到场景合适的方位上。不过，在
// 那之后我们还是需要对结果模型进行定位和缩放运算，以便在归一化的设备坐标系中进行显式。
//
// 齐次坐标是用 n+1 维向量来表示 n 维空间中的点 的方法。为什么需要它？在普通三维坐标中，
// 平移不是线性变换（无法用一个 3×3 矩阵表示）。引入齐次坐标后，平移、旋转、缩放都能统一
// 为 4×4 矩阵乘法。透视除法：最终把 (x, y, z, w) 映射回三维时，除以 w，得到 (x/w, y/w,
// z/w)。这个 w 分量正是实现透视投影的核心，远处物体的 w 更大，除完后坐标被"压缩"，产生
// 近大远小的效果。将三维笛卡尔坐标转换为四维齐次坐标的两个好处，可以完成透视变换，还可以
// 使用线性变换实现模型的平移。
//      普通坐标        齐次坐标
//      (x, y, z)       (x, y, z, w=1)    表示点，可以平移
//      (x, y, z)       (x, y, z, w=0)    表示向量/方向，不受平移影响，平移无效
//
// 三维向量与 3x3 矩阵的乘法，可以实现缩放和旋转的线性变换。但是平移无法通过 3x3 矩阵乘法
// 完成，我们还需要一个额外的向量，将点 (0, 0, 0) 移动到另一个位置，这一步叫作仿射变换
// （affine transformation），它不属于线性变换，线性变换保持原点不变，总是将 (0, 0, 0)
// 映射到 (0, 0, 0)。加入这个额外的运算意味着无法在运用线性变换的优势将多个变换合并成一
// 次变换。因此我们需要一种方法，通过使用线性变换来表达平移过程。幸运的是，只要将数据置入
// 思维坐标空间，仿射变换就回归变为一种简单的线性变换。例如向量沿 y 移动 0.3，对应的转换
// 如下。同时，我们需要这个额外的分量来使用透视投影变换。齐次坐标总是有一个额外的分量，并
// 且如果所有的分量都除以一个相同的值，那么不会改变它所表达的坐标位置。这样齐次坐标其实表
// 达的是方向而不是位置，对一个方向值的缩放不会改变方向本身。透视变换会将 w 分量修改为 1.0
// 以外的值，如果 w 更大，那么坐标将位于更远的位置。当 OpenGL 准备显示几何体的时候，它会
// 使用 w 分量除以前三个分量，从而将齐次坐标重新变到三维笛卡尔坐标。因此距离更远的物体（w
// 值越大）的笛卡尔坐标也会更小，从而绘制的比例也就更小。从理论上来说，使用负数的 w 值并
// 没有错误，例如下面的坐标值表达同一个点，(2.0, 3.0, 5.0, 1.0) 和 (-2.0, -3.0, -5.0,
// -1.0)，但是负数 w 值可能会给图形管线的某些环节带来麻烦，尤其是可能会与其他整数 w 值进
// 行插值计算，得到的结果可能非常接近或者正好为 0.0。而 w 为 0.0 表示位于无限近的位置，
// 物体与观察点无限近导致它的透视效果无限大，这样可能会产生无法预知的结果。要避免这个问题，
// 最简单的方法就是保证 w 值总是正数。
//      [1.0 0.0 0.0 0.0] ( x )    ( x )
//      [0.0 1.0 0.0 0.3] ( y ) => ( y + 0.3 )
//      [0.0 0.0 1.0 0.0] ( z )    ( z )
//      [0.0 0.0 0.0 1.0] (1.0)    (1.0)
//
// 线性变换是满足以下两个条件的映射 T，保持原点不变、直线仍然是直线、平行线仍然平行。
//      可加性：T(u + v) = T(u) + T(v)
//      齐次性：T(c·v) = c·T(v)
//
// 三维图形学中的常见线性变换如下，多个变换可以预先相乘合成一个矩阵 M = M₃ × M₂ × M₁，
// 对大量顶点只需一次矩阵乘法。齐次坐标用"多一维"的技巧把平移也变成矩阵乘法，线性变换用
// 矩阵统一描述几何变形，两者配合实现了三维到二维的高效投影流水线。
//      变换    矩阵特点               说明
//      缩放    对角矩阵               沿各轴拉伸/压缩
//      旋转    正交矩阵               绕 X/Y/Z 轴旋转
//      错切    非对角非对称            倾斜变形
//      平移    4×4 矩阵（齐次坐标下）  普通三维中不是线性变换，齐次坐标下是
//
// 矩阵计算：
//  v' = Av                     向量经过一次变换（矩阵 A）变成向量 v'
//  v'' = Bv' = BAv             向量经过两次变换（矩阵 A 和 B）变成向量 v''
//  v'' = Cv  c = BA            矩阵 A 和 B 可以合并成一个矩阵 C，其中 cij = bi1 a1j + bi2 a2j + bi3 a3j + bi4 a4j
//  AB != BA                    矩阵乘法不满足交换率
//  Av != vA                    矩阵与向量的乘法不满足交换率
//  C(B(Av))==C(BA)v==(CBA)v    矩阵乘法满足结合率
//
// 平移变换：物体平移 t_x t_y t_z，变换矩阵为 T，坐标 v' = Tv。
//  T = [1.0 0.0 0.0 t_x]   v' = (x + t_x)
//      [0.0 1.0 0.0 t_y]        (y + t_y)
//      [0.0 0.0 1.0 t_z]        (z + t_z)
//      [0.0 0.0 0.0 1.0]        (    1.0)
//
// 缩放变换：在转换矩阵对角线分量设置合适的缩放值，可以实现物体的放大和压缩。变换矩阵 S，
// 假设缩放值为 s_x s_y s_z，坐标 v' = Sv。注意，如果这个物体并不在原点上，那么缩放会
// 物体远离或逼近原点。缩放值可以针对每个分量，很容易实现非同型的缩放变换，但是在模型视图
// 变换时我们很少这样做，如果需要将结果进行垂直或水平拉伸，可以在视图变换完之后再进行，过
// 早进行缩放会导致物体在旋转时变形。注意当进行缩放时，不会影响 w 的值，物体平移变换也不
// 会改变 w 的值。如果物体缩放时其中心没有处于原点处，那么这个简单的矩阵在缩放的同时也会
// 将物体推远或拉近。通常来说，为了比较容易理解操作的过程，我们可以先将物体放置在原点。此
// 时缩放在改变物体大小的时候不会再影响到它的位置。如果需要改变一个偏离中心的物体的大小，
// 并且不希望它的位置发生改变，可以首先将物体中心平移到原点，再进行缩放，最后平移回原来的
// 位置。这里用到三个矩阵 T、S、和 T'，坐标 v' = T'STv = (T'ST)v 。
//  S = [s_x 0.0 0.0 0.0]   T'ST = [1.0 0.0 0.0 -t_x] [s_x 0.0 0.0 0.0] [1.0 0.0 0.0 t_x] = [s_x 0.0 0.0 -t_x] [1.0 0.0 0.0 t_x] = [s_x 0.0 0.0 s_x * t_x - t_x]
//      [0.0 s_y 0.0 0.0]          [0.0 1.0 0.0 -t_y] [0.0 s_y 0.0 0.0] [0.0 1.0 0.0 t_y]   [0.0 s_y 0.0 -t_y] [0.0 1.0 0.0 t_y]   [0.0 s_y 0.0 s_y * t_y - t_y]
//      [0.0 0.0 s_z 0.0]          [0.0 0.0 1.0 -t_z] [0.0 0.0 s_z 0.0] [0.0 0.0 1.0 t_z]   [0.0 0.0 s_z -t_z] [0.0 0.0 1.0 t_z]   [0.0 0.0 s_z s_z * t_z - t_z]
//      [0.0 0.0 0.0 1.0]          [0.0 0.0 0.0  1.0] [0.0 0.0 0.0 1.0] [0.0 0.0 0.0 1.0]   [0.0 0.0 0.0  1.0] [0.0 0.0 0.0 1.0]   [0.0 0.0 0.0             1.0]
//
//  v' = (x * s_x)           v' = (x * s_x + s_x * t_x - t_x) = ((x + t_x) * s_x - t_x)
//       (y * s_y)                (y * s_y + s_y * t_y - t_y)   ((y + t_y) * s_y - t_y)
//       (z * s_z)                (z * s_z + s_z * t_z - t_z)   ((z + t_z) * s_z - t_z)
//       (    1.0)                (                      1.0)   (                  1.0)
//
// 旋转变换：模型坐标是右手系，物体绕 x 轴旋转（从 y 轴看向 z 轴的右手系逆时针旋转）r_x
// 弧度，绕 y 轴旋转 （从 x 轴看向 z 轴的右手系逆时针旋转）r_y 弧度，绕 z 轴旋转（从 x
// 轴看向 y 轴的右手系逆时针旋转）r_z 弧度，变换矩阵分别为 Rx Ry Rz。在旋转时，如果物体
// 本身偏离原点，那么这一操作会导致物体整体沿着各轴转动发生位移，出现物体位置的改变。与缩
// 放变换时类似，我们可以首先将物体中心放置到原点上，再执行旋转变换，最后移回原位，这样坐
// 标经过变换变为 v' = T'RTv。在所有这些情况下，旋转都是沿着第一个轴的方向看向第二个轴的
// 方向，也就是说按照各轴正向的顺序，从 cosθ -sinθ 形式的一行旋转到 sinθ cosθ 的行。
//  Rx = [1.0  0.0       0.0       0.0]    Ry = [cos(r_y)  0.0  -sin(r_y) 0.0]    Rz = [cos(r_z)  -sin(r_z)  0.0  0.0]
//       [0.0  cos(r_x)  -sin(r_x) 0.0]         [0.0       1.0  0.0       0.0]         [sin(r_z)  cos(r_z)   0.0  0.0]
//       [0.0  sin(r_x)  cos(r_x)  0.0]         [sin(r_y)  0.0  cos(r_y)  0.0]         [0.0       0.0        1.0  0.0]
//       [0.0  0.0       0.0       1.0]         [0.0       0.0  0.0       1.0]         [0.0       0.0        0.0  1.0]
//
//  v' = (x                          )      v' = (x * cos(r_y) - z * sin(r_y))      v' = (x * cos(r_z) - y * sin(r_z))
//       (y * cos(r_x) - z * sin(r_x))           (y                          )           (x * sin(r_z) + y * sin(r_z))
//       (y * sin(r_x) + z * cos(r_x))           (x * sin(r_y) + z * cos(r_y))           (z                          )
//       (                        1.0)           (                        1.0)           (                        1.0)
//
//  T'RT = [1.0 0.0 0.0 -t_x] [1.0  0.0       0.0       0.0] [1.0 0.0 0.0 t_x] = [1.0  0.0       0.0       -t_x] [1.0 0.0 0.0 t_x] = [1.0  0.0       0.0                                          0.0]
//         [0.0 1.0 0.0 -t_y] [0.0  cos(r_x)  -sin(r_x) 0.0] [0.0 1.0 0.0 t_y]   [0.0  cos(r_x)  -sin(r_x) -t_y] [0.0 1.0 0.0 t_y]   [0.0  cos(r_x)  -sin(r_x)  cos(r_x) * t_y - sin(r_x) * t_z - t_y]
//         [0.0 0.0 1.0 -t_z] [0.0  sin(r_x)  cos(r_x)  0.0] [0.0 0.0 1.0 t_z]   [0.0  sin(r_x)  cos(r_x)  -t_z] [0.0 0.0 1.0 t_z]   [0.0  sin(r_x)  cos(r_x)   sin(r_x) * t_y + cos(r_x) * t_z - t_z]
//         [0.0 0.0 0.0  1.0] [0.0  0.0       0.0       1.0] [0.0 0.0 0.0 1.0]   [0.0  0.0       0.0        1.0] [0.0 0.0 0.0 1.0]   [0.0  0.0       0.0                                          1.0]
//
//  v' = (x                                                )
//       ((y + t_y) * cos(r_x) - (z + t_z) * sin(r_x) - t_y)
//       ((y + t_y) * sin(r_x) + (z + t_z) * cos(r_x) - t_z)
//       (                                              1.0)
//
// 模型变换是将模型放入世界坐标，视图变换是从世界坐标的某一角度去观察世界，这个观察视角可
// 以通过 lookat 的三个向量 vec3 eye center up 定义。eye 相当于我站在世界坐标的哪个位
// 置（定位置），center 表示世界坐标上的观察目标点（定朝向），up 表示哪边是上表示你头顶
// 朝天还是朝地（定翻滚），是相机的上方向，通常是归一化的世界坐标。lookat 的本质是构造一
// 个视图变换的观察矩阵（view matrix）。
//
// 透视投影（perspective projection）：透视变换过程相对复杂一些，我们将考虑以下两种情形，
// 对于所有情况而言，观察点总是位于原点，并且朝着 z 轴正方向看。
//  1.  中心对称的视锥体，z 轴位于锥体的中心位置
//  2.  不对称的视锥体，就好像我们靠近窗口观察景色，但没有正对窗户中心
//
// 不过，首先我们考虑一种理想的简化透视投影，注意到矩阵最后一个分量替换为了 z 坐标。这样
// z 值更大的物体（距离更远）看起来显得更小，因为所有分量都会除以 w，这样就实现了透视效
// 果。但是，这种特殊方法有一些缺陷，例如此时所有的 z 值结果都将是 1.0 丢失了深度信息。
// 而我们对投影锥体以及矩形也不能进一步控制，最后我们并没有将结果缩放到视口变换所需的-1.0
// 到 1.0 范围之内。
//      [1.0 0.0 0.0 0.0] ( x ) = (x)
//      [0.0 1.0 0.0 0.0] ( y )   (y)
//      [0.0 0.0 1.0 0.0] ( z )   (z)
//      [0.0 0.0 1.0 0.0] (1.0)   (z)
//
// 现在考虑一个更实际的 OpenGL 模型，并且使用中心对称的视锥体，人眼观察位置在原点，看向
// z 轴正方向，近平面的 z 坐标为 z_near，远平面 z_far，近平面矩阵宽度和高度 width 和
// height。锥体内的坐标 (x y z) 投影到近平面为 (x_proj y_proj z_near)，由于 z_near/z
// = x_proj/x = y_proj/y，因此得到 x_proj y_proj 如下。将其都归一化到 [-1.0, 1.0]，
// 相当于 [0, width/2] 和 [0, height/2] 归一化到 [0, 1.0]，得到的 x_ndc y_ndc 如下。
// 代入得到，当 x_proj = 0 时映射到 x_ndc = 0，当 x_proj = width/2 时映射到 x_ndec
// = 1.0，因此映射成立。
//      x_proj = x * z_near / z
//      x_proj = y * z_near / z
//      x_ndc = x_proj / (width / 2) = x * z_near / (z * width / 2)
//      y_ndc = y_proj / (height/ 2) = y * z_near / (z * height/ 2)
//
//      [n/(w/2)  0.0      0.0  0.0] ( x ) = (x*n/(w/2))
//      [0.0      n/(h/2)  0.0  0.0] ( y )   (y*n/(h/2))
//      [0.0      0.0      1.0  0.0] ( z )   (z        )
//      [0.0      0.0      1.0  0.0] (1.0)   (z        )
//
// 对于深度坐标 n <= z <= f，需要将 [n, f] 归一化到 [-1.0, 1.0]，并且根据以上转换矩阵，
// 最后需要除以 z，因此假设归一化公式 z_ndc = (A * z + B) / z，将 n 和 f 代入得到：
//      (A * n + B) / n = -1.0
//      (A * f + B) / f = 1.0
//      A * (f - n) = f + n
//      A = (f + n) / (f - n)
//      B = f - A * f = f - (f * f + f * n) / (f - n) = (f * f - f * n - f * f - f * n) / (f - n) = -2 * f * n / (f - n)
//
//      [n/(w/2)  0.0      0.0                   0.0] ( x ) = (x*n/(w/2))
//      [0.0      n/(h/2)  0.0                   0.0] ( y )   (y*n/(h/2))
//      [0.0      0.0      (f+n)/(f-n)  -2*f*n/(f-n)] ( z )   (A*z+B    )
//      [0.0      0.0      1.0                   0.0] (1.0)   (z        )
//
// 最后需要将右手系观察坐标转换成左手系的屏幕坐标，需要将观察方向看向 z 轴的负方向，并将
// z 坐标的符号反向，即要将 z 转换成 -z，n 和 f 都加上一个负号。
//      z_ndc = (A * z + B) / z => (-A * z + B) / -z
//      A = (f+n)/(f-n) => -(f+n)/(n-f) => (f+n)/(f-n)
//      B = -2*f*n/(f-n) => -2*f*n/(n-f) => 2*f*n(f-n)
//
//      [n/(w/2)  0.0      0.0                   0.0] ( x ) = (x*n/(w/2))
//      [0.0      n/(h/2)  0.0                   0.0] ( y )   (y*n/(h/2))
//      [0.0      0.0      -(f+n)/(f-n)  2*f*n/(f-n)] ( z )   (-A*z+B   )
//      [0.0      0.0      -1.0                  0.0] (1.0)   (-z       )
//
// 这里再考虑第二种透视投影即不对称视锥体（asymmetric frustum），这也是更为通用的视锥体
// 类型，此时近平面的中心可能不在 z 轴上，z 轴甚至有可能完全在近平面之外。对称视锥体，左
// 右边界 left = -right，上下边界 top = -bottom，而不对称视锥体左右边界、上下边界可以都
// 不相等。不对称视锥体可以实现斜投影（投影到倾斜的墙面）、多显示器拼接、光学偏移、移轴效
// 果（tilt shift）、左右眼视锥体 IPD 瞳距。这种情况下，近平面上的位置依然是正确的，但是
// 由于近平面上的投影已经不再居中，所以远离近平面的点都需要重新进行调整，它实际上可以将点
// 沿着偏离近平面投影中心的方向移动一定距离，并且根据点的远近进行缩放（因为这一列都要乘以
// z）。这样，一个视锥体可以唯一由六个参数 float left right bottom up near far 决定。
//
//      [n/(w/2)  0.0      (l+r)/(w/2)           0.0] ( x ) = [n/(r-l)/2  0.0        (l+r)/(r-l)/2          0.0] ( x )
//      [0.0      n/(h/2)  (t+b)/(h/2)           0.0] ( y )   [0.0        n/(t-b)/2  (t+b)/(t-b)/2          0.0] ( y )
//      [0.0      0.0      -(f+n)/(f-n)  2*f*n/(f-n)] ( z )   [0.0        0.0        -(f+n)/(f-n)   2*f*n/(f-n)] ( z )
//      [0.0      0.0      -1.0                  0.0] (1.0)   [0.0        0.0        -1.0                   0.0] (1.0)
//
// 如上所述，模型变换（Model）、视图变换（View）、投影变换（Project）可将矩阵相乘合并为
// 一个单一的 MVP 矩阵。透视投影到屏幕上的最后一个步骤是遍历每个顶点，除掉其中的 w 坐标
// 分量，这一步是 OpenGL 内部完成的，不需要再着色器中完成这项工作。
//
// 正交投影（orthographic projection）：正交投影得到的观察体形状是一个矩形的平行六面体，
// 或者说是一个盒子。与透视投影不同的是，这个观察体远近两端的尺寸不会发生变化，因此物体与
// 相机的距离不会造成物体大小的变化。因此坐标 (x, y, z) 投影到近屏幕上还是 (x, y, z_near)，
// 但是从平行六面体的正面直接进行投影，x y z 都需要经过缩放以分别归一化到 [-1, 1] [-1, 1]
// [0, 1] 的范围内，这一过程可以通过顶点除以六面体的宽度、高度和深度来完成。对于对称观察
// 体来说，z 轴正方向穿过平行六面体的中心位置。而不对称观察体，z 轴正方向没有穿过观察体中
// 心，但仍然平行于观察模型的 z 轴。
//
//      [1/(r-l)/2  0.0        0.0          -(r+l)/(r-l)] ( x ) = (x/(r-l)/2 -  (r+l)/(r-l)) = (( 2x-r-l)/(r-l))
//      [0.0        1/(t-b)/2  0.0          -(t+b)/(t-b)] ( y )   (y/(t-b)/2 -  (t+b)/(t-b))   (( 2y-t-b)/(t-b))
//      [0.0        0.0        -1/(f-n)/2   -(f+n)/(f-n)] ( z )   (-z/(f-n)/2 - (f+n)/(f-n))   ((-2z-f-n)/(f-n))
//      [0.0        0.0        0.0                   1.0] (1.0)   (                     1.0)   (            1.0)
//
// 法线变换（transforming normals）：除了顶点的变换之外，还需要对表面法线进行变换，法线
// 是方向与物体表面垂直的一个向量。法线需要进行归一化，也就是它的长度必须为 1.0，归一化主
// 要是为了光照计算的目的。通常来说，当计算光照时，每个顶点都会关联一个法线数据，这样在光
// 照的计算过程中，我们就可以知道物体表面的哪个方向回反射光线。法线向量通常是只有三个分量
// 的量，没有使用齐次坐标。我们要注意一件事，就是物体表面的平移不会影响到法线的值，因此法
// 线不需要考虑平移操作，这也是不使用齐次坐标的原因之一。由于法线的主要作用是计算光照，而
// 这一步通常是在透视变换之前完成的，因此这就是我们不需要使用齐次坐标的另一个原因。与直觉
// 相违背的是，法向量并不像顶点或位置向量那样进行变换。想象一个倾斜角度的表面被某个变换拉
// 伸，拉伸会使表面的角度变得更平缓，这会对法向量施加相同拉伸相反的方式改变垂直方向。例如，
// 如果你将一个球体拉伸成椭圆，就会发生这种情况。我们需要推导出一个与顶点变换矩阵不同的矩
// 阵来变换法向量。
//
// 那么如何变换法线呢？我们首先令 M 为一个 3x3 矩阵，它已经包含必要的旋转和缩放信息，可以
// 将物体从模型坐标变换到观察坐标，但是不包含投影变换信息。这也就是之前的 4x4 变换矩阵的左
// 上 3x3 子矩阵，没有包含平移或者透视变换的运算。然后我们需要使用下面的方程式来完成法线变
// 换： n' = M逆矩阵的转置 * n。也就是说，我们取 M 的逆矩阵的转置来完成法线的变换，如果变
// 换的内容只涉及旋转和等轴缩放（不产生形态的变化），那么只需要使用 M 来变换方向。
//
// OpenGL 矩阵。由于着色器可以完成矩阵的乘法运算，因此不需要使用 OpenGL 核心模式的 API
// 去进行矩阵运算，只需要将它们通过 uniform 和逐顶点的属性设置给着色器即可。我们自己可以
// 根据着色器中的具体需求去构建矩阵，也可以直接使用辅助数学库在 CPU 端计算出矩阵。在应用
// 程序中我们可能需要预先对矩阵做乘法运算，然后再设置给着色器，这样再性能上更有优势。举例
// 来说，假设现在需要使用矩阵来完成以下的变换过程：将相机移动到正确的观察位置，平移、旋转；
// 将模型移动到世界中，平移、旋转、缩放；应用透视投影。这里总共有 6 个矩阵，我们可以使用
// 顶点着色器来完成所有的数学运算。
//
//      #version 430 core
//      uniform mat4 view_t, view_r, model_t, model_r, model_s, proj;
//      in vec4 vertex;
//      void main() {
//          gl_Position = proj * model_s * model_r * model_t * view_r * view_t * vertex;
//      }
//
// 然而，对每个顶点都进行如此大量的算术运算是相当繁琐的。幸运的是，对于许多顶点而言，中间
// 结果每次都是相同的。如果连续变换（矩阵）在大量顶点中保持不变，你应当改为在应用程序中预
// 先计算它们的复合（乘积），然后将单一的最终矩阵发送给着色器。一种中间情况可能是：使用单
// 一的视图变换和单一的透视投影，但存在多个模型变换。如果你重复使用同一个模型，在同一场景
// 中绘制某个对象的多个实例，就可能采用这种方式。在这种情况下，应用程序会比其他矩阵更频繁
// 地更改模型矩阵。只有当每次更改模型矩阵时绘制的顶点数量足够多，这样做才是经济的。如果每
// 个模型实例仅绘制少量顶点，那么将模型矩阵作为顶点属性发送会更快。另一种创建多个实例的方
// 法是在顶点着色器中基于内置变量 gl_InstanceID 构造模型变换，这在使用 OpenGL 绘图章节
// 中有详细描述。
//
//      #version 430 core
//      uniform mat4 view, model, project;
//      in vec4 vertex;
//      void main() {
//          gl_Postion = view * model * project * vertex;
//      }
//
//      #version 430 core
//      uniform mat4 view, project;
//      in vec4 vertex;
//      in mat4 model; // 在顶点属性中保存模型变换矩阵
//      void main() {
//          gl_Position = view * model * project * vertex;
//      }
//
// 当然，当你能够用相同的累积变换绘制大量顶点时，你自然希望只在着色器中执行一次乘法运算。
//
//      #version 430 core
//      uniform mat4 model_view_project;
//      in vec4 vertex;
//      void main() {
//          gl_Position = model_view_project * vertex;
//      }
//
// 这里考虑的矩阵并不考虑数据是如何设置的，所说的矩阵的一列，总是指数学形式上矩阵垂直方向
// 的一组数据。在着色器中，可以使用数组语法获取矩阵的一列，例如 matrix[1] 表示矩阵的第二
// 列数据。这些都与矩阵的列主序或者行主序无关，数据在内存中的排列方式与 GLSL 中的语法操作
// 是无关的，事实上我们并不需要知道矩阵内部到底是按照列主序还是行主序记录的。只有一种情形
// 下我们需要考虑矩阵的列主序或者行主序，那就是将 GLSL 矩阵放入自定义的内存映射。当将矩阵
// 传递到 uniform 块中时，就需要考虑这个问题，当管理 uniform 块时为了通知 GLSL 从内存中
// 正确加载矩阵数据，需要使用布局限定符 row_major 或 column_major。由于 OpenGL 不负责
// 创建和解析矩阵数据，因此我们可以根据需要自行处理。如果需要使用矩阵乘法来完成矩阵的变换，
// 并且矩阵位于方程式的右侧，如下还是会得到预期的结果，GLSL 会将向量看成一行数据，然后分
// 别与矩形每列相乘，gl_Position 的 x、y、z、w 分量依次是与第 1 列、第 2 列、第 3 列、
// 第 4 列的点乘。即 GLSL 会根据向量在矩阵的左边还是右边，自动决定将向量处理为行行向量还
// 是列向量。这种时候，向量就与单列或者单行的矩阵不同。
//
//      #version 430 core
//      uniform mat4 matrix;
//      in vec4 vertex;
//      void main() {
//          gl_Position = vertex * matrix; // 非常规乘法顺序
//      }
//
// 如果我们需要通知 OpenGL 在何处放置近平面和远平面，可以调用 glDepthRange()，另外可以
// 调用 glViewport() 指定视口，其中 x 和 y 表示视口的左下角坐标，width 和 height 设置
// 了视口大小，默认情况下视口初始值为 (0, 0, win_width, win_height)，其中 win_width
// 和 win_height 为窗口的像素尺寸。默认情况下视口设置为打开窗口的真个像素区域，我们可以
// 使用 glViewport() 选择一个更小的绘制区域，例如分隔窗口产生多个视图。有时候我们需要使
// 用多个视口（multiple viewports）来完成一个场景的渲染，OpenGL 提供了相应的命令支持，
// 并且可以在几何着色阶段选择具体要进行绘制的视口。见后面几何渲染部分的多视口与分层渲染
// （multiple viewports and layered rendering）。
//
// 高级技巧：z 的精度。矩阵变换可能产生一个不太理想的效果，就是 z-fighting。计算过程中，
// 硬件浮点数精度支持是有限的，因此有时候虽然在数学上深度坐标应该是不同的，但硬件中最终记
// 录的浮点数 z 值可能是相同的，甚至与实际结果相反。这样就会造成深度缓存中的隐藏面计算结
// 果不正确。由于这个现象可能多个像素都有影响，因而导致相互距离较为接近的物体会发生闪烁交
// 叠的情形。经过透视变换之后，z 的精度问题可能会恶化，无论对于深度坐标还是其他类型的坐标
// 值都是如此，此时如果深度坐标远离近剪切平面，那么它的位置精度将越来越低。就算没有经过透
// 视变换，浮点数的精度也是有限的，而透视的结果会导致它恶化并且称为非线性的形式，这样在深
// 度值较大的时候会带来更多的问题。这个问题的根源是我们在一个过小的 z 值区域内绘制了过多
// 的数据。如果要避免这个问题，我们需要尽量将远平面与近平面靠近，并且尽可能不要在一个很小
// 的区域内绘制过多的 z 值。
//
// 高级技巧：用户剪切（user culling and clipping）。OpenGL 会自动根据视口和近平面与远
// 平面的设置来剪切几何体。用户剪切的意思是再添加一些任意方向的剪切平面来剪切几何数据，例
// 如只允许几何体在平面的一侧可见，而另一侧不可见。OpenGL 的用户剪切操作需要用到一个特殊
// 的内置顶点着色器数组 gl_CullDistance 和 gl_ClipDistance，我们需要自行设置它的内容，
// 这些变量允许我们控制剪切平面与顶点的关系。它的值经过插值之后设置给顶点之间的各个片元，
// 下面是 gl_ClipDistance 一种最直接的简单用法。
//
//      #version 430 core
//      uniform vec4 plane; // 平面方程 Ax + By + Cz + D = 0 的四个系数
//      in vec4 vertex; // w = 1.0
//      float gl_ClipDistance[1]; // 使用一个剪切平面
//      void main() {
//          gl_ClipDistance[0] = dot(vertex, plane); // 计算平面方程
//          // 或者使用 gl_CullDistance[0] 用于剔除
//      }
//
// 约定是：距离为 0 表示顶点位于平面上；正距离表示顶点位于裁剪平面或剔除平面的内侧（保留
// 侧）；负距离表示顶点位于裁剪平面或剔除平面的外侧（剔除侧）。这些距离将在图元上线性插值。
// OpenGL 会剔除完全位于任意一个剔除平面外侧的整个图元，保留图元的空间是所有剔除平面内侧
// 的交集。此外，OpenGL 还会剔除所有插值后裁剪距离小于 0 的片段。gl_ClipDistance 数组的
// 每个元素以及 gl_CullDistance 数组的每个元素都代表一个平面。平面的数量是有限的，通常约
// 为八个或更多，这些数量通常必须在 gl_ClipDistance 元素和 gl_CullDistance 元素之间共
// 享。也就是说，你可能有八个裁剪距离可用，或八个剔除距离可用，或者各四个，或者一种两个另
// 一种六个，但两个数组的总数永远不会超过八个。通常情况下，允许的总数由 gl_MaxCombinedClipAndCullDistances
// 给出，允许用于剔除的数量是 gl_MaxCullDistances，允许用于裁剪的数量是 gl_MaxClipDistances。
// 请注意，这些内置变量在声明时没有指定大小，但使用的平面数量（数组元素）由着色器决定。这
// 意味着你必须用特定大小重新声明这些数组，或者仅使用编译时常量索引来访问它们，这个大小决
// 定了有多少个平面参与运算。
//
// 所有阶段中声明或使用 gl_ClipDistance[] 的所有着色器都应使该数组保持相同大小，gl_CullDistances[]
// 同理。这个大小需要包含通过 OpenGL API 启用的所有裁剪平面；如果大小未包含所有启用的平
// 面，结果将是未定义的。要在应用程序中启用上例中写入的裁剪平面的 OpenGL 裁剪功能，需要
// 启用以下枚举值：glEnable(GL_CLIP_PLANE0); 还有其他枚举值，如 GL_CLIP_PLANE1、GL_CLIP_PLANE2。
// 这些枚举值按顺序组织，因此 GL_CLIP_PLANEi 等于 GL_CLIP_PLANE0 + i。这允许以编程方式
// 选择使用哪些用户裁剪或剔除平面以及使用多少个。你的着色器应当写入所有启用的平面，否则会
// 出现奇怪的实现行为。内置变量 gl_CullDistance 和 gl_ClipDistance 在片段着色器中同样
// 可用，允许片段读取来自每个平面的插值距离。
//
// 控制 OpenGL 变换。OpenGL 的许多固定功能操作发生在裁剪空间中，即你的顶点着色器（或如果
// 启用了细分着色器或几何着色器）生成坐标的空间。默认情况下，OpenGL 将裁剪空间坐标 (0, 0)
// 映射到窗口空间的中心，正 x 坐标指向右方，正 y 坐标指向上方。这使得 (–1, –1) 坐标位于
// 窗口的左下角，(1, 1) 位于右上角。可以将其想象成一张方格纸：在数学、建筑学和其他工程领
// 域中，正 y 方向是向上的。然而，许多图形系统将正 y 视为向下，这是早期阴极射线管扫描电子
// 束方式的遗留产物，视频内存中的数据布局意味着让 y 指向下方更为方便。此外，为了一致性和
// 正交性，正如范围 –1.0 到 1.0 映射到可见的 x 和 y 范围一样，–1.0 到 1.0 也映射到可见
// 的深度范围，其中 –1.0 是近裁剪面，1.0 是远裁剪面。不幸的是，由于浮点数的工作方式，大部
// 分精度集中在 0.0 附近，而 0.0 位于远离观察者的某处，尽管我们实际上希望在靠近观察者的近
// 裁剪面附近拥有大部分深度精度。同样，其他一些图形系统使用另一种映射方式，其中裁剪空间中
// 的负 z 坐标位于观察者后方，可见深度范围映射到裁剪空间中的 0.0 到 1.0。OpenGL 允许使用
// 单次调用 glClipControl() 重新配置上述一种或两种映射，即修改裁剪坐标到窗口坐标的映射执
// 行方式。origin 设置窗口中 x 和 y 的原点，depth 指定裁剪空间深度值范围如何映射到 glDepthRange()
// 指定的值。origin 应为 GL_LOWER_LEFT 或 GL_UPPER_LEFT 之一。如果 origin 是 GL_LOWER_LEFT，
// 裁剪空间 xy 坐标 –1.0, –1.0 映射到窗口的左下角，裁剪空间中正 y 增量向窗口空间的上方移
// 动。如果 origin 是 GL_UPPER_LEFT，裁剪空间 xy 坐标 –1.0, –1.0 映射到窗口空间的左上
// 角，裁剪空间中正 y 增量向窗口空间的下方移动。如果 depth 是 GL_NEGATIVE_ONE_TO_ONE，
// 窗口空间中可表示的深度范围在裁剪空间中编码为 –1.0 到 1.0 的范围，–1.0 映射到近裁剪面，
// 1.0 映射到远裁剪面。如果 depth 是 GL_ZERO_TO_ONE，裁剪空间中 0.0 到 1.0 的范围映射
// 到窗口空间中可表示的深度值范围，0.0 是近裁剪面，1.0 是远裁剪面。因此，裁剪空间中的负 z
// 值被视为位于近裁剪面后方，但在靠近观察者附近可获得更高的精度。透视投影的 Z 归一化，z_eye
// 线性（-n 到 -f），z_ndc 非线性（0 到 1），大量精度挤在 0 处，远处精度稀疏。即近处精度
// 高，大部分精度集中在近裁剪面附近；远处精度低，远离相机的物体深度区分能力差（z-fighting）。
// 由于非线性分布，近裁剪面不能设太近，例如近裁剪面 0.1 远裁剪面 1000，90% 的深度精度花在
// [0.1, 10] 范围内，远处几乎无区分能力，而近裁剪面 1.0 远裁剪面 1000，分布更均匀，z-fighting
// 显著减少。反向 Z（Reverse Z），现代图形学常用技巧，将 NDC Z 反向映射，让近处对应 1.0，
// 远处对应 0.0，depth = 1.0 - (z_ndc + 1) / 2 或配合浮点深度缓冲。配合浮点深度缓冲
// GL_DEPTH_COMPONENT32F 时，由于浮点数在 0 附近精度更高，反向 Z 能极大改善深度精度。
//
// void glDepthRange(GLdouble near, GLdouble far);
// void glDepthRangef(GLdouble near, GLdouble far);
// void glViewport(GLint x, GLint y, GLint width, GLint height);
// void glClipControl(GLenum origin, GLenum depth);
//
// 变换反馈（transform feedback）。变换反馈是 OpenGL 管线中，顶点处理阶段结束之后，图元
// 装配和光栅化之前的一个步骤。更准确的说，变换反馈是与图元装配过程紧密结合的，这是因为整
// 个图元数据都会被捕获到缓存对象中。而这里我们可以认为是缓存的空间不够，因此必须丢弃一部
// 分图元。为了确保这一过程，需要在变换反馈阶段给出当前的图元类型信息。变换反馈可以重新捕
// 获即将装配为图元（点、线段、三角形）的顶点，然后将它们的部分或者全部属性传递到缓存对象
// 中。实际上，最小的 OpenGL 管线就是一个顶点着色器加上变换反馈的组合，这里不一定用到片元
// 着色器。每当一个顶点传递到图元装配阶段时，将所有需要捕获的属性数据记录到一个或者多个缓
// 存对象中。用户程序可以回读这些缓存对象的内容，或者 OpenGL 将它们用户后续的渲染工作。
//