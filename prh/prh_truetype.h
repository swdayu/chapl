// prh_truetype.h - v0.01 - public domain - swdayu <github.com/swdayu>
// No warranty implied, use at your own risk.

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   INTERFACE
////
////

#ifndef PRH_IMPL_TRUETYPE_INCLUDED_H
#define PRH_IMPL_TRUETYPE_INCLUDED_H

#define PRH_FILE_INCLUDE
#define PRH_ARRAY_INCLUDE
#include "prh_include.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // PRH_IMPL_TRUETYPE_INCLUDED_H

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
////
////   IMPLEMENTATION
////
////

#ifdef PRH_TRUETYPE_IMPLEMENTATION

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
// https://github.com/adobe-fonts/source-han-serif/tree/release#downloading-source-han-serif
//
// 可变字体和静态字体
//      特性        可变字体（Variable Font）                       静态字体（Static Font）
//      文件数量    一个文件包含全部字重                            每个字重一个文件（如 Regular、Bold、Light 各一个）
//      字重调节    连续可调（如 100-900 任意值）                   固定档位（通常 7 种：ExtraLight、Light、Regular、Medium、SemiBold、Bold、Heavy）
//      文件大小    更小（所有字重共享字形轮廓数据）                更大（多个文件总和）
//      渲染方式    运行时插值生成中间字重                          直接使用预先生成的字形
//      兼容性      需要较新系统/应用支持                           兼容性更好，老旧系统也能用
//      性能        首次加载略慢（需解析变轴数据），但缓存后正常    直接加载，无额外计算
//
// 字体的选择
//      只需要简体中文，用常规字重          静态 OTF 或 TTF
//      需要简体中文多种字重                可变 OTF 或可变 TTF
//      需要简/繁/日/韩多种语言             可变 OTC 或超级 OTC
//      系统很新（Win10 1703+/macOS 10.8+） 可变 OTC（最小体积）
//      系统较老，兼容性优先                静态 OTF/TTF
//      Web 使用（WOFF2）                   可变 WOFF2（压缩率更高）
//
// 单字体文件和字体集合文件
//      格式     全称                    本质           特点
//      TTF      TrueType Font           单个字体文件   使用 TrueType 轮廓的单个字体文件（例如）
//      OTF      OpenType Font           单个字体文件   最基础的格式，一个文件 = 一种语言的一个字重（例如 SourceHanSerifSC-Regular.otf）
//      OTC      OpenType Collection     字体集合       一个文件包含多个字体（如 5 种语言 × 7 字重 = 35 个字体在一个文件里，例如 SourceHanSerif.ttc）
//      TTC      TrueType Collection     字体集合       原始名称，由 TrueType 规范定义，OTC 和 TTC 本质上是同一个东西，OpenType 标准沿用
//
// 字体格式 OTF 和 TTF 的区别
//      功能特性        OTF（CFF 轮廓）                 TTF（TrueType 轮廓）
//      曲线数学        三次贝塞尔曲线（Cubic Bezier）  二次贝塞尔曲线（Quadratic Bezier）
//      文件大小        通常更小（曲线更紧凑）          通常更大
//      渲染 hinting    依赖 PostScript 渲染器          依赖 TrueType 解释器
//      屏幕小字号      可能略逊                        通常更好（hinting 更成熟）
//      打印/专业排版   更优                            良好
//
// 思源宋体是一款开源的泛中日韩（Pan-CJK）字体，其 OpenType/CFF 字体和基于 CID 的源文件
// 均受 SIL 开源字体许可证 1.1 版条款保护（另请参阅 LICENSE 和 FAQ）。在此您可以找到可直
// 接安装的 OpenType/CFF 字体资源，既可单独下载单个字体文件，也可打包下载 ZIP 压缩文件。
// 字体的 CID 源文件和构建说明可在仓库的主分支中获取。http://scripts.sil.org/OFL
//
// 可变 OTC 字体（Variable OTCs）。如果您的系统同时支持可变字体和 OTC 格式，且需要使用              *** 02_SourceHanSerif-VF\Variable\OTC
// 全部五种语言及全部字重，请选择此部署格式。注意：不存在区域子集可变 OTC 字体，因为其
// 文件大小会大于语言特定 OTC，从而失去文件大小优势。
// https://github.com/adobe-fonts/source-han-serif/raw/release/Variable/OTC/SourceHanSerif-VF.otf.ttc
// https://github.com/adobe-fonts/source-han-serif/raw/release/Variable/OTC/SourceHanSerif-VF.ttf.ttc
//
// 语言特定可变字体（Language-specific Variable Fonts）。如果您的系统支持可变字体，且您只           *** 02_SourceHanSerif-VF\Variable\[TTF|OTF|WOFF2]\
// 使用一种语言，但同时需要完整的字符覆盖范围，或希望通过对文本进行语言标记来使用适合其他
// 语言的字形（这需要支持语言标记和 OpenType 'locl' GSUB 特性的应用程序），请选择此部署格
// 式。语言特定字节包含完整 CJK 统一表意文字，Simplied Chinese 和 Tranditional Chinese 和
// Japanese 等都包含完整的 CJK 字符集。这三个版本的核心区别在于字形设计规范和语言特定的排
// 版习惯，而非字符覆盖范围。
//      特性            简体中文 (SC)               繁体中文—台湾 (TC)      日语 (J)
//      字形标准        中国大陆规范字形            台湾教育部标准字形      日本常用汉字表 (Jōyō)
//      简繁关系        以简体为主，繁体回退        以繁体为主，简体回退    日文汉字独立规范
//      具体字形差异    骨、角、雨等部首写法不同    与 SC 在细节笔画上有别  大量日文独有字形
//      标点位置        居左下（横排）              居中                    居中
//      引号样式        「」、""                    「」、""                「」、『』
//      假名            无                          无                      包含平假名、片假名
//      谚文            无                          无                      无
//      字符覆盖        完整 CJK                    完整 CJK                完整 CJK + 假名
//
// 具体字形差异示例
//      汉字    简体中文        台湾繁体        日语
//      骨      骨（上部开口）  骨（上部开口）  骨（上部封口，更方）
//      角      角（下部"用"）  角（下部"用"）  角（下部不同）
//      雨      雨（四点平直）  雨（四点略弯）  雨（四点独立，更竖）
//      门      门（简体）      門（繁体）      門（繁体，但内部细节不同）
//      直      直（内部三横）  直（内部三横）  直（内部两横，日文字形）
//      真      真（内部三横）  真（内部三横）  真（日文字形差异）
//
// 技术实现：通过 OpenType 'locl' 特性切换。三个版本的字形数据实际上都存储在字体中，区别
// 通过 GSUB 表和 语言标记控制：
//      /* CSS 示例 */
//      font-family: "Source Han Serif SC";  /* 简体中文字形优先 */
//      font-family: "Source Han Serif TC";  /* 台湾繁体字形优先 */
//      font-family: "Source Han Serif J";   /* 日文字形优先 */
//      /* 或通过语言标记让同一个字体自动切换 */
//      <p lang="zh-CN">骨</p>   /* 显示简体字形 */
//      <p lang="zh-TW">骨</p>   /* 显示台湾繁体字形 */
//      <p lang="ja">骨</p>      /* 显示日文字形 */
//
// 韩语（Korean）版本与中日文版本的核心区别在于文字系统的根本差异——韩语使用谚文（Hangul），
// 而非汉字。
//      特性            简体中文 (SC)   繁体中文 (TC)   日语 (J)        韩语 (K)
//      主要文字        汉字（简体）    汉字（繁体）    汉字 + 假名     谚文（Hangul）
//      谚文支持        无              无              无              完整
//      汉字覆盖        完整 CJK        完整 CJK        完整 CJK        完整 CJK + 韩语汉字
//      假名支持        无              无              平假名、片假名  无
//      汉字使用频率    高              高              中高            低（主要用谚文）
//      汉字字形标准    大陆规范        台湾/香港规范   日本常用汉字表  韩语汉字规范
//
// Unicode 中文字和韩文谚文是不同的区块如下，韩语版本必须额外包含 11,000+ 谚文码点 的设计，
// 这是其他版本完全没有的。
//      CJK Unified Ideographs（中日韩统一汉字）← 四者都包含
//      Hiragana（平假名）← 仅日语
//      Katakana（片假名）← 仅日语
//      Hangul Syllables（谚文音节）← 仅韩语
//      Hangul Jamo（谚文字母）← 仅韩语
//
// 区域子集可变字体（Region-specific Subset Variable Fonts）。如果您的系统支持可变字体，且          *** 02_SourceHanSerif-VF\Variable\[TTF|OTF|WOFF2]\Subset
// 您只需要特定区域的字符字形，请选择此部署格式。例如简体中文语言特定字体和区域子集字体的
// 区别如下。
//      特性            语言特定（Language Specific）           区域子集（Region Specific Subset）
//      字符数量        约 65,000+（完整 CJK 统一表意文字）     约 8,000-15,000（仅该区域常用字）
//      覆盖范围        中日韩越全部 CJK 字符                   仅中国大陆常用字符
//      文件大小        大（约 8-20 MB）                        小（约 2-4 MB）
//      用途            专业排版、多语言混排、古籍              日常网页、普通文档
//      生僻字          支持                                    不支持（显示为空白或回退字体）
//      旧字形/异体字   包含                                    不包含
//
// 超级 OTC（Super OTC）。如果您希望在一个易于管理的单一字体资源中获取全部五种语言和全部            *** 01_SourceHanSerif.ttc
// 七种字重（共 35 款字体），请选择此部署格式。切换语言可通过选择所需语言字体，或对文本进
// 行语言标记来实现。支持语言标记及相应 OpenType 'locl'（本地化字形）GSUB 特性的应用程序
// 数量有限，如 Adobe InDesign 和现代浏览器。相当于将所有静态字体打包。
// https://github.com/adobe-fonts/source-han-serif/releases/download/2.003R/01_SourceHanSerif.ttc.zip
//
// 特别说明：此部署格式需要 macOS（OS X）10.8（即 Mountain Lion）或更高版本、iOS 7 或更高版
// 本、Windows 10 版本 1703（即创意者更新）或更高版本、使用 fontconfig 且 FreeType 版本为
// 2.5.0.1 或更高的 Linux 发行版，或 Adobe CS6 及更高版本应用程序。
//
// OTC 字体（OTCs）。如果您需要全部五种语言和特定字重，或者您的环境不支持超级 OTC，请选择           *** 03_SourceHanSerifOTC
// 此部署格式。切换语言的方式与超级 OTC 相同。如果您需要特定字重，请从 OTC 文件夹下载单个
// 字体资源，否则请点击以下链接。相当于将所有静态字体打包到一起，但每个字重一个文件，即一
// 个文件包含了所有五种语言，但只有一个字重。
// https://github.com/adobe-fonts/source-han-serif/releases/download/2.003R/03_SourceHanSerifOTC.zip
//
// 特别说明：此部署格式需要 macOS（OS X）10.8（即 Mountain Lion）或更高版本、iOS 7 或更高
// 版本、Windows 10 版本 1607（即周年更新）或更高版本、使用 fontconfig 且 FreeType 版本为
// 2.5.0.1 或更高的 Linux 发行版，或 Adobe CS6 及更高版本应用程序。
//
// 语言特定 OTF 字体（分散的单个语言特定静态字体）。如果您只使用一种语言，但同时需要完整的          *** 04_SourceHanSerifOTF
// 字符覆盖范围，或希望通过对文本进行语言标记来使用适合其他语言的字形（与超级 OTC 和 OTC 字
// 体一样，这需要支持语言标记和 OpenType 'locl' GSUB 特性的应用程序），请选择此部署格式。
//
// 区域子集 OTF 字体（分散的单个区域子集静态字体）。如果您只需要特定区域的字符字形，或不确          *** 05_SourceHanSerifSubsetOTF
// 定选择哪种部署格式，请选择此部署格式。每个 ZIP 文件包含七款字体资源，涵盖全部七种字重。
//
// WOFF2 文件比 OTF/TTF 小的核心原因是采用了更高效的压缩算法，并针对字体数据做了专门优化。
// WOFF2 使用每个表独立用 Brotli 压缩（可选择不同压缩级别）的算法替代了旧的（WOFF 1.0）整
// 个文件统一 deflate，加上字体专用的轮廓编码优化和按表独立压缩，比 OTF 小 50-60%，比 WOFF
// 1.0 小 30% 左右。专为网页传输设计，不适合本地编辑。WOFF2 不适合本地编辑，主要有以下几个
// 原因。
//
//  1.  格式设计目标不同
//                  WOFF2               OTF/TTF
//      设计目的    网页传输、下载      本地安装、编辑、排版
//      优化方向    压缩率最大化        随机访问、快速渲染
//      数据结构    压缩后的流式数据    可直接寻址的表结构
//  2.  需要解压才能使用
//      编辑软件需要频繁随机访问字体的各个表（字形、度量、字距等），WOFF2 每次都要先解压，
//      效率极低。
//  3.  软件生态不支持，法律/许可层面 WOFF2 规范定位为传输格式，而非分发格式。
//  4.  元数据丢失或简化，WOFF2 为压缩会丢弃或简化一些本地编辑需要的信息。
//  5.  解压后的文件并不等价，专业排版需要字节级精确的字体文件，解压还原无法保证。WOFF2
//      解压 → OTF，但这个过程可能丢失原始 hinting 的精确性，表顺序可能改变，某些私有表
//      （非标准）可能无法还原。
//
// https://github.com/be5invis/Iosevka（西文等宽字体）
//
// Iosevka 支持连字，部分负号不等宽。Iosevka Term 和 Fixed 不支持连字，字符全部严格等宽：
//      特性                Iosevka                         Iosevka Term        Iosevka Fixed
//      设计目标            通用编程字体，平衡可读性与紧凑  终端/命令行优化     严格等宽，无连字
//      字符宽度            大部分等宽，但部分符号可比例    全部严格等宽        全部严格等宽
//      连字（Ligatures）   支持                            不支持              不支持
//      箭头符号宽度        -> => 可连成比例宽度            保持单个字符等宽    保持单个字符等宽
//      M W 宽度            可能略宽于标准格子              严格占一个格子      严格占一个格子
//      Powerline 符号      支持                            优化对齐            优化对齐
//      终端兼容性          良好                            最佳                良好
//
// Thin 100 ExtraLight 200 Light 300 Regular 400 Medium 500 SemiBold 600 Bold 700 ExtraBold 800 Black/Heavy 900
// 薄（bao）   超细         细体      常规        中等        半粗        粗体      超粗           黑体

// CID-Keyed sfnt Font File Format for the Macintosh Version 2.0
// Technical Note 5180, 12 February 1997
// https://adobe-type-tools.github.io/font-tech-notes/
//
// CID 键控 sfnt 字体文件格式（Macintosh 版）
//
// CID 键控字体。一种多字节 Type 1 字体的文件组织方式，字形通过字符 ID（CID）而非名称查找
// 来访问。sfnt 是 spline font（样条字体）或 scalable font（可缩放字体）的缩写。它最初由
// Apple 为 Macintosh 上的 TrueType 字体开发，作为 QuickDraw 使用的字体数据包装格式。后来，
// Adobe 在《CID-Keyed sfnt Font File Format for the Macintosh》（Adobe Technical Note 5180）
// 中扩展了该格式，用于将 CID 键控字体（CID-keyed font）嵌入到 sfnt 包装器中，以便在 Macintosh
// 系统上使用。
//
// 本文档描述 Macintosh 的 CID sfnt 字体格式。它仅描述 Adobe 对 Apple 格式所做的扩展，以容
// 纳 CID 键控字体。此外，它假定读者熟悉 Apple Computer 发布的 sfnt 规范（参见附录 B）。
//
// CID 键控 sfnt 字体由嵌入 sfnt 包装器子表中的 PostScript CID 键控字体程序（CIDFont）组成。
// CID 键控格式专为大型字符集字体（如中文、日文、韩文（CJK）语言字体）的最大灵活性和性能而
// 设计。该格式支持常规和重新排列的 CID 键控字体。
//
// 有关 CID 键控字体文件格式的更多信息，请参阅 Adobe 技术说明 5014《Adobe CMap 和 CIDFont
// 文件规范》以及 5092《CID 键控字体技术概述》。此外，Adobe 开发者协会提供 CID 软件开发工
// 具包（SDK）。
//
// 本文档仅描述少数超出 CID sfnt 字体最低要求的表。例如，ALMX 表允许为比例间距字符（proportionally
// spaced characters）指定替代度量（alternate metrics）。此外，许多其他高级功能可以通过使用
// mort 表来支持（mort 表的格式在 Apple 文档中指定）。
//
// 本文档的目的。本文档描述如何为将来可能仅支持 sfnt 格式字体的 Macintosh 操作系统制作 CID
// 键控字体。目前拥有 CID 键控字体或 PostScript 语言 OCF（原始复合格式，Original Composite
// Format）字体的字体开发者必须将这些字体转换为 sfnt 格式，以供将来的 Macintosh 系统使用。OCF
// 字体可以转换为 CID 键控字体文件格式，CID 键控字体可以通过将 CIDFont 嵌入 sfnt 资源的子表
// 中来转换为 sfnt 格式。
//
// 展望未来，为尽可能广泛的市场开发功能齐全的字体的最佳方式将是使用新的 OpenType 字体格式，
// 该格式由 Adobe Systems 和 Microsoft Corporation 联合开发。在不久的将来，希望为即将推出
// 的 Macintosh 操作系统制作多字节字体的开发者应使用 CID sfnt 格式。
//
// sfnt 和 OpenType 格式本质上都是字体打包格式，允许将 CID 键控字体作为单个子表嵌入。这意
// 味着将来字体开发者应该可以相对容易地将 CID sfnt 字体转换为 OpenType 格式。
//
// 本文档使用的约定。在本文档中，所有 sfnt 表名和表元素以无衬线字体表示，PostScript 语言和
// Type 1 字体程序操作符以粗体无衬线样式表示。
//
// 对文档的引用在首次引用时包含完整信息，后续引用可使用缩写形式。完整的参考书目信息请参阅附
// 录 B。
//
// CID sfnt 格式（The CID sfnt Format）
//
// CID sfnt 字体由包含完整 CIDFont 作为子表的 sfnt"包装器"组成。sfnt 包装器中的表由唯一的四
// 字符标签标识，例如 'cmap'。Apple 定义的所有数据表名使用全部小写字符。Adobe 开发的所有非
// Apple 表使用全部大写字母，例如 'CID'。
//
// CID sfnt 字体与 TrueType 字体的区别在于 sfnt 头部的 4 字节 Fixed 版本字段中存储的标签为
// 'typ1'，而 TrueType 字体则使用标签 'true' 或值 1.0。CID 键控 Type 1 sfnt 字体与 Type 1
// 罗马 sfnt 字体的区别在于存在 CID 表且不存在 TYP1 表。
//
// 从概念上讲，sfnt 表分为三类。系统特定表（如 name 和 cmap）是注册字体并使操作系统能够使用
// 它所需的每个字体必备表。可选的布局特定表是用于格式化文本的表（本文档中未描述）。最后，栅
// 格化器特定表是 Type 1 栅格化器或 TrueType 栅格化器用于栅格化字形、生成字形轮廓、准备字体
// 下载或返回字距信息的表。本文档中描述的表属于第一类和第三类。
//
// 注意：CID sfnt 字体不使用 Adobe 技术说明 5014《Adobe CMap 和 CIDFont 文件规范》中描述的
// CMap 文件形式。相反，使用 Apple 定义的 cmap 表进行字符编码。
//
// 兼容性。本规范定义的 CID sfnt 字体可与 QuickDraw 和 QuickDraw GX 一起使用。该字体通过具
// 有 FNAM 表与 QuickDraw 兼容，该表还允许 Adobe Type Manager 软件（ATM）使用 QuickDraw GX
// 的 QuickDraw NFNT 资源来显示位图。FNAM 表还用于选择 cmap 子表进行编码。有关兼容模式下如
// 何选择编码的更多信息，请参见第 12.1 节。
//
// 要在 PostScript 语言打印机上打印 CID 键控 sfnt 字体，必须从 sfnt 包装器中提取 CID 键控
// 字体。这可以通过获取 sfnt 资源表目录中 CIDFont 的偏移量和长度来完成。
//
// CID sfnt 格式还允许使用重新排列的 CID 键控字体，有关更多信息请参见第 12.2 节。
//
// 字形变换（Glyph Transformations）。CID sfnt 格式允许指定可应用于字体中字形的一组变换。对
// 于 CJK 字体，这可能包括连字替换（ligature substitution）和为比例间距字形（proportionally
// spaced glyphs）指定替代度量等功能。
//
// 这些字形变换通过使用字形变形表（glyph metamorphosis table，标签名 mort）来实现，该表由
// Apple 定义和记录。在为比例间距字形提供替代度量的情况下，必须使用 Adobe 定义并在本文档第
// 4 节中描述的 CID 特定表 ALMX（替代度量）表。

// https://learn.microsoft.com/en-us/typography/opentype/spec/
// https://gpuopen.com/learn/mesh_shaders/mesh_shaders-font_and_vector_art_rendering_with_mesh_shaders/
// https://github.com/nothings/stb/blob/master/stb_truetype.h
// https://freetype.org
//
// OpenType 概述（OpenType Overview）
//
// OpenType 字体格式是一种广泛支持的字体数据格式，具有丰富的数字排版功能。它作为原始
// TrueType 格式的扩展而开发，使用相同的 'sfnt' 容器结构，并保持与遵循原始 TrueType
// 规范创建的字体兼容。但它还支持多种附加功能，包括：
//  1.  字形轮廓数据可以使用 CFF 或 CFF 版本 2（"CFF2"）格式，以及 TrueType 字形格式。
//  2.  多色字形呈现支持使用嵌入的彩色位图或 SVG 文档，或使用在字体中以二进制格式定义
//      的二维图形合成，该格式将轮廓格式字形（outline-format glyphs）与各种图形操作相
//      结合。
//  3.  支持所有 Unicode 字符，包括辅助平面字符（supplementary-plane characters）以及
//      Unicode 变体序列（unicode variation sequences）。
//  4.  OpenType 布局表提供了高质量排版所需的高级排版功能，以及使用 Unicode 标准支持
//      的广泛脚本进行国际化文本处理。
//  5.  数学排版表（mathematical typesetting table）允许字体包含复杂数学公式布局所需
//      的数据。
//  6.  OpenType 集合文件使共享公共数据的多个字体能够存放在单个文件中，从而实现数据
//      去重。这对于例如共享大部分字形但某些字符具有特定区域字形变体的 CJK（中文、日
//      文、韩文）字体集特别有用。
//  7.  字体变体（"可变字体"）使字形轮廓或其他字体数据能够基于一个或多个设计轴参数而
//      变化。集合文件可以包含多个离散的静态字体资源，而可变字体可以在每个轴上提供连
//      续的设计变化。这可以为内容作者和设计师提供极大的灵活性，同时也允许以高效格式
//      表示整个字体家族的数据。
//
// 本规范旨在与其他规范配合使用。
//  1.  虽然支持各种传统字符编码标准，但它主要设计用于与 Unicode 标准配合使用，Unicode
//      标准为书面字符和符号提供通用编码，以及不同脚本文本表示方式的规范。
//  2.  本规范定义了 OpenType 布局表以及高质量排版和在各种脚本中正确显示 Unicode 文本
//      所需的低级字形替换和定位操作。请参阅 OpenType 布局概述。OpenType 布局功能注册
//      （feature registry）定义了各种功能，这些功能代表字体中可能支持的特定排版功能，
//      并用于在给定字体中激活这些功能。许多功能公开了作者和排版师可以酌情选择使用的
//      可选功能；例如，小型大写字母形式或字距调整。但许多其他功能用于激活正确显示文本
//      所必需的功能；例如，阿拉伯文所需的连字或标记字形的定位。Unicode 支持的许多脚本
//      具有复杂的结构行为，需要非平凡的操作，在应用程序或文本布局和"塑形"库中实现，以
//      导出用于呈现底层 Unicode 字符串的正确定位字形序列。功能注册表将包括可用于这些
//      操作的功能。然而，不同脚本的塑形算法（shaping algorithms）的完整规范超出了本规
//      范的范围。这些算法可以是特定应用程序中的专有"闭源"实现；或者可以在供应商特定的
//      规范或其他行业规范中定义。
//      https://learn.microsoft.com/en-us/typography/opentype/spec/featuretags
//  3.  涉及复杂公式的数学文本支持需要内容格式来描述文本语义，并结合布局和呈现功能。本
//      规范定义了呈现所需的字体特定数据。这样，OpenType 可以用于实现其他文档格式规范
//      （如 TEX 或 MathML）的呈现功能。某些功能可以在 OpenType 布局功能标签注册表中定
//      义，以支持数学布局操作。
//  4.  文本布局涉及单行文本内的操作，但也涉及将行排列到页面或类似上下文中的更大块中的
//      控制。本规范定义了用于块级布局的某些数据，例如默认行度量（升部、降部、行距）。
//      此外，某些功能可以在 OpenType 布局功能标签注册表中定义，以支持水平或垂直布局方
//      向的文本块布局。然而，块级布局的完整规范超出了本规范的范围。它可以与其他规范
//      （如 Unicode 标准附件 #50：Unicode 垂直文本布局）配合使用。
//  5.  某些字体功能可能受应用程序或其他更高级别协议的定制影响。例如，本规范中定义的
//      TrueType 指令定义了用于字形轮廓栅格化的操作，但应用程序可以补充自己的最终栅格
//      化算法以优化字形的可读性。例如，使用超采样（over-sampling）或子像素显示元素控
//      制（control of subpixel display elements）。此外，虽然 OpenType 布局功能标签
//      注册表中的功能描述可以指定预期用法，但应用程序可以根据其他规范或根据自己的需求
//      定制其功能使用。
//
// OpenType 字体文件（OpenType Font File）
// https://learn.microsoft.com/en-us/typography/opentype/spec/otff
//
// OpenType 字体文件包含以表格式排列的数据，用于文本渲染。部分数据用于应用程序计算使用
// 字体的文本布局（the layout of text），即字形（glyph）的选择及其在行中的放置。其他数
// 据提供字形描述，作为 TrueType 或紧凑字体格式（CFF，Compact Font Format）轮廓（outlines）。
// 还有其他数据可以提供单色或彩色位图（monochromatic or color bitmaps）、SVG 文档或二
// 进制、二维矢量图形合成作为替代字形描述。字体数据还包括元信息，例如可用于在字体选择器
// 用户界面中将字体作为可用选项呈现的名称字符串。这些类型的数据每种都存储在一个或多个表
// 中，每个表都针对特定目的设计。
//
// 文件名。OpenType 字体文件可以具有扩展名 .OTF、.TTF、.OTC 或 .TTC。扩展名可以是大写或
// 小写。扩展名 .OTC 和 .TTC 应仅用于字体集合文件（font collection file）。有关文件名扩
// 展名约定的其他信息，请参阅"OpenType 字体建议"中的"文件名"。
// https://learn.microsoft.com/en-us/typography/opentype/spec/recom#filenames
//
// 数据类型。以下数据类型在 OpenType 字体文件中使用。所有 OpenType 字体都使用大端（网络）
// 字节顺序：
//      数据类型        描述
//      uint8           8 位无符号整数。
//      int8            8 位有符号整数。
//      uint16          16 位无符号整数。
//      int16           16 位有符号整数。
//      uint24          24 位无符号整数。
//      uint32          32 位无符号整数。
//      int32           32 位有符号整数。
//      Fixed           32 位有符号定点数（16.16）
//      FWORD           描述字体设计单位中数量（describe a quantity in font design units）的 int16。
//      UFWORD          描述字体设计单位中数量的 uint16。
//      F2DOT14         低 14 位为分数的 16 位有符号定点数（2.14）。
//      LONGDATETIME    自 1904 年 1 月 1 日午夜 12:00 UTC 以来的秒数表示的日期和时间。该值表示为有符号 64 位整数。
//      Tag             四个 uint8 的数组（长度 = 32 位），用于标识表、设计变化轴（design-variation axis）、脚本、语言系统、功能或基线（baseline）。
//      Offset8         表的 8 位偏移，与 uint8 相同，NULL 偏移 = 0x00
//      Offset16        表的短偏移，与 uint16 相同，NULL 偏移 = 0x0000
//      Offset24        表的 24 位偏移，与 uint24 相同，NULL 偏移 = 0x000000
//      Offset32        表的长偏移，与 uint32 相同，NULL 偏移 = 0x00000000
//      Version16Dot16  包含主要和次要版本号的打包 32 位值。请参阅表版本号。
//
// F2DOT14 格式由带符号的二进制补码整数和无符号分数组成。要计算实际值，取整数并加上分数。
// 2.14 值的示例如下：
//      十进制值    十六进制值  整数    分数
//      1.999939    0x7fff      1       16383/16384
//      1.75        0x7000      1       12288/16384
//      0.000061    0x0001      0       1/16384
//      0.0         0x0000      0       0/16384
//      -0.000061   0xffff      -1      16383/16384
//      -2.0        0x8000      -2      0/16384
//
// Tag 值是 uint8 数组。数组中的每个字节必须具有 0x20 到 0x7E 范围内的值。这对应于 UTF-8
// 编码中 Unicode 基本拉丁文字符的值范围，与可打印 ASCII 字符相同。因此，Tag 值可以重新
// 解释为四个字符序列，这是通常引用它们的方式。然而，正式地说，该值是字节数组。重新解释
// 为字符时，Tag 值区分大小写。它必须有一到四个非空格字符，用尾部空格（字节值 0x20）填充。
// 空格字符后面不能跟非空格字符。
//
// 在本规范中，许多结构根据上述数据类型定义。结构被表征为记录或表。记录和表之间的区别基于
// 以下一般标准：
//  * 表通过偏移引用。如果表包含对子结构的偏移，偏移通常从该表的开头开始。
//  * 记录按顺序出现在父结构中，无论是在表字段序列内还是在给定类型记录数组内。如果记录包
//    含对子结构的偏移，则该结构逻辑上是记录父表的子表，偏移通常从父表的开头开始。
//
// 在某些情况下，子表偏移的字段允许为 NULL 值，当给定子表是可选时。例如，在 BASE 表头中，
// horizAxisOffset 和 vertAxisOffset 字段可能为 NULL，意味着任一子表（或两者）是可选的。
// NULL 子表偏移始终表示给定子表不存在。应用程序绝不能将 NULL 偏移值解释为子表数据的偏移。
// 对于未记录为允许 NULL 值的子表偏移字段，字体编译器必须包含指定格式的子表，即使它是没
// 有进一步数据的头存根（例如，没有字形 ID 的覆盖表）。然而，解析字体数据的应用程序应预
// 期非符合性字体数据，其中在只预期非 NULL 值的地方出现 NULL 子表偏移。
// https://learn.microsoft.com/en-us/typography/opentype/spec/base#base-header
// https://learn.microsoft.com/en-us/typography/opentype/spec/chapter2#coverage-table
//
// 表版本号。大多数表都有版本号，整个字体的版本号包含在表目录中。请注意，有五种不同的表版
// 本号类型，每种都有自己的编号方案。
//  1.  单个 uint16 字段。这在许多表中使用，通常版本从零（0）开始。
//  2.  单独的 uint16 主要和次要版本字段。这在许多表中使用，通常版本从 1.0 开始。
//  3.  具有枚举值的 uint32 字段。
//  4.  具有数值的 uint32 字段。这仅在 DSIG 和 'meta' 表中使用。
//  5.  用于主要/次要版本号的 Version16Dot16 字段。这仅在 'maxp'、'post' 和 'vhea' 表中使用。
//
// Version16Dot16 类型用于某些表的版本字段，仅出于向后兼容的原因。在早期版本中，这些字段
// 被记录为使用 Fixed 值，但次要版本号不符合 Fixed 类型的定义。Version16Dot16 是一个打包值：
// 高 16 位包含主要版本号，低 16 位包含次要版本号。非零次要版本号使用低 16 位的最高阶半字
// 节中的数字 0 到 9 表示。例如，'maxp' 表版本 0.5 的版本字段为 0x00005000，'vhea' 表版本
// 1.1 的版本字段为 0x00011000。此类型仅在 'maxp'、'post' 和 'vhea' 表中使用，将来不会用于
// 任何其他表。
//
// 表目录使用 uint32 字段 sfntVersion，具有定义值的枚举，其中一些表示四字符标签；有关详细
// 信息，请参阅下面的"OpenType 字体组织"部分。
//
// 读取表的实现必须包含检查版本号的代码，以便在格式和版本号更改时，旧实现能够优雅地处理新
// 版本。次要版本号更改始终意味着格式更改是兼容扩展。如果实现理解主要版本号，则可以安全地
// 继续读取表。如果次要版本大于实现识别的最新版本，则扩展字段对实现不可检测。为了兼容性，
// 使用单个 uint16 或 uint32 值表示的版本号被视为次要版本号，任何格式更改都是兼容扩展。
//
// 请注意，早期修订版中未定义或保留的某些字段值可能在次要版本更改中被赋予含义。实现不应对
// 保留或未分配的字段值或位字段中的位进行假设，如果遇到可以忽略它们。编写字体数据时，工具
// 应始终为保留字段或位写入零。验证器应对针对验证数据的给定版本未定义的字段或位的任何非零
// 值发出警告。
//
// 如果主要版本未被识别，实现不得读取表，因为它无法对二进制数据的解释做出任何假设。实现应
// 将表视为缺失。
//
// OpenType 字体组织（Organization of an OpenType Font）
//
// 表目录（Table Directory）。OpenType 格式的一个关键特征是 TrueType sfnt "包装器"，它以通
// 用和可扩展的方式为字体中的表集合提供组织。OpenType 字体以表目录开头，这是字体中顶级表的
// 目录。如果字体文件只包含一个字体，表目录将从文件的字节 0 开始。如果字体文件是 OpenType
// 字体集合文件（见下文），每个字体的表目录起始点由 TTCHeader 指示。表目录如下表，注意符号
// "**" 是幂运算符，如几种编程语言中使用。
//
//      类型        名称                        描述
//      uint32      sfntVersion                 0x00010000 或 0x4F54544F ('OTTO') — 见下文
//      uint16      numTables                   表的数量
//      uint16      searchRange                 小于或等于 numTables 的 2 的最大幂乘以 16，(2**floor(log2(numTables))) * 16
//      uint16      entrySelector               小于或等于 numTables 的 2 的最大幂的 Log2，log2(searchRange/16)，等于 floor(log2(numTables))）
//      uint16      rangeShift                  numTables 乘以 16 减去 searchRange，(numTables * 16) - searchRange
//      TableRecord tableRecords[numTables]     表记录数组，字体中每个顶级表都有一个表记录数组
//
// 包含 TrueType 轮廓（outlines）的 OpenType 字体应使用 0x00010000 作为 sfntVersion。包含
// 紧凑字体格式（CFF，Compact Font Format）轮廓数据（版本 1 或 2）的 OpenType 字体应使用
// 0x4F54544F ('OTTO'，重新解释为 Tag 时) 作为 sfntVersion。注意：Apple 的 TrueType 参考
// 手册允许 'true' 和 'typ1' 作为 sfntVersion。这些版本标签不应在 OpenType 字体中使用。
// https://developer.apple.com/fonts/TrueType-Reference-Manual/
//
// 表目录格式允许大量表。为了协助快速二分搜索，searchRange、entrySelector 和 rangeShift
// 字段作为参数包含在内，可用于配置搜索算法。当条目数为 2 的幂时，二分搜索是最优的。searchRange
// 字段提供可以用该约束搜索的最大项目数（2 的最大幂）。rangeShift 字段提供还需要搜索的剩余
// 项目数。entrySelector 字段指示需要进入二叉树的最大层数。值乘以 16，即每个 TableRecord
// 的大小。
//
// 在具有有限硬件功能的设备上的早期实现中，searchRange、entrySelector 和 rangeShift 字段
// 提供的优化非常重要。它们在现代设备上的重要性较低，但仍可能在某些实现中使用。然而，不正
// 确的值可能被用作对某些实现的攻击向量。由于这些值可以在解析文件时从 numTables 字段派生，
// 强烈建议解析实现不依赖字体中的 searchRange、entrySelector 和 rangeShift 字段，而是从
// numTables 独立派生它们。然而，字体文件应继续为这些字段提供有效值，以维持与所有现有实现
// 的兼容性。
//
// 表目录以 tableRecords 数组结束。TableRecord 格式如下。
//      类型        名称        描述
//      Tag         tableTag    表标识符
//      uint32      checksum    此表的校验和
//      Offset32    offset      从字体文件开头开始的偏移
//      uint32      length      此表的长度
//
// 表标签是 OpenType 字体文件中表的名称。tableRecords 数组使给定字体能够只包含它实际需要的
// 表。因此，numTables 没有标准值。数组中的记录必须按标签（tag）升序排序。有关 Tag 值的要求，
// 请参见上面的数据类型。本规范中定义了几个标签及其关联的表格式。对于本规范中定义的表标签，
// 字体资源最多应有一个使用给定标签的表记录。如果字体资源确实包含多个给定类型的表，则行为不
// 可预测：应用程序或平台可能任意选择其中一个表，或可能拒绝字体为无效。
//
// 平台供应商可以定义附加表和关联标签以提供平台特定功能。例如，请参阅 Apple 的 TrueType 参考
// 手册，其中定义了 OpenType 中未定义的各种表及其关联标签。某些字体开发工具还可以定义特殊表。
// 包含此类附加表的字体如果满足本规范的要求，仍可以作为 OpenType 字体。对于本规范外部定义的
// 自定义表，此类表的外部规范可能允许单个字体资源中存在多个该类型的表。
//
// 注意：供应商在定义自定义标签时应通知 Microsoft，以确保将来 OpenType 扩展时的前向兼容性。注
// 意：Apple 的规范规定，由全部小写字母组成的标签名称保留给 Apple 使用。
//
// 所有表必须在四字节边界上开始，表之间的任何剩余空间必须用零填充。每个表的长度应记录在表记
// 录中，使用数据的实际长度，而不是填充长度。注意：四字节对齐要求仅适用于顶级表，不延伸到表
// 或记录内的子表偏移、记录或字段。
//
// 某些表具有内部结构，子表位于指定偏移处，因此可以构造具有不同表数据交错排列的字体。通常，
// 表应连续排列，不重叠不同表的范围。然而，无论如何，表长度测量包含表的所有数据的连续字节
// 范围。这适用于任何子表以及顶级表。
//
// 计算校验和。表校验和是给定表的 uint32 单元的无符号和。在 C 中，以下函数可用于确定校验和。
// 注意：此函数假设任何表的长度是四字节倍数，或者表填充零到四字节对齐偏移。然而，表目录中记
// 录的实际表长度不应包括填充。为了适应长度不是四倍数的数据，必须修改上述算法，将数据视为包
// 含零填充到四倍数长度。
//
// 'head' 表是校验和计算中的特殊情况，因为它包含一个 checksumAdjustment 字段，该字段在表的
// 校验和计算并写入表目录条目后计算和写入，必然使该校验和值无效。生成字体数据时，要计算和写
// 入 'head' 表校验和和 checksumAdjustment 字段，请执行以下操作：
//  1.  将 checksumAdjustment 字段设置为 0
//  2.  计算所有表（包括 'head' 表）的校验和，并将每个表的值输入表目录中的相应记录
//  3.  计算整个字体的校验和
//  4.  从 0xB1B0AFBA 中减去该值
//  5.  将结果存储在 'head' 表 checksumAdjustment 字段中
//
// 尝试验证 'head' 表未更改的应用程序应假设 checksumAdjustment 值为零来计算该表的校验和，而
// 不是字体中的实际值，然后再将结果与表目录中的 'head' 表记录进行比较。在字体集合文件（见下文）
// 中，表校验和必须反映集合文件中的表。'head' 表中的 checksumAdjustment 字段不用于集合文件，
// 可以设置为零。

#define PRH_TTF_OUTLINE 0x00010000 // TrueType 1.0
#define PRH_TTF_APLTRUE 0x74727565 // 'true'
#define PRH_CFF_APLTYP1 0x74797031 // 'typ1'
#define PRH_CFF_OUTLINE 0x4F54544F // 'OTTO'
#define PRH_TTC_HEADER  0x74746366 // 'ttcf'
#define PRH_TTC_DSIG    0x44534947 // 'DSIG'

typedef struct {
    prh_r32 sfntversion;    // 0x00010000 或 0x4F54544F ('OTTO')
    prh_r16 numtables;      // 表的数量
    prh_r16 searchrange;
    prh_r16 entryselector;
    prh_r16 rangeshift;
} prh_font_header;

// 字体集合（Font Collections）
//
// OpenType 字体集合（TTC 或 OTC，以前称为 TrueType Collection）是在单个文件结构中传递多个
// OpenType 字体资源的手段。字体集合格式允许两个或多个字体之间相同的字体表共享。包含轮廓字
// 形数据（TrueType、CFF、CFF2 或 SVG）的字体集合在要一起传递的字体共享许多共同字形时最有用。
// 通过允许多个字体共享字形集和其他公共字体表，字体集合可以显著节省文件空间。
//
// 例如，一组日文字体可能每个都有自己的假名设计，但共享相同的汉字设计。使用普通 OpenType
// 字体文件，包含公共汉字字形的唯一方法是将它们的字形数据复制到每个字体中。由于汉字代表比
// 假名多得多的数据，这导致大量浪费的字形数据重复。字体集合被定义为解决这个问题。
//
// 注意：即使字体集合的原始定义（作为 TrueType 规范的一部分）旨在用于包含 TrueType 轮廓的
// 字体，并且在早期 OpenType 版本中保持了此约束，但这不再是 OpenType 中的约束。字体集合文
// 件可以包含各种类型的轮廓（或它们的混合），无论字体是否具有布局表（layout table）。为了
// 向后兼容和简单性，字体集合文件结构的描述使用术语"TrueType 集合"或"TTC"，但理解为它用于
// 标识包含任何类型轮廓表的通用字体集合结构。
//
// 注意：OpenType 可变字体（variable font）在功能上等价于多个非可变字体（non-variable font）。
// 可变字体不需要包含在集合文件中。集合文件可以包含一个甚至多个可变字体，甚至可以组合可变和
// 非可变字体。
//
// 字体集合文件结构。字体集合文件由单个 TTC 头表、一个或多个表目录（每个对应不同的字体资源）
// 以及多个 OpenType 表组成。TTC 头必须位于 TTC 文件的开头。
//
// TTC 文件必须为每个字体资源包含完整的表目录。每个字体资源在集合文件中使用与非集合文件相同
// 的 TableDirectory 格式。TTC 文件中所有表目录中的表偏移都是从 TTC 文件的开头开始测量的。
//
// TTC 文件中的每个 OpenType 表通过使用它的每个字体的表目录来引用。某些 OpenType 表必须出现
// 多次，对于 TTC 中包含的每个字体一次；而其他表可以由 TTC 中的多个字体共享。例如，考虑一个
// 组合两个日文字体（Font1 和 Font2）的 TTC 文件。字体具有不同的假名设计（Kana1 和 Kana2），
// 但使用相同的汉字设计。TTC 文件包含单个 'glyf' 表，其中包括两种假名设计以及汉字；两个字体
// 的表目录都指向此 'glyf' 表。但每个字体的表目录指向不同的 'cmap' 表，该表标识要使用的字形
// 集。Font1 的 'cmap' 表指向 'loca' 和 'glyf' 表的 Kana1 区域以获取假名字形，以及汉字区域以
// 获取汉字。Font2 的 'cmap' 表指向 'loca' 和 'glyf' 表的 Kana2 区域以获取假名字形，以及相同
// 的汉字区域以获取汉字。
//
// 每个字体应具有唯一副本的表是系统用于标识字体及其字符映射的表，包括 'cmap'、'name' 和 OS/2。
// 应由 TTC 中的字体共享的表是定义字形和指令数据或使用字形索引访问数据的表：'glyf'、'loca'、
// 'hmtx'、'hdmx'、LTSH、'cvt '、'fpgm'、'prep'、EBLC、EBDT、EBSC、'maxp' 等。实际上，两个或
// 多个字体具有相同数据的任何表都可以共享。
//
// 注意：从单独的字体文件构建集合文件时，需要密切关注字体中字形重新编号的问题以及可能在 'cmap'
// 表和其他地方导致的副作用。要合并的字体还需要具有兼容的 TrueType 指令；即，它们的预程序
// （preprograms）、函数定义和控制值不能冲突。
//
// 包含 TrueType 字形轮廓的集合文件应使用文件扩展名 .TTC。包含 CFF 或 CFF2 轮廓的集合文件
// 应使用文件扩展名 .OTC。
//
// TTC 头（TTC header）。TTC 头有两个版本：版本 1.0 已用于没有数字签名的 TTC 文件。版本 2.0
// 可用于有或没有数字签名的 TTC 文件，如果没有签名，则版本 2.0 头的最后三个字段留空。如果使
// 用数字签名，文件的 DSIG 表必须位于 TTC 文件的末尾，在任何其他字体表之后。TTC 文件中的签
// 名预计为格式 1 签名。
//
// TTC 头表的目的是定位 TTC 文件中不同的表目录。TTC 头位于 TTC 文件的开头（偏移 = 0）。它由
// 标识标签、版本号、文件中 OpenType 字体数量的计数以及指向每个表目录的偏移数组组成。
//
// TTCHeader 版本 1.0：
//      类型        名称                            描述
//      Tag         ttcTag                          字体集合 ID 字符串：'ttcf'（用于具有 CFF 或 CFF2 轮廓的字体以及 TrueType 轮廓）
//      uint16      majorVersion                    TTCHeader 的主要版本，= 1
//      uint16      minorVersion                    TTCHeader 的次要版本，= 0
//      uint32      numFonts                        TTC 中的字体数量
//      Offset32    tableDirectoryOffsets[numFonts] 每个字体的 TableDirectory 偏移数组，从文件开头开始
//
// TTCHeader 版本 2.0：
//      类型        名称                            描述
//      Tag         ttcTag                          字体集合 ID 字符串：'ttcf'
//      uint16      majorVersion                    TTCHeader 的主要版本，= 2
//      uint16      minorVersion                    TTCHeader 的次要版本，= 0
//      uint32      numFonts                        TTC 中的字体数量
//      Offset32    tableDirectoryOffsets[numFonts] 每个字体的 TableDirectory 偏移数组，从文件开头开始
//      uint32      dsigTag                         指示 DSIG 表存在的标签，0x44534947 ('DSIG')（如果没有签名为 null）
//      uint32      dsigLength                      DSIG 表的长度（以字节为单位）（如果没有签名为 null）
//      uint32      dsigOffset                      从 TTC 文件开头到 DSIG 表的偏移（以字节为单位）（如果没有签名为 null）

typedef struct {
    prh_r32 ttctag;
    prh_r16 majorversion;
    prh_r16 minorversion;
    prh_r32 numfonts;
} prh_font_ttc_header;

typedef struct {
    prh_r32 dsigtag;    // 0x44534947 ('DSIG')，没有签名时为 NULL
    prh_r32 dsiglength; // DSIG 表的长度，没有签名时为 NULL
    prh_r32 dsigoffset; // DSIG 表所在的文件偏移，没有签名时为 NULL
} prh_font_ttc_signature;

typedef struct {
    prh_reader reader;
    prh_font_header font_header;
    prh_font_ttc_header ttc_header;
    prh_font_ttc_signature ttc_signature;
    prh_r32 *ttc_font_header_offset_big;
    prh_r32 file_size;
} prh_font_file;

prh_static_assert(sizeof(prh_font_header) == sizeof(prh_font_ttc_header));

void prh_font_file_free(prh_font_file *f) {
    prh_da_free(f->ttc_font_header_offset_big);
    prh_read_free(&f->reader);
}

void prh_load_font_file(prh_font_file *f, const prh_byte *name) {
    f->reader = prh_read_from_file(name, prh_vmem_unit_size, prh_local_alloc());
    prh_read_exact_bytes(&f->reader, (prh_byte *)&f->font_header, sizeof(prh_font_header));
    prh_set_r32_be_to_host(f->font_header.sfntversion);
    if (f->font_header.sfntversion == PRH_TTF_OUTLINE || f->font_header.sfntversion == PRH_TTF_APLTRUE ||
        f->font_header.sfntversion == PRH_CFF_OUTLINE || f->font_header.sfntversion == PRH_CFF_APLTYP1) {
        prh_set_r16_be_to_host(f->font_header.numtables);
        memset(&f->ttc_header, 0, sizeof(prh_font_ttc_header));
        memset(&f->ttc_signature, 0, sizeof(prh_font_ttc_signature));
        f->ttc_font_header_offset_big = prh_null;
        f->ttc_header.numfonts = 1;
    } else if (f->font_header.sfntversion == PRH_TTC_HEADER) {
        f->ttc_header = *(prh_font_ttc_header *)&f->font_header;
        prh_set_r16_be_to_host(f->ttc_header.majorversion);
        prh_set_r16_be_to_host(f->ttc_header.minorversion);
        prh_set_r32_be_to_host(f->ttc_header.numfonts);
        if (f->ttc_header.majorversion != 1 && f->ttc_header.majorversion != 2) {
            prh_abort_error(f->ttc_header.majorversion);
        }
        prh_da_init(f->ttc_font_header_offset_big, f->ttc_header.numfonts);
        prh_read_exact_bytes(&f->reader, (prh_byte *)f->ttc_font_header_offset_big, f->ttc_header.numfonts * sizeof(prh_r32));
        if (f->ttc_header.majorversion == 2) {
            prh_read_exact_bytes(&f->reader, (prh_byte *)&f->ttc_signature, sizeof(prh_font_ttc_signature));
            prh_set_r32_be_to_host(f->ttc_signature.dsigtag);
            prh_set_r32_be_to_host(f->ttc_signature.dsiglength);
            prh_set_r32_be_to_host(f->ttc_signature.dsigoffset);
            if (f->ttc_signature.dsigtag != 0 && f->ttc_signature.dsigtag != PRH_TTC_DSIG) {
                prh_abort_error(f->ttc_signature.dsigtag);
            }
        }
        f->font_header.numtables = 0;
    } else {
        prh_eprinf_r32(f->font_header.sfntversion, prh_pf_print_base | prh_pf_hex | 8);
        prh_abort_error(__LINE__);
    }
    f->file_size = prh_file_size_32(f->reader.handle);
}

prh_r32 prh_font_count(const prh_font_file *f) {
    return f->ttc_header.numfonts;
}

typedef struct {
    prh_r32 tabletag;   // 表标识符，必须按 tag 值升序排列
    prh_r32 checksum;   // 此表校验和
    prh_r32 offset;     // 此表所在的文件偏移
    prh_r32 length;     // 此表的长度
} prh_font_thead;

typedef struct {
    prh_r32 tabletag;
    prh_r32 checksum;
    prh_r32 offset;
    prh_r32 length;
    prh_r32 table_index;
} prh_font_table;

typedef struct prh_open_font {
    prh_font_file *font_file;
    prh_font_header font_header;
    prh_font_thead *table_header;
    prh_r32 font_header_offset;
    prh_r32 font_index;
    bool is_cff_outline;
    prh_r32 maxp_version;
    prh_r16 num_glyphs;
    prh_r16 max_points;
    prh_r16 max_contours;
    prh_r16 max_composite_points;
    prh_r16 max_composite_contours;
    prh_r16 max_zones;
    prh_r16 max_twilinght_points;
    prh_r16 max_storage;
    prh_r16 max_function_defs;
    prh_r16 max_instruction_defs;
    prh_r16 max_stack_elements;
    prh_r16 max_size_of_instructions;
    prh_r16 max_component_elements;
    prh_r16 max_component_depth;
    prh_font_table base; // 'BASE'
    prh_font_table cff1; // 'CFF '
    prh_font_table cff2; // 'CFF2'
    prh_font_table dsig; // 'DSIG'
    prh_font_table gdef; // 'GDEF'
    prh_font_table gpos; // 'GPOS'
    prh_font_table gsub; // 'GSUB'
    prh_font_table os_2; // 'OS/2'
    prh_font_table stat; // 'STAT'
    prh_font_table vdmx; // 'VDMX'
    prh_font_table vorg; // 'VORG'
    prh_font_table cmap; // 'cmap'
    prh_font_table hdmx; // 'hdmx'
    prh_font_table head; // 'head'
    prh_font_table hhea; // 'hhea'
    prh_font_table hmtx; // 'hmtx'
    prh_font_table maxp; // 'maxp'
    prh_font_table name; // 'name'
    prh_font_table post; // 'post'
    prh_font_table vhea; // 'vhea'
    prh_font_table vmtx; // 'vmtx'
} prh_open_font;

prh_r16 prh_font_table_count(const prh_open_font *f) {
    return f->font_header.numtables;
}

void prh_open_font_free(prh_open_font *p) {
    prh_da_free(p->table_header);
}

void prh_impl_font_header_read(prh_open_font *p);

void prh_load_open_font(prh_open_font *p, prh_font_file *f, prh_r32 font_index) {
    memset(p, 0, sizeof(prh_open_font));
    prh_assert(font_index < prh_font_count(f));
    if (f->font_header.sfntversion == PRH_TTC_HEADER) {
        p->font_header_offset = f->ttc_font_header_offset_big[font_index];
        prh_set_r32_be_to_host(p->font_header_offset);
    } else {
        p->font_header_offset = 0;
    }
    p->font_file = f;
    p->font_index = font_index;
    prh_impl_font_header_read(p);
}

void prh_print_ttff_header(prh_font_file *f) {
    prh_print(
        "file tag 0x%08x (%c%c%c%c)\n"
        "file size %d-byte %d-KB %d-MB\n"
        "font count %d\n"
        "font ttc version %d.%d\n"
        "font ttc data signature 0x%08x (%c%c%c%c) offset %d length %d\n\n",
        (prh_reg)f->font_header.sfntversion,
        (prh_reg)prh_byte_4(f->font_header.sfntversion),
        (prh_reg)prh_byte_3(f->font_header.sfntversion),
        (prh_reg)prh_byte_2(f->font_header.sfntversion),
        (prh_reg)prh_byte_1(f->font_header.sfntversion),
        (prh_reg)f->file_size,
        (prh_reg)f->file_size / 1024,
        (prh_reg)f->file_size / 1024 / 1024,
        (prh_reg)prh_font_count(f),
        (prh_reg)f->ttc_header.majorversion,
        (prh_reg)f->ttc_header.minorversion,
        (prh_reg)f->ttc_signature.dsigtag,
        (prh_reg)prh_byte_4(f->ttc_signature.dsigtag),
        (prh_reg)prh_byte_3(f->ttc_signature.dsigtag),
        (prh_reg)prh_byte_2(f->ttc_signature.dsigtag),
        (prh_reg)prh_byte_1(f->ttc_signature.dsigtag),
        (prh_reg)f->ttc_signature.dsigoffset,
        (prh_reg)f->ttc_signature.dsiglength);
}

void prh_print_font_header(prh_open_font *f) {
    prh_print(
        "font index %d / %d\n"
        "font tag 0x%08x (%c%c%c%c)\n"
        "font offset %d\n"
        "font tables %d\n\n",
        (prh_reg)f->font_index + 1,
        (prh_reg)prh_font_count(f->font_file),
        (prh_reg)f->font_header.sfntversion,
        (prh_reg)prh_byte_4(f->font_header.sfntversion),
        (prh_reg)prh_byte_3(f->font_header.sfntversion),
        (prh_reg)prh_byte_2(f->font_header.sfntversion),
        (prh_reg)prh_byte_1(f->font_header.sfntversion),
        (prh_reg)f->font_header_offset,
        (prh_reg)prh_font_table_count(f));
}

void prh_print_font_table(prh_open_font *f, prh_r16 table_index) {
    prh_font_thead *t = f->table_header + table_index;
    prh_r32 tabletag = prh_r32_be_to_host(t->tabletag);
    prh_r32 offset = prh_r32_be_to_host(t->offset);
    prh_r32 length = prh_r32_be_to_host(t->length);
    prh_r32 checksum = prh_r32_be_to_host(t->checksum);
    prh_print(
        "table index %d / %d\n"
        "table tag 0x%08x (%c%c%c%c)\n"
        "table offset %.10d (%d/4)\n"
        "table length %.10d (%d/4)\n"
        "table checksum 0x%08x\n\n",
        (prh_reg)table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)tabletag,
        (prh_reg)prh_byte_4(tabletag),
        (prh_reg)prh_byte_3(tabletag),
        (prh_reg)prh_byte_2(tabletag),
        (prh_reg)prh_byte_1(tabletag),
        (prh_reg)offset, (prh_reg)offset % 4,
        (prh_reg)length, (prh_reg)length % 4,
        (prh_reg)checksum);
}

// 字体表（Font Tables）
//
// 必需表（Required Tables）。无论 TrueType 还是 CFF 轮廓用于 OpenType 字体，以下表对于
// 字体正确运行是必需的：
//      标签    名称
//      'cmap'  字符到字形映射
//      'head'  字体头
//      'hhea'  水平头
//      'hmtx'  水平度量
//      'maxp'  最大轮廓
//      'name'  命名表
//      OS/2    OS/2 和 Windows 特定度量
//      'post'  PostScript 信息
//
// 与 TrueType 轮廓相关的表，对于基于 TrueType 轮廓的 OpenType 字体，使用以下表：
//      标签    名称
//      'cvt '  控制值表（可选表）
//      'fpgm'  字体程序（可选表）
//      'glyf'  字形数据
//      'loca'  位置索引
//      'prep'  控制值程序（可选表）
//      'gasp'  网格拟合/扫描转换（可选表）
//
// 与 CFF 轮廓相关的表，对于基于 CFF 轮廓的 OpenType 字体，使用以下表。强烈建议用于垂直
// 书写的 CFF OpenType 字体包含垂直原点（VORG）表。
//      标签    名称
//      'CFF '  紧凑字体格式 1.0
//      CFF2    紧凑字体格式 2.0
//      VORG    垂直原点（可选表）
//
// 与 SVG 轮廓相关的表：
//      标签    名称
//      'SVG '  SVG（可缩放矢量图形）表
//
// 与位图字形相关的表。OpenType 字体还可以包含字形位图，以及轮廓。手工调整的位图在 OpenType
// 字体中特别有用，用于在非常小的尺寸表示复杂字形。如果字体中提供了特定尺寸的位图，则在渲染
// 字形时系统将使用它而不是轮廓。
//      标签    名称
//      EBDT    嵌入式位图数据
//      EBLC    嵌入式位图位置数据
//      EBSC    嵌入式位图缩放数据
//      CBDT    彩色位图数据
//      CBLC    彩色位图位置数据
//      'sbix'  标准位图图形
//
// 高级排版表（Advanced Typographic Tables），几个可选表支持高级排版功能。有关通用表格式
// 的信息，请参阅 OpenType 布局通用表格式。https://learn.microsoft.com/en-us/typography/opentype/spec/chapter2
//      标签    名称
//      BASE    基线数据
//      GDEF    字形定义数据
//      GPOS    字形定位数据
//      GSUB    字形替换数据
//      JSTF    两端对齐数据
//      MATH    数学布局数据
//
// 用于 OpenType 字体变体（Font Variations）的表：
//      标签    名称
//      'avar'  轴变体
//      'cvar'  CVT 变体（仅限 TrueType 轮廓）
//      'fvar'  字体变体
//      'gvar'  字形变体（仅限 TrueType 轮廓）
//      HVAR    水平度量变体
//      MVAR    度量变体
//      STAT    样式属性（可变字体必需，非可变字体可选）
//      VVAR    垂直度量变体
//
// 有关 OpenType 字体变体的概述和插值算法的规范，请参阅 OpenType 字体变体概述。有关可变字
// 体中哪些表是必需或可选的详细信息，请参阅概述章节中的变化数据表和杂项要求。有关用于变体
// 的通用表格式，请参阅 OpenType 字体变体通用表格式。
// https://learn.microsoft.com/en-us/typography/opentype/spec/otvaroverview
// https://learn.microsoft.com/en-us/typography/opentype/spec/otvaroverview#vartables
// https://learn.microsoft.com/en-us/typography/opentype/spec/otvarcommonformats
//
// 请注意，某些变化相关（variation-related）格式可能用于上述变化特定表之外的其他表中。特别
// 是，可变字体中的 GDEF、BASE 或 COLR 表可以使用通用表格式包含变化数据。可变字体中的 CFF2
// 表也可以包含变化数据，但使用特定于 CFF2 表的格式。
//
// 与彩色字体相关的表，请注意，这些表中的几个也在其他部分列出，用于与 SVG 轮廓相关的表，
// 以及与位图字形相关的表。
//      标签    名称
//      COLR    颜色表
//      CPAL    调色板表
//      CBDT    彩色位图数据
//      CBLC    彩色位图位置数据
//      'sbix'  标准位图图形
//      'SVG '  SVG（可缩放矢量图形）表
//
// 其他 OpenType 表。请注意，STAT 表在可变字体中是必需的。此外，'hdmx' 和 VDMX 表在可变
// 字体中不使用。
//      标签    名称
//      DSIG    数字签名
//      'hdmx'  水平设备度量
//      'kern'  字距调整
//      LTSH    线性阈值数据
//      MERG    合并
//      'meta'  元数据
//      STAT    样式属性
//      PCLT    PCL 5 数据
//      VDMX    垂直设备度量
//      'vhea'  垂直度量头
//      'vmtx'  垂直度量
//
// EM 是字体排版中的基本度量单位，来源于金属活字时代，最初指一个"M"字形的宽度（这也是其
// 名称的来源）。在数字字体（如 OpenType）中，em 的含义如下：
//  1.  与字号的关系。em 的高度等于字体的当前字号大小。例如，在 12pt 的字体中，1 em =
//      12pt；在 16px 的字体中，1 em = 16px。
//  2.  设计坐标系中的基准单位。字体文件内部使用一个设计单位坐标系，由 unitsPerEm（每 em
//      的设计单位数）定义。所有字形度量值（如字宽、升部、降部、下标/上标大小等）都以这些
//      设计单位表示，最终换算为 em 的分数来确定实际显示尺寸。
//      * 如果 unitsPerEm = 1000，则 1 em = 1000 个设计单位
//      * 如果 unitsPerEm = 2048（如 Times New Roman），则 1 em = 2048 个设计单位
//  3.  典型应用。字宽，字符的宽度通常表示为 em 的分数（如 0.5 em）。行距，由升部（ascender）
//      和降部（descender）之和决定，通常接近 1 em。CSS 中的 em 单位：直接继承自字体排版，
//      1 em 等于当前元素的字体大小。
//
// 简单来说，em 是字体度量系统的"基准方块"，一个虚拟的正方形，其边长等于字体字号，所有其他
// 度量都围绕它展开。
//
// 常用的字体度量单位可分为绝对物理单位、字体相对单位、数字字体设计单位和屏幕/Web 单位四大类：
//
// 一、绝对物理单位（印刷排版）
//      单位    符号    说明
//      点      pt      最常用的字号单位。桌面出版（PostScript）中 1 pt = 1/72 英寸；传统
//                      活字印刷中 1 pt ≈ 0.3528 mm
//      派卡    pc      1 pc = 12 pt，常用于排版行宽、栏宽
//      毫米    mm      国际标准单位，1 mm ≈ 2.835 pt
//      厘米    cm      1 cm = 10 mm
//      英寸    in      1 in = 72 pt（PostScript 标准）
//
// 二、字体相对单位（排版核心）
//      单位    说明
//      em      等于当前字体大小。在金属活字时代指"M"字形的宽度；在数字字体中，em 是设计
//              坐标系的基准方块，由 unitsPerEm 定义。所有字形度量（字宽、升部、降部等）
//              都以 em 的分数表示
//      en      通常等于 0.5 em，即 em 的一半宽度
//      ex      约等于当前字体中小写字母 "x" 的高度，用于垂直对齐
//      ch      等于当前字体中数字 "0" 的宽度（CSS 特有）
//
// 三、数字字体设计单位（OpenType/TrueType 内部）
//      单位                        说明
//      设计单位 (design units)     字体文件内部坐标系中的抽象单位。例如 unitsPerEm = 1000
//                                  表示 1 em 被划分为 1000 个设计单位。所有字形轮廓坐标、字
//                                  宽、升部/降部值都以设计单位存储
//      FWORD                       OpenType 表中的有符号 16 位整数，以设计单位表示（如 sTypoAscender）
//      UFWORD                      无符号 16 位整数，以设计单位表示（如 usWinAscent）
//      TWIP                        二十分之一磅（1/20 pt），OpenType OS/2 v5 表中 usLowerOpticalPointSize
//                                  等字段使用
//      Fixed                       16.16 定点数格式，用于 italicAngle 等需要小数的字段
//
// 四、屏幕/Web 单位
//      单位                        说明
//      px (像素)                   屏幕显示的基本单位，与设备分辨率相关
//      rem                         相对于根元素（html）字体大小的 em
//      vw/vh                       视口宽/高的百分比
//
// 五、其他专业单位
//      单位                        说明
//      Q (Quart)                   日本排版常用单位，1 Q = 0.25 mm
//      Didot 点                    欧洲大陆传统活字单位，1 Didot pt ≈ 0.376 mm，略大于 PostScript pt
//      Cicero                      欧洲大陆对应派卡的单位，1 Cicero = 12 Didot pt
//
// 日常排版：最常用的是 pt（点） 和 em
// 字体开发：核心单位是设计单位（相对于 unitsPerEm 的分数）
// Web 开发：最常用 px、em、rem
// OpenType 表内部：大量使用设计单位 和 TWIP

#define PRH_OTF_BASE_TABLE 0x42415345
#define PRH_OTF_CFF1_TABLE 0x43464620 // 紧凑字体格式 1.0
#define PRH_OTF_CFF2_TABLE 0x43464632 // 紧凑字体格式 2.0
#define PRH_OTF_DSIG_TABLE 0x44534947 // 数组签名
#define PRH_OTF_GDEF_TABLE 0x47444546
#define PRH_OTF_GPOS_TABLE 0x47504F53
#define PRH_OTF_GSUB_TABLE 0x47535542
#define PRH_OTF_OS_2_TABLE 0x4F532F32 // OS/2 和 Windows 特定度量信息
#define PRH_OTF_STAT_TABLE 0x53544154 // 样式属性，可变字体必需，非可变字体可选
#define PRH_OTF_VDMX_TABLE 0x56444D58 // 垂直设备度量，可变字体不使用
#define PRH_OTF_VORG_TABLE 0x564F5247 // 垂直原点，可选表

#define PRH_OTF_CMAP_TABLE 0x636D6170 // 字符 char 到字形 glyph 的映射
#define PRH_OTF_HDMX_TABLE 0x68646D78 // 水平设备度量，可变字体不使用
#define PRH_OTF_HEAD_TABLE 0x68656164 // 字体头
#define PRH_OTF_HHEA_TABLE 0x68686561 // 水平度量头
#define PRH_OTF_HMTX_TABLE 0x686D7478 // 水平度量
#define PRH_OTF_MAXP_TABLE 0x6D617870 // 最大配置
#define PRH_OTF_NAME_TABLE 0x6E616D65 // 命名表
#define PRH_OTF_POST_TABLE 0x706F7374 // PostScript 信息
#define PRH_OTF_VHEA_TABLE 0x76686561 // 垂直度量头
#define PRH_OTF_VMTX_TABLE 0x766D7478 // 垂直度量

prh_byte *prh_load_font_table(prh_open_font *f, prh_font_table *t) {
    prh_r32 round_length = prh_round_r32_04_byte(t->length);
    prh_byte *table_data; prh_da_init(table_data, round_length);
    prh_pread_exact_bytes(&f->font_file->reader, table_data, round_length, t->offset);
    return table_data;
}

prh_r32 prh_font_table_checksum(const prh_r32 *table_data, prh_r32 table_length) {
    prh_r32 sum = 0;
    const prh_r32 *end = table_data + prh_round_r32_04_byte(table_length) / 4;
    while (table_data < end) sum += *table_data++;
    return sum;
}

void prh_impl_font_header_read(prh_open_font *p) {
    struct { prh_r32 tag; prh_font_table *table; } tables[] = {
        {PRH_OTF_BASE_TABLE, &p->base},
        {PRH_OTF_CFF1_TABLE, &p->cff1},
        {PRH_OTF_CFF2_TABLE, &p->cff2},
        {PRH_OTF_DSIG_TABLE, &p->dsig},
        {PRH_OTF_GDEF_TABLE, &p->gdef},
        {PRH_OTF_GPOS_TABLE, &p->gpos},
        {PRH_OTF_GSUB_TABLE, &p->gsub},
        {PRH_OTF_OS_2_TABLE, &p->os_2},
        {PRH_OTF_STAT_TABLE, &p->stat},
        {PRH_OTF_VDMX_TABLE, &p->vdmx},
        {PRH_OTF_VORG_TABLE, &p->vorg},
        {PRH_OTF_CMAP_TABLE, &p->cmap},
        {PRH_OTF_HDMX_TABLE, &p->hdmx},
        {PRH_OTF_HEAD_TABLE, &p->head},
        {PRH_OTF_HHEA_TABLE, &p->hhea},
        {PRH_OTF_HMTX_TABLE, &p->hmtx},
        {PRH_OTF_MAXP_TABLE, &p->maxp},
        {PRH_OTF_NAME_TABLE, &p->name},
        {PRH_OTF_POST_TABLE, &p->post},
        {PRH_OTF_VHEA_TABLE, &p->vhea},
        {PRH_OTF_VMTX_TABLE, &p->vmtx},
        {0xFFFFFFFF, prh_null}};
    prh_r32 table_i = 0, tabletag;
    prh_font_file *f = p->font_file;
    prh_pread_exact_bytes(&f->reader, (prh_byte *)&p->font_header, sizeof(prh_font_header), p->font_header_offset);
    prh_set_r32_be_to_host(f->font_header.sfntversion);
    if (f->font_header.sfntversion == PRH_TTF_OUTLINE || f->font_header.sfntversion == PRH_TTF_APLTRUE ||
        f->font_header.sfntversion == PRH_CFF_OUTLINE || f->font_header.sfntversion == PRH_CFF_APLTYP1) {
        prh_set_r16_be_to_host(f->font_header.numtables);
        p->is_cff_outline = (f->font_header.sfntversion == PRH_CFF_OUTLINE || f->font_header.sfntversion == PRH_CFF_APLTYP1);
    } else {
        prh_eprinf_r32(f->font_header.sfntversion, prh_pf_print_base | prh_pf_hex | 8);
        prh_abort_error(__LINE__);
    }
    prh_font_thead *thead;
    prh_da_init(p->table_header, p->font_header.numtables + 1);
    prh_pread_exact_bytes(&f->reader, (prh_byte *)(p->table_header + 1), p->font_header.numtables * sizeof(prh_font_thead), p->font_header_offset + sizeof(prh_font_header));
    for (prh_r32 i = 1; i <= p->font_header.numtables; i += 1) {
        thead = p->table_header + i;
        tabletag = prh_r32_be_to_host(thead->tabletag);
        while (tables[table_i].tag < tabletag) table_i += 1;
        if (tabletag == tables[table_i].tag) {
            tables[table_i].table->tabletag = tabletag;
            tables[table_i].table->checksum = prh_r32_be_to_host(thead->checksum);
            tables[table_i].table->offset = prh_r32_be_to_host(thead->offset);
            tables[table_i].table->length = prh_r32_be_to_host(thead->length);
            tables[table_i].table->table_index = i;
            table_i += 1;
        }
        if (tables[table_i].tag == 0xFFFFFFFF) break;
    }
}

// 字体头表（head）
//
// 本表提供有关字体的全局信息。边界框值应仅使用具有轮廓的字形计算。无轮廓的字形应在此计
// 算中忽略。
//
//      类型    名称                描述
//      uint16  majorVersion        字体头表的主要版本号，设置为 1
//      uint16  minorVersion        字体头表的次要版本号，设置为 0
//      Fixed   fontRevision        由字体制造商设置（32-bit signed fixed-point number，16.16）
//      uint32  checksumAdjustment  计算方法：将其设置为 0，将整个字体作为 uint32 求和，然后存储
//                                  0xB1B0AFBA - 总和。如果字体用作字体集合文件中的组件，则此字段
//                                  的值将因文件结构和字体表目录的更改而失效，必须忽略。
//      uint32  magicNumber         设置为 0x5F0F3CF5
//      uint16  flags
//          位 0：字体的基线在 y=0。
//          位 1：左侧承点位于 x=0（仅与 TrueType 栅格化器相关），请参阅下面关于可变字体
//                的附加信息。
//          位 2：指令可能取决于点大小。
//          位 3：强制 ppem 为整数值用于所有内部缩放器数学；如果此位清除，可以使用分数 ppem
//                大小。强烈建议在提示字体中设置此位。
//          位 4：指令可能改变前进宽度（前进宽度可能不按比例缩放）。
//          位 5：此位在 OpenType 中不使用，不应设置以确保所有平台上的兼容行为。如果设置，
//                在某些平台上可能导致垂直布局的不同行为。有关 Apple 平台上的行为详细信息，
//                请参阅 Apple 的规范。http://developer.apple.com/fonts//TrueType-Reference-Manual/RM06/Chap6head.html
//          位 6~10：这些位在 OpenType 中不使用，应始终清除。有关 Apple 平台上遗留使用的
//                 详细信息，请参阅 Apple 的规范。
//          位 11：字体数据是"无损的"，因为经过了优化转换和/或压缩（如 ISO/IEC 14496-18、
//                 MicroType Express、WOFF 2.0 或类似机制定义的压缩机制），其中保留了原始
//                 字体功能和特性，但输入和输出字体文件之间的二进制兼容性不保证。由于应用
//                 的转换，DSIG 表也可能失效。
//          位 12：字体已转换（产生兼容度量）。
//          位 13：字体针对 ClearType 优化。注意，依赖嵌入式位图（EBDT）进行渲染的字体不应
//                 被视为针对 ClearType 优化，因此应保持此位清除。
//          位 14：最后手段（Last Resort）字体。如果设置，表示 'cmap' 子表中编码的字形只是
//                 代码点范围的通用符号表示，并不真正代表对这些代码点的支持。如果未设置，
//                 表示 'cmap' 子表中编码的字形代表对这些代码点的正确支持。
//          位 15：保留，设置为 0。
//      uint16 unitsPerEm           设置为 16 到 16384 之间的值。此范围内的任何值都有效。在具有
//                                  TrueType 轮廓的字体中，建议使用 2 的幂，因为这允许某些栅格化器
//                                  中的性能优化。
//      LONGDATETIME created        自 1904 年 1 月 1 日午夜 12:00 以来的秒数，以 GMT/UTC 时区计算。
//      LONGDATETIME modified       自 1904 年 1 月 1 日午夜 12:00 以来的秒数，以 GMT/UTC 时区计算。
//      int16 xMin                  所有字形边界框的最小 x 坐标
//      int16 yMin                  所有字形边界框的最小 y 坐标
//      int16 xMax                  所有字形边界框的最大 x 坐标
//      int16 yMax                  所有字形边界框的最大 y 坐标
//      uint16 macStyle
//          位 0：粗体 Bold（如果设置为 1）
//          位 1：斜体 Italic（如果设置为 1）
//          位 2：下划线 Underline（如果设置为 1）
//          位 3：轮廓 Outline（如果设置为 1）
//          位 4：阴影 Shadow（如果设置为 1）
//          位 5：压缩 Condensed（如果设置为 1）
//          位 6：扩展 Extended（如果设置为 1）
//          位 7 – 15：保留（设置为 0）
//      uint16 lowestRecPPEM        最小可读大小（像素）
//      int16 fontDirectionHint     已弃用（设置为 2）
//          0：完全混合方向字形
//          1：仅强（strongly）从左到右
//          2：类似 1 但也包含中性（neutrals）
//          -1：仅强从右到左
//          -2：类似 -1 但也包含中性
//      int16 indexToLocFormat      短偏移（Offset16）为 0，长偏移（Offset32）为 1
//      int16 glyphDataFormat       当前格式为 0
//
// fontDirectionHint 字段旨在支持从右到左的脚本。强从左到右、强从右到左和中性是字符的属
// 性。中性字符没有固有的方向性；它不是零（0）宽度的字符。空格和标点符号是中性字符的示例。
// 非中性字符是具有固有方向性的字符。例如，罗马字母（从左到右）和阿拉伯字母（从右到左）
// 具有强方向性。在"正常"的罗马字体中，如果存在空格和标点符号，字体方向提示应设置为 2。
//
// unitsPerEm 值确定字体坐标网格的粒度，在该网格中可以指定视觉元素（如轮廓控制点或标记附
// 着锚点位置）的坐标。有关更多信息，请参阅 TrueType 基础章节。
// https://learn.microsoft.com/en-us/typography/opentype/spec/ttch01#funits-and-the-grid
//
// 字形的边界框（bounding box）是包含字形所有控制点的最小矩形。有关更多信息，请参阅 'glyf'
// 表章节。xMin、yMin、xMax 和 yMax 值提供一个将包含字体所有字形的边界框。无轮廓的字形被
// 忽略。
//
// macStyle 位必须与 OS/2 表中的 fsSelection 位一致。在 Windows 中，fsSelection 位优先于
// macStyle 位。PANOSE 值和 'post' 表值在确定粗体或斜体字体时被忽略。
//
// 由于历史原因，Windows 不使用此表中包含的 fontRevision 值来确定字体版本。相反，Windows
// 评估 'name' 表中的版本字符串（ID 5）。
//
// 每个字形的左侧承点对应于第一个虚点（下面的"pp1"，请参阅虚点 Phantom Points），并与字形
// 左侧承（lsb，glyph left sidebearing）和 xMin 相关，如下。如果 flags 字段的位 1 被设置，
// 则所有字形的 pp1 = 0，且每个字形的 xMin 和左侧承必须相等。
//      pp1 = xMin - lsb
// 
// 在具有 TrueType 轮廓的可变字体中，每个字形的左侧承必须等于 xMin，且 flags 字段中的位 1
// 必须设置。此外，所有可变字体中必须清除位 5。有关 OpenType 字体变体的一般信息，请参阅
// OpenType 字体变体概述章节。
//
// 此外，在可变字体中，控制点的最小或最大 x 或 y 值可能变化，包含任何给定字形实例轮廓或所
// 有点的紧密边界矩形可能比该字形的默认实例更小或更大。此表中的 xMin、yMin、xMax 和 yMax
// 值可能包含也可能不包含字体非默认实例的派生字形轮廓。此外，不为这些值提供变化增量。如果
// 应用程序需要一个包含字体非默认实例字形的边界矩形，应处理该实例的派生字形轮廓以确定边界
// 矩形。

#define prh_font_flag_baseline_y_0 0x0001

typedef struct {
    prh_r32 head_version;
    prh_r32 font_revision;
    prh_r32 checksum_adjustment;
    prh_r32 magic_number;
    prh_r16 flags;
    prh_r16 units_per_em; // upem
    prh_r08 create_time[8];
    prh_r08 modify_time[8];
    prh_i16 xmin;
    prh_i16 ymin;
    prh_i16 xmax;
    prh_i16 ymax;
    prh_r16 mac_style;
    prh_r16 lowest_rec_ppem; // pixels per em
    prh_r16 font_direction_hint;
    prh_i16 index_to_loc_format;
    prh_i16 glyph_data_format;
    prh_r16 aligned;
} prh_font_head_table;

void prh_print_font_head_table(prh_open_font *f) {
    if (f->head.length == 0) prh_abort_line();
    prh_font_head_table t = {0};
    prh_r32 head_length = (prh_r32)sizeof(prh_font_head_table) - 2;
    if (head_length != f->head.length) {
        prh_print("table 'head' invalid length %d %d\n", (prh_reg)head_length, (prh_reg)f->head.length);
        return;
    }
    prh_pread_exact_bytes(&f->font_file->reader, (prh_byte *)&t, head_length, f->head.offset);
    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)&t, head_length);
    prh_print(
        "head table index %d / %d\n"
        "head table tag 0x%08x (%c%c%c%c)\n"
        "head table offset %.10d (%d/4)\n"
        "head table length %.10d (%d/4)\n"
        "head table checksum 0x%08x 0x%08x (valid %d)\n"
        "head table version %08x\n"
        "----------------------------\n"
        "head font reversion %08x\n"
        "head font checksum adjustment 0x%08x\n"
        "head font magic number 0x%08x\n"
        "head font flags %04x\n"
        "head font units per em %d\n"
        "head font create time %ld\n"
        "head font modify time %ld\n"
        "head font x min %d max %d\n"
        "head font y min %d max %d\n"
        "head font mac style %d\n"
        "head font lowest rec ppem %d\n"
        "head font direction hint %d\n"
        "head font index to loc format %d\n"
        "head font glyph data format %d\n\n",
        (prh_reg)f->head.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->head.tabletag,
        (prh_reg)prh_byte_4(f->head.tabletag),
        (prh_reg)prh_byte_3(f->head.tabletag),
        (prh_reg)prh_byte_2(f->head.tabletag),
        (prh_reg)prh_byte_1(f->head.tabletag),
        (prh_reg)f->head.offset, (prh_reg)f->head.offset % 4,
        (prh_reg)f->head.length, (prh_reg)f->head.length % 4,
        (prh_reg)f->head.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->head.checksum),
        (prh_reg)prh_r32_be_to_host(t.head_version),
        (prh_reg)prh_r32_be_to_host(t.font_revision),
        (prh_reg)prh_r32_be_to_host(t.checksum_adjustment),
        (prh_reg)prh_r32_be_to_host(t.magic_number),
        (prh_reg)prh_r16_be_to_host(t.flags),
        (prh_reg)prh_r16_be_to_host(t.units_per_em),
        (prh_r64)prh_bp_8b_to_host(t.create_time),
        (prh_r64)prh_bp_8b_to_host(t.modify_time),
        (prh_int)(prh_i16)prh_r16_be_to_host(t.xmin), (prh_int)(prh_i16)prh_r16_be_to_host(t.xmax),
        (prh_int)(prh_i16)prh_r16_be_to_host(t.ymin), (prh_int)(prh_i16)prh_r16_be_to_host(t.ymax),
        (prh_reg)prh_r16_be_to_host(t.mac_style),
        (prh_reg)prh_r16_be_to_host(t.lowest_rec_ppem),
        (prh_reg)prh_r16_be_to_host(t.font_direction_hint),
        (prh_reg)prh_r16_be_to_host(t.index_to_loc_format),
        (prh_reg)prh_r16_be_to_host(t.glyph_data_format));
}

// 水平头表（hhea - Horizontal Header Table）
//
// 本表包含用于水平布局的信息。minRightSidebearing、minLeftSideBearing 和 xMaxExtent 的值
// 应仅使用具有轮廓的字形来计算。对于无轮廓的字形，在计算时应忽略。所有保留区域必须设为 0。
//      类型    名称                    说明
//      uint16  majorVersion            水平表头表的主版本号 — 设为 1
//      uint16  minorVersion            水平表头表的次版本号 — 设为 0
//      FWORD   ascender                字体排印升部 — 参见下文备注
//      FWORD   descender               字体排印降部 — 参见下文备注
//      FWORD   lineGap                 字体排印行距。在某些旧平台实现中，负的 lineGap 值被视为零
//      UFWORD  advanceWidthMax         'hmtx' 表中的最大前进宽度值
//      FWORD   minLeftSideBearing      'hmtx' 表中具有轮廓的字形的最小左侧字距值（空字形应被忽略）
//      FWORD   minRightSideBearing     最小右侧字距值；计算方式为 min(aw - (lsb + xMax - xMin))，仅针对具有轮廓的字形（空字形应被忽略）
//      FWORD   xMaxExtent              Max(lsb + (xMax - xMin))
//      int16   caretSlopeRise          用于计算光标斜率（上升量/前进量），垂直时为 1
//      int16   caretSlopeRun           垂直时为 0
//      int16   caretOffset             倾斜高亮显示在字形上需要偏移的量，以获得最佳外观，对于非倾斜字体设为 0
//      int16   (保留)                  设为 0
//      int16   (保留)                  设为 0
//      int16   (保留)                  设为 0
//      int16   (保留)                  设为 0
//      int16   metricDataFormat        当前格式为 0
//      uint16  numberOfHMetrics        'hmtx' 表中 hMetric 条目的数量
//
// 本表中的 ascender、descender 和 linegap 值是 Apple 特有的；有关 Apple 平台的详细信息，请
// 参阅 Apple 的规范。OS/2 表中的 sTypoAscender、sTypoDescender 和 sTypoLineGap 字段在 Windows
// 平台上使用，并建议用于新的文本布局实现。字体开发人员应评估目标应用程序中使用本表或 OS/2
// 表字段的行为，以确保布局一致。有关更多详细信息，请参阅 OS/2 字段的说明。
// http://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6hhea.html
//
// 'hhea' 表与 OpenType 字体变体。在可变字体中，水平头表中的各种字体度量值可能需要针对不同的
// 变体实例进行调整。'hhea' 条目的变体数据可以在度量变体（MVAR）表中提供。不同的 'hhea' 条目
// 通过值标签与 MVAR 表中的特定变体数据相关联，如下所示。有关 OpenType 字体变体的一般信息，请
// 参阅"OpenType 字体变体概述"章节。
//      'hhea' 条目         标签
//      caretOffset         'hcof'
//      caretSlopeRise      'hcrs'
//      caretSlopeRun       'hcrn'

typedef struct {
    prh_r16 major_version;
    prh_r16 minor_version;
    prh_i16 ascender;
    prh_i16 descender;
    prh_i16 line_cap;
    prh_r16 advance_width_max;
    prh_i16 min_left_side_bearing;
    prh_i16 min_right_side_bearing;
    prh_i16 x_max_extent;
    prh_i16 caret_slope_rise;
    prh_i16 caret_slope_run;
    prh_i16 caret_offset;
    prh_i16 reserved_1;
    prh_i16 reserved_2;
    prh_i16 reserved_3;
    prh_i16 reserved_4;
    prh_i16 metric_data_format;
    prh_r16 number_of_hmetrics;
} prh_font_hhea_table;

void prh_print_font_hhea_table(prh_open_font *f) {
    if (f->hhea.length == 0) prh_abort_line();
    prh_font_hhea_table t = {0};
    prh_r32 head_length = (prh_r32)sizeof(prh_font_hhea_table);
    if (head_length != f->hhea.length) {
        prh_print("table 'hhea' invalid length %d %d\n", (prh_reg)head_length, (prh_reg)f->hhea.length);
        return;
    }
    prh_pread_exact_bytes(&f->font_file->reader, (prh_byte *)&t, head_length, f->hhea.offset);
    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)&t, head_length);
    prh_print(
        "hhea table index %d / %d\n"
        "hhea table tag 0x%08x (%c%c%c%c)\n"
        "hhea table offset %.10d (%d/4)\n"
        "hhea table length %.10d (%d/4)\n"
        "hhea table checksum 0x%08x 0x%08x (valid %d)\n"
        "hhea table version %08x\n"
        "----------------------------\n"
        "hhea font ascender %d\n"
        "hhea font descender %d\n"
        "hhea font line_cap %d\n"
        "hhea font advance_width_max %d\n"
        "hhea font min_left_side_bearing %d\n"
        "hhea font min_right_side_bearing %d\n"
        "hhea font x_max_extent %d\n"
        "hhea font caret_slope_rise %d\n"
        "hhea font caret_slope_run %d\n"
        "hhea font caret_offset %d\n"
        "hhea font metric_data_format %d\n"
        "hhea font number_of_hmetrics %d\n\n",
        (prh_reg)f->hhea.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->hhea.tabletag,
        (prh_reg)prh_byte_4(f->hhea.tabletag),
        (prh_reg)prh_byte_3(f->hhea.tabletag),
        (prh_reg)prh_byte_2(f->hhea.tabletag),
        (prh_reg)prh_byte_1(f->hhea.tabletag),
        (prh_reg)f->hhea.offset, (prh_reg)f->hhea.offset % 4,
        (prh_reg)f->hhea.length, (prh_reg)f->hhea.length % 4,
        (prh_reg)f->hhea.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->hhea.checksum),
        (prh_reg)prh_bp_4b_to_host((prh_byte *)&t),
        (prh_reg)prh_r16_be_to_host(t.ascender),
        (prh_reg)prh_r16_be_to_host(t.descender),
        (prh_reg)prh_r16_be_to_host(t.line_cap),
        (prh_reg)prh_r16_be_to_host(t.advance_width_max),
        (prh_reg)prh_r16_be_to_host(t.min_left_side_bearing),
        (prh_reg)prh_r16_be_to_host(t.min_right_side_bearing),
        (prh_reg)prh_r16_be_to_host(t.x_max_extent),
        (prh_reg)prh_r16_be_to_host(t.caret_slope_rise),
        (prh_reg)prh_r16_be_to_host(t.caret_slope_run),
        (prh_reg)prh_r16_be_to_host(t.caret_offset),
        (prh_reg)prh_r16_be_to_host(t.metric_data_format),
        (prh_reg)prh_r16_be_to_host(t.number_of_hmetrics));
}

// 水平度量表（hmtx - Horizontal Metrics Table）
//
// 用于水平文本布局的字形度量包括字形前进宽度、字距和 X 方向最小值与最大值（xMin、xMax）。
// 这些值通过字形轮廓数据（'glyf'、'CFF ' 或 CFF2）和水平度量表组合得出。水平度量（'hmtx'）
// 表提供字形前进宽度和左侧字距。
//
// 在包含 TrueType 轮廓数据的字体中，'glyf' 表提供 xMin 和 xMax 值，但不提供前进宽度或字距。
// 前进宽度始终从 'hmtx' 表获取。在某些字体中，取决于 'head' 表中标志的状态，左侧字距可能与
// 'glyf' 表中的 xMin 值相同，但这并非对所有字体都成立。请参阅 'head' 表 flags 字段位 1 的
// 说明。因此，'hmtx' 表中提供了左侧字距。右侧字距始终使用 'hmtx' 表中的前进宽度和左侧字距
// 值，加上字形描述中的边界框信息来推导，详见下文。
//
// 在包含 TrueType 轮廓数据的可变字体中，'hmtx' 表中的左侧字距值必须始终等于 xMin（'head'
// flags 字段的位 1 必须设置）。因此，这些值也可以直接从 'glyf' 表推导得出。注意，这些值仅
// 适用于可变字体的默认实例：非默认实例可能具有不同的字距值。这些值可以从插值的"虚点"（phantom
// point）坐标使用 'gvar' 表推导得出（详见下文），或者通过将 HVAR 表中的变体数据应用于 'glyf'
// 或 'hmtx' 表中的默认实例值来获得。
//
// 在包含 CFF 版本 1 轮廓数据的字体中，'CFF ' 表确实包含前进宽度。这些值由 PostScript 处理器
// 使用，但在 OpenType 布局中不使用。在 OpenType 上下文中，'hmtx' 表是必需的，必须用于前进宽
// 度。注意，在字体集合文件中共享 'CFF ' 表的字形，可以在特定字形的字体特定 'hmtx' 表中指定不
// 同的前进宽度。还要注意，CFF2 表不包含前进宽度。此外，对于 CFF 或 CFF2 数据，没有显式的 xMin
// 和 xMax 值；字距隐式包含在 CharString 数据中，可以从 CFF / CFF2 光栅化器获取。然而，某些布
// 局引擎可能使用 'hmtx' 表中的左侧字距值；因此，字体生产工具应确保 'hmtx' 表中的左侧字距值与
// CharString 数据中反映的隐式 xMin 值匹配。在包含 CFF2 轮廓数据的可变字体中，非默认实例的左
// 侧字距和前进宽度值应通过结合 'hmtx' 和 HVAR 表的信息来获取。
//
// 该表使用 LongHorMetric 记录来给出字形的前进宽度和左侧字距。记录按字形 ID 索引。作为优化，记
// 录数量可以少于字形数量，在这种情况下，最后一条记录的前进宽度值适用于所有剩余的字形 ID。这在
// 等宽字体中，或在具有大量相同前进宽度字形的字体中（前提是字形按适当顺序排列）非常有用。
// LongHorMetric 记录的数量由 'hhea' 表中的 numberOfHMetrics 字段确定。
//
// 如果 numberOfHMetrics 小于字形总数，则 hMetrics 数组后跟一个数组，包含剩余字形的左侧字距值。
// leftSideBearings 数组中的元素数量由 'maxp' 表中的 numGlyphs 字段减去 numberOfHMetrics 得出。
//
// 水平度量表：
//      类型            名称                                            说明
//      LongHorMetric   hMetrics[numberOfHMetrics]                      每个字形成对的前进宽度和左侧字距值，记录按字形 ID 索引
//      FWORD           leftSideBearings[numGlyphs - numberOfHMetrics]  字形 ID 大于或等于 numberOfHMetrics 的字形的左侧字距
//
// LongHorMetric 记录：
//      类型    名称            说明
//      UFWORD  advanceWidth    前进宽度，以字体设计单位表示
//      FWORD   lsb             字形左侧字距，以字体设计单位表示
//
// 在包含 TrueType 轮廓的字体中，每个字形的 xMin 和 xMax 值在 'glyf' 表中给出。前进宽度
// （"aw"）和左侧字距（"lsb"）可以从字形"虚点"（phantom points）推导得出，这些虚点由 TrueType
// 光栅化器计算；或者可以从 'hmtx' 表获取。在包含 CFF 或 CFF2 轮廓的字体中，xMin（= 左侧字距）
// 和 xMax 值可以从 CFF / CFF2 光栅化器获取。从这些值中，右侧字距（"rsb"）计算如下：
//      rsb = aw - (lsb + xMax - xMin)
//
// 如果 pp1 和 pp2 是用于控制 lsb 和 rsb 的 TrueType 虚点，则它们在 X 方向上的初始位置计
// 算如下。如果字形没有轮廓，则 xMax/xMin 未定义。'hmtx' 表中此类字形的左侧字距应指示为零。
//      pp1 = xMin - lsb
//      pp2 = pp1 + aw

// 水平设备度量（hdmx - Horizontal Device Metrics）
//
// 'hdmx' 表可用于包含 TrueType 轮廓的字体中，以存储缩放到特定像素尺寸的整数前进宽度。这允
// 许文本布局引擎在不调用缩放器的情况下为每个字形构建整数宽度表。通常，此表仅包含某些像素尺
// 寸的宽度。
//
// 对于非方形像素网格，使用 em 方块宽度（以像素为单位）来确定使用哪个设备记录。例如，在分辨
// 率为 72×96 的设备上，12 点的 em 方块高 12 像素、宽 16 像素。将使用每 em 16 像素的 'hdmx'
// 设备记录。
//
// 如果 'head' 表 flags 字段的位 4 未设置，则假定字体线性缩放；在这种情况下，不需要 'hdmx'
// 表，也不应构建。如果 flags 字段的位 4 已设置，则假定字体中有一个或多个字形非线性缩放。在
// 这种情况下，通过包含重要尺寸的宽度数据的 'hdmx' 表可以提高性能。有关更多详细信息，请参阅
// "OpenType 字体建议"章节。https://learn.microsoft.com/en-us/typography/opentype/spec/recom#hdmx
//
// 表的开头如下（HdmxHeader）：
//      类型            名称                说明
//      uint16          version             表版本号 — 设为 0
//      uint16          numRecords          设备记录的数量
//      uint32          sizeDeviceRecord    设备记录的大小，32 位对齐
//      DeviceRecord    records[numRecords] 设备记录数组
//
// DeviceRecord 数组必须按像素尺寸递增顺序排序。DeviceRecord 格式如下：
//      类型    名称                说明
//      uint8   pixelSize           以下宽度的像素尺寸（以 ppem 计）
//      uint8   maxWidth            最大宽度
//      uint8   widths[numGlyphs]   宽度数组（numGlyphs 来自 'maxp' 表）
//
// 每个 DeviceRecord 用 0 填充，使其 32 位对齐。widths 数组中的每个值是特定字形在 pixelSize
// 字段给出的每 em 像素（ppem）尺寸下的宽度，以像素为单位。ppem 尺寸沿 y 轴测量。

// 垂直表头表（vhea - Vertical Header Table）
//
// 垂直头表包含中文、日文、韩文（CJK）及其他表意文字进行垂直布局所需的信息。在垂直布局中，
// 这些文字从上到下或从下到上书写。本表包含适用于整个字体的通用信息。与特定字形相关的信息
// 在垂直度量（'vmtx'）表中给出。这些表的格式与水平度量的 'hhea' 和 'hmtx' 表类似。
//
// 垂直头表中的数据必须与垂直度量表中的数据保持一致。垂直度量表中的前进高度和顶部字距值
// 必须与垂直头表中的最大前进高度和最小底部字距值相对应。有关构建 CJK（中文、日文和韩文）
// 字体的更多信息，请参阅"OpenType CJK 字体指南"章节。
// https://learn.microsoft.com/en-us/typography/opentype/spec/recom#cjk
//
// 'vhea' 表有两个版本：1.0 和 1.1。版本 1.0 和版本 1.1 之间的差异在于以下字段的名称和定义：
//  1.  ascender 变为 vertTypoAscender
//  2.  descender 变为 vertTypoDescender
//  3.  lineGap 变为 vertTypoLineGap
//
// 版本 1.0 的垂直头表格式（VheaHeader）
//      类型    名称                    说明
//      Version16Dot16  version         垂直表头表的版本号；版本 1.0 为 0x00010000
//      FWORD   ascent                  从中心线到上一行降部的距离，以字体设计单位表示
//      FWORD   descent                 从中心线到下一行升部的距离，以字体设计单位表示
//      FWORD   lineGap                 保留，设为 0
//      UFWORD  advanceHeightMax        字体中找到的最大前进高度度量值，以字体设计单位表示。此值必须与垂直度量表中的条目一致。
//      FWORD   minTopSideBearing       字体中找到的最小顶部字距度量值，以字体设计单位表示。此值必须与垂直度量表中的条目一致。
//      FWORD   minBottomSideBearing    字体中找到的最小底部字距度量值，以字体设计单位表示。此值必须与垂直度量表中的条目一致。
//      FWORD   yMaxExtent              定义为 yMaxExtent = max(tsb + (yMax - yMin))。
//      int16   caretSlopeRise          caretSlopeRise 字段的值除以 caretSlopeRun 字段的值决定插入符号的斜率。上升量为 0、
//                                      前进量为 1 表示水平插入符号；上升量为 1、前进量为 0 表示垂直插入符号。对于字形为
//                                      倾斜或斜体的字体，中间值是理想的。对于垂直字体，水平插入符号最佳。
//      int16   caretSlopeRun           参见 caretSlopeRise 字段。非倾斜垂直字体时值为 1。
//      int16   caretOffset             倾斜字形上的高亮显示需要远离字形偏移的量，以获得最佳外观。非倾斜字体时设为 0。
//      int16   reserved                设为 0
//      int16   reserved                设为 0
//      int16   reserved                设为 0
//      int16   reserved                设为 0
//      int16   metricDataFormat        设为 0
//      uint16  numOfLongVerMetrics     垂直度量表中的前进高度数量
//
// 版本 1.1 的垂直表头表格式（VheaHeader）
//      类型    名称                    说明
//      Version16Dot16  version         垂直表头表的版本号；版本 1.1 为 0x00011000
//      FWORD   vertTypoAscender        此字体的垂直字体排印升部。它是从垂直中心基线到 CJK/表意文字字形（或"表意文字 em
//                                      框"）设计空间右边缘的距离，以字体设计单位表示。通常设为 (head.unitsPerEm)/2。例
//                                      如，em 为 1000 设计单位的字体将此字段设为 500。有关表意文字 em 框的描述，请参阅
//                                      OpenType 布局标签注册表的基线标签章节。https://learn.microsoft.com/en-us/typography/opentype/spec/baselinetags#ideoembox
//      FWORD   vertTypoDescender       此字体的垂直字体排印降部。它是从垂直中心基线到 CJK/表意文字字形设计空间左边缘的
//                                      距离，以字体设计单位表示。通常设为 -(head.unitsPerEm/2)。例如，em 为 1000 设计单
//                                      位的字体将此字段设为 -500。
//      FWORD   vertTypoLineGap         此字体的垂直字体排印行距。应用程序可以通过以下表达式确定 OpenType
//                                      字体单倍行距垂直文本的推荐行距：表意文字 em 框宽度 + vhea.vertTypoLineGap。
//      UFWORD  advanceHeightMax        字体中找到的最大前进高度度量值，以字体设计单位表示。此值必须与垂直度量表中的条目一致。
//      FWORD   minTopSideBearing       字体中找到的最小顶部字距度量值，以字体设计单位表示。此值必须与垂直度量表中的条目一致。
//      FWORD   minBottomSideBearing    字体中找到的最小底部字距度量值，以字体设计单位表示。此值必须与垂直度量表中的条目一致。
//      FWORD   yMaxExtent              定义为 yMaxExtent = max(tsb + (yMax - yMin))。
//      int16   caretSlopeRise          caretSlopeRise 字段的值除以 caretSlopeRun 字段的值决定插入符号的斜率。上升量为 0、
//                                      前进量为 1 表示水平插入符号；上升量为 1、前进量为 0 表示垂直插入符号。对于字形为
//                                      倾斜或斜体的字体，中间值是理想的。对于垂直字体，水平插入符号最佳。
//      int16   caretSlopeRun           参见 caretSlopeRise 字段。非倾斜垂直字体时值为 1。
//      int16   caretOffset             倾斜字形上的高亮显示需要远离字形偏移的量，以获得最佳外观。非倾斜字体时设为 0。
//      int16   reserved                设为 0
//      int16   reserved                设为 0
//      int16   reserved                设为 0
//      int16   reserved                设为 0
//      int16   metricDataFormat        设为 0
//      uint16  numOfLongVerMetrics     垂直度量表中的前进高度数量
//
// 'vhea' 表与 OpenType 字体变体。在可变字体中，'vhea' 表中的各种字体度量值可能需要针对不同的
// 变体实例进行调整。'vhea' 条目的变体数据可以在度量变体（MVAR）表中提供。不同的 'vhea' 条目通
// 过值标签与 MVAR 表中的特定变体数据相关联，如下所示。有关 OpenType 字体变体的一般信息，请参
// 阅"OpenType 字体变体概述"章节。
//      'vhea' 条目                 标签
//      ascent, vertTypoAscender    'vasc'
//      caretOffset                 'vcof'
//      caretSlopeRun               'vcrn'
//      caretSlopeRise              'vcrs'
//      descent, vertTypoDescender  'vdsc'
//      lineGap, vertTypoLineGap    'vlgp'
//
// 垂直头表示例
//      偏移/长度   值          名称                    注释
//      0/4         0x00011000  version                 垂直表头表的版本号为 1.1
//      4/2         1024        vertTypoAscender        em 方块高度的一半
//      6/2         -1024       vertTypoDescender       em 方块高度一半的负值
//      8/2         0           vertTypoLineGap         字体排印行距为 0 个字体设计单位
//      10/2        2079        advanceHeightMax        字体中找到的最大前进高度度量值为 2079 个字体设计单位
//      12/2        -342        minTopSideBearing       字体中找到的最小顶部字距度量值为 -342 个字体设计单位
//      14/2        -333        minBottomSideBearing    字体中找到的最小底部字距度量值为 -333 个字体设计单位
//      16/2        2036        yMaxExtent              max(tsb + (yMax - yMin)) = 2036
//      18/2        0           caretSlopeRise          caret 斜率上升量为 0、caret 斜率前进量为 1 表示垂直字体的水平插入符号
//      20/2        1           caretSlopeRun           caret 斜率上升量为 0、caret 斜率前进量为 1 表示垂直字体的水平插入符号
//      22/2        0           caretOffset             非倾斜字体时设为 0
//      24/4        0           reserved                设为 0
//      26/2        0           reserved                设为 0
//      28/2        0           reserved                设为 0
//      30/2        0           reserved                设为 0
//      32/2        0           metricDataFormat        设为 0
//      34/2        258         numOfLongVerMetrics     垂直度量表中的前进高度数量为 258

// 垂直度量表（vmtx - Vertical Metrics Table）
//
// 垂直度量表允许您为垂直字体中的每个字形指定垂直间距。本表由一个或两个包含度量信息（每个
// 字形垂直布局的前进高度和顶部字距）的数组组成。垂直度量坐标系如下所示。小写 b 字形，带有
// 升部、降部、顶部字距和前进高度度量。
//
// OpenType 垂直字体需要同时包含垂直头表（'vhea'）和垂直度量表。垂直头表包含适用于整个字体
// 的通用信息。垂直度量表包含与特定字形相关的信息。这些表的格式与水平度量的 'hhea' 和 'hmtx'
// 表类似。有关构建 CJK（中文、日文和韩文）字体的更多信息，请参阅"OpenType CJK 字体指南"章节。
//
// 垂直原点和前进高度（Vertical origin and advance height）。字形垂直原点的 y 坐标指定为字
// 形顶部字距（记录在 'vmtx' 表中）与字形边界框顶部（即最大 y 值）之和。
//
// 使用 TrueType 轮廓的 OpenType 字体在字形数据（'glyf'）表中包含字形边界框信息。使用 CFF
// 轮廓的字体不包含字形边界框信息，因此对于这些字体，字形边界框的顶部只能从 'CFF ' 或 CFF2
// 表中的 CharString 数据计算得出。可选的垂直原点（VORG）表可用于使用 CFF 字形数据的字体中，
// 直接记录字形垂直原点的 y 坐标，从而无需将计算边界框作为中间步骤。这提高了应用程序的准确
// 性和效率。
//
// 字形垂直原点的 x 坐标未在 'vmtx' 表中指定。垂直书写实现可以利用基线（BASE）表中的基线值
// （如果存在），以便在 x 方向上根据所需的垂直基线适当对齐字形。字形的前进高度从字形垂直原
// 点的 y 坐标开始，向下延伸。默认情况下，其终点位于文本行中下一个字形垂直原点的 y 坐标处。
// 度量调整的 OpenType 布局特性（如垂直字距调整 'vkrn'）可以以类似于水平模式下字距调整的方
// 式修改垂直前进量。
//
// 垂直度量表的整体结构由下面所示的两个数组组成：vMetrics 数组后跟一个顶部字距数组。顶部字
// 距是相对于字形原点顶部度量的，用于表意文字的垂直排版。本表没有表头，但要求两个数组中包含
// 的字形数量等于字体中的字形总数。
//
// vMetrics 数组中的条目数量由垂直表头表中的 numOfLongVerMetrics 字段值确定。vMetrics 数组
// 为每个包含的条目包含两个值：前进高度和顶部字距。在等宽字体（如 Courier 或汉字）中，所有
// 字形具有相同的前进高度。如果字体是等宽的，第一个数组中只需一个条目，但该条目是必需的。
//
// 垂直度量数组中条目的格式如下：
//      类型    名称            说明
//      UFWORD  advanceHeight   字形的前进高度，以字体设计单位表示
//      FWORD   topSideBearing  字形的顶部字距，以字体设计单位表示
//
// 第二个数组是可选的，通常用于字体中等宽字形的连续段。每个字体只允许一个这样的连续段，且必
// 须位于字体末尾。此数组包含第一个数组中未表示的字形的顶部字距，且此数组中的所有字形必须与
// vMetrics 数组中最后一个条目的前进高度相同。因此，此数组中的所有条目都是等宽的。
//
// 此数组中的条目数量通过字体中的字形数量减去 numOfLongVerMetrics 的值来计算。第一个数组中表
// 示的字形数量加上第二个数组中表示的字形数量之和因此等于字体中的字形数量。顶部字距数组的格
// 式如下：
//      类型    名称                说明
//      FWORD   topSideBearing[]    字形的顶部字距，以字体设计单位表示

// 垂直设备度量（VDMX - Vertical Device Metrics）
//
// VDMX 表可用于包含 TrueType 轮廓的字体中。在 Windows 下，OS/2 表中的 usWinAscent 和
// usWinDescent 值将用于确定字体在任何给定字号下的最大黑色高度。Windows 将此距离称为字
// 体高度。由于 TrueType 指令可能导致字体高度与实际缩放和舍入后的值不同，严格基于 yMax
// 和 yMin 确定字体高度可能导致"像素丢失"。Windows 将裁剪任何延伸到 yMax 上方或 yMin
// 下方的像素。为了避免对整个字体进行网格拟合以确定正确高度，定义了 VDMX 表。
//
// VDMX 表由表头后跟 VDMX 记录组组成（VdmxHeader）：
//      类型        名称                            说明
//      uint16      version                         版本号（0 或 1）
//      uint16      numRecs                         存在的 VDMX 组数量
//      uint16      numRatios                       纵横比分组数量
//      RatioRange  ratRange[numRatios]             比率记录数组
//      Offset16    vdmxGroupOffsets[numRatios]     从此表开头到对应 RatioRange 记录的 VDMXGroup 表的偏移量
//
// RatioRange 记录
//      类型    名称            说明
//      uint8   bCharSet        字符集（见下文）
//      uint8   xRatio          用于 x 比率的值
//      uint8   yStartRatio     起始 y 比率值
//      uint8   yEndRatio       结束 y 比率值
//
// 比率设置如下：
//      纵横比          设置
//      1:1             Ratios.xRatio = 1; Ratios.yStartRatio = 1; Ratios.yEndRatio = 1;
//      1:1 至 2:1      Ratios.xRatio = 2; Ratios.yStartRatio = 1; Ratios.yEndRatio = 2;
//      1.33:1          Ratios.xRatio = 4; Ratios.yStartRatio = 3; Ratios.yEndRatio = 3;
//      所有纵横比      Ratio.xRatio = 0; Ratio.yStartRatio = 0; Ratio.yEndRatio = 0;
//
// 所有值设为 0 表示要使用的默认分组；如果存在，这必须是表中最后一个比率组。2:2 的比率与
// 1:1 相同。纵横比通过基于当前 X 分辨率规范化整个比率范围记录来与目标设备匹配，然后在规
// 范化后对每个记录的 Y 分辨率执行范围检查。一旦找到匹配，搜索即停止。如果在搜索过程中遇
// 到 0,0,0 组，则使用它（因此如果该组不在比率分组的末尾，则其后的任何组都不会被使用）。
// 如果没有匹配且没有 0,0,0 记录，则该纵横比没有 VDMX 数据。
//
// 范围检查概念上按以下方式执行：
//      (deviceXRatio == Ratio.xRatio) && (deviceYRatio >= Ratio.yStartRatio) && (deviceYRatio <= Ratio.yEndRatio)
//
// 每个比率分组引用一个特定的 VDMX 记录组；表中必须至少有 1 个 VDMX 组。bCharSet 值用于表示
// VDMX 组是基于字体文件中存在字形的子集计算的情况。bCharSet 的语义根据 VDMX 表的版本而不同。
// 建议使用 VDMX 版本 1。当前定义的字符集值如下：
//
// 字符集值，版本 0
//      值  说明
//      0   无子集；VDMX 组适用于字体中的所有字形。用于符号或装饰符号字体。
//      1   Windows ANSI 子集；VDMX 组仅使用完成 Windows ANSI 字符集所需的字形计算。Windows 将忽略非 ANSI 子集的 VDMX
//          条目（即 ANSI_CHARSET）。
//
// 字符集值，版本 1
//      值  说明
//      0   无子集；VDMX 组适用于字体中的所有字形。如果向现有字体添加新字符集，请添加此标志和必要的组以支持它。这应仅
//          与 ANSI_CHARSET 结合使用。
//      1   无子集；VDMX 组适用于字体中的所有字形。用于为 Windows 创建新字体时。无需支持 SYMBOL_CHARSET。
//
// VDMX 组紧跟在表头之后。每组记录（只需一组）具有以下布局（VDMXGroup）：
//      类型    名称            说明
//      uint16  recs            此组中的高度记录数量
//      uint8   startsz         起始 yPelHeight
//      uint8   endsz           结束 yPelHeight
//      vTable  entry[recs]     VDMX 记录
//
// vTable 记录
//      类型    名称        说明
//      uint16  yPelHeight  值适用的 yPelHeight
//      int16   yMax        此 yPelHeight 的最大值（以像素计）
//      int16   yMin        此 yPelHeight 的最小值（以像素计）
//
// 此表必须按排序顺序出现（按 yPelHeight 排序），但不需要连续。对于每个 yMax 和 yMin 不线性
// 缩放的 yPelHeight，都应有一个条目，其中线性缩放高度定义为：提示后的 yMax 和 yMin 与缩放/
// 舍入后的 yMax 和 yMin 相同。
//
// 假设一旦 yPelHeight 达到 255，所有高度都将线性，或者至少足够接近线性以至于不再重要。请注
// 意，虽然 Ratios 结构只能支持最大 255 的 ppem 尺寸，但 vTable 结构可以支持更大的 pel 高度
// （最大 65535）。vTable 记录选择 int16 和 uint16 的原因是 yMax 和 yMin 需要是有符号值（127
// 到 -128 的范围太小），并且希望将 vTable 元素按字对齐。

// 最大配置（maxp，Maximum Profile）
//
// 本表确定此字体的内存需求。具有 CFF 或 CFF2 轮廓的字体必须使用本表的版本 0.5，仅指定
// numGlyphs 字段。具有 TrueType 轮廓的字体必须使用本表的版本 1.0，其中所有数据都是必需的。
//
// 版本 0.5
//      类型            名称        描述
//      Version16Dot16  version     版本 0.5 为 0x00005000
//      uint16          numGlyphs   字体中的字形数
//
// 版本 1.0
//      类型            名称    描述
//      Version16Dot16  version                 版本 1.0 为 0x00010000
//      uint16          numGlyphs               字体中的字形数
//      uint16          maxPoints               非复合字形中的最大点数
//      uint16          maxContours             非复合字形中的最大轮廓数
//      uint16          maxCompositePoints      复合字形中的最大点数
//      uint16          maxCompositeContours    复合字形中的最大轮廓数
//      uint16          maxZones                如果指令不使用暮光区（Z0）则为 1，如果使用 Z0 则为 2；在大多数情况下应设置为 2
//      uint16          maxTwilightPoints       Z0 中使用的最大点数
//      uint16          maxStorage              存储区位置的数量
//      uint16          maxFunctionDefs         FDEF 的数量，等于最高函数编号 + 1
//      uint16          maxInstructionDefs      IDEF 的数量
//      uint16          maxStackElements        字体程序（'fpgm' 表）、CVT 程序（'prep' 表）和所有字形指令（在 'glyf' 表中）的最大栈深度
//      uint16          maxSizeOfInstructions   字形指令的最大字节数
//      uint16          maxComponentElements    任何复合字形在"顶层"引用的最大组件数
//      uint16          maxComponentDepth       最大递归级别；简单组件为 1

typedef struct {
    prh_r16 major_version;
    prh_r16 minor_version;
    prh_r16 glyphs;
} prh_font_maxp_table_0_5;

typedef struct {
    prh_r32 maxp_version;
    prh_r16 num_glyphs;
    prh_r16 max_points;
    prh_r16 max_contours;
    prh_r16 max_composite_points;
    prh_r16 max_composite_contours;
    prh_r16 max_zones;
    prh_r16 max_twilinght_points;
    prh_r16 max_storage;
    prh_r16 max_function_defs;
    prh_r16 max_instruction_defs;
    prh_r16 max_stack_elements;
    prh_r16 max_size_of_instructions;
    prh_r16 max_component_elements;
    prh_r16 max_component_depth;
} prh_font_maxp_table;

void prh_load_font_maxp_table(prh_open_font *f) {
    if (f->maxp.length == 0) prh_abort_line();
    prh_font_maxp_table header;
    if (f->is_cff_outline) {
        prh_pread_exact_bytes(&f->font_file->reader, (prh_byte *)&header, sizeof(prh_font_maxp_table_0_5), f->maxp.offset);
        f->maxp_version = prh_bp_4b_to_host((prh_byte *)&header);
        if (f->maxp_version != 0x00005000) prh_abort_error(f->maxp_version);
    } else {
        prh_pread_exact_bytes(&f->font_file->reader, (prh_byte *)&header, sizeof(prh_font_maxp_table), f->maxp.offset);
        f->maxp_version = prh_bp_4b_to_host((prh_byte *)&header);
        if (f->maxp_version != 0x00010000) prh_abort_error(f->maxp_version);
        f->max_points = prh_r16_be_to_host(header.max_points);
        f->max_contours = prh_r16_be_to_host(header.max_contours);
        f->max_composite_points = prh_r16_be_to_host(header.max_composite_points);
        f->max_composite_contours = prh_r16_be_to_host(header.max_composite_contours);
        f->max_zones = prh_r16_be_to_host(header.max_zones);
        f->max_twilinght_points = prh_r16_be_to_host(header.max_twilinght_points);
        f->max_storage = prh_r16_be_to_host(header.max_storage);
        f->max_function_defs = prh_r16_be_to_host(header.max_function_defs);
        f->max_instruction_defs = prh_r16_be_to_host(header.max_instruction_defs);
        f->max_stack_elements = prh_r16_be_to_host(header.max_stack_elements);
        f->max_size_of_instructions = prh_r16_be_to_host(header.max_size_of_instructions);
        f->max_component_elements = prh_r16_be_to_host(header.max_component_elements);
        f->max_component_depth = prh_r16_be_to_host(header.max_component_depth);
    }
    f->num_glyphs = prh_r16_be_to_host(header.num_glyphs);
    if (f->num_glyphs == 0) prh_abort_line();
}

void prh_print_font_maxp_table(prh_open_font *f) {
    if (f->maxp.length == 0) prh_abort_line();
    prh_font_maxp_table t = {0};
    prh_r32 maxp_length = f->is_cff_outline ? (prh_r32)sizeof(prh_font_maxp_table_0_5) : (prh_r32)sizeof(prh_font_maxp_table);
    if (maxp_length != f->maxp.length) {
        prh_print("table 'maxp' invalid length %d %d\n", (prh_reg)maxp_length, (prh_reg)f->maxp.length);
        return;
    }
    if (f->is_cff_outline) {
        prh_pread_exact_bytes(&f->font_file->reader, (prh_byte *)&t, maxp_length, f->maxp.offset);
        f->maxp_version = prh_bp_4b_to_host((prh_byte *)&t);
        if (f->maxp_version != 0x00005000) prh_abort_error(f->maxp_version);
    } else {
        prh_pread_exact_bytes(&f->font_file->reader, (prh_byte *)&t, maxp_length, f->maxp.offset);
        f->maxp_version = prh_bp_4b_to_host((prh_byte *)&t);
        if (f->maxp_version != 0x00010000) prh_abort_error(f->maxp_version);
        f->max_points = prh_r16_be_to_host(t.max_points);
        f->max_contours = prh_r16_be_to_host(t.max_contours);
        f->max_composite_points = prh_r16_be_to_host(t.max_composite_points);
        f->max_composite_contours = prh_r16_be_to_host(t.max_composite_contours);
        f->max_zones = prh_r16_be_to_host(t.max_zones);
        f->max_twilinght_points = prh_r16_be_to_host(t.max_twilinght_points);
        f->max_storage = prh_r16_be_to_host(t.max_storage);
        f->max_function_defs = prh_r16_be_to_host(t.max_function_defs);
        f->max_instruction_defs = prh_r16_be_to_host(t.max_instruction_defs);
        f->max_stack_elements = prh_r16_be_to_host(t.max_stack_elements);
        f->max_size_of_instructions = prh_r16_be_to_host(t.max_size_of_instructions);
        f->max_component_elements = prh_r16_be_to_host(t.max_component_elements);
        f->max_component_depth = prh_r16_be_to_host(t.max_component_depth);
    }
    f->num_glyphs = prh_r16_be_to_host(t.num_glyphs);
    if (f->num_glyphs == 0) prh_abort_line();
    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)&t, maxp_length);
    prh_print(
        "maxp table index %d / %d\n"
        "maxp table tag 0x%08x (%c%c%c%c)\n"
        "maxp table offset %.10d (%d/4)\n"
        "maxp table length %.10d (%d/4)\n"
        "maxp table checksum 0x%08x 0x%08x (valid %d)\n"
        "maxp table version %08x\n"
        "----------------------------\n"
        "maxp num_glyphs %d\n",
        (prh_reg)f->maxp.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->maxp.tabletag,
        (prh_reg)prh_byte_4(f->maxp.tabletag),
        (prh_reg)prh_byte_3(f->maxp.tabletag),
        (prh_reg)prh_byte_2(f->maxp.tabletag),
        (prh_reg)prh_byte_1(f->maxp.tabletag),
        (prh_reg)f->maxp.offset, (prh_reg)f->maxp.offset % 4,
        (prh_reg)f->maxp.length, (prh_reg)f->maxp.length % 4,
        (prh_reg)f->maxp.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->maxp.checksum),
        (prh_reg)f->maxp_version,
        (prh_reg)f->num_glyphs);
    if (f->maxp_version == 0x00010000) {
        prh_print(
            "maxp max_points %d\n"
            "maxp max_contours %d\n"
            "maxp max_composite_points %d\n"
            "maxp max_composite_contours %d\n"
            "maxp max_zones %d\n"
            "maxp max_twilinght_points %d\n"
            "maxp max_storage %d\n"
            "maxp max_function_defs %d\n"
            "maxp max_instruction_defs %d\n"
            "maxp max_stack_elements %d\n"
            "maxp max_size_of_instructions %d\n"
            "maxp max_component_elements %d\n"
            "maxp max_component_depth %d\n",
            (prh_reg)f->max_points,
            (prh_reg)f->max_contours,
            (prh_reg)f->max_composite_points,
            (prh_reg)f->max_composite_contours,
            (prh_reg)f->max_zones,
            (prh_reg)f->max_twilinght_points,
            (prh_reg)f->max_storage,
            (prh_reg)f->max_function_defs,
            (prh_reg)f->max_instruction_defs,
            (prh_reg)f->max_stack_elements,
            (prh_reg)f->max_size_of_instructions,
            (prh_reg)f->max_component_elements,
            (prh_reg)f->max_component_depth);
    }
    prh_print("\n");
}

// 垂直原点表（VORG, Vertical Origin Table）
//
// 此可选表指定字体中每个字形的垂直原点的 y 坐标。此表只能在 CFF 或 CFF2 OpenType 字体中使用。
// 如果存在于包含 TrueType 轮廓数据的 OpenType 字体中，则必须忽略：垂直度量（'vmtx'）和字形数
// 据（'glyf'）表提供准确计算字形垂直原点 y 坐标所需的所有信息。有关更多详细信息，请参阅 'vmtx'
// 表规范中的"垂直原点和前进高度"部分。
//
// 对于所有支持垂直书写的 OpenType 字体，'vmtx' 和垂直头（'vhea'）表仍然是必需的。前进高度必须
// 继续从 'vmtx' 表获取；这是存储它们的唯一位置。
//
// 如果 CFF 或 CFF2 OpenType 字体中存在 VORG 表，应用程序可以选择通过以下方式获取字形垂直原点
// 的 y 坐标：
//  1.  直接从 VORG 表获取，或
//  2.  首先通过 CFF 或 CFF2 CharString 数据计算字形边界框的顶部，然后加上 'vmtx' 表中字形的顶
//      部侧承（top side bearing）
//
// 前一种方法提供更高准确性和效率的优势，因为根据边界框算法的舍入决策和数据类型，从 CFF 或 CFF2
// CharString 计算的边界框结果可能不同。后一种方法为不了解或选择不支持 VORG 的应用程序提供兼容性。
// 因此，VORG 表本身不添加任何新的字体度量值；它只是提高了应用程序的准确性和效率，因为从 CFF 或
// CFF2 CharString 计算边界框的中间步骤变得不必要。
//
// 有关构建 CJK（中文、日文和韩文）字体的更多信息，请参阅建议章节中的 OpenType CJK 字体指南。
// https://learn.microsoft.com/en-us/typography/opentype/spec/recom#cjk
//
// 垂直原点表格式。VORG 表的结构如下：
//      类型                名称                                        描述
//      uint16              majorVersion                                主要版本——设置为 1
//      uint16              minorVersion                                次要版本——设置为 0
//      int16               defaultVertOriginY                          如果 vertOriginYMetrics 数组中没有该字形的条目，
//                                                                      则使用的字形垂直原点 y 坐标（以字体的设计坐标系
//                                                                      表示）
//      uint16              numVertOriginYMetrics                       vertOriginYMetrics 数组中的元素数
//      VertOriginYMetrics  vertOriginYMetrics[numVertOriginYMetrics]   按字形 ID 排序的 VertOriginYMetrics 记录数组
//
// VertOriginYMetrics 记录具有以下格式：
//      类型    名称        描述
//      uint16  glyphIndex  字形索引
//      int16   vertOriginY 字形垂直原点的 y 坐标，以字体的设计坐标系表示
//
// 此数组必须按 glyphIndex 递增排序，且不得有多个具有相同 glyphIndex 的元素。在大小优化的
// 实现中，垂直原点 y 坐标等于 defaultVertOriginY 的字形在此数组中没有条目。
//
// 如果字体中所有字形共享相同的 defaultVertOriginY 值，则在大小优化的实现中，VORG 表的长度
// 将为 8 字节，因为 vertOriginYMetrics 数组将不存在。
//
// 通常，只有东亚字体中的全角拉丁字形会在 vertOriginYMetrics 数组中有条目。为垂直书写旋转
// 的字形，例如垂直替代和旋转（'vrt2'）功能中使用的字形，如果设计适当，可以利用默认值。
//
// 在以下 1000 单位 em 字体（1000-unit-em font）的完整 VORG 表示例中，除字形索引 10、12 和
// 13 外，字体中每个字形都指定为 vertOriginY 为 880：
//      majorVersion         =1
//      minorVersion         =0
//      defaultVertOriginY   =880
//      numVertOriginYMetrics=3
//      --- vertOriginYMetrics[index]=(glyphIndex,vertOriginY)
//      [0]=(10,889)
//      [1]=(12,861)
//      [2]=(13,849)

typedef struct {
    prh_r16 major_version;
    prh_r16 minor_version;
    prh_i16 default_vorg_y;
    prh_r16 num_vorg_metrics;
} prh_font_vorg_table;

typedef struct {
    prh_r16 glyph_index;
    prh_i16 vert_orgin_y;
} prh_font_vorg_metric;

#define prh_impl_font_vert_metric_glyph_index(metric) prh_bp_2b_to_host((prh_byte *)&(metric)->glyph_index)

prh_i16 prh_font_glyph_vert_origin_y(prh_r32 glyph_index, prh_i16 default_origin_y, prh_font_vorg_metric *metric, prh_r32 n) {
    prh_reg i;
    prh_generic_bsearch_last_greater_equal(i, glyph_index, metric, n, prh_impl_font_vert_metric_glyph_index);
    if (i != (prh_reg)-1 && glyph_index == prh_bp_2b_to_host((prh_byte *)&metric[i].glyph_index)) return prh_bp_2b_to_host((prh_byte *)&metric[i].vert_orgin_y);
    return default_origin_y;
}

void prh_print_font_vorg_table(prh_open_font *f) {
    if (f->vorg.length == 0) return;
    prh_byte *table_data = prh_load_font_table(f, &f->vorg);
    prh_r32 table_version = prh_bp_4b_to_host(table_data);
    prh_i16 default_y = (prh_i16)prh_bp_2b_to_host(table_data + 4);
    prh_r16 num_metrics = prh_bp_2b_to_host(table_data + 6);
    prh_r32 table_length = (prh_r32)sizeof(prh_font_vorg_table) + num_metrics * (prh_r32)sizeof(prh_font_vorg_metric);
    prh_real_assert(table_length == f->vorg.length);

    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)table_data, f->vorg.length);
    prh_print(
        "vorg table index %d / %d\n"
        "vorg table tag 0x%08x (%c%c%c%c)\n"
        "vorg table offset %.10d (%d/4)\n"
        "vorg table length %.10d (%d/4)\n"
        "vorg table checksum 0x%08x 0x%08x (valid %d)\n"
        "vorg table version %08x\n"
        "vorg table default y %d\n"
        "vorg table num metrics %d\n",
        (prh_reg)f->vorg.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->vorg.tabletag,
        (prh_reg)prh_byte_4(f->vorg.tabletag),
        (prh_reg)prh_byte_3(f->vorg.tabletag),
        (prh_reg)prh_byte_2(f->vorg.tabletag),
        (prh_reg)prh_byte_1(f->vorg.tabletag),
        (prh_reg)f->vorg.offset, (prh_reg)f->vorg.offset % 4,
        (prh_reg)f->vorg.length, (prh_reg)f->vorg.length % 4,
        (prh_reg)f->vorg.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->vorg.checksum),
        (prh_reg)table_version,
        (prh_reg)default_y,
        (prh_reg)num_metrics);

    prh_font_vorg_metric *vorg_metrics = (prh_font_vorg_metric *)(table_data + 8);
    if (num_metrics > 2) {
        prh_r32 glyph_a = prh_bp_2b_to_host((prh_byte *)&vorg_metrics[0].glyph_index);
        prh_r32 glyph_b = prh_bp_2b_to_host((prh_byte *)&vorg_metrics[1].glyph_index);
        prh_r32 glyph_c = prh_bp_2b_to_host((prh_byte *)&vorg_metrics[num_metrics - 1].glyph_index);
        prh_print(
            "vorg table glyph %d origin y %d\n"
            "vorg table glyph %d origin y %d\n"
            "vorg table glyph %d origin y %d\n"
            "vorg table glyph %d origin y %d\n"
            "vorg table glyph %d origin y %d\n"
            "vorg table glyph %d origin y %d\n"
            "vorg table glyph %d origin y %d\n"
            "vorg table glyph %d origin y %d\n"
            "vorg table glyph %d origin y %d\n",
            (prh_reg)glyph_a - 1, (prh_reg)prh_font_glyph_vert_origin_y(glyph_a - 1, default_y, vorg_metrics, num_metrics),
            (prh_reg)glyph_a + 0, (prh_reg)prh_font_glyph_vert_origin_y(glyph_a, default_y, vorg_metrics, num_metrics),
            (prh_reg)glyph_a + 1, (prh_reg)prh_font_glyph_vert_origin_y(glyph_a + 1, default_y, vorg_metrics, num_metrics),
            (prh_reg)glyph_b - 1, (prh_reg)prh_font_glyph_vert_origin_y(glyph_b - 1, default_y, vorg_metrics, num_metrics),
            (prh_reg)glyph_b + 0, (prh_reg)prh_font_glyph_vert_origin_y(glyph_b, default_y, vorg_metrics, num_metrics),
            (prh_reg)glyph_b + 1, (prh_reg)prh_font_glyph_vert_origin_y(glyph_b + 1, default_y, vorg_metrics, num_metrics),
            (prh_reg)glyph_c - 1, (prh_reg)prh_font_glyph_vert_origin_y(glyph_c - 1, default_y, vorg_metrics, num_metrics),
            (prh_reg)glyph_c + 0, (prh_reg)prh_font_glyph_vert_origin_y(glyph_c, default_y, vorg_metrics, num_metrics),
            (prh_reg)glyph_c + 1, (prh_reg)prh_font_glyph_vert_origin_y(glyph_c + 1, default_y, vorg_metrics, num_metrics));
    }

    prh_r32 i, count = num_metrics > 8 ? 8 : num_metrics;
    for (i = 0; i < count; i += 1) {
        prh_print(
            "----------------------------\n"
            "vorg metric index %d / %d\n"
            "vorg metric glyph index %d\n"
            "vorg metric origin y %d\n",
            (prh_reg)(i + 1),
            (prh_reg)num_metrics,
            (prh_reg)prh_bp_2b_to_host((prh_byte *)&vorg_metrics[i].glyph_index),
            (prh_reg)prh_bp_2b_to_host((prh_byte *)&vorg_metrics[i].vert_orgin_y));
    }

    for (i = num_metrics - 8; i < num_metrics; i += 1) {
        prh_print(
            "----------------------------\n"
            "vorg metric index %d / %d\n"
            "vorg metric glyph index %d\n"
            "vorg metric origin y %d\n",
            (prh_reg)(i + 1),
            (prh_reg)num_metrics,
            (prh_reg)prh_bp_2b_to_host((prh_byte *)&vorg_metrics[i].glyph_index),
            (prh_reg)prh_bp_2b_to_host((prh_byte *)&vorg_metrics[i].vert_orgin_y));
    }

    prh_print("\n");
    prh_da_free(table_data);
}

// PostScript 表（post）
//
// 本表包含 PostScript 打印机使用的 OpenType 字体所需的附加信息。其中包括 Type 1 字体 FontInfo
// 字典中的某些数据，以及所有字形的 PostScript 名称。有关 PostScript 名称的更多信息，请
// 参阅 Adobe 字形列表规范。https://github.com/adobe-type-tools/agl-specification
//
// 版本 1.0、2.0 和 2.5 的 'post' 表仅用于包含 TrueType 或 CFF 版本 2 轮廓数据的字体。包
// 含 TrueType 或 CFF 版本 2 数据的字体也可以使用版本 3.0 的 'post' 表。包含 CFF 版本 1
// 轮廓数据的 OpenType 字体必须仅使用版本 3.0 的 'post' 表。
//
// 表头。表的开头如下：
//      类型            名称                说明
//      Version16Dot16  version             0x00010000 表示版本 1.0；0x00020000 表示版本 2.0；0x00025000 表示版本 2.5（已弃
//                                          用）；0x00030000 表示版本 3.0。
//      Fixed           italicAngle         斜体角度，以逆时针方向偏离垂直线的度数表示。正立文本为 0，向右倾斜（前倾）为负值。
//      FWORD           underlinePosition   建议的下划线顶部的 y 坐标。注意，PostScript 语言将 UnderLinePosition FontInfo 定
//                                          义为从基线到下划线中心的距离。此定义不用于 'post' 表。
//      FWORD           underlineThickness  建议的下划线厚度值。通常，下划线厚度应与下划线字符（U+005F LOW LINE）的粗细相匹配，
//                                          并且也应与删除线粗细相匹配，后者在 OS/2 表 yStrikeoutSize 中指定。
//      uint32          isFixedPitch        如果字体为比例间距（proportionally spaced），则设为 0；如果字体不是比例间距（即等
//                                          宽，monospaced），则设为非零。
//      uint32          minMemType42        OpenType 字体下载时的最小内存使用量。
//      uint32          maxMemType42        OpenType 字体下载时的最大内存使用量。
//      uint32          minMemType1         OpenType 字体作为 Type 1 字体下载时的最小内存使用量。
//      uint32          maxMemType1         OpenType 字体作为 Type 1 字体下载时的最大内存使用量。
//
// 表中最后四项的存在是因为，如果 PostScript 驱动程序在下载 OpenType 字体之前就知道其虚拟
// 内存（VM）需求，则可以更好地进行内存管理。如果已知，应提供此信息；如果未知，则将值设为
// 零。驱动程序仍然可以工作，但效率会降低。最大内存使用量等于最小内存使用量加上最大运行时
// 内存使用量。最大运行时内存使用量取决于字体光栅化器可能光栅化的任何位图的最大条带大小。
// 可以通过在不同字号下渲染字符并比较内存使用量来计算运行时内存使用量。
//
// 如果版本为 1.0 或 3.0，则表到此结束。版本 2.0 和 2.5 的附加条目如下所示。Apple 为其平台
// 定义了版本 4.0（请参阅 Apple 的规范），但 OpenType 不支持。
// https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6post.html
//
// 版本 1.0。此版本用于字体文件恰好包含标准 Macintosh TrueType 字体文件中的 258 个字形时（有
// 关 258 个 Macintosh 字形名称的列表，请参阅 Apple 规范中的 'post' 格式 1），且字体未另行提
// 供字形名称。因此，字形名称从系统中获取，字体无需存储。
//
// 版本 2.0。版本 2.0 用于使用不在 Macintosh 字形名称集合中的字形名称的字体。给定字体可能将
// 其某些字形映射到标准 Macintosh 字形名称，某些映射到其自己的自定义名称。版本 2.0 的 'post'
// 表可用于包含 TrueType 或 CFF 版本 2 轮廓的字体。对于版本 2.0，以下字段附加在表头末尾：
//      类型    名称                        说明
//      uint16  numGlyphs                   字形数量（应与 'maxp' 表中的 numGlyphs 相同）
//      uint16  glyphNameIndex[numGlyphs]   指向字符串数据的索引数组
//      uint8   stringData[variable]        字符串数据的存储空间
//
// 此字体文件包含不在标准 Macintosh 集合中的字形，或者字体文件中字形的排列顺序与标准 Macintosh
// 集合不同。glyphNameIndex 数组将字形 ID 映射到字形名称索引。如果字形名称索引在 0 到 257（含）
// 之间，则将该索引视为 Macintosh 标准字形集中的字形索引，并使用 Macintosh 字形名称。如果字形
// 名称索引在 258 到 65535 之间，则减去 258，并用该值索引表末尾的 Pascal 字符串列表。
//
// 例如，假设 glyphNameIndex[302]（对应字形 ID 302）为 217：由于该字形名称索引小于 258，因此字
// 形名称为字形 ID 217 的 Macintosh 字形名称。假设 glyphNameIndex[408] 为 262：减去 258，差值为
// 4；该字形的字形名称为字符串数据中的第五个字符串（索引 4，从 0 开始）。
//
// 字符串数据采用 Pascal 字符串格式，即给定字符串的第一个字节是长度：该字符串中的字符数。长度字
// 节本身不计入；例如，长度字节为 8 表示随后的 8 个字节构成字符串字符数据。要查找给定字形名称索
// 引的字符串，从第一个长度字节开始，前进该字节指定的字节数以找到下一个字符串条目的长度字节，依
// 此类推。
//
// 字形名称字符串使用 ASCII 编码。有效字符限于 A–Z、a–z、0–9、"."（FULL STOP）和 "_"（LOW LINE）。
// 名称长度不得超过 63 个字符；某些旧实现可能假设长度限制为 31 个字符。如果不想为特定字形关联
// PostScript 名称，请使用 0（指向名称 .notdef）作为该字形 ID 的 glyphNameIndex 条目。
//
// 版本 2.5。版本 2.5 的 'post' 表已弃用。此版本为包含 TrueType 轮廓的字体提供节省空间的表，这些
// 字体仅包含标准 Macintosh 字形集的子集，或对标准 Macintosh 字形集进行了简单的重新排序。
//      类型    名称                说明
//      uint16  numGlyphs           字形数量
//      int8    offset[numGlyphs]   字形索引与标准顺序中字形索引的差值
//
// 此版本已用于某些旧版字体，这些字体包含 TrueType 轮廓，仅包含标准 Macintosh 字形集中的字形，但
// 这些字形的排列顺序非标准，或缺少某些字形。表中包含字体文件中每个字形的一个字节。该字节被视为有
// 符号偏移量，将字体中使用的字形索引映射到标准字形索引。例如，假设字体包含三个字形 A、B 和 C，它
// 们在标准顺序中分别是第 37、38 和 39 个字形，则 'post' 表将包含字节 +36、+36、+36。
//
// 版本 3.0。此版本使得创建不受大量字形名称负担的字体成为可能。版本 3.0 的 'post' 表可由包含
// TrueType 或 CFF（版本 1 或 2）数据的 OpenType 字体使用。此版本指定不为该字体文件中的字形提
// 供 PostScript 名称信息。此版本在 PostScript 打印机上的打印行为未指定，但不应导致致命或不可
// 恢复的错误。某些驱动程序可能不打印任何内容；其他驱动程序可能尝试使用默认命名方案进行打印。
// 注意：Windows 使用 'post' 表中的斜体角度值，但实际上不需要将任何字形名称存储为 Pascal 字符
// 串。
//
// 'post' 表与 OpenType 字体变体。在可变字体中，'post' 表中的各种字体度量值可能需要针对不同的
// 变体实例进行调整。'post' 条目的变体数据可以在度量变体（MVAR）表中提供。不同的 'post' 条目通
// 过值标签与 MVAR 表中的特定变体数据相关联，如下所示：
//      'post' 条目         标签
//      underlinePosition   'undo'
//      underlineThickness  'unds'
//
// 注意：italicAngle 值不由变体数据调整，因为这对应于可用于定义字体变体空间的 'slnt' 变体轴。
// 变体实例的适当 post.italicAngle 值可以从用于选择特定变体实例的 'slnt' 用户坐标推导得出。有
// 关 italicAngle 与 'slnt' 轴之间关系的详细信息，请参阅 OpenType 设计变体轴标签注册表中的 'slnt'
// 轴讨论。https://learn.microsoft.com/en-us/typography/opentype/spec/dvaraxisreg
//
// 有关 OpenType 字体变体的一般信息，请参阅"OpenType 字体变体概述"章节。
// https://learn.microsoft.com/en-us/typography/opentype/spec/otvaroverview

typedef struct {
    prh_r32 version;
    prh_i16 italic_angle_upper; // 32 位有符号定点数（16.16）
    prh_r16 italic_angle_lower;
    prh_i16 underline_y_position;
    prh_i16 underline_thickness;
    prh_r32 is_fixed_pitch;
    prh_r32 type42_min_mem;
    prh_r32 type42_max_mem;
    prh_r32 type1_min_mem;
    prh_r32 type1_max_mem;
} prh_font_post_table;

void prh_print_font_post_table(prh_open_font *f) {
    if (f->post.length == 0) return;
    prh_font_post_table *p = (prh_font_post_table *)prh_load_font_table(f, &f->post);
    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)p, f->post.length);
    prh_print(
        "post table index %d / %d\n"
        "post table tag 0x%08x (%c%c%c%c)\n"
        "post table offset %.10d (%d/4)\n"
        "post table length %.10d (%d/4)\n"
        "post table checksum 0x%08x 0x%08x (valid %d)\n"
        "post table version %08x\n"
        "----------------------------\n"
        "post metric - italic angle %d.%d\n"
        "post metric - underline y position %d\n"
        "post metric - underline thickness %d\n"
        "post metric - monospace font %d\n"
        "post metric - type42 memory %d ~ %d\n"
        "post metric - type1 memory %d ~ %d\n\n",
        (prh_reg)f->post.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->post.tabletag,
        (prh_reg)prh_byte_4(f->post.tabletag),
        (prh_reg)prh_byte_3(f->post.tabletag),
        (prh_reg)prh_byte_2(f->post.tabletag),
        (prh_reg)prh_byte_1(f->post.tabletag),
        (prh_reg)f->post.offset, (prh_reg)f->post.offset % 4,
        (prh_reg)f->post.length, (prh_reg)f->post.length % 4,
        (prh_reg)f->post.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->post.checksum),
        (prh_reg)prh_r32_be_to_host(p->version),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->italic_angle_upper),
        (prh_reg)prh_r16_be_to_host(p->italic_angle_lower),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->underline_y_position),
        (prh_reg)prh_r16_be_to_host(p->underline_thickness),
        (prh_reg)prh_r32_be_to_host(p->is_fixed_pitch),
        (prh_reg)prh_r32_be_to_host(p->type42_min_mem), (prh_reg)prh_r32_be_to_host(p->type42_max_mem),
        (prh_reg)prh_r32_be_to_host(p->type1_min_mem), (prh_reg)prh_r32_be_to_host(p->type1_max_mem));
    prh_da_free((prh_byte *)p);
}

// OS/2 和 Windows 度量表
//
// OS/2 表由一组在 OpenType 字体中所需的度量值和其他数据组成。OS/2 表格式。OS/2 表已定义
// 六个版本：版本 0 到 5。所有版本均受支持，但强烈建议使用版本 4 或更高版本。
//
// 版本 5 的格式如下：
//      类型    名称                    备注
//      uint16  version                 0x0005
//      FWORD   xAvgCharWidth
//      uint16  usWeightClass
//      uint16  usWidthClass
//      uint16  fsType
//      FWORD   ySubscriptXSize
//      FWORD   ySubscriptYSize
//      FWORD   ySubscriptXOffset
//      FWORD   ySubscriptYOffset
//      FWORD   ySuperscriptXSize
//      FWORD   ySuperscriptYSize
//      FWORD   ySuperscriptXOffset
//      FWORD   ySuperscriptYOffset
//      FWORD   yStrikeoutSize
//      FWORD   yStrikeoutPosition
//      int16   sFamilyClass
//      uint8   panose[10]
//      uint32  ulUnicodeRange1         位 0 – 31
//      uint32  ulUnicodeRange2         位 32 – 63
//      uint32  ulUnicodeRange3         位 64 – 95
//      uint32  ulUnicodeRange4         位 96 – 127
//      Tag     achVendID
//      uint16  fsSelection
//      uint16  usFirstCharIndex
//      uint16  usLastCharIndex
//      FWORD   sTypoAscender
//      FWORD   sTypoDescender
//      FWORD   sTypoLineGap
//      UFWORD  usWinAscent
//      UFWORD  usWinDescent
//      uint32  ulCodePageRange1        位 0 – 31
//      uint32  ulCodePageRange2        位 32 – 63
//      FWORD   sxHeight
//      FWORD   sCapHeight
//      uint16  usDefaultChar
//      uint16  usBreakChar
//      uint16  usMaxContext
//      uint16  usLowerOpticalPointSize
//      uint16  usUpperOpticalPointSize
//
// 版本 4 在 OpenType 1.5 中定义。版本 4 比版本 5 少两个字段，其余字段与版本 3 相同。虽然
// 未在版本 3 的基础上添加新字段，但某些字段的规范已修订。版本 4 的格式如下：
//      类型    名称
//      uint16  version
//      FWORD   xAvgCharWidth
//      uint16  usWeightClass
//      uint16  usWidthClass
//      uint16  fsType
//      FWORD   ySubscriptXSize
//      FWORD   ySubscriptYSize
//      FWORD   ySubscriptXOffset
//      FWORD   ySubscriptYOffset
//      FWORD   ySuperscriptXSize
//      FWORD   ySuperscriptYSize
//      FWORD   ySuperscriptXOffset
//      FWORD   ySuperscriptYOffset
//      FWORD   yStrikeoutSize
//      FWORD   yStrikeoutPosition
//      int16   sFamilyClass
//      uint8   panose[10]
//      uint32  ulUnicodeRange1
//      uint32  ulUnicodeRange2
//      uint32  ulUnicodeRange3
//      uint32  ulUnicodeRange4
//      Tag     achVendID
//      uint16  fsSelection
//      uint16  usFirstCharIndex
//      uint16  usLastCharIndex
//      FWORD   sTypoAscender
//      FWORD   sTypoDescender
//      FWORD   sTypoLineGap
//      UFWORD  usWinAscent
//      UFWORD  usWinDescent
//      uint32  ulCodePageRange1
//      uint32  ulCodePageRange2
//      FWORD   sxHeight
//      FWORD   sCapHeight
//      uint16  usDefaultChar
//      uint16  usBreakChar
//      uint16  usMaxContext
//
// 版本 3 在 OpenType 1.4 中定义。版本 3 的字段与版本 4 相同，也与版本 2 相同。虽然未在
// 版本 2 的基础上添加新字段，但某些字段的规范已修订以反映 Unicode 3.2 的变化。版本 3 的
// 格式与上述版本 4 的格式相同。
//
// 版本 2 在 OpenType 1.1 中定义。版本 2 的字段与版本 3 相同，比版本 1 多五个字段。版本 2
// 的格式与上述版本 4 的格式相同。
//
// 版本 1 在 TrueType 修订版 1.66 中定义。版本 1 比版本 2 少五个字段，比版本 0 多两个字段。
// 版本 1 的格式如下：
//      类型    名称
//      uint16  version
//      FWORD   xAvgCharWidth
//      uint16  usWeightClass
//      uint16  usWidthClass
//      uint16  fsType
//      FWORD   ySubscriptXSize
//      FWORD   ySubscriptYSize
//      FWORD   ySubscriptXOffset
//      FWORD   ySubscriptYOffset
//      FWORD   ySuperscriptXSize
//      FWORD   ySuperscriptYSize
//      FWORD   ySuperscriptXOffset
//      FWORD   ySuperscriptYOffset
//      FWORD   yStrikeoutSize
//      FWORD   yStrikeoutPosition
//      int16   sFamilyClass
//      uint8   panose[10]
//      uint32  ulUnicodeRange1
//      uint32  ulUnicodeRange2
//      uint32  ulUnicodeRange3
//      uint32  ulUnicodeRange4
//      Tag     achVendID
//      uint16  fsSelection
//      uint16  usFirstCharIndex
//      uint16  usLastCharIndex
//      FWORD   sTypoAscender
//      FWORD   sTypoDescender
//      FWORD   sTypoLineGap
//      UFWORD  usWinAscent
//      UFWORD  usWinDescent
//      uint32  ulCodePageRange1
//      uint32  ulCodePageRange2
//
// 版本 0 在 TrueType 修订版 1.5 中定义。版本 0 的格式如下：
//      类型    名称
//      uint16  version
//      FWORD   xAvgCharWidth
//      uint16  usWeightClass
//      uint16  usWidthClass
//      uint16  fsType
//      FWORD   ySubscriptXSize
//      FWORD   ySubscriptYSize
//      FWORD   ySubscriptXOffset
//      FWORD   ySubscriptYOffset
//      FWORD   ySuperscriptXSize
//      FWORD   ySuperscriptYSize
//      FWORD   ySuperscriptXOffset
//      FWORD   ySuperscriptYOffset
//      FWORD   yStrikeoutSize
//      FWORD   yStrikeoutPosition
//      int16   sFamilyClass
//      uint8   panose[10]
//      uint32  ulUnicodeRange1
//      uint32  ulUnicodeRange2
//      uint32  ulUnicodeRange3
//      uint32  ulUnicodeRange4
//      Tag     achVendID
//      uint16  fsSelection
//      uint16  usFirstCharIndex
//      uint16  usLastCharIndex
//      FWORD   sTypoAscender
//      FWORD   sTypoDescender
//      FWORD   sTypoLineGap
//      UFWORD  usWinAscent
//      UFWORD  usWinDescent
//
// 注意：Apple TrueType 参考手册中对 OS/2 版本 0 的文档在 usLastCharIndex 字段处停止，未
// 包含 Microsoft 定义的表中最后五个字段。某些旧版 TrueType 字体可能是使用缩短的版本 0
// OS/2 表构建的。应用程序应在读取这些字段之前检查版本 0 OS/2 表的长度。
//
// OS/2 字段详情：
//
// version
//      格式： uint16
//      单位： 无
//      标题： OS/2 表版本号
//      说明： OS/2 表的版本号：0x0000 到 0x0005
//      备注： 版本号允许识别 OS/2 表的确切内容和布局
//
// xAvgCharWidth
//      格式： FWORD
//      单位： 字体设计单位（font design units）
//      标题： 平均加权字宽（escapement/width）
//      说明： 平均字符宽度字段指定字体中所有非零宽度字形的字宽（宽度）的算术平均值。
//      备注： xAvgCharWidth 的值通过获取字体中所有非零宽度字形的宽度的算术平均值来计算。
//             强烈建议实现者不要依赖此值来计算文本行的布局，尤其是在使用复杂文字的情况
//             下。注意，某些字体的 xAvgCharWidth 值不符合此计算。字体可能最初使用版本 0
//             到版本 2 的 OS/2 表创建，并采用下面描述的遗留计算方式，后来更新为较新的
//             OS/2 表版本但未更改 xAvgCharWidth 值。在某些 CJK 字体中，xAvgCharWidth 可
//             能几乎是上述计算预期值的一半。应用程序不应使用 xAvgCharWidth 来确定实际的
//             字形前进宽度。
//      版本差异： 版本 0 到 2：首次定义时，该规范偏向基本拉丁字符，并认为 xAvgCharWidth
//             值可用于估计文本行的平均长度。提供了以下计算 xAvgCharWidth 的公式：对于下
//             表中给出的字符，将每个字符的宽度乘以下指示的权重因子，然后将总和除以 1000。
//             此计算该字段值的方法在引入 OS/2 表版本 3 时被取代，并已弃用。
//                  字母    权重因子        字母    权重因子
//                  a       64              o       56
//                  b       14              p       17
//                  c       27              q       4
//                  d       35              r       49
//                  e       100             s       56
//                  f       20              t       71
//                  g       14              u       31
//                  h       42              v       10
//                  i       63              w       18
//                  j       3               x       3
//                  k       6               y       18
//                  l       35              z       2
//                  m       20              space   166
//                  n       56
//
// usWeightClass
//      格式： uint16
//      标题： 字重类别
//      说明： 指示字体中字符的视觉字重（笔画黑度或粗细程度）。支持 1 到 1000 的值。
//      备注： usWeightClass 值使用与可变字体的 'fvar' 表和 STAT 表中使用的 'wght' 轴相同的
//             刻度。虽然支持 1 到 1000 的整数值，但某些旧平台可能对支持的值有限制。以下是
//             常用值：
//                  值      说明                                C 定义（来自 windows.h）
//                  100     极细（Thin）                        FW_THIN
//                  200     特轻（Extra-light / Ultra-light）   FW_EXTRALIGHT
//                  300     轻（Light）                         FW_LIGHT
//                  400     常规（Normal / Regular）            FW_NORMAL
//                  500     中等（Medium）                      FW_MEDIUM
//                  600     半粗（Semi-bold / Demi-bold）       FW_SEMIBOLD
//                  700     粗体（Bold）                        FW_BOLD
//                  800     特粗（Extra-bold / Ultra-bold）     FW_EXTRABOLD
//                  900     黑体（Black / Heavy）               FW_BLACK
//
// usWidthClass
//      格式： uint16
//      标题： 宽度类别
//      说明： 指示字体中字形相对于字体设计师指定的正常宽高比的相对变化
//      备注： 虽然字体中每个字形可以有不同的数字宽高比，但正常宽度字体中的每个字形被认为
//             具有相对宽高比 1。当创建不同宽度类别的新字体样式时（由字体设计师或通过某些
//             自动化方式），新字体中字形的相对宽高比是正常字体中相应字形的某个百分比更大
//             或更小——正是这个差异由本字段指定。
//             有效的 usWidthClass 值如下表所示。注意，usWidthClass 值与可变字体的 'fvar'
//             表和 STAT 表中使用的 'wdth' 轴的刻度相关但不同。下表中的"正常百分比"列提供
//             了从 usWidthClass 值 1 – 9 到 'wdth' 值的映射。
//                  值  说明                            C 定义                  正常百分比
//                  1   超紧缩（Ultra-condensed）       FWIDTH_ULTRA_CONDENSED  50
//                  2   特紧缩（Extra-condensed）       FWIDTH_EXTRA_CONDENSED  62.5
//                  3   紧缩（Condensed）               FWIDTH_CONDENSED        75
//                  4   半紧缩（Semi-condensed）        FWIDTH_SEMI_CONDENSED   87.5
//                  5   中等（正常）（Medium / normal） FWIDTH_NORMAL           100
//                  6   半扩展（Semi-expanded）         FWIDTH_SEMI_EXPANDED    112.5
//                  7   扩展（Expanded）                FWIDTH_EXPANDED         125
//                  8   特扩展（Extra-expanded）        FWIDTH_EXTRA_EXPANDED   150
//                  9   超扩展（Ultra-expanded）        FWIDTH_ULTRA_EXPANDED   200
//
// fsType
//      格式： uint16
//      标题： 类型标志
//      说明： 指示字体的字体嵌入（font embedding）许可权限。标志的解释如下：
//                  位      掩码        说明
//                  0 – 3   0x000F      使用权限。有效字体最多只能设置位 1、2 或 3 中的一个；位 0 永久保留且必须为零。此
//                                      子字段的有效值为 0、2、4 或 8。这些值的含义如下：
//                              0       可安装嵌入。字体可以嵌入，并可以永久安装在远程系统上，或供其他用户使用。远程系统
//                                      的用户获得与该字体原始购买者相同的权利、义务和许可，并受与原始购买者相同的最终用
//                                      户许可协议、版权、设计专利和/或商标的约束。
//                              2       受限许可嵌入。未经法律所有者的明确许可，不得以任何方式修改、嵌入或交换字体。
//                              4       预览和打印嵌入。字体可以嵌入，并可以临时加载到其他系统上以用于查看或打印文档。包
//                                      含预览和打印字体的文档必须以"只读"方式打开；不得对文档进行编辑。
//                              8       可编辑嵌入。字体可以嵌入，并可以临时加载到其他系统上。与预览和打印嵌入一样，包含
//                                      可编辑字体的文档可以打开以供阅读。此外，允许编辑，包括使用嵌入字体格式化新文本的
//                                      能力，并且可以保存更改。
//                  4 – 7   0x00F0      保留，必须为零
//                  8       0x0100      禁止子集化（no subsetting）。设置此位时，嵌入前不得对字体进行子集化。位 0 – 3 和
//                                      位 9 指定的其他嵌入限制也适用。
//                  9       0x0200      仅位图嵌入。设置此位时，只能嵌入字体中包含的位图。不得嵌入任何轮廓数据。如果字体
//                                      中没有可用的位图，则该字体被视为不可嵌入，嵌入服务将失败。位 0-3 和位 8 指定的其
//                                      他嵌入限制也适用。
//                  10 – 15 0xFC00      保留，必须为零
//      备注： 可嵌入字体可以存储在文档中。当在打开包含嵌入字体的文档时，如果系统未安装该
//             字体（远程系统），嵌入字体可以由支持嵌入的应用程序临时（在某些情况下永久）
//             加载到该系统上。嵌入许可权限由字体的供应商授予。
//             实现字体嵌入支持的应用程序不得嵌入未获许可允许嵌入的字体。此外，将字体嵌入文
//             档时，应用程序不得修改此字段中指示的嵌入权限和限制。此外，为临时使用加载嵌入
//             字体（预览和打印或可编辑嵌入）的应用程序必须在包含嵌入字体的文档关闭时删除字
//             体。
//             位 0 到 3（嵌入权限子字段）是互斥的：字体不应设置其中多个位。注意，如果设置了
//             两个或更多位，某些应用程序可能采用指示的限制最少的权限。有关更多讨论，请参阅版
//             本差异。注意：字体供应商有责任正确设置这些位以获得所需的应用程序行为。要使受限
//             许可嵌入生效，嵌入权限子字段的值必须为 2（即，仅设置了位 1）。
//             注意，Apple 的 TrueType 参考手册将 fsType 字段的位 1（且仅位 1）指定为具有分配
//             的语义。这源自 OS/2 表的预发布草案规范。然而，OS/2 表版本 0 的最终规范定义了位
//             0 到 3。此外，某些早期字体实现错误地使用了值 1（设置了位 0），导致了互操作性问
//             题。因此，位 0 在版本 0 的最终规范中被指定为保留。位 0 永久保留，其使用已弃用。
//      版本差异： 版本 0 到 1：仅分配了位 0 到 3。应用程序在读取版本 0 或版本 1 表时必须忽略
//             位 4 到 15。版本 0 到 2：版本 0 到 2 的规范未指定位 0 到 3 必须互斥。相反，这些
//             规范指出，如果在给定字体中设置了位 0 到 3 中的多个位，则采用指示的限制最少的权
//             限。特别是，某些使用版本 0 到版本 2 OS/2 表的字体同时设置了位 2 和位 3，意图指
//             示预览/打印和编辑权限。允许应用程序对具有版本 0 到版本 2 OS/2 表的字体使用此行
//             为。版本 3 及更高版本：版本 3 的规范（在 OpenType 1.4 中添加）引入了位 0 到 3
//             必须互斥的明确要求。
//
// ySubscriptXSize
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 下标水平字体大小
//      说明： 此字体下标的建议水平大小（以字体设计单位表示）。应大于 0。
//      备注： 如果字体有两个建议的下标大小，例如数字和其他字符，应强调数字大小。此大小字段映
//             射到用于下标的字体的 em 大小。水平字体大小指定字体设计师推荐的与此字体关联的下
//             标字形的水平大小。如果字体未包含应用程序所需的所有下标字形，并且应用程序可以通
//             过缩放字体字形或从其他字体替换字形来替代，则此字段指定这些下标字形的建议标称宽
//             度。例如，如果字体的 em 为 2048 单位，ySubScriptXSize 设置为 205，则模拟下标字
//             形的水平大小将是正常字形的 1/10。注意，这在 OS/2 平台中实现为有符号值，尽管只
//             应预期大于 0 的值。
//
// ySubscriptYSize
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 下标垂直字体大小
//      说明： 此字体下标的建议垂直大小（以字体设计单位表示）。应大于 0。
//      备注： 如果字体有两个建议的下标大小，例如数字和其他字符，应强调数字大小。此大小字段映射
//             到用于下标的字体的 em 大小。垂直字体大小指定字体设计师推荐的与此字体关联的下标字
//             形的垂直大小。如果字体未包含应用程序所需的所有下标字形，并且应用程序可以通过缩放
//             字体中的字形或从其他字体替换字形来替代，则此字段指定这些下标字形的建议标称高度。
//             例如，如果字体的 em 为 2048 单位，ySubScriptYSize 设置为 205，则模拟下标字形的垂
//             直大小将是正常字形的 1/10。注意，这在 OS/2 平台中实现为有符号值，尽管只应预期大于
//             0 的值。
//
// ySubscriptXOffset
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 下标 x 偏移
//      说明： 此字体下标的建议水平偏移（以字体设计单位表示）。
//      备注： ySubscriptXOffset 字段指定字体设计师推荐的与此字体关联的下标字形的水平偏移，从字
//             形原点到下标字形的原点。如果字体未包含应用程序所需的所有下标字形，并且应用程序可
//             以替换字形，则此字段指定第一个下标字形之前的最后一个字形的字形的字宽点（glyph
//             escapement point）到的建议水平位置。对于正立字形（upright glyphs），此值通常为零；
//             但是，如果字体的字形具有倾斜（斜体或倾斜），下标字形的参考点通常会调整以补偿倾斜
//             角度。
//
// ySubscriptYOffset
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 下标 y 偏移
//      说明： 此字体下标相对于基线的建议垂直偏移（以字体设计单位表示）。
//      备注： ySubscriptYOffset 字段指定字体设计师推荐的与此字体关联的下标字形从字形基线到字形
//             基线的垂直偏移。值表示为字形基线下方的正偏移。如果字体未包含应用程序所需的所有下
//             标字形，则此字段指定应用程序提供的替代下标字形在字形基线下方的建议垂直距离。
//
// ySuperscriptXSize
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 上标水平字体大小
//      说明： 此字体上标的建议水平大小（以字体设计单位表示）。应大于 0。
//      备注： 如果字体有两个建议的上标大小，例如数字和其他字符，应强调数字大小。此大小字段映射
//             到用于上标的字体的 em 大小。水平字体大小指定字体设计师推荐的与此字体关联的上标字
//             形的水平大小。如果字体未包含应用程序所需的所有上标字形，并且应用程序可以通过缩放
//             字体字形或从其他字体替换字形来替代，则此字段指定这些上标字形的建议标称宽度。例如，
//             如果字体的 em 为 2048 单位，ySuperScriptXSize 设置为 205，则模拟上标字形的水平大
//             小将是正常字形的 1/10。注意，这在 OS/2 平台中实现为有符号值，尽管只应预期大于 0
//             的值。
//
// ySuperscriptYSize
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 上标垂直字体大小
//      说明： 此字体上标的建议垂直大小（以字体设计单位表示）。应大于 0。
//      备注： 如果字体有两个建议的上标大小，例如数字和其他字符，应强调数字大小。此大小字段映射
//             到用于上标的字体的 em 大小。垂直字体大小指定字体设计师推荐的与此字体关联的上标字
//             形的垂直大小。如果字体未包含应用程序所需的所有上标字形，并且应用程序可以通过缩放
//             字体中的字形或从其他字体替换字形来替代，则此字段指定这些上标字形的建议标称高度。
//             例如，如果字体的 em 为 2048 单位，ySuperScriptYSize 设置为 205，则模拟上标字形的
//             垂直大小将是正常字形的 1/10。注意，这在 OS/2 平台中实现为有符号值，尽管只应预期大
//             于 0 的值。
//
// ySuperscriptXOffset
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 上标 x 偏移
//      说明： 此字体上标的建议水平偏移（以字体设计单位表示）。
//      备注： ySuperscriptXOffset 字段指定字体设计师推荐的与此字体关联的上标字形从字形原点到上
//             标字形原点的水平偏移。如果字体未包含应用程序所需的所有上标字形，则此字段指定从第
//             一个上标字形之前的字形的字宽点的建议水平位置。对于正立字形，此值通常为零；但是，
//             如果字体的字形具有倾斜（斜体或倾斜），上标字形的参考点通常会调整以补偿倾斜角度。
//
// ySuperscriptYOffset
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 上标 y 偏移
//      说明： 此字体上标相对于基线的建议垂直偏移（以字体设计单位表示）。
//      备注： ySuperscriptYOffset 字段指定字体设计师推荐的与此字体关联的上标字形从字形基线到上
//             标字形基线的垂直偏移。此字段的值表示为字形基线上方的正偏移。如果字体未包含应用程
//             序所需的所有上标字形，则此字段指定应用程序提供的替代上标字形在字形基线上方的建议
//             垂直距离。
//
// yStrikeoutSize
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 删除线大小
//      说明： 删除线笔画的厚度（以字体设计单位表示）。应大于 0。
//      备注： 此字段通常应为当前字体的 em 破折号的厚度，并且也应与 'post' 表中指定的下划线粗细
//             相匹配。
//      注意：这在 OS/2 平台中实现为有符号值，尽管只应预期大于 0 的值。
//
// yStrikeoutPosition
//      格式： FWORD
//      单位： 字体设计单位
//      标题： 删除线位置。
//      说明： 删除线笔画顶部相对于基线的位置（以字体设计单位表示）。
//      备注： 正值表示基线上方的距离；负值表示基线下方的距离。建议将删除线位置与 em 破折号（em
//             dash）对齐。但是，删除线位置不应干扰标准字符的识别，因此不应与字体中的横杠（crossbar）
//             对齐。
//
// sFamilyClass
//      格式： int16
//      标题： 字体族类别和子类
//      说明： 此字段提供字体族设计的分类
//      备注： 族类别和子类根据族设计分配给字体。此字段旨在用于在请求的字体不可用时选择替代字
//             体族。类别和子类提供两级分类。此字段的高字节包含族类别，低字节包含族子类。子类
//             值的解释取决于类别值。注册的类别和子类值最初由 IBM 定义。有关注册值的详细信息，
//             请参阅 IBM 字体族分类。https://learn.microsoft.com/en-us/typography/opentype/spec/ibmfc
//
// panose
//      格式： uint8[10]
//      标题： PANOSE 分类号
//      国际化： 需要额外的规范才能使 PANOSE 对非拉丁字符集进行分类
//      说明： 此 10 字节数字数组用于描述给定字体的视觉特征。然后使用这些特征将字体与其他外观
//             相似但名称不同的字体相关联。每个数字的变量如下所列。
//      备注： PANOSE 定义包含十个字节，每个字节可以有多个可能的值。PANOSE 值在 PANOSE 分类度
//             量指南中有完整描述。注意，第一个字节用于高级分类"族类（Family Kind）"，其余字节
//             的解释取决于第一个字节的值。例如，如果族类值为 2（拉丁文本），则下一个字节指定
//             "衬线样式（Serif Style）"；但如果族类值为 3（拉丁手写），则下一个字节指定"工具
//             类型（Tool Kind）"。某些应用程序可能仅支持某些族类值。下表给出了族类为拉丁文本时
//             panose 数组的解释。https://monotype.github.io/panose/pan1.htm
//                  类型    名称
//                  uint8   bFamilyType
//                  uint8   bSerifStyle
//                  uint8   bWeight
//                  uint8   bProportion
//                  uint8   bContrast
//                  uint8   bStrokeVariation
//                  uint8   bArmStyle
//                  uint8   bLetterform
//                  uint8   bMidline
//                  uint8   bXHeight
//             某些应用程序可以使用 PANOSE 值进行字体选择，以选择匹配某些参数的字体。例如，比例
//             （对于族类 = 拉丁文本）可能用于确定字体是否为等宽字体；或者衬线样式可能用于确定字
//             体是否属于通用衬线或无衬线类别。某些应用程序将使用族类 = 5（拉丁符号）来识别符号
//             字体，这可能会影响字体选择或回退行为。对应用程序应如何使用 PANOSE 值没有要求。
//             在使用 OpenType 字体变体机制的可变字体中，无法为字体支持的不同实例表示不同的 PANOSE
//             值。PANOSE 值可以基于默认实例设置。
//      版本差异： 本规范的早期版本提供了有关 PANOSE 值的更多详细信息。但是，上述引用的外部规范
//             是规范性来源，应参考以获取此类详细信息。
//
// ulUnicodeRange1（位 0 – 31）
// ulUnicodeRange2（位 32 – 63）
// ulUnicodeRange3（位 64 – 95）
// ulUnicodeRange4（位 96 – 127）
//      格式： uint32[4]（共 128 位）
//      标题： Unicode 字符范围
//      说明： 此字段用于指定字体文件中 'cmap' 子表，平台 3 编码 ID 1（Microsoft 平台，Unicode
//             BMP）和平台 3 编码 ID 10（Microsoft 平台，Unicode 完整字符集）所涵盖的 Unicode
//             块或范围。如果设置了某位（1），则认为该位分配的 Unicode 范围是功能性的。如果该
//             位清零（0），则认为该范围不是功能性的。每个位都被视为独立的标志，位可以以任何组
//             合设置。"功能性"的确定由字体设计师决定，但字符集选择应尽可能按范围实现功能性。
//             所有保留字段必须为零。每个 uint32 为大端序形式。
//                  位      Unicode 范围                            块范围          备注
//                  0       基本拉丁文                              0000-007F
//                  1       拉丁文-1 补充                           0080-00FF
//                  2       拉丁文扩展-A                            0100-017F
//                  3       拉丁文扩展-B                            0180-024F
//                  4       国际音标扩展                            0250-02AF
//                          语音学（phonetic）扩展                  1D00-1D7F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                          语音学扩展补充                          1D80-1DBF       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  5       间距修饰符号（spacing modifier）        02B0-02FF
//                          修饰音调字母（modifier tone）           A700-A71F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  6       组合附加符号（combining diacritical）   0300-036F
//                          组合附加符号补充                        1DC0-1DFF       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  7       希腊文和科普特文                        0370-03FF
//                  8       科普特文（coptic）                      2C80-2CFF       在 OpenType 1.5 中为 OS/2 版本 4 添加，有关其他版本差异，请参见下文
//                  9       西里尔文（cyrillic）                    0400-04FF
//                          西里尔文补充                            0500-052F       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          西里尔文扩展-A                          2DE0-2DFF       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                          西里尔文扩展-B                          A640-A69F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  10      亚美尼亚文                              0530-058F
//                  11      希伯来文                                0590-05FF
//                  12      瓦伊文（vai）                           A500-A63F       在 OpenType 1.5 中为 OS/2 版本 4 添加，有关其他版本差异，请参见下文
//                  13      阿拉伯文                                0600-06FF
//                          阿拉伯文补充                            0750-077F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  14      西非书面文字（nko）                     07C0-07FF       在 OpenType 1.5 中为 OS/2 版本 4 添加，有关其他版本差异，请参见下文
//                  15      天城文（devanagari）                    0900-097F
//                  16      孟加拉文（bangla）                      0980-09FF
//                  17      古尔穆基文（gurmukhi）                  0A00-0A7F
//                  18      古吉拉特文（gujarati）                  0A80-0AFF
//                  19      奥里亚文（odia）                        0B00-0B7F
//                  20      泰米尔文（tamil）                       0B80-0BFF
//                  21      泰卢固文（telugu）                      0C00-0C7F
//                  22      卡纳达文（kannada）                     0C80-0CFF
//                  23      马拉雅拉姆文（malayalam）               0D00-0D7F
//                  24      泰文                                    0E00-0E7F
//                  25      老挝文                                  0E80-0EFF
//                  26      格鲁吉亚文（georgian）                  10A0-10FF
//                          格鲁吉亚文补充                          2D00-2D2F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  27      巴厘文（balinese）                      1B00-1B7F       在 OpenType 1.5 中为 OS/2 版本 4 添加，有关其他版本差异，请参见下文
//                  28      谚文（hangul jamo）                     1100-11FF
//                  29      拉丁文扩展附加                          1E00-1EFF
//                          拉丁文扩展-C                            2C60-2C7F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                          拉丁文扩展-D                            A720-A7FF       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  30      希腊文扩展                              1F00-1FFF
//                  31      通用标点符号                            2000-206F
//                          补充标点符号                            2E00-2E7F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  32      上标和下标                              2070-209F
//                  33      货币符号                                20A0-20CF
//                  34      符号用组合附加符号                      20D0-20FF
//                  35      类字母符号                              2100-214F
//                  36      数字形式                                2150-218F
//                  37      箭头                                    2190-21FF
//                          补充箭头-A                              27F0-27FF       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          补充箭头-B                              2900-297F       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          杂项符号和箭头                          2B00-2BFF       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  38      数学运算符                              2200-22FF
//                          补充数学运算符                          2A00-2AFF       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          杂项数学符号-A                          27C0-27EF       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          杂项数学符号-B                          2980-29FF       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                  39      杂项技术符号                            2300-23FF
//                  40      控制图片                                2400-243F
//                  41      光学字符识别                            2440-245F
//                  42      带圈字母数字                            2460-24FF
//                  43      制表符                                  2500-257F
//                  44      块元素                                  2580-259F
//                  45      几何图形                                25A0-25FF
//                  46      杂项符号                                2600-26FF
//                  47      杂项符号和箭头                          2700-27BF
//                  48      CJK 符号和标点                          3000-303F
//                  49      平假名                                  3040-309F
//                  50      片假名                                  30A0-30FF
//                          片假名语音扩展                          31F0-31FF       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                  51      注音符号（bopomofo）                    3100-312F
//                          注音符号扩展                            31A0-31BF       在 OpenType 1.3 中添加，扩展 OS/2 版本 2
//                  52      谚文兼容                                3130-318F
//                  53      八思巴文（phags-pa）                    A840-A87F       在 OpenType 1.5 中为 OS/2 版本 4 添加，有关其他版本差异，请参见下文
//                  54      带圈 CJK 字母和月份                     3200-32FF
//                  55      CJK 兼容                                3300-33FF
//                  56      谚文音节                                AC00-D7AF
//                  57      非零平面                                10000-10FFFF    设置此位意味着此字体至少支持基本多文种平面之外的一个字符，首次在 OpenType 1.3 中为 OS/2 版本 2 分配
//                  58      腓尼基文（phoenician）                  10900-1091F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  59      CJK 统一表意文字                        4E00-9FFF
//                          CJK 部首补充（radical）                 2E80-2EFF       在 OpenType 1.3 中为 OS/2 版本 2 添加
//                          康熙部首                                2F00-2FDF       在 OpenType 1.3 中为 OS/2 版本 2 添加
//                          表意文字描述字符                        2FF0-2FFF       在 OpenType 1.3 中为 OS/2 版本 2 添加
//                          CJK 统一表意文字扩展 A                  3400-4DBF       在 OpenType 1.3 中为 OS/2 版本 2 添加
//                          CJK 统一表意文字扩展 B                  20000-2A6DF     在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          宽文，汉文标注号（kanbun）              3190-319F       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                  60      专用区（平面 0）                        E000-F8FF
//                  61      CJK 笔画（strokes）                     31C0-31EF       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                          CJK 兼容表意文字                        F900-FAFF
//                          CJK 兼容表意文字补充                    2F800-2FA1F     在 OpenType 1.4 中为 OS/2 版本 3 添加
//                  62      字母呈现形式                            FB00-FB4F
//                  63      阿拉伯文呈现形式-A                      FB50-FDFF
//                  64      组合半符号（combining half mark）       FE20-FE2F
//                  65      竖排形式（vertical forms）              FE10-FE1F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                          CJK 兼容形式                            FE30-FE4F
//                  66      小写变体形式                            FE50-FE6F
//                  67      阿拉伯文呈现形式-B                      FE70-FEFF
//                  68      半角和全角形式                          FF00-FFEF
//                  69      特殊符号                                FFF0-FFFF
//                  70      藏文（tibetan）                         0F00-0FFF       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  71      叙利亚文（syriac）                      0700-074F       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  72      塔纳文（thaana）                        0780-07BF       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  73      僧伽罗文（sinhala）                     0D80-0DFF       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  74      缅甸文（myanmar）                       1000-109F       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  75      埃塞俄比亚文（ethiopic）                1200-137F       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                          埃塞俄比亚文补充                        1380-139F       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                          埃塞俄比亚文扩展                        2D80-2DDF       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  76      切罗基文（cherokee）                    13A0-13FF       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  77      加拿大原住民音节文字                    1400-167F       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  78      欧甘文（ogham）                         1680-169F       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  79      如尼文（runic）                         16A0-16FF       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  80      高棉文（khmer）                         1780-17FF       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                          高棉符号                                19E0-19FF       在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  81      蒙古文                                  1800-18AF       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  82      盲文图案（braille patterns）            2800-28FF       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                  83      彝文音节（yi syllables）                A000-A48F       首次在 OpenType 1.3 中分配，扩展 OS/2 版本 2
//                          彝文部首                                A490-A4CF       在 OpenType 1.3 中添加，扩展 OS/2 版本 2
//                  84      他加禄文（tagalog）                     1700-171F       首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                          哈努诺文（hanunoo）                     1720-173F       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          布希德文（buhid）                       1740-175F       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          塔格班瓦文（tagbanwa）                  1760-177F       在 OpenType 1.4 中为 OS/2 版本 3 添加
//                  85      古意大利文                              10300-1032F     首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                  86      哥特文（gothic）                        10330-1034F     首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                  87      德塞雷特文（deseret）                   10400-1044F     首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                  88      拜占庭音乐符号（byzantine）             1D000-1D0FF     首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                          音乐符号（musical symbols）             1D100-1D1FF     在 OpenType 1.4 中为 OS/2 版本 3 添加
//                          古希腊音乐符号                          1D200-1D24F     在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  89      数学字母数字符号                        1D400-1D7FF     首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                  90      专用区（平面 15）                       F0000-FFFFD     首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                          专用区（平面 16）                       100000-10FFFD   在 OpenType 1.4 中为 OS/2 版本 3 添加
//                  91      变体选择符                              FE00-FE0F       首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                          变体选择符补充                          E0100-E01EF     在 OpenType 1.4 中为 OS/2 版本 3 添加
//                  92      标签（tags）                            E0000-E007F     首次在 OpenType 1.4 中为 OS/2 版本 3 分配
//                  93      林布文（limbu）                         1900-194F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  94      傣仂文（tai le）                        1950-197F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  95      新傣仂文                                1980-19DF       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  96      布吉斯文（buginese）                    1A00-1A1F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  97      格拉哥里文（glagolitic）                2C00-2C5F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  98      提非纳文（tifinagh）                    2D30-2D7F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  99      易经六十四卦符号                        4DC0-4DFF       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  100     锡尔赫特文（syloti nagri）              A800-A82F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  101     线形文字 B 音节                         10000-1007F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                          线形文字 B 表意文字                     10080-100FF     在 OpenType 1.5 中为 OS/2 版本 4 添加
//                          爱琴海数字（aegean numbers）            10100-1013F     在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  102     古希腊数字                              10140-1018F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  103     乌加里特文（ugaritic）                  10380-1039F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  104     古波斯文                                103A0-103DF     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  105     萧伯纳文（shavian）                     10450-1047F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  106     奥斯曼亚文（osmanya）                   10480-104AF     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  107     塞浦路斯音节文字（cypriot syllabary）   10800-1083F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  108     佉卢文（kharoshthi）                    10A00-10A5F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  109     太玄经符号                              1D300-1D35F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  110     楔形文字（cuneiform）                   12000-123FF     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                          楔形文字数字和标点                      12400-1247F     在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  111     算筹数字（counting rod numbers）        1D360-1D37F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  112     巽他文（sundanese）                     1B80-1BBF       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  113     雷布查文（lepcha）                      1C00-1C4F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  114     桑塔利文（ol chiki）                    1C50-1C7F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  115     索拉什特拉文（saurashtra）              A880-A8DF       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  116     克耶李文（kavah li）                    A900-A92F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  117     拉让文（rejang）                        A930-A95F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  118     占文（cham）                            AA00-AA5F       首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  119     古代符号                                10190-101CF     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  120     斐斯托斯圆盘（phaistos disc）           101D0-101FF     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  121     卡里亚文（carian）                      102A0-102DF     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                          吕基亚文（lycian）                      10280-1029F     在 OpenType 1.5 中为 OS/2 版本 4 添加
//                          吕底亚文（lydian）                      10920-1093F     在 OpenType 1.5 中为 OS/2 版本 4 添加
//                  122     多米诺骨牌（domino tiles）              1F030-1F09F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                          麻将牌（mahjong tiles）                 1F000-1F02F     首次在 OpenType 1.5 中为 OS/2 版本 4 分配
//                  123-127 保留
//      备注： 截至 Unicode 5.1，所有可用位已用尽。位分配最后更新于 OpenType 1.5 中的 OS/2 版本
//             4。当前版本的 Unicode 支持许多未在 OS/2 表这些字段中支持的其他范围。请参阅 'meta'
//             表中的 'dlng' 和 'slng' 标签，以获取声明字体可以支持或为之设计的文字或语言的替代
//             机制。
//      版本差异： 创建不同版本的 OS/2 表时，对应的 Unicode 版本不同，给定版本的初始规范定义的位
//             分配少于后续版本。某些应用程序可能不支持具有较早 OS/2 版本的字体中的所有分配。上述
//             所有位分配对 OS/2 表的任何版本均有效，但 OS/2 版本 1 和 2 的规范包含一些分配，这些
//             分配未对应于定义明确的 Unicode 范围，且与后续分配冲突，详见下文。如果字体具有设置
//             了这些位之一的版本 1 或版本 2 OS/2 表，则可能是预期的过时解释。但是，由于这些分配
//             不对应于定义明确的范围，因此隐含字符覆盖范围不明确。
//             版本 0： 首次指定版本 0 时，未定义位分配。某些应用程序可能忽略版本 0 OS/2 表中的
//             这些字段。
//             版本 1： 版本 1 首次与 Unicode 1.1 同时指定，仅为位 0 到 69 定义了位分配。对于具有
//             版本 1 表的字体，某些应用程序可能仅识别位 0 到 69。此外，版本 1 的规范包含一些未对
//             应于定义明确的 Unicode 范围的位分配，这些分配自版本 2 起已停用。
//                  位 8："希腊符号和科普特文"（位 7 指定为"基本希腊文"）
//                  位 12："希伯来文扩展"（位 11 指定为"基本希伯来文"）
//                  位 14："阿拉伯文扩展"（位 13 指定为"基本阿拉伯文"）
//                  位 27："格鲁吉亚文扩展"（位 26 指定为"基本格鲁吉亚文"）
//             此外，版本 1 和 2 将位 53 定义为"CJK 杂项"，这也不对应于任何定义明确的 Unicode 范
//             围。此分配自版本 3 起已停用。
//             版本 2： 版本 2 在 OpenType 1.1 中定义，与 Unicode 2.1 同时。当时，仅为位 0 到 69
//             定义了位分配。版本 2 的位分配在 OpenType 1.3 中更新，添加了对应于 Unicode 2.0 和
//             Unicode 3.0 中新分配的位 70 到 83。对于具有版本 2 表的字体，某些应用程序可能仅识别
//             OpenType 1.2 或 OpenType 1.3 中分配的位。此外，版本 2 的规范继续使用位 53 的问题分
//             配，请参阅版本 1 的详细信息。此分配自版本 3 起已停用。
//             版本 3： 版本 3 在 OpenType 1.4 中定义，为对应于 Unicode 3.2 中附加范围的位 84 到
//             92 分配了位。此外，某些已分配的位已扩展以覆盖相关字符的附加 Unicode 范围；详见上表。
//             版本 4： 版本 4 在 OpenType 1.5 中定义，为对应于 Unicode 5.1 中附加范围的位 58 和
//             位 93 到 122 分配了位。此外，位 8、12、14、27 和 53 被重新分配（请参阅版本 1 了解
//             以前的分配）。此外，某些已分配的位已扩展以覆盖相关字符的附加 Unicode 范围；详见上
//             表。
//
// achVendID
//      格式： Tag
//      标题： 字体供应商标识
//      说明： 给定字体供应商的四字符标识符
//      备注： 这不是原始艺术作品的版权所有者。这是负责所分类字体的营销和分销的公司。例如，可以有
//             多个 ITC Zapf Dingbats 的供应商，某些供应商在其字体中提供差异化的优势（更多的字距
//             对、非规范化数据、手工提示等）。此标识符将允许使用正确的供应商字体，而不是另一个可
//             能较差的字体文件。
//             Microsoft 维护供应商 ID 注册表。注册的 ID 必须是唯一的，对应于单个供应商。也可以使
//             用未注册的 ID，但不鼓励：强烈建议供应商注册 ID，以确保不同供应商在使用给定 ID 时不
//             会发生冲突，并且客户能够找到给定字体的供应商联系信息。此字段也可以留空（设为空，或
//             四个空格字符组成的标签）。
//             所有供应商 ID 使用 Tag 数据类型，它等同于由有限 ASCII 字符集组成的四字符字符串。有
//             关 Tag 数据类型的详细信息，请参阅数据类型。按照惯例，只有注册的标签应仅由大写字母
//            （或空格）组成。
//             有关已注册供应商 ID 的列表，或有关注册供应商 ID 或更新供应商信息的详细信息，请参阅
//             已注册的字体供应商。https://www.microsoft.com/typography/links/vendorlist.aspx
//
// fsSelection
//      格式： uint16
//      标题： 字体选择标志
//      说明： 包含有关字体图案性质的信息，如下所示：
//                  位 #    macStyle 位     C 定义              说明
//                  0       位 1            ITALIC              字体包含斜体或倾斜字形（italic or oblique），否则为正立（upright）
//                  1                       UNDERSCORE          字形带下划线
//                  2                       NEGATIVE            字形的前景和背景反转
//                  3                       OUTLINED            轮廓（空心）字形，否则为实心
//                  4                       STRIKEOUT           字形带删除线
//                  5       位 0            BOLD                字形加粗
//                  6                       REGULAR             字形处于字体的标准字重/样式
//                  7                       USE_TYPO_METRICS    如果设置，强烈建议应用程序使用 OS/2.sTypoAscender - OS/2.sTypoDescender + OS/2.sTypoLineGap 作为此字体的默认行距
//                  8                       WWS                 字体的 'name' 表字符串与字重/宽度/斜率族一致，无需使用名称 ID 21 和 22。详见下文更详细的描述。
//                  9                       OBLIQUE             字体包含倾斜字形
//                  10–15                   Reserved            保留，设为 0
//      备注： 所有未定义的位必须为零。fsSelection 字段最初是为与 OS/2 平台兼容而创建的。
//             某些样式描述位（如位 0）被现代应用程序使用，而其他样式描述位则不是。
//             位 0： 位 0 的设置必须与 'head' 表的 macStyle 字段中位 1 的设置匹配。
//             位 1 – 4： 位 1 – 4 是很少使用的位，指示字体主要是装饰性或特殊用途字体。
//             位 5： 位 5 的设置必须与 'head' 表的 macStyle 字段中位 0 的设置匹配。
//             位 6： 位 6 在现代应用程序中未广泛使用。如果设置了位 6，则位 0 和位 5 必须清零，否则
//                    行为未定义。注意，如果位 0 和位 5 都清零，这并不表示位 6 是否会清零。例如，
//                    Arial Light 不是 Arial 的常规样式，所有位都将清零。在扩展字体族中，位 6 不需要
//                    为使用名称 ID 2 为"Regular"的非常规样式字体设置。有关更多信息，请参阅名称 ID。
//             位 7： 位 7 在版本 4 中定义，并在许多现代应用程序中使用。对于新字体，鼓励供应商使用版
//                    本 4 或更高版本的 OS/2 表，并设置位 7。
//                    如果字体是使用较早版本的 OS/2 表创建的，并更新到当前版本的 OS/2 表，则设置位 7
//                    可能会对使用这些字体的现有文档造成重新排版的风险。为最小化此类风险，仅当使用
//                    OS/2.usWin* 度量值作为行高会比使用 OS/2.sTypo* 值产生明显较差的结果时，才应设
//                    置该位。
//             位 8： 如果设置了位 8，则提供的 'name' 表族和子族字符串与字重/宽度/斜率（WWS）族模型
//                    一致，无需使用名称 ID 21 或 22。位 8 和名称 ID 21 和 22 的引入是为了供假设 WWS
//                    族模型的应用程序使用，因为存在不适合三个属性（字重、宽度或斜率）之一的样式变体
//                    字体。
//                    许多字体族包含仅在这些三个属性之一或多个方面不同的字形。在这种情况下，名称 ID
//                    1 和 2 或 ID 16 和 17 不会对假设 WWS 模型的软件造成挑战。但某些字体族包含在其
//                    他属性方面不同的字形。例如，字体族可能包含"手写"、"说明"、"展示"、"光学大小"等
//                    变体。在这种情况下，字体可以将该属性纳入字体排印子族名称（ID 17 字符串），而假
//                    设 WWS 模型的应用程序会要求将该属性纳入族名称（ID 1 或 ID 16）。
//                    如果字形在其族内仅在字重、宽度或斜率属性方面不同，则应设置位 8。这甚至适用于属
//                    于包含由其他属性区分的其他字形的大型族的字形。设置位 8 作为向假设 WWS 模型的应
//                    用程序指示名称 ID 1 和 2 或 ID 16 和 17 与 WWS 模型一致，无需进一步处理（包括
//                    检查名称 ID 21 和 22 是否存在）的标志。因此，当设置了位 8 时，永远不需要名称
//                    ID 21 和 22，也不应包含。
//                    如果字形在其族内与常规字形的差异在于字重、宽度或斜率之外的属性，则不应设置位
//                    8，并且应包含名称 ID 21 和 22。通过不设置位 8，作为向假设 WWS 模型的应用程序指
//                    示它们应查找名称 ID 21 和 22，或可能需要对名称进行其他处理。
//                    STAT 表在 OpenType 的后续版本中引入，在位 8 和名称 ID 21 和 22 定义之后。STAT
//                    表的引入部分是为了解决相同类型的需求，但以一种更通用的方式，可能对假设其他族模
//                    型的应用程序有用。如果字形具有字重、宽度或斜率之外的属性，并且没有名称 ID 21
//                    和 22，某些假设 WWS 模型的应用程序可能能够使用 STAT 表中的信息来支持该字体。但
//                    是，即使存在 STAT 表（可变字体需要），仍应遵循上述关于位 8 和名称 ID 21 和 22
//                    的建议。
//                    在此上下文中，"字体排印族"是名称 ID 16 的 Microsoft Unicode 字符串（如果存在），
//                    否则是名称 ID 1 的 Microsoft Unicode 字符串；"字重"是 OS/2.usWeightClass；"宽
//                    度"是 OS/2.usWidthClass；"斜率"是 OS/2.fsSelection 位 0（ITALIC）和位 9
//                    （OBLIQUE）。
//             位 9： 如果设置了位 9，则此字体应被区分斜体和倾斜样式的进程视为"倾斜"样式，例如级联样
//                    式表字体匹配。例如，通过算法倾斜正立面创建的字体会设置此位。
//                    如果字体具有版本 4 或更高版本的 OS/2 表，并且未设置此位，则此字体不应被视为"倾
//                    斜"样式。例如，具有经典斜体设计的字体不会设置此位。
//                    此位与 ITALIC 位（位 0）不同，后者与假设由常规、斜体、粗体和粗斜体组成的四成员
//                    字体族模型的应用程序中的样式链接无关。它可以独立于 ITALIC 位设置或取消设置。在
//                    大多数情况下，如果设置了 OBLIQUE，则 ITALIC 也会设置，但这不是必需的。
//             位 15： 位 15 永久保留。它已在某些旧实现中使用，并可能在某些实现中导致特殊行为。此位
//                    的使用已弃用。
//      版本差异：
//             版本 0 到 3：仅分配了位 0（斜体）到位 6（常规）。位 7 到 15 保留且必须设为 0。应用程
//             序应忽略具有版本 0 到版本 3 OS/2 表的字体中的位 7 到 15。
//             版本 4 到 5：位 7 到 9 在版本 4（OpenType 1.5）中定义。位 10 到 15 保留且必须设为 0。
//             应用程序应忽略具有版本 4 或版本 5 OS/2 表的字体中的位 10 到 15。
//
// usFirstCharIndex
//      格式： uint16
//      说明： 此字体中的最小 Unicode 索引（字符代码），根据平台 ID 3 和平台特定编码 ID 0
//             或 1 的 'cmap' 子表。对于支持 Win-ANSI 或其他字符集的大多数字体，此值为
//             0x0020。此字段不能表示补充字符值（大于 0xFFFF 的码点）。支持补充字符的字体
//             应将此字段的值设为 0xFFFF（如果最小索引值是补充字符）。
//
// usLastCharIndex
//      格式： uint16
//      说明： 此字体中的最大 Unicode 索引（字符代码），根据平台 ID 3 和编码 ID 0 或 1 的
//             'cmap' 子表。此值取决于字体支持的字符集。此字段不能表示补充字符值（大于 0xFFFF
//             的码点）。支持补充字符的字体应将此字段的值设为 0xFFFF。
//
// sTypoAscender
//      格式： FWORD
//      单位： 字体设计单位
//      说明： 此字体的字体排印升部。此字段应与 sTypoDescender 和 sTypoLineGap 值结合使用，
//             以确定默认行距。此字段类似于 'hhea' 表中的 ascender 字段以及本表中的 usWinAscent
//             字段。但是，旧平台实现以平台特定的方式使用这些字段。因此，这些字段受向后兼容
//             性要求的约束，无法确保跨实现的一致布局。sTypoAscender、sTypoDescender 和
//             sTypoLineGap 字段旨在允许应用程序以字体排印正确且可移植的方式布局文档。
//             fsSelection 字段的 USE_TYPO_METRICS 标志（位 7）用于选择使用 sTypo* 值或 usWin*
//             值作为默认行度量。有关其他详细信息，请参阅 fsSelection。
//             差值 sTypoAscender - sTypoDescender 等于 unitsPerEm 不是通用要求。这些值应
//             设置为适合字体设计支持的主要语言的默认行距。
//             对于打算用于垂直（以及水平）布局的 CJK（中文、日文和韩文）字体，sTypoAscender
//             的所需值是描述表意文字 em 框顶部的值。例如，如果字体的表意文字 em 框从坐标
//             0,-120 延伸到 1000,880（即，一个 1000 × 1000 的框，设置在拉丁基线下方 120 个
//             设计单位），则 sTypoAscender 的值必须设为 880。不遵守这些要求将导致垂直布局不
//             正确。有关此字段的更多信息，另请参阅建议部分。
//             https://learn.microsoft.com/en-us/typography/opentype/spec/baselinetags#ideoembox
//             https://learn.microsoft.com/en-us/typography/opentype/spec/recom#tad
//
// sTypoDescender
//      格式： FWORD
//      单位： 字体设计单位
//      说明： 此字体的字体排印降部。此字段应与 sTypoAscender 和 sTypoLineGap 值结合使用，
//             以确定默认行距。此字段类似于 'hhea' 表中的 descender 字段以及本表中的 usWinDescent
//             字段。但是，旧平台实现以平台特定的方式使用这些字段。因此，这些字段受向后兼
//             容性要求的约束，无法确保跨实现的一致布局。sTypoAscender、sTypoDescender 和
//             sTypoLineGap 字段旨在允许应用程序以字体排印正确且可移植的方式布局文档。
//             fsSelection 字段的 USE_TYPO_METRICS 标志（位 7）用于选择使用 sTypo* 值或 usWin*
//             值作为默认行度量。有关其他详细信息，请参阅 fsSelection。
//             差值 sTypoAscender - sTypoDescender 等于 unitsPerEm 不是通用要求。这些值应
//             设置为适合字体设计支持的主要语言的默认行距。
//             对于打算用于垂直（以及水平）布局的 CJK（中文、日文和韩文）字体，sTypoDescender
//             的所需值是描述表意文字 em 框底部的值。例如，如果字体的表意文字 em 框从坐标
//             0,-120 延伸到 1000,880（即，一个 1000 × 1000 的框，设置在拉丁基线下方 120
//             个设计单位），则 sTypoDescender 的值必须设为 -120。不遵守这些要求将导致垂直
//             布局不正确。有关此字段的更多信息，另请参阅建议部分。
//
// sTypoLineGap
//      格式： FWORD
//      单位： 字体设计单位
//      说明： 此字体的字体排印行距。此字段应与 sTypoAscender 和 sTypoDescender 值结合使用，
//             以确定默认行距。此字段类似于 'hhea' 表中的 lineGap 字段。但是，旧平台实现以
//             平台特定的方式处理该字段。因此，该字段受向后兼容性要求的约束，无法确保跨实现
//             的一致布局。sTypoAscender、sTypoDescender 和 sTypoLineGap 字段旨在允许应用程
//             序以字体排印正确且可移植的方式布局文档。
//             fsSelection 字段的 USE_TYPO_METRICS 标志（位 7）用于选择使用 sTypo* 值或 usWin*
//             值作为默认行度量。有关其他详细信息，请参阅 fsSelection。
//
// usWinAscent
//      格式： UFWORD
//      单位： 字体设计单位
//      说明： "Windows 升部"度量。应用于指定裁剪区域（clipping region）基线上方的高度。这
//             类似于 sTypoAscender 字段，也类似于 'hhea' 表中的 ascender 字段。但是，它们
//             之间存在重要差异。
//             在 Windows GDI 实现中，usWinAscent 和 usWinDescent 值已用于确定 TrueType 光栅
//             化器中位图表面（bitmap surface）的大小。Windows GDI 将裁剪任何出现在 usWinAscent
//             值上方的 TrueType 字形轮廓部分。如果任何裁剪不可接受，则该值应设为大于或等于
//             yMax。
//             注意：这涉及字形的默认位置，而不是在应用 GPOS 或 'kern' 表中的数据后布局中的最
//             终位置。此外，此裁剪行为与 VDMX 表交互：如果存在 VDMX 表，并且有当前设备纵横比
//             和光栅化大小的数据，则 VDMX 数据将取代 usWinAscent 和 usWinDescent 值。
//             某些旧应用程序使用 usWinAscent 和 usWinDescent 值来确定默认行距。强烈建议不要这
//             样做。应为此目的使用 sTypo* 字段。
//             注意，某些应用程序根据 fsSelection 字段的 USE_TYPO_METRICS 标志（位 7）是否设置，
//             使用 usWin* 值或 sTypo* 值来确定默认行距。这对于在使用旧字体的旧文档提供兼容性
//             的同时，也使用新字体提供更好的、更可移植的布局可能很有用。有关其他详细信息，请
//             参阅 fsSelection。
//             使用 sTypo* 字段作为默认行距的应用程序可以使用 usWin* 值来确定裁剪区域的大小。
//             某些应用程序在编辑场景中使用裁剪区域来确定在编辑文本时重新绘制显示表面的哪一部
//             分，或在选择文本时绘制多大的选择矩形。这是 usWin* 值的适当用途。
//             本规范的早期版本建议将 usWinAscent 值计算为 Windows "ANSI"字符集中所有字符的
//             yMax。对于新字体，该值应基于字体设计支持的主要语言确定，并应考虑容纳高字形或
//             标记定位可能需要的额外高度。
//
// usWinDescent
//      格式： UFWORD
//      单位： 字体设计单位
//      说明： "Windows 降部"度量。应用于指定裁剪区域基线下方的垂直范围。这类似于 sTypoDescender
//             字段，也类似于 'hhea' 表中的 descender 字段。但是，它们之间存在重要差异。其中一些
//             差异描述如下。此外，usWinDescent 值将基线下方的距离视为正值；因此，usWinDescent
//             通常为正值，而 sTypoDescender 和 hhea.descender 通常为负值。
//             在 Windows GDI 实现中，usWinDescent 和 usWinAscent 值已用于确定 TrueType 光栅化器
//             中位图表面的大小。Windows GDI 将裁剪任何出现在 (-1 × usWinDescent) 下方的 TrueType
//             字形轮廓部分。如果任何裁剪不可接受，则该值应设为大于或等于 (-yMin)。
//             注意：这涉及字形的默认位置，而不是在应用 GPOS 或 'kern' 表中的数据后布局中的最终
//             位置。此外，此裁剪行为与 VDMX 表交互：如果存在 VDMX 表，并且有当前设备纵横比和光栅
//             化大小的数据，则 VDMX 数据将取代 usWinAscent 和 usWinDescent 值。
//             某些旧应用程序使用 usWinAscent 和 usWinDescent 值来确定默认行距。强烈建议不要这样
//             做。应为此目的使用 sTypo* 字段。
//             注意，某些应用程序根据 fsSelection 字段的 USE_TYPO_METRICS 标志（位 7）是否设置，
//             使用 usWin* 值或 sTypo* 值来确定默认行距。这对于在使用旧字体的旧文档提供兼容性的
//             同时，也使用新字体提供更好的、更可移植的布局可能很有用。有关其他详细信息，请参阅
//             fsSelection。
//             使用 sTypo* 字段作为默认行距的应用程序可以使用 usWin* 值来确定裁剪区域的大小。某
//             些应用程序在编辑场景中使用裁剪区域来确定在编辑文本时重新绘制显示表面的哪一部分，
//             或在选择文本时绘制多大的选择矩形。这是 usWin* 值的适当用途。
//             本规范的早期版本建议将 usWinDescent 值计算为 Windows"ANSI"字符集中所有字符的 -yMin。
//             对于新字体，该值应基于字体设计支持的主要语言确定，并应考虑容纳低降部字形或标记定位
//             可能需要的额外垂直范围。
//
// ulCodePageRange1（位 0 – 31）
// ulCodePageRange2（位 32 – 63）
//      格式： uint32[2]（共 64 位）
//      标题： 代码页字符范围
//      说明： 此字段用于指定字体文件中 'cmap' 子表平台 3，编码 ID 1（Windows 平台，Unicode
//             BMP）所涵盖的代码页。如果字体文件是编码 ID 0，则应设置符号字符集位（Symbol
//             Character Set bit）。
//             如果设置了给定位（1），则认为该代码页是功能性的。如果该位清零（0），则认为该
//             代码页不是功能性的。每个位都被视为独立的标志，位可以以任何组合设置。"功能性"
//             的确定由字体设计师决定，但字符集选择应尽可能按代码页实现功能性。
//             符号字符集具有特殊含义。如果设置了符号位（31），并且字体文件包含平台为 3 且编
//             码 ID 为 1 的 'cmap' 子表，则 Unicode 范围 0xF000 - 0xF0FF（含）中的所有字符
//             将用于枚举符号字符集。如果未设置该位，则该范围中的任何字符都不会被枚举为符号
//             字符集。所有保留字段必须为零。每个 uint32 为大端序形式。
//                  位      代码页  说明
//                  0       1252    拉丁文 1
//                  1       1250    拉丁文 2：东欧
//                  2       1251    西里尔文
//                  3       1253    希腊文
//                  4       1254    土耳其文
//                  5       1255    希伯来文
//                  6       1256    阿拉伯文
//                  7       1257    Windows 波罗的海文
//                  8       1258    越南文
//                  9 – 15          保留供替代 ANSI 使用
//                  16      874     泰文
//                  17      932     JIS/日本
//                  18      936     中文：简体字—中国和新加坡
//                  19      949     韩文 Wansung
//                  20      950     中文：繁体字—台湾和香港特别行政区
//                  21      1361    韩文 Johab
//                  22 – 28         保留供替代 ANSI 或 OEM 使用
//                  29              Macintosh 字符集（美国罗马体）
//                  30              OEM 字符集
//                  31              符号字符集
//                  32 – 47         保留供 OEM 使用
//                  48      869     IBM 希腊文
//                  49      866     MS-DOS 俄文
//                  50      865     MS-DOS 北欧文
//                  51      864     阿拉伯文
//                  52      863     MS-DOS 加拿大法文
//                  53      862     希伯来文
//                  54      861     MS-DOS 冰岛文
//                  55      860     MS-DOS 葡萄牙文
//                  56      857     IBM 土耳其文
//                  57      855     IBM 西里尔文；主要为俄文
//                  58      852     拉丁文 2
//                  59      775     MS-DOS 波罗的海文
//                  60      737     希腊文；原 437 G
//                  61      708     阿拉伯文；ASMO 708
//                  62      850     西欧/拉丁文 1
//                  63      437     美国
//      版本差异：
//              版本 0： 这些字段在版本 0 中未定义。如果版本 0 OS/2 表的大小延伸到 usWinDescent
//                       字段之外，则应忽略 usWinDescent 字段之外的附加数据。
//              版本 1： 位 8 在版本 1 中未分配。所有其他当前分配的位在版本 1 中定义。
//              版本 2 及更高版本： 所有当前分配的位在版本 2 中定义。
//      代码页标识符 https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
//              下表定义了可用的代码页标识符。注意，ANSI 代码页在不同计算机上可能不同，或者
//              可以在单台计算机上更改，从而导致数据损坏。为了获得最一致的结果，应用程序应使
//              用 Unicode（如 UTF-8 或 UTF-16），而不是特定的代码页。
//                  标识符      .NET 名称           附加信息
//                  037         IBM037              IBM EBCDIC 美国-加拿大
//                  437         IBM437              OEM 美国
//                  500         IBM500              IBM EBCDIC 国际
//                  708         ASMO-708            阿拉伯语 (ASMO 708)
//                  709                             阿拉伯语 (ASMO-449+, BCON V4)
//                  710                             阿拉伯语 - 透明阿拉伯语
//                  720         DOS-720             阿拉伯语 (透明 ASMO)；阿拉伯语 (DOS)
//                  737         ibm737              OEM 希腊语 (原 437G)；希腊语 (DOS)
//                  775         ibm775              OEM 波罗的海语；波罗的海语 (DOS)
//                  850         ibm850              OEM 多语言拉丁语 1；西欧 (DOS)
//                  852         ibm852              OEM 拉丁语 2；中欧 (DOS)
//                  855         IBM855              OEM 西里尔语 (主要为俄语)
//                  857         ibm857              OEM 土耳其语；土耳其语 (DOS)
//                  858         IBM00858            OEM 多语言拉丁语 1 + 欧元符号
//                  860         IBM860              OEM 葡萄牙语；葡萄牙语 (DOS)
//                  861         ibm861              OEM 冰岛语；冰岛语 (DOS)
//                  862         DOS-862             OEM 希伯来语；希伯来语 (DOS)
//                  863         IBM863              OEM 加拿大法语；加拿大法语 (DOS)
//                  864         IBM864              OEM 阿拉伯语；阿拉伯语 (864)
//                  865         IBM865              OEM 北欧语；北欧语 (DOS)
//                  866         cp866               OEM 俄语；西里尔语 (DOS)
//                  869         ibm869              OEM 现代希腊语；希腊语，现代 (DOS)
//                  870         IBM870              IBM EBCDIC 多语言/ROECE (拉丁语 2)；IBM EBCDIC 多语言拉丁语 2
//                  875         cp875               IBM EBCDIC 现代希腊语
//                  932         shift_jis           ANSI/OEM 日语；日语 (Shift-JIS)
//                  936         gb2312              ANSI/OEM 简体中文 (中国、新加坡)；简体中文 (GB2312)
//                  949         ks_c_5601-1987      ANSI/OEM 韩语 (统一韩文代码)
//                  950         big5                ANSI/OEM 繁体中文 (台湾；中国香港特别行政区)；繁体中文 (Big5)
//                  1026        IBM1026             IBM EBCDIC 土耳其语 (拉丁语 5)
//                  1047        IBM01047            IBM EBCDIC 拉丁语 1/开放系统
//                  1140        IBM01140            IBM EBCDIC 美国-加拿大 (037 + 欧元符号)；IBM EBCDIC (美国-加拿大-欧元)
//                  1141        IBM01141            IBM EBCDIC 德国 (20273 + 欧元符号)；IBM EBCDIC (德国-欧元)
//                  1142        IBM01142            IBM EBCDIC 丹麦-挪威 (20277 + 欧元符号)；IBM EBCDIC (丹麦-挪威-欧元)
//                  1143        IBM01143            IBM EBCDIC 芬兰-瑞典 (20278 + 欧元符号)；IBM EBCDIC (芬兰-瑞典-欧元)
//                  1144        IBM01144            IBM EBCDIC 意大利 (20280 + 欧元符号)；IBM EBCDIC (意大利-欧元)
//                  1145        IBM01145            IBM EBCDIC 拉丁美洲-西班牙 (20284 + 欧元符号)；IBM EBCDIC (西班牙-欧元)
//                  1146        IBM01146            IBM EBCDIC 英国 (20285 + 欧元符号)；IBM EBCDIC (英国-欧元)
//                  1147        IBM01147            IBM EBCDIC 法国 (20297 + 欧元符号)；IBM EBCDIC (法国-欧元)
//                  1148        IBM01148            IBM EBCDIC 国际 (500 + 欧元符号)；IBM EBCDIC (国际-欧元)
//                  1149        IBM01149            IBM EBCDIC 冰岛语 (20871 + 欧元符号)；IBM EBCDIC (冰岛语-欧元)
//                  1200        utf-16              Unicode UTF-16，小端字节序 (ISO 10646 的 BMP)；仅适用于托管应用程序
//                  1201        unicodeFFFE         Unicode UTF-16，大端字节序；仅适用于托管应用程序
//                  1250        windows-1250        ANSI 中欧；中欧 (Windows)
//                  1251        windows-1251        ANSI 西里尔语；西里尔语 (Windows)
//                  1252        windows-1252        ANSI 拉丁语 1；西欧 (Windows)
//                  1253        windows-1253        ANSI 希腊语；希腊语 (Windows)
//                  1254        windows-1254        ANSI 土耳其语；土耳其语 (Windows)
//                  1255        windows-1255        ANSI 希伯来语；希伯来语 (Windows)
//                  1256        windows-1256        ANSI 阿拉伯语；阿拉伯语 (Windows)
//                  1257        windows-1257        ANSI 波罗的海语；波罗的海语 (Windows)
//                  1258        windows-1258        ANSI/OEM 越南语；越南语 (Windows)
//                  1361        Johab               韩语 (Johab)
//                  10000       macintosh           MAC 罗马体；西欧 (Mac)
//                  10001       x-mac-japanese      日语 (Mac)
//                  10002       x-mac-chinesetrad   MAC 繁体中文 (Big5)；繁体中文 (Mac)
//                  10003       x-mac-korean        韩语 (Mac)
//                  10004       x-mac-arabic        阿拉伯语 (Mac)
//                  10005       x-mac-hebrew        希伯来语 (Mac)
//                  10006       x-mac-greek         希腊语 (Mac)
//                  10007       x-mac-cyrillic      西里尔语 (Mac)
//                  10008       x-mac-chinesesimp   MAC 简体中文 (GB 2312)；简体中文 (Mac)
//                  10010       x-mac-romanian      罗马尼亚语 (Mac)
//                  10017       x-mac-ukrainian     乌克兰语 (Mac)
//                  10021       x-mac-thai          泰语 (Mac)
//                  10029       x-mac-ce            MAC 拉丁语 2；中欧 (Mac)
//                  10079       x-mac-icelandic     冰岛语 (Mac)
//                  10081       x-mac-turkish       土耳其语 (Mac)
//                  10082       x-mac-croatian      克罗地亚语 (Mac)
//                  12000       utf-32              Unicode UTF-32，小端字节序；仅适用于托管应用程序
//                  12001       utf-32BE            Unicode UTF-32，大端字节序；仅适用于托管应用程序
//                  20000       x-Chinese_CNS       CNS 台湾；繁体中文 (CNS)
//                  20001       x-cp20001           TCA 台湾
//                  20002       x_Chinese-Eten      Eten 台湾；繁体中文 (Eten)
//                  20003       x-cp20003           IBM5550 台湾
//                  20004       x-cp20004           TeleText 台湾
//                  20005       x-cp20005           Wang 台湾
//                  20105       x-IA5               IA5 (IRV 国际字母表第 5 号，7 位)；西欧 (IA5)
//                  20106       x-IA5-German        IA5 德语 (7 位)
//                  20107       x-IA5-Swedish       IA5 瑞典语 (7 位)
//                  20108       x-IA5-Norwegian     IA5 挪威语 (7 位)
//                  20127       us-ascii            US-ASCII (7 位)
//                  20261       x-cp20261           T.61
//                  20269       x-cp20269           ISO 6937 非间距重音符号
//                  20273       IBM273              IBM EBCDIC 德国
//                  20277       IBM277              IBM EBCDIC 丹麦-挪威
//                  20278       IBM278              IBM EBCDIC 芬兰-瑞典
//                  20280       IBM280              IBM EBCDIC 意大利
//                  20284       IBM284              IBM EBCDIC 拉丁美洲-西班牙
//                  20285       IBM285              IBM EBCDIC 英国
//                  20290       IBM290              IBM EBCDIC 日语片假名扩展
//                  20297       IBM297              IBM EBCDIC 法国
//                  20420       IBM420              IBM EBCDIC 阿拉伯语
//                  20423       IBM423              IBM EBCDIC 希腊语
//                  20424       IBM424              IBM EBCDIC 希伯来语
//                  20833       x-EBCDIC-KoreanExtended IBM EBCDIC 韩语扩展
//                  20838       IBM-Thai            IBM EBCDIC 泰语
//                  20866       koi8-r              俄语 (KOI8-R)；西里尔语 (KOI8-R)
//                  20871       IBM871              IBM EBCDIC 冰岛语
//                  20880       IBM880              IBM EBCDIC 西里尔语俄语
//                  20905       IBM905              IBM EBCDIC 土耳其语
//                  20924       IBM00924            IBM EBCDIC 拉丁语 1/开放系统 (1047 + 欧元符号)
//                  20932       EUC-JP              日语 (JIS 0208-1990 和 0212-1990)
//                  20936       x-cp20936           简体中文 (GB2312)；简体中文 (GB2312-80)
//                  20949       x-cp20949           韩语 Wansung
//                  21025       cp1025              IBM EBCDIC 西里尔语塞尔维亚-保加利亚语
//                  21027                           (已弃用)
//                  21866       koi8-u              乌克兰语 (KOI8-U)；西里尔语 (KOI8-U)
//                  28591       iso-8859-1          ISO 8859-1 拉丁语 1；西欧 (ISO)
//                  28592       iso-8859-2          ISO 8859-2 中欧；中欧 (ISO)
//                  28593       iso-8859-3          ISO 8859-3 拉丁语 3
//                  28594       iso-8859-4          ISO 8859-4 波罗的海语
//                  28595       iso-8859-5          ISO 8859-5 西里尔语
//                  28596       iso-8859-6          ISO 8859-6 阿拉伯语
//                  28597       iso-8859-7          ISO 8859-7 希腊语
//                  28598       iso-8859-8          ISO 8859-8 希伯来语；希伯来语 (ISO-Visual)
//                  28599       iso-8859-9          ISO 8859-9 土耳其语
//                  28603       iso-8859-13         ISO 8859-13 爱沙尼亚语
//                  28605       iso-8859-15         ISO 8859-15 拉丁语 9
//                  29001       x-Europa            Europa 3
//                  38598       iso-8859-8-i        ISO 8859-8 希伯来语；希伯来语 (ISO-Logical)
//                  50220       iso-2022-jp         ISO 2022 日语，无半角片假名；日语 (JIS)
//                  50221       csISO2022JP         ISO 2022 日语，含半角片假名；日语 (JIS-允许 1 字节假名)
//                  50222       iso-2022-jp         ISO 2022 日语 JIS X 0201-1989；日语 (JIS-允许 1 字节假名 - SO/SI)
//                  50225       iso-2022-kr         ISO 2022 韩语
//                  50227       x-cp50227           ISO 2022 简体中文；简体中文 (ISO 2022)
//                  50229                           ISO 2022 繁体中文
//                  50930                           EBCDIC 日语 (片假名) 扩展
//                  50931                           EBCDIC 美国-加拿大和日语
//                  50933                           EBCDIC 韩语扩展和韩语
//                  50935                           EBCDIC 简体中文扩展和简体中文
//                  50936                           EBCDIC 简体中文
//                  50937                           EBCDIC 美国-加拿大和繁体中文
//                  50939                           EBCDIC 日语 (拉丁语) 扩展和日语
//                  51932       euc-jp              EUC 日语
//                  51936       EUC-CN              EUC 简体中文；简体中文 (EUC)
//                  51949       euc-kr              EUC 韩语
//                  51950                           EUC 繁体中文
//                  52936       hz-gb-2312          HZ-GB2312 简体中文；简体中文 (HZ)
//                  54936       GB18030             Windows XP 及更高版本：GB18030 简体中文 (4 字节)；简体中文 (GB18030)
//                  57002       x-iscii-de          ISCII 天城文
//                  57003       x-iscii-be          ISCII 孟加拉语
//                  57004       x-iscii-ta          ISCII 泰米尔语
//                  57005       x-iscii-te          ISCII 泰卢固语
//                  57006       x-iscii-as          ISCII 阿萨姆语
//                  57007       x-iscii-or          ISCII 奥里亚语
//                  57008       x-iscii-ka          ISCII 卡纳达语
//                  57009       x-iscii-ma          ISCII 马拉雅拉姆语
//                  57010       x-iscii-gu          ISCII 古吉拉特语
//                  57011       x-iscii-pa          ISCII 旁遮普语
//                  65000       utf-7               Unicode (UTF-7)
//                  65001       utf-8               Unicode (UTF-8)
//
// sxHeight
//      格式： FWORD
//      单位： 字体设计单位
//      说明： 此度量指定基线与非上升小写字母的近似顶部之间的距离（以字体设计单位表示）。
//             此值通常由字体设计师指定，但在不可能的情况下，例如转换旧字体时，可以将值
//             设为编码在 U+0078（拉丁小写字母 X）位置的未缩放和未提示字形边界框的顶部。
//             如果此位置未编码字形，则该字段应设为 0。
//             如果指定了此度量，可用于字体替换：一个字体的 xHeight 值可以缩放以近似另
//             一个字体的表观大小。
//      版本差异：
//              版本 0、版本 1： 此字段在版本 0 或版本 1 中未定义。如果版本 0 OS/2 表的大
//              小延伸到 usWinDescent 字段之外，或者版本 1 OS/2 表的大小延伸到代码页范围
//              字段之外，则应忽略此附加数据。
//              版本 2 及更高版本： 此字段在 OS/2 表的版本 2 中定义。
//
// sCapHeight
//      格式： FWORD
//      单位： 字体设计单位
//      说明： 此度量指定基线与大写字母的近似高度之间的距离（以字体设计单位表示）。此值通
//             常由字体设计师指定，但在不可能的情况下，例如转换旧字体时，可以将值设为编码
//             在 U+0048（拉丁大写字母 H）位置的未缩放和未提示字形边界框的顶部。如果此位置
//             未编码字形，则该字段应设为 0。
//             如果指定了此度量，可用于以毫米为单位指定大写高度的系统中指定字号。它也可以
//             用作对齐度量；例如，首字下沉（a drop capital）的顶部可以与第一行文本的 sCapHeight
//             度量对齐。
//      版本差异：
//              版本 0、版本 1： 此字段在版本 0 或版本 1 中未定义。如果版本 0 OS/2 表的大小
//              延伸到 usWinDescent 字段之外，或者版本 1 OS/2 表的大小延伸到代码页范围字段之
//              外，则应忽略此附加数据。
//              版本 2 及更高版本： 此字段在 OS/2 表的版本 2 中定义。
//
// usDefaultChar
//      格式： uint16
//      说明： 这是 Unicode 码点，以 UTF-16 编码，可用于在字体不支持请求的字符时用作默认字形。
//             如果此字段的值为零，则字形 ID 0 将用作默认字符。此字段不能表示补充平面字符值
//             （大于 0xFFFF 的码点），因此强烈建议应用程序不要使用此字段。
//      版本差异：
//              版本 0、版本 1： 此字段在版本 0 或版本 1 中未定义。如果版本 0 OS/2 表的大小延
//              伸到 usWinDescent 字段之外，或者版本 1 OS/2 表的大小延伸到代码页范围字段之外，
//              则应忽略此附加数据。
//              版本 2 及更高版本： 此字段在 OS/2 表的版本 2 中定义。
//
// usBreakChar
//      格式： uint16
//      说明： 这是 Unicode 码点，以 UTF-16 编码，可用作默认间断字符。换行字符用于分隔单词和
//             使文本对齐。大多数字体将 U+0020 SPACE 指定为间断字符。此字段不能表示补充平面字
//             符值（大于 0xFFFF 的码点），因此强烈建议应用程序不要使用此字段。
//      版本差异：
//              版本 0、版本 1： 此字段在版本 0 或版本 1 中未定义。如果版本 0 OS/2 表的大小延
//              伸到 usWinDescent 字段之外，或者版本 1 OS/2 表的大小延伸到代码页范围字段之外，
//              则应忽略此附加数据。
//              版本 2 及更高版本： 此字段在 OS/2 表的版本 2 中定义。
//
// usMaxContext
//      格式： uint16
//      说明： 此字体中任何功能的目标字形上下文的最大长度。例如，仅具有字距调整功能的字体应将
//             此字段设为 2。如果字体还具有连字功能，其中字形序列"f f i"被连字"ffi"替换，则此
//             字段应设为 3。此字段可能对复杂的换行引擎有用，帮助它们确定应向前查看多远以测试
//             是否可能影响换行的变化。对于链式上下文查找（chaining contextual lookups），应考
//             虑覆盖的字形（covered glyph）+ 输入序列（input sequence）+ 前瞻序列（lookahead
//             sequence）的字符串长度。
//      版本差异：
//              版本 0、版本 1： 此字段在版本 0 或版本 1 中未定义。如果版本 0 OS/2 表的大小延
//              伸到 usWinDescent 字段之外，或者版本 1 OS/2 表的大小延伸到代码页范围字段之外，
//              则应忽略此附加数据。
//              版本 2 及更高版本： 此字段在 OS/2 表的版本 2 中定义。
//
// usLowerOpticalPointSize
//      格式： uint16
//      单位： TWIPs，TWIP 的全称是 twentieth of a point（二十分之一磅，或二十分之一点）。在字
//             体排版中，1 TWIP = 1/20 pt（点），即 1 英寸 = 1440 TWIPs（因为 1 英寸 = 72 pt，
//             72 × 20 = 1440）。
//      说明： 此字段用于具有多种光学样式的字体。此值是为此字体设计的大小范围的下限值。此字段
//             的单位是 TWIPs（二十分之一磅，或每英寸 1440）。该值是包含性的，意味着该字体被设
//             计为在此字号到 usUpperOpticalPointSize 指示的字号（但不包括）范围内最佳工作。当
//             与字体排印族中也指定了 usLowerOpticalPointSize 和 usUpperOpticalPointSize 值的
//             其他光学大小变体字体一起使用时，预期另一个字体的 usUpperOpticalPointSize 字段将
//             设为此字段中的相同值，除非此字体是族中设计用于最低大小范围的字号。光学大小集中
//             最小的字体应将此值设为 0。在多个光学大小变体字体之间工作时，范围中不应有故意的
//             间隙或重叠。
//             usLowerOpticalPointSize 值必须小于 usUpperOpticalPointSize。最大有效值为 0xFFFE。
//             对于未设计为多种光学大小变体的字体，此字段应设为 0（零），usUpperOpticalPointSize
//             应设为 0xFFFF。注意：此字段的使用已被 STAT 表取代。有关更多信息，请参阅建议部分。
//             https://learn.microsoft.com/en-us/typography/opentype/spec/recom#OptSize
//      版本差异：
//              版本 0 – 4： 此字段在版本 0 – 4 中未定义。如果 OS/2 表的大小延伸到给定版本定义的
//              最后一个字段之外，则应忽略此附加数据。
//              版本 5： 此字段在 OS/2 表的版本 5 中定义。
//
// usUpperOpticalPointSize
//      格式： uint16
//      单位： TWIPs
//      说明： 此字段用于具有多种光学样式的字体。此值是为此字体设计的大小范围的上限值。此字段
//             的单位是 TWIPs（二十分之一磅，或每英寸 1440）。该值是排他性的，意味着该字体被设
//             计为在此字号以下到 usLowerOpticalPointSize 阈值范围内最佳工作。当与字体排印族中
//             也指定了 usLowerOpticalPointSize 和 usUpperOpticalPointSize 值的其他光学大小变
//             体字体一起使用时，预期另一个字体的 usLowerOpticalPointSize 字段将设为此字段中的
//             相同值，除非此字体是族中设计用于最高大小范围的字号。光学大小集中最大的字体应将
//             此值设为 0xFFFF，解释为无穷大。
//             在多个光学大小变体字体之间工作时，范围中不应有故意的间隙或重叠。
//             usUpperOpticalPointSize 值必须大于 usLowerOpticalPointSize。此字段的最小有效值
//             为 2。此字段表示的最大可能包含字号为 3276.65 磅；任何更高的值将表示为无穷大。
//             对于未设计为多种光学大小变体的字体，此字段应设为 0xFFFF，usLowerOpticalPointSize
//             应设为 0（零）。注意：此字段的使用已被 STAT 表取代。有关更多信息，请参阅建议部分。
//      版本差异：
//              版本 0 – 4： 此字段在版本 0 – 4 中未定义。如果 OS/2 表的大小延伸到给定版本定义
//              的最后一个字段之外，则应忽略此附加数据。
//              版本 5： 此字段在 OS/2 表的版本 5 中定义。
//
// OS/2 表与 OpenType 字体变体。在可变字体中，默认行度量应始终使用 sTypoAscender、sTypoDescender
// 和 sTypoLineGap 值设置，并且应在 fsSelection 字段中设置 USE_TYPO_METRICS 标志。'hhea' 表中的
// ascender、descender 和 lineGap 字段应设为与 sTypoAscender、sTypoDescender 和 sTypoLineGap 相
// 同的值。usWinAscent 和 usWinDescent 字段应用于指定推荐的裁剪矩形。
//
// 在可变字体中，OS/2 表中的各种字体度量值可能需要针对不同的变体实例进行调整。OS/2 条目的变体数
// 据可以在度量变体（MVAR）表中提供。不同的 OS/2 条目通过值标签与 MVAR 表中的特定变体数据相关联，
// 如下所示：
//      OS/2 条目               标签
//      sCapHeight              'cpht'
//      sTypoAscender           'hasc'
//      sTypoDescender          'hdsc'
//      sTypoLineGap            'hlgp'
//      sxHeight                'xhgt'
//      usWinAscent             'hcla'
//      usWinDescent            'hcld'
//      yStrikeoutPosition      'stro'
//      yStrikeoutSize          'strs'
//      ySubscriptXOffset       'sbxo'
//      ySubScriptXSize         'sbxs'
//      ySubscriptYOffset       'sbyo'
//      ySubscriptYSize         'sbys'
//      ySuperscriptXOffset     'spxo'
//      ySuperscriptXSize       'spxs'
//      ySuperscriptYOffset     'spyo'
//      ySuperscriptYSize       'spys'
//
// 注意：usWeightClass 和 usWidthClass 值不由变体数据调整，因为这些对应于可用于定义字体变
// 体空间的 'wght' 和 'wdth' 变体轴。变体实例的适当 usWeightClass 和 usWidthClass 值可以从
// 用于选择特定变体实例的 'wght' 和 'wdth' 用户坐标推导得出。对于大于 200 的 'wdth' 值，
// usWidthClass 值被钳位（clamped）到 9。有关这些 OS/2 字段与相应设计轴之间关系的详细信息，
// 请参阅 OpenType 设计变体轴标签注册表中的 'wght' 和 'wdth' 轴讨论。
// https://learn.microsoft.com/en-us/typography/opentype/spec/dvaraxisreg
//
// 注意：usLowerOpticalPointSize 和 usUpperOpticalPointSize 值不由变体数据调整。这些值（现
// 在已被 STAT 表取代）用于指示给定字体设计的大小范围。假设针对不同大小的变体将使用 'opsz'
// 变体轴实现。如果可变字体支持 'opsz' 作为变体轴，则 usLowerOpticalPointSize 和 usUpperOpticalPointSize
// 字段可以设为与 'fvar' 表中 'opsz' 轴的 minValue 和 maxValue 字段相同的值。
//
// 要在可变字体中具有可变行度量（variable line metrics），应在 MVAR 表中使用 'hasc'、'hdsc'
// 和 'hlgp' 值标签来使升部、降部和行距值从 sTypoAscender、sTypoDescender 和 sTypoLineGap
// 字段中指定的默认值变化。此外，可以使用 'hcla' 和 'hcld' 值标签来使裁剪区域的大小从
// winAscent 和 winDescent 字段中指定的默认值变化。可以使用上面列出的值标签来变化其他度量。
// 有关 OpenType 字体变体的一般信息，请参阅"OpenType 字体变体概述"章节。
//
// IBM 字体族分类（IBM Font Class Parameters）
//
// 本节定义可在字体的 OS/2 表 sFamilyClass 字段中使用的族类别和子类值。字体设计师或供应商
// 可以在字体中分配这些值，以提供字体族设计的分类。此处记录的注册值最初由 IBM 定义。这些值
// 将字体设计分类为其外观，但不标识特定的字体族、字形变体、设计师、供应商、大小或度量表差异。
// 应注意，某些字体设计可以同样好地归入多个类别或子类。此类设计应匹配到一个类别或子类，从同
// 一类别或子类中替换另一种字体设计通常会导致呈现的文档具有相似的外观。
//
//      类别 ID = 0 无分类。此类别 ID 用于指示相关字体没有设计分类，或者设计分类对字体资源
//      的创建者或用户不重要。
//
//      类别 ID = 1 旧式衬线体（Oldstyle Serifs）。此样式通常基于 15 至 17 世纪的拉丁印刷
//      风格，笔画强调具有温和的对角线对比（左上到右下较轻，右上到左下较重）和有括号的衬线。
//      此 IBM 类别反映了 ISO 衬线类别、旧式和易读性子类别，如 ISO/IEC 9541-1 修正案 1 标
//      准中所述。
//          子类 ID = 0：无分类（No Classification）
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者
//              或用户不重要。
//          子类 ID = 1：IBM 圆角易读体（IBM Rounded Legibility）
//              此样式通常以大 x 高度、短升部和降部为特征。具体来说，它以中等分辨率、手工调
//              整的位图呈现的更一般圆角易读子类别为区别。此 IBM 子类别未在 ISO/IEC 9541-1
//              修正案 1 标准中具体反映。
//          子类 ID = 2：加拉德体（Garalde）
//              此样式通常以中等 x 高度、高升部为特征。此字体样式的一个例子是 ITC Garamond
//              族。此 IBM 子类别反映了 ISO 衬线类别、旧式子类别和加拉德特定组，如 ISO/IEC
//              9541-1 修正案 1 标准中所述。
//          子类 ID = 3：威尼斯体（Venetian）
//              此样式通常以中等 x 高度、相对单调的外观和基于早期威尼斯印刷商设计的扫尾为特
//              征。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准中具体反映。
//          子类 ID = 4：改良威尼斯体（Modified Venetian）
//              此样式通常以大 x 高度、相对单调的外观和基于早期威尼斯印刷商设计的扫尾为特征。
//              此字体样式的一个例子是 Allied Linotype Palatino 族。此 IBM 子类别反映了 ISO
//              衬线类别、过渡子类别和改良特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 5：荷兰现代体（Dutch Modern）
//              此样式通常以大 x 高度、楔形衬线和碗的圆形外观为特征，类似于下面的荷兰传统子
//              类别，但笔画较轻。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准中具体反映。
//          子类 ID = 6：荷兰传统体（Dutch Traditional）
//              此样式通常以大 x 高度、楔形衬线和碗的圆形外观为特征。此字体样式的一个例子是
//              IBM Press Roman 族。此 IBM 子类别反映了 ISO 衬线类别和易读性子类别，如 ISO/IEC
//              9541-1 修正案 1 标准中所述。
//          子类 ID = 7：当代体（Contemporary）
//              此样式通常以小 x 高度、轻笔画和衬线为特征。此字体样式的一个例子是 University
//              族。此 IBM 子类别反映了 ISO 衬线类别和当代子类别，如 ISO/IEC 9541-1 修正案 1
//              标准中所述。
//          子类 ID = 8：书法体（Calligraphic）
//              此样式通常以书法的精细手写风格为特征，同时保留旧式外观的特征。此 IBM 子类别
//              未在 ISO/IEC 9541-1 修正案 1 标准中反映。
//          子类 ID = 9-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项（Miscellaneous）
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 2 过渡衬线体（Transitional Serifs）。此样式通常基于 18 至 19 世纪的拉丁印刷
//      风格，笔画强调具有明显的垂直对比（垂直笔画比水平笔画粗）和有括号的衬线。此 IBM 类别反
//      映了 ISO 衬线类别、过渡子类别，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者或
//              用户不重要。
//          子类 ID = 1：直线体（Direct Line）
//              此样式通常以中等 x 高度、精细衬线、明显的对比和宽度大致相同的大写字母为特征。
//              此字体样式的一个例子是 Monotype Baskerville 族。此 IBM 子类别反映了 ISO 衬线
//              类别、过渡子类别和直线特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 2：手写体（Script）
//              此样式通常以手写体外观为特征，同时保留过渡直线体风格。此字体样式的一个例子是
//              IBM Nasseem（阿拉伯文）族。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准中
//              具体反映，尽管 ISO 衬线类别、过渡子类别和直线特定组将是近似值。
//          子类 ID = 3-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 3 现代衬线体（Modern Serifs）。此样式通常基于 20 世纪的拉丁印刷风格，笔画
//      粗细部分之间的对比极端。此 IBM 类别反映了 ISO 衬线类别、现代子类别，如 ISO/IEC 9541-1
//      修正案 1 标准中所述。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者或
//              用户不重要。
//          子类 ID = 1：意大利体（Italian）
//              此样式通常以中等 x 高度、细发丝衬线为特征。此字体样式的一个例子是 Monotype
//              Bodoni 族。此 IBM 子类别反映了 ISO 衬线类别、现代子类别和意大利特定组，如
//              ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 2：手写体（Script）
//              此样式通常以手写体外观为特征，同时保留现代意大利体风格。此字体样式的一个例子
//              是 IBM Narkissim（希伯来文）族。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标
//              准中具体反映，尽管 ISO 衬线类别、现代子类别和意大利特定组将是近似值。
//          子类 ID = 3-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 4 克拉伦登衬线体（Clarendon Serifs）。此样式是旧式衬线体和过渡衬线体的变体，
//      具有温和的垂直笔画对比和有括号的衬线。此 IBM 类别反映了 ISO 衬线类别、方衬线子类别，
//      如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者或
//              用户不重要。
//          子类 ID = 1：克拉伦登体（Clarendon）
//              此样式通常以大 x 高度、衬线和笔画等重为特征。此字体样式的一个例子是 Allied
//              Linotype Clarendon 族。此 IBM 子类别反映了 ISO 衬线类别、方衬线子类别和克拉伦
//              登特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 2：现代体（Modern）
//              此样式通常以大 x 高度、衬线比笔画轻、笔画比传统体轻为特征。此字体样式的一个
//              例子是 Monotype Century Schoolbook 族。此 IBM 子类别反映了 ISO 衬线类别、方衬
//              线子类别和克拉伦登特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 3：传统体（Traditional）
//              此样式通常以大 x 高度、衬线比笔画轻为特征。此字体样式的一个例子是 Monotype
//              Century 族。此 IBM 子类别反映了 ISO 衬线类别、方衬线子类别和克拉伦登特定组，
//              如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 4：报纸体（Newspaper）
//              此样式通常以大 x 高度、设计风格更简单、衬线比笔画轻为特征。此字体样式的一个例
//              子是 Allied Linotype Excelsior 族。此 IBM 子类别反映了 ISO 衬线类别、方衬线子
//              类别和克拉伦登特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 5：短衬线体（Stub Serif）
//              此样式通常以大 x 高度、短粗衬线和相对粗的字干为特征。此字体样式的一个例子是
//              Cheltenham 族。此 IBM 子类别反映了 ISO 衬线类别、方衬线子类别和短特定组，如
//              ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 6：单调体（Monotone）
//              此样式通常以大 x 高度、单调字干为特征。此字体样式的一个例子是 ITC Korinna 族。
//              此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准中具体反映。
//          子类 ID = 7：打字机体（Typewriter）
//              此样式通常以大 x 高度、打字机特有的中等笔画粗细为特征。此字体样式的一个例子是
//              Prestige Elite 族。此 IBM 子类别反映了 ISO 衬线类别、方衬线子类别和打字机特定
//              组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 8-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 5 粗衬线体（Slab Serif）。此样式的特征是笔画和衬线之间的过渡为方形（无括号）。
//      此 IBM 类别反映了 ISO 衬线类别、方衬线子类别（克拉伦登特定组除外），如 ISO/IEC 9541-1
//      修正案 1 标准中所述。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者或
//              用户不重要。
//          子类 ID = 1：单调体（Monotone）
//              此样式通常以大 x 高度、衬线和笔画等重为特征。此字体样式的一个例子是 ITC Lubalin
//              族。此 IBM 子类别反映了 ISO 衬线类别、方衬线子类别和单调特定组，如 ISO/IEC 9541-1
//              修正案 1 标准中所述。
//          子类 ID = 2：人文主义体（Humanist）
//              此样式通常以中等 x 高度、衬线比笔画轻为特征。此字体样式的一个例子是 Candida 族。
//              此 IBM 子类别反映了 ISO 衬线类别、方衬线子类别和单调特定组，如 ISO/IEC 9541-1
//              修正案 1 标准中所述。
//          子类 ID = 3：几何体（Geometric）
//              此样式通常以大 x 高度、衬线和笔画等重以及几何（圆和线）设计为特征。此字体样式的
//              一个例子是 Monotype Rockwell 族。此 IBM 子类别反映了 ISO 衬线类别、方衬线子类别
//              和单调特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 4：瑞士体（Swiss）
//              此样式通常以大 x 高度、衬线和笔画等重以及强调字符的空白为特征。此字体样式的一个
//              例子是 Allied Linotype Serifa 族。此 IBM 子类别反映了 ISO 衬线类别、方衬线子类
//              别和单调特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 5：打字机体（Typewriter）
//              此样式通常以大 x 高度、衬线和笔画等重但中等粗细以及几何设计为特征。此字体样式的
//              一个例子是 IBM Courier 族。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准中具体
//              反映，尽管 ISO 衬线类别、方衬线子类别和单调特定组将是近似值。
//          子类 ID = 6-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 6 保留供将来使用（Reserved for Future Use）。此类别 ID 保留供将来分配，
//      未经 IBM 正式分配不得使用。
//
//      类别 ID = 7 自由形式衬线体（Freeform Serifs）。此样式包含衬线，但表达了一种设计自由度，
//      通常不适合其他衬线设计分类。此 IBM 类别反映了 ISO/IEC 9541-1 修正案 1 标准中记录的剩
//      余 ISO 衬线类别子类别。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者或用
//              户不重要。
//          子类 ID = 1：现代体（Modern）
//              此样式通常以中等 x 高度、笔画中的轻对比和圆润饱满的设计为特征。此字体样式的一个
//              例子是 ITC Souvenir 族。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准中反映。
//          子类 ID = 2-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 8 无衬线体（Sans Serif）。此样式包含大多数基本字母形式（不包括手写体和装
//      饰体），笔画上没有衬线。此 IBM 类别反映了 ISO/IEC 9541-1 修正案 1 标准中记录的 ISO
//      无衬线类别。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者或
//              用户不重要。
//          子类 ID = 1：IBM 新怪诞哥特体（IBM Neo-grotesque Gothic）
//              此样式通常以大 x 高度、均匀笔画宽度和简单的一层楼设计为特征，以中等分辨率、
//              手工调整的位图呈现的更一般新怪诞哥特子类别为区别。此字体样式的一个例子是
//              IBM Sonoran Sans Serif 族。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准
//              中具体反映。
//          子类 ID = 2：人文主义体（Humanist）
//              此样式通常以中等 x 高度、笔画中的轻对比和经典罗马字母形式为特征。此字体样式
//              的一个例子是 Allied Linotype Optima 族。此 IBM 子类别反映了 ISO 无衬线类别、
//              人文主义子类别，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 3：低 x 高度圆几何体（Low-x Round Geometric）
//              此样式通常以低 x 高度、单调笔画粗细和圆几何设计为特征。此字体样式的一个例子
//              是 Fundicion Tipograficia Neufville Futura 族。此 IBM 子类别反映了 ISO 无衬
//              线类别、几何子类别、圆特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 4：高 x 高度圆几何体（High-x Round Geometric）
//              此样式通常以高 x 高度、均匀笔画粗细和圆几何设计为特征。此字体样式的一个例子
//              是 ITC Avant Garde Gothic 族。此 IBM 子类别反映了 ISO 无衬线类别、几何子类别、
//              圆特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 5：新怪诞哥特体（Neo-grotesque Gothic）
//              此样式通常以高 x 高度、均匀笔画宽度和简单的一层楼设计为特征。此字体样式的一个
//              例子是 Allied Linotype Helvetica 族。此 IBM 子类别反映了 ISO 无衬线类别、哥特
//              子类别、新怪诞特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 6：改良新怪诞哥特体（Modified Neo-grotesque Gothic）
//              此样式类似于新怪诞哥特体风格，但对 G 和 Q 进行了设计变体。此字体样式的一个例
//              子是 Allied Linotype Univers 族。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标
//              准中具体反映，尽管 ISO 无衬线类别、哥特子类别、新怪诞特定组将是近似值。
//          子类 ID = 7-8：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 9：打字机哥特体（Typewriter Gothic）
//              此样式类似于新怪诞哥特体风格，具有打字机特有的中等笔画粗细。此字体样式的一个
//              例子是 IBM Letter Gothic 族。此 IBM 子类别反映了 ISO 无衬线类别、哥特子类别、
//              打字机特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 10：矩阵体（Matrix）
//              此样式通常是点阵打印机特有的简单设计。此字体样式的一个例子是 IBM Matrix Gothic
//              族。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准中反映。
//          子类 ID = 11-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 9 装饰体（Ornamentals）。此样式包含高度装饰或风格化的字符形状，通常用于标题。
//      此 IBM 类别反映了 ISO/IEC 9541-1 修正案 1 标准中记录的 ISO 装饰类别和 ISO 黑体类别。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者或
//              用户不重要。
//          子类 ID = 1：雕刻体（Engraver）
//              此样式以字干上的细线或雕刻线为特征。此字体样式的一个例子是 Copperplate 族。此
//              IBM 子类别反映了 ISO 装饰类别和内联子类别，或 ISO 衬线类别和雕刻子类别，如
//              ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 2：黑体（Black Letter）
//              此样式通常基于 12 至 15 世纪德国修道院和印刷商的印刷风格。此字体样式的一个例子
//              是 Old English 族。此 IBM 子类别反映了 ISO/IEC 9541-1 修正案 1 标准中记录的 ISO
//              黑体类别。
//          子类 ID = 3：装饰体（Decorative）
//              此样式以装饰性设计（通常来自自然，如叶子、花朵、动物等）融入字符的字干和笔画为
//              特征。此字体样式的一个例子是 Saphire 族。此 IBM 子类别反映了 ISO 装饰类别和装饰
//              子类别，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 4：三维体（Three Dimensional）
//              此样式以字符的三维（凸起）外观为特征，通过阴影或几何效果创建。此字体样式的一个
//              例子是 Thorne Shaded 族。此 IBM 子类别反映了 ISO 装饰类别和三维子类别，如 ISO/IEC
//              9541-1 修正案 1 标准中所述。
//          子类 ID = 5-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 10 手写体（Scripts）。此样式包含旨在模拟手写的字体。此 IBM 类别反映了 ISO/IEC
//      9541-1 修正案 1 标准中记录的 ISO 手写类别和安色尔体类别。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建者或
//              用户不重要。
//          子类 ID = 1：安色尔体（Uncial）
//              此样式以不连接（非连接）字符为特征，通常基于欧洲 6 至 9 世纪的手写风格。此字
//              体样式的一个例子是 Libra 族。此 IBM 子类别反映了 ISO/IEC 9541-1 修正案 1 标准
//              中记录的 ISO 安色尔体类别。
//          子类 ID = 2：毛笔连接体（Brush Joined）
//              此样式以连接（连接）字符为特征，具有用毛笔绘制的外观，粗细笔画之间有中等对比。
//              此字体样式的一个例子是 Mistral 族。此 IBM 子类别反映了 ISO 手写类别、连接子类
//              别和非正式特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 3：正式连接体（Formal Joined）
//              此样式以连接（连接）字符为特征，具有印刷（或用硬毛笔绘制）的外观，粗细笔画之间
//              有极端对比。此字体样式的一个例子是 Coronet 族。此 IBM 子类别反映了 ISO 手写类
//              别、连接子类别和正式特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 4：单调连接体（Monotone Joined)
//              此样式以连接（连接）字符为特征，具有统一的外观，笔画中几乎没有或没有对比。此
//              字体样式的一个例子是 Kaufmann 族。此 IBM 子类别反映了 ISO 手写类别、连接子类
//              别和单调特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 5：书法体（Calligraphic）
//              此样式以精美手绘、不连接（非连接）字符为特征，具有用宽边笔绘制的外观。此字体
//              样式的一个例子是 Thompson Quillscript 族。此 IBM 子类别反映了 ISO 手写类别、
//              不连接子类别和书法特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 6：毛笔不连接体（Brush Unjoined)
//              此样式以不连接（非连接）字符为特征，具有用毛笔绘制的外观，粗细笔画之间有中等
//              对比。此字体样式的一个例子是 Saltino 族。此 IBM 子类别反映了 ISO 手写类别、
//              不连接子类别和毛笔特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 7：正式不连接体（Formal Unjoined）
//              此样式以不连接（非连接）字符为特征，具有印刷（或用硬毛笔绘制）的外观，粗细笔
//              画之间有极端对比。此字体样式的一个例子是 Virtuosa 族。此 IBM 子类别反映了 ISO
//              手写类别、不连接子类别和正式特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 8：单调不连接体（Monotone Unjoined）
//              此样式以不连接（非连接）字符为特征，具有统一的外观，笔画中几乎没有或没有对比。
//              此字体样式的一个例子是 Gilles Gothic 族。此 IBM 子类别反映了 ISO 手写类别、
//              不连接子类别和单调特定组，如 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 9-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 11 保留供将来使用（Reserved for Future Use）。此类别 ID 保留供将来分配，
//      未经 IBM正式分配不得使用。
//
//      类别 ID = 12 符号体（Symbolic）。此样式通常独立于设计，使其适合与任何字体一起使用
//      的 Pi 和特殊字符（图标、装饰符号、技术符号等）。此 IBM 类别反映了各种 ISO 特定组，
//      如下所述和 ISO/IEC 9541-1 修正案 1 标准中所述。
//          子类 ID = 0：无分类
//              此子类 ID 用于指示相关字体没有设计子分类，或者设计子分类对字体资源的创建
//              者或用户不重要。
//          子类 ID = 1-2：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 3：混合衬线体（Mixed Serif）
//              此样式的特征是字体中设计重要的字符（例如，上标和下标字符、数字、版权或商
//              标符号等）上同时或组合使用衬线和无衬线设计。此字体样式的一个例子可以在 IBM
//              Symbol 族中找到。此 IBM 子类别未在 ISO/IEC 9541-1 修正案 1 标准中反映。
//          子类 ID = 4-5：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 6：旧式衬线体（Oldstyle Serif）
//              此样式的特征是字体中设计重要的字符（例如，上标和下标字符、数字、版权或商标
//              符号等）上具有旧式衬线 IBM 类别设计。此字体样式的一个例子可以在 IBM Sonoran
//              Pi Serif 族中找到。此 IBM 子类别未直接在 ISO/IEC 9541-1 修正案 1 标准中反映，
//              但间接由 ISO 衬线类别和易读性子类别反映（意味着字体的所有字符都表现出设计外
//              观，而只有一部分字符实际表现出设计）。
//          子类 ID = 7：新怪诞无衬线体（Neo-grotesque Sans Serif）
//              此样式的特征是字体中设计重要的字符（例如，上标和下标字符、数字、版权或商标
//              符号等）上具有新怪诞无衬线 IBM 字体类别和子类别设计。此字体样式的一个例子可
//              以在 IBM Sonoran Pi Sans Serif 族中找到。此 IBM 子类别未直接在 ISO/IEC 9541-1
//              修正案 1 标准中反映，但间接由 ISO 无衬线类别和哥特子类别反映（意味着字体的
//              所有字符都表现出设计外观，而只有一部分字符实际表现出设计）。
//          子类 ID = 8-14：保留供将来使用
//              这些子类 ID 保留供将来分配，未经 IBM 正式分配不得使用。
//          子类 ID = 15：杂项
//              此子类 ID 用于相关设计类别中未被其他子类别覆盖的杂项设计。
//
//      类别 ID = 13 保留
//      类别 ID = 14 保留

typedef prh_packed_struct {
    /* 02 */ prh_r16 version; // 5 4 3 2 1 0
    /* 04 */ prh_i16 average_char_width; // 所有非零宽度字形的字宽的算术平均值
    /* 06 */ prh_r16 weight_class; // 字重，1 ~ 1000，一般 100 200 ~ 900
    /* 08 */ prh_r16 width_class; // 字体紧缩程度，1 ~ 9
    /* 10 */ prh_r16 right_flags;
    /* 12 */ prh_i16 subscript_x_size;
    /* 14 */ prh_i16 subscript_y_size;
    /* 16 */ prh_i16 subscript_x_offset;
    /* 18 */ prh_i16 subscript_y_offset;
    /* 20 */ prh_i16 superscript_x_size;
    /* 22 */ prh_i16 superscript_y_size;
    /* 24 */ prh_i16 superscript_x_offset;
    /* 26 */ prh_i16 superscript_y_offset;
    /* 28 */ prh_i16 strikeout_size;
    /* 30 */ prh_i16 strikeout_position;
    /* 32 */ prh_i16 family_class;
    /* 42 */ prh_r08 panose[10];
    /* 46 */ prh_r32 unicode_range_00_31;
    /* 50 */ prh_r32 unicode_range_32_63;
    /* 54 */ prh_r32 unicode_range_64_95;
    /* 58 */ prh_r32 unicode_range_96_127;
    /* 62 */ prh_r32 vendor_tag;
    /* 64 */ prh_r16 selection_flags;
    /* 66 */ prh_r16 first_char_index;
    /* 68 */ prh_r16 last_char_index;                // Apple TrueType OS/2 版本 0 在此处停止
    /* 70 */ prh_i16 typo_ascender;
    /* 72 */ prh_i16 typo_descender;
    /* 74 */ prh_i16 typo_line_gap;
    /* 76 */ prh_r16 win_ascent;
    /* 78 */ prh_r16 win_descent;                    // 版本 0 结束位置
    /* 82 */ prh_r32 code_page_range_00_31;          // 仅版本 1 2 3 4 5
    /* 86 */ prh_r32 code_page_range_32_63;          // 仅版本 1 2 3 4 5
    /* 88 */ prh_i16 height;                         // 仅版本 2 3 4 5
    /* 90 */ prh_i16 capital_height;                 // 仅版本 2 3 4 5
    /* 92 */ prh_r16 default_char;                   // 仅版本 2 3 4 5
    /* 94 */ prh_r16 break_char;                     // 仅版本 2 3 4 5
    /* 96 */ prh_r16 max_glyph_context_length;       // 仅版本 2 3 4 5
    /* 98 */ prh_r16 lower_optical_point_size_twip;  // 仅版本 5
    /*100 */ prh_r16 upper_optical_point_size_twip;  // 仅版本 5
} prh_font_os_2_table;
prh_packing_reset()

void prh_print_font_os_2_table(prh_open_font *f) {
    if (f->os_2.length == 0) prh_abort_line();
    prh_font_os_2_table *p = (prh_font_os_2_table *)prh_load_font_table(f, &f->os_2);
    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)p, f->os_2.length);
    prh_r16 table_version = prh_r16_be_to_host(p->version);
    prh_i16 typo_ascender = 0;
    prh_i16 typo_descender = 0;
    prh_i16 typo_line_gap = 0;
    prh_r16 win_ascent = 0;
    prh_r16 win_descent = 0;
    prh_r32 code_page_range_00_31 = 0;
    prh_r32 code_page_range_32_63 = 0;
    prh_i16 height = 0;
    prh_i16 capital_height = 0;
    prh_r16 default_char = 0;
    prh_r16 break_char = 0;
    prh_r16 max_glyph_context_length = 0;
    prh_r16 lower_optical_point_size_twip = 0;
    prh_r16 upper_optical_point_size_twip = 0;
    if (table_version == 0) {
        if (f->os_2.length == prh_offsetof(prh_font_os_2_table, typo_ascender)) {
        } else if (f->os_2.length == prh_offsetof(prh_font_os_2_table, code_page_range_00_31)) {
            goto label_version_0;
        } else {
            prh_abort_error(f->os_2.length);
        }
    } else if (table_version == 1) {
        prh_real_assert(f->os_2.length == prh_offsetof(prh_font_os_2_table, height));
        goto label_version_1;
    } else if (table_version <= 4) {
        prh_real_assert(f->os_2.length == prh_offsetof(prh_font_os_2_table, lower_optical_point_size_twip));
        goto label_version_2_4;
    } else if (table_version == 5) {
        prh_real_assert(f->os_2.length == sizeof(prh_font_os_2_table));
        lower_optical_point_size_twip = prh_r16_be_to_host(p->lower_optical_point_size_twip);
        upper_optical_point_size_twip = prh_r16_be_to_host(p->upper_optical_point_size_twip);
label_version_2_4:
        height = (prh_i16)prh_r16_be_to_host(p->height);
        capital_height = (prh_i16)prh_r16_be_to_host(p->capital_height);
        default_char = prh_r16_be_to_host(p->default_char);
        break_char = prh_r16_be_to_host(p->break_char);
        max_glyph_context_length = prh_r16_be_to_host(p->max_glyph_context_length);
label_version_1:
        code_page_range_00_31 = prh_r32_be_to_host(p->code_page_range_00_31);
        code_page_range_32_63 = prh_r32_be_to_host(p->code_page_range_32_63);
label_version_0:
        typo_ascender = (prh_i16)prh_r16_be_to_host(p->typo_ascender);
        typo_descender = (prh_i16)prh_r16_be_to_host(p->typo_descender);
        typo_line_gap = (prh_i16)prh_r16_be_to_host(p->typo_line_gap);
        win_ascent = prh_r16_be_to_host(p->win_ascent);
        win_descent = prh_r16_be_to_host(p->win_descent);
    } else {
        prh_real_assert(table_version);
    }
    prh_print(
        "os/2 table index %d / %d\n"
        "os/2 table tag 0x%08x (%c%c%c%c)\n"
        "os/2 table offset %.10d (%d/4)\n"
        "os/2 table length %.10d (%d/4)\n"
        "os/2 table checksum 0x%08x 0x%08x (valid %d)\n"
        "os/2 table version %d\n"
        "----------------------------\n"
        "os/2 metric - average char width %d\n"
        "os/2 metric - weight class %d\n"
        "os/2 metric - width class %d\n"
        "os/2 metric - right flags %04x\n"
        "os/2 metric - subscript x size %d\n"
        "os/2 metric - subscript y size %d\n"
        "os/2 metric - subscript x offset %d\n"
        "os/2 metric - subscript y offset %d\n"
        "os/2 metric - superscript x size %d\n"
        "os/2 metric - superscript y size %d\n"
        "os/2 metric - superscript x offset %d\n"
        "os/2 metric - superscript y offset %d\n"
        "os/2 metric - strikeout size %d\n"
        "os/2 metric - strikeout position %d\n"
        "os/2 metric - family class %04x\n"
        "os/2 metric - panose - family_type %d serif_style %d weight %d proportion %d contrast %d stroke_variation %d arm_style %d letter_form %d midline %d x_height %d\n"
        "os/2 metric - unicode range 31 ~ 00 %032?b\n"
        "os/2 metric - unicode range 63 ~ 32 %032?b\n"
        "os/2 metric - unicode range 95 ~ 64 %032?b\n"
        "os/2 metric - unicode range 127~ 96 %032?b\n"
        "os/2 metric - vendor tag %08x '%c%c%c%c'\n"
        "os/2 metric - selection flags %04x\n"
        "os/2 metric - first char index %02x\n"
        "os/2 metric - last char index %02x\n"
        "os/2 metric - typo ascender %d\n"
        "os/2 metric - typo descender %d\n"
        "os/2 metric - typo line gap %d\n"
        "os/2 metric - win ascent %d\n"
        "os/2 metric - win descent %d\n"
        "os/2 metric - code page range 31 ~ 00 %032?b\n"
        "os/2 metric - code page range 63 ~ 32 %032?b\n"
        "os/2 metric - height %d\n"
        "os/2 metric - capital height %d\n"
        "os/2 metric - default char %02x\n"
        "os/2 metric - break char %02x\n"
        "os/2 metric - max glyph context length %d\n"
        "os/2 metric - lower optical point size %d\n"
        "os/2 metric - upper optical point size %d\n",
        (prh_reg)f->os_2.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->os_2.tabletag,
        (prh_reg)prh_byte_4(f->os_2.tabletag),
        (prh_reg)prh_byte_3(f->os_2.tabletag),
        (prh_reg)prh_byte_2(f->os_2.tabletag),
        (prh_reg)prh_byte_1(f->os_2.tabletag),
        (prh_reg)f->os_2.offset, (prh_reg)f->os_2.offset % 4,
        (prh_reg)f->os_2.length, (prh_reg)f->os_2.length % 4,
        (prh_reg)f->os_2.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->os_2.checksum),
        (prh_reg)table_version,
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->average_char_width),
        (prh_reg)prh_r16_be_to_host(p->weight_class),
        (prh_reg)prh_r16_be_to_host(p->width_class),
        (prh_reg)prh_r16_be_to_host(p->right_flags),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->subscript_x_size),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->subscript_y_size),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->subscript_x_offset),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->subscript_y_offset),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->superscript_x_size),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->superscript_y_size),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->superscript_x_offset),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->superscript_y_offset),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->strikeout_size),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->strikeout_position),
        (prh_reg)(prh_int)(prh_i16)prh_r16_be_to_host(p->family_class),
        (prh_reg)p->panose[0], (prh_reg)p->panose[1], (prh_reg)p->panose[2], (prh_reg)p->panose[3], (prh_reg)p->panose[4],
        (prh_reg)p->panose[5], (prh_reg)p->panose[6], (prh_reg)p->panose[7], (prh_reg)p->panose[8], (prh_reg)p->panose[9],
        (prh_reg)prh_r32_be_to_host(p->unicode_range_00_31),
        (prh_reg)prh_r32_be_to_host(p->unicode_range_32_63),
        (prh_reg)prh_r32_be_to_host(p->unicode_range_64_95),
        (prh_reg)prh_r32_be_to_host(p->unicode_range_96_127),
        (prh_reg)prh_r32_be_to_host(p->vendor_tag),
        (prh_reg)((prh_byte *)&p->vendor_tag)[0],
        (prh_reg)((prh_byte *)&p->vendor_tag)[1],
        (prh_reg)((prh_byte *)&p->vendor_tag)[2],
        (prh_reg)((prh_byte *)&p->vendor_tag)[3],
        (prh_reg)prh_r16_be_to_host(p->selection_flags),
        (prh_reg)prh_r16_be_to_host(p->first_char_index),
        (prh_reg)prh_r16_be_to_host(p->last_char_index),
        (prh_reg)(prh_int)(prh_i16)typo_ascender,
        (prh_reg)(prh_int)(prh_i16)typo_descender,
        (prh_reg)(prh_int)(prh_i16)typo_line_gap,
        (prh_reg)win_ascent,
        (prh_reg)win_descent,
        (prh_reg)code_page_range_00_31,
        (prh_reg)code_page_range_32_63,
        (prh_reg)(prh_int)(prh_i16)height,
        (prh_reg)(prh_int)(prh_i16)capital_height,
        (prh_reg)default_char,
        (prh_reg)break_char,
        (prh_reg)max_glyph_context_length,
        (prh_reg)lower_optical_point_size_twip,
        (prh_reg)upper_optical_point_size_twip);
    prh_da_free((prh_byte *)p);
}

// 命名表（name）
//
// 命名表允许将多语言字符串与 OpenType 字体关联。这些字符串可以代表版权声明、字体名称、
// 家族名称、样式名称等。为了保持此表简短，字体制造商可能希望在某些少量语言中制作有限
// 的一组条目；稍后，字体可以"本地化"，字符串被翻译或添加。OpenType 字体中需要这些字符
// 串的其他部分可以使用语言无关的名称 ID 引用它们。除了语言变体外（language variants），
// 该表还允许平台特定的字符编码变体（character-encoding variants）。需要特定字符串的应
// 用程序可以通过其平台 ID、编码 ID、语言 ID 和名称 ID 查找它。注意，不同平台可能对字符
// 串编码有不同的要求。
//
// 许多较新的平台可以使用为不同平台设计的字符串，如果字体不包含该平台的字符串。然而，某
// 些应用程序可能会显示不正确的字符串，如果当前平台的字符串未包含。
//
// 命名表头（Naming Table Header）
//
// 命名表（Naming Table）有两个版本。版本 0 使用平台特定的数字语言标识符。版本 1 允许使用
// 语言标签字符串来指示字符串的语言。两个版本都包含可变大小的字符串数据存储，以及名称记录
// 数组，用于标识字符串的类型（名称 ID）、平台、编码和语言变体，以及字符串在存储中的位置。
//
//      命名表版本 0，版本 0 命名表的组织结构如下：
//      类型        名称                描述
//      uint16      version             表版本号（=0）
//      uint16      count               名称记录的数量
//      Offset16    storageOffset       字符串存储起始偏移（从表开头开始）
//      NameRecord  nameRecord[count]   名称记录，其中 count 是记录数
//      Variable                        实际字符串数据的存储
//
// 版本 0 与版本 1 在语言标识处理方面有所不同：它仅使用数字语言 ID，这些值通常小于 0x8000，
// 具有平台特定的解释。有关更多详细信息，请参阅下面的名称记录。命名表版本 1，版本 1 命名表
// 添加了附加元素，如下：
//      类型            名称                        描述
//      uint16          version                     表版本号（=1）
//      uint16          count                       名称记录的数量
//      Offset16        storageOffset               字符串存储起始偏移（从表开头开始）
//      NameRecord      nameRecord[count]           名称记录，其中 count 是记录数
//      uint16          langTagCount                语言标签记录的数量
//      LangTagRecord   langTagRecord[langTagCount] 语言标签记录，其中 langTagCount 是记录数
//      Variable                                    实际字符串数据的存储
//
// 使用版本 1 时，名称记录中的语言 ID 可以小于或大于 0x8000。如果语言 ID 小于 0x8000，则与
// 版本 0 命名表一样具有平台特定的解释。如果语言 ID 等于或大于 0x8000，则与引用语言标签字符
// 串的语言标签记录（LangTagRecord）相关联。这样，语言 ID 与指定使用该语言 ID 的名称记录的
// 语言的语言标签字符串相关联，无论平台如何。这些可用于支持此语言标签机制的任何平台。
//
// 使用版本 1 命名表的字体可以对给定平台和编码组合使用平台特定语言 ID 和语言标签记录的混合。
// 每个 LangTagRecord 的组织结构如下：
//      类型        名称            描述
//      uint16      length          语言标签字符串长度（以字节为单位）
//      Offset16    langTagOffset   从存储区开头开始的语言标签字符串偏移（以字节为单位）
//
// 命名表中存储的语言标签字符串必须使用 UTF-16BE 编码。语言标签必须符合 IETF 规范 BCP 47。
// 这提供了诸如 "en"、"fr-CA" 和 "zh-Hant" 之类的标签来标识语言，包括方言、书写形式和其他
// 语言变体。
//
// 语言标签记录按顺序与从 0x8000 开始的语言 ID 相关联。每个语言标签记录对应于比前一个语言
// 标签记录的语言 ID 大 1 的语言 ID。因此，与语言标签记录相关联的语言 ID 必须在 0x8000 到
// 0x8000 + langTagCount - 1 的范围内。如果名称记录使用的语言 ID 大于此值，则该语言的身份
// 未知；不应使用此类名称记录。
//
// 例如，假设字体有两个语言标签记录引用存储中的字符串：第一个引用字符串"en"，第二个引用字
// 符串"zh-Hant-HK"。在这种情况下，名称记录中使用语言 ID 0x8000 来索引英语语言字符串。名称
// 记录中使用语言 ID 0x8001 来索引香港特别行政区使用的繁体中文字符串。
//
// 名称记录（Name Records）。字符串存储中的每个字符串由名称记录引用。名称记录具有多部分键，
// 用于标识字符串的逻辑类型及其语言或平台特定实现变体，以及字符串在字符串存储中的位置。每
// 个 NameRecord 的组织结构如下：
//      类型        名称            描述
//      uint16      platformID      平台 ID
//      uint16      encodingID      平台特定编码 ID
//      uint16      languageID      语言 ID
//      uint16      nameID          名称 ID
//      uint16      length          字符串长度（以字节为单位）
//      Offset16    stringOffset    从存储区开头开始的字符串偏移（以字节为单位）
//
// 名称 ID 标识逻辑字符串类别，如家族名称或版权。名称 ID 对所有平台和语言都相同；这些在下面
// 详细描述。键的其他三个元素允许平台特定实现：平台 ID、平台特定编码 ID 和语言 ID。
//
// 与 'cmap' 表中的编码记录一样，名称记录必须首先按平台 ID 排序，然后按平台特定编码 ID 排序，
// 然后按语言 ID 排序，最后按名称 ID 排序。各种 ID 的描述如下。
//
// 平台、编码和语言。名称记录的平台、编码和语言 ID 允许平台特定实现。不同平台可以支持不同的
// 编码和不同的语言。所有编码 ID 都是平台特定的。语言 ID 同样是平台特定的，除非在版本 1 命名
// 表的语言标签机制中使用的 ID 情况，如上所述。注意：平台 ID、平台特定编码 ID 以及某些情况下
// 的平台特定语言 ID 也在 'cmap' 表中使用。
//
// 语言 ID 指标识特定字符串所用语言的值。小于 0x8000 的值在平台特定基础上定义。版本 0 命名表
// 必须仅使用下面给出的平台特定枚举中小于 0x8000 的语言 ID 值。但是，此要求的例外允许用于用户
// 定义的平台，平台 ID 240 到 255。大于或等于 0x8000 的值可以在版本 1 命名表中与语言标签记录
// 一起使用，如上所述。并非所有平台都有平台特定语言 ID，并非所有平台都支持语言标签记录。
//
// 本章末尾提供了 'name' 表中使用的平台、编码和语言 ID 的详细列表。
//
// 名称 ID（Name IDs）。以下名称 ID 是预定义的，适用于所有平台，除非另有说明。名称 ID 26 到
// 255（含）保留用于未来标准名称。名称 ID 256 到 32767（含）保留用于字体特定名称，如字体布局
// 功能引用的名称。
//
//      ID  含义
//      0   版权声明（Copyright Notice）。
//      1   字体家族名称（Font Family Name）。字体家族名称与字体子家族名称（名称 ID 2）结合
//          使用，应在最多四个仅在字重或样式（斜体/倾斜）方面不同的字体之间共享，如下所述。
//          这种四向区分（four-way distinction）也应反映在 OS/2.fsSelection 字段中，使用位
//          0 和 5。
//          虽然某些平台或应用程序没有此限制，但许多使用这对名称的现有应用程序假设字体家族
//          名称最多由四个形成字体样式链接组的字体共享：常规、斜体（或倾斜）、粗体和粗斜体
//          （或粗倾斜）。为了与最广泛的平台和应用程序兼容，强烈建议字体以这种方式限制字体
//          家族名称的使用。
//          对于包含四种基本样式（常规、斜体、粗体、粗斜体）以外的字体的扩展排版家族，强烈
//          建议在字体中使用名称 ID 16 和 17 来创建扩展的排版分组。请参阅下面提供的示例。
//          还强烈建议应用程序使用名称 ID 16 和 17 支持扩展排版家族分组。注意，可变字体可以
//          包含大量命名实例，每个实例将使用共享的排版家族名称（名称 ID 16），并将具有排版
//          子家族名称（相当于名称 ID 17）。基于名称 ID 1 和 2 假设四样式家族分组的应用程序
//          可能会为可变字体提供糟糕的用户体验。
//          对于扩展排版家族中超出基本四向区分的字体，区分属性应反映在字体家族名称中，以便
//          这些字体在仅支持四成员家族的应用程序中显示为单独的字体家族。例如，Arial Narrow
//          字体的字体家族名称是"Arial Narrow"；Arial Black 字体的字体家族名称是"Arial Black"。
//          注意，在这种情况下，还应包含名称 ID 16，并使用共享名称（例如"Arial"）来反映完整
//          的排版家族。
//      2   字体子家族名称（Font Subfamily Name）。字体子家族与字体家族名称（名称 ID 1）结合
//          使用，区分具有相同字体家族名称的组中的字体。这应仅用于字重和样式（斜体/倾斜）变体，
//          如下所述。这种四向区分也应反映在 OS/2.fsSelection 字段中，使用位 0 和 5。
//          虽然某些平台或应用程序没有此限制，但许多使用名称 ID 1 和 2 的现有应用程序假设字体
//          家族名称最多由四个形成字体样式链接组的字体共享，并且字体子家族名称反映四种基本样式
//          之一：常规、斜体（或倾斜）、粗体和粗斜体（或粗倾斜）。为了与最广泛的平台和应用程序
//          兼容，强烈建议字体应以这种方式限制字体子家族的使用。
//          对于包含四种基本样式（常规、斜体、粗体、粗斜体）以外的字体的扩展排版家族，强烈建议
//          在字体中使用名称 ID 16 和 17 来创建扩展的排版分组。
//          在包含常规、粗体、斜体或粗斜体以外字体的扩展排版家族中，这些其他字体的区分在字体家
//          族名称中进行，以便字体显示为单独的家族。在某些情况下，这可能导致为可能不被视为常规
//          字体的字体指定子家族名称"Regular"。例如，Arial Black 字体的字体家族名称为"Arial Black"，
//          子家族名称为"Regular"。注意，在这种情况下，还应包含名称 ID 16 和 17，使用名称 ID 16
//          的共享值（例如"Arial"）来反映完整的排版家族，以及适当反映每个字体实际设计变体的名称
//          ID 17 值。
//          不属于扩展排版家族且没有独特字重或样式（例如，中等字重，非斜体）的字体应使用"Regular"
//          作为字体子家族名称（对于英语）。
//      3   唯一字体标识符（Unique Font Identifier）。
//      4   反映所有家族和相关子家族描述符的完整字体名称。完整字体名称通常是名称 ID 1 和 2 的组
//          合，或名称 ID 16 和 17 的组合，或类似的人类可读变体。
//          对于扩展排版家族（即包含常规、斜体、粗体和粗斜体变体以外的家族的字体），名称 ID 1
//          和 2 的值通常选择为与某些假设家族最多有四个样式链接字体的应用程序兼容。在这种情况下，
//          某些字体可能最终具有子家族名称（名称 ID 2）"Regular"，即使该字体在排版上不被视为常规
//          字体。对于名称 ID 2 指定为"Regular"的此类非常规字体，"Regular"描述符通常从名称 ID 4
//          中省略。例如，Arial Black 字体的字体家族名称（名称 ID 1）为"Arial Black"，子家族名称
//          （名称 ID 2）为"Regular"，但完整字体名称（名称 ID 4）为"Arial Black"。注意，这些字体
//          中还应包含名称 ID 16 和 17，并且名称 ID 4 通常是名称 ID 16 和 17 的组合，无需任何关
//          于"Regular"的附加限定。
//      5   版本字符串（Version String）。应以模式 "Version <number>.<number>" 表示，"Version"
//          不区分大小写。字符串必须包含以下形式的版本号：值小于 65,535 的一个或多个数位（0-9），
//          后跟一个句点，后跟值小于 65,535 的一个或多个数位。除数字以外的任何字符将终止次要编
//          号。诸如";"之类的字符有助于分隔版本信息的不同部分。
//          字符串中的第一个此类匹配可由安装软件用于比较字体版本。某些安装程序可能要求字符串以
//          "Version "开头，后跟上述版本号。
//      6   字体的 PostScript 名称。名称 ID 6 指定用于调用与此 OpenType 字体对应的 PostScript
//          语言字体的字符串。转换为 ASCII 时，名称字符串不得超过 63 个字符，并限制为可打印
//          ASCII 子集，代码 33 到 126，但以下 10 个字符除外：'['、']'、'('、')'、'{'、'}'、'<'、
//          '>'、'/'、'%'。
//          在 CFF OpenType 字体中，不要求此名称与 CFF 名称 INDEX 中的字体名称相同。因此，相同
//          的 CFF 可以在字体集合中的多个字体组件之间共享。有关其他信息，请参阅"OpenType 字体建
//          议"的 'name' 表部分。
//          https://learn.microsoft.com/en-us/typography/opentype/spec/recom#name
//      7   商标（Trademark）。用于保存此字体的任何商标通知/信息。此类信息应基于法律建议。这与
//          版权（Copyright）明显分开。
//      8   制造商名称（Manufacturer Name）。
//      9   设计师（Designer）。字体（typeface）设计师的姓名。
//      10  描述（Description）。字体描述。可以包含修订信息、使用建议、历史、特性等。
//      11  供应商（Vendor）URL。字体供应商的 URL（带协议，例如 http://、ftp://）。如果 URL 中
//          嵌入了唯一序列号（unique serial number），可用于注册字体。
//      12  设计师 URL。字体设计师的 URL（带协议，例如 http://、ftp://）。
//      13  许可证描述（License Description）。提供字体所依据的许可证或许可证的描述。这可以是对
//          命名许可协议的引用（例如，常见的开源许可证）、标识捆绑字体的软件使用许可证、有关如何
//          定位外部许可证的信息（另请参阅名称 ID 14）、允许用途的摘要，或许可协议的完整法律文本。
//          对此名称 ID 的内容寻求法律建议是明智的，以避免其或许可证之间可能的解释冲突。
//      14  许可证信息（License Info）URL。附加许可信息的 URL。
//      15  保留（Reserved）。
//      16  排版家族名称（Typographic Family Name）。排版家族分组对其中包含的字形数量没有任何限
//          制，与 4 样式家族分组（ID 1）形成对比，后者既出于历史原因存在，也用于表达样式链接组。
//          如果名称 ID 16 不存在，则名称 ID 1 被视为排版家族名称。在规范的早期版本中，名称 ID
//          16 被称为"首选家族"。
//      17  排版子家族名称（Typographic Subfamily Name）。这允许字体设计师在排版家族分组中指定子
//          家族名称。此字符串必须在特定排版家族中唯一。如果不存在，则名称 ID 2 被视为排版子家族
//          名称。在规范的早期版本中，名称 ID 17 被称为"首选子家族"。
//      18  兼容全名（仅限 Macintosh）。在 Macintosh 上，菜单名称使用 FOND 资源构造。这通常与全
//          名匹配。如果希望字体名称与全名不同，可以在 ID 18 中插入兼容全名。
//      19  示例文本（Sample Text）。这可以是字体名称，或设计师认为显示字体的最佳示例的任何其他
//          文本。
//      20  PostScript CID findfont 名称。其在字体中的存在意味着名称 ID 6 保存的 PostScript 字
//          体名称旨在与 PostScript 解释器中的"composefont"调用一起使用，以调用字体。请参阅名称
//          ID 6 的定义。
//          名称 ID 20 字符串中保存的值被解释为 PostScript 字体名称，旨在与 PostScript 解释器中
//          的"findfont"调用一起使用，以调用字体。
//          转换为 ASCII 时，此名称字符串必须限制为可打印 ASCII 子集，代码 33 到 126，但以下 10
//          个字符除外：'['、']'、'('、')'、'{'、'}'、'<'、'>'、'/'、'%'。
//          有关其他信息，请参阅"OTF 字体建议"。
//          https://learn.microsoft.com/en-us/typography/opentype/spec/recom
//      21  WWS 家族名称。用于在 ID 16 和 17 的条目不符合 WWS 模型时提供符合 WWS 的家族名称。即，
//          ID 17 的条目包含字重、宽度或倾斜以外某些属性的限定符的情况。如果设置了 OS/2 fsSelection
//          字段的位 8，则不需要也不应包含 WWS 家族名称条目。相反，如果包含此 ID 的条目，则不应
//          设置位 8。有关详细信息，请参阅 OS/2.fsSelection 字段。名称 ID 21 的示例："Minion Pro
//          Caption"和"Minion Pro Display"。这些示例中名称 ID 16 为"Minion Pro"。
//          请参阅下面关于 ID 21 和 22 的附加说明。
//      22  WWS 子家族名称。与 ID 21 结合使用，此 ID 在 ID 16 和 17 的条目不符合 WWS 模型时提供
//          符合 WWS 的子家族名称（仅反映字重、宽度和倾斜属性）。与 ID 21 的情况一样，此 ID 的使
//          用应与设置 OS/2 fsSelection 字段的位 8 成反比。名称 ID 22 的示例："Semibold Italic"、
//          "Bold Condensed"。例如，名称 ID 17 可以是"Semibold Italic Caption"或"Bold Condensed
//          Display"。
//          请参阅下面关于 ID 21 和 22 的附加说明。
//      23  浅色背景调色板（Light Background Palette）。如果在 CPAL 表的调色板标签数组中使用此
//          ID，则指定 CPAL 表中的相应调色板适合在浅色背景（如白色）上显示字体时使用。此 ID 的
//          字符串用作与此调色板关联的用户界面字符串。
//      24  深色背景调色板（Dark Background Palette）。如果在 CPAL 表的调色板标签数组中使用此
//          ID，则指定 CPAL 表中的相应调色板适合在深色背景（如黑色）上显示字体时使用。此 ID 的
//          字符串用作与此调色板关联的用户界面字符串。
//      25  变体 PostScript 名称前缀。如果在可变字体中存在，可用作变体字体 PostScript 名称生成
//          算法中的家族前缀。字符集限制为 ASCII 范围内的大写拉丁字母、小写拉丁字母和数字。字体
//          中名称 ID 25 的所有名称字符串，转换为 ASCII 时必须相同。有关在字体中包含名称 ID 25
//          的原因和示例，请参阅 Adobe 技术说明 5902："变体字体的 PostScript 名称生成"。有关
//          OpenType 字体变体的一般信息，请参阅 OpenType 字体变体概述章节。
//
// 对于包含在字重、宽度或倾斜以外属性方面与常规不同的成员字形的排版家族，也可能有一些仅在
// 以下三个方面不同的成员字形。例如，Minion Pro 家族包括 Minion Pro Display，但也包括 Minion
// Pro Bold 和 Minion Pro Italic。ID 21 和 22 应仅用于在字重、宽度或倾斜以外属性方面与常规
// 字形不同的字体中。例如，ID 21 和 22 应在 Minion Pro Display 中使用，但不在 Minion Pro
// Bold 或 Minion Pro Italic 中使用。
//
// 注意：虽然 Apple 和 Microsoft 都支持相同的名称字符串集，但解释可能略有不同。但由于名称字
// 符串按平台、编码和语言存储（为 Apple 和 MS 平台放置单独的字符串），这不应构成问题。
//
// 此表对 Microsoft 平台的关键信息涉及名称 ID 1、2、4、16 和 17 的使用。注意，某些较新的应用
// 程序将使用名称 ID 16 和 17，而某些旧版应用程序需要名称 ID 1 和 2，并且还假设这些值有一定
// 的限制（请参阅上面名称 ID 1 和 2 的描述）。字体应包含所有这些字符串，以实现最广泛的应用程
// 序兼容性。为了更好地理解如何为这些名称 ID 设置值，创建了一些名称使用、字重类和样式标志的
// 示例。https://learn.microsoft.com/en-us/typography/opentype/spec/namesmp

const void *prh_impl_font_get_name_type_string(prh_r32 name_id) {
    const char *s[] = {
        "Copyright", "Font Family", "Font Subfamily", "Unique Font Identifier", "Full Font Name", "Version", "PostScript Name", "Trademark",
        "Manufacturer", "Designer", "Description", "Vendor URL", "Designer URL", "License Description", "License Info URL", "Reserved",
        "Typographic Family Name", "Typographic Subfamily Name", "Compatible Full Name (for Macintosh)", "Sample Text", "PostScript CID findfont Name",
        "WWS Family Name", "WWS Subfamily Name", "Light Background Palette", "Dark Background Palette", "Variations PostScript Name Prefix"};
    if (name_id <= 25) return s[name_id];
    if (name_id <= 255) return "Standard Reserved Name";
    if (name_id <= 32767) return "Font Specific Reserved Name";
    return "Unknnown";
}

// 名称 ID 示例。以下是基于 Times New Roman Bold 的这些字符串可能如何定义的示例：
//  * 字体供应商的版权字符串：© Copyright the Monotype Corporation plc，1990
//  * 用户看到的名称：Times New Roman
//  * 样式的名称：Bold
//  * 应用程序可以存储以标识所用字体的唯一标识符：Monotype: Times New Roman Bold: 1990
//  * 字体的完整、唯一、人类可读名称，此名称由 Windows 使用：Times New Roman Bold
//  * 字体供应商的发布和版本信息：Version 1.00 June 1, 1990, initial release
//  * 字体在 PostScript 打印机上的名称：TimesNewRoman-Bold
//  * 商标字符串：Times New Roman is a registered trademark of the Monotype Corporation.
//  * 制造商：Monotype Corporation
//  * 设计师：Stanley Morison
//  * 描述：Designed in 1932 for the Times of London newspaper. Excellent readability and a narrow overall width, allowing more words per line than most fonts.
//  * 供应商 URL：http://www.monotype.com
//  * 设计师 URL：http://www.monotype.com
//  * 许可证描述：This font may be installed on all of your machines and printers, but you may not sell or give these fonts to anyone else.
//  * 许可证信息 URL：http://www.monotype.com/license/
//  * 保留。
//  * 首选家族。不存在名称字符串，因为它与名称 ID 1（字体家族名称）相同。
//  * 首选子家族。不存在名称字符串，因为它与名称 ID 2（字体子家族名称）相同。
//  * 兼容全名（仅限 Macintosh）。不存在名称字符串，因为它与名称 ID 4（全名）相同。
//  * 示例文本：The quick brown fox jumps over the lazy dog.
//  * PostScript CID findfont 名称。不存在名称字符串。因此，名称 ID 6 定义的 PostScript 名称
//    应与 PostScript 解释器中的"findfont"调用一起使用，以定位字体。
//  * WWS 家族名称：由于 Times New Roman 是 WWS 字体，因此不需要指定此字段。如果字体包含"caption"、
//    "display"、"handwriting"等样式，则应在此处注明。
//  * WWS 子家族名称：由于 Times New Roman 是 WWS 字体，因此不需要指定此字段。
//  * 浅色背景调色板名称。不存在名称字符串，因为这不是彩色字体。
//  * 深色背景调色板名称。不存在名称字符串，因为这不是彩色字体。
//  * 变体 PostScript 名称前缀。不存在名称字符串，因为这不是可变字体。
//
// 以下是 CFF OpenType 日文字体 Kozuka Mincho Std Regular 中名称 ID 6 和 20 的示例（此字体中
// 还包含其他名称 ID）：
//  * PostScript 名称：KozMinStd-Regular。由于字体中存在名称 ID 20，因此名称 ID 6 定义的 PostScript
//    名称应与 PostScript 解释器中的"composefont"调用一起使用，以定位字体。
//  * PostScript CID findfont 名称：KozMinStd-Regular-83pv-RKSJ-H，位于平台 1 [Macintosh]、
//    平台特定脚本 1 [日语]、语言：0xFFFF [英语] 的名称记录中。此名称字符串是应与 PostScript
//    解释器中的"findfont"调用一起使用的 PostScript 名称，以定位字体，并与以下 'cmap' 子表
//    指定的编码相关联，该子表必须存在于字体中：平台：1 [Macintosh]；平台特定编码：1 [日语]；
//    语言：0 [非语言特定]。
//
// 以下是扩展 WWS 专用家族的家族/子家族命名示例。考虑 Adobe Caslon Pro，有六个成员：常规、半
// 粗体和粗体字重的正体和斜体版本。OS/2 表版本 4 的 fsSelection 字段的位 8 应在所有六种字体
// 中设置，且都不应包含 ID 21 或 22 的 'name' 条目。
//  * Adobe Caslon Pro Regular：
//      名称 ID 1：Adobe Caslon Pro
//      名称 ID 2：Regular
//  * Adobe Caslon Pro Italic：
//      名称 ID 1：Adobe Caslon Pro
//      名称 ID 2：Italic
//  * Adobe Caslon Pro Semibold：
//      名称 ID 1：Adobe Caslon Pro
//      名称 ID 2：Bold
//      名称 ID 16：Adobe Caslon Pro
//      名称 ID 17：Semibold
//  * Adobe Caslon Pro Semibold Italic：
//      名称 ID 1：Adobe Caslon Pro
//      名称 ID 2：Bold Italic
//      名称 ID 16：Adobe Caslon Pro
//      名称 ID 17：Semibold Italic
//  * Adobe Caslon Pro Bold：
//      名称 ID 1：Adobe Caslon Pro Bold
//      名称 ID 2：Regular
//      名称 ID 16：Adobe Caslon Pro
//      名称 ID 17：Bold
//  * Adobe Caslon Pro Bold Italic：
//      名称 ID 1：Adobe Caslon Pro Bold
//      名称 ID 2：Italic
//      名称 ID 16：Adobe Caslon Pro
//      名称 ID 17：Bold Italic
//
// 以下是扩展非 WWS 家族的家族/子家族命名示例。考虑 Minion Pro Opticals，有 32 个成员字体：
// 每个四种光学尺寸（常规、caption、display 和 subhead）中常规、中等、半粗体和粗体字重的正
// 体和斜体版本。以下显示此家族中部分字体的名称。OS/2 表版本 4 的 fsSelection 字段的位 8
// 应在不包含 ID 21 或 22 的 'name' 条目的那些字体中设置，且仅在这些字体中设置。
//  * Minion Pro Regular：
//      名称 ID 1：Minion Pro
//      名称 ID 2：Regular
//  * Minion Pro Italic：
//      名称 ID 1：Minion Pro
//      名称 ID 2：Italic
//  * Minion Pro Semibold：
//      名称 ID 1：Minion Pro SmBd
//      名称 ID 2：Regular
//      名称 ID 16：Minion Pro
//      名称 ID 17：Semibold
//  * Minion Pro Semibold Italic：
//      名称 ID 1：Minion Pro SmBd
//      名称 ID 2：Italic
//      名称 ID 16：Minion Pro
//      名称 ID 17：Semibold Italic
//  * Minion Pro Caption：
//      名称 ID 1：Minion Pro Capt
//      名称 ID 2：Regular
//      名称 ID 16：Minion Pro
//      名称 ID 17：Caption
//      名称 ID 21：Minion Pro Caption
//      名称 ID 22：Regular
//  * Minion Pro Semibold Italic Caption：
//      名称 ID 1：Minion Pro SmBd Capt
//      名称 ID 2：Italic
//      名称 ID 16：Minion Pro
//      名称 ID 17：Semibold Italic Caption
//      名称 ID 21：Minion Pro Caption
//      名称 ID 22：Semibold Italic
//
// 平台、编码和语言 ID。以下部分提供有关 'name' 表中使用的平台 ID、平台特定编码 ID 和平台特定
// 语言 ID 的详细信息。有关 'cmap' 表中使用的平台、编码或语言 ID 的详细信息，请参阅 'cmap' 表
// 章节中的编码记录和编码。

// 平台 ID。以下平台 ID 可在 'name' 表中使用，注意，其他平台 ID 的定义，仅用于 'cmap' 表。
//      平台 ID     平台名称        平台特定编码 ID     语言 ID
//      0           Unicode         各种                无
//      1           Macintosh       脚本管理器代码      各种
//      3           Windows         Windows 编码        各种
//
// 平台 ID。定义了以下平台 ID，平台 ID 值 240 到 255 保留给用户定义的平台，不得分配给
// 注册的平台。
//      平台 ID     平台名称            平台特定编码 ID
//      0           Unicode             各种
//      1           Macintosh           脚本管理器代码
//      2           ISO [已弃用]        ISO 编码 [已弃用]
//      3           Windows             Windows 编码
//      4           自定义（Custom）    自定义

#define prh_otf_platform_unicode        0
#define prh_otf_platform_macintosh      1
#define prh_otf_platform_iso_deprecated 2 // 已废弃
#define prh_otf_platform_windows        3
#define prh_otf_platform_custom         4
#define prh_otf_platform_user_defined   240 // 240 ~ 255
#define prh_otf_platform_user_last      255

// Unicode 平台（平台 ID = 0），以下 Unicode 平台的编码 ID 可在 'name' 表中使用：
//      编码 ID     描述
//      0           Unicode 1.0 语义—已弃用
//      1           Unicode 1.1 语义—已弃用
//      2           ISO/IEC 10646 语义—已弃用
//      3           Unicode 2.0 及更高版本语义，仅 Unicode BMP
//      4           Unicode 2.0 及更高版本语义，完整 Unicode 库
//
// 使用编码 ID 0、1 或 2 已弃用。名称条目可以使用编码 ID 3 或 4。注意，其他平台 ID 定义，仅用
// 于 'cmap' 表。另请注意，当定义新的 'cmap' 子表格式时，有时会为 Unicode 平台分配新的编码 ID，
// 这些也可能适用于 'name' 表。例如，当将 'cmap' 子表格式 10 和 12 添加到规范时，也添加了编码
// ID 4。
//
// Unicode 平台没有定义平台特定语言 ID。语言 ID = 0 可用于 Unicode 平台字符串，但这不表示任何
// 特定语言。大于或等于 0x8000 的语言 ID 可以与语言标签记录一起使用，如上所述。Unicode 平台的
// 字符串必须使用 UTF-16BE 编码。
//
// Unicode 平台（平台 ID = 0）。为 Unicode 平台定义了以下编码 ID：
//      编码 ID     描述
//      0           Unicode 1.0 语义 [已弃用]
//      1           Unicode 1.1 语义 [已弃用]
//      2           ISO/IEC 10646 语义 [已弃用]
//      3           Unicode 2.0 及更高版本语义，仅 Unicode BMP
//      4           Unicode 2.0 及更高版本语义，完整 Unicode 库
//      5           Unicode 变体序列，用于子表格式 14
//      6           完整 Unicode 库，用于子表格式 13
//
//      * 子表格式 14 只能使用在 UNICODE 平台并使用 5 编码
//      * 子表格式 13 只能使用在 UNICODE 平台并使用 6 编码
//
// 编码 ID 3 应与 'cmap' 子表格式 4 或 6 结合使用。编码 ID 4 应与子表格式 10 或 12 结合
// 使用。字体支持的 Unicode 变体序列应在 'cmap' 表中使用格式 14 子表指定。格式 14 子表只
// 能用于平台 ID 0 和编码 ID 5，编码 ID 5 只能与格式 14 子表一起使用。编码 ID 6 只能与
// 'cmap' 子表格式 13 结合使用，子表格式 13 只能用于平台 ID 0 和编码 ID 6。

#define prh_otf_unicode_1_0_deprecated                         0 // 已废弃
#define prh_otf_unicode_1_1_deprecated                         1 // 已废弃
#define prh_otf_unicode_iso_deprecated                         2 // 已废弃
#define prh_otf_unicode_bmp_only                               3
#define prh_otf_unicode_full_repertoire                        4
#define prh_otf_unicode_variation_sequences_for_format_14      5
#define prh_otf_unicode_full_repertoire_for_format_13          6

// Macintosh 平台（平台 ID = 1），Macintosh 编码 ID（脚本管理器代码，script manager codes）为
// Macintosh 平台定义了以下编码 ID。Macintosh 平台（平台 ID 1）的字符串根据给定名称记录指定的
// 编码 ID 使用平台特定的单字节或双字节编码。
//      编码 ID     脚本                    编码 ID     脚本
//      0           罗马                    17          马拉雅拉姆（Malayalam）
//      1           日语                    18          僧伽罗（Sinhalese）
//      2           中文（繁体）            19          缅甸（Burmese）
//      3           韩语                    20          高棉（Khmer）
//      4           阿拉伯语                21          泰语（Thai）
//      5           希伯来语                22          老挝（Laotian）
//      6           希腊语                  23          格鲁吉亚（Gerogian）
//      7           俄语                    24          亚美尼亚（Armenian）
//      8           RSymbol                 25          中文（简体）
//      9           天城文（Devanagari）    26          藏语
//      10          古尔穆基（Gurmukhi）    27          蒙古
//      11          古吉拉特（Gujarati）    28          吉兹
//      12          奥里亚（Odia）          29          斯拉夫
//      13          孟加拉（Bangla）        30          越南
//      14          泰米尔（Tamil）         31          Sindhi
//      15          泰卢固（Telugu）        32          Uninterpreted
//      16          卡纳达（Kannada）
//
// Macintosh 语言 ID。有关 Macintosh 平台特定语言 ID 的信息，请参阅 Apple 的 TrueType 参考
// 手册。https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6name.html
//
// Macintosh 平台（平台 ID = 1）。旧版 Macintosh 版本要求字体具有平台 ID 1 的 'cmap' 子表。
// 对于当前 Apple 平台，不鼓励使用平台 ID 1。有关为 Macintosh 平台定义的编码 ID 的详细信息，
// 请参阅 'name' 表章节。

#define prh_otf_macintosh_roman 0
#define prh_otf_macintosh_japanese 1
#define prh_otf_macintosh_chinese_traditional 2
#define prh_otf_macintosh_korean 3
#define prh_otf_macintosh_arabic 4
#define prh_otf_macintosh_hebrew 5
#define prh_otf_macintosh_greek 6
#define prh_otf_macintosh_russian 7
#define prh_otf_macintosh_rsymbol 8
#define prh_otf_macintosh_devanagari 9
#define prh_otf_macintosh_gurmukhi 10
#define prh_otf_macintosh_gujarati 11
#define prh_otf_macintosh_odia 12
#define prh_otf_macintosh_bangla 13
#define prh_otf_macintosh_tamil 14
#define prh_otf_macintosh_telugu 15
#define prh_otf_macintosh_kannada 16
#define prh_otf_macintosh_malayalam 17
#define prh_otf_macintosh_sinhalese 18
#define prh_otf_macintosh_burmese 19
#define prh_otf_macintosh_khmer 20
#define prh_otf_macintosh_thai 21
#define prh_otf_macintosh_laotian 22
#define prh_otf_macintosh_georgian 23
#define prh_otf_macintosh_armenian 24
#define prh_otf_macintosh_chinese_simplified 25
#define prh_otf_macintosh_tibetan 26
#define prh_otf_macintosh_mongolian 27
#define prh_otf_macintosh_geez 28
#define prh_otf_macintosh_slavic 29
#define prh_otf_macintosh_vietnamese 30
#define prh_otf_macintosh_sindhi 31
#define prh_otf_macintosh_uninterpreted 32

// ISO 平台（平台 ID = 2）。此平台 ID 的使用已弃用。为 ISO 平台定义了以下编码 ID：
//      代码    ISO 编码
//      0       7 位 ASCII
//      1       ISO 10646
//      2       ISO 8859-1

#define prh_otf_iso_ascii_deprecated   0
#define prh_otf_iso_10646_deprecated   1
#define prh_otf_iso_88591_deprecated   2

// Windows 平台（平台 ID = 3）。Windows 编码 ID 为 Windows 平台定义了以下编码 ID：
//      编码 ID     描述
//      0           Symbol
//      1           Unicode BMP
//      2           ShiftJIS
//      3           PRC         - code page 936
//      4           Big5        - code page 950
//      5           Wansung     - code page 949
//      6           Johab
//      7           保留
//      8           保留
//      9           保留
//      10          完整 Unicode 库
//
// 平台 3 'name' 条目的编码 ID 应与 'cmap' 表中平台 3 子表使用的编码 ID 匹配。为 Windows 构建
// Unicode 字体时，平台 ID 应为 3，编码 ID 应为 1。为 Windows 构建符号字体时，平台 ID 应为 3，
// 编码 ID 应为 0。如果字体具有编码 ID 3、4 或 5 的记录，则相应的字符串数据应分别使用代码页
// 936、950 和 949 编码。否则，平台 3 的所有字符串数据必须使用 UTF-16BE 编码。
//
// 注意：某些旧版繁体中文字体具有平台 3、编码 ID 4（Big5）的名称条目，某些字符串数据使用代码页
// 950 编码，但名称 ID 2（字体子家族）的字符串数据改为使用 UTF-16BE 编码。例如，Windows 95 繁
// 体中文版中包含的 MingLi 字体就是这种情况。某些旧版软件实现（包括 Windows GDI）允许此例外。
//
// Windows 语言 ID。有关 Windows 平台特定语言 ID 和相应 BCP 47 语言标签的信息，请参阅 [MS-LCID]：
// Windows 语言代码标识符（LCID, Language Code Identifiers）参考。
// https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-lcid/70feba9f-294e-491e-b6eb-56532684c37f
//
// Windows 平台（平台 ID = 3）。Windows 平台支持多种编码。为 Windows 创建字体时，应始终使
// 用 Unicode 'cmap' 子表，平台 3 与编码 1 或 10。Windows 平台支持以下编码 ID：
//      平台 ID     编码 ID     描述
//      3           0           Symbol
//      3           1           Unicode BMP
//      3           2           ShiftJIS
//      3           3           PRC
//      3           4           Big5
//      3           5           Wansung
//      3           6           Johab
//      3           7           保留
//      3           8           保留
//      3           9           保留
//      3           10          完整 Unicode 库
//
// 仅支持 Unicode BMP 字符（U+0000 到 U+FFFF）的 Windows 平台字体必须使用编码 1 和格式 4
// 子表。此编码不得用于支持 Unicode 辅助平面字符。
//
// 在 Windows 平台上支持 Unicode 辅助平面字符（U+10000 到 U+10FFFF）的字体必须使用编码 10
// 和格式 12 子表。
//
// 符号编码是为了支持包含 Unicode 或其他标准编码不支持的任意装饰或符号的字体而创建的。通
// 常使用格式 4 子表，最多 224 个图形字符分配在从 0xF020 开始的代码位置。这对应于 Unicode
// 私用区（PUA）的子范围，尽管这不是 Unicode 编码。在旧版使用中，某些应用程序会使用单字节
// 编码表示符号字符，然后将 0x20 映射到字体中的 OS/2.usFirstCharIndex 值。在新字体中，Unicode
// 中不存在的符号或字符应使用 Unicode 'cmap' 子表中的 PUA 代码点编码。有关更多信息，请参
// 阅建议章节。https://learn.microsoft.com/en-us/typography/opentype/spec/recom#cmap-table

#define prh_otf_windows_symbol             0
#define prh_otf_windows_bmp_only           1
#define prh_otf_windows_shiftjis           2
#define prh_otf_windows_prc                3
#define prh_otf_windows_big5               4
#define prh_otf_windows_wansung            5
#define prh_otf_windows_johab              6
#define prh_otf_windows_reserved_1         7
#define prh_otf_windows_reserved_2         8
#define prh_otf_windows_reserved_3         9
#define prh_otf_windows_full_repertoire    10

// 自定义平台（平台 ID = 4）和 OTF Windows NT 兼容性映射。平台 ID 4 是一个遗留平台，创
// 建用于为已适应旧 Type 1 字体的 OpenType 字体提供旧应用程序兼容性。此平台今天不常用，
// 不应在新字体中使用。
//      编码 ID     自定义编码
//      0-255       OTF Windows NT 兼容性映射
//
// 此 'cmap' 平台为非 Unicode 应用程序提供兼容性机制，这些应用程序使用该字体时就像它是
// Windows ANSI 编码一样。非 Windows ANSI Type 1 字体，例如 Adobe 过去发布的西里尔文和
// 中欧字体，在 .PFM 文件的 CharSet 字段中记录 "0"（Windows ANSI）；适用于 Windows 9x
// 的 Adobe Type Manager 完全忽略 CharSet。Adobe 在每个从 Type1 字体转换的 OpenType 字
// 体中提供此兼容性 'cmap' 编码，其中 Encoding 不是 StandardEncoding。
//
// 使用平台 ID 4 时，编码 ID 必须设置为原始 Type 1 字体的 .PFM 文件中存在的 Windows 字
// 符集值（范围为 0 到 255，含）。
//
// 如果平台 ID 4、编码 ID 0 – 255 的 'cmap' 编码存在于具有 CFF 轮廓的 OpenType 字体中，
// 则 Windows NT 中的 OTF 字体驱动程序将执行以下流程。注意，'cmap' 子表需要对其子表使用
// 格式 0 或 6，并且编码需要与 CFF 的编码相同。
//  a)  将编码中字符代码 0-255 的字形叠加到其向系统报告的 Unicode 编码中的相应 Windows
//      ANSI（代码页 1252）Unicode 值上
//  b)  将 Windows ANSI（CharSet 0）添加到字体支持的 CharSet 列表中，以及
//  c)  将编码 ID 的值视为 Windows CharSet 值，并将其添加到字体支持的 CharSet 列表中

const void *prh_impl_font_get_platform_string(prh_r32 platform_id) {
    const char *platform[] = {"Unicode", "Macintosh", "ISO [deprecated]", "Windows", "Custom"};
    if (platform_id <= 4) return platform[platform_id];
    if (platform_id >= 240 && platform_id <= 255) return "User Defined";
    return "Unknown Platform ID";
}

const void *prh_impl_font_get_encoding_string(prh_r32 platform_id, prh_r32 encoding_id) {
    if (platform_id == 0) {
        const char *unicode_encoding[] = {
            "Unicode 1.0 [deprecated]",
            "Unicode 1.1 [deprecated]",
            "ISO/IEC 10646 [deprecated]",
            "Unicode 2.0+ BMP only",
            "Unicode 2.0+ full repertoire",
            "Unicode variation sequences - for cmap format 14",
            "Unicode full repertoire - for cmap format 13"};
        if (encoding_id <= 6) return unicode_encoding[encoding_id];
    } else if (platform_id == 1) {
        const char *macintosh_encoding[] = {
            "Roman", "Japanese", "Chinese Traditional", "Korean", "Arabic", "Hebrew", "Greek", "Russian", "RSymbol", "Devanagari", "Gurmukhi", "Gujarati", "Odia", "Bangla", "Tamil", "Telugu", "Kannada",
            "Malayalam", "Sinhalese", "Burmese", "Khmer", "Thai", "Laotian", "Georgian", "Armenian", "Chinese Simplified", "Tibetan", "Mongolian", "Geez", "Slavic", "Vietnamese", "Sindhi", "Uninterpreted"};
        if (encoding_id <= 32) return macintosh_encoding[encoding_id];
    } else if (platform_id == 2) {
        const char *iso_encoding[] = {"7-bit ASCII", "ISO 10646", "ISO 8859-1"};
        if (encoding_id <= 2) return iso_encoding[encoding_id];
    } else if (platform_id == 3) {
        const char *windows_encoding[] = {"Symbol", "Unicode BMP", "ShiftJIS", "PRC", "Big5", "Wansung", "Johab", "Reserved", "Reserved", "Reserved", "Unicode full repertoire"};
        if (encoding_id <= 10) return windows_encoding[encoding_id];
    } else if (platform_id == 4) {
        if (encoding_id <= 255) return "OTF Windows NT compatibility mapping";
    }
    return "Unknown Encoding ID";
}

// uint16      version             表版本号（=0）
// uint16      count               名称记录的数量
// Offset16    storageOffset       字符串存储起始偏移（从表开头开始）
// NameRecord  nameRecord[count]   名称记录，其中 count 是记录数
// Variable                        实际字符串数据的存储
//
// uint16      platformID      平台 ID
// uint16      encodingID      平台特定编码 ID
// uint16      languageID      语言 ID
// uint16      nameID          名称 ID
// uint16      length          字符串长度（以字节为单位）
// Offset16    stringOffset    从存储区开头开始的字符串偏移（以字节为单位）
//
// uint16          version                     表版本号（=1）
// uint16          count                       名称记录的数量
// Offset16        storageOffset               字符串存储起始偏移（从表开头开始）
// NameRecord      nameRecord[count]           名称记录，其中 count 是记录数
// uint16          langTagCount                语言标签记录的数量
// LangTagRecord   langTagRecord[langTagCount] 语言标签记录，其中 langTagCount 是记录数
// Variable                                    实际字符串数据的存储
//
// 类型        名称            描述
// uint16      length          语言标签字符串长度（以字节为单位）
// Offset16    langTagOffset   从存储区开头开始的语言标签字符串偏移（以字节为单位）

typedef struct {
    prh_r16 version;
    prh_r16 name_count;
    prh_r16 string_start;
} prh_font_name_table;

typedef struct {
    prh_r16 platform_id; // 名称记录必须首先按平台 ID 排序，然后按平台特定编码 ID 排序，然后按语言 ID 排序，最后按名称 ID 排序
    prh_r16 encoding_id;
    prh_r16 language_id; // < 0x8000 特定平台语言标签，>= 0x8000 平台无关语言标签
    prh_r16 name_id;
    prh_r16 string_length;
    prh_r16 string_offset;
} prh_font_name_record;

typedef struct { // 语言标签字符串必须使用 UTF-16BE 编码，并非所有平台都有平台特定语言 ID，并非所有平台都支持语言标签记录
    prh_r16 lang_tag_length;
    prh_r16 lang_tag_offset;
} prh_font_name_lang_tag;

void prh_print_font_name_table(prh_open_font *f) {
    if (f->name.length == 0) prh_abort_line();
    prh_byte *table_data = prh_load_font_table(f, &f->name);
    prh_r16 table_version = prh_bp_2b_to_host(table_data);
    prh_r16 name_count = prh_bp_2b_to_host(table_data + 2);
    prh_r16 string_start = prh_bp_2b_to_host(table_data + 4);

    prh_byte *name_record = table_data + 6;
    prh_byte *lang_tag_count = name_record + name_count * sizeof(prh_font_name_record);
    prh_byte *lang_tag_record = lang_tag_count + 2;
    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)table_data, f->name.length);
    if (table_version == 0) {
        prh_real_assert(lang_tag_count - table_data <= string_start);
        prh_real_assert(string_start <= f->name.length);
    } else if (table_version == 1) {
        prh_r16 tag_count = prh_bp_2b_to_host(lang_tag_count);
        prh_real_assert(lang_tag_record + tag_count * sizeof(prh_font_name_lang_tag) - table_data <= string_start);
        prh_real_assert(string_start <= f->name.length);
    } else {
        prh_abort_error(table_version);
    }

    prh_print(
        "name table index %d / %d\n"
        "name table tag 0x%08x (%c%c%c%c)\n"
        "name table offset %.10d (%d/4)\n"
        "name table length %.10d (%d/4)\n"
        "name table checksum 0x%08x 0x%08x (valid %d)\n"
        "name table version %d\n"
        "name table name count %d\n"
        "name table lang tags %d\n"
        "name table string start %d\n",
        (prh_reg)f->name.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->name.tabletag,
        (prh_reg)prh_byte_4(f->name.tabletag),
        (prh_reg)prh_byte_3(f->name.tabletag),
        (prh_reg)prh_byte_2(f->name.tabletag),
        (prh_reg)prh_byte_1(f->name.tabletag),
        (prh_reg)f->name.offset, (prh_reg)f->name.offset % 4,
        (prh_reg)f->name.length, (prh_reg)f->name.length % 4,
        (prh_reg)f->name.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->name.checksum),
        (prh_reg)table_version,
        (prh_reg)name_count,
        (prh_reg)(table_version == 1 ? prh_bp_2b_to_host(lang_tag_count) : 0),
        (prh_reg)string_start);

    prh_r32 count = name_count > 20 ? 20 : name_count;
    for (prh_r32 i = 0; i < count; i += 1) {
        prh_r16 platform_id = prh_bp_2b_to_host(name_record + i * sizeof(prh_font_name_record));
        prh_r16 encoding_id = prh_bp_2b_to_host(name_record + i * sizeof(prh_font_name_record) + 2);
        prh_r16 language_id = prh_bp_2b_to_host(name_record + i * sizeof(prh_font_name_record) + 4);
        prh_r16 name_id = prh_bp_2b_to_host(name_record + i * sizeof(prh_font_name_record) + 6);
        prh_r16 string_length = prh_bp_2b_to_host(name_record + i * sizeof(prh_font_name_record) + 8);
        prh_r16 string_offset = prh_bp_2b_to_host(name_record + i * sizeof(prh_font_name_record) + 10);
        prh_print(
            "----------------------------\n"
            "name record index %d / %d\n"
            "name record platform %d '%s'\n"
            "name record encoding %d '%s'\n"
            "name record language %04x\n"
            "name record string length %d\n"
            "name record string offset %d\n"
            "name record name_id [%d] %s: %Ls\n",
            (prh_reg)(i + 1),
            (prh_reg)name_count,
            (prh_reg)platform_id, prh_impl_font_get_platform_string(platform_id),
            (prh_reg)encoding_id, prh_impl_font_get_encoding_string(platform_id, encoding_id),
            (prh_reg)language_id,
            (prh_reg)string_length,
            (prh_reg)(string_start + string_offset),
            (prh_reg)name_id, prh_impl_font_get_name_type_string(name_id),
            (prh_reg)string_length, table_data + string_start + string_offset);
    }
    prh_print("\n");
    prh_da_free(table_data);
}

// 字符到字形索引映射表（cmap）
//
// 表概述。本表定义字符代码到默认字形索引的映射。可以定义不同的子表，每个子表包含不同
// 字符编码方案的映射。表头指示存在哪些编码的子表。无论编码方案如何，不对应字体中任何
// 字形的字符代码应映射到字形索引 0。此位置的字形必须是代表缺失字符的特殊字形，通常称
// 为 .notdef。
//
// 每个子表采用七种可能格式之一，并以指示所用格式的格式字段开头。前四种格式（格式 0、2、
// 4 和 6）最初在 Unicode 2.0 之前定义。这些格式允许 8 位单字节、8 位多字节和 16 位编码。
// 随着 Unicode 2.0 中辅助平面的引入，Unicode 可寻址代码空间扩展到 16 位以上。为适应这一
// 点，添加了三种附加格式（格式 8、10 和 12）允许 32 位编码方案。
//
// Unicode 的其他增强功能导致了其他子表格式的添加。子表格式 13 允许将许多字符高效映射到
// 单个字形；这对于"最后手段"字体很有用，这些字体为所有可能的 Unicode 字符提供回退渲染，
// 并为不同的 Unicode 范围提供不同的回退字形。子表格式 14 提供支持 Unicode 变体序列的统
// 一机制。
//
// 在七种可用格式中，并非所有格式今天都常用。格式 4 或 12 适用于大多数新字体，取决于支持
// 的 Unicode 字符库。格式 14 在许多应用程序中用于支持 Unicode 变体序列。某些平台还使用
// 格式 13 作为最后手段回退字体。其他子表格式不建议用于新字体。然而，应用程序开发人员应
// 预期字体中可能使用任何格式。注意，使用较新子表格式的字体的 'cmap' 表版本号保持为 0x0000。
//
// 'cmap' 表头。字符到字形索引映射表的组织结构如下。
//      类型    名称        描述
//      uint16  version     表版本号（0）
//      uint16  numTables   后续编码表的数量
//      EncodingRecord encodingRecords[numTables]
//
// 编码记录和编码。编码记录（EncodingRecord）数组指定特定编码以及每个编码的子表偏移。
//      类型        名称            描述
//      uint16      platformID      平台 ID
//      uint16      encodingID      平台特定编码 ID
//      Offset32    subtableOffset  从表开头到此编码子表的字节偏移
//
// 编码记录中的平台 ID 和平台特定编码 ID 用于指定特定字符编码。对于 Macintosh 平台，映射
// 子表中的语言字段也用于此目的。'cmap' 表头中的编码记录条目必须首先按平台 ID 排序，然后
// 按平台特定编码 ID 排序，然后按相应子表中的语言字段排序。每个平台 ID、平台特定编码 ID
// 和子表语言组合在 'cmap' 表中只能出现一次。
//
// 除格式 14 子表外，所有其他子表都是互斥的：应用程序应选择并使用其中一个，而忽略其他子表。
// 如果使用 Unicode 子表（平台 0，或平台 3 编码 1 或 10），则还可以补充使用平台 0 编码 5
// 的格式 14 子表来映射 Unicode 变体序列。
//
// 如果字体同时包含 16 位编码（通常是格式 4）和 32 位编码（格式 10 或 12）的 Unicode 子表，
// 则 32 位编码子表支持的字符应是 16 位编码子表支持字符的超集，应用程序应使用 32 位编码。
// 字体不应包含同时使用格式 4 和格式 6 的 16 位 Unicode 子表，应使用格式 4。同样，字体不应
// 包含同时使用格式 10 和格式 12 的 32 位 Unicode 子表，应使用格式 12。
//
// 如果字体包含相同格式但不同平台 ID 的 Unicode 子表的编码记录，应用程序可以选择合适的那个，
// 但应在每次使用字体时一致地进行此选择。

typedef struct {
    prh_r16 platform_id; // 编码记录必需先按平台 ID 排序，然后按编码 ID 排序，然后按记录中的语言字段（language field）排序
    prh_r16 encoding_id; // 每组（平台ID + 编码ID + 语言）的记录只能唯一有一个
    prh_r32 record_offset; // 基于 cmap 表头的字节偏移
} prh_font_cmap_header;

typedef struct {
    prh_r16 version;
    prh_r16 numrecords;
    prh_font_cmap_header header[1];
} prh_font_cmap_table;

prh_r16 prh_font_find_table(prh_open_font *f, prh_r32 table_tag) { // 返回 0 表示失败
    prh_r16 numtables = prh_font_table_count(f);
    for (prh_r16 i = 1; i <= numtables; i += 1) {
        prh_font_thead *table = f->table_header + i;
        if (prh_r32_be_to_host(table->tabletag) == table_tag) return i;
    }
    return 0;
}

typedef struct {
    prh_open_font *font;
    prh_byte *table_data;
    prh_r16 table_index;
    prh_r16 record_index;
    prh_r32 table_offset;
    prh_r32 table_length;
    prh_r16 cmap_version;
    prh_r16 cmap_records;
    prh_r16 record_platform; // 编码记录必需先按平台 ID 排序，然后按编码 ID 排序，然后按记录中的语言字段（language field）排序
    prh_r16 record_encoding; // 每组（平台ID + 编码ID + 语言）的记录只能唯一有一个
    prh_r32 record_offset;
} prh_font_cmap_record;

void prh_impl_print_font_cmap_format_4(prh_font_cmap_record *p);
void prh_impl_print_font_cmap_format_12_13(prh_font_cmap_record *p);
void prh_impl_print_font_cmap_format_14(prh_font_cmap_record *p);

void prh_impl_print_font_cmap_record(prh_font_cmap_record *p) {
    prh_byte *record_data = p->table_data + p->record_offset;
    prh_r16 record_format = prh_bp_2b_to_host(record_data);

    prh_print(
        "----------------------------\n"
        "cmap record index %d / %d\n"
        "cmap record platform %d %s\n"
        "cmap record encoding %d %s\n"
        "cmap record offset %.10d\n"
        "cmap record format %d\n",
        (prh_reg)p->record_index + 1,
        (prh_reg)p->cmap_records,
        (prh_reg)p->record_platform, prh_impl_font_get_platform_string(p->record_platform),
        (prh_reg)p->record_encoding, prh_impl_font_get_encoding_string(p->record_platform, p->record_encoding),
        (prh_reg)p->record_offset,
        (prh_reg)record_format);

    if (record_format == 4) {
        prh_impl_print_font_cmap_format_4(p);
    } else if (record_format == 12 || record_format == 13) {
        prh_impl_print_font_cmap_format_12_13(p);
    } else if (record_format == 14) {
        prh_impl_print_font_cmap_format_14(p);
    }
}

void prh_print_font_cmap(prh_open_font *f) {
    if (f->cmap.length == 0) prh_abort_line();
    prh_byte *table_data = prh_load_font_table(f, &f->cmap);
    prh_font_cmap_table *cmap = (prh_font_cmap_table *)table_data;
    prh_set_r16_be_to_host(cmap->version);
    prh_set_r16_be_to_host(cmap->numrecords);

    if (f->cmap.length < 4 + sizeof(prh_font_cmap_header) * cmap->numrecords) {
        prh_print("table 'cmap' invalid length %d numrecord %d\n", (prh_reg)f->cmap.length, (prh_reg)cmap->numrecords);
        return;
    }

    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)table_data, f->cmap.length);
    prh_print(
        "cmap table index %d / %d\n"
        "cmap table tag 0x%08x (%c%c%c%c)\n"
        "cmap table offset %.10d (%d/4)\n"
        "cmap table length %.10d (%d/4)\n"
        "cmap table checksum 0x%08x 0x%08x (valid %d)\n"
        "cmap table version %04x\n",
        (prh_reg)f->cmap.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->cmap.tabletag,
        (prh_reg)prh_byte_4(f->cmap.tabletag),
        (prh_reg)prh_byte_3(f->cmap.tabletag),
        (prh_reg)prh_byte_2(f->cmap.tabletag),
        (prh_reg)prh_byte_1(f->cmap.tabletag),
        (prh_reg)f->cmap.offset, (prh_reg)f->cmap.offset % 4,
        (prh_reg)f->cmap.length, (prh_reg)f->cmap.length % 4,
        (prh_reg)f->cmap.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->cmap.checksum),
        (prh_reg)cmap->version);

    prh_font_cmap_record record;
    for (int i = 0; i < cmap->numrecords; i += 1) {
        prh_font_cmap_header *header = cmap->header + i;
        record.font = f;
        record.table_index = f->cmap.table_index;
        record.record_index = i;
        record.table_offset = f->cmap.offset;
        record.table_length = f->cmap.length;
        record.table_data = table_data;
        record.cmap_version = cmap->version;
        record.cmap_records = cmap->numrecords;
        record.record_platform = prh_r16_be_to_host(header->platform_id);
        record.record_encoding = prh_r16_be_to_host(header->encoding_id);
        record.record_offset = prh_r32_be_to_host(header->record_offset);
        prh_impl_print_font_cmap_record(&record);
    }

    prh_print("\n");
    prh_da_free(table_data);
}

// 'cmap' 子表格式
//
// 'cmap' 子表中语言字段的使用。所有 'cmap' 子表格式都包含语言字段。对于平台 ID 不是 Macintosh
// （平台 ID 1）的所有 'cmap' 子表，语言字段必须设置为零。对于平台 ID 为 Macintosh 的
// 'cmap' 子表，将此字段设置为 'cmap' 子表的 Macintosh 语言 ID 加一，或者如果 'cmap'
// 子表不是语言特定的，则设置为零。例如，Mac OS 土耳其语 'cmap' 子表必须将此字段设置为
// 18，因为土耳其语的 Macintosh 语言 ID 为 17。Mac OS 罗马语 'cmap' 子表必须将此字段设
// 置为 0，因为 Mac OS 罗马语不是语言特定的编码。
//
// 格式 0：字节编码表（Byte Encoding Table）
//
// 格式 0 是旧版 Macintosh 平台上使用的标准映射子表，但新版 Apple 平台不需要。'cmap' 子
// 表格式 0：
//      类型    名称                描述
//      uint16  format              格式号设置为 0
//      uint16  length              子表的字节长度
//      uint16  language            特定语言
//      uint8   glyphIdArray[256]   将字符代码映射到字形索引值的数组
//
// 这是字符代码到字形索引的简单 1 对 1 映射。字形集限制为 256。如果此格式用于索引到更大
// 的字形集，则只能访问前 256 个字形。

typedef struct {
    prh_r16 format;
    prh_r16 length;
    prh_r16 language;
    prh_r08 glyph_id[256];
} prh_font_cmap_format_0;

prh_r32 prh_font_glyph_index_from_cmap_format_0(prh_font_cmap_format_0 *f, prh_r32 char_code) {
    if (char_code < (prh_r32)(f->length - 6)) return f->glyph_id[char_code];
    return 0;
}

// 格式 2：高字节映射表（High Byte Mapping Through Table）
//
// 此子表格式是为遵循日本、中国和韩国字符使用的国家字符代码标准的"双字节"编码创建的。
// 这些代码标准使用混合 8/16 位编码。此格式今天不常用。在这些混合 8/16 位编码中，某些
// 字节值表示双字节字符的第一个字节。这些字节值作为双字节字符的第二个字节也是有效的。
// 此外，即使是双字节字符，字符代码到字形索引值的映射也很大程度上取决于第一个字节。因
// 此，表以将第一个字节映射到 SubHeader 记录的数组开头。对于双字节字符代码，SubHeader
// 用于将第二个字节的值映射到字形索引数组的子范围（子数组），如下所述。处理混合 8/16
// 位文本时，SubHeader 0 是特殊的：它用于单字节字符代码。使用 SubHeader 0 时，不需要第
// 二个字节；单字节值通过指定的子数组映射。
//
// 'cmap' 子表格式 2：
//      类型        名称                描述
//      uint16      format              格式号设置为 2
//      uint16      length              子表的字节长度
//      uint16      language            特定语言
//      uint16      subHeaderKeys[256]  将高字节映射到 subHeaders 数组的数组，值为 subHeaders 索引 × 8
//      SubHeader   subHeaders[ ]       可变长度 SubHeader 记录数组
//      uint16      glyphIdArray[ ]     可变长度数组，包含用于映射双字节字符低字节的子数组，数组长度为各 entryCount 之和
//
// SubHeader 记录的结构如下：
//      类型    名称            描述
//      uint16  firstCode       此 SubHeader 的第一个有效低字节
//      uint16  entryCount      此 SubHeader 中有效低字节的个数
//      int16   idDelta         请参阅下文
//      uint16  idRangeOffset   请参阅下文
//
// firstCode 和 entryCount 值指定一个从 firstCode 开始、长度等于 entryCount 值的子范围。
// 此子范围保持在被映射字节的 0-255 范围内。此子范围外的字节映射到字形索引 0（缺失字形）。
// 字节在此子范围内的偏移量然后用作 glyphIdArray 相应子数组的索引。此子数组的长度也是
// entryCount。idRangeOffset 的值是 idRangeOffset 字段实际位置之后的字节数，其中对应于
// firstCode 的 glyphIdArray 元素。
//
// 最后，如果从子数组获得的值不为 0（该值表示缺失字形），则应将 idDelta 添加到其中以获
// 得 glyphIndex。idDelta 值允许相同的子数组用于几个不同的子表头。idDelta 算术是模 65536
// 的。如果在添加 idDelta 到子数组值后的结果小于零，则添加 65536 以获得有效的字形 ID。

typedef struct {
    prh_r16 format;
    prh_r16 length;
    prh_r16 language;
    prh_r16 sub_header_offset[256]; // 相当于将字符分成了 256 组，每一组中的字符最多 256 个，字体实现的字符在范围 [first_code, first_code + entry_count) 内
} prh_font_cmap_format_2;

typedef struct {
    prh_r16 first_code;
    prh_r16 entry_count;
    prh_i16 glyph_id_delta;
    prh_r16 glyph_id_offset;
} prh_font_cmap_sub_header;

prh_r32 prh_font_glyph_index_from_cmap_format_2(prh_font_cmap_record *p, prh_r16 char_code) {
    prh_font_cmap_format_2 *record = (prh_font_cmap_format_2 *)(p->table_data + p->record_offset);
    prh_r16 format = prh_bp_2b_to_host((prh_byte *)&record->format);
    prh_r16 length = prh_bp_2b_to_host((prh_byte *)&record->length);
    prh_r16 sub_header_offset = prh_bp_2b_to_host((prh_byte *)(record->sub_header_offset + prh_byte_2(char_code)));
    prh_assert(format == 2 && sub_header_offset % sizeof(prh_font_cmap_sub_header) == 0);
    prh_assert(length > sizeof(prh_font_cmap_format_2) && (prh_byte *)record + length <= p->table_data + p->table_length);

    prh_font_cmap_sub_header *sub_header = (prh_font_cmap_sub_header *)((prh_byte *)(record + 1) + sub_header_offset);
    prh_assert((prh_byte *)sub_header + sizeof(prh_font_cmap_sub_header) <= (prh_byte *)record + length);
    prh_r16 first_code = prh_bp_2b_to_host((prh_byte *)&sub_header->first_code);
    prh_byte low_byte = prh_byte_1(char_code); prh_r32 i = low_byte - first_code;
    if (low_byte < first_code || i >= prh_bp_2b_to_host((prh_byte *)&sub_header->entry_count)) return 0;

    prh_r16 glyph_id_offset = prh_bp_2b_to_host((prh_byte *)&sub_header->glyph_id_offset);
    prh_r16 *ptr_glyph_index = (prh_r16 *)((prh_byte *)&sub_header->glyph_id_offset + glyph_id_offset) + i;
    prh_assert((prh_byte *)ptr_glyph_index < (prh_byte *)record + length);
    prh_r16 glyph_index = prh_bp_2b_to_host((prh_byte *)ptr_glyph_index);
    return glyph_index == 0 ? 0 : (prh_r32)(prh_r16)((prh_i16)glyph_index + (prh_i16)prh_bp_2b_to_host((prh_byte *)&sub_header->glyph_id_delta));
}

// 格式 4：段映射到增量值（Segment Mapping to Delta Values）
//
// 这是仅支持 Unicode 基本多文种平面字符（U+0000 到 U+FFFF）的字体的标准字符到字形索引
// 映射子表。注意：要支持 Unicode 辅助平面字符，应使用格式 12。
//
// 当字体表示的字符的字符代码落入多个连续范围时，使用此格式，可能在某些或所有范围中有
// 空洞（即，某些范围中的代码可能没有在字体中的表示）。格式相关数据分为三部分，必须按
// 以下顺序出现：
//  1.  四字（four-word）头部给出段列表优化搜索的参数
//  2.  四个并行数组描述段（每个连续代码范围一个段）
//  3.  可变长度的字形 ID 数组（无符号字）
//
// 'cmap' 子表格式 4：
//      类型    名称                        描述
//      uint16  format                      格式号设置为 4
//      uint16  length                      子表的字节长度
//      uint16  language                    特定语言
//      uint16  segCountX2                  2 × segCount
//      uint16  searchRange                 小于或等于 segCount 的 2 的最大幂乘以 2，2**floor(log2(segCount)) * 2，其中**是幂运算符
//      uint16  entrySelector               小于或等于 segCount 的 2 的最大幂的 Log2，log2(searchRange/2)，等于 floor(log2(segCount))
//      uint16  rangeShift                  segCount 乘以 2 减去 searchRange，(segCount * 2) - searchRange
//      uint16  endCode[segCount]           每个段的结束字符代码，最后一个=0xFFFF
//      uint16  reservedPad                 设置为 0
//      uint16  startCode[segCount]         每个段的起始字符代码
//      int16   idDelta[segCount]           段中所有字符代码的增量
//      uint16  idRangeOffset[segCount]     glyphIdArray 的偏移量或 0
//      uint16  glyphIdArray[ ]             任意长度的字形索引数组
//
// 段数由 segCount 指定，segCount 不在头部直接给出，但可以从 segCountX2 轻松推导。所有其他
// 头部参数都从中推导。searchRange 值是小于或等于 segCount 的 2 的最大幂的两倍。例如，如果
// segCount=39，我们有以下：
//      segCountX2      78
//      searchRange     64 (= 2 × 小于等于39的最大2的幂)
//      entrySelector   5 (= log232)
//      rangeShift      14 (= 2 × 39 - 64)
//
// 为了协助快速二分搜索，searchRange、entrySelector 和 rangeShift 字段作为参数包含在内，
// 可用于配置搜索算法。特别是，当条目数为 2 的幂时，二分搜索是最优的。searchRange 字段
// 提供可以用该约束搜索的最大项目数（2 的最大幂）。rangeShift 字段提供还需要搜索的剩余
// 项目数。entrySelector 字段指示需要进入二叉树的最大层数。在具有有限硬件功能的设备上的
// 早期实现中，searchRange、entrySelector 和 rangeShift 字段提供的优化非常重要。它们在
// 现代设备上的重要性较低，但仍可能在某些实现中使用。然而，不正确的值可能被用作对某些实
// 现的攻击向量。由于这些值可以在解析文件时从 segCountX2 字段推导，强烈建议解析实现不依
// 赖字体中的 searchRange、entrySelector 和 rangeShift 字段，而是从 segCountX2 独立推导
// 它们。然而，字体文件应继续为这些字段提供有效值，以维持与所有现有实现的兼容性。
//
// 每个段由 startCode 和 endCode 描述，以及用于映射段中字符代码的 idDelta 和 idRangeOffset。
// 段按 endCode 值递增顺序排序，段值在四个并行数组中指定。搜索第一个大于或等于要映射的字
// 符代码的 endCode。如果相应的 startCode 小于或等于字符代码，则使用相应的 idDelta 和
// idRangeOffset 将字符代码映射到字形索引（否则，返回 missingGlyph）。为了使搜索终止，
// 最终的 startCode 和 endCode 值必须为 0xFFFF。此段不需要包含任何有效映射。它可以将单
// 个字符代码 0xFFFF 映射到 missingGlyph。但是，该段必须存在。
//
// 如果段的 idRangeOffset 值不为 0，则字符代码的映射依赖于 glyphIdArray。从 startCode 的
// 字符代码偏移量添加到 idRangeOffset 值。此总和用作从 idRangeOffset 本身当前位置开始的偏
// 移量，以索引出正确的 glyphIdArray 值。这种晦涩的索引技巧有效，因为 glyphIdArray 在字体
// 文件中紧随 idRangeOffset 之后。产生字形索引的 C 表达式为：
//      glyphId = *(idRangeOffset[i]/2 + (c - startCode[i]) + &idRangeOffset[i])
//
// 值 c 是所讨论的字符代码，i 是 c 出现的段索引。如果从索引操作获得的值不是 0（表示 missingGlyph），
// 则将 idDelta[i] 添加到其中以获得字形索引。idDelta 算术是模 65536 的。如果 idRangeOffset
// 为 0，则直接将 idDelta 值添加到字符代码偏移量（即 idDelta[i] + c）以获得相应的字形索引。
// 同样，idDelta 算术是模 65536 的。如果在添加 idDelta[i] + c 后的结果小于零，则添加 65536
// 以获得有效的字形 ID。
//
// 例如，将字符 10-20、30-90 和 153-480 映射到连续字形索引范围的表的变体部分可能如下所示：
//      segCountX2:     8
//      searchRange:    8
//      entrySelector:  2
//      rangeShift:     0
//      endCode:        20 90 480 0xffff
//      reservedPad:    0
//      startCode:      10 30 153 0xffff
//      idDelta:        -9 -18 -80 1
//      idRangeOffset:  0 0 0 0
//
// 此表产生以下映射，注意，增量值可以重新排列以重新排序段。
//      10 ⇒ 10 - 9 = 1
//      20 ⇒ 20 - 9 = 11
//      30 ⇒ 30 - 18 = 12
//      90 ⇒ 90 - 18 = 72
//      153 ⇒ 153 - 80 = 73
//      480 ⇒ 480 - 80 = 400
//      0xffff ⇒ 0

#define prh_impl_font_cmap_format_4_end_code(ptr_end_code) prh_bp_2b_to_host((prh_byte *)(ptr_end_code))

prh_r32 prh_font_glyph_index_from_cmap_format_4(prh_font_cmap_record *p, prh_r32 char_code) {
    prh_byte *record = p->table_data + p->record_offset;
    if (char_code > 0xFFFF) return 0;

    prh_r16 format = prh_bp_2b_to_host(record);
    prh_r16 length = prh_bp_2b_to_host(record + 2);
    prh_r16 num_segments = prh_bp_2b_to_host(record + 6) >> 1;
    prh_assert(format == 4);

    prh_byte *ptr_end_code = record + 14;
    prh_byte *ptr_start_code = ptr_end_code + 2 + 2 * num_segments;
    prh_byte *ptr_glyph_id_delta = ptr_start_code + 2 * num_segments;
    prh_byte *ptr_glyph_id_offset = ptr_glyph_id_delta + 2 * num_segments;
    prh_assert(ptr_end_code + 2 + num_segments * 8 <= record + length && record + length <= p->table_data + p->table_length);

    prh_reg i;
    prh_generic_bsearch_first_less_equal(i, (prh_r16)char_code, (prh_r16 *)ptr_end_code, num_segments, prh_impl_font_cmap_format_4_end_code);
    prh_r16 start_code = prh_bp_2b_to_host(ptr_start_code + 2 * i);
    if (char_code < start_code) return 0;

    prh_r16 glyph_id_offset = prh_bp_2b_to_host(ptr_glyph_id_offset + 2 * i);
    if (glyph_id_offset == 0) return (prh_r32)(prh_r16)((prh_i16)char_code + (prh_i16)prh_bp_2b_to_host(ptr_glyph_id_delta + 2 * i));
    prh_byte *glyph_id = ptr_glyph_id_offset + glyph_id_offset + 2 * (char_code - start_code);
    prh_assert(glyph_id < record + length);
    return prh_bp_2b_to_host(glyph_id);
}

void prh_impl_print_font_cmap_format_4(prh_font_cmap_record *p) {
    prh_byte *record = p->table_data + p->record_offset;
    prh_r16 format = prh_bp_2b_to_host(record);
    prh_r16 length = prh_bp_2b_to_host(record + 2);
    prh_r16 language = prh_bp_2b_to_host(record + 4);
    prh_r16 num_segments = prh_bp_2b_to_host(record + 6) >> 1;
    prh_assert(format == 4);

    prh_byte *end_code = record + 14;
    prh_byte *header_end = end_code + 2 + num_segments * 8;
    prh_byte *record_end = record + length;
    if (record_end < header_end || record_end > p->table_data + p->table_length) {
        prh_print("table 'cmap' record format 4 invalid length %d %d %d with segments %d\n",
            (prh_reg)(header_end - record), (prh_reg)length, (prh_reg)(p->table_length - p->record_offset), (prh_reg)num_segments);
        return;
    }

    prh_print(
        "----------------------------\n"
        "cmap record format 4 length %d\n"
        "cmap record format 4 language %04x\n"
        "cmap record format 4 segments %d\n",
        (prh_reg)length,
        (prh_reg)language,
        (prh_reg)num_segments);

    prh_r32 print_count = 8;
    prh_r32 seg_count = num_segments > print_count ? print_count : num_segments;
    prh_byte *start_code = end_code + 2 * num_segments + 2;
    prh_byte *glyph_id_delta = start_code + 2 * num_segments;
    prh_byte *glyph_id_offset = glyph_id_delta + 2 * num_segments;
    prh_r32 i = 0;

    for (; i < seg_count; i += 1) {
        prh_r16 start_char_code = prh_bp_2b_to_host(start_code + 2 * i);
        prh_r16 end_char_code = prh_bp_2b_to_host(end_code + 2 * i);
        prh_print(
            "cmap record format 4 segment index %d / %d\n"
            "cmap record format 4 segment range U+%04x U+%04x (%05d %05d) glyph index %d %d\n"
            "cmap record format 4 glyph id delta %d\n"
            "cmap record format 4 glyph id offset %d\n",
            (prh_reg)i + 1,
            (prh_reg)num_segments,
            (prh_reg)start_char_code,
            (prh_reg)end_char_code,
            (prh_reg)start_char_code,
            (prh_reg)end_char_code,
            (prh_reg)prh_font_glyph_index_from_cmap_format_4(p, start_char_code),
            (prh_reg)prh_font_glyph_index_from_cmap_format_4(p, end_char_code),
            (prh_reg)(prh_int)(prh_i16)prh_bp_2b_to_host(glyph_id_delta + 2 * i),
            (prh_reg)prh_bp_2b_to_host(glyph_id_offset + 2 * i));
    }

    i = num_segments > print_count ? num_segments - print_count : 0;
    for (; i < num_segments; i += 1) {
        prh_r16 start_char_code = prh_bp_2b_to_host(start_code + 2 * i);
        prh_r16 end_char_code = prh_bp_2b_to_host(end_code + 2 * i);
        prh_print(
            "cmap record format 4 segment index %d / %d\n"
            "cmap record format 4 segment range U+%04x U+%04x (%05d %05d) glyph index %d %d\n"
            "cmap record format 4 glyph id delta %d\n"
            "cmap record format 4 glyph id offset %d\n",
            (prh_reg)i + 1,
            (prh_reg)num_segments,
            (prh_reg)start_char_code,
            (prh_reg)end_char_code,
            (prh_reg)start_char_code,
            (prh_reg)end_char_code,
            (prh_reg)prh_font_glyph_index_from_cmap_format_4(p, start_char_code),
            (prh_reg)prh_font_glyph_index_from_cmap_format_4(p, end_char_code),
            (prh_reg)(prh_int)(prh_i16)prh_bp_2b_to_host(glyph_id_delta + 2 * i),
            (prh_reg)prh_bp_2b_to_host(glyph_id_offset + 2 * i));
    }
}

// 格式 6：修剪表映射（Trimmed Table Mapping）
//
// 格式 6 旨在将 16 位字符映射到字形索引，当字体的字符代码落入单个连续范围时。'cmap' 子表
// 格式 6：
//      类型    名称                        描述
//      uint16  format                      格式号设置为 6
//      uint16  length                      子表的字节长度
//      uint16  language                    特定语言
//      uint16  firstCode                   子范围的起始字符代码
//      uint16  entryCount                  子范围中的字符代码数
//      uint16  glyphIdArray[entryCount]    范围内字符代码的字形索引值数组
//
// firstCode 和 entryCount 值指定可能字符代码范围内的子范围（从 firstCode 开始，长度 =
// entryCount）。此子范围外的代码映射到字形索引 0。代码（从第一个代码开始）在此子范围内
// 的偏移量用作 glyphIdArray 的索引，后者提供字形索引值。

typedef struct {
    prh_r16 format;
    prh_r16 length;
    prh_r16 language;
    prh_r16 first_code;
    prh_r16 entry_count;
    prh_r16 glyph_id[1];
} prh_font_cmap_format_6;

prh_r32 prh_font_glyph_index_from_cmap_format_6(prh_font_cmap_format_6 *f, prh_r32 char_code) {
    if (char_code >= f->first_code) {
        prh_r32 i = char_code - f->first_code;
        if (i < f->entry_count) return f->glyph_id[i];
    }
    return 0;
}

// 格式 8：混合 16 位和 32 位覆盖（Mixed 16-bit and 32-bit Coverage）
//
// 子表格式 8 旨在支持 UTF-16 编码中的 Unicode 辅助平面字符，尽管它不常用。格式 8 类似于
// 格式 2，因为它提供混合长度字符代码。然而，它允许 16 位和 32 位字符代码，而不是 8 位和
// 16 位字符代码。
//
// 如果字体包含 Unicode 辅助平面字符（U+10000 到 U+10FFFF），则很可能它还包含 Unicode BMP
// 字符（U+0000 到 U+FFFF）。因此，需要映射 16 位和 32 位字符代码的混合。做了一个简化假设：
// 即没有 32 位字符代码与任何 16 位字符代码共享相同的前 16 位。由于 Unicode 代码空间仅扩展
// 到 U+10FFFF，潜在冲突仅存在于字符 U+0000 到 U+0010，这些是非打印控制字符。这意味着可以
// 直接通过查看 16 位值来确定特定 16 位值是独立字符代码还是 32 位字符代码的开始，无需进一
// 步信息。
//
// 'cmap' 子表格式 8：
//      类型                名称                描述
//      uint16              format              子表格式，设置为 8
//      uint16              reserved            保留，设置为 0
//      uint32              length              此子表的字节长度（包括头部）
//      uint32              language            特定语言
//      uint8               is32[8192]          紧密打包的位数组（总共 8K 字节），指示特定 16 位（索引）值是否是 32 位字符代码的开始
//      uint32              numGroups           后续分组的数量
//      SequentialMapGroup  groups[numGroups]   SequentialMapGroup 记录数组
//
// 每个顺序映射组记录指定一个字符范围以及从第一个字符映射的起始字形 ID。后续字符的字形
// ID 按顺序排列。SequentialMapGroup 记录：
//      类型    名称            描述
//      uint32  startCharCode   此组中的第一个字符代码，注意，如果此组用于一个或多个 16 位字符代码（由 is32 数组确定），则此 32 位值的高 16 位设置为零
//      uint32  endCharCode     此组中的最后一个字符代码，与上面 startCharCode 列出的条件相同
//      uint32  startGlyphID    对应于起始字符代码的字形索引
//
// 这里有一些说明。endCharCode 被使用，而不是计数，因为组匹配的比较通常在现有字符代码上进行，
// 而显式拥有 endCharCode 可以节省每个组进行加法运算的必要性。组必须按 startCharCode 递增排
// 序。组的 endCharCode 必须小于以下组的 startCharCode（如果有）。
//
// 要确定特定字（cp）是否是 32 位代码点的前半部分，可以使用 is32[cp/8] & (1 << (7 - (cp % 8)))
// 的表达式。如果此值非零，则该字是 32 位代码点的前半部分。0 不是 32 位代码点高字（high word）
// 的特殊值。字体不得同时具有代码点 0x0000 的字形和高字为 0x0000 的代码点的字形。
//
// 即使字体不包含特定 16 位起始值的字形，指示特定 16 位值是否是 32 位字符代码开始的打包位数组
// 的存在也是有用的。这是因为系统软件通常需要知道下一个字符提前多少字节开始，即使当前字符映射
// 到缺失字形。通过在表中显式包含此信息，不需要将"秘密"知识编码到操作系统中。
//
// 虽然此格式是为了支持 Unicode 辅助平面字符而创建的，但它没有得到广泛支持或使用。此外，除
// Unicode 之外的任何字符编码都不使用混合 16/32 位字符。不鼓励使用此格式。

typedef struct {
    prh_r32 start_char_code;
    prh_r32 end_char_code;
    prh_r32 start_glyph_id;
} prh_font_cmap_sequential_group;

typedef struct {
    prh_r16 format;
    prh_r16 reserved;
    prh_r32 length;
    prh_r32 language;
    prh_r08 is32[8192];
    prh_r32 num_groups;
    prh_font_cmap_sequential_group groups[1];
} prh_font_cmap_format_8;

// 格式 10：修剪数组（Trimmed Array）
//
// 子表格式 10 旨在支持 Unicode 辅助平面字符，尽管它不常用。格式 10 类似于格式 6，因为它
// 为紧密的字符代码范围定义了修剪数组。然而，它使用 32 位字符代码。'cmap' 子表格式 10：
//      类型    名称                描述
//      uint16  format              子表格式，设置为 10
//      uint16  reserved            保留，设置为 0
//      uint32  length              此子表的字节长度（包括头部）
//      uint32  language            特定语言
//      uint32  startCharCode       覆盖的第一个字符代码
//      uint32  numChars            覆盖的字符代码数
//      uint16  glyphIdArray[]      覆盖的字符代码的字形索引数组
//
// 此格式未广泛使用，Windows 平台不支持。它最适合仅支持连续范围的 Unicode 辅助平面字符的
// 字体，但此类字体很少见。

typedef struct {
    prh_r16 format;
    prh_r16 reserved;
    prh_r32 length;
    prh_r32 language;
    prh_r32 start_char_code;
    prh_r32 num_chars;
    prh_r16 glyph_id[2];
} prh_font_cmap_format_10;

prh_r32 prh_font_glyph_index_from_cmap_format_10(prh_font_cmap_format_10 *f, prh_r32 char_code) {
    if (char_code >= f->start_char_code) {
        prh_r32 i = char_code - f->start_char_code;
        if (i < f->num_chars) return f->glyph_id[i];
    }
    return 0;
}

// 格式 12：分段覆盖（Segmented Coverage）
//
// 这是支持包含辅助平面字符（U+10000 到 U+10FFFF）的 Unicode 字符库的字体的标准字符到
// 字形索引映射子表。包含格式 12 子表的字体还可以包含格式 4 子表以兼容旧应用程序。然而
// 这不是必需的，但请参阅建议章节以获取更多信息。https://learn.microsoft.com/en-us/typography/opentype/spec/recom#cmap-table
//
// 格式 12 类似于格式 4，因为它为稀疏表示定义了段。然而，它使用 32 位字符代码。'cmap'
// 子表格式 12：
//      类型    名称                            描述
//      uint16  format                          子表格式，设置为 12
//      uint16  reserved                        保留，设置为 0
//      uint32  length                          此子表的字节长度（包括头部）
//      uint32  language                        特定语言
//      uint32  numGroups                       后续分组的数量
//      SequentialMapGroup groups[numGroups]    SequentialMapGroup 记录数组
//
// 顺序映射组记录与格式 8 子表使用的格式相同。然而，此处不适用有关 16 位字符代码的限定，
// 因为字符代码统一为 32 位。SequentialMapGroup 记录：
//      类型    名称            描述
//      uint32  startCharCode   此组中的第一个字符代码
//      uint32  endCharCode     此组中的最后一个字符代码
//      uint32  startGlyphID    对应于起始字符代码的字形索引
//
// 组必须按 startCharCode 递增排序。组的 endCharCode 必须小于以下组的 startCharCode
// （如果有）。使用 endCharCode 而不是计数，因为组匹配的比较通常在现有字符代码上进行，
// 而显式拥有 endCharCode 可以节省每个组进行加法运算的必要性。

// 格式 13：多对一范围映射（Many-to-One Range Mappings）
//
// 此子表适用于将相同字形用于跨越代码空间多个范围的数百甚至数千个连续字符的情况。此子表
// 格式可能对"最后手段"字体有用，尽管这些字体也可以使用其他合适的子表格式。对于"最后手段"
// 字体，另请参阅 'head' 表标志，14 比特位。注意：子表格式 13 与格式 12 具有相同的结构；
// 仅在 startGlyphID/glyphID 字段的解释上有所不同。
//
// 'cmap' 子表格式 13：
//      类型    名称                        描述
//      uint16  format                      子表格式；设置为 13
//      uint16  reserved                    保留；设置为 0
//      uint32  length                      此子表的字节长度（包括头部）
//      uint32  language                    特定语言
//      uint32  numGroups                   后续分组的数量
//      ConstantMapGroup groups[numGroups]  ConstantMapGroup 记录数组
//
// 常量映射组记录具有与顺序映射组记录相同的结构，具有起始和结束字符代码以及映射的字形 ID。
// 然而，相同的字形 ID 适用于指定范围内的所有字符，而不是顺序的字形 ID。
//
// ConstantMapGroup 记录：
//      类型    名称            描述
//      uint32  startCharCode   此组中的第一个字符代码，组必需按 startCharCode 递增排序
//      uint32  endCharCode     此组中的最后一个字符代码
//      uint32  glyphID         用于组范围内所有字符的字形索引

typedef struct {
    prh_r16 format;
    prh_r16 reserved;
    prh_r32 length;
    prh_r32 language;
    prh_r32 num_groups;
    prh_font_cmap_sequential_group groups[1];
} prh_font_cmap_format_12;

#define prh_impl_font_cmap_sequential_group_end_char_code(group) prh_bp_4b_to_host((prh_byte *)&(group)->end_char_code)

prh_r32 prh_font_glyph_index_from_cmap_format_12(prh_font_cmap_record *p, prh_r32 char_code) {
    prh_byte *record = p->table_data + p->record_offset;
    prh_r16 format = prh_bp_2b_to_host(record);
    prh_r32 length = prh_bp_4b_to_host(record + 4);
    prh_r32 language = prh_bp_4b_to_host(record + 8);
    prh_r32 num_groups = prh_bp_4b_to_host(record + 12);
    prh_font_cmap_sequential_group *group = (prh_font_cmap_sequential_group *)(record + 16);
    prh_reg i;

    prh_assert(format == 12 && (prh_byte *)(group + num_groups) <= record + length && record + length <= p->table_data + p->table_length);
    prh_generic_bsearch_first_less_equal(i, char_code, group, num_groups, prh_impl_font_cmap_sequential_group_end_char_code);
    if (i < num_groups) {
        prh_r32 start_char_code = prh_bp_4b_to_host((prh_byte *)&group[i].start_char_code);
        if (char_code >= start_char_code) return prh_bp_4b_to_host((prh_byte *)&group[i].start_glyph_id) + (char_code - start_char_code);
    }

    return 0;
}

prh_r32 prh_font_glyph_index_from_cmap_format_13(prh_font_cmap_record *p, prh_r32 char_code) {
    prh_byte *record = p->table_data + p->record_offset;
    prh_r16 format = prh_bp_2b_to_host(record);
    prh_r32 length = prh_bp_4b_to_host(record + 4);
    prh_r32 language = prh_bp_4b_to_host(record + 8);
    prh_r32 num_groups = prh_bp_4b_to_host(record + 12);
    prh_font_cmap_sequential_group *group = (prh_font_cmap_sequential_group *)(record + 16);
    prh_reg i;

    prh_assert(format == 13 && (prh_byte *)(group + num_groups) <= record + length && record + length <= p->table_data + p->table_length);
    prh_generic_bsearch_first_less_equal(i, char_code, group, num_groups, prh_impl_font_cmap_sequential_group_end_char_code);
    if (i == num_groups || char_code < prh_bp_4b_to_host((prh_byte *)&group[i].start_char_code)) return 0;

    return prh_bp_4b_to_host((prh_byte *)&group[i].start_glyph_id);
}

void prh_impl_print_font_cmap_format_12_13(prh_font_cmap_record *p) {
    prh_byte *record = p->table_data + p->record_offset;
    prh_r16 format = prh_bp_2b_to_host(record);
    prh_r32 length = prh_bp_4b_to_host(record + 4);
    prh_r32 language = prh_bp_4b_to_host(record + 8);
    prh_r32 num_groups = prh_bp_4b_to_host(record + 12);
    prh_font_cmap_sequential_group *group = (prh_font_cmap_sequential_group *)(record + 16);
    prh_r32 i = 0;

    prh_assert(format == 12 || format == 13);
    prh_assert((prh_byte *)(group + num_groups) <= record + length && record + length <= p->table_data + p->table_length);

    prh_print(
        "----------------------------\n"
        "cmap record format %d length %d\n"
        "cmap record format %d language %04x\n"
        "cmap record format %d character groups %d\n",
        (prh_reg)format, (prh_reg)length,
        (prh_reg)format, (prh_reg)language,
        (prh_reg)format, (prh_reg)num_groups);

    prh_r32 print_count = 8;
    prh_r32 group_count = num_groups > print_count ? print_count : num_groups;
    for (; i < group_count; i += 1) {
        prh_print(
            "cmap record format %d group index %d / %d\n"
            "cmap record format %d character U+%04x U+%04x\n"
            "cmap record format %d start glyph index %d\n",
            (prh_reg)format, (prh_reg)i + 1, (prh_reg)num_groups,
            (prh_reg)format, (prh_reg)prh_bp_4b_to_host((prh_byte *)&group[i].start_char_code), (prh_reg)prh_bp_4b_to_host((prh_byte *)&group[i].end_char_code),
            (prh_reg)format, (prh_reg)prh_bp_4b_to_host((prh_byte *)&group[i].start_glyph_id));
    }

    i = num_groups > print_count ? num_groups - print_count : 0;
    for (; i < num_groups; i += 1) {
        prh_print(
            "cmap record format %d group index %d / %d\n"
            "cmap record format %d character U+%04x U+%04x\n"
            "cmap record format %d start glyph index %d\n",
            (prh_reg)format, (prh_reg)i + 1, (prh_reg)num_groups,
            (prh_reg)format, (prh_reg)prh_bp_4b_to_host((prh_byte *)&group[i].start_char_code), (prh_reg)prh_bp_4b_to_host((prh_byte *)&group[i].end_char_code),
            (prh_reg)format, (prh_reg)prh_bp_4b_to_host((prh_byte *)&group[i].start_glyph_id));
    }
}

// 格式 14：Unicode 变体序列（Unicode Variation Sequences）
//
// 子表格式 14 指定字体支持的 Unicode 变体序列（UVSes）。根据 Unicode 标准，变体序列由
// 基本字符后跟变体选择器组成。例如，<U+82A6, U+E0101>。此子表格式只能用于平台 ID 0 和
// 编码 ID 5。                          ^       ^
//                                      |       | 变体选择器
//                                  基本字符
// Unicode 变体序列（Unicode Variation Sequences, UVS） 是一种机制，允许同一个 Unicode
// 码点显示为不同的字形变体，而不需要分配新的独立码点。为什么需要变体序列：
//      历史字形差异    同一汉字在不同标准中写法不同
//      地区字形差异    同一字符在中国大陆、台湾、日本有不同写法
//      专业排版需求    数学、音乐、文字学需要特定变体
//      避免码点爆炸    不每个变体都分配新码点，保持 Unicode 简洁
//
// 变体选择器范围：
//      范围                用途
//      U+FE00 – U+FE0F     标准变体选择器（VS1-VS16），用于已有字符的变体
//      U+E0100 – U+E01EF   变体选择器补充（VS17-VS256），用于 CJK 等大量变体
//
// 在字体中的支持，字体通过 cmap 格式 14 子表声明支持哪些变体序列，字体中的两种声明方式：
//  1.  默认变体（Default UVS）
//      基本字符在 Unicode cmap 中映射的字形，就是该变体序列要显示的字形
//      无需额外指定字形 ID
//  2.  非默认变体（Non-Default UVS）
//      变体序列需要显示与 Unicode cmap 中不同的字形
//      必须显式指定字形 ID
//
// 子表将字体支持的 UVSes 分为两类："默认" 和 "非默认" UVSes。给定一个 UVS，如果在 Unicode
// 'cmap' 子表（即 BMP 子表或 BMP+辅助平面子表）中查找该序列的基本字符获得的字形是该序列
// 要使用的字形，则该序列是 "默认" UVS。否则，它是 "非默认" UVS，并且该序列要使用的字形在
// 格式 14 子表本身中指定。页面底部的示例显示了字体供应商如何为支持 JIS-2004 的字体使用格
// 式 14。
//
// 'cmap' 子表格式 14：
//      类型                名称                                描述
//      uint16              format                              子表格式，设置为 14
//      uint32              length                              此子表的字节长度（包括此头部）
//      uint32              numVarSelectorRecords               变体选择器记录的数量
//      VariationSelector   varSelector[numVarSelectorRecords]  VariationSelector 记录数组
//
// 每个 VariationSelector 记录指定一个变体选择器字符，以及用于映射使用该变体选择器的变体
// 序列的 "默认" 和 "非默认" 表的偏移。VariationSelector 记录：
//      类型        名称                描述
//      uint24      varSelector         变体选择器
//      Offset32    defaultUVSOffset    从格式 14 子表开头到默认 UVS 表的偏移，可能为 0
//      Offset32    nonDefaultUVSOffset 从格式 14 子表开头到非默认 UVS 表的偏移，可能为 0
//
// VariationSelector 记录按 varSelector 递增顺序排序。两条记录不得具有相同的 varSelector
// 值。VariationSelector 记录及其偏移指向的数据指定字体支持的 UVSes，其中变体选择器是记录
// 的 varSelector 值。UVSes 的基本字符存储在偏移指向的表中。UVSes 按是否为默认或非默认
// UVSes 进行分区。非默认 UVSes 要使用的字形 ID 在非默认 UVS 表中指定。
//
// 默认 UVS 表。默认 UVS 表是 Unicode 标量值的简单范围压缩列表，表示使用相关 VariationSelector
// 记录的 varSelector 的默认 UVSes 的基本字符。默认 UVS 表：
//      类型            名称                            描述
//      uint32          numUnicodeValueRanges           Unicode 字符范围的数量
//      UnicodeRange    ranges[numUnicodeValueRanges]   UnicodeRange 记录数组
//
// 每个 Unicode 范围记录指定一个连续的 Unicode 值范围。UnicodeRange 记录：
//      类型    名称                描述
//      uint24  startUnicodeValue   此范围中的第一个值
//      uint8   additionalCount     此范围中的附加值数量
//
// 例如，范围 U+4E4D – U+4E4F（3 个值）将设置 startUnicodeValue 为 0x004E4D，additionalCount
// 为 2。单例范围将设置 additionalCount 为 0。（startUnicodeValue + additionalCount）的
// 总和不得超过 0xFFFFFF。
//
// Unicode 值范围按 startUnicodeValue 递增顺序排序。范围不得重叠，即（startUnicodeValue +
// additionalCount）必须小于以下范围的起始 Unicode 值（如果有）。范围数组中列出的所有代码点
// 应在 Unicode 'cmap' 子表中有相应的条目。然而，应用程序可能遇到并非如此的情况。
//
// 非默认 UVS 表。非默认 UVS 表是 Unicode 标量值和字形 ID 对的列表。Unicode 值表示使用
// 相关 VariationSelector 记录的 varSelector 的所有非默认 UVSes 的基本字符，字形 ID 指
// 定 UVSes 要使用的字形 ID。非默认 UVS 表：
//      类型        名称                            描述
//      uint32      numUVSMappings                  后续 UVS 映射的数量
//      UVSMapping  uvsMappings[numUVSMappings]     UVSMapping 记录数组
//
// 每个 UVSMapping 记录为单个 Unicode 基本字符提供字形 ID 映射，当该基本字符与当前变体选
// 择器一起用于变体序列时。UVSMapping 记录：
//      类型    名称            描述
//      uint24  unicodeValue    UVS 的 Unicode 基本值
//      uint16  glyphID         UVS 的字形 ID
//
// UVS 映射按 unicodeValue 递增顺序排序。此表中的两条映射不得具有相同的 unicodeValue 值。
// 通常，uvsMappings 数组中列出的代码点在 Unicode 'cmap' 子表中有相应的条目。然而，这不
// 是必需的。例如，如果字体旨在用于给定 Unicode 字符仅在变体序列中出现的内容，则可能就是
// 这种情况。
//
// 示例。以下是如何在支持 JIS-2004 变体字形的字体中使用格式 14 'cmap' 子表的示例。本示例
// 中的 CID（字符 ID）指 Adobe 字符集合 "Adobe-Japan1" 中的那些，可以假设它们与示例中字体
// 的字形 ID 相同。JIS-2004 更改了其某些代码点的默认字形变体。例如：
//      JIS-90：U+82A6 ⇒ CID 1142
//      JIS-2004：U+82A6 ⇒ CID 7961
//
// 这两个字形变体都通过使用 Unicode 变体序列来支持，如 Unicode 的 UVS 注册表中的以下示例
// 所示：
//      U+82A6 U+E0100 ⇒ CID 1142
//      U+82A6 U+E0101 ⇒ CID 7961
//
// 如果字体想要默认支持 JIS-2004 变体，它将：
//  * 在 Unicode 'cmap' 子表中将字形 ID 7961 编码在 U+82A6
//  * 在 UVS 'cmap' 子表的默认 UVS 表中指定 <U+82A6, U+E0101>，varSelector 将为 0x0E0101，
//    defaultUVSOffset 将指向包含 0x0082A6 Unicode 值的数据
//  * 在 UVS 'cmap' 子表的非默认 UVS 表中指定 <U+82A6, U+E0100> ⇒ 字形 ID 1142。varSelector
//    将为 0x0E0100，nonDefaultBaseUVSOffset 将指向包含 unicodeValue 0x0082A6 和 glyphID
//    1142 的数据。
//
// 然而，如果字体想要默认支持 JIS-90 变体，它将：
//  * 在 Unicode 'cmap' 子表中将字形 ID 1142 编码在 U+82A6
//  * 在 UVS 'cmap' 子表的默认 UVS 表中指定 <U+82A6, U+E0100>
//  * 在 UVS 'cmap' 子表的非默认 UVS 表中指定 <U+82A6, U+E0101> ⇒ 字形 ID 7961

typedef struct {
    prh_r16 aligned;
    prh_r16 format;
    prh_r32 length;
    prh_r32 num_selectors;
} prh_font_cmap_format_14;

typedef struct {
    prh_r08 aligned_byte;
    prh_r08 selector_bytes[3];
    prh_r32 default_variation_offset;
    prh_r32 non_default_variation_offset;
} prh_font_cmap_variation_selector;

typedef struct { // uint32 numUnicodeValueRanges
    prh_r08 aligned_byte;
    prh_r08 start_unicode_value[3];
    prh_r08 additional_count;
} prh_font_cmap_default_variation_unicode_range;

typedef struct { // uint32 numUVSMappings
    prh_r08 aligned;
    prh_r08 unicode_value[3];
    prh_r16 glyph_id;
} prh_font_cmap_non_default_variation_mapping;

void prh_impl_print_font_cmap_format_14(prh_font_cmap_record *p) {
    prh_byte *record_data = p->table_data + p->record_offset;
    prh_byte *record_start = record_data;

    prh_r16 record_format = prh_bp_2b_to_host(record_data); record_data += 2;
    prh_r32 record_length = prh_bp_4b_to_host(record_data); record_data += 4;
    prh_r32 num_selectors = prh_bp_4b_to_host(record_data); record_data += 4;
    prh_assert(record_format == 14);

    prh_byte *header_end = record_data + num_selectors * 11;
    prh_byte *record_end = record_start + record_length;
    if (record_end < header_end || record_end > p->table_data + p->table_length) {
        prh_print("table 'cmap' record format 14 invalid length %d %d %d with selectors %d\n",
            (prh_reg)(header_end - record_start), (prh_reg)record_length, (prh_reg)(p->table_length - p->record_offset), (prh_reg)num_selectors);
        return;
    }

    prh_print(
        "----------------------------\n"
        "cmap record format 14 length %d\n"
        "cmap record format 14 language 0\n"
        "cmap record format 14 selectors %d\n",
        (prh_reg)record_length,
        (prh_reg)num_selectors);

    for (prh_r32 i = 0; i < num_selectors; i += 1) {
        prh_r32 selector_character = prh_bp_3b_to_host(record_data); record_data += 3;
        prh_r32 default_variation_offset = prh_bp_4b_to_host(record_data); record_data += 4;
        prh_r32 non_default_variation_offset = prh_bp_4b_to_host(record_data); record_data += 4;

        if (default_variation_offset != 0 && default_variation_offset < header_end - record_start || default_variation_offset >= record_length) {
            prh_print("table 'cmap' record format 14 selector index %d invalid variation default table offset %d\n", (prh_reg)i+1, default_variation_offset);
            return;
        }

        if (non_default_variation_offset != 0 && non_default_variation_offset < header_end - record_start || non_default_variation_offset >= record_length) {
            prh_print("table 'cmap' record format 14 selector index %d invalid variation non-default table offset %d\n", (prh_reg)i+1, non_default_variation_offset);
            return;
        }

        prh_print(
            "cmap record format 14 variation selector index %d / %d\n"
            "cmap record format 14 variation selector character U+%x \n"
            "cmap record format 14 variation default table offset %d\n",
            (prh_reg)i + 1, (prh_reg)num_selectors,
            (prh_reg)selector_character,
            (prh_reg)default_variation_offset);

        if (default_variation_offset != 0) {
            prh_byte *variation_data = record_start + default_variation_offset;
            prh_r32 variation_unicode_ranges = prh_bp_4b_to_host(variation_data); variation_data += 4;
            if (variation_data + variation_unicode_ranges * 4 > record_end) {
                prh_print("table 'cmap' record format 14 selector index %d variation default table invalid num_ranges %d offset %d record_length %d\n",
                    (prh_reg)i+1, (prh_reg)variation_unicode_ranges, (prh_reg)default_variation_offset, (prh_reg)record_length);
                return;
            }
            prh_r32 num_ranges = variation_unicode_ranges > 8 ? 8 : variation_unicode_ranges;
            for (prh_r32 range_i = 0; range_i < num_ranges; range_i += 1) {
                prh_r32 start_unicode_value = prh_bp_3b_to_host(variation_data); variation_data += 3;
                prh_r08 additional_count = *variation_data; variation_data += 1;
                prh_print(
                    "cmap record format 14 variation default table unicode range %d / %d\n"
                    "cmap record format 14 variation default table unicode U+%x U+%x\n",
                    (prh_reg)range_i + 1,
                    (prh_reg)variation_unicode_ranges,
                    (prh_reg)start_unicode_value,
                    (prh_reg)start_unicode_value + additional_count);
            }
        }

        prh_print(
            "cmap record format 14 variation non-default table offset %d\n",
            (prh_reg)non_default_variation_offset);

        if (non_default_variation_offset != 0) {
            prh_byte *variation_data = record_start + non_default_variation_offset;
            prh_r32 variation_num_mappings = prh_bp_4b_to_host(variation_data); variation_data += 4;
            if (variation_data + variation_num_mappings * 5 > record_end) {
                prh_print("table 'cmap' record format 14 selector index %d variation non-default table invalid num_mappings %d offset %d record_length %d\n",
                    (prh_reg)i+1, (prh_reg)variation_num_mappings, (prh_reg)non_default_variation_offset, (prh_reg)record_length);
                return;
            }
            prh_r32 num_mappings = variation_num_mappings > 8 ? 8 : variation_num_mappings;
            for (prh_r32 mapping_i = 0; mapping_i < num_mappings; mapping_i += 1) {
                prh_r32 unicode_value = prh_bp_3b_to_host(variation_data); variation_data += 3;
                prh_r16 glyph_id = prh_bp_2b_to_host(variation_data); variation_data += 2;
                prh_print(
                    "cmap record format 14 variation non-default table mapping %d / %d\n"
                    "cmap record format 14 variation non-default table unicode U+%x U+%x glyph index %d\n",
                    (prh_reg)mapping_i + 1,
                    (prh_reg)variation_num_mappings,
                    (prh_reg)unicode_value,
                    (prh_reg)selector_character,
                    (prh_reg)glyph_id);
            }
        }
    }
}

prh_r32 prh_font_glyph_index_from_cmap_format_14(prh_font_cmap_record *p, prh_r32 base_character, prh_r32 variation_selector, prh_r32 (*default_glyph_index_func)(void *f, prh_r32 char_code), void *specified_format) {
    prh_byte *record_data = p->table_data + p->record_offset;
    prh_byte *record_start = record_data;

    prh_r16 record_format = prh_bp_2b_to_host(record_data); record_data += 2;
    prh_r32 record_length = prh_bp_4b_to_host(record_data); record_data += 4;
    prh_r32 num_selectors = prh_bp_4b_to_host(record_data); record_data += 4;
    prh_assert(record_format == 14);

    prh_byte *header_end = record_data + num_selectors * 11;
    prh_byte *record_end = record_start + record_length;
    if (record_end < header_end || record_end > p->table_data + p->table_length) {
        prh_eprint("table 'cmap' record format 14 invalid length %d %d %d with selectors %d\n",
            (prh_reg)(header_end - record_start), (prh_reg)record_length, (prh_reg)(p->table_length - p->record_offset), (prh_reg)num_selectors);
        return 0;
    }

    for (prh_r32 i = 0; i < num_selectors; i += 1) {
        prh_r32 selector_character = prh_bp_3b_to_host(record_data); record_data += 3;
        prh_r32 default_variation_offset = prh_bp_4b_to_host(record_data); record_data += 4;
        prh_r32 non_default_variation_offset = prh_bp_4b_to_host(record_data); record_data += 4;
        if (variation_selector != selector_character) continue;

        if (default_variation_offset != 0 && default_variation_offset < header_end - record_start || default_variation_offset >= record_length) {
            prh_eprint("table 'cmap' record format 14 selector index %d invalid variation default table offset %d\n", (prh_reg)i+1, default_variation_offset);
            return 0;
        }

        if (non_default_variation_offset != 0 && non_default_variation_offset < header_end - record_start || non_default_variation_offset >= record_length) {
            prh_eprint("table 'cmap' record format 14 selector index %d invalid variation non-default table offset %d\n", (prh_reg)i+1, non_default_variation_offset);
            return 0;
        }

        if (default_variation_offset != 0) {
            prh_byte *variation_data = record_start + default_variation_offset;
            prh_r32 default_variation_num_ranges = prh_bp_4b_to_host(variation_data); variation_data += 4;
            if (variation_data + default_variation_num_ranges * 4 > record_end) {
                prh_eprint("table 'cmap' record format 14 selector index %d variation default table invalid num_ranges %d offset %d record_length %d\n",
                    (prh_reg)i+1, (prh_reg)default_variation_num_ranges, (prh_reg)default_variation_offset, (prh_reg)record_length);
                return 0;
            }
            for (prh_r32 range_i = 0; range_i < default_variation_num_ranges; range_i += 1) {
                prh_r32 start_unicode_value = prh_bp_3b_to_host(variation_data); variation_data += 3;
                prh_r08 additional_count = *variation_data; variation_data += 1;
                if (base_character >= start_unicode_value && base_character <= start_unicode_value + additional_count) {
                    return default_glyph_index_func(specified_format, base_character);
                }
            }
        }

        if (non_default_variation_offset != 0) {
            prh_byte *variation_data = record_start + non_default_variation_offset;
            prh_r32 variation_num_mappings = prh_bp_4b_to_host(variation_data); variation_data += 4;
            if (variation_data + variation_num_mappings * 5 > record_end) {
                prh_eprint("table 'cmap' record format 14 selector index %d variation non-default table invalid num_mappings %d offset %d record_length %d\n",
                    (prh_reg)i+1, (prh_reg)variation_num_mappings, (prh_reg)non_default_variation_offset, (prh_reg)record_length);
                return 0;
            }
            for (prh_r32 mapping_i = 0; mapping_i < variation_num_mappings; mapping_i += 1) {
                prh_r32 unicode_value = prh_bp_3b_to_host(variation_data); variation_data += 3;
                prh_r16 glyph_id = prh_bp_2b_to_host(variation_data); variation_data += 2;
                if (base_character == unicode_value) return glyph_id;
            }
        }
    }

    return 0;
}

// CFF — 紧凑字体格式（版本 1）
//
// 本表包含紧凑字体格式（CFF）字体表示，其结构依据 Adobe 技术说明 5176：《紧凑字体格式规范》，
// 以及 Adobe 技术说明 5177：《Type 2 Charstring 格式》。注意：下面有关提到的 CFF INDEX、
// DICT 和 FontSet 结构的详细信息，请参阅 Adobe 技术说明 5176。
// http://partners.adobe.com/public/developer/en/font/5176.CFF.pdf
// http://partners.adobe.com/public/developer/en/font/5177.Type2.pdf
//
// CFF 字体格式在 OpenType 开发之前就已作为独立字体格式存在。当纳入 OpenType 格式时，某些
// 基本字体信息继续使用 CFF 格式定义的机制来表示，这复制了 OpenType 中用于 TrueType 轮廓字
// 体的其他数据表示。例如，CFF 将字形宽度纳入字形轮廓描述中，尽管这些信息也可以在 'hmtx'
// 表中提供。使用 TrueType 轮廓的 OpenType 字体使用字形索引配合 'loca' 表中的偏移来访问
// 'glyf' 表内的字形数据。这一概念在 OpenType CFF 字体中得以保留，只是字形数据通过 'CFF '
// 表内的 CharStrings INDEX 结构来访问。
//
// 有关 'glyf'、'CFF ' 和 CFF2 表之间显著差异的摘要，请参阅《'glyf'、'CFF ' 和 CFF2 表的
// 比较》。https://learn.microsoft.com/en-us/typography/opentype/spec/glyphformatcomparison
//
// 'CFF ' 表中的 Name INDEX 必须只包含一个条目；也就是说，CFF FontSet 中必须只有一个字体。    *** Name INDEX 必须只包含一个条目，即 CFF FontSet 必须只有一个字体
// 并不要求此名称与 'name' 表中名称 ID 6 的条目相同。注意，在 OpenType 字体集合文件中，单
// 个 'CFF ' 表可以跨多个字体共享；应用程序使用的名称必须是 'name' 表中提供的名称，而非
// Name INDEX 条目。
//
// CFF 顶层 DICT 必须指定 CharstringType 值为 2。'maxp' 表中的 numGlyphs 字段必须与 CFF 的    *** Top DICT 必须指定 CharstringType 为 2
// CharStrings INDEX 中的条目数相同。对于字体中的所有字形，OpenType 字体字形索引与 CFF 字     *** 'maxp' 表中的 numGlyphs 必须与 CFF 的 CharStrings INDEX 条目数相同
// 形索引相同。
//
// CFF2 — 紧凑字体格式（CFF）版本 2 简介
//
// 紧凑字体格式表版本 2（CFF2）用于描述 OpenType 字体中的字形。它是 'glyf' 表的替代方案，
// 使用一种源自 Adobe Postscript 语言的高效格式来表示字形轮廓。在 CFF2 表中，使用三次（三
// 阶）贝塞尔曲线和直线序列来定义字形轮廓。CFF2 数据还可以包含"混合"操作，由 OpenType 字
// 体变体机制控制，以改变字形形状。使用栅格化填充规则来提供每个字形的纯色单色形状。CFF2
// 数据可以包含影响此栅格化的"提示"操作。与 COLR 和 CPAL 表结合使用时，CFF2 表可用于表示
// 多色字形。
//
// CFF2 是 'CFF ' 表和字形格式的后继和改进版本。由于 CFF 起源于 Postscript 语言，完整的
// 'CFF ' 表中的数据可以作为独立字体使用。然而，当在 OpenType 字体中使用时，CFF 的这一方面
// 会导致冗余。CFF2 通过依赖其他字体表中的数据来避免冗余。CFF2 还添加了用于可变字体的新操
// 作符，并使用新的 CFF2 CharString 规范。
//
// 有关 'glyf'、'CFF ' 和 'CFF2' 表之间显著差异的摘要，请参阅《'glyf'、'CFF ' 和 CFF2 表
// 的比较》。
//
// 'glyf'、'CFF ' 和 CFF2 表的比较
// https://learn.microsoft.com/en-us/typography/opentype/spec/glyphformatcomparison
//
// OpenType 字体中有三种可选格式可用于存储单色字形轮廓：'glyf' 表、'CFF ' 表和 CFF2 表。
// 功能正常的 OpenType 字体必须包含这三种表之一。本节提供这些字形数据替代格式的比较。
//
// CFF2 和 CFF 使用三次（三阶）贝塞尔曲线表示字形轮廓，而 'glyf' 表使用二次（二阶）贝塞
// 尔曲线。CFF2 和 CFF 在"提示"概念模型上也与 'glyf' 表不同。这三种表在变体支持以及变体
// 数据存储方式方面也存在差异。下表提供了 CFF2、'CFF ' 和 'glyf' 表的比较摘要。注意，其
// 中一些差异可能不会在高阶字体编辑软件或运行时编程接口中暴露。
//
//      考量因素        glyf                                CFF                                                 CFF2
//      曲线            二次（二阶）贝塞尔                  三次（三阶）贝塞尔                                  三次（三阶）贝塞尔
//      坐标精度        1 设计单位                          1/65536 设计单位                                    1/65536 设计单位
//      去重            多个字形可通过复合字形共享轮廓      多个字形可通过子程序共享 CharString 数据            多个字形可通过子程序共享 CharString 数据
//      提示            TrueType 指令按控制量移动轮廓点     对齐区域应用于所有字形，主干位置在每个字形中声明    对齐区域应用于任意字形组，主干位置在每个字形中声明
//      解码            非基于栈（除 TrueType 指令外）      主要基于栈                                          主要基于栈
//      OpenType 变体   支持：轮廓变体数据存储在 'gvar'     不支持                                              支持：轮廓和提示的变体数据存储在 CFF2 表内，部分使
//                      表中（OFF: 7.3.4）；提示变体数据                                                        用其他表也使用的通用变体数据格式，部分与单个字形描
//                      存储在 'cvar' 表中（OFF: 7.3.2）                                                        述的 CharString 数据交错
//      数据冗余        低                                  中等                                                低
//      重叠轮廓        支持                                不支持                                              支持
//
//      *** 按控制量：by controlled amounts
//      *** 移动轮廓点：move outline points
//      *** 主干位置：stem locations
//      *** 重叠轮廓：overlapping contours（contour 外形、轮廓、等高线）
//
// CFF2 与 CFF 的其他差异。CFF2 格式从 CFF 格式更改而来，以与 OpenType 格式的其他部分整合，
// 消除 OpenType 字体中不需要的数据，并支持 OpenType 字体变体。以下是主要差异。
//  1.  CFF2 表中不支持 FontSet。因此，CFF2 表只能包含一个 TopDICT，不支持 TopDICT INDEX。
//  2.  CFF2 表中不支持名称 INDEX 和字符串 INDEX。
//  3.  CFF2 不支持编码和字符集表。相应的 TopDICT 操作符被移除，同时还有其他几个 TopDICT 操作符也被移除。
//  4.  CFF2 CharStrings 不包含前进宽度值（advance width）。
//  5.  对于 CFF2 表，CharStrings 的填充规则必须始终为非零绕数规则（nonzero winding number rule），而非
//      偶奇规则（even-odd rule）。这是为了支持可变字体数据，其中强制移除路径之间的重叠是不切实际的。
//  6.  对于 CFF2，CharString 栈深度从 48 增加到 513。
//  7.  CFF2 中的 PrivateDICT 和 CharString 操作符集扩展，包含 blend 和 vsindex 操作符。
//  8.  CFF2 中移除了 Type 2 的 endchar 和 return CharString 操作符，以及所有逻辑、存储和数学操作符。

// 技术说明 5176：《紧凑字体格式规范》
// http://partners.adobe.com/public/developer/en/font/5176.CFF.pdf
//
// 本⽂档描述了⼀种字体格式的规范，该格式适合紧凑地表⽰⼀个或多个 Type 1 或 CID 键控字体。
// 与以往的 Type 1 和 CID 键控字体格式不同，CFF 允许将多个字体⼀起存储在⼀个称为字体集
// （FontSet）的单元中。空间节省主要来⾃三个⽅⾯：对⼤部分信息采⽤紧凑的⼆进制表⽰、在字体
// 之间共享公共数据，以及对频繁出现的数据使⽤默认值。
//
// CFF 格式设计上与 Type 2 charstring 配合使⽤，后者作为字符轮廓描述过程（参⻅ Adobe 技术
// 说明 5177：《Type 2 Charstring 格式》）。该设计⽀持嵌⼊ PostScript 语⾔代码，从⽽在打印
// 环境中使⽤时为格式提供额外的灵活性和可扩展性（参⻅附录 E）。
//
// 数据布局（Data Layout）
//
// 从概念上讲，⼆进制数据被组织为若⼲独⽴的数据结构。⼆进制数据内的总体布局如下表所⽰。由
// 于其中⼀些数据结构是通过偏移量定位的，其次序可以改变，但前五个占据固定位置。附录 D 有
// CFF 字体的示例。
//
//      Header                              --
//      Name INDEX                          --
//      Top DICT INDEX                      --
//      String INDEX                        --
//      Global Subr INDEX                   全局子例程索引
//      Encodings                           编码
//      Charsets                            字符集
//      FDSelect                            仅 CIDFont
//      CharStrings INDEX                   per-font
//      Font DICT INDEX                     per-font, 仅 CIDFonts
//      Private DICT                        per-font
//      Local Subr INDEX                    per-font or per-Private DICT for CIDFonts
//      Copyright and Trademark Notices     版权与商标声明
//
// 数据类型（Data Type）
//
// 本节描述该格式所⽤的数据表⽰和数据类型。所有多字节数值数据和偏移量字段均按⼤端字节序
// 存储（⾼位字节在低偏移处），且不受任何对⻬限制。这使得该格式不含填充字节。
//
// 数据对象通常通过相对于 CFF 数据内某参考点的字节偏移量来指定。这些偏移量⻓度为 1 ⾄ 4
// 字节。本⽂档约定将参考点括在括号内：⽤参考点 (0) 表⽰相对于 CFF 数据起点的偏移量，⽤
// (self) 表⽰相对于包含该偏移量的数据结构的偏移量。
//
// CFF 格式的数据类型如下表所⽰。本⽂档通过列出字段类型、名称和描述来说明数据结构。数据
// 结构可以被赋予⼀个类型名称并在随后加以描述。对象数组⽤通常的⽅括号约定表⽰，⽅括号内
// 为数组⻓度。CFF 数据的⼤部分都包含在两种分别称为 DICT 和 INDEX 的数据结构中，后续章节
// 将对其进⾏描述。
//
//      名称        范围        描述
//      Card8       0 – 255     1 字节⽆符号数
//      Card16      0 – 65535   2 字节⽆符号数
//      Offset      可变        1、2、3 或 4 字节偏移量（由 OffSize 字段指定）
//      OffSize     1 – 4       1 字节⽆符号数，指定 Offset 字段的⼤⼩
//      SID         0 – 64999   2 字节字符串标识符（string identifier）
//
// DICT 数据
//
// 由键值对组成的字体字典数据以⼀种紧凑的标记化格式表⽰，类似于 Type 1 charstring 所⽤的
// 表⽰⽅式。字典的键编码为 1 字节或 2 字节的操作符，字典的值编码为可变⼤⼩的数值操作数，
// 表⽰整数或实数。操作符之前是给出其值的操作数。⼀个 DICT 就是⼀串拼接在⼀起的操作数/操
// 作符字节序列。
//
// 格式定义了若⼲种⼤⼩不同的整数操作数类型，其编码如下表所⽰（操作数⾸字节为 b0，次字节
// 为 b1，依此类推）。
//
//      操作数编码，1、2 和 3 字节整数格式与 Type 2 charstring 所⽤的格式相同：
//      ⼤⼩    b0 范围     值范围                  值计算
//      1       32 ~ 246    –107 ~ +107             b0–139
//      2       247 ~ 250   +108 ~ +1131            (b0–247)*256+b1+108
//      2       251 ~ 254   –1131 ~ –108            –(b0–251)*256–b1–108
//      3       28          –32768 ~ +32767         b1<<8|b2
//      5       29          –(2^31) ~ +(2^31–1)     b1<<24|b2<<16|b3<<8|b4
//
// 整数格式的⽰例如下表所⽰。整数格式⽰例：
//      值          编码
//      0           8b
//      100         ef
//      –100        27
//      1000        fa 7c
//      –1000       fe 7c
//      10000       1c 27 10
//      –10000      1c d8 f0
//      100000      1d 00 01 86 a0
//      –100000     1d ff fe 79 60
//
// 除整数操作数外，还提供了实数操作数。该操作数以字节值 30 开头，后跟⼀段可变⻓度的字节
// 序列。每个字节由两个 4 位半字节（nibble）组成，其定义⻅下表 。⼀对半字节中的第⼀个存
// 储在字节的⾼ 4 位，第⼆个存储在字节的低 4 位。半字节定义：
//      半字节      表⽰
//      0–9         0–9
//      a           .（⼩数点）
//      b           E
//      c           E–
//      d           <保留>
//      e           –（负号）
//      f           数字结束
//
// 实数以⼀个（或两个）0xf 半字节终⽌，使其总能填充到整字节。因此，值 –2.25 编码为字节
// 序列 (1e e2 a2 5f)，值 0.140541E–3 编码为序列 (1e 0a 14 05 41 c3 ff)。
//
// 通过检查⾸字节即可区分操作符与操作数：0–21 表⽰操作符，28、29、30 和 32–254 表⽰操作
// 数（数值）。字节值 22–27、31 和 255 为保留值。⼀个操作符之前最多可以有 48 个操作数。
//
// 操作符可以带有⼀个或多个下表所⽰类型的操作数。操作数类型：
//      类型        描述
//      number      整数或实数
//      boolean     整数类型，取值 0（假）和 1（真）
//      SID         字符串 id（⻅第 10 节）
//      array       ⼀个或多个数值
//      delta       ⼀个数值或⼀个 delta 编码的数值数组（⻅下⽂）
//
// array 或 delta 类型的⻓度通过数操作符之前的操作数个数来确定。delta 中第⼆个及以后的
// 数值编码为相邻值之差。例如，数组 a0, a1, ..., an 将编码为：a0 (a1–a0) (a2–a1) ..., 
// (an–a(n–1))。
//
// 双字节操作符以转义字节 12 开头。
//
// 通过为各种 DICT 键设定默认值，可以进⼀步压缩字典数据。对于具有默认值的键，DICT 中相
// 应操作符的缺失即意味着该键应取其默认值。Top DICT 和 Private DICT 的 DICT 操作符列表
// 分别⻅第 9 节和第 15 节。
//
// 字体字典数据由键值对所组成的紧凑标记化格式数据。dictionary keys 被编码为一字节或两字
// 节的操作符（操作符以字节值 0~21 开始，其中双字节操作符以字节值 12 开头），操作符之前
// 是给出其值的一个或多个操作数，一个操作符最多 48 个操作数。dictionary values 被编码为
// 变长数值操作数，可表示整数或实数。对于具有默认值的键，DICT 中相应操作符的缺失即意味着
// 该键应取其默认值。

prh_i32 prh_font_cff1_dict_value_integer(const prh_byte *b, prh_r32 *value_bytes) {
    if (b[0] >= 32 && b[0] <= 246) { *value_bytes = 1; return (prh_i32)b[0] - 139; } // -107 ~ +107
    if (b[0] >= 247 && b[0] <= 250) { *value_bytes = 2; return ((prh_i32)b[0] - 247) * 256 + (prh_i32)b[1] + 108; } // +108 ~ +1131
    if (b[0] >= 251 && b[0] <= 254) { *value_bytes = 2; return -((prh_i32)b[0] - 251) * 256 - (prh_i32)b[1] - 108; } // -1131 ~ -108
    if (b[0] == 28) { *value_bytes = 3; return ((prh_i32)b[1] << 8) | b[2]; } // -32768 ~ + 32767
    if (b[0] == 29) { *value_bytes = 5; return ((prh_i32)b[1] << 24) | ((prh_i32)b[2] << 16) | ((prh_i32)b[3] << 8) | b[4]; } // –(2^31) ~ +(2^31–1)
    *value_bytes = 0; return 0;
}

prh_r32 prh_font_cff1_dict_value_bytes(const prh_byte *b) {
    if (b[0] <= 21) { return b[0] == 12 ? 2 : 1; }
    if (b[0] >= 32 && b[0] <= 246) return 1;
    if (b[0] >= 247 && b[0] <= 250) return 2;
    if (b[0] >= 251 && b[0] <= 254) return 2;
    if (b[0] == 28) return 3;
    if (b[0] == 29) return 5;
    if (b[0] == 30) {
        prh_r32 i = 1;
        prh_byte a;
        bool finish;
        for (; ;) {
            a = b[i] >> 4;
            finish = a == 0xf;
            a = b[i++] & 0xf;
            if (finish) {
                if (a != 0xf) prh_abort_error(0xf);
                return i;
            }
            if (a == 0xf) return i;
        }
    }
    prh_abort_error(b[0]);
    return 0;
}

#include <math.h>

double prh_font_cff1_dict_value_float(const prh_byte *b, prh_r32 *value_bytes) {
    double fpart = 0.0;
    double scale = 0.1;
    prh_r32 ipart = 0;
    prh_r32 epart = 0;
    prh_r32 i = 1;
    prh_byte a;
    bool negative = false;
    bool finish = false;
    bool point = false;
    bool exponent = false;
    bool neg_exp = false;
    if (b[0] != 30) { *value_bytes = 0; return 0; }
    for (; ;) {
        switch ((a = b[i] >> 4)) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            if (exponent) epart = epart * 10 + a;
            else if (point) { fpart += scale * a; scale *= 0.1; }
            else ipart = ipart * 10 + a;
            break;
        case 0xa: point = true; break;
        case 0xb: exponent = true; break;
        case 0xc: exponent = true; neg_exp = true; break;
        case 0xe: negative = true; break;
        case 0xf: finish = true; break;
        default: break;
        }
        a = b[i++] & 0xf;
        if (finish) {
            if (a != 0xf) prh_abort_error(0xf);
            goto label_parse_finish;
        }
        switch (a) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            if (exponent) epart = epart * 10 + a;
            else if (point) { fpart += scale * a; scale *= 0.1; }
            else ipart = ipart * 10 + a;
            break;
        case 0xa: point = true; break;
        case 0xb: exponent = true; break;
        case 0xc: exponent = true; neg_exp = true; break;
        case 0xe: negative = true; break;
        case 0xf: goto label_parse_finish;
        default: break;
        }
    }
label_parse_finish:
    *value_bytes = i;
    fpart += ipart;
    if (exponent) {
        fpart *= pow(10.0, neg_exp ? -(double)epart : (double)epart);
    }
    if (negative) fpart = -fpart;
    return fpart;
}

// INDEX 数据
//
// INDEX 是可变⼤⼩对象的数组，由头部、偏移量数组和对象数据组成。偏移量数组指定对象数据
// 内的偏移量。检索对象时，先索引偏移量数组，再按指定的偏移量取出对象。对象⻓度可通过⽤
// 偏移量数组中的下⼀个偏移量减去其偏移量得到。偏移量数组末尾额外附加⼀个偏移量，以便确
// 定最后⼀个对象的⻓度。INDEX 格式如下表所⽰。
//
//      类型    名称                描述
//      Card16  count               INDEX 中存储的对象数
//      OffSize offSize             偏移量数组元素⼤⼩
//      Offset  offset [count+1]    偏移量数组（⾃对象数据前⼀字节起）
//      Card8   data[<可变>]        对象数据
//
// 偏移量数组中的偏移量相对于对象数据之前的那个字节。因此偏移量数组的第⼀个元素总是 1。
// 这确保每个对象都有对应的⾮零偏移量，并允许⾼效地实现对象的动态加载。空 INDEX 由值为
// 0 的 count 字段表⽰，且不含其他字段。因此空 INDEX 的总⼤⼩为 2 字节。注：可通过跳转
// 到偏移量数组最后⼀个元素所指定的偏移量来跳过整个 INDEX。

typedef struct {
    prh_r16 object_count;
    prh_r08 offset_size; // 1 ~ 4
    prh_r08 object_offset[1]; // 大小为 offset_size * (object_count + 1)
} prh_font_cff1_index_data;

prh_r32 prh_font_cff1_offset_value(prh_r08 *offset, prh_r08 offset_size, prh_r32 offset_index) {
    offset += offset_size * offset_index;
    switch (offset_size) {
        case 2: return prh_bp_2b_to_host(offset);
        case 3: return prh_bp_3b_to_host(offset);
        case 4: return prh_bp_4b_to_host(offset);
        default: return *offset;
    }
}

prh_r32 prh_font_cff1_object_offset(prh_r08 *offset_array, prh_r08 offset_size, prh_r32 object_index) {
    prh_r32 offset_value = prh_font_cff1_offset_value(offset_array, offset_size, object_index);
    prh_assert(offset_value > 0);
    return offset_value - 1;
}

prh_r32 prh_font_cff1_object_size(prh_r08 *offset_array, prh_r08 offset_size, prh_r32 object_index, prh_r32 *object_size) {
    prh_r32 offset_value = prh_font_cff1_offset_value(offset_array, offset_size, object_index);
    prh_r32 next_offset = prh_font_cff1_offset_value(offset_array, offset_size, object_index + 1);
    prh_assert(offset_value > 0 && next_offset >= offset_value);
    *object_size = next_offset - offset_value;
    return offset_value - 1;
}

prh_r32 prh_font_cff1_index_data_size(prh_byte *index_data) {
    prh_r16 object_count = prh_bp_2b_to_host(index_data);
    if (object_count == 0) return 2;
    prh_r08 offset_size = *(index_data + 2);
    prh_r32 header_bytes = 3 + offset_size * (object_count + 1);
    prh_assert(offset_size >= 1 && offset_size <= 4);
    return header_bytes + prh_font_cff1_object_offset(index_data + 3, offset_size, object_count);
}

prh_r08 *prh_font_cff1_object_data(prh_byte *index_data, prh_r16 object_index, prh_r32 *object_size) {
    prh_r16 object_count = prh_bp_2b_to_host(index_data);
    if (object_index >= object_count) return prh_null;
    prh_r08 offset_size = *(index_data + 2); prh_assert(offset_size >= 1 && offset_size <= 4);
    prh_r32 header_bytes = 3 + offset_size * (object_count + 1);
    return index_data + header_bytes + prh_font_cff1_object_size(index_data + 3, offset_size, object_index, object_size);
}

// 头部（Header）
//
// ⼆进制数据以头部开始，其格式如下表所⽰。头部格式：
//      类型    名称        描述
//      Card8   major       格式主版本号（从 1 开始）
//      Card8   minor       格式次版本号（从 0 开始）
//      Card8   hdrSize     头部⼤⼩（字节）
//      OffSize offSize     绝对偏移量 (0) 的⼤⼩
//
// 读取字体集（font set）⽂件的实现必须包含检查版本号的代码，以便当格式及版本号发⽣变化时，
// 旧实现能够妥善地拒绝新版本。如果实现理解主版本号，就可以安全地继续读取字体。次版本号表
// ⽰格式的扩展；不⽀持这些扩展的实现⽆法察觉它们，因⽽也⽆法利⽤这些扩展。
//
// 定位 Name INDEX 时必须使⽤ hdrSize 字段。设置该字段是为了让格式的未来版本能够在 offSize
// 字段与 Name INDEX 之间引⼊额外数据，同时与旧实现保持兼容。offSize 字段指定所有相对于 CFF
// 数据起点的偏移量 (0) 的⼤⼩。

typedef struct {
    prh_r08 major_version;
    prh_r08 minor_version;
    prh_r08 header_length;
    prh_r08 offset_size;
} prh_font_cff1_header;

// Name INDEX
//
// 其中包含 FontSet 内所有字体的 PostScript 语⾔名称（FontName 或 CIDFontName），存储在⼀个
// INDEX 结构中。字体名称经过排序，因⽽在 FontSet 内定位特定字体时可以执⾏⼆分查找。排序基于
// 按 8 位⽆符号整数处理的字符代码。若⼀字体名称是另⼀字体名称的前缀，则较短者排在前⾯。该
// INDEX 中⾄少要有⼀个条⽬，即 FontSet ⾄少要包含⼀个字体。
//
// 为与 PostScript 解释器、Acrobat 等客⼾端软件兼容，字体名称不应超过 127 个字符，且不应包含
// 以下任何 ASCII 字符：
//      []
//      ()
//      {}
//      <>
//      /
//      %
//      null（NUL）
//      空格（SP）
//      制表符（\t）
//      回⻋（\r）
//      换⾏（\n）
//      换⻚（\f）
//
// 建议将字体名称限制在可打印 ASCII ⼦集（代码 33 ⾄ 126）之内。Adobe Type Manager（ATM）软
// 件进⼀步将字体名称⻓度限制为 63 个字符。注：有关与早期 PostScript 解释器的兼容性，参⻅技
// 术说明 5088《Font Naming Issues》。
//
// ⽆需移除字体数据即可将字体从 FontSet 中删除：只需将其名称在 Name INDEX 中的⾸字节设为 0
// （NUL）。这种删除⽅式为处理字体升级提供了⼀种简单办法，⽆需重建整个字体集。⼆分查找软件
// 必须能检测已删除项，并从 INDEX 中的前⼀个或后⼀个名称重新开始查找，以确保所有合适的名称
// 都能被匹配。

typedef struct {
    prh_byte *table_data;
    prh_r32 table_length;
    prh_r08 major_version;
    prh_r08 minor_version;
    prh_r08 header_length;
    prh_r08 offset_size;
    prh_byte *name_index_data;
    prh_byte *cff_font_name;
    prh_byte *top_dict_index;
    prh_byte *top_dict_data;
    prh_byte *string_index;
    prh_byte *global_subrs_index;
    prh_byte *charstrings_index;
    prh_r32 name_index_bytes;
    prh_r32 font_name_bytes;
    prh_r32 top_dict_index_bytes;
    prh_r32 top_dict_length;
    prh_r32 string_index_bytes;
    prh_r32 global_subrs_index_bytes;
    prh_r32 global_subr_count;
    prh_r32 charstrings_offset;
    prh_r32 charstrings_index_bytes;
    prh_r32 charstrings_glyphs;
    prh_r32 charset_offset;
} prh_font_cff1_table;

prh_r32 prh_impl_font_cff1_index_objects(prh_byte *index) {
    return prh_bp_2b_to_host(index);
}

prh_r32 prh_impl_font_cff1_index_offsize(prh_byte *index) {
    return index[2];
}

void prh_font_cff1_load_name_index(prh_font_cff1_table *p) {
    p->name_index_data = p->table_data + p->header_length;
    p->name_index_bytes = prh_font_cff1_index_data_size(p->name_index_data);
    if (prh_impl_font_cff1_index_objects(p->name_index_data) == 0) prh_abort_error(__LINE__);
    p->cff_font_name = prh_font_cff1_object_data(p->name_index_data, 0, &p->font_name_bytes);
}

void prh_font_cff1_parse_top_dict(prh_font_cff1_table *p);

void prh_font_cff1_load_top_dict_index(prh_font_cff1_table *p) {
    p->top_dict_index = p->name_index_data + p->name_index_bytes;
    p->top_dict_index_bytes = prh_font_cff1_index_data_size(p->top_dict_index);
    if (prh_impl_font_cff1_index_objects(p->top_dict_index) == 0) prh_abort_error(__LINE__);
    p->top_dict_data = prh_font_cff1_object_data(p->top_dict_index, 0, &p->top_dict_length);
    prh_font_cff1_parse_top_dict(p);
}

void prh_font_cff1_load_string_index(prh_font_cff1_table *p) {
    p->string_index = p->top_dict_index + p->top_dict_index_bytes;
    p->string_index_bytes = prh_font_cff1_index_data_size(p->string_index);
}

void prh_font_cff1_load_global_subrs_index(prh_font_cff1_table *p) {
    p->global_subrs_index = p->string_index + p->string_index_bytes;
    p->global_subrs_index_bytes = prh_font_cff1_index_data_size(p->global_subrs_index);
    p->global_subr_count = prh_impl_font_cff1_index_objects(p->global_subrs_index);
}

void prh_font_cff1_load_charstrings_index(prh_font_cff1_table *p) {
    p->charstrings_index = p->table_data + p->charstrings_offset;
    p->charstrings_index_bytes = prh_font_cff1_index_data_size(p->charstrings_index);
    p->charstrings_glyphs = prh_impl_font_cff1_index_objects(p->charstrings_index);
    if (p->charstrings_glyphs == 0) prh_abort_error(__LINE__);
}

void prh_print_cff1_dict_data(prh_byte *dict, prh_r32 dict_length);
void prh_print_cff1_string_index(prh_font_cff1_table *p);
void prh_print_cff1_global_subrs_index(prh_font_cff1_table *p);
void prh_print_cff1_charstrings_index(prh_font_cff1_table *p);
void prh_print_cff1_charset(prh_font_cff1_table *p);

void prh_print_font_cff1(prh_open_font *f) {
    if (f->cff1.length == 0) return;
    prh_byte *table_data = prh_load_font_table(f, &f->cff1);
    prh_font_cff1_header *p = (prh_font_cff1_header *)table_data;
    if (p->header_length < sizeof(prh_font_cff1_header) || f->cff1.length < p->header_length) {
        prh_print("table 'cff ' invalid length %d %d\n", (prh_reg)p->header_length, (prh_reg)f->cff1.length);
        return;
    }

    prh_r32 checksum = prh_font_table_checksum((prh_r32 *)table_data, f->cff1.length);

    prh_font_cff1_table cff;
    cff.table_data = table_data;
    cff.table_length = f->cff1.length;
    cff.major_version = p->major_version;
    cff.minor_version = p->minor_version;
    cff.header_length = p->header_length;
    cff.offset_size = p->offset_size;
    prh_font_cff1_load_name_index(&cff);

    prh_print(
        "cff1 table index %d / %d\n"
        "cff1 table tag 0x%08x (%c%c%c%c)\n"
        "cff1 table offset %.10d (%d/4)\n"
        "cff1 table length %.10d (%d/4)\n"
        "cff1 table checksum 0x%08x 0x%08x (valid %d)\n"
        "cff1 version %d.%d\n"
        "cff1 header length %d\n"
        "cff1 offset size %d\n"
        "----------------------------\n"
        "cff1 Name INDEX offset %d\n"
        "cff1 Name INDEX length %d\n"
        "cff1 Name INDEX object offsize %d\n"
        "cff1 Name INDEX object 1 / %d\n"
        "cff1 Name INDEX object 1 offset %d\n"
        "cff1 Name INDEX object 1 length %d\n"
        "cff1 Name INDEX object 1 '%s'\n",
        (prh_reg)f->cff1.table_index,
        (prh_reg)prh_font_table_count(f),
        (prh_reg)f->cff1.tabletag,
        (prh_reg)prh_byte_4(f->cff1.tabletag),
        (prh_reg)prh_byte_3(f->cff1.tabletag),
        (prh_reg)prh_byte_2(f->cff1.tabletag),
        (prh_reg)prh_byte_1(f->cff1.tabletag),
        (prh_reg)f->cff1.offset, (prh_reg)f->cff1.offset % 4,
        (prh_reg)f->cff1.length, (prh_reg)f->cff1.length % 4,
        (prh_reg)f->cff1.checksum, (prh_reg)checksum, (prh_reg)(checksum == f->cff1.checksum),
        (prh_reg)cff.major_version,
        (prh_reg)cff.minor_version,
        (prh_reg)cff.header_length,
        (prh_reg)cff.offset_size,
        (prh_reg)cff.header_length,
        (prh_reg)cff.name_index_bytes,
        (prh_reg)prh_impl_font_cff1_index_offsize(cff.name_index_data),
        (prh_reg)prh_impl_font_cff1_index_objects(cff.name_index_data),
        (prh_reg)(cff.cff_font_name - cff.name_index_data),
        (prh_reg)cff.font_name_bytes,
        (cff.cff_font_name && *cff.cff_font_name) ? cff.cff_font_name : "NULL");

    prh_font_cff1_load_top_dict_index(&cff);
    prh_print(
        "----------------------------\n"
        "cff1 Top DICT INDEX offset %d\n"
        "cff1 Top DICT INDEX length %d\n"
        "cff1 Top DICT INDEX object offsize %d\n"
        "cff1 Top DICT object 1 / %d\n"
        "cff1 Top DICT object 1 offset %d\n"
        "cff1 Top DICT object 1 length %d\n"
        "----------------------------\n",
        (prh_reg)(cff.top_dict_index - cff.table_data),
        (prh_reg)cff.top_dict_index_bytes,
        (prh_reg)prh_impl_font_cff1_index_offsize(cff.top_dict_index),
        (prh_reg)prh_impl_font_cff1_index_objects(cff.top_dict_index),
        (prh_reg)(cff.top_dict_data - cff.top_dict_index),
        (prh_reg)cff.top_dict_length);
    prh_print_cff1_dict_data(cff.top_dict_data, cff.top_dict_length);

    prh_font_cff1_load_string_index(&cff);
    prh_print(
        "----------------------------\n"
        "cff1 String INDEX offset %d\n"
        "cff1 String INDEX length %d\n"
        "cff1 String INDEX object offsize %d\n"
        "cff1 String INDEX string count %d\n",
        (prh_reg)(cff.string_index - cff.table_data),
        (prh_reg)cff.string_index_bytes,
        (prh_reg)prh_impl_font_cff1_index_offsize(cff.string_index),
        (prh_reg)prh_impl_font_cff1_index_objects(cff.string_index));
    prh_print_cff1_string_index(&cff);

    prh_font_cff1_load_global_subrs_index(&cff);
    prh_print(
        "----------------------------\n"
        "cff1 Global Subrs INDEX offset %d\n"
        "cff1 Global Subrs INDEX length %d\n"
        "cff1 Global Subrs INDEX object offsize %d\n"
        "cff1 Global Subrs INDEX subr count %d\n"
        "----------------------------",
        (prh_reg)(cff.global_subrs_index - cff.table_data),
        (prh_reg)cff.global_subrs_index_bytes,
        (prh_reg)prh_impl_font_cff1_index_offsize(cff.global_subrs_index),
        (prh_reg)prh_impl_font_cff1_index_objects(cff.global_subrs_index));
    prh_print_cff1_global_subrs_index(&cff);

    prh_font_cff1_load_charstrings_index(&cff);
    prh_print_cff1_charset(&cff);

    prh_print_cff1_charstrings_index(&cff);

    prh_da_free(table_data);
}

// Top DICT INDEX
//
// 其中包含 FontSet 内所有字体的顶层 DICT，存储在⼀个 INDEX 结构中。该 INDEX 所含对象与
// Name INDEX 中的对象在顺序和数量上⼀⼀对应。每个对象都是⼀个 DICT 结构，对应于⼀个
// PostScript 字体的顶层字典。
//
// 一个字体由 Name INDEX 中的⼀个条⽬标识，其数据通过对应的 Top DICT 访问。
//
// Top DICT 数据
//
// 下表所⽰ Top DICT 操作符的名称尽可能与对应的 Type 1 字典键（Type 1 dict key）保持⼀致。
// 没有对应 Type 1 字典键的操作符在下表中注明，并给出默认值（如有）。有若⼲操作符源⾃
// FontInfo 字典键，但为简便起⻅与 Top DICT 操作符列在⼀起；来⾃ FontInfo 字典的键在下表
// 的 “默认值/注释” 列中标出。Top DICT 操作符条⽬：
//
//      名称                值      操作数      默认值/注释
//      version             0       SID         –，FontInfo
//      Notice              1       SID         –，FontInfo
//      Copyright           12 0    SID         –，FontInfo
//      FullName            2       SID         –，FontInfo
//      FamilyName          3       SID         –，FontInfo
//      Weight              4       SID         –，FontInfo
//      isFixedPitch        12 1    boolean     0（假），FontInfo
//      ItalicAngle         12 2    number      0，FontInfo
//      UnderlinePosition   12 3    number      –100，FontInfo
//      UnderlineThickness  12 4    number      50，FontInfo
//      PaintType           12 5    number      0
//      CharstringType      12 6    number      2
//      FontMatrix          12 7    array       0.001 0 0 0.001 0 0
//      UniqueID            13      number      –
//      FontBBox            5       array       0 0 0 0
//      StrokeWidth         12 8    number      0
//      XUID                14      array       –
//      charset             15      number      0，charset 偏移量 (0)
//      Encoding            16      number      0，encoding 偏移量 (0)
//      CharStrings         17      number      –，CharStrings 偏移量 (0)
//      Private             18    number number –，Private DICT ⼤⼩和偏移量 (0)
//      SyntheticBase       12 20   number      –，合成基础字体索引
//      PostScript          12 21   SID         –，嵌⼊的 PostScript 语⾔代码
//      BaseFontName        12 22   SID         –，（由基于 Adobe 的技术按需添加）
//      BaseFontBlend       12 23   delta       –，（由基于 Adobe 的技术按需添加）
//
// 嵌⼊的 PostScript 操作符提供了⼀种转义机制，可⽤于解决打印机字体中的可扩展性或兼容性
// 问题（参⻅附录 F）。
//
// 将字典数据分为顶层字典和 Private 字典，反映了 Type 1 的⽤法：Top DICT 数据在 findfont
// 时被解析，⽤于构造合法的 Type 1 字体字典；Private 操作符的值给出⼀个⼤⼩和⼀个偏移量，
// 在字体渲染时据此定位，以构造与 Private DICT 数据相关的数据结构。
//
// BaseFontName 和 BaseFontBlend 操作符被添加到嵌⼊ PDF ⽂档中的多⺟板（multiple master）
// 字体实例中。BaseFontName 是原始多⺟板字体的 FontName，BaseFontBlend 是⽤于计算该实例
// 的⽤⼾设计向量（User Design Vector）。
//
// CIDFont 还需要下表所⽰的附加 Top DICT 操作符。CIDFont 操作符扩展：
//      名称                值      操作数              默认值/注释
//      ROS                 12 30   SID SID number      –，Registry Ordering Supplement
//      CIDFontVersion      12 31   number              0
//      CIDFontRevision     12 32   number              0
//      CIDFontType         12 33   number              0
//      CIDCount            12 34   number              8720
//      UIDBase             12 35   number              –
//      FDArray             12 36   number              –，Font DICT（FD）INDEX 偏移量 (0)
//      FDSelect            12 37   number              –，FDSelect 偏移量 (0)
//      FontName            12 38   SID                 –，FD FontName
//
// ROS 操作符将 Registry、Ordering 和 Supplement 三个键组合在⼀起。CIDFont 在第 18 节中有
// 完整描述。合成字体的 Top DICT 以 SyntheticBase 操作符开头，CIDFont 则以 ROS 操作符开头；
// 常规 Type 1 字体以其他操作符开头。这样⽆需解析整个 Top DICT 即可判断字体的种类。
//
// Private DICT 数据
//
// 下表所⽰ Private DICT 操作符的名称尽可能与对应的 Type 1 字典键保持⼀致。没有对应 Type 1
// 字典键的操作符在下表中注明。
//
//      Private DICT 操作符
//      名称                值      操作数      默认值/注释
//      BlueValues          6       delta       –
//      OtherBlues          7       delta       –
//      FamilyBlues         8       delta       –
//      FamilyOtherBlues    9       delta       –
//      BlueScale           12 9    number      0.039625
//      BlueShift           12 10   number      7
//      BlueFuzz            12 11   number      1
//      StdHW               10      number      –
//      StdVW               11      number      –
//      StemSnapH           12 12   delta       –
//      StemSnapV           12 13   delta       –
//      ForceBold           12 14   boolean     false
//      LanguageGroup       12 17   number      0
//      ExpansionFactor     12 18   number      0.06
//      initialRandomSeed   12 19   number      0
//      Subrs               19      number      –，局部⼦例程的偏移量 (self)
//      defaultWidthX       20      number      0，⻅下⽂
//      nominalWidthX       21      number      0，⻅下⽂
//
// 局部⼦例程（local subrs）的偏移量相对于 Private DICT 数据的起点。
//
// defaultWidthX 和 nominalWidthX 操作符为字形提供宽度值。若字形宽度等于 defaultWidthX 值，
// 则可在 charstring 中省略；否则字形宽度等于 charstring 中的宽度加上 nominalWidthX 值。若
// 精⼼选择 nominalWidthX，charstring 中⼤部分宽度可从 2 字节数缩减为单字节数，从⽽节省空
// 间。
//
// OtherBlues 和 FamilyOtherBlues 操作符必须分别出现在 BlueValues 和 FamilyBlues 操作符之
// 后。Private DICT 是必需的；但如果没有需要存储的⾮默认值，可将其⻓度指定为 0。

#define PRH_CFF_OP_VERSION              0x00 // 0
#define PRH_CFF_OP_NOTICE               0x01 // 1
#define PRH_CFF_OP_FULLNAME             0x02 // 2
#define PRH_CFF_OP_FAMILYNAME           0x03 // 3
#define PRH_CFF_OP_WEIGHT               0x04 // 4
#define PRH_CFF_OP_FONTGOX              0x05 // 5
#define PRH_CFF_OP_BLUEVALUES           0x06 // 6
#define PRH_CFF_OP_OTHERBLUES           0x07 // 7
#define PRH_CFF_OP_FAMILYBLUES          0x08 // 8
#define PRH_CFF_OP_FAMILYOTHERBLUES     0x09 // 9
#define PRH_CFF_OP_STDHW                0x0A // 10
#define PRH_CFF_OP_STDVW                0x0B // 11
#define PRH_CFF_OP_ESCAPE               0x0C // 12
#define PRH_CFF_OP_UNIQUEID             0x0D // 13
#define PRH_CFF_OP_XUID                 0x0E // 14
#define PRH_CFF_OP_CHARSET              0x0F // 15
#define PRH_CFF_OP_ENCODING             0x10 // 16
#define PRH_CFF_OP_CHARSTRINGS          0x11 // 17
#define PRH_CFF_OP_PRIVATE              0x12 // 18
#define PRH_CFF_OP_SUBRS                0x13 // 19
#define PRH_CFF_OP_DEFAULTWIDTHX        0x14 // 20
#define PRH_CFF_OP_NOMINALWIDTHX        0x15 // 21

#define PRH_CFF_EOP_COPYRIGHT           0x00 // 0
#define PRH_CFF_EOP_ISFIXEDPITCH        0x01 // 1
#define PRH_CFF_EOP_ITALICANGLE         0x02 // 2
#define PRH_CFF_EOP_UNDERLINEPOSITION   0x03 // 3
#define PRH_CFF_EOP_UNDERLINETHICKNESS  0x04 // 4
#define PRH_CFF_EOP_PAINTTYPE           0x05 // 5
#define PRH_CFF_EOP_CHARSTRINGTYPE      0x06 // 6
#define PRH_CFF_EOP_FONTMATRIX          0x07 // 7
#define PRH_CFF_EOP_STROKEWIDTH         0x08 // 8
#define PRH_CFF_EOP_BLUESCALE           0x09 // 9
#define PRH_CFF_EOP_BLUESHIFT           0x0A // 10
#define PRH_CFF_EOP_BLUEFUZZ            0x0B // 11
#define PRH_CFF_EOP_STEMSNAPH           0x0C // 12
#define PRH_CFF_EOP_STEMSNAPV           0x0D // 13
#define PRH_CFF_EOP_FORCEBOLD           0x0E // 14
#define PRH_CFF_EOP_LANGUAGEGROUP       0x11 // 17
#define PRH_CFF_EOP_EXPANSIONFACTOR     0x12 // 18
#define PRH_CFF_EOP_INITIALRANDOMSEED   0x13 // 19
#define PRH_CFF_EOP_SYNTHETICBASE       0x14 // 20
#define PRH_CFF_EOP_POSTSCRIPT          0x15 // 21
#define PRH_CFF_EOP_BASEFONTNAME        0x16 // 22
#define PRH_CFF_EOP_BASEFONTBLEND       0x17 // 23
#define PRH_CFF_EOP_ROS                 0x1E // 30
#define PRH_CFF_EOP_CIDFONTVERSION      0x1F // 31
#define PRH_CFF_EOP_CIDFONTREVISION     0x20 // 32
#define PRH_CFF_EOP_CIDFONTTYPE         0x21 // 33
#define PRH_CFF_EOP_CIDCOUNT            0x22 // 34
#define PRH_CFF_EOP_UIDBASE             0x23 // 35
#define PRH_CFF_EOP_FDARRAY             0x24 // 36
#define PRH_CFF_EOP_FDSELECT            0x25 // 37
#define PRH_CFF_EOP_FONTNAME            0x26 // 38

void prh_font_cff1_parse_top_dict(prh_font_cff1_table *p) {
    prh_r32 count = 0;
    prh_r32 operand_count = 0;
    prh_i32 integer = 0;
    while (count < p->top_dict_length) {
        prh_byte *b = p->top_dict_data + count;
        count += prh_font_cff1_dict_value_bytes(b);
        if (*b <= 21) { // 0 ~ 21 表⽰操作符
            if (*b == PRH_CFF_OP_CHARSET) {
                prh_assert(operand_count == 1);
                p->charset_offset = integer;
            } else if (*b == PRH_CFF_OP_CHARSTRINGS) {
                prh_assert(operand_count == 1);
                p->charstrings_offset = integer;
            }
            operand_count = 0;
        } else { // 字节值 22 ~ 27、31 和 255 为保留值
            prh_r32 value_bytes;
            operand_count += 1;
            if (*b != 30) { // 28、29、30 和 32–254 表⽰操作数（数值）
                integer = prh_font_cff1_dict_value_integer(b, &value_bytes);
            } else {
                integer = 0;
            }
        }
    }
    prh_assert(count == p->top_dict_length);
}

const char *prh_impl_print_font_cff_operator(prh_byte *p) {
    const char *oper[] = {
        "version", "Notice", "FullName", "FamilyName", "Weight", "FontBBox", "BlueValues", "OtherBlues",
        "FamilyBlues", "FamilyOtherBlues", "StdHW", "StdVW", "escape", "UniqueID", "XUID", "charset",
        "Encoding", "CharStrings", "Private", "Subrs", "defaultWidthX", "nominalWidthX"};
    const char *ext_oper[] = {
        "Copyright", "isFixedPitch", "ItalicAngle", "UnderlinePosition", "UnderlineThickness", "PaintType", "CharstringType", "FontMatrix",
        "StrokeWidth", "BlueScale", "BlueShift", "BlueFuzz", "StemSnapH", "StemSnapV", "ForceBold", "Reserved",
        "Reserved", "LanguageGroup", "ExpansionFactor", "initialRandomSeed", "SyntheticBase", "PostScript", "BaseFontName", "BaseFontBlend",
        "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "ROS", "CIDFontVersion",
        "CIDFontRevision", "CIDFontType", "CIDCount", "UIDBase", "FDArray", "FDSelect", "FontName"};
    if (p[0] != 12) return oper[p[0]];
    if (p[1] <= 38) return ext_oper[p[1]];
    return "Unknown";
}

void prh_print_cff1_dict_data(prh_byte *dict, prh_r32 dict_length) {
    prh_r32 bytes, count = 0;
    prh_r32 operand_count = 0;
    prh_r32 value_count = 0;
    while (count < dict_length) {
        prh_byte *b = dict + count;
        bytes = prh_font_cff1_dict_value_bytes(b);
        value_count += 1;
        prh_print("DICT DATA [%02d] %04d %04d len %02d: ",
            (prh_reg)value_count,
            (prh_reg)dict_length,
            (prh_reg)count,
            (prh_reg)bytes);
        count += bytes;
        for (prh_r32 i = 0; i < bytes; i += 1) prh_print("%02x ", (prh_reg)b[i]);
        if (bytes * 3 < 21) prh_print_byte(' ', 21 - bytes * 3);
        if (*b <= 21) { // 0 ~ 21 表⽰操作符
            prh_print("OPERATOR(%d) %s\n", (prh_reg)operand_count, prh_impl_print_font_cff_operator(b));
            prh_real_assert(operand_count <= 48); // ⼀个操作符之前最多可以有 48 个操作数
            operand_count = 0;
        } else { // 字节值 22 ~ 27、31 和 255 为保留值
            prh_r32 value_bytes;
            operand_count += 1;
            if (*b == 30) prh_print("FLOAT TODO\n");
            else { // 28、29、30 和 32–254 表⽰操作数（数值）
                prh_i32 integer = prh_font_cff1_dict_value_integer(b, &value_bytes);
                prh_print("INTEGER %d\n", (prh_reg)integer);
                prh_real_assert(bytes == value_bytes);
            }
        }
    }
    prh_real_assert(count == dict_length);
}

// String INDEX
//
// FontSet 内各字体使⽤的所有字符串（出现在 Name INDEX 中的 FontName 和 CIDFontName 除外）
// 被汇集到⼀个 INDEX 结构中，并由⼀个称为字符串标识符（SID）的 2 字节⽆符号数引⽤。表中只
// 存储不重复的字符串，从⽽消除了字体之间的重复。此外，通过为常⽤字符串分配预定义 SID 还可
// 进⼀步节省空间。这些字符串称为标准字符串，描述了 ISOAdobe 和 Expert 字符集中使⽤的所有
// 名称，以及 Type 1 字体中常⻅的其他⼀些字符串。标准字符串的完整列表⻅附录 A。
//
// 客⼾端程序内含⼀个具有 nStdStrings 个元素的标准字符串数组。因此标准字符串占⽤ 0 到
// (nStdStrings – 1) 范围内的 SID。String INDEX 中的第⼀个字符串对应 SID 值等于 nStdStrings
// 的字符串（即第⼀个⾮标准字符串），依此类推。当客⼾端需要确定某个 SID 对应的字符串时，执
// ⾏如下操作：判断 SID 是否在标准范围内，若是则从内部表中取出；否则以 (SID – nStdStrings)
// 为索引从 String INDEX 中取出字符串。
//
// SID 定义为 2 字节⽆符号数，但只取 0–64999（含）范围内的值。SID 值 65000 及以上可供实现
// 使⽤。不含⾮标准字符串的 FontSet ⽤空 INDEX 表⽰。

#define PRH_CFF_STD_STRINGS 391

const char *prh_font_cff1_get_string(prh_font_cff1_table *p, prh_r16 sid, prh_r32 *string_bytes) {
    if (sid < PRH_CFF_STD_STRINGS) {
        *string_bytes = 3;
        return "std";
    }
    if ((prh_r32)(sid - PRH_CFF_STD_STRINGS) >= prh_impl_font_cff1_index_objects(p->string_index)) {
        *string_bytes = 0;
        return "";
    }
    return prh_font_cff1_object_data(p->string_index, sid - PRH_CFF_STD_STRINGS, string_bytes);
}

void prh_print_cff1_string_index(prh_font_cff1_table *p) {
    prh_r32 string_count = prh_impl_font_cff1_index_objects(p->string_index);
    if (string_count == 0) return;

    prh_r32 n = string_count > 32 ? 32 : string_count;
    prh_r32 string_bytes;

    for (prh_r16 i = 0; i < n; i += 1) {
        prh_byte *string = prh_font_cff1_object_data(p->string_index, i, &string_bytes);
        prh_print(
            "cff1 String INDEX string %02d SID %03d: %03d '%Ls'\n",
            (prh_reg)(i + 1),
            (prh_reg)(PRH_CFF_STD_STRINGS + i),
            (prh_reg)string_bytes,
            (prh_reg)string_bytes,
            string);
    }
}

// 局部/全局子例程 INDEX
//
// Type 1 和 Type 2 charstring 都⽀持⼦例程（subr）的概念。subr 通常是⼀段 charstring 字节
// 序列，表⽰在字体 charstring 数据中多处出现的⼦程序。该 subr 只需存储⼀次，即可通过 callsubr
// 操作符（其操作数为要调⽤的 subr 编号）从⼀个或多个 charstring 中多次引⽤。subr 是特定字
// 体私有的，不能在字体之间共享。
//
// Type 2 charstring 还允许全局 subr，其⼯作⽅式相同，但由 callgsubr 操作符调⽤，且可跨字体
// 共享。局部 subr 存储在⼀个 INDEX 结构中，通过 Private DICT 中 Subrs 操作符的偏移量操作数
// 定位。没有局部 subr 的字体，其 Private DICT 中没有 Subrs 操作符。
//
// 全局 subr 存储在紧随 String INDEX 之后的 INDEX 结构中。不含全局 subr 的 FontSet ⽤空的
// Global Subrs INDEX 表⽰。
//
// Type 2 charstring 中的 subr 编号要加上⼀个称为 “subr 编号偏置”（subr number bias）的数
// 值，该偏置根据局部或全局 subr INDEX 中的⼦例程数量计算。偏置计算如下：
//      Card16 bias;
//      Card16 nSubrs = subrINDEX.count;
//      if (CharstringType == 1)
//          bias = 0;
//      else if (nSubrs < 1240)
//          bias = 107;
//      else if (nSubrs < 33900)
//          bias = 1131;
//      else
//          bias = 32768;
//
// 为正确选择 subr，必须在访问相应 subr INDEX 之前将算得的偏置加到 subr 编号操作数上。
// 该技术允许⽤负数和正数指定 subr 编号，从⽽充分利⽤可⽤的数值范围以节省空间。上述计
// 算免去了在字体中显式存储偏置的需要，Type 1 字体⽬前仍需显式存储。下面各表展⽰了不同
// 偏置⽅案下 subr 索引、编号、编号⼤⼩和区间数量的关系。
//
//      nSubrs < 1240，bias = 107
//      有序索引    重排索引    加偏置编号      ⼤⼩    数量
//      0 – 214     0 – 214     –107 – +107     1       215
//      215 – 1238  215 – 1238  +108 – +1131    2       1024
//                                               合计： 1239
//      nSubrs < 33900，bias = 1131
//      有序索引        重排索引        加偏置编号      ⼤⼩    数量
//      0 – 214         1024 – 1238     –107 – +107     1       215
//      215 – 1238      0 – 1023        –1131 – –108    2       1024
//      1239 – 2262     1239 – 2262     +108 – +1131    2       1024
//      2263 – 33898    2263 – 33898    +1132 – +32767  3       31636
//                                                       合计： 33899
//      nSubrs >= 33900，bias = 32768
//      有序索引        重排索引        加偏置编号      ⼤⼩    数量
//      0 – 214         32661 – 32875   –107 – +107     1       215
//      215 – 1238      31637 – 32660   –1131 – –108    2       1024
//      1239 – 2262     32876 – 33899   +108 – +1131    2       1024
//      2263 – 33899    0 – 31636       –32768 – –1132  3       31637
//      33900 – 65535   33900 – 65535   +1132 – +32767  3       31636
//                                                       合计： 65536
// 各列标题含义如下：
//  1.  有序索引，按 subr 使⽤频率从⾼到低排序的 subr 索引
//  2.  重排索引，为偏置⽽重排后的有序索引（即局部/全局 Subr INDEX 结构中 subr 的索引）
//  3.  加偏置编号，重排索引减去偏置后的值（callsubr/callgsubr 的操作数）
//  4.  ⼤⼩，加偏置编号的⼤⼩（字节）
//  5.  数量，区间内的 subr 数量

void prh_impl_print_cff1_charstring_glyph(prh_byte *glyph, prh_r32 length);

void prh_print_cff1_global_subrs_index(prh_font_cff1_table *p) {
    if (p->global_subr_count == 0) return;
    prh_r32 n = p->global_subr_count > 3 ? 3 : p->global_subr_count;
    prh_r32 global_subr_bytes;

    for (prh_r16 i = 0; i < n; i += 1) {
        prh_byte *global_subr = prh_font_cff1_object_data(p->global_subrs_index, i, &global_subr_bytes);
        prh_prinf(
                "\ncff1 Global Subr %d / %d offset %d length %d\n",
                (prh_reg)(i + 1),
                (prh_reg)p->global_subr_count,
                (prh_reg)(global_subr - p->global_subrs_index),
                (prh_reg)global_subr_bytes);
            prh_impl_print_cff1_charstring_glyph(global_subr, global_subr_bytes);
    }
}

// 合成字体（Synthetic Fonts）
//
// 合成字体是通过不同的变换矩阵或编码对另⼀字体加以修改⽽得到的字体。倾斜（obliqued）、
// 加宽（expanded）和压缩（condensed）字体都是可构造为合成字体的例⼦。合成字体具有名称
// 和引⽤基础字体的 Top DICT。Top DICT 可包含以下操作符：FullName、ItalicAngle、FontMatrix、
// SyntheticBase 和 Encoding。SyntheticBase 操作符是必需的，它指定⽤作基础字体的字体的
// 从零开始的索引。对该基础字体应⽤ FontMatrix 和/或 Encoding，即以算法⽅式创建新字体。
// 其他操作符的值将覆盖基础字体中给定的值。Top DICT 必须以 SyntheticBase 操作符开头。基
// 础字体不得是 CID 键控字体或合成字体。
//
// CID 键控字体（CID-keyed Fonts）
//
// CIDFont 的表⽰设计为与其编码相分离。遵循这⼀策略，CFF 表⽰不包含任何编码信息，编码信息
// 改由 CMap ⽂件承载。若将来需要更紧凑的 CMap ⽂件表⽰，可扩展 CFF 以容纳之。
//
// CFF CIDFont 在 Name INDEX 中具有 CIDFontName 和相应的 Top DICT。Top DICT 以 ROS 操作符
// 开头，该操作符指定字体的 Registry-Ordering-Supplement。这会告知 CFF 解析器应对此字体应
// ⽤特殊的 CID 处理。具体⽽⾔：
//  1.  FDArray 操作符应存在，其唯⼀参数指定指向 Font DICT INDEX 的偏移量。该数组中的每个
//      Font DICT 指定字体中某⼀组特定字形所独有的信息。字形到 Font DICT 的映射由下⽂描述
//      的 FDSelect 结构指定。每个 Font DICT 通过 Private DICT 操作符指定相应的 Private
//      DICT。
//  2.  字符集数据虽与⾮ CIDFont 格式相同，但表⽰的是 CID ⽽⾮ SID，即 CIDFont 中的 charstring
//      以 CID “命名”。在完整的 CIDFont 中，字符集表指定⼀个恒等映射（对所有字形 GID 等于
//      CID），即从 CID 1 开始（省略 CID 0，即 .notdef 字形）并覆盖字体中所有字形的单个区间。
//      ⼦集化的 CIDFont（Subset CIDFont）通常需要使⽤更复杂的字符集表来表⽰⾮恒等映射（CID
//      不等于 GID）。
//  3.  Top DICT 包含 FDSelect 操作符，指定指向⼀个类字符集数据结构（⻅下⼀节）的偏移量，该结
//      构包含⼀个（可能经区间编码的）索引列表，可从中为每个字形导出单个索引。该索引标识光栅
//      化字形时所⽤的 Font DICT 及其 Private DICT。
//  4.  编码数据被省略（⻅上⽂）：不存在 Encoding 操作符，也不应应⽤默认的 StandardEncoding。
//
// CID 字体没有预定义字符集。
//
// FDSelect
//
// FDSelect 通过为字形指定 FD 索引，将 FD（Font DICT）与字形关联起来。FD 索引⽤于访问存储在
// Font DICT INDEX 中的某个 Font DICT。FDSelect 数据通过 Top DICT 中 FDSelect 操作符的偏移量
// 操作数定位。FDSelect 数据由⼀个格式类型标识字节后跟格式特定数据组成。⽬前定义了两种格式：
//
//      格式 0
//      类型  名称          描述
//      Card8 format        =0
//      Card8 fds[nGlyphs]  FD 选择器数组
//
// fds 数组的每个元素表⽰对应字形的 FD 索引。当 FD 索引顺序较为随机时应使⽤此格式。字形数
// （nGlyphs）即 CharStrings INDEX 中 count 字段的值。此格式与字符集格式 0 相同，只是此时
// 包含 .notdef 字形。
//
//      格式 3
//      类型   名称             描述
//      Card8  format           =3
//      Card16 nRanges          区间数
//      struct Range3[nRanges]  Range3 数组（⻅表 29）
//      Card16 sentinel         哨兵 GID（⻅下⽂）
//
//      Range3 格式
//      类型   名称     描述
//      Card16 first    区间中的⾸个字形索引
//      Card8  fd       区间内所有字形的 FD 索引
//
// 每个 Range3 描述⼀组具有相同 FD 索引的连续 GID。每个区间包含从 ‘first’ GID 到下⼀区间
// 元素的 ‘first’ GID（不含）之间的 GID。因此 Range3 数组的元素按 ‘first’ GID 递增排序。
// 第⼀个区间的 ‘first’ GID 必须 为 0。最后⼀个区间元素之后跟随⼀个哨兵 GID，⽤于界定数组
// 中的最后⼀个区间。哨兵 GID 等于字体中的字形数，即⽐字体中最后⼀个 GID ⼤ 1。此格式特别
// 适合排序良好的 FD 索引（通常情形）。

// 字形组织（Glyph Organization）
//
// 字体中的字形构成⼀个字符集（charset），并通过编码（encoding）访问。编码是与字体中部分或
// 全部字形关联的代码数组，字符集是字体中所有字形的“名称”数组。在 CFF 中，这些名称实际是
// SID 或 CID，且必须唯⼀。
//
// 为理解 CFF 中字符集、编码和字形的关系，可将它们视为 3 个同步索引的“平⾏”数组。这样，对于
// 给定字形索引（GID）处的字形，可分别⽤ GID 索引字符集数组和编码数组来命名和编码。按定义，
// 第⼀个字形（GID 0）是 “.notdef”，且必须存在于所有字体中。由于总是如此，因此⽆需为 GID 0
// 表⽰其编码（不编码）或名称（.notdef）。利⽤这⼀优化，编码和字符集数组总是从 GID 1 开始。
//
// 编码（Encoding）- 使用字形索引 GID 访问该数组得到字形对应的字符编码，CIDFont 不指定编码
//
// 编码数据通过 Top DICT 中 Encoding 操作符的偏移量操作数定位。除不指定编码的 CIDFont 外，
// 每个字体只能指定⼀个 Encoding 操作符。字形的编码由 1 字节代码指定，取值范围 0–255。每种
// 编码由⼀个格式类型标识字节后跟格式特定数据描述。⽬前定义了两种格式，如下：
//
//      格式 0，code 数组的每个元素表⽰对应字形的编码。当代码顺序较为随机时应使⽤此格式。
//      类型  名称          描述
//      Card8 format        =0
//      Card8 nCodes        已编码字形数
//      Card8 code[nCodes]  代码数组
//
//      格式 1
//      类型   名称             描述
//      Card8  format           =1
//      Card8  nRanges          代码区间数
//      struct Range1[nRanges]  Range1 数组（⻅下表）
//
//      Range1 格式（编码）
//      类型  名称  描述
//      Card8 first 区间中的⾸个代码
//      Card8 nLeft 区间中剩余的代码数（不含⾸个）
//
// 每个 Range1 描述⼀组连续代码。例如，代码 51 52 53 54 55 可⽤ Range1：51 4 表⽰；⼀个完全
// 有序的 256 代码编码可⽤ Range1：0 255 描述。此格式特别适合排序良好的编码。
//
// 少数字体具有多重编码的字形，上述任何格式都不直接⽀持。这种情况通过将格式字节的最⾼位置 1
// 来表⽰，并（⽆论格式类型如何）按下表补充编码数据。
//
//      补充编码数据
//      类型   名称                 描述
//      Card8  nSups                补充映射数
//      struct Supplement[nSups]    补充编码数组（⻅下表）
//
//      Supplement 格式
//      类型  名称  描述
//      Card8 code  编码
//      SID   glyph 名称
//
// 每个 Supplement 描述⼀个代码到字形的映射，为已在主编码表中出现过的字形提供另⼀个编码。
// 先按编码排序字形、再对未编码字形按 SID 排序（记住 .notdef 必须在最前），通常可得到⾮
// 常⼩的字体编码。进⼀步的优化基于以下观察：许多字体采⽤两种常⽤编码之⼀。此时 Top DICT
// 中 Encoding 操作符的操作数指定下表所定义的预定义编码 id，⽽⾮偏移量。
//
//      编码 ID
//      Id      名称
//      0       Standard Encoding
//      1       Expert Encoding
//
// 若字体使⽤ Standard Encoding，由于 Encoding 操作符的默认值为 0，可将其从 Top DICT 中
// 省略。预定义编码的详情⻅附录 B。字体⽆需包含预定义编码所指定的全部字形即可使⽤该编码。
// 唯⼀的要求是：字体中每个字形的编码与预定义编码中的编码完全相同（包括未编码字形）。
//
// 两个或多个字体可通过将各⾃ Encoding 操作符的偏移量操作数设为相同值来共享同⼀编码。按
// 定义，未被⾃定义或预定义编码指定的字形为未编码字形。注：预定义编码可应⽤于各种字体⽽
// ⽆论其字符集如何，⽽⾃定义编码只能应⽤于具有特定字符集的字体。因此，预定义编码以代码
// 到 SID 的映射指定，⾃定义编码以代码到 GID 的映射指定。
//
// 字符集（Charsets）- 使用字形索引 GID 访问该数组得到字形对应的字符名称
//
// 字符集数据通过 Top DICT 中 charset 操作符的偏移量操作数定位。每种字符集由⼀个格式类型
// 标识字节后跟格式特定数据描述。⽬前定义了三种格式，如下：
//
//      格式  0
//      类型  名称              描述
//      Card8 format            =0
//      SID   glyph[nGlyphs–1]  字形名称数组
//
// glyph 数组的每个元素表⽰对应字形的名称。当 SID 顺序较为随机时应使⽤此格式。字形数（nGlyphs）
// 即 CharStrings INDEX 中 count 字段的值。字形名称数组⽐ nGlyphs 少⼀个元素，因为省略了
// .notdef 字形名。
//
//      格式 1
//      类型   名称             描述
//      Card8  format           =1
//      struct Range1[<可变>]   Range1 数组（⻅下表）
//
//      Range1 格式（字符集）
//      类型  名称  描述
//      SID   first 区间中的⾸个字形
//      Card8 nLeft 区间中剩余的字形数（不含⾸个）
//
// 每个 Range1 描述⼀组连续 SID。字体中并不显式指定区间数；使⽤该数据的软件只需依次处理
// 区间，直到覆盖字体中所有字形。此格式特别适合排序良好的字符集。
//
//      格式 2
//      类型   名称             描述
//      Card8  format           =2
//      struct Range2[<可变>]   Range2 数组（⻅下表）
//
//      Range2 格式
//      类型  名称      描述
//      SID   first     区间中的⾸个字形
//      Card16 nLeft    区间中剩余的字形数（不含⾸个）
//
// 格式 2 与格式 1 的区别仅在于每个区间中 nLeft 字段的⼤⼩。此格式最适合具有⼤型且排序
// 良好字符集的字体，例如亚洲 CIDFont。
//
// 精⼼安排 SID 的分配顺序通常可得到⾮常⼩的字体字符集。进⼀步的优化基于以下观察：许多
// 字体采⽤ 3 种常⽤字符集之⼀。此时 Top DICT 中 charset 操作符的操作数指定下表所⽰的预
// 定义字符集 id，⽽⾮偏移量。
//
//      字符集 ID
//      Id  名称
//      0   ISOAdobe
//      1   Expert
//      2   ExpertSubset
//
// 若字体采⽤ ISOAdobe 字符集，由于 charset 操作符的默认值为 0，可将其从 Top DICT 中省
// 略。预定义字符集的详情⻅附录 C。若字体的前 nGlyphs 个字形与某预定义字符集完全匹配，
// 则可以使⽤该预定义字符集。CID 字体不得使⽤预定义字符集。两个或多个字体可通过将各⾃
// charset 操作符的偏移量操作数设为相同值来共享同⼀字符集。

prh_r16 prh_font_cff1_charset_glyph(prh_font_cff1_table *p, prh_r32 glyph) {
    prh_byte *charset = p->table_data + p->charset_offset;
    prh_byte format = charset[0];
    if (glyph == 0) return 0;
    switch (format) {
    case 0: return prh_bp_2b_to_host(charset + 1 + 2 * (glyph - 1));
    case 1: case 2: {
        prh_r32 start_glyph = 1;
        prh_r32 end_glyph;
        for (prh_r32 i = 0; ; i += 1) {
            prh_byte *ptr_sid = charset + 1 + (2 + format) * i;
            end_glyph = start_glyph + ((format == 1) ? *(ptr_sid + 2) : prh_bp_2b_to_host(ptr_sid + 2));
            if (glyph >= start_glyph && glyph <= end_glyph) {
                return prh_bp_2b_to_host(ptr_sid) + (glyph - start_glyph);
            }
            start_glyph = end_glyph + 1;
        }
    } break;
    default:
        prh_abort_error(format);
        break;
    }
    return 0;
}

void prh_print_cff1_charset(prh_font_cff1_table *p) {
    prh_byte *charset = p->table_data + p->charset_offset;
    prh_byte format = charset[0];
    prh_r32 i = 0, print_count = 8;
    prh_r32 total_glyphs = p->charstrings_glyphs;
    prh_r32 last_glyph = total_glyphs - 1;
    prh_print(
        "----------------------------\n"
        "cff1 charset data offset %d\n"
        "cff1 charset data format %d\n"
        "cff1 charset data glyphs %d\n",
        (prh_reg)p->charset_offset,
        (prh_reg)format,
        (prh_reg)(total_glyphs - 1));
    switch (format) {
    case 0: {
        prh_r16 *ptr_sid = (prh_r16 *)(charset + 1);
        prh_print("cff1 charset data length %d\n", (prh_reg)(1 + 2 * (total_glyphs - 1)));
        if (print_count > total_glyphs - 1) print_count = total_glyphs - 1;
        for (; i < print_count; i += 1) {
            prh_print(
                "cff1 charset glyph %04d SID %03d\n",
                (prh_reg)(i + 1),
                (prh_reg)prh_bp_2b_to_host((prh_byte *)(ptr_sid + i)));
        }
    } break;
    case 1: case 2: {
        prh_r32 start_glyph = 1;
        prh_r32 end_glyph = 0;
        prh_r32 ranges = 0;
        while (last_glyph > end_glyph) {
            prh_byte *ptr_sid = charset + 1 + (2 + format) * ranges++;
            end_glyph = start_glyph + ((format == 1) ? *(ptr_sid + 2) : prh_bp_2b_to_host(ptr_sid + 2));
            start_glyph = end_glyph + 1;
        }
        prh_print(
            "cff1 charset data ranges %d\n"
            "cff1 charset data length %d\n",
            (prh_reg)ranges,
            (prh_reg)(1 + ranges * (2 + format)));
        prh_r32 turns = 4, stride;
        if (ranges / print_count < turns) turns = ranges / print_count;
        stride = ranges / turns;
        start_glyph = 1;
        for (prh_r32 turn_i = 0; turn_i < turns; turn_i += 1) {
            for (i = 0; i < stride; i += 1, start_glyph = end_glyph + 1) {
                prh_byte *ptr_sid = charset + 1 + (2 + format) * (turn_i * stride + i);
                end_glyph = start_glyph + ((format == 1) ? *(ptr_sid + 2) : prh_bp_2b_to_host(ptr_sid + 2));
                if (i < print_count) {
                    prh_print(
                        "cff1 charset range %04d / %d glyph %04d %04d SID %03d %03d (%d)\n",
                        (prh_reg)(turn_i * stride + i + 1),
                        (prh_reg)ranges,
                        (prh_reg)start_glyph,
                        (prh_reg)end_glyph,
                        (prh_reg)prh_bp_2b_to_host(ptr_sid),
                        (prh_reg)prh_bp_2b_to_host(ptr_sid) + (end_glyph - start_glyph),
                        (prh_reg)(end_glyph + 1 - start_glyph));
                }
            }
        }
    } break;
    default:
        prh_abort_error(format);
        break;
    }
}

// CharStrings INDEX
// https://adobe-type-tools.github.io/font-tech-notes/
//
// 其中包含字体中所有字形的 charstring，存储在⼀个 INDEX 结构中。该 INDEX 中的 charstring
// 对象按 GID 访问。第⼀个 charstring（GID 0）必须是 .notdef 字形。字体中可⽤的字形数
// 可由 INDEX 的 count 字段确定。
//
// charstring 数据的格式及其解释⽅法由 Top DICT 中的 CharstringType 操作符指定。CharstringType
// 操作符的默认值为 2，表⽰与 CFF 配套设计的 Type 2 charstring 格式。Type 1 charstring
// 记录于 Addison-Wesley 出版的 《 Adobe Type 1 Font Format 》。 Type 2 charstring ⻅ Adobe
// 技 术 说 明 5177 《 Type 2 Charstring Format》。此⽅法也可⽀持其他 charstring 类型。
//
// Type 2 格式为轮廓字体程序中的字形过程（glyph procedure）提供了⼀种紧凑编码⽅法。Type 2
// charstring 必须在 CFF（紧凑字体格式）或 OpenType 字体⽂件中使⽤，才能构成完整的字体程序。
// 本⽂档仅描述 Type 2 charstring 的编码⽅式，不试图解释各种⽅案选择的理由。Type 2 charstring
// 基于 Type 1 字体概念，本⽂档假定读者熟悉 Type 1 字体格式规范。更多信息请参⻅《Adobe Type
// 1 Font Format》1.1 版（Addison Wesley，1991）。此外，还假定读者熟悉 CFF 格式，请参⻅
// Adobe 技术说明 5176《紧凑字体格式规范》。
//
// 与 Type 1 格式相⽐，Type 2 编码体积更⼩，并有机会获得更好的渲染质量和性能。Type 2
// charstring 操作符（除⼀个例外）是 Type 1 操作符的超集。借助适当的转换程序，合法的
// Type 1 字体程序（即与 Adobe Type Manager 软件兼容的程序）可以转换为 Type 2 字体程序，
// Type 2 程序也可以转换回 Type 1 字体程序，且不损失信息或渲染质量。
//
// Type 2 Charstring
//
// 以下各节描述编码 Type 2 charstring 的⼀般概念。
//
// 提⽰（Hints）。Type 2 charstring 格式⽀持六个提⽰操作符：hstem、vstem、hstemhm、vstemhm、
// hintmask 和 cntrmask。提⽰信息必须在 charstring 的开头声明（⻅第 3.1 节），使⽤ hstem、
// hstemhm、vstem 和 vstemhm 操作符，每个操作符都可以携带多组 stem（词⼲）提⽰参数。
//
// Type 2 提⽰操作符帮助光栅化器识别和控制字形内的词⼲和字怀（counter）区域。⼀个词⼲通常
// 由两个位置（边缘）及相应的宽度组成。边缘词⼲提⽰（edge stem hint）⽤于控制只有单个边缘
// 的字符特征（⻅第 4.3 节）。
//
// Type 2 格式包含边缘提⽰，它等价于 Type 1 中幽灵提⽰（ghost hint）的概念，⻅《Adobe Type
// 1 Font Format》第 57 ⻚幽灵提⽰⼀节。它们⽤于定位⼀个边缘，⽽不是具有两个边缘的词⼲。词
// ⼲宽度值 –20 保留给顶部或右侧边缘，–21 保留给底部或左侧边缘。使⽤其他负宽度值的提⽰，其
// ⾏为未定义。
//
// hintmask 操作符的功能与《Adobe Type 1 Font Format》第 69 ⻚ 8.1 节 “在字符内更改提⽰”
// 所述相同。它提供了⼀种激活或停⽤词⼲提⽰的⼿段，使得同⼀时刻只有⼀组互不重叠的提⽰处于
// 激活状态。
//
// hintmask 操作符后跟⼀个或多个数据字节，指定后续路径构造中哪些词⼲提⽰处于激活状态。数据
// 字节的数量必须恰好是表⽰原始词⼲列表（即由 hstem、vstem、hstemhm 或 vstemhm 命令指定的
// 词⼲）中词⼲数量所需的字节数，数据字节中每⼀位对应原始词⼲列表中的⼀个词⼲。位值为 1 表
// ⽰该词⼲激活，位值为 0 表⽰该词⼲停⽤。
//
// cntrmask（countermask，字怀掩码）提⽰使字符中任意但不重叠的字怀空间集合受到控制，其⽅式
// 类似于词⼲提⽰命令对词⼲宽度的控制，更多信息⻅技术说明 5015《Type 1 Font Format Supplement》。
//
// cntrmask 操作符后跟⼀个或多个数据字节，指定字怀空间两侧词⼲提⽰的索引编号。数据字节的数
// 量必须恰好是表⽰原始词⼲列表（即由 hstem、vstem、hstemhm 或 vstemhm 命令指定的词⼲）中词
// ⼲数量所需的字节数——数据字节中每⼀位对应原始词⼲列表中的⼀个词⼲。
//
// 对于图 1 所⽰的例⼦，该字形的词⼲列表为：
//      H1 H2 H3 H4 H5 H6 H7 H8 V1 V2 V3 V4 V5
//
// 并使⽤以下 cntrmask 命令控制这些词⼲之间的字怀空间：
//      cntrmask 0xB5 0xE8（H1 H3 H4 H6 H8 V1 V2 V3 V5）
//      cntrmask 0x4A 0x00（H2 H5 H7）
//
// 数据字节中被置位的位表⽰相应的词⼲提⽰界定了所需的字怀集合。第⼀条命令中指定的提⽰优先级
// ⾼于第⼆条命令中的提⽰。注意，V4 词⼲并未界定合适的字怀空间，因此本例中未引⽤它。
//
// 请注意，提⽰终究只是提⽰，即建议。它们是提供给智能光栅化器的附加指导信息。
//
// 如果字体的 LanguageGroup 不等于 1（LanguageGroup 值为 1 表⽰复杂的亚洲语⾔字形），那么在
// 满⾜ Type 1 规范所规定的相关条件的前提下，可以使⽤带三个词⼲的 cntrmask 操作符来代替 Type
// 1 格式中的 hstem3 和 vstem3 提⽰。
//
// 有关字怀控制（Counter Control）提⽰的更多信息，请参⻅ Adobe 技术说明 5015《Type 1 Font
// Format Supplement》。
//
// Flex 机制（The Flex Machanism）。提供 flex（伸缩）机制是为了改善浅曲线的渲染：在⼩字号下
// 将浅曲线表⽰为线段，⽽不是字符形状上的⼩凸起或凹陷。它本质上是⼀种路径构造机制：其参数描
// 述两条曲线的构造，另有⼀个附加参数作为提⽰，指⽰在较⼩字号和分辨率下何时应把曲线渲染为直
// 线。
//
// Type 2 的 flex 机制是通⽤的：对可⽤ flex 操作符表达的曲线类型或⽅向没有限制。flex 操作符
// ⽤于⼀般情形；特殊情形可以使⽤ flex1、hflex 或 hflex1 操作符以获得更⾼效的编码。图 2 展⽰
// 了 flex 机制⽤于⽔平曲线的例⼦，图 3 展⽰了⾮标准⻆度下 flex 曲线的例⼦。
//
// flex 操作符可⽤于任何⽅向、任何深度的弯曲字符特征，只要满⾜以下要求：
//  1.  该字符特征必须能够恰好⽤两条曲线表⽰，由两个 rrcurveto 操作符绘制。
//  2.  两条曲线必须在⼀个称为连接点（joining point）的公共点相接。
//  3.  组合曲线的⻓度必须⼤于其深度。
//
// ⼦例程（Subroutines）。Type 2 字体程序可以使⽤⼦例程（subroutine），通过合并描述字体中
// 各字符公共元素的程序语句来降低存储需求。
//
// ⼦例程可以是局部的或全局的。局部⼦例程仅可从当前字体的 charstring 程序中访问。全局⼦例
// 程则在⼀个 FontSet 内的多个字体之间共享，更多信息⻅ Adobe 技术说明 5176《CFF 字体格式规
// 范》。
//
// ⼦例程可以包含 charstring ⽚段，编码⽅式与 Type 2 charstring 相同。调⽤时使⽤ callsubr
// （局部⼦例程）或 callgsubr（全局⼦例程）操作符，参数为指向局部或全局 Subrs 数组的加偏置
// 索引。注：与 Type 1 格式的偏置不同，Type 2 中的偏置不是可选的，⽽是固定的，取决于⼦例程
// 的数量。
//
// charstring ⼦例程可以调⽤其他⼦例程，嵌套深度以实现限制为准（⻅附录 B）。charstring ⼦例
// 程必须以 endchar 或 return 操作符结束。如果⼦例程以 endchar 操作符结束，则⽆需 return。
//
// Charstring 编码（Charstring Encoding）
//
// Type 2 charstring 程序是⼀串⽆符号 8 位字节序列，⽤于编码数值和操作符。字节值指定⼀个操作
// 符、⼀个数值，或需要以特定⽅式解释的后续字节。字节被解码为数值和操作符。该格式⽐ Type 1 更
// 节省的原因之⼀是：Type 2 charstring 解释器需要对参数栈上的参数个数进⾏计数，因此可以检测到
// 属于单个操作符的多组参数。栈深度实现限制⻅附录 B。
//
// 从 charstring 解码出的数值被压⼊ Type 2 参数栈。操作符按顺序从参数栈获取其参数，通常所有参
// 数都取⾃栈底（第⼀个参数在最底部）；但有些操作符（尤其是⼦例程操作符）通常从栈顶取参数。如
// 果操作符返回结果，结果也被压⼊ Type 2 参数栈（最后⼀个结果在最顶部）。
//
// 在以下讨论中，除⾮特别说明，所有数值常量均为⼗进制数。
//
// Type 2 Charstring 的组织。Type 2 charstring 程序的序列和形式可表⽰为：
//      w? {hs* vs* cm* hm* mt subpath}? {mt subpath}* endchar
// 其中：
//      w = 宽度（width）
//      hs = hstem 或 hstemhm 命令
//      vs = vstem 或 vstemhm 命令
//      cm = cntrmask 操作符
//      hm = hintmask 操作符
//      mt = moveto（即任意⼀种 moveto）操作符
//      subpath = 指⼀个⼦路径（⼀条完整闭合轮廓）的构造，其中可在适当位置包含 hintmask 操作符
//
// 以下符号表⽰特定⽤法：
//      * 允许出现零次或多次
//      ? 允许出现零次或⼀次
//      + 允许出现⼀次或多次
//      {} 表⽰分组
//
// ⽤⽂字表述，charstring 中操作符序列的约束如下。Type 2 charstring 必须按以下特定顺序
// 组织操作符（或操作符类别）：
//  1.  宽度：如果 charstring 的宽度与 defaultWidthX 不同，⻅技术说明 5176《紧凑字体格
//      式规范》，则必须将其指定为 charstring 中的第⼀个数值，并按与 nominalWidthX 的差
//      值编码。
//  2.  提⽰：下列各提⽰操作符各出现零次或多次，且严格按以下顺序：hstem、hstemhm、vstem、
//      vstemhm、cntrmask、hintmask。每⼀项都是可选的，且每⼀项都可以⽤该操作符的⼀次或
//      多次出现来表⽰。如果 charstring 没有词⼲提⽰，则不得出现 cntrmask 和/或 hintmask
//      操作符。
//  3.  路径构造：不含提⽰的 charstring 的第⼀条路径必须以某种 moveto 操作符开始，以便正
//      确检测前⾯的宽度。使⽤零个或多个路径构造操作符绘制字符的路径；第⼆条及后续所有⼦
//      路径也必须以某种 moveto 操作符开始。hintmask 操作符可按需使⽤。
//  4.  endchar：字符必须以 endchar 操作符结束。
//
// 注：charstring 可以在任意完整标记（操作符或数值）之间的位置按需包含 subr 和 gsubr 调
// ⽤。这意味着 subr（gsubr）调⽤不得出现在多字节命令（例如 hintmask）的字节之间。
//
// Charstring 数值编码（Number Encoding）。值在 32 到 254（含）之间的 charstring 字节表⽰
// ⼀个整数。这些值分三个范围解码（另⻅表 1）：
//  1.  charstring 字节的值 v 在 32 到 246（含）之间时，指定整数 v − 139。因此，–107 到
//      107（含）的整数值可以⽤单字节编码。
//  2.  charstring 字节的值 v 在 247 到 250（含）之间时，表⽰⼀个涉及下⼀字节 w 的整数，
//      按公式：(v − 247) * 256 + w + 108。因此，108 到 1131（含）的整数值可以⽤ 2 字节
//      编码。
//  3.  charstring 字节的值 v 在 251 到 254（含）之间时，表⽰⼀个涉及下⼀字节 w 的整数，
//      按公式：− [(v − 251) * 256] − w − 108。因此，–1131 到 –108（含）的整数值可以⽤
//      2 字节编码。
//
// 如果 charstring 字节的值为 255，则随后四个字节表⽰⼀个⼆进制补码有符号数。这四个字节
// 中，第⼀个含最⾼位，第⼆个含次⾼位，第四个含最低位。该数被解释为 Fixed 类型，即带 16
// 位⼩数部分的有符号数。
//
// 注：Type 2 对五字节编码数值（⾸字节值为 255）的解释与 Type 1 格式中的解释不同。
//
// 除 32 到 255 的范围外，还可以⽤操作符 (28) 后跟两个字节来指定 ShortInt 值，这两个字节
// 表⽰ –32768 到 +32767 之间的数。最⾼有效字节紧随 (28)。这使得字体中偶尔出现的⼤数能有
// 更紧凑的表⽰；但也许更重要的是，它使得⽤作 callsubr 和 callgsubr 参数的数值能有更紧凑
// 的编码。
//
//      Type 2 Charstring 编码值
//      Charstring 字节值       解释                            可表⽰的数值范围                    所需字节数
//      0 – 11                  操作符                          操作符 0 到 11                      1
//      12                      转义：下⼀字节解释为附加操作符  操作符代码增加 0 到 255 的范围      2
//      13 – 18                 操作符                          操作符 13 到 18                     1
//      19, 20                  操作符（hintmask 和 cntrmask）  操作符 19、20                       2 或更多
//      21 – 27                 操作符                          操作符 21 到 27                     1
//      28                      随后 2 字节解释为 16 位⼆进制补码数         –32768 到 +32767        3
//      29 – 31                 操作符                          操作符 29 到 31                     1
//      32 – 246                结果 = v–139                    –107 到 +107                        1
//      247 – 250               与下⼀字节 w，结果 = (v–247)*256+w+108      +108 到 +1131           2
//      251 – 254               与下⼀字节 w，结果 = –[(v–251)*256]–w–108   –108 到 –1131           2
//      255                     随后 4 字节解释为 32 位⼆进制补码数     带16位⼩数的16位有符号整数  5
//
// Charstring 操作符编码（Operator Encoding）。charstring 操作符以 1 或 2 字节编码。单字节
// 操作符编码在⼀个字节中，其值在 0 到 31（含）之间，但不包括 12 和 28。并⾮所有可能的操作
// 符编码值都有定义（操作符编码值列表⻅附录 A）。未定义操作符的⾏为不作规定。
//
// 如果操作符字节的值为 12，则下⼀字节中的值指定⼀个操作符。这种转义机制允许编码许多额外的
// 操作符。

prh_i32 prh_font_cff1_charstring_integer(const prh_byte *b, prh_r32 *value_bytes) {
    if (b[0] == 28) { *value_bytes = 3; return ((prh_i32)b[1] << 8) | b[2]; } // -32768 ~ + 32767
    if (b[0] >= 32 && b[0] <= 246) { *value_bytes = 1; return (prh_i32)b[0] - 139; } // -107 ~ +107
    if (b[0] >= 247 && b[0] <= 250) { *value_bytes = 2; return ((prh_i32)b[0] - 247) * 256 + (prh_i32)b[1] + 108; } // +108 ~ +1131
    if (b[0] >= 251 && b[0] <= 254) { *value_bytes = 2; return -((prh_i32)b[0] - 251) * 256 - (prh_i32)b[1] - 108; } // -1131 ~ -108
    if (b[0] == 255) { *value_bytes = 5; return ((prh_i32)b[1] << 24) | ((prh_i32)b[2] << 16) | ((prh_i32)b[3] << 8) | b[4]; } // –(2^31) ~ +(2^31–1)
    *value_bytes = 0; return 0;
}

prh_r32 prh_font_cff1_charstring_value_bytes(const prh_byte *b) {
    if (b[0] <= 31) { if (b[0] == 12) return 2; if (b[0] == 28) return 3; return 1; }
    if (b[0] >= 32 && b[0] <= 246) return 1;
    if (b[0] >= 247 && b[0] <= 250) return 2;
    if (b[0] >= 251 && b[0] <= 254) return 2;
    return 5;
}

#define PRH_CFF_CHARSTRING_OP_HSTEM         0x01 // 1
#define PRH_CFF_CHARSTRING_OP_VSTEM         0x03 // 3
#define PRH_CFF_CHARSTRING_OP_VMOVETO       0x04 // 4
#define PRH_CFF_CHARSTRING_OP_RLINETO       0x05 // 5
#define PRH_CFF_CHARSTRING_OP_HLINETO       0x06 // 6
#define PRH_CFF_CHARSTRING_OP_VLINETO       0x07 // 7
#define PRH_CFF_CHARSTRING_OP_RRCURVETO     0x08 // 8
#define PRH_CFF_CHARSTRING_OP_CLOSEPATH     0x09 // 9 - type 1 operator
#define PRH_CFF_CHARSTRING_OP_CALLSUBR      0x0A // 10
#define PRH_CFF_CHARSTRING_OP_RETURN        0x0B // 11
#define PRH_CFF_CHARSTRING_OP_ESCAPE        0x0C // 12
#define PRH_CFF_CHARSTRING_OP_HSBW          0x0D // 13 - type 1 operator
#define PRH_CFF_CHARSTRING_OP_ENDCHAR       0x0E // 14
#define PRH_CFF_CHARSTRING_OP_HSTEMHM       0x12 // 18
#define PRH_CFF_CHARSTRING_OP_HINTMASK      0x13 // 19
#define PRH_CFF_CHARSTRING_OP_CNTRMASK      0x14 // 20
#define PRH_CFF_CHARSTRING_OP_RMOVETO       0x15 // 21
#define PRH_CFF_CHARSTRING_OP_HMOVETO       0x16 // 22
#define PRH_CFF_CHARSTRING_OP_VSTEMHM       0x17 // 23
#define PRH_CFF_CHARSTRING_OP_RCURVELINE    0x18 // 24
#define PRH_CFF_CHARSTRING_OP_RLINECURVE    0x19 // 25
#define PRH_CFF_CHARSTRING_OP_VVCURVETO     0x1A // 26
#define PRH_CFF_CHARSTRING_OP_HHCURVETO     0x1B // 27
#define PRH_CFF_CHARSTRING_OP_CALLGSUBR     0x1D // 29
#define PRH_CFF_CHARSTRING_OP_VHCURVETO     0x1E // 30
#define PRH_CFF_CHARSTRING_OP_HVCURVETO     0x1F // 31

#define PRH_CFF_CHARSTRING_EOP_DOTSECTION   0x00 // 0 - type 1 operator
#define PRH_CFF_CHARSTRING_EOP_VSTEM3       0x01 // 1 - type 1 operator
#define PRH_CFF_CHARSTRING_EOP_HSTEM3       0x02 // 2 - type 1 operator
#define PRH_CFF_CHARSTRING_EOP_AND          0x03 // 3
#define PRH_CFF_CHARSTRING_EOP_OR           0x04 // 4
#define PRH_CFF_CHARSTRING_EOP_NOT          0x05 // 5
#define PRH_CFF_CHARSTRING_EOP_SEAC         0x06 // 6 - type 1 operator
#define PRH_CFF_CHARSTRING_EOP_SBW          0x07 // 7 - type 1 operator
#define PRH_CFF_CHARSTRING_EOP_ABS          0x09 // 9
#define PRH_CFF_CHARSTRING_EOP_ADD          0x0A // 10
#define PRH_CFF_CHARSTRING_EOP_SUB          0x0B // 11
#define PRH_CFF_CHARSTRING_EOP_DIV          0x0C // 12
#define PRH_CFF_CHARSTRING_EOP_NEG          0x0E // 14
#define PRH_CFF_CHARSTRING_EOP_EQ           0x0F // 15
#define PRH_CFF_CHARSTRING_EOP_CALLOTHERSUBR 0x10 // 16 - type 1 operator
#define PRH_CFF_CHARSTRING_EOP_POP          0x11 // 17 - type 1 operator
#define PRH_CFF_CHARSTRING_EOP_DROP         0x12 // 18
#define PRH_CFF_CHARSTRING_EOP_PUT          0x14 // 20
#define PRH_CFF_CHARSTRING_EOP_GET          0x15 // 21
#define PRH_CFF_CHARSTRING_EOP_IFELSE       0x16 // 22
#define PRH_CFF_CHARSTRING_EOP_RANDOM       0x17 // 23
#define PRH_CFF_CHARSTRING_EOP_MUL          0x18 // 24
#define PRH_CFF_CHARSTRING_EOP_SQRT         0x1A // 26
#define PRH_CFF_CHARSTRING_EOP_DUP          0x1B // 27
#define PRH_CFF_CHARSTRING_EOP_EXCH         0x1C // 28
#define PRH_CFF_CHARSTRING_EOP_INDEX        0x1D // 29
#define PRH_CFF_CHARSTRING_EOP_ROLL         0x1E // 30
#define PRH_CFF_CHARSTRING_EOP_SETCURRENTPOINT 0x21 // 33 - type 1 operator
#define PRH_CFF_CHARSTRING_EOP_HFLEX        0x22 // 34
#define PRH_CFF_CHARSTRING_EOP_FLEX         0x23 // 35
#define PRH_CFF_CHARSTRING_EOP_HFLEX1       0x24 // 36
#define PRH_CFF_CHARSTRING_EOP_FLEX1        0x25 // 37

const char *prh_impl_print_cff1_charstring_operator(prh_byte *p) {
    const char *oper[] = {
        "Reserved", "hstem", "Reserved", "vstem", "vmoveto", "rlineto", "hlineto", "vlineto",
        "rrcurveto", "closepath (typ1)", "callsubr", "return", "escape", "hsbw (typ1)", "endchar", "Reserved",
        "Reserved", "Reserved", "hstemhm", "hintmask", "cntrmask", "rmoveto", "hmoveto", "vstemhm",
        "rcurveline", "rlinecurve", "vvcurveto", "hhcurveto", "Unknown", "callgsubr", "vhcurveto", "hvcurveto"};
    const char *ext_oper[] = {
        "dotsection (typ1)", "vstem3 (typ1)", "hstem3 (typ1)", "and", "or", "not", "seac (typ1)", "sbw (typ1)",
        "Reserved", "abs", "add", "sub", "div", "Reserved", "neg", "eq"
        "callothersubr (typ1)", "pop (typ1)", "drop", "Reserved", "put", "get", "ifelse", "random",
        "mul", "Reserved", "sqrt", "dup", "exch", "index", "roll", "Reserved",
        "Reserved", "setcurrentpoint (typ1)", "hflex", "flex", "hflex1", "flex1"};
    if (p[0] == 12) {
        if (p[1] <= 37) return ext_oper[p[1]];
        return "Reserved";
    } else {
        return oper[p[0]];
    }
}

void prh_impl_print_cff1_charstring_glyph(prh_byte *glyph, prh_r32 length) {
    prh_r32 bytes, count = 0;
    prh_r32 operand_count = 0;
    prh_r32 value_count = 0;
    while (count < length) {
        prh_byte *b = glyph + count;
        bytes = prh_font_cff1_charstring_value_bytes(b);
        value_count += 1;
        prh_print("GLYPH DATA %04d (%03d) %04d len %02d: ",
            (prh_reg)length,
            (prh_reg)value_count,
            (prh_reg)count,
            (prh_reg)bytes);
        count += bytes;
        for (prh_r32 i = 0; i < bytes; i += 1) prh_print("%02x ", (prh_reg)b[i]);
        if (bytes * 3 < 21) prh_print_byte(' ', 21 - bytes * 3);
        if (*b == 28 || *b > 31) { // 28 和 32–255 表⽰操作数（数值）
            prh_r32 value_bytes;
            prh_i32 integer = prh_font_cff1_charstring_integer(b, &value_bytes);
            prh_print("INTEGER %d\n", (prh_reg)integer);
            prh_real_assert(bytes == value_bytes);
            operand_count += 1;
        } else { // 0 ~ 31 除 28 外表⽰操作符
            prh_print("OPERATOR(%d) %s\n", (prh_reg)operand_count, prh_impl_print_cff1_charstring_operator(b));
            prh_real_assert(operand_count <= 48); // ⼀个操作符之前最多可以有 48 个操作数
            operand_count = 0;
        }
    }
    prh_real_assert(count == length);
}

void prh_print_cff1_charstrings_index(prh_font_cff1_table *p) {
    prh_r32 charstrings_glyphs = p->charstrings_glyphs;
    prh_r32 turns = 3, count = 3, stride;
    prh_byte *glyph_data;
    prh_r32 data_length;
    prh_r32 glyph_index;
    if (charstrings_glyphs / count < turns) turns = charstrings_glyphs / count;
    stride = charstrings_glyphs / turns;

    prh_print(
        "----------------------------\n"
        "cff1 CharStrings INDEX offset %d\n"
        "cff1 CharStrings INDEX length %d\n"
        "cff1 CharStrings INDEX object offsize %d\n"
        "cff1 CharStrings glyphs %d (GID 0 included)\n"
        "----------------------------",
        (prh_reg)p->charstrings_offset,
        (prh_reg)p->charstrings_index_bytes,
        (prh_reg)prh_impl_font_cff1_index_offsize(p->charstrings_index),
        (prh_reg)prh_impl_font_cff1_index_objects(p->charstrings_index));

    for (prh_r32 turn_i = 0; turn_i < turns; turn_i += 1) {
        for (prh_r32 i = 0; i < count; i += 1) {
            glyph_index = stride * turn_i + i;
            glyph_data = prh_font_cff1_object_data(p->charstrings_index, (prh_r16)glyph_index, &data_length);
            prh_prinf(
                "\ncff1 CharStrings #glyph %04d (%04d/%d) SID %03d\n",
                (prh_reg)glyph_index, (prh_reg)(glyph_index + 1),
                (prh_reg)charstrings_glyphs,
                (prh_reg)prh_font_cff1_charset_glyph(p, glyph_index));
            prh_impl_print_cff1_charstring_glyph(glyph_data, data_length);
        }
    }
}

// Charstring 操作符（Charstring Operators）
//
// Type 2 charstring 操作符按功能分为七组：
//      1）路径构造
//      2）结束路径
//      3）提⽰
//      4）算术
//      5）存储
//      6）条件
//      7）⼦例程
//
// 以下定义采⽤与《PostScript 语⾔参考⼿册》类似的格式。操作符名称后的括号内，是该操
// 作符在 charstring 字节中的操作符值，或者是表⽰双字节操作符的两个值（以 12 开头）。
//
// 许多操作符从 Type 2 参数栈的最底部取参数；此⾏为⽤出现在第⼀个参数左侧的栈底符号“|-”
// 表⽰。会清空参数栈的操作符，则在操作符定义的结果位置以栈底符号“|-”表⽰。
//
// 由于这种清栈⾏为，⼀般来说参数不会在 Type 2 参数栈上累积以供随后⼀系列操作符取⽤；通常
// 只能为下⼀个操作符提供参数。明显的例外出现在⼦例程调⽤以及算术和条件操作符中。所有栈操
// 作都必须遵守栈限制（⻅附录 B）。
//
// 路径构造操作符（Path Construction Operators）
//
// 在 Type 2 charstring 中，路径由⼀个或多个路径构造操作符依次应⽤⽽构成。当前点最初是字符
// 坐标系的 (0, 0) 点。本节列出的操作符会使当前点发⽣改变，或通过 moveto 操作，或通过在当
// 前点上追加⼀个或多个曲线或直线段。操作完成后，当前点更新为移动所到的位置，或各线段的最
// 后⼀个点。许多操作符可以接受多组参数，表⽰⼀系列路径构造操作。操作数量仅受栈⼤⼩限制（⻅
// 附录 B）。
//
// 所有⻉塞尔曲线路径段都使⽤六个参数绘制：dxa、dya、dxb、dyb、dxc、dyc；其中 dxa 和 dya
// 相对于当前点，所有后续参数均相对于前⼀个点。⼀些曲线操作符利⽤了某些切线点处于⽔平或垂
// 直⽅向（因⽽相应值为零）的情形，从⽽减少了所需参数的数量。
//
// flex 操作符被视为路径构造命令，因为它们指定了两条曲线的绘制。另有⼀个附加参数作为提⽰，
// 指⽰在⼩字号和低分辨率下何时把曲线渲染为直线。
//
// moveto 操作符有三种类型。对于 charstring 中的第⼀个 moveto 操作符，参数相对于字符坐标系
// 的 (0, 0) 点；后续 moveto 操作符的参数相对于当前点。
//
// 每个字符路径和⼦路径都必须以某种 moveto 操作符开始。如果遇到 moveto 操作符时当前路径未
// 闭合，则在执⾏ moveto 操作之前先闭合该路径。
//
// rmoveto |- dx1 dy1 rmoveto (21) |-
//
//      将当前点移动到相对坐标 (dx1, dy1) 处。注 4：第⼀个清栈操作符（必须是 hstem、hstemhm、
//      vstem、vstemhm、cntrmask、hintmask、hmoveto、vmoveto、rmoveto 或 endchar 之⼀）接
//      受⼀个附加参数，宽度（如前所述），可表⽰为零个或⼀个数值参数。
//
// hmoveto |- dx1 hmoveto (22) |-
//
//      将当前点沿⽔平⽅向移动 dx1 个单位。⻅注 4。
//
// vmoveto |- dy1 vmoveto (4) |-
//
//      将当前点沿垂直⽅向移动 dy1 个单位。⻅注 4。
//
// rlineto |- {dxa dya}+ rlineto (5) |-
//
//      从当前点向相对坐标 dxa、dya 处追加⼀条直线。对所有后续参数对执⾏额外的 rlineto 操作。
//      直线条数由栈上参数个数决定。
//
// hlineto |- dx1 {dya dxb}* hlineto (6) |-
//         |- {dxa dyb}+ hlineto (6) |-
//
//      向当前点追加⼀条⻓度为 dx1 的⽔平线。参数个数为奇数时，后续参数对被解释为 dy 和 dx
//      交替的值，相应的额外 lineto 操作绘制垂直与⽔平交替的直线。参数个数为偶数时，参数被
//      解释为⽔平与垂直交替的直线。直线条数由栈上参数个数决定。
//
// vlineto |- dy1 {dxa dyb}* vlineto (7) |-
//         |- {dya dxb}+ vlineto (7) |-
//
//      向当前点追加⼀条⻓度为 dy1 的垂直线。参数个数为奇数时，后续参数对被解释为 dx 和 dy
//      交替的值，相应的额外 lineto 操作绘制⽔平与垂直交替的直线。参数个数为偶数时，参数被
//      解释为垂直与⽔平交替的直线。直线条数由栈上参数个数决定。
//
// rrcurveto |- {dxa dya dxb dyb dxc dyc}+ rrcurveto (8) |-
//
//      向当前点追加⼀条由 dxa…dyc 定义的⻉塞尔曲线。对随后每组六个参数，再向当前点追加⼀
//      条曲线。曲线段数由数值栈上的参数个数决定，仅受数值栈⼤⼩限制。
//
// hhcurveto |- dy1? {dxa dxb dyb dxc}+ hhcurveto (27) |-
//
//      向当前点追加⼀条或多条由 dxa…dxc 参数组描述的⻉塞尔曲线。对每条曲线，若为 4 个参数，
//      则曲线⽔平开始且⽔平结束。第⼀条曲线不必⽔平开始（奇数参数情形）。注意奇数参数情形
//      的参数顺序。
//
// hvcurveto |- dx1 dx2 dy2 dy3 {dya dxb dyb dxc dxd dxe dye dyf}* dxf? hvcurveto (31) |-
//           |- {dxa dxb dyb dyc dyd dxe dye dxf}+ dyf? hvcurveto (31) |-
//
//      向当前点追加⼀条或多条⻉塞尔曲线。第⼀条⻉塞尔曲线的切线必须是⽔平的，第⼆条必须是
//      垂直的（下述情形除外）。如果参数个数是 4 的倍数，曲线⽔平开始、垂直结束。注意曲线
//      在“⽔平开始、垂直结束”与“垂直开始、⽔平结束”之间交替。最后⼀条曲线（奇数参数情形）
//      不必以⽔平/垂直结束。
//
// rcurveline |- {dxa dya dxb dyb dxc dyc}+ dxd dyd rcurveline (24) |-
//
//      等价于对每组六个参数 dxa…dyc 执⾏⼀次 rrcurveto，随后⽤参数 dxd、dyd 恰好执⾏⼀次
//      rlineto。曲线条数由参数栈上的计数决定。
//
// rlinecurve |- {dxa dya}+ dxb dyb dxc dyc dxd dyd rlinecurve (25) |-
//
//      等价于：除⼀次 rrcurveto 命令所需的六个参数 dxb…dyd 之外，对其余每对参数执⾏⼀次
//      rlineto。直线条数由参数栈上的项数决定。
//
// vhcurveto |- dy1 dx2 dy2 dx3 {dxa dxb dyb dyc dyd dxe dye dxf}* dyf? vhcurveto (30) |-
//           |- {dya dxb dyb dxc dxd dxe dye dyf}+ dxf? vhcurveto (30) |-
//
//      向当前点追加⼀条或多条⻉塞尔曲线，其中第⼀条切线是垂直的，第⼆条切线是⽔平的。
//      此命令是 hvcurveto 的互补命令；更多信息⻅ hvcurveto 的描述。
//
// vvcurveto |- dx1? {dya dxb dyb dyc}+ vvcurveto (26) |-
//
//      向当前点追加⼀条或多条曲线。若参数个数是 4 的倍数，曲线垂直开始且垂直结束。若参数
//      个数为奇数，第⼀条曲线不以垂直切线开始。
//
// flex |- dx1 dy1 dx2 dy2 dx3 dy3 dx4 dy4 dx5 dy5 dx6 dy6 fd flex (12 35) |-
//
//      使参数所描述的两条⻉塞尔曲线（如下图 2 所⽰）在 flex 深度⼩于 fd/100 个设备像素时
//      渲染为直线，在 flex 深度⼤于或等于 fd/100 个设备像素时渲染为曲线。
//
//      如图 2 所⽰，⽔平曲线的 flex 深度是从连接点到曲线起点与终点连线的距离。如果曲线并
//      ⾮严格的⽔平或垂直，则必须按下⽂ flex1 描述中所述的⽅法（如图 3 所⽰）判断曲线更偏
//      ⽔平还是更偏垂直。图 2 Flex 提⽰⽰例：
//
//      注 5：当曲线中的某些点与其他点具有相同的 x 或 y 坐标时，可以使⽤ flex 操作符的下列
//      形式之⼀，hflex、hflex1 或 flex1，来省略参数。
//
// hflex |- dx1 dx2 dy2 dx3 dx4 dx5 dx6 hflex (12 34) |-
//
//      使参数 dx1…dx6 所描述的两条曲线在 flex 深度⼩于 0.5（即 fd 为 50）个设备像素时渲染
//      为直线，在 flex 深度⼤于或等于 0.5 个设备像素时渲染为曲线。当以下条件全部成⽴时使⽤
//      hflex：
//      a) 起点和终点、第⼀个和最后⼀个控制点具有相同的 y 值。
//      b) 连接点及相邻控制点具有相同的 y 值。
//      c) flex 深度为 50。
//
// hflex1 |- dx1 dy1 dx2 dy2 dx3 dx4 dx5 dy5 dx6 hflex1 (12 36) |-
//
//      使参数所描述的两条曲线在 flex 深度⼩于 0.5 个设备像素时渲染为直线，在 flex 深度⼤
//      于或等于 0.5 个设备像素时渲染为曲线。当不满⾜ hflex 的条件但以下条件全部成⽴时使⽤
//      hflex1：
//      a) 起点和终点具有相同的 y 值。
//      b) 连接点及相邻控制点具有相同的 y 值。
//      c) flex 深度为 50。
//
// flex1 |- dx1 dy1 dx2 dy2 dx3 dy3 dx4 dy4 dx5 dy5 d6 flex1 (12 37) |-
//
//      使参数所描述的两条曲线在 flex 深度⼩于 0.5 个设备像素时渲染为直线，在 flex 深度⼤
//      于或等于 0.5 个设备像素时渲染为曲线。参数 d6 是 dx 还是 dy 值取决于曲线（⻅图 3）。
//      为确定正确的值，计算从起点 (x, y)（第⼀条曲线的第⼀个点）到最后⼀个 flex 控制点
//       (dx5, dy5) 的距离：将除 d6 之外的所有参数求和，记为 (dx, dy)。若 abs(dx) > abs(dy)，
//      则最后⼀个点的 x 值由 d6 给出，其 y 值等于 y；否则，最后⼀个点的 x 值等于 x，其 y
//      值由 d6 给出。当不满⾜ hflex 和 hflex1 的条件但以下条件全部成⽴时使⽤ flex1：
//      a) 起点和终点具有相同的 x 或 y 值。
//      b) flex 深度为 50。
//
//      图 3 Flex 深度计算（左：垂直 Flex，dy > dx；右：⽔平 Flex，dx > dy）
//
// 结束路径的操作符（Operator for Finishing a Path）
//
// endchar – endchar (14) |–
//
//      结束⼀个 charstring 轮廓定义，必须是字符轮廓中的最后⼀个操作符。注 6：charstring
//      本⾝可以以 call(g)subr 结尾；此时该⼦例程必须以 endchar 操作符结束。注 7：没有路
//      径的字符（例如空格字符）可以只由宽度值后跟 endchar 操作符组成。虽然宽度必须在字
//      体中指定，但它可以在 CFF 数据中指定为 defaultWidthX，此时不应在 charstring 中指定；
//      也可以作为与 nominalWidthX 的差值出现在 charstring 中。因此，最⼩的合法 charstring
//      仅由单个 endchar 操作符组成。注 8：endchar 还有⼀个已弃⽤的功能；⻅附录 C《兼容性
//      与已弃⽤操作符》。

// 提⽰操作符（Hint Operators）
//
// 所有提⽰必须在 charstring 程序的开头、宽度之后声明（详⻅第 3.1 节）。
//
// hstem |- y dy {dya dyb}* hstem (1) |-
//
//      指定⼀个或多个⽔平词⼲提⽰（关于⽔平词⼲提⽰的更多信息⻅下节）。这允许⽤⼀个 hstem
//      操作符携带多对数值参数，数量受栈深度限制。
//
//      要求词⼲按升序编码（按底边缘递增定义）。编码值都是相对的：第⼀对中，y 相对于 0，dy
//      指定距 y 的距离；后续每对的第⼀个值相对于前⼀对所定义的最后⼀条边缘。
//
//      宽度 –20 指定边缘提⽰的上边缘，–21 指定边缘提⽰的下边缘。所有其他负宽度值的含义未定
//      义。图 4 展⽰了使⽤上、下边缘提⽰编码字符词⼲的例⼦。边缘词⼲提⽰⽤于在控制词⼲宽度
//      不是主要⽬的的情形下控制词⼲边缘的位置。
//
//      图 4 边缘提⽰的编码。图 4 所⽰边缘词⼲提⽰的编码为：
//          121 –21 400 –20 hstem
//
//      图 5 展⽰了字符轮廓某特征上提⽰重叠的例⼦。重叠的提⽰必须通过两个 hintmask 操作符来
//      解决，使它们不会同时激活。
//
//      ⽔平词⼲提⽰不得互相重叠。如有重叠，必须紧跟提⽰声明之后使⽤ hintmask 操作符来建⽴
//      所需的互不重叠的提⽰集合。hintmask 之后还可以在路径中再次使⽤，以激活另⼀组互不重叠
//      的提⽰。
//
//      图 5 重叠提⽰的编码。图 5 所⽰例⼦的编码为：
//          280 100 –70 40 hstem
//
// vstem |- x dx {dxa dxb}* vstem (3) |-
//
//      在 x 坐标 x 与 x+dx 之间指定⼀个或多个垂直词⼲提⽰，其中 x 相对于坐标轴原点。要求词
//      ⼲按升序编码（按左边缘递增定义）。编码值都是相对的：第⼀对中，x 相对于 0，dx 指定距
//      x 的距离；后续每对的第⼀个值相对于前⼀对所定义的最后⼀条边缘。
//
//      宽度 –20 指定边缘提⽰的右边缘，–21 指定边缘提⽰的左边缘。所有其他负宽度值的含义未定
//      义。垂直词⼲提⽰不得互相重叠。如有重叠，必须紧跟提⽰声明之后使⽤ hintmask 操作符来建
//      ⽴所需的互不重叠的提⽰集合。hintmask 之后还可以在路径中再次使⽤，以激活另⼀组互不重
//      叠的提⽰。
//
// hstemhm |- y dy {dya dyb}* hstemhm (18) |-
//
//      含义与 hstem (1) 相同，但如果 charstring 包含⼀个或多个 hintmask 操作符，则必须使⽤
//      它来代替 hstem。
//
// vstemhm |- x dx {dxa dxb}* vstemhm (23) |-
//
//      含义与 vstem (3) 相同，但如果 charstring 包含⼀个或多个 hintmask 操作符，则必须使⽤
//      它来代替 vstem。
//
// hintmask |- hintmask (19 + mask) |-
//
//      指定哪些提⽰激活、哪些不激活。如果任何提⽰重叠，必须使⽤ hintmask 来建⽴⼀个互不重
//      叠的提⽰⼦集。hintmask 在 charstring 中可以出现任意多次。出现在 hintmask 之后的路径
//      操作符受新提⽰集合影响，但当前点不移动。如果词⼲提⽰区重叠且未通过 hintmask 操作符
//      妥善管理，结果未定义。
//
//      掩码（mask）数据字节定义如下：
//      1. 数据字节的数量恰好是引⽤ charstring 程序开头所声明词⼲提⽰数量所需的字节数，每个
//         提⽰占⼀位。
//      2. 掩码的每⼀位（从第⼀个字节的最⾼有效位开始）按 charstring 开头声明提⽰的顺序表⽰
//         相应的提⽰区。
//      3. 掩码中每⼀位的值为“1”表⽰相应提⽰应激活，位值为“0”表⽰该提⽰应停⽤。
//      4. 掩码中未使⽤的位（如有）必须为零。
//
//      如果 hstem 和 vstem 提⽰都在 charstring 开头声明，且该序列之后紧跟 hintmask 或 cntrmask
//      操作符，则可以不包含 vstem 提⽰操作符。例如，图 6 展⽰了⼀个带有 hstem 和 vstem 提⽰
//      的字符的⼀部分：
//
//      图 6 提⽰编码⽰例。如果第⼀组提⽰是 280 到 380 的 hstem 和 400 到 450 的 vstem（且只
//      定义了这三个提⽰），则提⽰应指定为：
//          280 100 –70 40 hstemhm 400 50 hintmask 0xa0
//
//      其中⼗六进制数据 0xa0（10100000）指⽰在路径构造开始时哪些提⽰处于激活状态。注意，这⾥
//      使⽤ hstemhm 来表⽰启⽤了提⽰替换（hint substitution）。
//
// cntrmask |- cntrmask (20 + mask) |-
//
//      指定要控制的字怀空间及其相对优先级。操作符之后字节中的掩码位引⽤各词⼲提⽰声明：第⼀
//      个字节的最⾼有效位指向第⼀个声明的词⼲提⽰，依此类推直到最后⼀个提⽰声明。受控制的字
//      怀是由被引⽤词⼲提⽰界定的那些。第⼀个 cntrmask 命令中置为 1 的位具有最⾼优先级；后
//      续 cntrmask 命令指定优先级较低的字怀（⻅图 1 及相应⽰例）。

// 算术操作符（Arithmetic Operators）
//
// abs num abs (12 9) num2
//
//      返回 num 的绝对值。
//
// add num1 num2 add (12 10) sum
//
//      返回两个数 num1 和 num2 的和。
//
// sub num1 num2 sub (12 11) difference
//
//      返回 num1 减去 num2 的结果。
//
// div num1 num2 div (12 12) quotient
//
//      返回 num1 除以 num2 的商。发⽣上溢时结果未定义，下溢时结果为零。
//
// neg num neg (12 14) num2
//
//      返回 num 的相反数。
//
// random random (12 23) num2
//
//      返回区间 (0,1] 内的伪随机数 num2，即⼤于零且⼩于或等于⼀。
//
// mul num1 num2 mul (12 24) product
//
//      返回 num1 与 num2 的乘积。发⽣上溢时结果未定义，下溢时返回零。
//
// sqrt num sqrt (12 26) num2
//
//      返回 num 的平⽅根。若 num 为负，结果未定义。
//
// drop num drop (12 18)
//
//      从 Type 2 参数栈移除栈顶元素 num。
//
// exch num1 num2 exch (12 28) num2 num1
//
//      交换参数栈顶的两个元素。
//
// index numX … num0 i index (12 29) numX … num0 numi
//
//      从参数栈顶起取出第 i 个元素，并将该元素的副本压⼊栈中。若 i 为负，则复制栈顶元素。
//      若 i ⼤于 X，操作未定义。
//
// roll num(N–1) … num0 N J roll (12 30) num((J–1) mod N) … num0 num(N–1) … num(J mod N)
//
//      将参数栈上的元素 num(N–1) … num0 循环移动 J 位。J 为正表⽰向上移动，J 为负表⽰向
//      下移动。N 必须是⾮负整数，否则操作未定义。
//
// dup any dup (12 27) any any
//
//      复制参数栈的栈顶元素。

// 存储操作符（Storage Operators）
//
// 存储操作符利⽤⼀个暂存数组（transient array），提供存储和检索暂存数组数据的⼿段。暂存
// 数组为中间值提供⾮持久性存储。除显式使⽤ put 操作符外，没有初始化该数组的⼿段；数组中
// 存储的值在单个字符渲染完成后不再保留。暂存数组的元素个数⻅附录 B《Type 2 Charstring
// 实现限制》。
//
// put val i put (12 20)
//
//      将 val 存⼊暂存数组中由 i 指定的位置。
//
// get i get (12 21) val
//
//      取出暂存数组中由 i 指定位置存储的值并压⼊参数栈。如果在当前 charstring 执⾏期间，
//      对 i 的 get 先于 put 执⾏，则返回值未定义。

// 条件操作符（Conditional Operators）
//
// and num1 num2 and (12 3) 1_or_0
//
//      若 num1 和 num2 都⾮零，则将 1 压⼊栈中；若任⼀参数为零，则将 0 压⼊栈中。
//
// or num1 num2 or (12 4) 1_or_0
//
//      若 num1 或 num2 任⼀⾮零，则将 1 压⼊栈中；若两个参数都为零，则将 0 压⼊栈中。
//
// not num1 not (12 5) 1_or_0
//
//      若 num1 ⾮零则返回 0；若 num1 为零则返回 1。
//
// eq num1 num2 eq (12 15) 1_or_0
//
//      若 num1 等于 num2，则将 1 压⼊栈中；否则将 0 压⼊栈中。
//
// ifelse s1 s2 v1 v2 ifelse (12 22) s1_or_s2
//
//      若 v1 ≤ v2，则将值 s1 留在栈上；若 v1 > v2，则将 s2 留在栈上。s1 和 s2 的值通常
//      是⼦例程的加偏置编号；⻅第 2.3 节。

// ⼦例程操作符（Subroutine Operators）
//
// ⼦例程编号通过利⽤数值空间的负半区来更紧凑地编码，这有效地使可紧凑编码的⼦例程编号数
// 量翻倍。所应⽤的偏置取决于 subr（gsubr）的数量：若 subr（gsubr）数量⼩于 1240，偏置
// 为 107；否则若⼩于 33900，偏置为 1131；否则为 32768。将该偏置加到编码的 subr（gsubr）
// 编号上，即可在 subr（gsubr）数组中找到相应条⽬。即使 FontSet 只包含⼀个字体，也可以
// 使⽤全局⼦例程。
//
// callsubr subr# callsubr (10) –
//
//      以索引 subr#（实际为 subr 编号加上⼦例程偏置数，如第 2.3 节所述）调⽤ Subrs 数组
//      中的 charstring ⼦例程。Subrs 数组的每个元素都是⼀个与其他 charstring 同样编码的
//      charstring。调⽤⼦例程之前压⼊ Type 2 参数栈的参数，以及⼦例程压⼊该栈的结果，按
//      ⼦例程的编码⽅式起作⽤。调⽤未定义的 subr（gsubr）结果未定义。
//
//      这些⼦例程通常⽤于编码在字体程序中反复出现的路径操作符序列，例如衬线轮廓序列。⼦
//      例程调⽤可以嵌套，深度以附录 B 中的实现限制为准。
//
// callgsubr globalsubr# callgsubr (29) –
//
//      ⼯作⽅式与 callsubr 相同，只是它调⽤的是全局⼦例程。
//
// return – return (11) –
//
//      从局部或全局 charstring ⼦例程返回，并在相应的 call(g)subr 之后继续执⾏。

// TrueType 轮廓和指令（TrueType outlines and instructions）
// https://learn.microsoft.com/en-us/typography/opentype/spec/ttch01
//
// TrueType 基础（TrueType Fundamentals）
//
// OpenType 支持 TrueType 字形轮廓格式，其中字形轮廓以二阶贝塞尔样条（second-order Bezier
// splines）描述。当轮廓被光栅化时，字体中包含的 TrueType "提示"（hinting）指令可用于优化
// 光栅化结果。特别是，TrueType 指令集提供了一种强大的手段，可在所有或特定字号和分辨率下
// 控制字形轮廓。
//
// 本章介绍创建和编写包含 TrueType 轮廓数据的 OpenType 字体所需的基本概念。首先概述将设计
// 从纸张转化为可发送到输出设备的位图所涉及的步骤，然后详细审视该过程中的每个步骤。本章还
// 包括 TrueType 指令及其解释方式的简介。其他章节提供更深入的详细信息：
//  1.  编写 TrueType 字形指令
//  2.  TrueType 指令集
//  3.  图形状态（Graphics State）摘要
//
// 在本章剩余部分中，"TrueType 字体" 将指包含 TrueType 字形轮廓的 OpenType 字体。
//
// 从设计到字体文件。TrueType 字体可以源自纸上绘制的新设计，或在计算机屏幕上创建的设计。
// TrueType 字体也可以通过转换其他格式的字体获得。无论哪种情况，都需要创建一个 TrueType
// 字体文件，其中除其他内容外，还需以 TrueType 格式描述字体中的每个字形轮廓。
//      在纸上设计字形 → 数字化过程 → 字体中的数字轮廓
//
// 从字体文件到纸张。本节描述将 TrueType 字体文件中的字形显示在光栅设备上的过程。首先，将
// 字体文件中存储的轮廓缩放到请求的字号。一旦缩放，构成轮廓的点不再以描述原始轮廓时使用的
// 字体单位（FUnits）记录，而变成了特定于设备的像素坐标。
//
// 接下来，由解释器执行与此字形关联的指令。执行指令的结果是为请求的字形生成一个网格拟合
// （grid-fitted）轮廓。然后对该轮廓进行扫描转换，生成可在目标设备上渲染的位图。光栅化八
// 步概览：
//  1.  在 TrueType 字体文件中使用 FUnit 坐标数字化轮廓
//  2.  缩放器将 FUnits 转换为像素坐标，并将轮廓缩放到应用程序请求的字号
//  3.  轮廓"调整"到新网格
//  4.  缩放后的轮廓（像素坐标）
//  5.  解释器执行与字形关联的指令并进行网格拟合
//  6.  网格拟合后的轮廓
//  7.  扫描转换器决定点亮哪些像素
//  8.  在光栅设备上渲染位图
//
// 数字化一个设计。本节描述用于确定定义字形轮廓的点位置的坐标系。同时记录了字形相对于坐标
// 轴的放置方式。
//
// 轮廓（Outlines）。在 TrueType 字体中，字形形状通过其轮廓描述。字形轮廓由一系列轮廓线
// （contours）组成。简单字形可能只有一个轮廓线。更复杂的字形可以有两个或更多轮廓线。复合
// 字形（composite glyph）可以通过组合两个或更多更简单的字形来构建。某些没有可见表现形式
// 的控制字符将映射到没有轮廓线的字形。
//
//  图 1-1：分别具有一个、两个、三个轮廓线的字形
//  .---------------------.---------.------------------------------------.
//  |     ._____________  |  .---.  | .---------------------+~.,         |
//  |    /             /  |  |   |  | '-.,_     .------+~.,_    -,       |
//  |   / .+-------.  /   |  *---*  |      |    |           '*~   *      |
//  |  / /        /  /    |  .___.  |      |    |              '   '     |
//  | '--        /  /     |  |   |  |      |    |              '   '     |
//  |           /  /      |  |   |  |      |    |            .~  .*      |
//  |          /  /       |  |   |  |      |    '________.~*' .~*        |
//  |         /  /        |  |   |  |      |                  *~_,       |
//  |        /  /         |  |   |  |      |    .---------+~.,_   ~_     |
//  |       /  /          |  |   |  |      |    |              *~,  *~   |
//  |      /  /           |  |   |  |      |    |                 '   '  |
//  |     /  /            |  |   |  |      |    |                 '   '  |
//  |    /  /             |  |   |  |      |    |                ~    ~  |
//  |   /  /              |  |   |  | ._,.-'    '___________..~*'  .~*   |
//  |  .__/               |  '---'  | '________________________..~*      |
//  '---------------------'---------'------------------------------------'
//
// 轮廓线由直线和曲线组成。曲线由一系列定义二阶贝塞尔样条的点描述。TrueType 贝塞尔样条
// 格式使用两种类型的点来定义曲线：在曲线上的点和不在曲线上的点（控制点）。定义曲线时，
// 离曲线点和在曲线点的任意组合都是可接受的。直线由两个连续的在曲线点定义。
//
//  图 1-2：由一系列在曲线和离曲线点组成的字形描述
//
// 构成曲线的点必须按连续顺序编号。顺序是递增还是递减，对于确定组成字形的填充模式很重要。
// 曲线的方向必须满足：如果沿点编号递增的方向跟随曲线，则黑色空间（填充区域）始终位于右
// 侧。
//
// FUnits 与 em 方块（square）。在 TrueType 字体文件中，点位置以字体单位（font units），
// 即 FUnits 描述。FUnit 是 em 方块中最小的可度量单位，em 方块是一个用于确定字形大小和对
// 齐的假想方块。em 方块的尺寸通常等于字体的全字身高加上一些额外的间距，以防止在不添加额
// 外行距（leading）进行排版时文本行发生碰撞。
//
//      大写字母 Q 的垂直尺寸
//      .---------------------·--------------------------------------
//      |                     |                                    ^
//      |      ~ ~ * ~ ~      |------------------------------      |
//      |    * *       * *    |  ^                         ^       |
//      |   * '         ' *   |  |                         |       |
//      |   ' '         ' '   |  | 升部（Ascent）          | Body  | EM
//      |    、*        * ，  |  v                         |       |
//      |_______*_*_~_*_*_____|_________ 基线（Baseline）  |       |
//      |         '、',       |  ^  降部（Descent）        |       |
//      |           *~_.      |__v_________________________v_      |
//      |                     |                                    v
//      '---------------------'--------------------------------------
//
// 虽然在金属活字时代，字形不能超出 em 方块，但数字字体不受此限制。em 方块可以做得足够大，
// 以完全包含所有字形，包括带重音符号的字形。或者，如果方便，字形的某些部分可以延伸到 em
// 方块之外。TrueType 字体可以处理这两种方式，因此选择权在于字体制造商。图 1-3：延伸到 em
// 方块之外的字符，一个升部（ascender）和降部（descender）超出 em 方块的 script f 字形。
//
// em 方块定义了一个二维坐标网格（coordinate grid），其 x 轴描述水平方向上的移动，y 轴描述
// 垂直方向上的移动。这将在下一节中更详细地讨论。
//
// FUnits 与网格。数字化字体的关键决策是确定描述构成字形轮廓的点时将使用的分辨率。这些点代
// 表网格中的位置，其最小可寻址单位称为 FUnit 或字体单位。网格是一个二维坐标系统，其 x 轴描
// 述水平方向上的移动，y 轴描述垂直方向上的移动。网格原点坐标为 (0,0)。网格不是无限平面。每
// 个点必须在 -16384 到 +16383 FUnits 的范围内。根据所选分辨率的不同，可寻址网格位置的范围
// 会更小。
//
// 坐标网格粒度（即每 em 的单位数，upem）的选择由字体制造商决定。如果每 em 单位数选择为 2 的
// 幂（如 2048），轮廓缩放将最快。注意，每 em 单位数在 x 和 y 方向上始终相同。
//
//      图 1-4：坐标系，从 (-16384,-16384) 到 (16383, 16383) 的笛卡尔坐标系
//
//                                  ^ y
//                                  |            。(16383, 16383)
//                                  |
//                                  |
//              --------------------|--------------------> x
//                                  |
//                                  |
//                  。              |
//              (-16384, -16384)    |
//
// em 方块的原点不需要与字形轮廓有任何一致的关系。然而，在实践中，应用程序依赖于给定字体的
// 字形放置存在某种约定。对于罗马字体（设计为水平排列），y 坐标值 0 通常被假定为对应字体的
// 基线；这通过字体 'head' 表 flags 字段的位 0 明确指示。x 坐标为 0 没有特定的含义，但制造
// 商可以通过为 x 原点选择标准含义来提高应用程序的性能。
//
// 一般来说，左侧字距（left side bearing）指的是沿 x 轴在初始笔位置（initial pen position）
// 和最左侧轮廓极端点（leftmost extreme outline point）之间的关系。左侧字距点是一个虚拟轮廓
// 点，参见下一章中的 "虚点（Phantom Points）" 部分，对应于初始笔位置。'hmtx' 表为每个字形提
// 供一个左侧字距值，表示左侧字距点与最左侧轮廓极端点之间的 x 坐标差。这使应用程序能够确定 em
// 方块原点与笔位置之间的 x 轴关系。为提高性能，可以设置 'head' 表 flags 字段的位 1，以指
// 示所有字形的左侧字距点位于 x = 0，尽管这不是必需的。
//
// 例如，你可以放置一个字形，使其美学中心（aesthetic center）位于 x 坐标值 0。也就是说，当
// 一组如此设计的字形被放置在一列中，且它们的 x 坐标值 0 重合时，它们看起来会很好地居中。此
// 选项可用于汉字或任何垂直排版的字体。另一种选择是将每个字形放置在其最左侧轮廓极端点的 x
// 值等于该字形的左侧字距。以这种方式创建的字体可能允许某些应用程序更快地打印到 PostScript
// 打印机。
//
//      图 1-5：罗马字体中字形原点的两种可能选择。第一种情况（左图）左侧字距为 x 零点。第二
//      种情况（右图）字符的美学中心为 x 零点
//             ^ y                                  ^ y
//             |                                    |
//             |    ~ ~ * ~ ~                   ~ ~ * ~ ~
//             |  * *       * *               * *   |   * *
//             | * '         ' *             * '    |    ' *
//             | ' '         ' '             ' '    |    ' '
//             |  、*        * ，             、*   |    * ，
//          ---|-----*-*-~-*-*------> x  --------*-*|~-*-*------> x
//             |                                    |
//
// 对于不同文字的字体，x 原点和 y 原点的含义可能偏好各种约定。为获得最佳的高亮和插入符号效果，
// 字符主体（Body）应大致位于前进宽度（advance width）的中心。例如，对称字符应具有相等的左右
// 侧承接字距。
//
//  1.  Initial Pen Position（初始笔/光标位置），这是绘制字形前，笔（光标）在水平排版中的起始
//      位置。可以理解为当前字形即将开始绘制的"原点"。
//      * 对于一行文本中的第一个字，初始笔位置通常在基线的最左端
//      * 完成一个字形绘制后，笔会向前移动，这个位置就成为下一个字形的初始笔位置
//  2.  Left Side Bearing Point（左侧字距点），这是一个虚拟轮廓点（不是字形实际轮廓上的点），
//      它对应于初始笔位置（initial pen position）。在 TrueType 字体中，它属于"虚点"（Phantom
//      points）之一，用于定义字形与排版流之间的空间关系。简单来说：
//      * 左侧字距点 = 初始笔位置
//      * 它与字形实际轮廓之间通常有一段空白距离，这段距离就是左侧字距（Left Side Bearing，简称 LSB）
//  3.  Leftmost Extreme Outline Point（最左侧轮廓极端点），这是字形实际轮廓中最靠左的那个点。
//      它是字形可见部分的物理边界。例如，字母 "o" 的最左侧点是它圆形轮廓的最左端；字母 "A"
//      的最左侧点可能是其左下衬线或左斜笔画的端点。
//  4.  Advance Width（前进宽度 / 字宽），这是从当前字形的初始笔位置到下一个字形的初始笔位置
//      之间的总水平距离。它决定了在排版时，绘制完当前字形后，笔要向前移动多少才能开始绘制下
//      一个字形。Advance width 通常由以下部分组成：
//          Advance Width = Left Side Bearing + 字形实际宽度 + Right Side Bearing
//          Left Side Bearing（左侧字距） = 最左侧轮廓极端点与左侧字距点（即初始笔位置）之间的水平距离
//          Right Side Bearing（右侧字距） = 最右侧轮廓极端点到 advance width 终点的距离
//
// em 方块的粒度由每 em 的 FUnits 数（或更简单地说是每 em 单位数）决定。em 方块被划分为 FUnits
// 后定义了一个坐标系，其中一个单位等于一个 FUnit。在此坐标系中定义的所有点必须具有整数位置。每
// em 单位数越大，在 em 方块内寻址位置的精度越高。图 1-6：两个 em 方块，左图为每 em 8 单位，右
// 图为每 em 16 单位。
//
// FUnits 是相对单位，因为它们的大小随 em 方块大小的变化而变化。对于给定字体，每 em 单位数保持
// 不变，无论字号大小如何。然而，每 em 的点数（points per em）将随字形的字号（point size）而变
// 化。当字形以 9 点（9 points）显示时，em 方块恰好为 9 点高；当字体以 10 点显示时，em 方块恰好
// 为 10 点高，依此类推。由于每 em 单位数不随字体显示的字号而变化，FUnit 的绝对大小随字号的变化
// 而变化。图 1-7：72 点的 M 和 127 点的 M 及其 em 方块。两种情况下每 em 单位数均为 8。
//
// 由于 FUnits 相对于 em 方块，无论字体以何种字号渲染，字形上的给定位置在 FUnits 中始终具有相同
// 的坐标位置。这很方便，因为它使得只需考虑原始轮廓即可对轮廓点进行一次指令编写，而更改将适用于
// 字形最终以任何字号和分辨率渲染。
//
// 缩放字形（Scaling a Glyph）。本节描述如何将字体文件中存储的主大小（master size）字形轮廓缩放
// 到应用程序请求的字号。
//
// 设备空间（Device Space）。无论用于定义字形轮廓的 em 方块分辨率如何，在显示该字形之前，必须对
// 其进行缩放以反映要显示它的大小、变换以及输出设备的特性。缩放后的轮廓必须使用反映绝对而非相对
// 测量系统的单位来描述字符轮廓。在这种情况下，构成字形轮廓的点（points）以像素（pixels）为单位
// 描述。直观地说，像素是将要出现在屏幕或打印机上的实际输出位。为了在管理轮廓时获得更高的精度，
// TrueType 将像素坐标描述到最近的六十四分之一像素。
//
// 将 FUnits 转换为像素（Coverting FUnits to Pixels）。em 方块中的值通过乘以一个比例转换为像素坐
// 标系中的值。该比例为：
//      font_point_size * resolution / (72 points per inch * units_per_em)
//      1em => units_per_em
//      1em => font_point_size
//      units => font_point_size
//      1-unit => font_point_size / units_per_em
//      1-point => 1 / 72 in
//      1-inch => 72 points，即 2.54 厘米
//      1-inch => n pixels，即 2.54 厘米
//      n-dpi => n dots per inch 或 n pixels per inch
//      1-unit => (font_point_size / units_per_em) * (1 / 72) inches
//             => (font_point_size / (72 * units_per_em)) inches
//             => (font_point_size / (72 * units_per_em)) * n pixels
//      1-unit => (n * font_point_size / (72 * units_per_em)) pixels
//
// 其中 font_point_size 是要显示字形的字号，resolution 是输出设备的分辨率（每英寸像素数）。
// 分母中的 72 反映了每英寸的点数（points）。例如，假设在 72 dpi 屏幕上以 18 点显示时，某
// 个字形特征在 FUnits 中的长度为 550。同时假设每 em 有 2048 个单位。以下计算表明该特征长
// 4.83 像素。
//      550 * 18 * 72 / (72 * 2048) = 4.83
//
// 显示设备特性（Display Device Characteristics）。任何特定显示设备的分辨率由显示的每英寸点数或
// 像素数（dpi）指定。例如，Windows 下的 VGA 显示器被视为 96 dpi 设备，大多数激光打印机的分辨率
// 为 300 dpi 或更高。某些设备在水平和垂直方向上可能具有不同的分辨率（即非方形像素）；例如，旧式
// EGA 显示器的分辨率为 96 dpi × 72 dpi。在这种情况下，必须区分水平每英寸点数和垂直每英寸点数。
//
// 每 em 像素数取决于输出设备的分辨率。在 72 dpi 设备上，18 点字符每 em 有 18 个像素。将设备分辨
// 率更改为 300 dpi，则每 em 有 75 个像素；或更改为 1200 dpi，则每 em 有 300 个像素。图 1-8：在
// 粗、中、细网格上数字化的数字 8（72 dpi、300 dpi 和 1200 dpi 下的 18 点数字 8）
//
// 在特定设备上以特定字号显示字体时，会产生以每 em 像素数（ppem）衡量的有效分辨率。计算每 em 像
// 素数的公式为：
//      ppem = dpi * font_point_size / 72
//           = (pixels per inch) * (inches per pica point) * (pica points per em)
//           = (pixels per inch) * (1 / 72 inches per point) * (pica points per em)
//           = (pixels per inch) * ((1 / 72) * font_point_size) inches per em
//           = (dpi * ((1 / 72) * font_point_size)) pixels per em
//           = (dpi * font_point_size / 72) pixels per em
//
// 在 300 dpi 激光打印机上，12 点字形将有 12×300/72 即 50 ppem。在 2400 dpi 的照排机上，它将
// 有 12×2400/72 即 400 ppem。在 VGA 显示器上，12 点字形将有 12×96/72 即 16 ppem。类似地，在
// 72 dpi 设备上，12 点字符的 ppem 为 12×72/72，即 12。最后一个计算指出了一条有用的经验法则：
// 在任何 72 dpi 设备上，字体点数大小和每 em 像素数相等。注意：在数字字体排印中，一英寸恰好包
// 含 72 点。然而，在传统字体排印中，一英寸包含 72.2752 点（而不是 72）；也就是说，一点等于
// 0.013836 英寸。
//
// 如果你知道 ppem，在 FUnits 和像素空间坐标之间转换的公式如下。给定每 em 2048 单位和 12 每
// em 像素，em 坐标位置 (1024, 0) 将产生设备像素坐标 (6, 0)。
//      pixel_coordinate = em_coordinate * ppem / upem
//
// 网格拟合字形轮廓（Grid-fitting a Glyph Outline）。编写字形指令的基本任务是识别原始设计的
// 关键特征，并使用指令确保当字形在不同设备上的不同字号渲染时，这些特征得以保留。一致的笔画
// 粗细、一致的灰度、均匀的间距以及消除像素丢失是常见的目标。
//
// 为实现这些目标，有必要确保在字形光栅化时点亮正确的像素。正是点亮的像素创建了字形的位图图
// 像。由于字形轮廓的形状决定了哪些像素将构成该字符在特定字号下的位图图像，因此有时需要更改
// 或扭曲原始轮廓描述以产生高质量的图像。这种对轮廓的扭曲（distortion）称为网格拟合（grid-fitting）。
//
// 下图说明了网格拟合字符如何扭曲原始设计中的轮廓。图 1-9：12 点轮廓，未网格拟合（左）和网
// 格拟合后（右）。如上图所示，TrueType 中采用的网格拟合远远超出了将字形左侧字距（left side
// bearing）对齐到像素网格（pixel grid）。这种复杂的网格拟合由指令指导。网格拟合的有益效果
// 如下图所示。网格拟合是根据与字形关联的指令拉伸字形轮廓的过程。一旦字形被网格拟合，点编号
// 保持不变，但点在坐标网格中的实际位置可能已经移动。也就是说，给定编号的点的坐标在字形网格
// 拟合后很可能已经改变。
//
//      图 1-10：12 点轮廓和位图，未网格拟合（左-轮廓线条细）和网格拟合后（右-轮廓线条粗）
//
//          |                               |      |            。 。 。 。         |
//          |      。                       |      |         。             。      |
//          |              。    。    。   |      |      。                  。    |
//          |   。            。       。   |      |   。 。      。 。       。    |
//          |   。      。    。       。   |      |   。      。    。       。    |
//          |   。   。       。            |      |   。   。    。          。    |
//          |   。   。       。    。      |      |   。   。    。 。    。       |
//          |   。      。    。 。         |      |   。   。 。 。 。 。    。    |
//          |      。                       |      |      。               。       |
//          |         。                    |      |      。 。         。 。       |
//          |              。 。            |      |           。 。 。             |
//
// 指令是什么（What are instructions）？TrueType 指令集提供了大量命令，旨在允许设计师指定
// 字符特征（character features）应如何渲染。指令是在缩放时保留字符设计的机制。换句话说，
// 指令控制字形轮廓在特定字号或设备上如何进行网格拟合。
//
// 编写字形指令将以特定方式在特定目标设备上的特定字号重塑轮廓，使得正确的像素包含在其轮廓
// 内。重塑轮廓（reshaping the outline）意味着移动轮廓点。被指令作用过的点称为被"触碰"（touched）
// 的点。注意，一个点实际上不需要移动就能被触碰。它只需被指令作用即可。例如，参见 MDAP 指令。
//
// TrueType 字体可以带或不带指令使用。不带指令的字体通常在足够高的分辨率和字号下会产生良好
// 的结果。不带指令的字体产生良好结果的字号范围不仅取决于输出设备分辨率和字符字号，还取决
// 于特定的字体设计。字体的预期用途也可以是决定是否应为特定字体添加指令的因素。对于大多数
// 字体，如果在低分辨率设备上小字号的易读性很重要，添加指令将是关键。
//
// 编写字体指令是一个涉及分析字形设计中的关键元素并使用 TrueType 指令集确保它们得以保留的
// 过程。指令足够灵活，允许在字号较小时将大致相同的特征"同质化（homogenized）"，同时允许在
// 像素足够多的字号下展现原始设计的完整风味。
//
// TrueType 解释器如何知道应以何种方式扭曲轮廓以产生理想的结果？这些信息包含在附加到字体中
// 每个字形的指令中。指令指定了在缩放时应保留的字形设计方面（aspects of a glyph's design）。
// 例如，使用指令可以控制单个字形或字体中所有字形的高度。你还可以保留字形内设计元素之间的
// 关系，从而确保例如小写 m 的三个垂直笔画（three vertical stems）在较小时不会有显著差异。
//
// 下图说明了在特定字号下更改字形轮廓如何产生更优的结果。它们显示了一个未编写指令的 9 点
// Arial 小写 m 由于笔画与像素中心关系的偶然效应（偏离像素中心）而丢失了一条笔画。在第二
// 个字形中，指令已将笔画对齐到网格，使得字形没有类似的损失。图 1-11：9 点 Arial m，未编
// 写指令（左），编写指令后（右）。
//
// TrueType 解释器（The TrueType Interpreter）。本节描述 TrueType 解释器的操作。顾名思义，
// 解释器"解释"或执行指令。更具体地说，解释器处理指令流或指令序列。通常，这些指令从解释器
// 栈中获取参数，并将结果放在该栈上。唯一的例外是一小部分用于将数据推入解释器栈的指令。这
// 些指令从指令流中获取参数。
//
// 解释器的所有操作都在图形状态（Graphics State）的上下文中进行，这是一组变量，其值指导解
// 释器的操作并确定特定指令的确切效果。有关更多信息，请参见下面的"图形状态"。解释器的操作
// 可以总结如下：
//  1.  解释器从指令流中获取一条指令，指令流是指令操作码和数据的有序序列。操作码大小为 1
//      字节。数据可以由单个字节或两个字节（一个字）组成。如果指令从指令流中获取字，它将
//      通过组合两个字节来创建这些字。字采用大端序：高字节在指令流中先出现，低字节后出现。
//      以下指令流将按照后续示例中的方式显示。注意，指针指示下一条要执行的指令。
//          * Other Instr *
//          *   Push      * <--
//          *   byte      *
//          *   byte      *
//          *   byte      *
//          *   Push      *
//          *   high      *
//          *   low       *
//          *   high      *
//          *   low       *
//          * Other Instr *
//          * Other Instr *
//  2.  执行指令
//      - 如果是压入指令，它将从指令流中获取参数
//      - 任何其他指令将从栈中弹出所需的任何数据
//      - 指令产生的任何数据都被推入解释器栈。如前面的讨论所示，解释器栈是一个 LIFO（后进先出）
//        数据结构。指令从放在栈上的最后一项中获取所需的任何数据。从栈中移除顶部项的操作通常称
//        为弹出。当指令产生某个结果时，它将该结果推入栈顶，在那里它可能成为下一条指令的潜在输
//        入。
//        指令集包括一整套用于操作栈的运算符，包括将项压入栈、从栈中弹出项、清除栈、复制栈元素
//        等操作。
//      - 执行的效果取决于构成图形状态的变量的值。
//      - 指令可以修改一个或多个图形状态变量。例如图形状态变量 rp0 使用从解释器栈中获取的值进
//        行更新。
//  3.  重复此过程，直到没有更多指令需要执行。
//
// 使用指令（Using Instructions）。指令可以出现在构成 TrueType 字体的字体文件表中的多个位置。
// 它们可以作为字体程序（font program）、控制值程序（control value program）的一部分出现，或
// 作为字形数据出现。出现在前两个中的指令适用于整个字体。在字形数据（'glyf' 表）中找到的指令
// 适用于字体中的单个字形。每次运行字体程序、控制值程序或字形程序时，解释器栈都会被清除。
//
// 字体程序（The Font Program）。字体程序存储在字体的 'fpgm' 表中，由一组指令组成，这些指令在
// 应用程序首次访问字体时执行一次。它用于创建函数定义（FDEFs）和指令定义（IDEFs）。在字体程序
// 中定义的函数和指令可以在控制值程序或字形程序中使用。在字体程序中定义的函数和指令也可以在控
// 制值程序中重新定义。
//
// 控制值程序（The Control Value Program）。控制值程序存储在字体的 'prep' 表中，由一组指令组
// 成，每当 x 或 y 方向的每 em 像素数发生变化或变换发生变化时执行。它用于对控制值表（Control
// Value Table）进行字体范围的更改，而不是管理单个字形。
//
// 控制值表（CVT）是一个编号列表，用于简化在编写字体指令时保持跨字形一致性的任务。CVT 从字体
// 的 'cvt ' 表初始化，然后使用控制值程序中的指令进行管理。参见"管理控制值表"。CVT 中的条目
// 可以通过两种间接指令（MIRP 和 MIAP）之一引用。CVT 条目可用于存储需要在字体中多个字形之间
// 保持相同的值。例如，指令可能引用一个 CVT 条目，其目的是在整个字体中统一笔画粗细。
//
//      图 1-12：一些示例 CVT 条目
//
//      条目 #      值          说明
//      0           0           大写和小写平底（基线）
//      1           -39         大写圆底
//      2           -35         小写圆底
//      3           -33         数字圆底
//      4           1082        x 高度平底
//      5           1114        x 高度圆重叠
//      6           1493        平底帽高
//      7           1522        圆顶帽高
//      8           1463        数字平底
//      9           1491        数字圆顶
//      10          1493        平底升部
//      11          1514        圆顶升部
//      12          157         x 笔画粗细
//      13          127         y 笔画粗细
//      14          57          衬线
//      15          83          i 的点与主体之间的间距
//
// 引用 CVT 中值的指令称为间接指令，与从字形轮廓中获取值的直接指令相对。作为 TrueType 字体
// 文件的一部分，CVT 中的值以 FUnits 表示。当轮廓从 FUnits 转换为像素单位时，CVT 中的值也
// 会转换。写入 CVT 时，你可以使用字形坐标系中的值（使用 WCVTP）或原始 FUnits 中的值（使用
// WCVTF）。解释器将适当地缩放所有值。从 CVT 读取的值始终为像素（F26Dot6 格式）。
//
// 控制值程序还可用于设置图形状态变量（参见下面的"图形状态"）。控制值程序可以使用 INSTCTRL
// 指令确定对图形状态变量的此类更改是否对使用指令的字形程序可见。控制值程序还可用于定义或
// 重新定义函数或指令，或重新定义字体程序中定义的函数或指令。给定函数或指令的最新定义将在
// 后续字形程序中使用。如果函数或指令首先在字体程序中定义，随后在控制值程序中重新定义，但
// 控制值程序在另一次字号或变换变化时运行未再次重新定义，则使用前一次控制值程序运行的重新
// 定义。
//
// 每次运行控制值程序时，区域 0，"暮光区（twilight zone）"，的轮廓数据都会初始化为 0。有关
// 区域 0 的详细信息，请参见"编写 TrueType 字形指令"章节中的"区域"。
//
// 存储区（The Storage Area）。解释器还维护一个存储区，由可用于临时存储来自解释器栈的数据
// 的内存部分组成。值可以由字体程序、控制值程序或字形程序写入存储区。字体程序写入的值可以
// 在控制值程序或字形程序中使用。类似地，在控制值程序中写入的值可以在字形程序中使用。有关
// 字形程序中写入值的持久性，请参见"字形程序与持久性"。存在可以读取存储数据值和向存储写入
// 新值的指令。存储位置范围从 0 到 n-1，其中 n 是字体文件中 'maxp' 表的 maxStorage 条目建
// 立的值。值为 32 位数字。
//
//      图 1-13：一些存储区条目
//          地址    值
//          0       343
//          1       241
//          2       -27
//          3       4654
//          4       125
//          5       11
//
// 图形状态（The Graphics State）。图形状态由变量及其值的表组成。所有指令都在图形状态的上下文
// 中操作。图形状态变量具有"图形状态摘要"中指定的默认值。它们的值可以使用指令确定或更改。图形
// 状态建立了所有字形被解释时的上下文。所有图形状态变量都有默认值。其中一些值可以根据需要在 CVT
// 程序中更改。无论默认值如何，它将在开始解释任何字形时重新建立。换句话说，图形状态没有跨字形
// 记忆。在处理单个字形时更改图形状态变量的值将导致仅对该字形保持有效的更改。
//
// 字形程序与持久性（Glyph Programs and Persistence）。字形程序可以在存储区或 CVT 中写入值。实
// 现可以在字形程序结束后持久化存储区或 CVT 中设置的值，允许这些值在其他字形程序中使用，或（仅
// 对于存储值）在控制值程序的后续运行中使用。实现也可以选择不这样做；也就是说，当字形程序结束时，
// 实现可以在运行该字形程序之前将存储区和 CVT 变量重置为其先前的值。另请注意，实现可以在字形程
// 序运行后缓存结果。由于这些原因，字体开发人员不应假设在字形程序运行时设置的值可用于影响应用程
// 序随后显示的字形。例如，模拟打字机色带的逐渐退化（to simulate gradual degradation of a
// typewriter ribbon）。
//
// 字形程序可以更改区域 0（"暮光区"）中的轮廓数据。这些值应始终持久化到下次运行控制值程序为止。
// 复合字形在这方面需要特别考虑。处理复合字形时，实现应处理组件字形（component glyphs），包括
// 运行其字形程序，而不是依赖组件字形先前缓存的结果。此外，即使实现通常不在字形程序运行后持久
// 化这些值，组件字形的字形程序中的存储区和 CVT 值也应可用于复合字形程序。
//
// 扫描转换器（The Scan Converter）。TrueType 扫描转换器获取字形的轮廓描述并生成该字形的位图
// 图像。TrueType 扫描转换器提供两种模式：带或不带丢落控制（dropout control），下文将描述。在
// 不带丢落控制的模式下，扫描转换器使用简单算法来确定哪些像素属于该字形。规则可以表述如下：
//
// 规则 1 - 如果像素的中心位于字形轮廓内，则该像素被点亮并成为该字形的一部分。
// 规则 2 - 如果轮廓恰好落在像素的中心，则该像素被点亮。
//
// 如果某点具有非零环绕数（non-zero winding number），则该点被视为字形的内部点。环绕数本身
// 通过从问题点向无穷远绘制一条射线来确定。射线指向的方向不重要。从零开始计数，每当字形轮
// 廓从右向左或从下向上穿过射线时，我们减一。这种交叉称为"开过渡"（on transition）。每当字
// 形轮廓从左向右或从上向下穿过射线时，我们加一。这种交叉称为"关过渡"（off transition）。如
// 果最终计数为非零，则该点是内部点。轮廓的方向可以通过查看点编号来确定。方向始终是从较低的
// 点编号指向较高的点编号。
//
// 下图演示了使用环绕数确定点是否在字形内部。点 p1 经历了一系列四次过渡（开过渡、关过渡、
// 开过渡、关过渡）。由于序列是偶数，环绕数为零，该点不在字形内部。第二个点 p2 经历了一次
// 关过渡，接着一次开过渡，再接着一次关过渡，产生环绕数 +1。该点在字形内部。
//
//      图 1-14：确定点的环绕数
//      |            。 。 。 。         |
//      |         。             。      |
//      |      。                  。    |
//      |   。 。      。 。       。    |
//      |   。      。    。       。    |
//      |   。   。    。          。    |
//      |   。   。 *--。-。----。-------|------> -1 +1 -1 +1 = 0
//      |   。   。 。 。*。-。----。----|------>    +1 -1 +1 = 1
//      |      。               。       |
//      |      。 。         。 。       |
//      |           。 。 。             |
//
// 什么是丢落（dropout）？每当字形的内部存在一个连通区域，其中包含两个黑色像素，但无法通过
// 一条只经过黑色像素的直线连接这两个像素时，就会发生丢落。
//
//      图 1-15：字母 m 有两个丢落。
//          。。*1  。。。*2  。
//          。        。      。
//          。        。      。
//          。        。      。
//          。        。      。
//          。        。      。
//
// 防止丢落（Preventing Dropouts）。TrueType 指令旨在允许你对字形进行网格拟合，使得无论使用
// 何种字号或变换，简单的扫描转换器都会点亮所需的像素。预见字形可能经历的所有可能变换可能很
// 困难。因此，很难编写指令以确保对于每种期望的变换，轮廓都能进行适当的网格拟合扭曲。对于每
// em 像素数很少的情况和复杂字体，这个问题尤其困难。在这些情况下，某些字形的渲染可能包含丢落。
//
// 可以通过查看连接两个相邻像素中心的假想线段来测试潜在的丢落。如果该线段被一条开过渡轮廓和
// 一条关过渡轮廓同时相交，则存在潜在的丢落条件。只有当两条轮廓线继续向两个方向延伸以切割其
// 他相邻像素中心之间的线段时，潜在的丢落才会成为实际的丢落。如果两条轮廓线在穿过扫描线后立
// 即连接在一起（形成短桩，stub），则不会发生丢落，尽管字形的笔画可能变得比期望的短。
//
// 为防止丢落，字体制造商可以选择让扫描转换器使用两条附加规则：
//
// 规则 3 - 如果两个相邻像素中心之间的扫描线（水平或垂直）被一条开过渡轮廓和一条关过渡轮廓同
//          时相交，且两个像素都尚未被规则 1 和规则 2 点亮，则点亮最左侧的像素（水平扫描线）
//          或最底部的像素（垂直扫描线）。
// 规则 4 - 仅当两条轮廓在两侧继续与其他扫描线相交时才应用规则 3。也就是说，不要为"短桩"点亮
//          像素。检查与相交扫描线段形成正方形的扫描线段，以验证它们是否被两条轮廓相交。这些
//          可能是与穿过丢落扫描线段相交的轮廓不同的轮廓。这种情况非常不可能，但可能需要在某
//          些奇异字形中通过网格拟合来控制。
//
// 字体制造商可以选择仅使用采用规则 1 和规则 2 的简单扫描转换器，或者可以选择性调用规则 3 或
// 规则 4。使用哪种扫描转换器模式的决定通过 SCANCTRL 指令控制。可以在字体范围内做出选择，或者
// 可以为每个字形指定不同的选择。在控制值程序中做出的选择将是整个字体的默认值。在单个字形的指
// 令中对默认值所做的更改将仅适用于该字形。

// OpenType 布局概述
// OpenType 布局表为高质量国际化排版提供高级排版功能：
// 字符和字形之间的丰富映射，允许连字、位置形式、替代形式和其他替换。
// 执行二维定位和字形附着的能力。
// 显式的脚本和语言信息，以便文本处理应用程序可以相应地调整其行为。
// 一种开放格式，允许字体开发人员定义自己的排版功能。
// 本概述介绍了 OpenType 布局字体模型的强大功能和灵活性。OpenType 布局表在 OpenType 规范的单独章节中有更详细的描述。请参阅高级排版表。
// 不同 OpenType 布局表中使用的通用格式记录在"OpenType 布局通用表格式"章节中。
// 注册的 OpenType 布局标签（用于脚本、语言、功能和基线）记录在 OpenType 布局标签注册表章节中。
// OpenType 布局一览
// OpenType 布局解决了正确显示许多不同脚本以及任何脚本中精细排版的复杂排版需求。
// 使用 OpenType 布局表，字体可以支持字符的替代形式并提供用于访问它们的数据。例如，在阿拉伯文中，字符的形状通常随字符在单词中的位置而变化。如下图所示，ha 字符将采用四种形状中的任何一种，取决于它是单独出现还是出现在单词的开头、中间或结尾。执行文本布局时，文本处理应用程序评估 ha 字符出现的单词位置上下文，然后 OpenType 布局数据通知应用程序应为每个上下文替换哪个字形。
// 阿拉伯文 ha 的不同位置形式字形
// 图 1a 阿拉伯文字符 ha 的独立、词首、词中、词尾形式。
// 类似地，当文本垂直定位而非水平定位时，OpenType 布局数据可被应用程序用于替换正确的字符形式，例如汉字。例如，汉字在垂直定位时使用括号的替代形式。
// 水平和垂直布局中的汉字及括号
// 图 1b 垂直定位汉字时使用的括号替代形式。
// OpenType 布局数据还支持连字的合成和分解。例如，使用拉丁文书写英语、法语和其他语言时，可以用单个连字（如"fi"）替换其组成字形——在这种情况下，是"f"和"i"。相反，单个"f"和"i"字形可以替换连字，可能是为了给文本处理应用程序在调整字形间距以填充两端对齐文本行时提供更大的灵活性。或者类似地，许多阿拉伯文脚本字形序列可以用单个连字字形替换。
// f 和 i 的字形及 fi 连字字形
// 图 1c 两个拉丁文字形及其关联的连字。
// 三个阿拉伯文字形序列及关联的连字字形
// 图 1d 三个阿拉伯文字形及其关联的连字。
// 字形替换只是 OpenType 布局扩展字体功能的一种方式。包含 OpenType 布局表的字体还可以指定字形如何相互附着。使用 X 和 Y 坐标来指定字形之间的附着点。此功能可用于将变音符号附着到字形，以及创建连笔（草书）文本。
// OpenType 布局字体还可以包含基线信息，指定如何水平或垂直定位字形。由于基线可能因一种脚本（字符集）而异，此信息对于对齐混合不同语言字形的文本特别有用。
// 一行包含拉丁文和阿拉伯文脚本的文本
// 图 1e 一行文本，基线已调整，混合拉丁文和阿拉伯文脚本。
// 尽可能多地，OpenType 布局表只定义特定于特定字体的信息。这些表不尝试编码在特定语言惯例或特定脚本的排版中保持恒定的信息。这种会在给定语言的所有字体中复制的信息属于该语言的文本处理应用程序，而不属于字体。
// OpenType 布局术语
// OpenType 布局模型围绕字形、脚本、语言系统、功能和查找组织。
// 字符与字形
// 用户不查看或打印字符：用户查看或打印字形。字符是数据中具有数字表示的抽象实体；字形是字符的可视化。例如，字符大写字母 A 在 Times New Roman Bold 等字体中通过字形"A"进行视觉描绘。字体包含字形的集合。要检索字形，客户端使用字体中的 'cmap' 表信息，将客户端的字符代码映射到表中的字形索引。
// 字形还可以表示字符的组合和字符的替代形式：字形和字符并不严格一一对应。例如，用户可能键入两个字符，这两个字符可能更好地用单个连字字形表示。相反，同一个字符在单词的开头、中间或结尾可能采用不同的形式，因此字体可能需要几个不同的字形来表示单个字符。OpenType 布局字体包含一个表，为客户提供有关可能的字形替换的信息。
// 替代 & 字形
// 图 1f & 字符的多个字形。
// 脚本
// 脚本由一组相关字符组成，可能由一种或多种语言使用。拉丁文、阿拉伯文和泰文是脚本的示例。字体可以支持来自单个脚本或许多脚本的字符。在 OpenType 布局字体中，脚本由唯一的 4 字节标签标识。
// 拉丁文、汉字和阿拉伯文字形
// 图 1g 拉丁文、汉字和阿拉伯文脚本中的字形。
// 语言系统
// 脚本又可以分为语言系统。例如，拉丁文脚本用于书写英语、法语或德语，但每种语言对文本处理都有自己的特殊要求。字体开发人员可以选择提供针对脚本、语言系统或两者定制的信息。
// 与脚本不同，当文本处理客户端检查正在使用的字符时，语言系统不一定显而易见。为避免歧义，用户或操作系统需要标识语言系统。否则，客户端将使用为每个脚本提供的默认语言系统信息。
// 使用英语和土耳其语语言系统应用小型大写字母功能的字符串'dil'：使用土耳其语语言系统时，i 的小型大写字母带点。
// 图 1h 英语和土耳其语语言系统之间的差异。
// 功能和查找
// 功能定义字体的排版功能，是应用程序用于调用这些功能的手段。这些可以包括显示某些脚本所必需的功能，以及精细排版的其他功能。支持变音符号定位的字体将实现 'mark' 功能。支持垂直字形替换的字体将实现 'vert' 功能。
// 查找是用于实现功能所调用功能的数据。查找表描述了应用程序应应用的字形替换或字形定位操作，以实现所需的排版效果。功能可用于以字体无关的方式引用排版功能，但查找提供用于实现该功能的字体特定数据。
// OpenType 布局功能模型为字体开发人员提供了灵活性，允许他们选择适合给定设计或其客户要求的字体功能。该模型还为未来的增强提供了可扩展性：持续创新可以随着时间的推移定义新功能的功能。
// 显示脚本、语言系统和功能表组织关系的框图
// 图 1i 脚本、语言系统、功能以及替换和定位表的查找之间的关系。
// OpenType 布局表
// OpenType 布局使用五个表：GSUB、GPOS、BASE、JSTF 和 GDEF。这些表及其格式在单独的章节中讨论。以下段落提供简要概述。
// GSUB：包含有关字形替换的信息，以处理单个字形替换、一对多替换（连字分解）、美学替代、多个字形替换（连字）和上下文字形替换。
// GPOS：包含有关字形 X 和 Y 定位的信息，以处理单个字形调整、成对字形调整、草书附着、标记附着和上下文字形定位。
// BASE：包含有关逐脚本基线偏移的信息。
// JSTF：包含两端对齐信息，包括空白和 Kashida 调整。
// GDEF：包含有关字体中所有单个字形的信息：类型（简单字形、连字或组合标记）、附着点（如果有）和连字插入符（如果是连字字形）。
// MATH 表是一个附加的高级排版表，包含布局数学表达式和公式所需的特殊度量值和其他数据。
// 通用表格式：几个通用表格式被 OpenType 布局表使用。
// 使用 OpenType 布局进行文本处理
// 文本处理客户端遵循标准过程，将用户输入的字符字符串转换为定位的字形。要使用 OpenType 布局字体生成文本：
// 使用字体中的 'cmap' 表，客户端将字符代码转换为字形索引序列。
// 使用 GSUB 表中的信息，客户端修改生成的字形序列，根据需要替换位置或垂直字形、连字或其他替代形式。
// 使用 GPOS 表中的定位信息和 BASE 表中的基线偏移信息，客户端然后定位字形。
// 使用设计坐标，客户端确定设备无关的换行符。设计坐标是高分辨率和设备无关的。
// 使用 JSTF 表中的信息，客户端两端对齐行，如果用户指定了此类对齐。
// 客户端栅格化字形行并在对应于输出设备分辨率的设备坐标中渲染字形。
// 在整个过程中，文本处理客户端保持原始字符串字符与最终渲染文本的字形索引之间的关联。此外，客户端可以在文本流中保存语言和脚本信息，以清楚地将原始文本中的运行与特定的排版行为相关联。
// 从左到右和从右到左的文本
// 当 OpenType 文本布局引擎应用 Unicode 双向算法并到达需要对具有偶数（即从左到右，LTR）解析级别的运行执行镜像时，它执行以下操作：
// 字形级镜像：
// 对整个 LTR 运行应用 'ltrm' 功能以替换镜像形式。
// LTR 字形替代：
// 对整个 LTR 运行应用 'ltra' 功能以优化字形选择。
// 对于具有奇数（即从右到左，RTL）解析级别的运行，引擎执行以下操作：
// 字符级镜像：
// 对于 RTL 运行中的每个字符 i：
// 如果它被 OMPL 映射到字符 j 且 cmap(j) 非零：
// 在字符 i 处使用字形 cmap(j)。
// 这里 OMPL 指的是 OpenType 镜像对列表，cmap(j) 指的是 Unicode 'cmap' 子表中从代码点 j 映射的字形。
// 例如，假设 U+0028 左括号出现在解析级别为 1 的运行中。该运行中该代码点处的字形将被替换为 cmap(U+0029)，因为 {U+0028, U+0029} 是 OMPL 中的一对。
// 字形级镜像：
// 引擎对整个 RTL 运行应用 'rtlm' 功能。如果存在，该功能替换 OMPL 对第一元素未覆盖的字符的镜像形式（否则，它会取消字符级镜像的效果）。
// OMPL 的数据内容与 Unicode 5.1 的双向镜像字形属性文件相同，并且不会修订。因此，如果需要，'rtlm' 功能将为（a）Unicode 5.1 中具有"镜像"属性但没有适当 Unicode 5.1 字符镜像的代码点，以及（b）所有未来添加到 Unicode 的"镜像"属性（无论是否存在字符镜像）提供镜像形式。
// 通过布局引擎和字体之间的这种分工，大多数字体不需要包含 'rtlm' 功能，因为其 Unicode 'cmap' 子表中的镜像形式就足够了。
// RTL 字形替代：
// 引擎对整个 RTL 运行应用 'rtla' 功能。如果存在，该功能替换适合从右到左文本的变体（镜像形式除外）。
// 实际上，引擎可以同时应用功能；因此，由字体供应商确保功能的查找顺序正确，以实现上述算法描述的效果。引擎可以以多种方式优化其实现，例如，利用字符级和字形级镜像不会同时应用于运行中同一元素的事实。
// OpenType 布局与字体变体
// OpenType 字体变体允许单个字体沿一个或多个变化轴支持许多设计变化。例如，具有字重和宽度变化的字体可能支持从细到黑的各种字重，以及从超压缩到超扩展的各种宽度。有关 OpenType 字体变体的一般信息，请参阅 OpenType 字体变体概述章节。
// 用于支持字体变体的数据被集成到用于 OpenType 布局的表中。字形轮廓和度量在字体变化空间中的变化可能会影响 OpenType 布局表中使用的网格设计距离，例如 GPOS 附着查找中使用的锚点位置。OpenType 布局格式中的数据元素可以与描述默认值如何针对不同变化实例进行调整的变化数据相关联。
// 在某些可变字体中，可能希望在字体的变化空间中的不同区域使用不同的字形替换或字形定位操作。例如，对于窄或重实例，其中字腔变小，可能希望进行某些字形替换，以使用具有某些笔画移除或轮廓简化的替代字形，以允许更大的字腔。这种效果可以使用 GSUB 或 GPOS 表中的功能变化表来实现。功能变化表在 OpenType 布局通用表格式章节中描述。另请参阅 OpenType 布局标签注册表中的必需变化替代（'rvrn'）功能。
// 可变字体的不同变化实例具有相同的字形 ID。因此，似乎可以在字形序列中应用查找，其中字形使用可变字体的不同变化实例进行格式化。然而，这样做可能导致不可预测的行为，因为字体开发人员可能无法充分控制查找表的生成方式，并且测试大量可能的跨实例交互是不可行的。由于这些原因，布局处理实现必须将可变字体的不同变化实例视为不同的样式运行，以用于 OpenType 布局处理。

// OpenType 布局通用表格式（OpenType Layout Common Table Formats）
// https://learn.microsoft.com/en-us/typography/opentype/spec/chapter2
//
// OpenType 布局使用五个表：字形替换表（GSUB）、字形定位表（GPOS）、基线表（BASE）、
// 两端对齐表（JSTF）和字形定义表（GDEF）。这些表使用一些相同的数据格式。本章定义这些
// 通用格式，并解释所有 OpenType 布局表中使用的约定。通用格式也用于数学排版表（MATH，
// Mathematical Typesetting Table）。单独的章节提供 GSUB、GPOS、BASE、JSTF、GDEF 和
// MATH 表的所有其他详细信息。本章末尾提供了说明通用数据格式的示例和列表。
//
// 概述。OpenType 布局表提供用于替换和定位字形的排版信息，这些操作是许多脚本正确文本
// 显示以及高质量排版所必需的。OpenType 布局数据按脚本（script）、语言系统、排版功能
// （typographic）和查找（lookup）组织。
//
// 在顶层，数据按脚本组织。脚本是用于以一种或多种语言书面形式表示的字形集合。例如，单
// 个脚本拉丁文用于书写英语、法语、德语和许多其他语言。相比之下，日语使用三种脚本书写：
// 平假名、片假名和汉字。使用 OpenType 布局，单个字体可以支持多种脚本。
//
// 对于每个脚本，数据然后按一个或多个语言系统组织，这允许字体支持不同语言上下文的不同
// 排版约定。例如，土耳其语与大多数使用拉丁文脚本书写的语言具有不同的大小写关系，这会
// 影响使用小型大写字母功能（'smcp'）时所需的字形选择。使用英语和土耳其语语言系统应用
// 小型大写字母功能（small caps feature）的字符串'dil'：使用土耳其语语言系统时，i 的小
// 型大写字母带点。英语和土耳其语语言系统之间的差异：
//      No features:            dil
//      Small caps, Englisth:   DIL
//      Small caps, Turkish:    DİL
//
// 语言系统特定功能，这些功能代表对字形的排版效果。一些功能示例包括用于替换日语中垂直
// 字形的 'vert' 功能、用于使用连字替代单独字形的 'liga' 功能，以及用于定位阿拉伯语中
// 变音符号相对于基础字形的 'mark' 功能。连字形成和标记定位，连字字形功能将 <et> 连字
// 替换为单个字形，'mark' 功能将变音符号定位在阿拉伯语连字字形上方。
//
// 在没有特定语言规则的情况下，默认语言系统指定用于给定脚本的功能（features）。例如，
// 阿拉伯文脚本的默认语言系统功能，将指定根据字形在单词中的位置替换词首、词中和词尾
// 字形形式的功能。应用程序可以使用自己的标准（its own criteria）来确定何时使用特定
// 语言系统（a specific language system）或默认语言系统。在对文本运行进行排版（doing
// layout）时，它使用一个或另一个系统指定的功能，但不会同时使用两者。
//
// 功能（features）通过文本处理客户端用于替换和定位字形的查找数据实现。查找描述了受操作影响的字形、应用于这些字形的操作类型以及生成的字形输出。
// 字体还可以在 GPOS 或 GSUB 表中包含 FeatureVariations 数据，允许在特定条件适用时将与功能关联的默认查找数据替换为备用查找数据。目前，此机制仅用于使用 OpenType 字体变体的可变字体。
// OpenType 布局与字体变体
// OpenType 字体变体允许单个字体沿一个或多个设计变化轴支持许多设计变化。例如，具有字重和宽度变化的字体可能支持从细到黑的各种字重，以及从超压缩到超扩展的各种宽度。有关 OpenType 字体变体的一般信息，请参阅 OpenType 字体变体概述章节。
// 选择不同的变化实例时，单个字形的设计和度量会改变。这可能会影响 GPOS、BASE、JSTF 或 GDEF 表中给出的字体单位值，例如附着锚点位置的 X 和 Y 坐标。这些表中给出的字体单位值适用于可变字体的默认实例。如果不同变化实例需要调整，则使用与字形轮廓和其他字体数据类似的过程，使用变化数据进行调整，如 OpenType 字体变体概述章节所述。GPOS、JSTF 或 GDEF 值的变化数据包含在 GDEF 表中的 ItemVariationStore 表中；BASE 值的变化数据包含在 BASE 表本身的 ItemVariationStore 表中。ItemVariationStore 的格式在 OpenType 字体变体通用表格式章节中详细描述。对于 GPOS、BASE、JSTF 或 GDEF 表中需要变化的字体单位值，通过下面描述的 VariationIndex 表提供对 ItemVariationStore 中特定变化数据的引用。
// 在某些可变字体中，可能希望在字体变化空间中的不同区域使用不同的字形替换或字形定位操作。例如，对于字腔变小的窄或重实例，可能希望进行某些字形替换，以使用具有某些笔画移除或轮廓简化的替代字形，以允许更大的字腔。这种效果可以使用 GSUB 或 GPOS 表中的 FeatureVariations 表来实现。FeatureVariations 表在下面描述。
// 表组织
// 两个 OpenType 布局表 GSUB 和 GPOS 使用相同的数据格式来描述它们支持的脚本和语言，以及用于每个支持脚本和语言的排版字形操作：ScriptList 表、FeatureList 表、LookupList 表和可选的 FeatureVariations 表。在 GSUB 中，这些表定义字形替换数据。在 GPOS 中，它们定义字形定位数据。本节描述这些格式之间的组织和关系；以下各节详细描述这些格式。
// ScriptList 标识字体中的脚本，每个脚本由 Script 表表示。每个 Script 表有一个默认语言系统表加上零个或多个特定语言系统的表。语言系统表引用 FeatureList 中定义的 Feature 表。每个 Feature 表引用 LookupList 中定义的 Lookup 表，这些表描述实现该功能的字形操作。
// 显示脚本、语言系统和功能表组织关系的框图
// 图 2d. 脚本、语言系统、功能以及替换和定位表的查找之间的关系
// 注意：BASE 和 JSTF 表中的数据也按脚本和语言系统组织。然而，数据格式与 GSUB 和 GPOS 中的不同，它们不包括 FeatureList 或 LookupList。BASE 和 JSTF 数据格式在 BASE 和 JSTF 章节中描述。
// 用于替换和定位字形的信息在查找子表中定义。每个子表提供一种类型的信息，取决于查找是 GSUB 还是 GPOS 表的一部分。例如，GSUB 查找可能指定要替换的字形以及发生替换的上下文，而 GPOS 查找可能指定字距调整的字形位置调整。OpenType 布局有八种类型的 GSUB 查找（在 GSUB 章节中描述）和九种类型的 GPOS 查找（在 GPOS 章节中描述）。
// 每个子表（扩展 LookupType 子表除外）包括一个 Coverage 表，列出将导致字形替换或定位操作的"覆盖"字形。Coverage 表格式在本章后面的部分描述。
// 某些替换或定位操作可以应用于字形组或类。GSUB 和 GPOS 查找子表使用类定义表将字形分配到类中。类定义表格式的描述在本章后面提供。
// 在非可变字体中，GPOS 查找子表还可以包含 Device 表，以调整特定输出尺寸和分辨率的缩放轮廓字形坐标。Device 表还可用于类似调整 BASE 和 GDEF 表中的基线度量或插入符偏移值。类似地，在可变字体中，GPOS 查找子表、BaseCoord 表和 CaretValue 表可以包含 VariationIndex 表，这些表引用变化数据以调整字体变化空间中不同变化实例的字体单位值。Device 和 VariationIndex 表在本章后面的部分描述。
// 如上所述，功能表引用查找列表中的一组查找。FeatureVariations 表允许在特定条件下将与给定功能一起使用的默认查找集替换为不同的查找集。这可以在可变字体中用于为不同的变化实例提供不同的替换或定位操作。例如，对于字腔变小的窄或重实例，可能希望进行某些字形替换，以使用具有某些笔画移除或轮廓简化的替代字形，以允许更大的字腔。
// 脚本和语言
// 三个表及其关联记录适用于脚本和语言：ScriptList 表及其脚本记录（ScriptRecord）、Script 表及其语言系统记录（LangSysRecord），以及语言系统表（LangSys）。
// ScriptList 表
// 字体可以包含一个或多个用于渲染各种脚本的字形组，这些脚本在 ScriptList 表中枚举。GSUB 和 GPOS 表都定义 ScriptList 表：
// GSUB 表使用 ScriptList 表来访问适用于脚本的字形替换功能。有关详细信息，请参阅字形替换表（GSUB）章节。
// GPOS 表使用 ScriptList 表来访问适用于脚本的字形定位功能。有关详细信息，请参阅字形定位表（GPOS）章节。
// ScriptList 表由字体中脚本表示的字形计数（ScriptCount）和记录数组（ScriptRecord）组成，每个脚本一个记录，字体为其定义了脚本特定功能（没有脚本特定功能的脚本不需要 ScriptRecord）。每个 ScriptRecord 由标识脚本的 ScriptTag 和指向 Script 表的偏移组成。ScriptRecord 数组按脚本标签的字母顺序存储。
// 字体中可以使用标签为 DFLT（默认）的 Script 表来定义非脚本特定的功能。如果文本格式化没有与特定脚本关联的脚本表，或者文本没有特定脚本（例如，它只包含符号或标点符号），应用程序应使用 DFLT 脚本表。
// 注意：如果符号或标点符号具有 Unicode 脚本属性"通用"，但与特定脚本的字符一起使用，则适用于这些符号或标点符号的功能不一定应组织在 DFLT 脚本下，而是可以组织在特定脚本下。应用程序可以将脚本中性字符与紧邻其前或后的脚本特定字符一起处理，以获得更好的处理效率。在这种情况下，应用程序将使用特定脚本的 Script 表来查找对这些中性字符进行操作的功能。然而，如果文本只包含中性字符，则仍使用 DFLT 脚本。
// 如果存在 DFLT 脚本表，它必须有一个默认语言系统表（defaultLangSysOffset 不得等于 NULL——见下文）。
// 由于语言使用特定脚本书写，通常预期语言特定的排版效果将与特定脚本关联，而不是与通用 DFLT 脚本关联。因此，DFLT 脚本表通常应只有默认语言系统表，没有特定语言表。然而，字体可以具有带有非默认语言系统表的 DFLT 脚本表，如果没有特定脚本的脚本表存在，或者文本上下文中没有特定脚本，应用程序可以使用其中一个特定语言系统的功能。在这种条件下，如果字体包含具有此配置的表，应用程序应支持使用与 DFLT 脚本关联的非默认语言系统表。
// 示例 1 在本章末尾显示了一个使用三种脚本的日文字体的 ScriptList 表和 ScriptRecord。
// ScriptList 表
// 类型	名称	描述
// uint16	scriptCount	ScriptRecord 的数量
// ScriptRecord	scriptRecords[scriptCount]	ScriptRecord 数组，按脚本标签字母顺序列出
// ScriptRecord
// 类型	名称	描述
// Tag	scriptTag	4 字节脚本标签标识符
// Offset16	scriptOffset	从 ScriptList 开头到 Script 表的偏移
// Script 表
// Script 表可以指定一个或多个语言系统，这些语言系统定义特定语言中脚本字形的行为。它还引用一个默认语言系统，该系统定义在没有语言特定信息时使用的脚本字形行为。
// Script 表以默认语言系统表（defaultLangSysOffset）的偏移开头，该表定义脚本默认行为的功能集。接下来是特定语言系统计数（langSysCount），然后是相应的语言系统记录数组（LangSysRecord）。每个记录使用语言系统标签（langSysTag）指定语言系统，并指定语言系统表（LangSys）的偏移。LangSysRecord 数组必须按语言系统标签的字母顺序排序。如果没有定义特定语言行为，langSysCount 字段设置为零，LangSysRecord 数组为空。
// Script 表
// 类型	名称	描述
// Offset16	defaultLangSysOffset	从 Script 表开头到默认 LangSys 表的偏移——可能为 NULL。
// uint16	langSysCount	langSysRecords 数组中的记录数。
// LangSysRecord	langSysRecords[langSysCount]	LangSysRecord 数组，按 LangSys 标签字母顺序排序。
// LangSysRecord
// 类型	名称	描述
// Tag	langSysTag	4 字节 LangSysTag 标识符。
// Offset16	langSysOffset	从 Script 表开头到 LangSys 表的偏移。
// 语言系统表
// 语言系统表（LangSys）标识用于脚本字形布局的功能。功能指定为 FeatureList 表中的从零开始的索引，定义在本章下一节。
// 可选地，LangSys 表可以定义一个必需功能索引（requiredFeatureIndex 字段），以指定在特定语言系统上下文中必需的一个功能。例如，在斯拉夫文脚本中，塞尔维亚语语言系统使用与俄罗斯语语言系统不同的某些字符字形。只能将一个功能索引值指定为必需功能。然而，这不是功能限制，因为功能和查找定义的结构使得一个功能表可以引用许多字形替换或定位查找。当没有定义必需功能时，requiredFeatureIndex 设置为 0xFFFF。
// 所有其他功能被视为语言系统表一般处理要求方面的可选功能。然而，应用程序可能在某些上下文中将某些功能视为必需，无论功能是否使用 requiredFeatureIndex 字段引用。特别是，应用程序可能将某些功能视为正确布局某些 Unicode 字符或脚本所必需的。此类更高级别的要求超出本规范的范围。
// 本章末尾的示例 2 显示了用于阿拉伯语脚本中上下文定位的 Script 表、LangSysRecord 和 LangSys 表。
// LangSys 表
// 类型	名称	描述
// Offset16	lookupOrderOffset	保留——设置为 NULL。
// uint16	requiredFeatureIndex	此语言系统所需功能的索引；如果没有必需功能，设置为 0xFFFF。
// uint16	featureIndexCount	featureIndices 数组中的元素数。
// uint16	featureIndices[featureIndexCount]	FeatureList 中的索引数组，顺序任意。
// lookupOrderOffset 字段保留供将来使用。featureIndices 数组中的索引可以按任意顺序排列。
// 功能和查找
// 概述
// 功能定义字体的高级布局功能，并命名以向文本处理客户端传达含义。考虑使用标签 'liga' 标识的功能来创建连字。由于其名称，客户端知道该功能的作用，并可以决定是否应用它。已经定义了几个功能，可以在字体和应用程序中使用；有关更多信息，请参阅 OpenType 布局标签注册表的功能标签部分。字体开发人员还可以定义自己的功能。
// 设计字体时，字体开发人员根据字体将支持的排版功能选择功能。对于每个功能，他们然后实现一个或多个查找，描述要执行的字形替换或字形定位操作。给定功能可以使用多个查找；在这种情况下，查找操作将按顺序执行。在某些情况下，可能需要按特定顺序应用多个查找才能获得所需效果。
// 对文本运行进行布局时，客户端选择要应用于运行的功能，然后按查找定义在 LookupList 中的顺序处理这些功能引用的查找。因此，在 GSUB 或 GPOS 表中，来自多个不同功能的查找可能在文本处理期间交错。由字体开发人员确定查找执行的操作的正确顺序。
// 客户端可以按有序阶段处理 GSUB 功能，每个阶段处理特定功能。客户端还可以在阶段之前或之间对字形序列执行某些操作，例如重新排序字形。许多脚本的正确支持需要此类处理。此类脚本特定处理的详细信息超出本规范的范围。然而，在任何此类功能处理阶段内，该阶段应用的功能引用的查找必须按 LookupList 顺序处理。
// 查找数据在一个或多个子表中定义，这些子表包含有关特定字形和要对它们执行的操作的信息。不同的查找类型支持不同类型的操作；例如，单个字形的定位调整与成对字形的定位调整。每种查找类型都有一个或多个相应的子表格式定义。子表类型和格式的选择取决于两个因素：应用于操作的信息的精确内容，以及所需的存储效率。有关所有查找类型和子表的完整定义，请参阅本文档的 GSUB 和 GPOS 章节。
// 当客户端在字形序列中定位目标字形或字形上下文并执行所描述的替换或定位操作时，查找处理完成。
// 功能和查找定义特定于给定字体中字形的信息。它们不编码在特定语言惯例或特定脚本排版中保持恒定的信息。这种会在给定脚本或语言的所有字体中复制的信息属于该语言或脚本的文本处理应用程序，而不属于字体。
// FeatureList 表
// GSUB 和 GPOS 表的头部包含指向 FeatureList 表的偏移，这些表枚举字体中的所有功能。FeatureList 中的功能不限于任何单个脚本。FeatureList 包含字体支持的所有脚本布局使用的 GSUB 或 GPOS 功能的完整列表。
// FeatureList 表在记录数组（FeatureRecord）中枚举功能。每个功能需要一个 FeatureRecord，由标识功能的特征标签和指向 Feature 表（在下一节中描述）的偏移组成。
// 注意：LangSys 表的 FeatureIndex 数组中存储的值是用于定位 FeatureList 表的 FeatureRecord 数组中的从零开始的索引。
// Feature 表描述了用于给定脚本和语言系统的给定功能的查找。对于不同脚本或语言系统，功能的实现通常不同，需要不同的 Feature 表。因此，当功能为多个脚本或语言系统实现时，FeatureList 表可能包含两个或更多具有相同功能标签的记录。
// FeatureRecord 数组应按功能标签的字母顺序排序。如果两个或更多记录具有相同的功能标签，则它们的相对顺序是任意的。
// FeatureList 表
// 类型	名称	描述
// uint16	featureCount	featureRecords 数组中的记录数。
// FeatureRecord	featureRecords[featureCount]	FeatureRecord 数组。
// FeatureRecord
// 类型	名称	描述
// Tag	featureTag	4 字节功能标识标签。
// Offset16	featureOffset	从 FeatureList 开头到 Feature 表的偏移。
// Feature 表
// Feature 表使用一个或多个查找定义功能的实现。GSUB 表中定义的 Feature 表包含对字形替换查找的引用；GPOS 表中定义的 Feature 表包含对字形定位查找的引用。如果功能需要字形替换和定位操作，则使用相同功能标签引用的 Feature 表需要在 GSUB 和 GPOS 表中都定义。
// Feature 表由功能参数表的偏移、功能列出的查找计数以及指向 LookupList 的任意排序索引数组组成。
// 功能参数表只能用于某些功能。功能参数表的格式特定于特定功能，并在 OpenType 布局标签注册表的功能标签部分中该功能的描述中指定。目前，仅为以下功能定义了功能参数表：
// 'cv01' – 'cv99'
// 'size'
// 'ss01' – 'ss20'
// 根据给定功能的规范，功能参数表可能是必需或可选的。功能参数表的长度必须在该表本身中隐式或显式指定。Feature 表中的 featureParamsOffset 字段给出相对于 Feature 表开头的偏移。如果没有为给定功能定义功能参数表，或者定义了功能参数表但在给定字体中未使用，则 featureParamsOffset 字段必须设置为 NULL。
// 要识别 GSUB 或 GPOS 表中给定脚本和语言系统的功能，文本处理客户端读取给定 LangSys 表中引用的每个 FeatureRecord 的功能标签。然后客户端选择要实施的功能，并使用所选功能的 Feature 表中的 lookupListIndices 数组获取所选功能的查找索引列表。接下来，客户端将索引按数字顺序排列到其 LookupList 顺序中。最后，客户端从 LookupList 检索引用的 Lookup 表，并将查找数据应用于替换或定位字形。
// 本章末尾的示例 3 显示了用于两种语言中连字替换的 FeatureList 和 Feature 表。
// Feature 表
// 类型	名称	描述
// Offset16	featureParamsOffset	从 Feature 表开头到功能参数表的偏移（如果为该功能定义并存在），否则为 NULL。
// uint16	lookupIndexCount	lookupListIndices 数组中的元素数。
// uint16	lookupListIndices[lookupIndexCount]	LookupList 中的索引数组——从零开始。
// LookupList 表
// GSUB 和 GPOS 表的头部包含指向 LookupList 表的偏移，用于字形替换和字形定位查找。LookupList 表包含指向 Lookup 表的偏移数组。字体开发人员定义数组中偏移的顺序，以控制文本处理客户端处理查找数据以执行字形替换或定位操作的顺序。
// 本章末尾的示例 4 显示了 LookupList 表中的三个连字查找。
// LookupList 表
// 类型	名称	描述
// uint16	lookupCount	lookupOffsets 数组中的元素数。
// Offset16	lookupOffsets[lookupCount]	从 LookupList 开头到 Lookup 表的偏移数组。
// Lookup 表
// Lookup 表定义用于实现功能的特定条件、类型和替换或定位操作结果。例如，替换操作需要目标字形索引列表、替换字形索引列表以及替换操作类型的描述。
// 描述查找操作的数据包含在一个或多个查找子表中。不同的查找类型支持不同类型的操作；例如，单个字形的定位调整与成对字形的定位调整。操作类型确定需要在查找子表中包含的信息。给定查找表可能只支持一种操作类型，因此可能只包含相同查找类型的子表。
// GSUB 表支持八种查找类型；GPOS 表支持九种查找类型。有关各种替换和定位查找的详细信息，请参阅 GSUB 和 GPOS 章节。
// 对于每种查找类型，定义了一种或多种子表格式。每种格式由操作所需信息的内容和所需存储效率确定。当字形信息最好以多种格式呈现时，单个查找可能包含多个子表，只要所有子表都是相同的查找类型。
// 在文本处理期间，客户端对字符串应用功能到某些字形序列。然后它按功能引用的查找在查找列表顺序中处理这些查找。对于每个查找，客户端在功能已应用的字形序列中的每个字形上处理该查找。在该查找已处理完整个字形序列后，它然后以相同方式处理功能引用的下一个查找。随着每个查找的处理，它作用于先前查找的替换或定位结果。这继续直到功能引用的所有查找都已处理。
// 应用程序可以同时处理多个功能的查找。在这种情况下，查找列表是所有这些功能引用的查找的并集，并且所有这些查找都按查找列表顺序处理。如果不同功能已应用于字符串的不同字形子序列，则每个查找仅应用于引用该查找的功能所应用的子序列。
// 查找指定一个或多个输入序列模式，每个模式有一个或多个字形。当在字符串中的字形序列上处理查找时，客户端从序列中的第一个字形开始，测试当前字形是否与查找指定的输入序列模式匹配。如果字形序列与任何查找输入序列模式不匹配，则该字形的查找处理完成，客户端前进到字形序列中的下一个字形。如果字形序列确实与查找输入序列模式匹配，则对匹配的输入序列执行相应的替换或定位操作。客户端执行替换或定位操作后，查找对该字形完成。要移动到"下一个"字形，客户端跳过参与查找操作的所有字形：被替换/定位的字形以及匹配输入序列中的任何其他字形。然而，对于成对定位操作（例如字距调整）有一个例外：序列中的"下一个"字形可能是输入序列对的第二个字形，而不是跳过整个输入序列。（有关详细信息，请参阅成对定位查找类型。）
// 如果 Lookup 表有多个子表，则按顺序处理子表，依次测试当前字形位置的字形序列是否与每个子表指定的输入序列模式匹配。如果字形序列与子表的模式不匹配，则处理移动到下一个子表。如果字形序列与任何查找子表的模式不匹配，则该字形位置的查找处理完成。如果字形序列确实与子表的模式匹配，则执行该子表的操作，并且查找处理完成——不再为该字形序列位置处理进一步的子表。
// GSUB 和 GPOS 表都包括允许链接上下文的查找类型：GSUB 查找类型 6 和 GPOS 查找类型 8。链接上下文查找类型支持指定在输入序列模式之前和之后的回溯和前瞻序列模式。这些也必须在字形序列中匹配，以便查找在给定位置应用于字形序列。与输入序列模式不同，回溯和前瞻序列的匹配不限于应用关联功能的字形序列。链接上下文查找可以为匹配输入序列中的字形指定操作，但不能为回溯或前瞻序列中的字形指定操作。处理链接上下文查找后，客户端通过跳过输入序列来设置"下一个"字形，但不跳过前瞻序列。
// Lookup 表指定定义查找子表中存储信息类型的查找类型。查找标志指定查找限定符，向文本处理客户端指示替换或定位字形时要使用的某些处理选项。偏移数组提供从 Lookup 表开头到一个或多个指定查找类型的查找子表的偏移。可选字段为标记字形提供附加限定符。
// Lookup 表
// 类型	名称	描述
// uint16	lookupType	GSUB 和 GPOS 的不同枚举。
// uint16	lookupFlag	查找限定符。
// uint16	subTableCount	subtableOffsets 数组中的元素数。
// Offset16	subtableOffsets[subTableCount]	从 Lookup 表开头到查找子表的偏移数组。
// uint16	markFilteringSet	如果设置了 USE_MARK_FILTERING_SET 查找标志，则 GDEF 标记字形集结构中的索引（从零开始）。
// subtableOffsets 数组中偏移的顺序决定查找子表的处理顺序。
// 查找标志使用两字节数据：
// 可以设置前四个低位中的每一个，以指定将查找应用于字形序列的附加指令。下表提供这些位使用的详细信息。
// 第五位指示 Lookup 表中是否存在 markFilteringSet 字段。
// 接下来的三位保留供将来使用。
// 高字节可用于指定标记字形类作为字形过滤器。
// LookupFlag 位枚举
// 掩码	名称	描述
// 0x0001	RIGHT_TO_LEFT	此位仅与草书附着定位（GPOS 查找类型 3）相关。设置此位时，匹配输入序列中的最后一个字形将定位在基线上。
// 0x0002	IGNORE_BASE_GLYPHS	如果设置，跳过基础字形
// 0x0004	IGNORE_LIGATURES	如果设置，跳过连字
// 0x0008	IGNORE_MARKS	如果设置，跳过所有组合标记
// 0x0010	USE_MARK_FILTERING_SET	如果设置，表示 Lookup 表结构包含 markFilteringSet 字段。布局引擎跳过不在指定标记过滤集中的所有标记字形。
// 0x00E0	reserved	供将来使用（设置为零）
// 0xFF00	MARK_ATTACHMENT_CLASS_FILTER	如果非零，跳过不在指定标记附着类中的所有标记。
// RIGHT_TO_LEFT 标志仅用于 GPOS 类型 3 查找，否则被忽略。客户端软件不使用它来确定文本方向。
// 如上所述，查找为已应用功能的字形序列中的每个字形处理。每种查找类型指定要匹配的输入序列模式：单个字形或字形序列，取决于查找类型。查找处理循环中的当前字形始终针对查找输入字形序列模式中的第一个字形进行测试。查找标志影响输入序列中其他字形的模式匹配，但不影响当前字形。对于链接上下文查找，标志还影响回溯和前瞻序列的匹配。
// IGNORE_BASE_GLYPHS、IGNORE_LIGATURES 或 IGNORE_MARKS 指 GDEF 表中字形类定义表中定义的基础字形、连字和标记。如果设置了这些标志中的任何一个，则查找必须忽略相应类型的字形；也就是说，必须处理其他字形，就好像这些字形不存在于字形序列中一样。
// 如果 MARK_ATTACHMENT_CLASS_FILTER 非零，则必须在 GDEF 表中的标记附着类定义表中定义标记附着类。处理字形序列时，查找必须忽略不在指定标记附着类中的任何标记字形；仅处理指定类中的标记。
// 如果任何查找设置了 USE_MARK_FILTERING_SET 标志，则 Lookup 头必须包含 markFilteringSet 字段，并且 GDEF 表中必须存在 MarkGlyphSets 表。查找必须忽略不在指定标记字形集中的任何标记字形；仅处理指定标记字形集中的字形。
// 如果指定了标记过滤集，这将取代查找标志中的任何标记附着类指示。如果设置了 IGNORE_MARKS 位，这将取代任何标记过滤集或标记附着类指示。
// 例如，在阿拉伯语文本中，字符字符串可能具有模式基础标记基础。该字符串可以转换为由两个组件组成的连字，每个基础字符一个组件，组合标记字形位于第一个组件上方。要生成此连字，字体开发人员将设置连字替换查找的 IGNORE_MARKS 位，告诉客户端忽略标记，首先替换连字字形，然后在后续 GPOS 查找中将标记字形定位在连字字形上方。或者，可以使用未设置 IGNORE_MARKS 位的替换查找来描述三组件连字字形，由第一个基础字形、标记字形和第二个基础字形组成。
// 再例如，将基础字形与上方标记创建连字的查找可以通过指定仅包含上方标记的标记附着类来跳过所有下方标记。
// 上下文查找类型支持查找数据的嵌套组织。在此结构中，查找子表指定可以修改的输入序列模式，然后引用 LookupList 中的一个或多个"嵌套"查找表，描述要应用于匹配序列中单个字形的操作。在这些情况下，主查找表中有一个 lookupFlag 字段，嵌套查找中有单独的 lookupFlag 字段。
// GPOS 查找类型 7 和类型 8 具有这种性质，以及 GSUB 查找类型 5、类型 6 和类型 8。对于这些查找类型，主查找的效果是过滤它们应用的字形序列，但不直接修改匹配序列中的字形。主查找表中的查找标志将影响此初始匹配过程。例如，IGNORE_MARKS 标志将导致在评估字形序列是否与查找指定的模式匹配时忽略标记字形。注意，RIGHT_TO_LEFT 标志从不用于主查找。
// 一旦序列与主查找中的模式匹配，就会在序列中的字形上处理嵌套查找。主查找表中的查找标志在嵌套查找处理时不被考虑。相反，考虑嵌套查找中的查找标志。注意，嵌套查找中的标志可能导致在初始主查找表匹配的字形序列上进行二级过滤。
// Coverage 表
// Lookup 表中的每个子表（扩展查找类型子表除外）引用一个 Coverage 表，该表指定受子表中描述的替换或定位操作影响的所有字形。GSUB、GPOS 和 GDEF 表依赖于这种覆盖概念。如果字形未出现在 Coverage 表中，客户端可以跳过该子表并立即移动到下一个子表。
// Coverage 表以两种方式之一通过字形 ID 标识字形：
// 作为字形集中的单个字形 ID 列表。
// 作为连续字形 ID 的范围。范围格式给出一个或多个起始和结束字形 ID 对，表示表覆盖的连续字形。
// 在 Coverage 表中，format 字段将格式指定为整数：1 = 列表，2 = 范围。
// Coverage 表为每个覆盖字形定义一个唯一的索引值，即 Coverage 索引。Coverage 索引是顺序的，从 0 到覆盖字形数减 1。此唯一值指定覆盖字形在 Coverage 表中的位置。客户端使用 Coverage 索引在子表中查找每个字形的值。
// Coverage 格式 1
// Coverage 格式 1 由格式字段和覆盖字形计数组成，后跟字形索引数组（glyphArray）。字形索引必须按数字顺序排列，以便对列表进行二分搜索。在 Coverage 表中找到字形时，其在 glyphArray 中的位置决定返回的 Coverage 索引——第一个字形的 Coverage 索引 = 0，最后一个字形的 Coverage 索引 = GlyphCount -1。
// 本章末尾的示例 5 显示了使用格式 1 列出字体中所有小写降部字形 ID 的 Coverage 表。
// CoverageFormat1 表：单个字形索引
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 1。
// uint16	glyphCount	字形数组中的字形数。
// uint16	glyphArray[glyphCount]	字形 ID 数组——按数字顺序。
// Coverage 格式 2
// 格式 2 由格式字段和字形索引范围计数组成，后跟记录数组（rangeRecords）。每个 RangeRecord 由起始字形索引、结束字形索引以及与范围起始字形关联的 Coverage 索引组成。范围必须按 startGlyphID 排序，并且必须不同，不重叠。
// 第一个范围的 Coverage 索引从零（0）开始，并顺序增加到（endGlyphId - startGlyphId）。对于每个后续范围，起始 Coverage 索引比前一个范围的结束 Coverage 索引大 1。因此，每个非初始范围的 startCoverageIndex 必须等于前一个范围的长度（endGlyphID - startGlyphID + 1）加上前一个范围的 startCoverageIndex。这允许使用公式快速计算任何范围内任何字形的 Coverage 索引：Coverage 索引（glyphID）= startCoverageIndex + glyphID - startGlyphID。
// 本章末尾的示例 6 显示了使用格式 2 标识字体中数字字形范围的 Coverage 表。
// CoverageFormat2 表：字形范围
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 2。
// uint16	rangeCount	RangeRecord 的数量。
// RangeRecord	rangeRecords[rangeCount]	按 startGlyphID 排序的字形范围数组。
// RangeRecord
// 类型	名称	描述
// uint16	startGlyphID	范围中的第一个字形 ID。
// uint16	endGlyphID	范围中的最后一个字形 ID。
// uint16	startCoverageIndex	范围中第一个字形 ID 的 Coverage 索引。
// 类定义表
// 为了效率和查找中的表示方便，字体开发人员可以将字形分组为字形类。类可用于 GSUB 和 GPOS 查找中的各种目的，包括描述字形上下文或要处理或忽略的一组标记。
// 考虑仅替换字形字符串中小写升部字形的替换操作。为了更容易描述替换的适当上下文，字体开发人员可以将字体的小写字形分为两类，一类包含升部，一类包含没有升部的字形。
// 字体开发人员可以将任何字形分配到任何类，每个类用整数标识。类定义表（ClassDef）将字形分配到类，从类 1 开始，然后是类 2，依此类推。所有未分配到类的字形归入类 0。在给定的类定义表内，字体中的每个字形恰好属于一个类。
// ClassDef 表可以有两种格式之一：一种将连续字形索引范围分配到不同的类，另一种将连续字形索引组放入同一个类。
// 类定义格式 1
// 第一种类定义格式（ClassDefFormat1）指定连续字形索引范围并列出相应的字形类值。当每个类中的字形索引未分组在一起时，此表可用于将每个字形分配到不同的类。
// ClassDefFormat1 表以格式标识符开头。表覆盖的字形 ID 范围由两个值标识：第一个字形的字形 ID（startGlyphID）和将分配类值的连续字形 ID 数（包括第一个）。整数数组列出分配给每个字形 ID 的类值，从 startGlyphID 的类值开始，并遵循与字形 ID 相同的顺序。任何未包含在覆盖字形 ID 范围内的字形被分配到类 0。
// 本章末尾的示例 7 使用格式 1 为字体中的小写、x 高度、升部和降部字形分配类值。
// ClassDefFormat1 表：类数组
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 1。
// uint16	startGlyphID	分配给类的第一个字形 ID。
// uint16	glyphCount	classValues 数组中的元素数。
// uint16	classValues[glyphCount]	类值数组——每个字形 ID 一个。
// 类定义格式 2
// 第二种类定义格式（ClassDefFormat2）定义属于同一类的多个字形索引组。每个组由连续顺序的字形索引范围组成。字形范围不得重叠。
// ClassDefFormat2 表包含格式标识符和 ClassRange 记录数组，这些记录指定字形 ID 范围以及分配给的类。记录必须按每个范围中的第一个字形 ID 排序。
// 任何未由 ClassRange 记录覆盖的字形被分配到类 0。
// 本章末尾的示例 8 使用格式 2 为阿拉伯语脚本中的四种类字形分配类值。
// ClassDefFormat2 表：类范围
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 2。
// uint16	classRangeCount	ClassRange 记录的数量。
// ClassRange	classRangeRecords[classRangeCount]	按 startGlyphID 排序的 ClassRangeRecord 数组。
// ClassRange 记录
// 类型	名称	描述
// uint16	startGlyphID	范围中的第一个字形 ID。
// uint16	endGlyphID	范围中的最后一个字形 ID。
// uint16	class	应用于范围内所有字形的类。
// 上下文查找子表的通用格式
// GSUB 和 GPOS 表各自使用不同的查找类型进行各种替换和定位操作。对于 GSUB 和 GPOS，都定义了上下文查找类型：
// GSUB 类型 5：上下文替换
// GPOS 类型 7：上下文定位
// 上下文查找类型支持指定被操作的输入字形序列，以及在序列中任何字形位置执行的操作列表。操作指定为对单独嵌套查找的引用（LookupList 中的索引）。操作为每个字形位置指定，但整个序列必须匹配，因此操作以上下文敏感方式指定。
// 由于上下文查找表链接到描述要执行的替换操作的"嵌套"查找表，因此每个嵌套查找表中以及主查找表中都有 lookupFlag 字段。有关主查找表和嵌套查找表中查找标志效果的详细信息，请参阅上面的 Lookup 表部分。
// 注意：字形序列以逻辑顺序给出。对于从右到左书写的文本，最右边的字形将是第一个；相反，对于从左到右书写的文本，最左边的字形将是第一个。
// 对于 GSUB 类型 5 和 GPOS 类型 7，定义了三种子表格式，以不同方式描述输入序列：
// 以特定字形 ID 表示（"简单字形上下文"）。
// 使用类定义表定义的字体集表示（"基于类的字形上下文"）。
// 使用 Coverage 表定义的字体集表示（"基于覆盖的字形上下文"）。
// 三种不同的子表格式使用不同的结构，但对于每种子表格式，结构在 GSUB 和 GPOS 表中是通用的。
// 此外，对于 GSUB 和 GPOS，都定义了链接上下文查找类型：
// GSUB 类型 6：链接上下文替换
// GPOS 类型 8：链接上下文定位
// 链接上下文查找在功能上类似于上下文查找，但添加了链接字形序列上下文：在输入序列之前的回溯字形序列，以及在输入序列之后的前瞻序列。（回溯和前瞻序列在下面更详细描述：请参阅链接序列上下文格式 1：简单字形上下文。）只能为输入序列中的字形指定操作，但回溯、输入和前瞻序列必须与当前正在处理的字形序列匹配，查找才能应用。完成指定操作后，客户端前进到匹配输入序列之后的字形位置（如果嵌套查找是 GPOS 类型 2、成对定位查找，则有特殊考虑）；特别是，客户端不会跳过匹配的前瞻序列。
// 为上下文查找定义了三种格式，类似于上下文查找的三种格式。使用的结构不同于上下文查找类型使用的结构，因为它们包含了链接上下文。但链接上下文结构在 GSUB 和 GPOS 表中是通用的。
// 注意：虽然替换和定位操作在 GSUB 和 GPOS 查找之间是不同的，但对于上下文查找类型，这种差异反映在嵌套查找中。例如，GSUB 上下文查找指定的操作由 GSUB 表中的嵌套查找引用。这样，GSUB 和 GPOS 表中使用的上下文查找子表结构可以是相同的，但指定的结果操作仍然是不同的。
// 对于上下文和链接上下文查找类型，定义了输入序列模式，并对匹配的字形序列执行操作。注意，模式在任何操作执行之前与当前字形序列匹配。在 GSUB 表内，替换操作将更改当前字形序列，但这不影响初始匹配操作。
// 序列查找记录
// 对于上下文和链接上下文查找的所有格式，使用通用记录格式来指定操作——嵌套查找——应用于输入序列中特定序列位置的字形。
// SequenceLookup 记录
// 类型	名称	描述
// uint16	sequenceIndex	输入字形序列中的索引（从零开始）。
// uint16	lookupListIndex	LookupList 中的索引（从零开始）。
// lookupListIndex 字段指示要应用于输入字形序列中由 sequenceIndex 指示的位置的 Lookup 表。
// 序列上下文格式 1：简单字形上下文
// GSUB 类型 5 格式 1 子表和 GPOS 类型 7 格式 1 子表以特定字形 ID 定义输入序列。可以指定多个序列，但每个序列都使用字形 ID 指定。
// 每个序列的第一个字形在 Coverage 表中指定。每个序列中的剩余字形在 SequenceRule 表中定义——每个序列一个。如果多个序列以相同的字形开头，则该字形 ID 必须在 Coverage 表中列出一次，并且相应的序列规则使用 SequenceRuleSet 表聚合——Coverage 表中指定的每个初始字形一个。
// 在字形序列中的给定位置评估 SequenceContextFormat1 子表时，客户端在 Coverage 表中搜索当前字形。如果找到，则检索相应的 SequenceRuleSet 表，并检查该集的 SequenceRule 表，以查看当前字形序列是否与任何序列规则中的输入序列模式匹配。使用第一个匹配的子表规则。
// SequenceContextFormat1 表
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 1。
// Offset16	coverageOffset	从 SequenceContextFormat1 表开头到 Coverage 表的偏移。
// uint16	seqRuleSetCount	SequenceRuleSet 表的数量。
// Offset16	seqRuleSetOffsets[seqRuleSetCount]	从 SequenceContextFormat1 表开头到 SequenceRuleSet 表的偏移数组（偏移可能为 NULL）。
// Coverage 表列出所有支持的输入字形序列的初始字形。seqRuleSetCount 应与 Coverage 表中的字形数量匹配。如果不同，则忽略额外的覆盖字形或额外的序列规则集。
// 每个覆盖字形有一个 SequenceRuleSet 表。seqRuleSetOffsets 数组中的偏移必须排序以匹配 Coverage 表中的字形顺序。
// SequenceRuleSet 表——所有以相同字形开头的上下文
// 类型	名称	描述
// uint16	seqRuleCount	SequenceRule 表的数量。
// Offset16	seqRuleOffsets[seqRuleCount]	从 SequenceRuleSet 表开头到 SequenceRule 表的偏移数组。
// SequenceRule 子表的偏移按所需结果排序。子表按偏移列出的顺序评估，并使用第一个与当前字形序列匹配的序列规则。较长、更具体的序列的规则通常排序在较短规则之前。
// 注意：如果一个规则指定的序列是另一个规则中指定的较长序列的初始子序列，并且较短的排序在较长的之前，则较长序列的规则将永远不会被使用。例如，考虑两个上下文 <abc><abcd><abc><abc><abcd><abcd>
// SequenceRule 表
// 类型	名称	描述
// uint16	glyphCount	输入字形序列中的字形数。
// uint16	seqLookupCount	SequenceLookup 的数量。
// uint16	inputSequence[glyphCount - 1]	输入字形 ID 数组——从第二个字形开始。
// SequenceLookup	seqLookupRecords[seqLookupCount]	序列查找记录数组。
// glyphCount 值是输入序列中的字形总数，包括第一个字形。inputSequence 数组按顺序指定输入序列中的剩余字形。（inputSequence 索引 0 处的字形对应于字形序列索引 1。）
// seqLookupRecords 数组列出指定要对输入序列中各个位置的字形执行的操作的序列查找记录。这些不必按序列位置顺序排列；它们按所需结果排序。所有序列查找记录按顺序处理，每个应用于前一条记录指示的操作结果。
// 序列上下文格式 2：基于类的字形上下文
// GSUB 类型 5 格式 2 子表和 GPOS 类型 7 格式 2 子表使用类定义表定义的字形类定义输入序列模式。可以指定多个序列模式，每个模式为每个输入序列位置指定一个字形类。
// 类分配整数编号，即类值。输入序列模式指定为类值序列。例如，模式 1, 4, 3 指定具有三个字形的字形序列集，第一个来自类 1，第二个来自类 4，第三个来自类 3。
// 每个模式在 ClassSequenceRule 表中指定。在第一个位置以相同类值开头的模式使用 ClassSequenceRuleSet 表聚合。
// SequenceContextFormat2 表具有指向 Coverage 表的偏移。Coverage 表包含作为任何基于类模式第一个字形出现的所有字形 ID。也就是说，Coverage 表包含所有作为任何指定输入序列模式中第一个字形的类中所有字形的字形索引列表。例如，如果模式以类 1 或类 2 开头，则 Coverage 表将列出类 1 或类 2 中的所有字形。Coverage 表中的字形 ID 只给出一次。
// 注意：由于类定义表的定义方式，每个字形 ID 恰好属于一个类。
// 在字形序列中的给定位置评估 SequenceContextFormat2 子表时，客户端在 Coverage 表中搜索当前字形。如果找到，客户端然后在类定义表中搜索分配给当前字形的类值。类值用作指向 ClassSequenceRuleSet 表数组的偏移的索引。检索该类值的 ClassSequenceRuleSet 表，并检查该集的 ClassSequenceRule 表，以查看当前序列是否与任何指定模式匹配。
// 注意：ClassSequenceRuleSet 和 SequenceRuleSet 表的格式本质上相同，ClassSequenceRule 和 SequenceRule 表的格式也相同，但语义不同：ClassSequenceRule 表具有字形类值序列，而 SequenceRule 表具有字形 ID 序列；因此它们被区分。相应地，ClassSequenceRuleSet 和 SequenceRuleSet 表由每个引用的子表区分。
// SequenceContextFormat2 表
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 2。
// Offset16	coverageOffset	从 SequenceContextFormat2 表开头到 Coverage 表的偏移。
// Offset16	classDefOffset	从 SequenceContextFormat2 表开头到 ClassDef 表的偏移。
// uint16	classSeqRuleSetCount	ClassSequenceRuleSet 表的数量。
// Offset16	classSeqRuleSetOffsets[classSeqRuleSetCount]	从 SequenceContextFormat2 表开头到 ClassSequenceRuleSet 表的偏移数组（可能为 NULL）。
// 类定义表中定义的每个类有一个指向 ClassSequenceRuleSet 子表的偏移。偏移按类值顺序列出。如果没有定义以特定类开头的模式，则该类值的偏移可以设置为 NULL。
// ClassSequenceRuleSet 表
// 类型	名称	描述
// uint16	classSeqRuleCount	ClassSequenceRule 表的数量。
// Offset16	classSeqRuleOffsets[classSeqRuleCount]	从 ClassSequenceRuleSet 表开头到 ClassSequenceRule 表的偏移数组。
// ClassSequenceRule 子表的偏移按所需结果排序。子表按偏移列出的顺序评估，并使用第一个与当前字形序列匹配的类序列规则。较长、更具体的序列的规则通常排序在较短规则之前。
// ClassSequenceRule 表
// 类型	名称	描述
// uint16	glyphCount	要匹配的字形数。
// uint16	seqLookupCount	SequenceLookup 记录的数量。
// uint16	inputSequence[glyphCount - 1]	要与输入字形序列匹配的类序列，从第二个字形位置开始。
// SequenceLookup	seqLookupRecords[seqLookupCount]	SequenceLookup 记录数组。
// glyphCount 值是输入序列模式中的字形类总数，包括第一个序列位置。inputSequence 数组按顺序指定输入序列模式中的剩余类值。
// seqLookupRecords 数组列出指定要对输入序列中各个位置的字形执行的操作的序列查找记录。这些不必按序列位置顺序排列；它们按所需结果排序。所有序列查找记录按顺序处理，每个应用于前一条记录指示的操作结果。
// 序列上下文格式 3：基于覆盖的字形上下文
// GSUB 类型 5 格式 3 子表和 GPOS 类型 7 格式 3 子表使用 Coverage 表定义的字体集定义输入序列模式。
// SequenceContextFormat3 表恰好指定一个输入序列模式。它有一个指向 Coverage 表的偏移数组。这些按顺序对应于输入序列模式中的位置。
// SequenceContextFormat3 表
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 3。
// uint16	glyphCount	输入序列中的字形数。
// uint16	seqLookupCount	SequenceLookup 记录的数量。
// Offset16	coverageOffsets[glyphCount]	从 SequenceContextFormat3 子表开头到 Coverage 表的偏移数组。
// SequenceLookup	seqLookupRecords[seqLookupCount]	SequenceLookup 记录数组。
// seqLookupRecords 数组列出指定要对输入序列中各个位置的字形执行的操作的序列查找记录。这些不必按序列位置顺序排列；它们按所需结果排序。所有序列查找记录按顺序处理，每个应用于前一条记录指示的操作结果。
// 链接序列上下文格式 1：简单字形上下文
// GSUB 类型 6 格式 1 和 GPOS 类型 8 格式 1 子表以特定字形 ID 定义输入序列以及链接的回溯和前瞻序列；这是 ChainedSequenceContextFormat1 表。其子表格式类似于 SequenceContextFormat1 表使用的格式，关键区别在于 ChainedSequenceRule 表包含链接的回溯和前瞻序列。
// 输入序列的第一个字形在 Coverage 表中指定。每个输入序列中的剩余字形以及回溯和前瞻序列在 ChainedSequenceRule 表中定义——每个组合的回溯 + 输入 + 前瞻序列一个。如果多个序列组合在输入序列中具有相同的初始字形，则该字形 ID 必须在 Coverage 表中列出一次，并且相应的规则使用 ChainedSequenceRuleSet 表聚合——Coverage 表中指定的每个输入序列初始字形一个。
// 在字形序列中的给定位置评估 ChainedSequenceContextFormat1 子表时，客户端在 Coverage 表中搜索当前字形。如果找到，则检索相应的 ChainedSequenceRuleSet 表，并检查该集的 ChainedSequenceRule 表，以查看当前字形序列是否与任何链接序列规则中的任何模式匹配。使用第一个匹配的子表规则。
// 序列规则与当前字形序列的匹配需要输入、回溯和前瞻序列的匹配。查找标志影响回溯和前瞻序列以及输入序列中的匹配。
// 注意，回溯序列以反向逻辑顺序给出：如果当前字形位于文本字形缓冲区中的位置 i，则回溯序列从 i-1 开始，并随着远离 i 而递减偏移值。前瞻序列在输入序列之后开始，并按逻辑顺序递增。
// 为了澄清输入、回溯和前瞻序列的字形数组顺序，提供以下说明。假设在逻辑排序的字形序列中，输入序列匹配从索引 i 开始，长度为 2。
// 逻辑顺序：	a	b	c	d	m	n	w	x	y	z
// 序列索引：			…	i - 1	i	i + 1	i + 2	…
// 输入序列索引：					0	1
// 回溯序列索引：	3	2	1	0
// 前瞻序列索引：							0	1	2	3
// 因此，在此示例中，输入序列将包含字形 ID "mn"；回溯序列将按该顺序包含字形 ID "dcba"；前瞻序列将包含字形 ID "wxyz"。序列查找记录中指定的操作只能为输入序列中的字形指定，即 "mn" 的字形。
// ChainedSequenceContextFormat1 表
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 1。
// Offset16	coverageOffset	从 ChainSequenceContextFormat1 表开头到 Coverage 表的偏移。
// uint16	chainedSeqRuleSetCount	ChainedSequenceRuleSet 表的数量。
// Offset16	chainedSeqRuleSetOffsets[chainedSeqRuleSetCount]	从 ChainedSequenceContextFormat1 表开头到 ChainedSeqRuleSet 表的偏移数组（可能为 NULL）。
// Coverage 表列出所有支持的输入字形序列的初始字形。chainedSeqRuleSetCount 应与 Coverage 表中的字形数量匹配。如果不同，则忽略额外的覆盖字形或额外的序列规则集。
// 每个覆盖字形有一个 ChainedSequenceRuleSet 表。chainedSeqRuleSetOffsets 数组中的偏移必须排序以匹配 Coverage 表中的字形顺序。
// ChainedSequenceRuleSet 表
// 类型	名称	描述
// uint16	chainedSeqRuleCount	ChainedSequenceRule 表的数量。
// Offset16	chainedSeqRuleOffsets[chainedSeqRuleCount]	从 ChainedSequenceRuleSet 表开头到 ChainedSequenceRule 表的偏移数组。
// ChainedSequenceRule 子表的偏移按所需结果排序。子表按偏移列出的顺序评估，并使用第一个与当前字形序列匹配的链接序列规则。较长、更具体的序列的规则通常排序在较短规则之前。
// ChainedSequenceRule 表
// 类型	名称	描述
// uint16	backtrackGlyphCount	回溯序列中的字形数。
// uint16	backtrackSequence[backtrackGlyphCount]	回溯字形 ID 数组。
// uint16	inputGlyphCount	输入序列中的字形数。
// uint16	inputSequence[inputGlyphCount - 1]	输入字形 ID 数组——从第二个字形开始。
// uint16	lookaheadGlyphCount	前瞻序列中的字形数。
// uint16	lookaheadSequence[lookaheadGlyphCount]	前瞻字形 ID 数组。
// uint16	seqLookupCount	SequenceLookup 记录的数量。
// SequenceLookup	seqLookupRecords[seqLookupCount]	SequenceLookup 记录数组。
// inputGlyphCount 值是输入序列中的字形总数，包括第一个字形。inputSequence 数组按顺序指定输入序列中的剩余字形。
// seqLookupRecords 数组列出指定要对输入序列中各个位置的字形执行的操作的序列查找记录。这些不必按序列位置顺序排列；它们按所需结果排序。所有序列查找记录按顺序处理，每个应用于前一条记录指示的操作结果。
// 链接序列上下文格式 2：基于类的字形上下文
// GSUB 类型 6 格式 2 和 GPOS 类型 8 格式 2 子表使用类定义表定义的字形类定义输入序列模式，以及链接回溯和前瞻序列模式。使用三个单独的类定义表来定义输入、回溯和前瞻序列模式的类。可以定义多个组合序列模式，序列模式中每个位置的字形使用相应类定义表的类值指定。
// 每个组合模式在 ChainedClassSequenceRule 表中指定。输入序列第一个位置使用相同类值的组合模式使用 ChainedClassSequenceRuleSet 表聚合。
// ChainedSequenceContextFormat2 表具有指向 Coverage 表的偏移。Coverage 表包含可能作为任何基于类输入序列模式第一个字形出现的所有字形 ID。Coverage 表中的字形 ID 只给出一次。
// 在字形序列中的给定位置评估 ChainedSequenceContextFormat2 子表时，客户端在 Coverage 表中搜索当前字形。如果找到，客户端然后在类定义表中搜索分配给当前字形的类值。类值用作指向 ChainedClassSequenceRuleSet 表数组的偏移的索引。检索该类值的 ChainedClassSequenceRuleSet 表，并检查该集的 ChainedClassSequenceRule 表，以查看当前字形序列是否与任何指定模式匹配。
// 序列规则与当前字形序列的匹配需要输入、回溯和前瞻序列的匹配。注意，查找标志影响回溯和前瞻序列以及输入序列中的匹配。回溯序列模式以反向逻辑顺序指定。回溯、输入和前瞻序列的指定与上面链接序列上下文格式 1 描述的相同，只是序列使用类值而不是字形 ID 指定。
// ChainedSequenceContextFormat2 表
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 2。
// Offset16	coverageOffset	从 ChainedSequenceContextFormat2 表开头到 Coverage 表的偏移。
// Offset16	backtrackClassDefOffset	从 ChainedSequenceContextFormat2 表开头到包含回溯序列上下文的 ClassDef 表的偏移。
// Offset16	inputClassDefOffset	从 ChainedSequenceContextFormat2 表开头到包含输入序列上下文的 ClassDef 表的偏移。
// Offset16	lookaheadClassDefOffset	从 ChainedSequenceContextFormat2 表开头到包含前瞻序列上下文的 ClassDef 表的偏移。
// uint16	chainedClassSeqRuleSetCount	ChainedClassSequenceRuleSet 表的数量。
// Offset16	chainedClassSeqRuleSetOffsets[chainedClassSeqRuleSetCount]	从 ChainedSequenceContextFormat2 表开头到 ChainedClassSequenceRuleSet 表的偏移数组（可能为 NULL）。
// 输入序列类定义表中定义的每个类有一个指向 ChainedClassSequenceRuleSet 子表的偏移。偏移按类值顺序列出。如果没有定义输入序列以特定类开头的模式，则该类值的偏移可以设置为 NULL。
// ChainedClassSequenceRuleSet 表
// 类型	名称	描述
// uint16	chainedClassSeqRuleCount	ChainedClassSequenceRule 表的数量。
// Offset16	chainedClassSeqRuleOffsets[chainedClassSeqRuleCount]	从 ChainedClassSequenceRuleSet 开头到 ChainedClassSequenceRule 表的偏移数组。
// ChainedClassSequenceRule 子表的偏移按所需结果排序。子表按偏移列出的顺序评估，并使用第一个与当前字形序列匹配的链接类序列规则。较长、更具体的序列的规则通常排序在较短规则之前。
// ChainedClassSequenceRule 表
// 类型	名称	描述
// uint16	backtrackGlyphCount	回溯序列中的字形数。
// uint16	backtrackSequence[backtrackGlyphCount]	回溯序列类数组。
// uint16	inputGlyphCount	输入序列中的字形总数。
// uint16	inputSequence[inputGlyphCount - 1]	输入序列类数组，从第二个字形位置开始。
// uint16	lookaheadGlyphCount	前瞻序列中的字形数。
// uint16	lookaheadSequence[lookaheadGlyphCount]	前瞻序列类数组。
// uint16	seqLookupCount	SequenceLookup 记录的数量。
// SequenceLookup	seqLookupRecords[seqLookupCount]	SequenceLookup 记录数组。
// inputGlyphCount 值是输入序列模式中的字形类总数，包括第一个序列位置。inputSequence 数组按顺序指定输入序列模式中的剩余类值。
// seqLookupRecords 数组列出指定要对输入序列中各个位置的字形执行的操作的序列查找记录。这些不必按序列位置顺序排列；它们按所需结果排序。所有序列查找记录按顺序处理，每个应用于前一条记录指示的操作结果。
// 链接序列上下文格式 3：基于覆盖的字形上下文
// GSUB 类型 6 格式 3 子表和 GPOS 类型 8 格式 3 子表使用 Coverage 表定义的字体集定义输入序列模式，以及链接回溯和前瞻序列模式。
// ChainedSequenceContextFormat3 表恰好指定一个输入序列模式。它有三组指向 Coverage 表的偏移：一组用于输入序列模式，一组用于回溯序列模式，一组用于前瞻序列模式。对于每组，偏移按顺序对应于序列模式中的位置。
// ChainedSequenceContextFormat3 表
// 类型	名称	描述
// uint16	format	格式标识符——格式 = 3。
// uint16	backtrackGlyphCount	回溯序列中的字形数。
// Offset16	backtrackCoverageOffsets[backtrackGlyphCount]	回溯序列的 Coverage 表偏移数组。
// uint16	inputGlyphCount	输入序列中的字形数。
// Offset16	inputCoverageOffsets[inputGlyphCount]	输入序列的 Coverage 表偏移数组。
// uint16	lookaheadGlyphCount	前瞻序列中的字形数。
// Offset16	lookaheadCoverageOffsets[lookaheadGlyphCount]	前瞻序列的 Coverage 表偏移数组。
// uint16	seqLookupCount	SequenceLookup 记录的数量。
// SequenceLookup	seqLookupRecords[seqLookupCount]	SequenceLookup 记录数组。
// 所有偏移都是从 ChainedSequenceContextFormat3 表的开头开始。
// seqLookupRecords 数组列出指定要对输入序列中各个位置的字形执行的操作的序列查找记录。这些不必按序列位置顺序排列；它们按所需结果排序。所有序列查找记录按顺序处理，每个应用于前一条记录指示的操作结果。
// Device 和 VariationIndex 表
// Device 表和 VariationIndex 表用于提供 GPOS、JSTF、GDEF 或 BASE 表中字体单位值的调整，例如附着锚点位置的 X 和 Y 坐标。Device 表仅用于非可变字体。VariationIndex 表仅用于可变字体，是 Device 表的变体格式。当值需要调整数据时，包含该值的表还将包含指向 Device 表或 VariationIndex 表的偏移。
// 注意：由于相同的字段用于提供指向 Device 表或指向 VariationIndex 表的偏移，因此对于给定定位值，Device 表和 VariationIndex 表不能同时使用。Device 表应仅在非可变字体中使用；VariationIndex 表只能在可变字体中使用。
// 字体中的字形以字体开发人员指定的设计单位定义。字体缩放会增大或减小字形大小，并将其舍入到最接近的整数像素。然而，精确的字形定位通常需要调整这些缩放和舍入后的值，特别是在小 PPEM 尺寸下。对字形轮廓中的点应用提示是解决此问题的有效方法，但可能需要字体开发人员重新设计或重新提示字形。
// 另一种解决方案，由非可变字体中的 GPOS、BASE、JSTF 和 GDEF 表使用，是使用 Device 表来指定对缩放设计单位的校正。Device 表将校正值应用于由 StartSize 和 EndSize 标识的尺寸范围，这些尺寸指定需要调整的最小和最大每像素点数（ppem）。
// 由于 Device 表调整通常非常小（一两个像素），因此可以将校正压缩为每个尺寸的 2、4 或 8 位表示。两位可以表示范围 {-2, -1, 0, 或 1} 中的数字，四位可以表示范围 {-8 到 7} 中的数字，八位可以表示范围 {-128 到 127} 中的数字。
// 在可变字体中，GPOS、JSTF 或 GDEF 数据中的 X 或 Y 字体单位值可能需要针对字体变化空间中的不同变化实例进行调整。此变化数据包含在 GDEF 表中的 ItemVariationStore 表中。类似地，BASE 表中的值可能需要调整，此变化数据包含在 BASE 表本身的 ItemVariationStore 表中。ItemVariationStore 的格式在 OpenType 字体变体通用表格式章节中详细描述。它包含许多增量值，组织成使用增量集索引引用的集。ItemVariationStore 外部的数据为每个需要变化的目标项提供增量集索引。在 GPOS、JSTF、GDEF 和 BASE 表中，增量集索引存储在 VariationIndex 表中。
// Device 和 VariationIndex 表包含一个 DeltaFormat 字段，用于标识包含数据的格式。格式值 0x0001 到 0x0003 用于 Device 表，并指示直接包含在设备表中的增量调整值的格式：有符号 2、4 或 8 位值。格式值 0x8000 用于 VariationIndex 表，并指示使用增量集索引来引用 ItemVariationStore 表中的增量数据。
// DeltaFormat 值
// 掩码	名称	描述
// 0x0001	LOCAL_2_BIT_DELTAS	有符号 2 位值，每个 uint16 8 个值。
// 0x0002	LOCAL_4_BIT_DELTAS	有符号 4 位值，每个 uint16 4 个值。
// 0x0003	LOCAL_8_BIT_DELTAS	有符号 8 位值，每个 uint16 2 个值。
// 0x8000	VARIATION_INDEX	VariationIndex 表，包含增量集索引对。
// 0x7FFC	Reserved	供将来使用——设置为 0。
// Device 表包含一个 uint16 值数组（deltaValue），以打包表示存储调整增量值。2、4 或 8 位有符号值从最高有效位开始打包到 uint16 值中。例如，使用 DeltaFormat 2（4 位值），等于 {1, 2, 3, -1} 的值数组将由 deltaValue 条目 0x123F 表示。
// 单个 Device 表为一个目标值在尺寸范围内提供增量信息。deltaValue 数组列出在目标范围内每个 ppem 尺寸处要添加或减去的指定 X 或 Y 值的像素数。在数组中，第一个索引位置指定在需要校正的最小 ppem 尺寸处要添加或减去的坐标像素数，第二个索引位置指定在下一个 ppem 尺寸处要添加或减去的坐标像素数，依此类推，对于范围内的每个 ppem 尺寸。
// 为指定范围内的每个 ppem 尺寸表示一个增量值。所需的 uint16 元素数取决于要表示的值数和每个值使用的位大小。如果增量值数乘以每个值使用的位大小不是 16 的倍数，则最后一个 uint16 元素的剩余低位设置为零。
// Device 表
// 类型	名称	描述
// uint16	startSize	要校正的最小尺寸，以 ppem 为单位。
// uint16	endSize	要校正的最大尺寸，以 ppem 为单位。
// uint16	deltaFormat	deltaValue 数组数据的格式：0x0001、0x0002 或 0x0003。
// uint16	deltaValue[ ]	压缩数据数组。
// 本章末尾的示例 9 显示了可用于指定 MATH 表中超脚本最小偏移的 Device 表。
// 在可变字体中，ItemVariationStore 表使用两级组织进行变化数据：存储可以有多个 ItemVariationData 子表，每个子表有多个增量集行。增量集索引是两部分索引：外部索引选择 ItemVariationStore 中的特定项目变化数据子表，内部索引选择该子表中的特定增量集行。VariationIndex 表指定增量集索引的外部部分和内部部分。
// VariationIndex 表
// 类型	名称	描述
// uint16	deltaSetOuterIndex	增量集外部索引——用于选择项目变化存储中的项目变化数据子表。
// uint16	deltaSetInnerIndex	增量集内部索引——用于选择项目变化数据子表中的增量集行。
// uint16	deltaFormat	格式，= 0x8000。
// 注意，VariationIndex 表比 Device 表短，因为它不直接包含增量数据数组。其格式类似于具有空增量数组的 Device 表。当应用程序获取指向 Device 或 VariationIndex 表的偏移时，它们应首先读取前三个字段，然后测试 DeltaFormat 字段以确定前两个字段的解释以及是否有要读取的附加数据。
// 功能变化
// FeatureVariations 表
// 功能变化表描述基于各种条件的功能效果变化。也就是说，它允许在特定条件下将与给定功能关联的默认查找集替换为备用查找集。
// 功能列表提供功能表和关联功能标签的数组，LangSys 表标识给定脚本和语言系统将支持的特定功能表/标签对集。LangSys 表中指定的功能表在当前条件与功能变化表中定义的任何变化条件不匹配时默认使用。这些默认值也将在不支持功能变化表的实现中的所有条件下使用。
// 功能变化表有一个条件记录数组，每个记录引用一组条件（条件集表）和一组在当前运行时上下文匹配条件集时使用的备用功能表。
// 给出的替换是将一个功能表替换为另一个。备用功能表附加在功能变化表的末尾，不包含在功能列表表中。因此，功能列表表中没有对应于备用功能表的功能记录。备用功能表保持与默认功能表相同的功能标签关联。此外，虽然功能列表表中的默认功能表使用 16 位偏移引用，但备用功能表在功能变化表中使用 32 位偏移引用。
// 处理文本时，从 LangSys 表获取默认功能表集，每个功能表都有关联的功能标签，用于给定脚本和语言系统。条件集按顺序评估，测试与当前运行时上下文匹配的条件集。找到第一个匹配时，使用相应的功能表替换表来修改通过 LangSys 表默认获取的功能表集，如下所述（请参阅 FeatureTableSubstitution 表）。
// FeatureVariations 表的格式如下。
// FeatureVariations 表
// 类型	名称	描述
// uint16	majorVersion	FeatureVariations 表的主要版本——设置为 1。
// uint16	minorVersion	FeatureVariations 表的次要版本——设置为 0。
// uint32	featureVariationRecordCount	功能变化记录的数量。
// FeatureVariationRecord	featureVariationRecords[featureVariationRecordCount]	功能变化记录数组。
// 功能变化记录具有指向条件集表和功能表替换表的偏移。
// 如果 ConditionSet 偏移为 0，则没有条件集表。这被视为通用条件：所有上下文都匹配。
// 如果 FeatureTableSubstitution 偏移为 0，则没有功能表替换表，不进行替换。
// 功能变化记录必须按条件集的优先级顺序排列。在处理过程中，将按它们出现的顺序读取功能变化记录，并测试相应的条件集。如果给定记录的条件集与运行时上下文不匹配，则检查下一条记录。对于条件集与运行时上下文匹配的第一个功能变化记录将被视为候选：如果支持 FeatureTableSubstitution 表的版本，则使用此功能变化记录，不再考虑其他功能变化记录。如果不支持 FeatureTableSubtitution 表的版本，则拒绝此功能变化记录，处理将移动到下一条功能变化记录。
// FeatureVariationRecord
// 类型	名称	描述
// Offset32	conditionSetOffset	从 FeatureVariations 表开头到条件集表的偏移。
// Offset32	featureTableSubstitutionOffset	从 FeatureVariations 表开头到功能表替换表的偏移。
// ConditionSet 表
// 条件集表指定应用功能表替换的一组条件。条件集可以指定与各种因素相关的条件；目前，支持一种因素类型：可变字体的变化实例。条件在子表中表示，根据定义条件的因素性质可以使用不同的格式。
// 对于给定条件集，条件是合取关系（布尔 AND）：必须满足所有指定条件才能应用关联的功能表替换。条件集不需要为所有可能的因素指定条件值。如果没有为某些因素指定值，则条件集匹配该因素的所有运行时值。
// 如果给定条件集不包含任何条件，则它匹配所有上下文，并且关联的功能表替换始终应用，除非数组中前面有 ConditionSet 与当前上下文匹配的功能变化记录。
// ConditionSet 表
// 类型	名称	描述
// uint16	conditionCount	此条件集的条件数。
// Offset32	conditionOffsets[conditionCount]	从 ConditionSet 表开头到条件表的偏移数组。
// Condition 表
// 条件表描述特定条件。可以为条件表定义不同的格式，每种格式用于特定类型的条件限定符。目前，定义了一种格式：ConditionTableFormat1，用于指定可变字体中变化轴值的值范围。
// 将来可能会添加其他条件限定符的条件表格式。如果布局引擎遇到具有无法识别格式的条件表，它应无法匹配条件集，但继续测试其他条件集。这样，新条件格式可以在现有实现中以向后兼容的方式定义和使用。
// 条件表格式 1：字体变化轴范围
// 字体变化轴范围条件指可变字体中设计变化轴的值范围。变化轴在字体的字体变化（'fvar'）表中指定。如果使用格式 1 条件表，字体中必须有 'fvar' 表，并且 axisIndex 值（从零开始）必须小于 'fvar' 表中的 axisCount 值。如果 axisIndex 无效，则包含此条件表的功能变化记录被忽略。
// 格式 1 条件表指定沿单个轴的变化实例值的匹配范围。缺少给定变化轴的格式 1 条件意味着该轴不是确定条件集适用性的因素。
// 'fvar' 表定义每个变化轴的有效值范围。处理特定变化实例时，应用归一化过程，将 'fvar' 表中定义范围内的用户值映射到 -1 到 1 范围的归一化尺度。格式 1 条件表中指定的值以归一化尺度表示，因此可以是 -1 到 1 之间的任何值。
// 如果当前选择的变化实例对给定轴的值大于或等于 filterRangeMinValue，且小于或等于 filterRangeMaxValue，则满足字体变化轴范围条件。
// ConditionFormat1 表
// 类型	名称	描述
// uint16	format	格式，= 1
// uint16	axisIndex	'fvar' 表中变化轴的索引（从零开始）。
// F2DOT14	filterRangeMinValue	满足此条件的字体变化实例的最小值。
// F2DOT14	filterRangeMaxValue	满足此条件的字体变化实例的最大值。
// FeatureTableSubstitution 表
// 功能表替换表描述在当前运行时上下文匹配相应条件集时要应用的一组功能表替换。这些替换使用功能表替换记录数组表示。每条记录给出从一个功能表到另一个的简单替换。检查特定功能索引时，匹配具有该索引的第一条记录，如果遇到具有更高索引值的记录，则搜索结束。
// 注意，记录必须按 FeatureIndex 值的递增顺序排列，并且两条记录不能具有相同的 FeatureIndex 值。
// FeatureTableSubstitution 表
// 类型	名称	描述
// uint16	majorVersion	功能表替换表的主要版本——设置为 1
// uint16	minorVersion	功能表替换表的次要版本——设置为 0。
// uint16	substitutionCount	功能表替换记录的数量。
// FeatureTableSubstitutionRecord	substitutions[substitutionCount]	功能表替换记录数组。
// FeatureTableSubstitutionRecord
// 类型	名称	描述
// uint16	featureIndex	要匹配的功能表索引。
// Offset32	alternateFeatureOffset	从 FeatureTableSubstitution 表开头到备用功能表的偏移。
// 如上所述，评估条件集并可能选择处理关联的功能表替换表，以替换从 LangSys 表获取的默认功能表与备用功能表。给定从 LangSys 表获取的选定功能的默认功能表数组，可以如下进行备用功能表的替换：
// 对于每个功能索引，按顺序评估 FeatureTableSubstitutionRecords。
// 如果遇到匹配的记录（FeatureIndex = 当前功能索引），则使用该记录中偏移给出的备用功能表替换该功能索引的功能表。停止处理该功能索引。
// 如果遇到具有更高功能索引值的记录，则停止搜索该功能索引；不进行替换。
// 通用表示例
// 本章的其余部分描述并说明了所有通用表格式的示例。所有示例反映唯一参数，但示例为构建特定于其他情况的表提供了有用的参考。
// 示例有三列，显示十六进制数据、来源和注释。
// 示例 1：ScriptList 表和 ScriptRecord
// 示例 1 说明了使用多种脚本的日文字体的 ScriptList 表和 ScriptRecord 定义：汉字表意文字、假名和拉丁文。每个脚本都有脚本特定行为。
// 示例 1
// 十六进制数据	来源	注释
// ScriptList
// TheScriptList	ScriptList 表定义
// 0003	3	scriptCount
// scriptRecords[0]	按脚本标签字母顺序排列。
// 68616E69	'hani'	scriptTag，汉字表意文字脚本
// 0014	HanIScriptTable	到 Script 表的偏移
// scriptRecords[1]	按脚本标签字母顺序排列。
// 6B616E61	'kana'	scriptTag，平假名和片假名脚本
// 0018	KanaScriptTable	到 Script 表的偏移
// scriptRecords[2]	按脚本标签字母顺序排列。
// 6C61746E	'latn'	scriptTag，拉丁文脚本
// 001C	LatinScriptTable	到 Script 表的偏移
// 示例 2：Script 表、LangSysRecord 和 LangSys 表
// 示例 2 说明了用于阿拉伯语脚本和乌尔都语语言系统的 Script 表、LangSysRecord 和 LangSys 表定义。默认 LangSys 表定义三个默认阿拉伯语脚本功能，用于将单词中的某些字形替换为其适当的词首、词中和词尾字形形式。这些上下文替换是不变的，并且发生在使用阿拉伯语脚本的所有语言系统中。
// 阿拉伯语脚本中的许多替代字形具有语言特定用途。例如，阿拉伯语、波斯语和乌尔都语语言系统使用不同的数字字形。为了保持字符集兼容性，Unicode 标准包括阿拉伯语和波斯语数字字形的单独字符代码。然而，标准对波斯语和乌尔都语数字使用相同的字符代码，即使三个乌尔都语字形（4、6 和 7）与波斯语字形不同。要访问和显示乌尔都语数字的适当字形，文本处理客户端的用户必须输入波斯语数字的字符代码。然后文本处理客户端使用必需的 OpenType 布局字形替换功能，在乌尔都语 LangSys 表中定义，以访问 4、6 和 7 数字的正确乌尔都语字形。
// 注意，乌尔都语 LangSys 表重复默认脚本功能。这种重复是必要的，因为乌尔都语语言系统也使用单词中词首、词中和词尾位置的替代字形。
// 示例 2
// 十六进制数据	来源	注释
// Script
// ArabicScriptTable	Script 表定义
// 000A	DefLangSys	到默认 LangSys 表的偏移
// 0001	1	langSysCount
// langSysRecords[0]	按 LangSys 标签字母顺序排列。
// 55524420	"URD "	langSysTag，乌尔都语语言
// 0016	UrduLangSys	到乌尔都语 LangSys 表的偏移
// LangSys
// DefLangSys	默认 LangSys 表定义
// 0000	NULL	lookupOrderOffset，保留，空
// FFFF	0xFFFF	requiredFeatureIndex，无必需功能
// 0003	3	featureIndexCount
// 0000	0	featureIndices[0]，顺序任意
// 'init' 功能（词首字形）
// 0001	1	featureIndices[1]，'fina' 功能（词尾字形）
// 0002	2	featureIndices[2]，'medi' 功能（词中字形）
// LangSys
// UrduLangSys	LangSys 表定义
// 0000	NULL	lookupOrderOffset，保留，空
// 0003	3	requiredFeatureIndex，乌尔都语数字替换
// 0003	3	featureIndexCount
// 0000	0	featureIndices[0]，顺序任意
// 'init' 功能（词首字形）
// 0001	1	featureIndices[1]，'fina' 功能（词尾字形）
// 0002	2	featureIndices[2]，'medi' 功能（词中字形）
// 示例 3：FeatureList 表和 Feature 表
// 示例 3 显示了用于拉丁文脚本中连字的 FeatureList 和 Feature 表定义。FeatureList 有三个功能，都是可选的，命名为 'liga'。一个功能（也是默认功能）在没有语言特定功能指定其他连字的情况下实现拉丁文中的连字。另外两个功能分别在土耳其语和德语中实现连字。
// 三个查找定义此字体中渲染连字的字形替换。第一个查找产生"ffi"和"fi"连字；第二个产生"ffl"、"fl"和"ff"连字；第三个产生 eszet 连字。
// 以"f"开头的连字分为两组，因为土耳其语有一个无点"i"字形，因此不使用"ffi"和"fi"连字。然而，土耳其语确实使用"ffl"、"fl"和"ff"连字，TurkishLigatures 功能表列出了这一个查找。
// 只有德语语言系统使用 eszet 连字，因此 GermanLigatures 功能表包含一个用于渲染该连字的查找。
// 由于拉丁文脚本可以使用两组连字，DefaultLigatures 功能表定义两个 LookupList 索引：一个用于"ffi"和"fi"连字，一个用于"ffl"、"fl"和"ff"连字。如果文本处理客户端选择此功能，则字体应用两个查找。
// 注意，TurkishLigatures 和 DefaultLigatures 功能表都列出了"ffl"、"fl"和"ff"连字查找的 LookupListIndex 为一（1）。这是因为语言特定查找覆盖所有默认语言系统查找，并且语言系统功能表必须明确列出适用于该语言的所有查找。
// 示例 3
// 十六进制数据	来源	注释
// FeatureList
// TheFeatureList	FeatureList 表定义
// 0003	3	featureCount
// featureRecords[0]
// 6C696761	'liga'	featureTag
// 0014	TurkishLigatures	到 Feature 表的偏移，FflFfFlLiga
// featureRecords[1]
// 6C696761	'liga'	featureTag
// 001A	DefaultLigatures	到 Feature 表的偏移，FfiFiLiga、FflFfFlLiga
// featureRecords[2]
// 6C696761	'liga'	featureTag
// 0022	GermanLigatures	到 Feature 表的偏移，EszetLiga
// Feature
// TurkishLigatures	Feature 表定义
// 0000	NULL	featureParamsOffset，空
// 0001	1	lookupIndexCount
// 0001	1	lookupListIndices[1]，ffl、fl、ff 连字替换查找
// Feature
// DefaultLigatures	Feature 表定义
// 0000	NULL	featureParamsOffset，空
// 0002	2	lookupIndexCount
// 0000	0	lookupListIndices[0]，顺序任意，ffi、fi 连字
// 0001	1	lookupListIndices[1]，ffl、fl、ff 连字替换查找
// Feature
// GermanLigatures	Feature 表定义
// 0000	NULL	featureParamsOffset，空
// 0003	3	lookupIndexCount
// 0000	0	lookupListIndices[0]，顺序任意，ffi、fi 连字
// 0001	1	lookupListIndices[1]，ffl、fl、ff 连字替换查找
// 0002	2	lookupListIndices[2]，eszet 连字替换查找
// 示例 4：LookupList 表和 Lookup 表
// 示例 3 的延续，示例 4 显示了 LookupList 表中的三个连字查找。第一个生成"ffi"和"fi"连字；第二个产生"ffl"、"fl"和"ff"连字；第三个生成 eszet 连字。每个查找表定义一个指向包含连字替换数据的子表的偏移。
// 示例 4
// 十六进制数据	来源	注释
// LookupList
// TheLookupList	LookupList 表定义
// 0003	3	lookupCount
// 0008	FfiFiLookup	到 lookups[0] 表的偏移，按设计顺序
// 0010	FflFlFfLookup	到 lookups[1] 表的偏移
// 0018	EszetLookup	到 lookups[2] 表的偏移
// Lookup
// FfiFiLookup	lookups[0] 表定义
// 0004	4	lookupType：连字替换
// 000C	0x000C	lookupFlag：IGNORE_LIGATURES、IGNORE_MARKS
// 0001	1	subTableCount
// 0018	FfiFiSubtable	到 FfiFi 连字替换子表的偏移
// Lookup
// FflFlFfLookup	lookups[1] 表定义
// 0004	4	lookupType：连字替换
// 000C	0x000C	lookupFlag：IGNORE_LIGATURES、IGNORE_MARKS
// 0001	1	subTableCount
// 0028	FflFlFfSubtable	到 FflFlFf 连字替换子表的偏移
// Lookup
// EszetLookup	lookups[2] 表定义
// 0004	4	lookupType：连字替换
// 000C	0x000C	lookupFlag：IGNORE_LIGATURES、IGNORE_MARKS
// 0001	1	subTableCount
// 0038	EszetSubtable	到 Eszet 连字替换子表的偏移
// 示例 5：CoverageFormat1 表（字形 ID 列表）
// 示例 5 说明了一个列出字体中所有小写降部字形 ID 的 Coverage 表。该表使用列表格式而不是范围格式，因为降部字形的字形 ID 不是连续排序的。
// 示例 5
// 十六进制数据	来源	注释
// CoverageFormat1
// DescenderCoverage	Coverage 表定义
// 0001	1	格式：字形 ID 列表
// 0005	5	glyphCount
// 0038	gGlyphID	glyphArray[0]，按字形 ID 顺序
// 003B	jGlyphID	glyphArray[1]
// 0041	pGlyphID	glyphArray[2]
// 0042	qGlyphID	glyphArray[3]
// 004A	yGlyphID	glyphArray[4]
// 示例 6：CoverageFormat2 表（字形 ID 范围）
// 示例 6 显示了一个定义十个数字字形（0 到 9）的 Coverage 表。该表使用范围格式而不是列表格式，因为字形 ID 在字体中连续排序。StartCoverageIndex 为零（0）表示第一个字形 ID（零字形）返回 Coverage 索引 0。第二个字形 ID（数字一（1）字形）返回 Coverage 索引 1，依此类推。
// 示例 6
// 十六进制数据	来源	注释
// CoverageFormat2
// NumeralCoverage	Coverage 表定义
// 0002	2	格式：字形 ID 范围
// 0001	1	rangeCount
// rangeRecords[0]
// 004E	0glyphID	startGlyphID
// 0057	9glyphID	endGlyphID
// 0000	0	StartCoverageIndex，第一个 CoverageIndex = 0
// 示例 7：ClassDefFormat1 表（类数组）
// 示例 7 中的 ClassDef 表为字体中的小写字形分配类值。x 高度字形在类 0 中，升部字形在类 1 中，降部字形在类 2 中。数组以小写"a"字形的索引开头。
// 示例 7
// 十六进制数据	来源	注释
// ClassDefFormat1
// LowercaseClassDef	ClassDef 表定义
// 0001	1	格式：类数组
// 0032	aGlyphID	startGlyph
// 001A	26	glyphCount
// classValueArray
// 0000	0	aGlyph，Xheight 类 0
// 0001	1	bGlyph，Ascender 类 1
// 0000	0	cGlyph，Xheight 类 0
// 0001	1	dGlyph，Ascender 类 1
// 0000	0	eGlyph，Xheight 类 0
// 0001	1	fGlyph，Ascender 类 1
// 0002	2	gGlyph，Descender 类 2
// 0001	1	hGlyph，Ascender 类 1
// 0000	0	iGlyph，Ascender 类 1
// 0002	2	jGlyph，Descender 类 2
// 0001	1	kGlyph，Ascender 类 1
// 0001	1	lGlyph，Ascender 类 1
// 0000	0	mGlyph，Xheight 类 0
// 0000	0	nGlyph，Xheight 类 0
// 0000	0	oGlyph，Xheight 类 0
// 0002	2	pGlyph，Descender 类 2
// 0002	2	qGlyph，Descender 类 2
// 0000	0	rGlyph，Xheight 类 0
// 0000	0	sGlyph，Xheight 类 0
// 0001	1	tGlyph，Ascender 类 1
// 0000	0	uGlyph，Xheight 类 0
// 0000	0	vGlyph，Xheight 类 0
// 0000	0	wGlyph，Xheight 类 0
// 0000	0	xGlyph，Xheight 类 0
// 0002	2	yGlyph，Descender 类 2
// 0000	0	zGlyph，Xheight 类 0
// 示例 8：ClassDefFormat2 表（类范围）
// 在示例 8 中，ClassDef 表为阿拉伯语脚本中的四种类字形分配类值：中等高度基础字形、高基础字形、非常高基础字形和默认标记字形。该表仅列出类 1、类 2 和类 3；所有未明确分配类的字形归入类 0。
// 该表使用范围格式，因为每个类中的字形 ID 在字体中连续排序。在 ClassRange 数组中，ClassRange 定义按每个范围中的起始字形索引排序。高基础字形的索引在字体中排在第一位，类值为 2，定义在 ClassRange[0] 中。ClassRange[1] 定义所有非常高基础字形并分配类值 3。ClassRange[2] 包含所有默认标记字形；类值为 1。类 0 由所有中等高度基础字形组成，这些字形未明确分配类值。
// 示例 8
// 十六进制数据	来源	注释
// ClassDefFormat2
// GlyphHeightClassDef	Class 表定义
// 0002	2	格式：范围
// 0003	3	classRangeCount
// classRangeRecords[0]	按 startGlyphID 排序
// 0030	tahGlyphID	startGlyphID——范围中的第一个字形 ID
// 0031	dhahGlyphID	endGlyphID——范围中的最后一个字形 ID
// 0002	2	类：高基础字形
// classRangeRecords[1]
// 0040	cafGlyphID	startGlyphID
// 0041	gafGlyphID	endGlyphID
// 0003	3	类：非常高基础字形
// classRangeRecords[2]
// 00D2	fathatanDefaultGlyphID	startGlyphID
// 00D3	dammatanDefaultGlyphID	endGlyphID
// 0001	1	类：默认标记
// 示例 9：Device 表
// 示例 9 定义了数学脚本的最小范围值，使用 Device 表根据输出字体大小调整值。此处，Device 表定义了从 11 ppem 到 15 ppem 字体大小的单像素调整。DeltaFormat 为 1，表示有符号 2 位值的打包数组，每个 uint16 八个值。
// 示例 9
// 十六进制数据	来源	注释
// DeviceTableFormat1
// MinCoordDeviceTable	Device 表定义
// 000B	11	startSize：11 ppem
// 000F	15	endSize：15 ppem
// 0001	1	deltaFormat：有符号 2 位值（每个 uint16 8 个值）
// 1	将 11ppem 增加 1 像素
// 1	将 12ppem 增加 1 像素
// 1	将 13ppem 增加 1 像素
// 1	将 14ppem 增加 1 像素
// 5540	1	将 15ppem 增加 1 像素

// OpenType 字体变体概述
// 本 OpenType 规范章节提供 OpenType 字体变体的概述，包括基本概念介绍、术语表以及关键算法的规范：坐标归一化和实例值插值。
// 介绍
// OpenType 字体变体允许字体设计师将字体家族中的多个字体面合并到单个字体资源中。可变字体——使用 OpenType 字体变体机制的字体——为内容作者和设计师提供了极大的灵活性，同时也允许以高效格式表示字体数据。
// 可变字体允许沿某个给定设计轴（如字重）进行连续变化：
// 沿字重轴变化的大写 G 变体
// 沿设计轴连续变化
// 从概念上讲，可变字体定义了一个或多个设计特征可以变化的变化轴。字重是一种可能的变化轴，但许多不同类型的变化都是可能的。可变字体可以组合两个或多个不同的变化轴。例如，下图说明了字重和宽度变化的组合：
// 具有字重和宽度轴的设计空间中的大写 G 变体
// 沿多个设计轴连续变化
// 通常，可变字体会变化字形轮廓的设计。然而，一般来说，视觉外观的任何方面都可能变化。例如，字体可以变化行高度量或彩色字形中的渐变外观，而不是（或除了）字形轮廓。
// 可变字体包含一个表，即字体变体（'fvar'）表，描述该字体使用的变化轴。该表确定可变字体及其变化参数如何呈现给用户和应用程序。每个轴使用 Fixed（16.16）数据类型表示的分数值定义一个数值范围。从概念上讲，这提供了连续的变化梯度，允许选择大量的设计变化实例。每个实例将由设计变化空间内的坐标数组指定——每个设计轴上的特定值。因此，例如，如果用户或应用程序需要对宽度进行微小调整或需要稍微更明显的衬线，则可以精细控制此类变化轴。
// 每个轴允许连续的实例选择值范围，并且通常对于给定轴会有连续的外观变化。然而，在某些情况下，随着轴设置的更改，外观可能以离散步骤变化。例如，轴可以触发对轴值离散子范围的不同替代字形的替换。
// 字体设计师可以预定义具有特定名称的某些实例。例如，字体可以在字重轴上具有连续变化，但设计师可以将特定变化实例标识为"Light"或"Semibold"。命名实例可用于支持的设计变化空间中的任何实例。例如，在具有字重和宽度轴的字体中，命名实例可能包括"Light"、"Extended"或"Semibold Condensed"。有关命名实例的详细信息也包含在字体变体表中。
// 字重和宽度是常用的设计变化轴，但可变字体可以使用广泛的其他可能变化轴。有关支持的轴的更多信息，请参阅字体变体（'fvar'）表章节。
// 除了字体变体表之外，可变字体还包含一个样式属性（STAT）表，描述每个变化轴的附加细节以及设计师选择的特定轴值。这些细节包括这些值的描述字符串，如"Bold"、"Extended"或"Semi-sans"。例如，字重/宽度可变字体可能支持"Bold Extended"变化，STAT 表将分别为字重和宽度轴上的特定值提供"Bold"和"Extended"字符串。这些字符串可用于创建字体选择器用户界面。它们还可用于将多轴字体家族的成员投影到假设有限数量子家族变化轴的不同模型中，如字重/宽度/倾斜模型。（有关更多信息，请参阅 STAT 表章节。）由于 STAT 表标识每个轴上的值，软件永远不需要解析子家族字符串并猜测诸如"Halbfett"之类的字符串标记是否指某个轴上的特定值。
// 注意：样式属性表使具有许多设计轴的字体能够定义为单个多轴家族，同时仍让实例在所有这些轴上得到支持，这些实例在可能只识别有限变化轴集或有限轴值数量的旧应用程序中。宿主平台必须支持样式属性表，可以将多轴家族中的实例转换为旧应用程序将识别的多个家族中的较少实例。
// 随着选择字体的不同变化实例，字体内的各种数据项可以相应调整。例如，'glyf' 表可以提供给定字形的默认轮廓，但轮廓可以以某种方式调整以反映不同的设计变化。除了字形轮廓之外，还有几个其他数据项也可能需要类似的调整，包括字体范围的度量、CVT 值或字形定位查找表中的锚点位置。可变字体包含必需和可选的表，描述字体中的这些项如何从默认值变化到不同变化实例所需的值。例如，虽然 'glyf' 表可以提供字形的默认轮廓，但字形变体（'gvar'）表将提供描述每个字形轮廓如何针对不同变化实例变化的数据。
// 可变字体具有默认实例，轴参数值设置为 'fvar' 表中为每个轴定义的默认值。字体中的几个表为许多不同的数据项提供默认值——例如 'glyf' 表中的字形轮廓点位置，或 OS/2 表中的字体范围升部距离。字体的默认实例使用此类项的默认值而不进行任何调整，并且不需要变化特定的表。如果从字体中移除或忽略变化特定的表——'fvar'、'gvar'、MVAR 等——剩余数据将构成默认实例的完整字体。
// TrueType 轮廓字体的字体变体机制最初由 Apple 在"TrueType GX"中引入。用于 OpenType 字体变体的一些表已从 Apple 的早期规范中改编，并进行了一些增强和修订。（特别是，'fvar' 表规范在格式和使用的数据值方面都有重大变化，并且不使用 'fmtx' 表。）还创建了其他扩展，以便将变体机制集成到 OpenType 中。实现者可能希望参考 Apple 的规范以获取历史见解，但应将 OpenType 规范作为实现 OpenType 字体变体的参考。
// 术语
// 讨论 OpenType 字体变体时有几个有用的术语，将在本规范中使用。
// OpenType 字体变体：本章描述的技术名称。
// 字体面：共享特定设计参数的字形数据的逻辑集合，以及关联的度量数据和名称或其他元数据。
// 字体资源：包含（至少）构成功能性字体面所需的最小表集的 OpenType 数据。
// 注意：在 OpenType 字体文件中，每个表目录及其引用的表构成一个字体资源。格式良好的 .OTF 或 .TTF 文件包含单个字体资源；格式良好的 .OTC 或 .TTC 文件包含一个或多个字体资源。没有变化相关表的字体资源为单个字体面提供数据。包含变化相关表的单个字体资源可以为多个字体面提供数据。
// 字体家族：共享共同家族名称的一组字体资源——名称 ID 16（排版家族名称）或名称 ID 1 的相同字符串值。
// 注意：假设家族中的所有字体将共享某些设计特征，但在其他方面有所不同。不同的设计特征可能使用 OpenType 字体变体机制支持。
// 变化轴：字体面设计中的设计师确定的变量，可用于在家族中导出多个变体设计。
// 可变字体：使用 OpenType 字体变体机制支持家族中多个字体面沿设计师定义的变化轴的字体资源——即通过字体中的变化表和其他表数据。
// 字形设计网格：字体字形轮廓设计的视觉二维空间。
// 设计变化空间：字体设计师设计字体家族时使用的变化轴定义的抽象多维空间。在可变字体的上下文中，变化空间指由字体 'fvar' 表中指定的变化轴定义的 n 维空间。
// 注意：变化空间可以有一个或多个轴。在可变字体中，变化空间由 'fvar' 表中指定的最小值和最大值界定。零原点在设计变化空间中没有特殊意义。然而，在可变字体中，零原点（使用归一化坐标尺度——定义如下）是一个标记位置，因为它对应于字体资源名称、字形和度量表直接表示的字体面，而不参考任何变化表或其他变化数据。
// 变化数据：可变字体中用于描述字体中数据项的值如何从默认值调整到变化空间内不同实例所需的值的数据。
// 变化表：与字体变体特别相关的 OpenType 表，包括：
// 轴变体（'avar'）表
// CVT（控制值表）变体（'cvar'）表
// 字体变体（'fvar'）表
// 字形变体（'gvar'）表
// 水平度量变体（HVAR）表
// 度量变体（MVAR）表
// 垂直度量变体（VVAR）表
// 注意：'fvar' 表描述字体的变化空间，其他变化表提供变化数据以描述不同数据项如何在字体的变化空间中变化。请注意，并非所有这些表都是可变字体必需的。另请注意，某些字体数据项的变化数据可能包含在与字体变体不特别相关的其他表中。此外，某些与字体变体不特别相关的表在可变字体中是必需的。有关更多详细信息，请参阅下面的变化数据表和杂项要求部分。
// 点：为避免歧义，点将仅用于指字形设计网格中的（X，Y）位置。讨论设计变化空间时，将使用位置来指该空间内的位置。
// 变化实例：对应于可变字体变化空间内特定位置的字体面。
// 命名实例：在 'fvar' 表中特别定义并分配名称的变化实例。
// 用户坐标尺度：用于表征给定变化轴的数值尺度，以及应用程序选择可变字体实例时使用的尺度。
// 注意：某些变化轴具有规定的有限范围，以用户尺度表示。使用特定可变字体时，给定轴的用户尺度由 'fvar' 表中最小值和最大值界定，可能是该轴通常有效范围的子范围。
// 归一化坐标尺度：处理可变字体中的变化数据以导出特定实例的值时，应用归一化过程将每个轴上的用户尺度值映射到适用于该字体的归一化尺度，范围从 -1 到 1。
// 注意：'fvar' 表指定每个轴的用户尺度最小值、默认值和最大值。在归一化过程中，这些分别映射到 -1、0 和 1，其他值沿每个轴映射到中间点。其他值的映射由 'avar' 表调节（如果存在）。字体中的所有变化数据都引用归一化尺度值或字体变化空间内的位置。
// 元组 / N 元组：用于指定字体变化空间内位置的有序坐标值集。
// 注意：此处"元组"的使用与计算机科学和数学中的常规用法一致。在 Apple TrueType 规范中，"元组"被用于不同的含义，指与字体设计变化空间特定区域相关联的变化数据集。在 OpenType 规范中，"元组变化数据"用于该含义，并且在许多情况下使用"n 元组"以避免与 Apple 规范中的用法混淆。
// 区域：变化空间的一个子空间（即某些部分或子集），在其上描述了变化调整。
// 注意：区域涉及字体变化空间的所有轴；它不是"子空间"意义上的只涉及轴的子集。在归一化坐标中，区域始终是矩形的：它们具有直边和直角。字体变化空间中可以定义多达 65,535 个区域的变化数据。
// 主控：包含特定字体面完整轮廓数据的源字体数据集，用于字体开发工作流程。
// 注意：某些字体开发工作流程使用多个主控作为创建家族中不同字体面的字体资源的源数据。多个源主控也可用于创建可变字体。每个源主控将对应于变化空间中的单个实例，并且可能对应于可变字体中特定区域的变化数据。然而，每个主控包含完整的轮廓数据，而可变字体只包含一组完整的轮廓数据（在 'glyf' 或 CFF2 表中），由不同区域的变化数据补充，以表示字体支持的完整实例范围。每个源主控可能对应于可变字体中的特定区域的变化数据，但源主控与可变字体内变化数据集之间的关系取决于设计的性质和用于创建可变字体的工具。
// 增量 / 调整增量：变化数据中的数值，指定变化空间内特定区域或特定轴子范围内数据项默认值的最大调整量。
// 增量集：与变化空间特定区域相关联的一组调整增量。
// 标量：应用于增量以导出特定变化实例所需调整值的系数值。
// 插值：为特定变化实例推导某些字体数据项（如字形轮廓点的 X 和 Y 坐标）的调整值的过程。
// 变化空间、默认实例和调整增量
// 可变字体支持一个或多个变化轴。常用变化轴应注册，但也可以使用自定义的、设计师定义的变化轴。每个轴具有不同的标签，用于在 'fvar' 表中标识它。请参阅 'fvar' 表规范以获取有关轴标签的更多详细信息。
// 可变字体的轴规范在 'fvar' 表中给出，以及每个轴的最小值、默认值和最大值。这定义了字体的变化空间。设计师决定每个轴支持的设计变化范围以及设计如何与每个轴的尺度对齐，完全由设计师决定。
// 例如，可变字体可能支持从细到黑的完整字重范围：
// 从细到黑变化的大写 G 变体
// 字体 A：从细到黑的字重变化
// 但设计师也可能选择仅支持有限的字重范围：
// 从常规到黑变化的大写 G 变体
// 字体 B：从常规到黑的字重变化
// 可变字体具有默认实例，对应于 'fvar' 表中为每个轴指定的默认值设置坐标的变化空间中的位置。默认实例使用各种数据项的默认值，这些数据项直接由非变化特定字体表提供，例如 'glyf' 表中字形轮廓点的网格坐标。
// 大写 Q 的轮廓
// 'glyf' 表条目中的默认字形轮廓数据
// 所有其他实例对一个或多个轴具有非默认坐标值。这些其他实例由变化数据支持，这些变化数据为各种字体数据项提供调整增量，以从其默认值产生调整。
// 大写 Q 的轮廓及变化调整后的轮廓
// 非默认实例的默认字形轮廓和调整后的点位置
// 控制点调整的增量细节
// 通常，增量为每个变化轴的极值提供，但也可以为变化空间中的其他位置提供增量。（请参阅下面的更多详细信息。）对于默认值和最小值或最大值极值之间的轴位置，其他值进行插值。
// 字体设计师可以确定哪个设计被视为默认，以及提供哪些增量。例如，具有从细到黑字重变化的字体可能以常规（400）实现为默认，以细（100）和黑（900）作为最小值/最大值。在这种情况下，变化数据将包括细极值的增量以及黑极值的增量。
// 两个轴极值都有增量的字重轴
// 默认靠近轴中点，最小值和最大值极值都有增量
// 但另一种具有从细到黑字重变化的可变字体可能以细为默认值和最小值，黑为最大值。在这种情况下，变化数据可能只包含黑极值的增量。
// 默认在轴最小值处且只有最大值增量的字重轴
// 默认在轴最小值处，只有最大值极值的增量
// 注意，默认选择的一个考虑因素是不支持字体变体的旧应用程序或平台中的期望行为：在这种软件中，只有可变字体的默认实例会得到支持。
// 创建可变字体的常见过程涉及使用多个源主控字体。每个主控提供变化空间内不同位置的完整字形轮廓数据。例如，字体设计师可能为字重轴的细和重极值创建字体。
// 细和黑的源主控
// 字体开发工作流程的细和黑源主控轮廓
// 从这些两个源主控，字体工具可以派生一个可变字体，该字体具有常规字重的完整字形轮廓以及一个或多个非默认字重的增量，包括最小值或最大值字重。
// 具有常规默认轮廓以及细和黑增量的生成字体
// 具有单个轮廓数据加增量的生成可变字体
// 注意，每个源主控字体都具有特定设计变体的完整轮廓数据。相比之下，可变字体只有一个变化实例的完整轮廓，所有其他实例使用默认轮廓加增量派生。每个源主控可能对应于可变字体中的特定区域的变化数据，但源主控与可变字体内变化数据集之间的关系取决于设计的性质和用于创建可变字体的工具。
// 另请注意，使用多个源主控字体派生可变字体的要求是对应的字形轮廓必须是点兼容的：它们必须具有相同数量的轮廓和每个轮廓中相同数量的点。
// 坐标尺度和归一化
// 变化空间内的位置可以表示为 n 元组——有序坐标值列表。示例如下。n 元组的坐标值可以使用用户轴尺度，也可以使用归一化尺度。这些尺度之间的精确关系将描述。
// 用户坐标指使用用户轴尺度表示的坐标值 n 元组。用户尺度指 'fvar' 表中用于描述变化轴的数值尺度。每个变化轴使用自己的数值尺度，适合该变化轴的性质。注册轴标签的尺度作为轴标签注册的一部分定义，但不同字体可能支持轴尺度的不同子范围。这样，给定字体的 'fvar' 表定义了该字体变化空间的特定坐标系，可能与其他字体不同。
// 而 'fvar' 表中的定义以用户坐标表示，可变字体中使用的变化数据格式使用归一化坐标系——归一化坐标——其中 'fvar' 表中为每个轴指定的最小值、默认值和最大值分别映射到 -1、0 和 1。
// 例如，下图说明了具有字重和宽度轴变化的可能字体的变化空间的用户坐标系：
// 使用"用户"坐标的二维坐标空间
// 以下图形说明了同一设计空间中位置的归一化坐标系：
// 使用归一化坐标的二维坐标空间
// 归一化变换使用默认变换，然后是由 'avar' 表定义的变换的二级修改（如果存在）。'avar' 表不影响最小值、默认值和最大值到 -1、0 和 1 的映射；它只能影响中间值的映射。这将在下面更详细地描述。
// 默认归一化映射将每个轴的变化范围分为两个段：最小值到默认值，以及默认值到最大值。最小值、默认值和最大值分别映射到 -1、0 和 1。在每个段内，所有其他值进行线性插值，如下所示：
// 设 userValue 为给定轴的用户选择实例值的用户尺度坐标值，设 defaultNormalizedValue 为默认归一化实例值，设 axisMin 为 'fvar' 表中指定的轴最小值，等等。
// 通过钳制到最小值和最大值来强制用户尺度坐标值在范围内：
// if userValue < axisMin
// userValue = axisMin;
// if userValue > axisMax
// userValue = axisMax;
// 在不同段内线性插值值：
// if (userValue < axisDefault)
// {
// defaultNormalizedValue = -(axisDefault - userValue) / (axisDefault - axisMin);
// }
// else if (userValue > axisDefault)
// {
// defaultNormalizedValue = (userValue - axisDefault) / (axisMax - axisDefault);
// }
// else
// {
// defaultNormalizedValue = 0;
// }
// 如果存在 'avar' 表，则对每个轴执行额外的归一化步骤以计算最终归一化值。在 'avar' 表中，AxisValueMap 记录将轴的默认归一化值映射到修改后的归一化值。连续的 AxisValueMap 记录对定义给定轴范围内的段。在段内，中间值进行线性解释。从上面计算的 defaultNormalizedValue 开始，额外的归一化步骤如下：
// 从 'avar' 表中检索给定轴的 SegmentMaps 记录，使用 'fvar' 表中定义的轴索引。
// 扫描 SegmentMaps.axisValueMaps 数组中的 AxisValueMaps 记录，以找到第一个 AxisValueMaps.fromCoordinate 值大于或等于 defaultNormalizedValue 的记录。将此记录指定为 endSeg。（注意，endSeg 不能是第一个映射记录，即 -1 的记录。）
// 如果 endSeg.fromCoordinate 等于 defaultNormalizedValue，则将 finalNormalizedValue 设置为 endSeg.toCoordinate。返回此值并结束。
// 否则 endSeg.fromCoordinate 严格大于 defaultNormalizedValue）：将前一个 AxisValueMaps 记录指定为 startSeg。
// finalNormalizedValue 计算如下：
// ratio = (defaultNormalizedValue - startSeg.fromCoordinate) /
// (endSeg.fromCoordinate - startSeg.fromCoordinate)
// finalNormalizedValue = startSeg.toCoordinate + ratio *
// (endSeg.toCoordinate - startSeg.toCoordinate)
// 请参阅上面提到的 'avar' 表章节的表格式部分，了解上述结构的详细信息。
// 处理变化实例坐标和变化数据时，使用的精度量和舍入处理方式可能对视觉结果产生明显影响。为了确保给定字体在不同实现之间的一致行为，实现必须遵守以下与精度和舍入相关的要求：
// 归一化的输入必须是 16.16 格式。如果应用程序提供以 float 或 double 数据类型表示的输入值，必须使用以下方法转换为 16.16。
// 上面指定的归一化数学计算以 16.16 进行。
// 执行默认归一化计算后，某些结果可能略超出 [-1, +1] 范围。值必须钳制到此范围：
// if result < -1
// result = -1;
// if result > 1
// result = 1;
// 如果存在 'avar' 表，数学计算以 16.16 进行，结果按上述方式钳制到 [-1, +1] 范围。
// 将最终的归一化 16.16 坐标值转换为 2.14，方法如下：加上 0x00000002，并符号扩展右移 2 位。
// 2.14 结果必须在某些操作中存储和返回，如下所述。
// 对于后续计算——插值标量的计算或缩放增量值的累积——2.14 表示可以转换为 float、16.16 或其他实现特定的表示。建议至少保持 16 位小数精度，并且任何舍入应在值使用前的最后一点进行。
// 从 float 或 double 数据类型转换为 16.16 时，必须使用以下方法：
// 将小数部分乘以 65536，并将结果四舍五入到最接近的整数（对于 0.5 及更高的分数值，取下一个更高的整数；对于其他分数值，截断）。将结果存储在低序字中。
// 将整数部分的二进制补码表示移入高序字中。
// 注意：除了将更高精度表示转换为 16.16 之外，本规范对实例坐标、缩放增量或派生实例值没有其他舍入要求。例如，对于应用增量后的轮廓点坐标，栅格化实现可以使用高精度浮点类型，或根据需要舍入到较低精度表示。不同的实现可以使用不同的精度来计算实例值，从而导致细微的视觉差异。如果字体实例的数据被转换或导出到另一种表示——例如，动态生成给定实例的静态字体——派生的静态字体与源可变字体之间可能存在细微差异。
// 必须严格按照上述步骤 1-5 获得 2.14 表示中的归一化值。在具有 TrueType 指令的字体中，此精确值必须由 GET VARIATION 指令返回。（请参阅 TrueType 指令集。）如果字体在 OpenType 布局表中使用 FeatureVariation 表，则在比较条件表中指定的轴范围值时，必须使用此精确值。
// 'avar' 归一化示例
// 以下示例说明了使用 'avar' 映射的归一化工作原理。
// 假设字体中某个轴的最小值为 100，默认值为 400，最大值为 900。假设所选实例的用户坐标为 250。根据上述算法，默认归一化值计算如下：
// defaultNormalizedValue = -(axisDefault - userValue) / (axisDefault - axisMin)
// = -(400 - 250) / (400 - 100)
// = -150 / 300
// = -0.5
// 假设字体还具有该轴的 'avar' 表，包含以下映射（AxisValueMap 记录）：
// 记录索引	fromCoordinate	toCoordinate
// 0	-1.0	-1.0
// 1	-0.75	-0.5
// 2	0	0
// 3	0.4	0.4
// 4	0.6	0.9
// 5	1.0	1.0
// 给定默认归一化值 -0.5，相关段由记录 1 和记录 2 定义：
// 第一个具有大于或等于 defaultNormalizedValue 的 fromCoordinate 的 AxisValueMaps 记录是记录索引 2。因此，记录 2 是 endSeg。
// endSeg.fromCoordinate 严格大于 defaultNormalizedValue。因此，前一个记录，记录 1，是 startSeg。
// 因此，最终归一化值计算如下：
// ratio = (defaultNormalizedValue - startSeg.fromCoordinate) /
// (endSeg.fromCoordinate - startSeg.fromCoordinate)
// = (-0.5 - (-0.75)) / (0 - (-0.75))
// = 0.3333
// finalNormalizedValue = startSeg.toCoordinate + ratio *
// (endSeg.toCoordinate - startSeg.toCoordinate)
// = -0.5 + 0.3333 * (0 - (-0.5))
// = -0.3333
// 下表显示了此 'avar' 数据将如何修改几个归一化坐标值：
// 默认归一化值	最终归一化值
// -1.0	-1.0
// -0.75	-0.5
// -0.5	-0.3333
// -0.25	-0.1667
// 0	0
// 0.25	0.25
// 0.5	0.65
// 0.75	0.9375
// 1.0	1.0
// 变化数据
// 变化数据提供描述特定字体值在变化空间上的变化的数据。例如，'gvar' 表中的变化数据描述了 'glyf' 表中的字形轮廓如何通过指定字形轮廓中的各个点如何针对不同变化实例移动来变换。
// 给定字体值的变化表示为适用于变化空间不同区域的增量组合，并以加权方式组合以导出变化空间中不同位置的实例的调整值。变化数据中的每个增量与变化空间上具有效果的特定区域相关联。增量及其关联区域的总体组合构成变化数据。字体中不同项的变化数据存储在不同位置。例如，'glyf' 表条目的变化数据存储在 'gvar' 表中；OS/2 表中某些条目的变化数据存储在 MVAR 表中。在 CFF2 表中的轮廓数据情况下，变化数据存储在 CFF2 表本身内。有关更多详细信息，请参阅以下部分。
// 如上所述，每个增量值与变化空间中具有适用性的特定区域相关联。增量的有效区域在归一化坐标中始终是矩形的。因此，该区域总是可以通过一对 n 元组指定，指定区域对角相对角的位置。在指定区域内，变化效果将从零变化变化到区域内特定位置处的峰值变化。因此，在一般情况下，有三个位置很重要：定义区域范围的对角相对角，以及发生峰值变化的位置。
// 注意：下图将使用两个变化轴。所陈述的概念和陈述适用于具有任意数量轴的字体：区域始终是矩形的，对角相对角加上峰值是描述区域的位置。
// 笛卡尔空间中的区域，具有定义区域范围的两个位置和区域内的峰值位置
// 此一般情况在实践中并不最常见。在大多数情况下，需要描述变化空间外边界处的最大变化，该变化在零原点——默认实例处减小到零变化。在这种情况下，零原点是适用区域的一个角位置，而峰值变化发生在对角相对位置。对于这种常见情况，有效区域和峰值位置可以使用单个 n 元组描述。
// 作为笛卡尔空间中的区域的整个象限
// 更一般但不常见的情况涉及任意区域，如前所述；这些被称为中间区域。在这些情况下，变化数据需要三个 n 元组：一个用于峰值变化位置，两个用于对角相对角处的起始和结束位置。
// 变化数据中的增量值指定峰值位置处的最大调整。效果向其他实例逐渐减弱，对于适用区域外的实例，调整效果降至零。选择给定变化实例时，计算标量值并将其应用于给定增量，以导出与该增量和该实例相关联的净调整。这些标量将始终在 0（零调整）到 1（最大调整）的范围内。下面提供了此标量计算的详细说明。
// 一个示例将有助于解释这些概念。考虑具有字重变化的单轴字体。'glyf' 表中定义的特定字形轮廓可能有一对点（在其他点中），它们是在笔画两侧的曲线上的点。'glyf' 表中的条目将指定字体默认实例的字体设计网格坐标，可能对应于常规字重：
// 字形轮廓中的两个点
// 字重轴上最大值处的变化数据将提供两个轮廓点的 X 和 Y 增量，以根据需要移动它们的位置以用于最重的支持字重实例：
// 使用完整增量量的轮廓点调整位置
// 在这种情况下，第一个点将具有 X 和 Y 增量 +40 和 +10；第二个点将具有增量 +140 和 +10。这些提供了轮廓点的最大调整，在用户选择的实例处于最大字重时应用。对于默认值和最大值之间的字重，如归一化字重值 0.5，效果会按比例减弱。
// 使用缩放增量量的轮廓点调整位置
// 在这种情况下，标量系数 0.5 应用于增量值。
// 标量计算可以被认为是将每个归一化轴值从 -1 到 1 映射到标量范围 0 到 1 的函数。每个具有关联变化数据的区域都有其自己的标量函数，标量函数由区域描述精确定义。
// 例如，在单轴字体中，如果为从 0 到 1 的区域提供增量，峰值效果在 1 处，标量函数如下：
// 标量函数图看起来像一个斜坡，在轴值 0 到 1 之间增加。
// 此示例考虑非中间区域。相同概念可以推广到中间区域。中间区域具有起始和结束轴值，在其间有一些调整效果，以及应用完整调整效果的峰值轴值。标量函数在适用范围内具有三角形形状，在峰值轴值处值为 1.0，在或低于起始轴值处为 0，在结束轴值以上也为 0。
// 标量函数图看起来像一个齿。
// 推广到两个或更多轴时，类似概念适用，但每个轴的贡献被组合成整体效果。为每个轴计算标量，并将每个轴的标量相乘在一起以产生给定增量的整体标量。例如，下图说明了双轴字体中峰值在 (1, 1) 处的区域的标量函数的近似：
// 双轴的标量函数图是一个三维图，看起来像一个具有方形底座的圆锥的一部分。
// 由于为每个轴计算的标量值在 0 和 1 之间，因此将每个轴的标量相乘时，乘积也在 0 和 1 之间。只有当实例轴值与给定增量区域的峰值坐标值在所有轴上对齐时，才能获得给定增量的最大调整效果。
// 'fvar' 表中为轴指定的最小值和最大值确定用户可以选择的实例的限制。如果用户请求低于最小值的轴值，则使用最小值；或者如果请求高于最大值的轴值，则使用最大值。因此，处理所选实例的变化数据时，归一化轴值将始终在 -1 和 1 之间。
// 在此约束假设下，让我们考虑当实例轴值在适用区域之外时给定增量的标量值。如果所选实例在任何轴上超出范围，则该轴的标量值将为 0。如上所述，每个轴的标量相乘在一起以产生整体标量。因此，如果所选实例在任何轴上超出范围，则该增量的整体标量将为 0，并且不会应用该增量的调整。
// 当为区域定义的 n 元组在某个轴上具有峰值值为 0 时，该轴不进入标量计算。这意味着该轴的任何值的调整效果都相同，如果其他轴值保持不变。实际上，适用区域跨越零化轴的完整范围。例如，假设字体有两个轴，字重和宽度，并且为从 (0, 0) 到 (1, 0) 的区域提供增量。在这种情况下，增量适用于第二个轴（宽度）的任何实例值，只要第一个轴（字重）的实例值在范围内：
// 笛卡尔空间中的区域覆盖两个右象限
// 在这种情况下，第二个轴（宽度）的标量函数实际上是一个常数值 1，对标量计算没有影响。以下图形说明了本示例中两个轴（字重和宽度）的标量函数：
// 第一个轴的标量函数图看起来像一个斜坡，在轴值 0 到 1 之间增加。第二个轴的标量函数图是一个常数值 1。
// 对于给定字体值，可以为变化空间中的几个不同区域提供增量。选择特定变化实例时，这些增量中的零个、一个或多个可能具有效果，取决于实例位置是否落入每个增量的适用区域。为每个适用增量计算不同的标量，并将适用增量的缩放值组合以产生整体调整。
// 创建单轴字体时，需要为轴的最小值和最大值极值提供增量。（两个极值，即，除非一个也是默认值。）还可以提供附加的中间区域增量。创建多轴字体时，通常需要为每个轴的最小值和最大值极值提供增量。下图说明了双轴字体的情况：
// 笛卡尔空间中的点位于 (-1,0)、(1,0)、(0,-1) 和 (0,1)
// 双轴字体，每个轴的最小值/最大值极值都有增量
// 如上所述，当为某个轴值具有零值的区域指定增量时，增量适用于该轴的所有值。因此，对于位于 (1, 1) 的实例，(1, 0) 和 (0, 1) 的增量都将适用。这意味着 (1, 0) 增量的调整和 (0, 1) 增量的调整都将应用以产生组合效果。如果每个轴的调整完全独立于其他轴的调整，则两组增量可能足以提供 (1, 1) 实例的预期值。
// 然而，通常仅这两组增量不足以提供所有实例的期望结果，并且需要为 (1, 1) 位置提供附加增量。一般而言，在多轴字体中，通常需要为角极值以及轴端点提供至少一些增量。
// 笛卡尔空间中的点位于 (-1,1)、(-1,0)、(-1,-1)、(0,1)、(0,-1)、(1,1)、(1,0) 和 (1,-1)
// 最小值/最大值极值加上极值交集角都有增量
// 如上所述，默认实例可以对应于一个或多个轴上的最小值或最大值。这允许使用更少的区域和关联增量数据来实现变化空间中的变化。下图说明了双轴字体的一些附加可能性。
// 笛卡尔空间中的点位于 (0,1)、(0,-1)、(1,1) 和 (1,0)
// 一个轴最小值处为默认的双轴字体
// 笛卡尔空间中的点位于 (0,1)、(1,1) 和 (1,0)
// 两个轴最小值处为默认的双轴字体
// 注意：角极值的增量是可选的。根据特定字体设计的需要，可以不为、部分或全部角极值添加增量。在上面的第一个示例中，设计空间角落在图的左上和左下对应于其中一个轴的最小值和最大值，因此需要这些角落的增量以提供该轴上的变化。但右上和右下角的增量是可选的；在此示例中，仅添加了右上角的补充增量。
// 如上所述，中间区域提供轴标量函数，其形状为三角形或"齿"。 barely 重叠且重叠处有急剧上升的一对中间区域可用于沿轴提供某个变化行为的拐点。
// 两个重叠齿函数的图
// 注意：每个中间区域都有其自己的关联增量值，并且增量可用于在重叠点处提供一些急剧过渡。例如，轮廓点可以突然移动以使字形结构的某些元素出现或消失，如下图所示。
// 字重变化的大写 Q 字形，在较重字重时简化笔画
// 较重字重时简化字形结构
// 注意：使用此类技术时，应一起考虑沿轴放置此类过渡点以及命名实例的放置，以便急剧过渡不会发生在命名实例附近。这将避免不同应用程序在使用命名实例时由于处理数值的小差异而出现不一致行为的可能性。
// 注意：使用此类技术时，重要的是要记住，某些应用程序将支持任意实例的选择，包括轴值在重叠范围内的实例，并且在重叠范围内，两个中间区域的缩放增量将具有累积效果。可能需要一些设计迭代，对增量值或区域重叠方式的小调整，以避免过渡范围内的意外或不良结果。
// 注意：上图说明了使用中间区域实现"笔画减少"效果。另一种可用于改变特定变化轴值范围的字形结构的技术是字形替换。OpenType 布局 GSUB 表中的必需变化替代功能与 FeatureVariations 表结合使用，可以在选择某个变化轴范围内的一个或多个轴的变化实例时执行字形替换。这可能是一种更容易维护的技术，通常推荐用于实现此类效果。
// 以上提供了变化数据的基本概念概述：适用区域、每个轴和整体标量以及多个适用增量的组合效果。下面提供了插值过程的详细规范。
// 变化数据表和杂项要求
// 上一节确定了字形轮廓点的 X 和 Y 坐标作为可以针对不同变化实例进行调整的数据项。字体中的许多其他数据项也可以调整，包括：
// OS/2、'hhea'、'vhea' 或 'post' 表中的字体范围度量值。
// 'hmtx'、'vmtx' 或 VORG 表中的字形度量值。
// 'gasp' 表中的 PPEM 范围。
// GPOS 或 JSTF 表中的锚点位置，以及字形位置或推进的调整。
// GDEF 表中连字插入符位置的 X 或 Y 坐标。
// BASE 表中基线度量的 X 或 Y 坐标。
// CVT 值。
// COLR 表中彩色字形的渐变放置和颜色停止偏移、颜色 alpha 值和变换。
// 可变字体可以为这些中的任何一个或全部包含变化数据。不同项的变化数据在字体中的各个表中提供。
// 注意：虽然字体中的几个数据项可能需要针对不同实例进行调整，但还有其他项不受变化影响。例如，字体家族和 unitsPerEm 不受变化影响。然而，应该特别注意的是，某些可能受变化影响的值不支持变化数据。特别是，字体头（'head'）表中的 xMin、yMin、xMax、yMax、macStyle 和 lowestRecPPEM 字段不支持变化数据，应仅与字体的默认实例相关。此外，字距调整（'kern'）表中的变化不受支持；可变字体应使用 GPOS 表处理字距调整。
// 可变字体中需要两个表：
// 字体变体（'fvar'）表，描述字体支持的变化。
// 样式属性（STAT）表，建立属于家族的字体之间的关系，并通过允许平台将涉及许多轴的变化实例投影到假设有限变化轴集的较旧字体家族模型中，提供与旧应用程序的某种兼容性。
// 可变字体必须包含一些其他变化相关数据，根据设计变化的方式，但没有其他特定类型的变化相关数据是所有可变字体必需的。
// 如果可变字体在 'glyf' 表中具有 TrueType 轮廓，轮廓变化数据可以在字形变体（'gvar'）表中提供。CVT 值的变化数据可以在可选的 CVT 变体（'cvar'）表中提供。
// 如果可变字体在紧凑字体格式 2.0（CFF2）表中具有 PostScript 风格轮廓，CFF2 表本身也可以包含关联的变化数据。
// 注意：CFF2 表可以在非可变字体和可变字体中使用。另请注意，不支持使用紧凑字体格式版本 1.0（'CFF '）表的轮廓变化。
// 度量变体（MVAR）表用于为 'gasp'、'hhea'、OS/2、'post' 和 'vhea' 表中的各种字体范围度量或其他数值提供变化数据。如果任何这些值需要调整，则应添加 MVAR 表。请注意，并非必须为 MVAR 表涵盖的所有数据项提供变化数据：所有项的变化数据都是可选的。如果给定项没有变化数据，默认值适用于所有实例。
// 注意：Apple 平台允许使用字体度量（'fmtx'）表，通过引用指定字形的轮廓点的 X 或 Y 坐标来指定各种字体范围度量值。OpenType 字体变体不使用字体度量表。
// 'hmtx' 和 'vmtx' 表提供水平和垂直字形度量。可以使用水平度量变体（HVAR）和垂直度量变体（VVAR）表为水平和垂直字形度量提供变化数据。
// 在具有 TrueType 轮廓的字体中，栅格化器将 'hmtx' 和 'vmtx' 值与 'glyf' 表中的字形 xMin、xMax、yMin 和 yMax 值组合，生成四个对应于字形水平和垂直度量值的"虚点"。（有关虚点的更多背景，请参阅指导 TrueType 字形章节。）在可变字体中，'gvar' 表中字形的变化数据将包括字形虚点的调整增量。因此，可以通过插值给定实例的虚点位置来获得给定实例的插值字形度量。然而，对于某些文本布局操作，这可能成本高昂。为了在所有平台上提供最佳性能，建议所有具有 TrueType 轮廓的可变字体包含 HVAR 表。如果字体支持垂直布局并包含 'vhea' 和 'vmtx' 表，建议字体包含 VVAR 表。
// CFF2 栅格化器不生成虚点，CFF2 变化数据不包含虚点的调整增量。因此，在具有 CFF2 轮廓的可变字体中，需要 'hmtx' 和 HVAR 表。类似地，如果字体支持垂直布局，则需要 'vmtx' 和 VVAR 表。
// 注意：'hdmx' 和 VDMX 表在可变字体中不使用。
// 如果字体具有 OpenType 布局表，GDEF、GPOS 或 JSTF 表中的值的变化数据将根据需要包含在 GDEF 表中。BASE 表的变化数据将根据需要包含在 BASE 表本身中。
// 在某些可变字体中，可能希望在字体的变化空间中的不同区域使用不同的字形替换或字形定位操作。例如，对于窄宽度或重字重实例，其中字腔变小，可能希望进行某些字形替换，以使用具有某些笔画移除或轮廓简化的替代字形，以允许更大的字腔。这种效果可以使用 GSUB 或 GPOS 表中的功能变化子表来实现。有关更多信息，请参阅 OpenType 布局通用表格式章节。
// 在具有 TrueType 轮廓的可变字体中，每个字形的左侧承必须等于 xMin，并且 'head' 表标志字段中的位 1 必须设置。
// 在所有可变字体中，'head' 表标志字段中的位 5 必须清除。（在某些平台上，位 5 影响垂直布局中的度量。位 5 必须清除以确保所有平台上的兼容行为。）
// 实例值插值算法
// 为不同变化实例插值调整值的过程用于所有需要变化的字体数据项——字形轮廓点的位置、升部或其他字体范围度量等。插值过程涉及：
// 确定适用于该实例的增量。
// 对于每个适用增量，计算该实例的每个轴标量，然后将每个轴的标量相乘以产生该增量的整体标量。
// 将每个适用增量乘以计算出的标量。
// 组合所有缩放增量以产生整体调整。
// 处理 'gvar' 表时，计算中有一个附加步骤，即当增量未明确给出时推断点的增量调整。这仅适用于 'gvar' 表，并在 'gvar' 表章节中描述。
// 如上所述，给定增量的实例轴值在适用区域之外等价于该增量的每个轴标量值为零。此外，对于给定增量没有影响的轴（n 元组对该轴的峰值值为零）等价于该轴的每个轴标量值为 1。因此，适用性确定和轴交互都可以组合成推导整体标量的步骤。
// 以下插值过程的描述将引用起始、峰值和结束坐标值。如上所述，中间区域使用三个 n 元组描述，两个用于对角相对角（起始和结束）指定区域范围，以及一个峰值。非中间区域的一个角在峰值处，另一个角在零原点。在某些变化数据结构中，非中间区域使用单个 n 元组指定，即峰值。在这种情况下，起始和结束坐标是隐含的：一个与峰值相同，另一个是零原点。
// 为了变化数据中的区域定义有效，起始、峰值和结束值必须良序。也就是说，对于每个轴，起始轴坐标必须小于或等于峰值坐标，并且峰值坐标必须小于或等于结束坐标。此外，起始和结束坐标必须同时为非负或非正——它们不能跨越零。
// 在到目前为止的讨论中，个体增量被描述为具有关联的区域。变化数据可以以不同的方式组织。在某些情况下，如在 'gvar' 表中，几个对应于许多目标项（字形的所有轮廓点）和变化空间单个区域的增量被组织在一起。在某些其他情况下，如在 MVAR 或 CFF2 表中，覆盖多个区域的增量由个体目标项组织在一起。无论哪种情况，个体增量都与变化空间的特定区域相关联。以下插值过程的描述将引用插值个体项的值，但当应用于特定上下文（如 'gvar' 表）时，应理解为相同的计算并行应用于许多不同的项。
// 如上所述，给定增量的效果由范围从 0 到 1 的标量函数调节，对于区域关联峰值位置处的实例，值为 1。整体标量是每个轴标量的乘积，每个轴标量计算为实例坐标值与峰值坐标值的接近度相对于峰值到区域边缘距离的比例。
// 例如，考虑双轴变化空间中的中间区域（下图中的绿色），角在 (0.3, 0.15) 和 (1, 1)，峰值（下图中的蓝色）在 (0.7, 0.5)：
// 具有从 (0.3, 0.1) 到 (1,1) 的矩形区域和 (0.75, 0.5) 峰值点的笛卡尔空间
// 然后考虑实例（下图中的红色）在 (0.5, 0.35)。每个轴的标量将是实例坐标值到区域最近边缘的距离除以峰值到该边缘的距离：
// 具有矩形区域的笛卡尔空间，以及用于计算每个轴标量的值
// 此区域的整体标量将是两个轴标量的乘积：0.5 × 0.571429 = 0.285714。
// 以下伪代码提供了为给定目标项和给定实例计算插值值的详细算法：
// C#
// netAdjustment = 0; /* 将累积调整初始化为零 /
// (foreach R in Regions) / 对于每个区域，计算标量 S /
// {
// S = 1; / 将区域的整体标量初始化为 1 */
// plain
// /* 对于每个轴，计算每个轴标量 AS */
// (for i = 0; i < axisCount; i++)
// {
//     /* 如果区域定义相对于某个轴无效，
//     则忽略该轴。对于区域相对于给定轴有效，
//     它必须具有在起始和结束值之间的峰值，
//     并且起始和结束值不能具有不同符号
//     如果峰值非零。（如果峰值为零，起始和结束
//     可以具有不同符号：这可用于如果轴在标量计算中
//     要被忽略。）*/
// 
//     if (startCoords[i] > peakCoords[i] || peakCoords[i] > endCoords[i])
//         AS = 1;
//     else if (startCoords[i] < 0 && endCoords[i] > 0 && peakCoords[i] != 0)
//         AS = 1;
// 
//     /* 注意：对于剩余情况，起始、峰值和结束都将 <= 0
//     或都将 >= 0，或者峰值将 == 0。*/
// 
//     /* 如果某个轴的峰值为零，则忽略该轴。*/
//     else if (peakCoords[i] == 0)
//         AS = 1;
// 
//     /* 如果实例坐标在某个轴上超出范围，则区域及其关联增量不适用。*/
//     else if (instanceCoords[i] < startCoords[i]
//              || instanceCoords[i] > endCoords[i])
//         AS = 0;
// 
//     /* 区域适用：计算每个轴标量作为实例到峰值在区域内接近度的比例。*/
//     else
//     {
//         if (instanceCoords[i] == peakCoords[i])
//             AS = 1;
//         else if (instanceCoords[i] < peakCoords[i])
//         {
//             AS = (instanceCoords[i] - startCoords[i])
//                  / (peakCoords[i] - startCoords[i]);
//         }
//         else /* instanceCoords[i] > peakCoords[i] */
//         {
//             AS = (endCoords[i] - instanceCoords[i])
//                  / (endCoords[i] - peakCoords[i]);
//         }
//     }
// 
//     /* 整体标量是所有每个轴标量的乘积。
//     注意：轴标量和整体标量将始终 >= 0 且 <= 1。*/
//     S = S * AS;
// } /* 每个轴循环 */
// 
// /* 获取此区域的缩放增量 */
// scaledDelta = S * delta;
// 
// /* 累积每个区域的调整 */
// netAdjustment = netAdjustment + scaledDelta;
// } /* 每个区域循环 */
// /* 将累积调整应用于默认值以导出插值值 */
// interpolatedValue = defaultValue + netAdjustment;
// 将缩放增量应用于默认值时，组合结果可能超出字体中默认值使用的数据类型的范围。例如，默认值可以在字体中表示为 16 位值，但应用增量后的实例值可能需要更多位来表示。计算期间和最终结果（interpolatedValue）可以使用实现确定的表示。计算中使用的数字范围必须至少与增量应用到的项的数据类型相同；例如，将缩放增量应用于 FWORD 值时至少为 [-32768, 32767]。
// 将缩放的变化增量应用于标量值需要涉及分数值的计算。在标量（S、AS）和插值值（scaledDelta、netAjustment、interpolatedValue）的计算中，应至少保持 16 位小数精度。如果需要内部表示，仅应在最终结果使用时进行舍入，并且可以保留比增量应用到的项的数据类型更大的小数位深度。另请参阅上面的坐标尺度和归一化，了解有关精度和舍入的相关讨论。
// 根据使用的内部表示，应用增量时算术操作的结果可能超出内部表示支持的范围。增量相加的顺序未规定，但也可能是溢出是否发生的因素。如果资源允许，应用程序应允许更大的范围以避免计算过程中任何点溢出的可能性，并确保增量应用的顺序不影响最终结果。无论字体表示中默认值使用的类型如何，计算中应至少使用 32 个有效位。
// 如果内部表示的溢出不可避免，可以使用饱和算术（钳制，而不是回绕）来缓解错误伪影。然而，一般来说，溢出行为未定义。因此，字体开发人员应注意增量组合可能超出增量应用到的字体数据的数据类型范围的情况，并预期结果行为在不同应用程序中可能不一致。特别是，字体开发人员不应依赖特定应用程序的溢出行为。
// 插值示例
// 以下示例说明了特定实例的插值过程。此示例基于 Skia 字体的字形 45（字形名称"hyphen.oldstyle"），即字符 U+002D 连字符减号的字形。
// 注意：Skia 字体包含在 Apple 的 OSX 平台中。在 OpenType 1.8 规范发布时，现有版本的 Skia 字体整体上不符合 OpenType 1.8 规范，但 'gvar' 表中变化数据的实现（此处说明）确实符合。
// 'glyf' 表中的字形条目有一个轮廓，包含四个点。基于 'hmtx' 表中字形 45 的值，栅格化器推断"虚点"来表示左和右侧承。（对于此示例，假设水平布局，因此忽略顶部和底部虚点。）这些虚点位于 (0, 0) 和 (698, 0)。因此，有六个点需要插值。
// 具有四个点的字形轮廓，加上两个'虚点'。字形看起来像连字符减号。
// Skia 字体具有字重和宽度轴。'gvar' 表中字形 45 的变化数据具有与字重宽度变化空间内 8 个区域相关联的增量。将考虑其中三个，称为 R1、R2 和 R3。每个都是非中间区域，因此使用单个 n 元组定义。每个的 n 元组如下：
// 区域	（字重，宽度）
// R1	(1, 0)
// R2	(0, 1)
// R3	(1, 1)
// 下图说明了这些区域中每个在变化空间上的适用性范围：
// 覆盖两个右象限的区域笛卡尔空间
// R1 对宽度轴具有零坐标值，这意味着变化实例的宽度变化对标量计算没有影响。
// 覆盖两个上象限的区域笛卡尔空间
// R2 对字重轴具有零坐标值，这意味着变化实例的字重变化对标量计算没有影响。
// 覆盖右上象限的区域笛卡尔空间
// R3 对字重和宽度轴都具有非零坐标值，这意味着变化实例的字重或宽度变化都会影响此区域的标量计算。
// 现在考虑字体中为每个点指定的与这三个区域关联的增量值。为每个点指定 X 和 Y 增量。
// R1 具有以下关联增量：
// pt 0	pt 1	pt 2	pt 3	pt 4	pt 5
// X	234	-26	-26	234	0	209
// Y	-135	-135	175	175	0	0
// 将这些增量应用于原始点位置，与 R1 关联的增量的最大效果将修改轮廓如下：
// 具有四个轮廓点的默认字形轮廓，类似连字符减号，以及虚点。还有应用 R1 区域增量后的调整轮廓和虚点。
// 对于变化实例 (1, 0)（最重字重，默认宽度），其他区域的标量将为零，因此这将是该实例的轮廓结果。降低实例的字重值将减弱变化的程度，轮廓在原始轮廓和此最大修改轮廓之间插值。
// 现在考虑 R2：它具有以下关联增量：
// pt 0	pt 1	pt 2	pt 3	pt 4	pt 5
// X	165	20	20	165	0	187
// Y	-2	-2	2	2	0	0
// 将这些增量应用于原始点位置，与 R2 关联的增量的最大效果将修改轮廓如下：
// 具有四个轮廓点的默认字形轮廓，类似连字符减号，以及虚点。还有应用 R2 区域增量后的调整轮廓和虚点。
// 对于变化实例 (0, 1)（常规字重，最宽宽度），其他区域的标量将为零，因此这将是该实例的插值轮廓结果。
// 现在考虑 R3：它具有以下关联增量：
// pt 0	pt 1	pt 2	pt 3	pt 4	pt 5
// X	0	0	0	0	0	0
// Y	0	0	0	0	0	0
// 由于所有增量值都为零，与此区域关联的数据对字形轮廓完全没有效果。（事实上，这些数据是多余的。）
// 现在，考虑变化实例 (1, 1)（最重字重，最宽宽度）。所有三个区域 R1、R2 和 R3 都适用于此实例。如上所述，与 R3 关联的变化数据对字形没有效果。但区域 R1 和 R2 的数据也适用于此实例，它们的最大效果将组合。也就是说，来自 R1 和 R2 关联数据的每个点的 X 和 Y 增量都将应用于点的 X 和 Y 坐标。这将修改字形轮廓如下：
// 具有四个轮廓点的默认字形轮廓，类似连字符减号，以及虚点。还有应用 R1 和 R2 区域增量后的调整轮廓和虚点。
// 对于其他字重 > 0 且 < 1 且宽度 > 0 且 < 1 的变化实例，区域 R1 和 R2 的数据都将应用，但两个区域的标量会变化，导致每个区域的数据对轮廓的不同比例效果。例如，考虑坐标为 (0.2, 0.7) 的变化实例——字重略微增加，宽度大幅增加。R1 和 R2 的区域标量将分别为 0.2 和 0.7。这些将分别应用于每个区域的增量，并且给定点的缩放增量值组合：
// pt 0	pt 1	pt 2	pt 3	pt 4	pt 5
// X	0.2 × 234
// 0.7 × 165
// = 162.3	0.2 × -26
// 0.7 × 20
// = 8.8	0.2 × -26
// 0.7 × 20
// = 8.8	0.2 × 234
// 0.7 × 165
// = 162.3	0.2 × 0
// 0.7 × 0
// = 0	0.2 × 209
// 0.7 × 187
// = 172.7
// Y	0.2 × -135
// 0.7 × -2
// = -28.4	0.2 × -135
// 0.7 × -2
// = -28.4	0.2 × 175
// 0.7 × 2
// = 36.4	0.2 × 175
// 0.7 × 2
// = 36.4	0.2 × 0
// 0.7 × 0
// = 0	0.2 × 0
// 0.7 × 0
// = 0
// 这将修改字形轮廓如下：
// 具有四个轮廓点的默认字形轮廓，类似连字符减号，以及虚点。还有应用 R1 和 R2 缩放增量后的调整轮廓和虚点。
// 静态实例字体的动态生成
// 在某些应用程序工作流程中，可能需要为特定实例动态生成静态字体资源——即使用特定实例插值值的常规非变化字体表。这可能需要向不理解或不支持可变字体的旧软件或数据格式提供字体数据，例如旧打印机驱动程序，或具有嵌入字体数据的 PDF 或 XPS 文件。
// 例如，可能需要处理可变字体中的 'glyf' 和 'gvar' 表以生成具有特定实例插值轮廓数据的新 'glyf' 表；或处理 'hhea' 和 MVAR 表以生成具有特定实例数据的新 'hhea' 表。
// 不同的应用场景可能需要更多或更少的完整字体数据，需要生成不同的非变化特定字体表集。此处未指定最低要求。但应注意以下几点：
// 某些场景可能需要在实例字体数据中使用 PostScript 名称（名称 ID 6），对于使用的每个实例具有不同的名称。Adobe 技术说明提供了可用于从可变字体派生的实例字体的 PostScript 名称生成规范。请参阅 Adobe 技术说明 #5902："变体字体的 PostScript 名称生成"。
// 对于具有 CFF2 轮廓的可变字体，某些工作流程——例如打印——可能需要生成具有 'CFF ' 表的实例字体。在这种情况下，如果可变字体在 FontDICTINDEX 中有多个 Font DICT，则应生成 CID 键控的 CFF 字体，ROS 为"Adobe-Identity-0"。如果可变字体在 FontDICTINDEX 中有一个 Font DICT，则可以生成名称键控的 CFF 字体，如果在 'post' 表中提供了字形名称（某些旧工作流程查找字形名称以获取语义）；否则，可以如上生成 CID 键控的 CFF。将 CFF2 CharStrings 转换为 Type2 CharStrings 将涉及重新优化 CharString 参数和运算符，以避免超过允许的最大堆栈深度。从 CFF2 规范中移除的大多数 CFF 字段可以省略，以便它们继承 CFF 默认值。有关 'glyf'、'CFF ' 和 CFF2 表之间的差异摘要，请参阅比较 'glyf'、'CFF ' 和 CFF2 表。
// 具有 TrueType 轮廓的可变字体可能利用 GET VARIATION 指令向字形程序提供当前变化轴坐标。在需要动态生成实例字体数据的场景中，应假设不支持此指令。在生成插值 'glyf' 表的过程中，需要对 GET VARIATION 指令进行特殊处理，以确保程序获得给定实例的适当轴坐标值。有关详细信息，请参阅 TrueType 指令集。

#endif // PRH_TRUETYPE_IMPLEMENTATION

// FULL VERSION HISTORY
//
//   0.01 (2026-09-01) initial release for basic parse
//

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2026 Godelder Brother
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
