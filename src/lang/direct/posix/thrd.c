// 线程间除全局内存还共享以下属性，它们对于进程而言是全局的，并非针对某个特定线程：
//
// 1. 进程ID和父进程ID，进程组ID与会话（session）ID，进程凭证（credential）
// 2. 控制终端，打开的文件描述符
// 3. 由 fcntl() 创建的记录锁（record lock）
// 4. 信号处置，间隔定时器和POSIX定时器
// 5. 文件系统的相关信息，例如文件权限掩码、当前工作目录和根目录
// 6. 系统 V（System V）信号量撤销（undo semadj）
// 7. 资源限制（resource limit），资源消耗（getrusage()）
// 8. CPU 时间消耗（times()），nice 值（setpriority() nice()）
//
// 各线程所独有的属性包括：
//
// 1. 线程ID，线程本地存储数据，errno 变量，浮点环境（fenv(3)）
// 2. 信号掩码，备选信号栈（sigaltstack()）
// 3. 实时调度策略和优先级
// 4. CPU 亲和力（affinity，Linux 所特有）
// 5. 能力（capability，Linux 所特有）
// 6. 线程栈，本地变量和函数的调用链接信息
//
// 所有的线程栈均驻留于同一虚拟地址空间，这意味着利用合适的指针，各线程可以在对方栈中
// 相互共享数据。这种方法偶尔也能派上用场，但由于局部变量的状态有效与否依赖于其所驻留
// 栈帧的生命周期，故而需要谨慎处理这一问题。当函数返回时，该函数栈帧所占用的内存区域
// 有可能为后续的函数调用所重用。如果线程终止，那么新线程有可能会对已终止线程的所占用
// 的内存空间重新加以利用。若无法正确处理这一依赖关系，由此产生的程序问题将难以捕获。
//
// 在多线程应用中处理信号，需要小心设计，作为通则一般建议在多线程程序中避免使用信号。
// UNIX 信号模型是基于 UNIX进程模型而设计的，问世比 pthreads 要早几十年。自然而然，
// 信号与线程模型之间存在一些明显的冲突。注意是因为，一方面针对单线程进程要保持传统的
// 信号语义（pthreads不应该改变传统进行的信号语义），与此同时有需要开发出适用于多线程
// 进程环境的新信号模型。信号与线程模型之间的差异意味着，将二者结合使用将会非常复杂，
// 应尽可能加以避免。尽管如此，有的时候还是必须在多线程中处理信号问题。
//
// 1995年POSIX.1c对POSIX线程API进行了标准化，该标准后来为SUSv3所接纳。Pthreads API
// 定义了一系列数据类型，如下所示。SUSv3 并未规定如何实现这些数据类型，可移植的程序应
// 该将其视为透明数据，程序应避免对此类数据类型变量的结构或内容产生任何依赖。 ::
//
//      pthread_t               线程ID
//      pthread_mutex_t         互斥对象
//      pthread_mutexattr_t     互斥属性
//      pthread_cond_t          条件变量
//      pthread_condattr_t      条件变量属性
//      pthread_key_t           线程特有数据的键
//      pthread_once_t          一次性初始化控制上下文
//      pthread_attr_t          线程属性
//
// 启动程序时产生的进程只有单条线程，称之为初始或主线程。调用 pthread_create() 负责
// 创建一个新的线程。新线程通过调用带有参数arg的函数start开始执行，并带有返回值类型
// 为 void*，当经强制转换的整数作为线程start函数的返回值时，必须避免与实现所定义的常
// 量 PTHREAD_CANCELED 值相同，否则线程退出后不能分辨是正常退出还是被取消而退出。参
// 数thread用户获取线程ID，SUSv3明确指出，在新线程开始执行之前，实现无需对thread参数
// 进行赋值，即新线程可能会在pthread_create返回给调用者之前已经开始运行。如新线程需
// 要获取自己的线程ID，则只能使用pthread_self()获取。调用pthread_create()之后，应用
// 程序无法确定接着会调度哪一个线程来使用CPU资源，程序不能隐含认为对特性调度顺序的依
// 赖。可以给线程设置线程属性，如果将attr设置为NULL，那么创建新线程时将使用各种默认属
// 性。
//
//      int pthread_create(
//          pthread_t *thread,
//          const pthread_attr_t *attr,
//          void *(*start)(void *),
//          void *arg);
//
// pthread_exit()函数用于终止调用线程，其返回值可由另一线程通过调用pthread_join()函
// 数来获取。调用pthread_exit相当于在线程start函数中执行return，不同之处在于，可在线
// 程start函数所调用的任意函数中调用pthread_exit()。参数retval执行了线程的返回值，
// retval所指向的内容不应该分配在线程栈中，因为线程终止后，将无法确定线程栈的内容是否
// 有效，例如系统可能会立刻将改进程虚拟内存的这片区域重新分配，供一个新的线程栈使用。
// 处于同样的理由，也不应该在线程栈中分配线程start函数的返回值。 ::
//
//      void pthread_exit(void *retval);
//
// 线程可以以以下方式终止运行：
//
// 1. 线程start函数执行return语句并返回指定值
// 2. 线程调用pthread_exit()
// 3. 调用pthread_cancel()取消线程
// 4. 任意线程调用了exit()，或者主线程执行了return语句，会导致进程中所有线程终止
// 5. 如果主线程调用了pthread_exit()，那么其他线程将继续运行
//
// 进程内部的每个线程都有一个唯一的标识（同一个进程中保证唯一，但不同进程中的线程ID可
// 能重名），称为线程ID。线程ID的用处：不同的pthreads函数利用线程ID来识别要操作的目标
// 线程，这些函数包括pthread_join() pthread_detach() pthread_cancel()
// pthread_kill()等；在一些应用中，以特定线程的线程ID作为动态数据结构的标签，以识别某
// 个数据结构的创建者和操作者。函数pthread_equal()可检查两个线程ID是否相同。因为必须
// 将pthread_t作为一种透明的数据类型对待，所以函数pthread_equal是必须的。Linux将
// pthread_t定义为无符号长整数，但在其他实现中则有可能是一个指针或结构体。SUSv3并未要
// 求将pthread_t实现为一个标量类型，该类型也可以是一个结构体。因此将线程ID强制类型转
// 换成整数值并不具有可移植性。在Linux的线程实现中，线程ID在所有进程中都是唯一的，不过
// 在其他实现中则未必如此，SUSv3特别之处，应用程序若使用线程ID来标识其他进程的线程，其
// 可移植性无法得到保证。此外，在对已终止线程施以pthread_join()，或者在已分离线程退出
// 后（detached thread），实现可以复用该线程的线程ID。 ::
//
//      pthread_t pthread_self(void);
//      int pthread_equal(pthread_t a, pthread_t b);
//
// POSIX线程ID与Linux专用的系统调用gettid()所返回的线程ID并不同。POSIX线程ID由线程
// 库实现和维护。gettid()返回的线程ID是一个内核分配的数字，类似于进程ID。虽然在Linux
// NPTL（Native POSIX Threads Library）实现中，每个POSIX线程都对应一个唯一的内核线
// 程ID，但应用程序一般无需了解内核线程ID，况且如果程序依赖于这一信息也将无法移植。
//
// 连接（joining）已终止的线程，函数pthread_join()等待对应的线程终止，如果线程已经
// 终止，pthread_join()会立即返回。retval可以获取线程return或通过pthread_exit()
// 终止时的返回值。如果前面一个终止的线程已经完成join，再次调用pthread_join将会导致
// 无法预知的行为。例如，相同的线程ID可能在join以后已经为另一个新建线程所重用，在度
// 调用pthread_join()可能等待的这个新线程。若线程并未分离（detached），则必须进行
// join。如果未能完成join，那么线程终止时将产生僵尸线程，与僵尸进程的概念相类似。除了
// 浪费系统资源以外，僵尸线程若积累过多，应用将再也无法创建新的线程。 ::
//
//      int pthread_join(pthread_t thread, void **retval);
//      int pthread_detach(pthread_t thread);
//      pthread_detach(pthread_self());
//
// pthread_join()执行的功能类似于针对进程的waitpid()，不过二者之间存在一些显著差别。
// 线程之间的关系时对等的（peers），进程中的任意线程均可以调用 pthread_join() 与该
// 进程的任何其他线程连接起来。例如如果线程A创建线程B，线程B再创建线程C，那么线程A可
// 以连接线程C，线程C也可以连接线程A。这与进程间的层次关系不同。父进程如果使用fork()
// 创建了子进程，那么它也是唯一能够对子进程调用wait()的进程。调用pthread_create()
// 创建的新线程与发起调用的线程之间，就没有这样的关系。无法“连接任何线程”，而对于进程
// 则可以通过调用waitpid(-1,&status,options)做得这一点，也不能以非阻塞方式进行连接，
// 类似于设置WHOHANG标志的waitpid()。限制pthread_join()只能连接特定线程ID，这样做
// 是“别有用心”的。其用意在于，程序应只能连接它所知道的线程。线程之间并无层次关系，如
// 果听任与任意线程连接的操作发生，那么所谓“任意”线程就可以包括由库函数私自创建的线
// 程，从而带来问题，而后面展示的使用条件变量技术也只运行线程连接它知道的其他线程。结
// 果是，函数库再获取线程返回状态时将不再能与该线程连接，只会一错再错，试图连接一个已
// 经过的线程ID。换言之，连接任意线程的操作与模块化的程序涉及理念背道而驰。
//
// 默认情况下，线程是可连接的（joinable），也就是说当线程退出时，其他线程可以通过调用
// pthread_join()获取其返回状态。有时程序员并不关心线程的返回状态，只是希望再线程终
// 止时能够自动清理并移除。在这种情况下，可以调用pthread_detach()将对应的线程标记为
// 分离（detached）状态。例如以下的代码，一个线程自己可以进行自我分离。一旦线程处于分
// 离状态，就不能再使用pthread_join()来获取其状态，也无法使其重返可连接状态。其他线程
// 调用了exit()或是主线程执行return语句时，即便遭到分离的线程也还是会受到影响。此时，
// 不管线程处于可连接状态还是已分离状态，进程的所有线程会立即终止。换言之，
// pthread_detach()只能控制线程终止之后所发生的事情，而非何时或如何终止线程。一个已经
// 处于分离状态的线程ID，不能再次使用它来调用pthread_join()或pthread_detach()。
//
// 可以使用 pthread_attr_t 设置线程的属性，包括初始的 detach 状态，栈大小和栈地址，
// 以及其他一些设定。其中新创建线程的 detach 状态默认是 PTHREAD_CREATE_JOINABLE。
// 创建线程时，每个线程都有一个属于自己的线程栈，且大小固定。在 Linux/x86-32架构上，
// 除主线程外的所有线程，其栈的缺省大小均为2MB，在一些64位架构下默认尺寸要大一些。为
// 了应对栈的增长，主线程的空间要打出许多。偶尔也需要改变线程栈的大小，再通过线程属性
// 对像创建线程时，调用函数pthread_attr_setstacksize()所设置的线程属性决定了线程栈
// 的大小。而使用与之相关的另一函数pthread_attr_setstack()可以同时控制线程栈的大小
// 和位置，不过设置栈的地址将降低程序的可移植性。更大的线程栈可以容纳大型的自动变量或
// 者深度的嵌套调用，这是改变每个线程栈大小的原因之一。而另一方面应用程序可能希望减少
// 每个线程栈，易变进程可以创建更多的线程。例如在x86-32系统中，用户模式可访问的虚拟地
// 址空间是3GB，而2MB的缺省栈大小则意味着最多只能创建1500各线程，更为准确的最大值还视
// 代码段、数据段、共享函数库等对虚拟内存的消耗量。特定架构的系统上，可采用的线程栈大
// 小最小值可以通过调用sysconf(_SC_THREAD_STACK_MIN)来确定，在Linux/x86-32上的
// NPTL实现中，该调用返回16384。在NPTL线程实现中，如果对线程栈尺寸资源限制
//（RLIMIT_STACK）的设置不同于unlimited，那么创建线程时会以其作为默认值。对该限制的
// 设置必须在运行程序之前，通过执行shell内建命令ulimit -s完成，在C shell下命令
// 为limit stacksize。在主程序中调用setrlimit()来设置限制的办法可能行不通，因为NPTL
// 在调用main()之前的运行时初始化期间就已经确定了默认栈的大小。
//
// pthread_attr_setstacksize() 函数将栈大小属性设置为 stacksize。栈大小属性决定了
// 线程分配的最小栈空间大小（以字节为单位）。pthread_attr_setstacksize() 可能会因以
// 下错误而失败：EINVAL 栈大小小于 PTHREAD_STACK_MIN（16384 字节）；在某些系统中，
// 如果 stacksize 不是系统页面大小的倍数，可能会返回 EINVAL 错误。线程的栈大小在创建
// 线程时就已固定，只有主线程的栈可以动态增长。缺陷说明：截至 glibc 2.8，如果指定的
// stacksize 不是 STACK_ALIGN（在大多数架构上为 16 字节）的倍数，它可能会向下取整，
// 这违反了 POSIX.1 标准。POSIX.1 规定分配的栈空间至少应为 stacksize 字节，而向下
// 取整会小于提供的 stacksize 参数值。
//
// pthread_attr_setstack() 函数将栈地址和栈大小属性设置为stackaddr和stacksize。这
// 些属性指定了线程应使用的栈的位置和大小。stackaddr 应指向调用者分配的大小为
// stacksize 字节的缓冲区的最低可寻址字节。已分配缓冲区的页面应同时具备可读和可写权
// 限。pthread_attr_setstack() 可能会因以下错误而失败：EINVAL：stacksize 小于
// PTHREAD_STACK_MIN（16384 字节）。在某些系统中，如果 stackaddr 或
// stackaddr + stacksize 未正确对齐，也可能会出现此错误。EACCES：POSIX.1 标准还提
// 到，如果 stackaddr 和 stacksize 所描述的栈区域对于调用者而言并非既可读又可写，则
// 会返回此错误。这些函数是为那些必须确保线程栈位于特定位置的应用程序提供的。对于大多
// 数应用程序来说，这并非必要，应避免使用这些函数。如果应用程序只是需要非默认的栈大小，
// 可使用 pthread_attr_setstacksize(3)。当应用程序使用 pthread_attr_setstack()
// 时，它就承担起了分配栈的责任。使用 pthread_attr_setguardsize(3) 设置的任何警戒区
// 大小值都将被忽略。如果认为有必要，应用程序有责任分配一个警戒区（一个或多个受保护的
// 页面，禁止读写）来处理栈溢出的可能性。stackaddr 中指定的地址应正确对齐：为了实现完
// 全的可移植性，应将其对齐到页面边界 sysconf(_SC_PAGESIZE)，使用 posix_memalign(3)
// 分配可能会很有用。可能地，stacksize 也应该是系统页面大小的倍数。如果使用 attr 创建
// 多个线程，那么调用者必须在每次调用 pthread_create(3) 之间更改栈地址属性；否则，这
// 些线程将尝试使用相同的内存区域作为它们的栈，从而导致混乱。
//
// pthread_attr_setguardsize() 函数将警戒区大小属性设置为 guardsize。如果大于 0，
// 那么系统会在线程栈的末尾额外分配至少 guardsize 字节的区域，作为栈的警戒区。如果
// guardsize 为 0，那么不会有警戒区。默认的警戒区大小与系统页面大小相同。如果已经设
// 置了栈地址属性（使用 pthread_attr_setstack() 或 pthread_attr_setstackaddr()），
// 这意味着调用者正在为线程分配栈，那么警戒区大小属性将被忽略（系统不会创建警戒区）：
// 应用程序有责任处理栈溢出问题。也许可以使用 mprotect() 手动在其分配的栈末尾定义一个
// 警戒区。警戒区由受保护的虚拟内存页面组成，以防止读写访问。如果一个线程的栈溢出到警
// 戒区，在大多数硬件架构上，它会收到一个 SIGSEGV 信号，从而得知发生了栈溢出。警戒区
// 从页面边界开始，并且在创建线程时，警戒区大小会在内部向上取整为系统页面大小。将警戒
// 区大小设置为 0 对于创建大量线程且确定不会发生栈溢出的应用程序来说，有助于节省内存。
// 如果线程在栈上分配大型数据结构，选择大于默认大小的警戒区可能对于检测栈溢出是必要的。
// 缺陷说明：截至 glibc 2.8，NPTL 线程实现将警戒区包含在栈大小分配之内，而不是像
// POSIX.1 要求的那样在栈末尾分配额外的空间。如果警戒区大小值太大，导致没有实际栈的空
// 间，这可能会导致 pthread_create(3) 返回 EINVAL 错误。过时的 LinuxThreads 实现则
// 做对了，它在栈末尾分配额外的空间作为警戒区。 ::
//
//      int pthread_attr_init(pthread_attr_t *attr);
//      int pthread_attr_destroy(pthread_attr_t *attr);
//      int pthread_attr_setdetachstate(attr, int detachstate);
//      int pthread_attr_getdetachstate(attr, int *detachstate);
//      int pthread_attr_setstacksize(attr, size_t stacksize);
//      int pthread_attr_getstacksize(attr, size_t *stacksize);
//      int pthread_attr_setstack(attr, void *stackaddr, size_t stacksize);
//      int pthread_attr_getstack(attr, void **stackaddr, size_t *stacksize);
//      int pthread_attr_setguardsize(attr, size_t guardsize);
//      int pthread_attr_getguardsize(attr, size_t *guardsize);
//      int pthread_attr_setscope(attr, int scope);
//      int pthread_attr_getscope(attr, int *scope);
//      int pthread_attr_setinheritsched(attr, int inheritsched);
//      int pthread_attr_getinheritsched(attr, int *inheritsched);
//      int pthread_attr_setschedpolicy(attr, int policy);
//      int pthread_attr_getschedpolicy(attr, int *policy);
//      int pthread_attr_setschedparam(attr, const struct sched_param *param);
//      int pthread_attr_getschedparam(attr, struct sched_param *param);
//
//      detachstate: PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_DETACHED
//      scope: PTHREAD_SCOPE_SYSTEM PTHREAD_SCOPE_PROCESS
//      inheritsched: PTHREAD_INHERIT_SCHED PTHREAD_EXPLICIT_SCHED
//      policy: SCHED_FIFO SCHED_RR SCHED_OTHER
//      struct sched_param { int sched_priority; } 0 ~ 99
//
// pthread_attr_setscope() 设置线程的竞争范围属性，竞争范围定义了一个线程为获取CPU
// 资源而与之竞争的线程集合。其中PTHREAD_SCOPE_SYSTEM表示所有进程中的线程都是对等的，
// 相当于它们基于调度策略和优先级对等的使用内核CPU。而PTHREAD_SCOPE_PROCESS线程只会
// 于进程内同样具有PTHREAD_SCOPE_PROCESS属性的线程竞争资源。POSIX.1要求实现至少支持
// 其中的一种竞争范围，Linux 支持 PTHREAD_SCOPE_SYSTEM，但不支持
// PTHREAD_SCOPE_PROCESS。在支持多种竞争范围的系统上，为了使
// pthread_attr_setscope()所做的参数设置在调用pthread_create()时生效，调用者必须使
// 用pthread_attr_setinheritsched()将属性对象的继承调度器属性设置为
// PTHREAD_EXPLICIT_SCHED。PTHREAD_SCOPE_SYSTEM 竞争范围通常表明一个用户空间线程直
// 接绑定到单个内核调度实体。在 Linux 系统中，过时的 LinuxThreads 实现和现代的 NPTL
// 实现都是如此，它们均为 1:1 线程实现（即一个用户空间线程对应一个内核线程）。POSIX.1
// 标准规定，默认的竞争范围由具体实现来定义。
//
// pthread_attr_setinheritsched() 函数将继承调度器属性设置为 inheritsched 指定的
// 值。继承调度器属性决定是从调用线程继承其调度属性，还是从 attr 中获取这些属性。以下
// 调度属性会受到该属性的影响：调度策略pthread_attr_setschedpolicy(3)、调度优先级
// pthread_attr_setschedparam(3) 和竞争范围 pthread_attr_setscope(3)。新初始化的
// 线程属性对象中，继承调度器属性的默认值是 PTHREAD_INHERIT_SCHED。
// pthread_attr_setinheritsched() 可能会因以下错误而失败：EINVAL：inheritsched 值
// 无效。ENOTSUP：POSIX.1 还记录了 pthread_attr_setinheritsched() 可能出现的一个
// 可选错误 ENOTSUP（“尝试将属性设置为不支持的值”）。缺陷说明：截至 glibc 2.8，如果
// 使用 pthread_attr_init(3) 初始化一个线程属性对象，那么该属性对象的调度策略会被设
// 置为 SCHED_OTHER，调度优先级会被设置为 0。然而，如果随后将继承调度器属性设置为
// PTHREAD_EXPLICIT_SCHED，那么使用该属性对象创建的线程会错误地从创建线程继承其调度
// 属性。如果在调用 pthread_create(3) 之前，在线程属性对象中显式设置了调度策略或调度
// 优先级属性，这个缺陷就不会出现。
//
// pthread_attr_setschedpolicy() 函数将调度策略属性设置为 policy。支持的值为
// SCHED_FIFO、SCHED_RR 和 SCHED_OTHER，其语义在 sched(7) 中有详细描述。
// 为了使 pthread_attr_setschedpolicy() 设置的策略在调用 pthread_create(3) 时生
// 效，调用者必须使用 pthread_attr_setinheritsched(3) 将继承调度器属性设置为
// PTHREAD_EXPLICIT_SCHED。pthread_attr_setschedpolicy() 可能会因以下错误而失败：
// EINVAL：policy 中的值无效。ENOTSUP：POSIX.1 还记录了
// pthread_attr_setschedpolicy() 可能出现的一个可选错误 ENOTSUP（“尝试将属性设置为
// 不支持的值”）。pthread_attr_setschedparam() 函数将调度参数属性设置为 param。
// 调度参数使用以下结构体：struct sched_param { int sched_priority; };关于每种调度
// 策略下允许的调度优先级范围的详细信息，请参阅 sched(7)。为了使
// pthread_attr_setschedparam() 设置的参数在调用 pthread_create(3) 时生效，调用者
// 必须使用 pthread_attr_setinheritsched(3) 将继承调度器属性设置为
// PTHREAD_EXPLICIT_SCHED。pthread_attr_setschedparam() 可能会因以下错误而失败：
// EINVAL：param 中指定的优先级对于当前的调度策略没有意义。ENOTSUP：POSIX.1 记录了
// pthread_attr_setschedparam() 可能出现的 ENOTSUP 错误。在 Linux 上，这个值永远
// 不会返回（但具有可移植性和前瞻性的应用程序仍然应该处理这个错误返回值）。
//
// 另外还有线程相关的CPU时间和调度函数： ::
//
// int pthread_getcpuclockid(pthread_t thread, clockid_t *clockid);
// int pthread_setschedprio(pthread_t thread, int prio);
// int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);
// int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param);
// nice
// getcpu
// getpriority
// setpriority
// sched_getcpu
// sched_setscheduler
// sched_setparam
// sched_getparam
// sched_get_priority_max
// sched_get_priority_min
// sched_rr_get_interval
// sched_yield
// sched_setaffinity
// sched_getaffinity
// sched_setattr
// sched_getattr
//
// 调度策略。调度器是内核组件，它决定下一个由 CPU 执行的可运行线程。每个线程都有一个
// 关联的调度策略和一个静态调度优先级 sched_priority。调度器根据系统中所有线程的调度
// 策略和静态优先级来做出决策。对于采用普通调度策略（SCHED_OTHER、SCHED_IDLE、
// SCHED_BATCH）调度的线程，sched_priority不用于调度决策（必须指定为 0）。采用实时
// 策略（SCHED_FIFO、SCHED_RR）调度的线程程，其 sched_priority 值的范围是 1（低）到
// 99（高）。如数字所示，实时线程的优先级总是高于普通线程。请注意：POSIX.1 要求实现仅
// 为实时策略支持至少 32 个不同的优先级级别，有些系统仅提供这个最小值。可移植的程序应
// 该使用 sched_get_priority_min(2) 和 sched_get_priority_max(2) 来查找特定策略支
// 持的优先级范围。从概念上讲，调度器为每个可能的 sched_priority 值维护一个可运行线
// 程列表。为了确定下一个运行的线程，调度器会查找具有最高静态优先级的非空列表，并选择
// 该列表头部的线程。线程的调度策略决定了它将被插入到具有相同静态优先级的线程列表中的
// 位置，以及它将如何在该列表中移动。所有调度都是抢占式的：如果一个具有更高静态优先级
// 的线程准备好运行，当前正在运行的线程将被抢占，并返回到其静态优先级级别的等待列表中。
// 调度策略仅决定具有相同静态优先级的可运行线程列表内的顺序。
//
// SCHED_FIFO先进先出调度，只能用于静态优先级高于0的情况，这意味着当一个SCHED_FIFO
// 线程变为可运行状态时，它将始终立即抢占任何当前正在运行的SCHED_OTHER、SCHED_BATCH
// 或SCHED_IDLE线程。SCHED_FIFO是一种简单的无时间片划分的调度算法。对于采用
// SCHED_FIFO策略调度的线程，适用以下规则：一个正在运行的SCHED_FIFO线程如果被另一个
// 更高优先级的线程抢占，它将停留在其优先级列表的头部，并在所有更高优先级的线程再次阻
// 塞后立即恢复执行。当一个阻塞的SCHED_FIFO线程变为可运行状态时，它将被插入到其优先级
// 列表的末尾。如果调用 sched_setscheduler(2)、sched_setparam(2)、
// sched_setattr(2)、pthread_setschedparam(3) 或 pthread_setschedprio(3) 更改由
// pid 标识的正在运行或可运行的 SCHED_FIFO 线程的优先级，对该线程在列表中位置的影响
// 取决于线程优先级的变化方向：如果线程的优先级提高，它将被放置在其新优先级列表的尾部。
// 因此，它可能会抢占当前正在运行的具有相同优先级的线程。如果线程的优先级不变，它在运行
// 列表中的位置不变。如果线程的优先级降低，它将被放置在其新优先级列表的头部。根据
// POSIX.1 - 2008，使用除 pthread_setschedprio(3) 之外的任何机制更改线程的优先级
//（或策略），应导致该线程被放置在其优先级列表的末尾。调用 sched_yield(2) 的线程将被
// 放置在列表的末尾。没有其他事件会使采用 SCHED_FIFO 策略调度的线程在具有相同静态优先
// 级的可运行线程等待列表中移动。一个 SCHED_FIFO 线程会一直运行，直到它因 I/O 请求而
// 阻塞、被更高优先级的线程抢占，或者调用 sched_yield(2)。
//
// SCHED_RR 轮转调度是 SCHED_FIFO 的简单增强。上述关于SCHED_FIFO的所有描述也适用于
// SCHED_RR，不同之处在于每个线程只允许运行最长一个时间片。如果一个 SCHED_RR 线程已
// 经运行了等于或超过时间片的时间段，它将被放置在其优先级列表的末尾。一个被更高优先级
// 线程抢占并随后作为运行线程恢复执行的 SCHED_RR 线程将完成其轮转时间片的未使用部分。
// 可以使用 sched_rr_get_interval(2) 来获取时间片的长度。
//
// SCHED_OTHER 默认的 Linux 分时调度。SCHED_OTHER 只能用于静态优先级为 0 的情况
//（即采用实时策略的线程的优先级总是高于 SCHED_OTHER 线程）。SCHED_OTHER 是标准的
// Linux 分时调度器，用于所有不需要特殊实时机制的线程。要运行的线程是从静态优先级为
// 0 的列表中根据动态优先级选择的，该动态优先级仅在这个列表内确定。动态优先级基于
// nice 值，并且对于线程准备运行但被调度器拒绝运行的每个时间片，动态优先级都会增加。
// 这确保了所有 SCHED_OTHER 线程的公平进展。在 Linux 内核源代码中，SCHED_OTHER 策
// 略实际上被命名为 SCHED_NORMAL。
//
// SCHED_BATCH 批量调度（从 Linux 2.6.16 版本开始支持）仅能在静态优先级为 0 的情况
// 下使用。该策略与 SCHED_OTHER 类似，都是根据线程的动态优先级（基于 nice 值）来调
// 度线程。不同之处在于，此策略会让调度器始终假定该线程是 CPU 密集型的。因此，调度器
// 在唤醒行为方面会施加一个小的调度惩罚，使得该线程在调度决策中略微处于劣势。这个策略
// 适用于非交互式的工作负载，且这些工作负载不想降低其 nice 值；同时也适用于那些希望
// 采用确定性调度策略，避免交互性导致额外抢占（在工作负载的任务之间）的工作负载。
//
// SCHED_IDLE：极低优先级作业调度（从 Linux 2.6.23 版本开始支持）仅能在静态优先级为
// 0 的情况下使用；进程的 nice 值对此策略没有影响。此策略旨在以极低的优先级运行作业，
// 甚至比采用 SCHED_OTHER 或 SCHED_BATCH 策略且 nice 值为 +19 的优先级还要低。
//
// nice 值是一个属性，可用于影响 CPU 调度器在调度决策中对某个进程的偏好或不偏好。它
// 会影响 SCHED_OTHER 和 SCHED_BATCH 线程的调度。可以使用 nice(2)、setpriority(2)
// 或 sched_setattr(2) 来修改 nice 值。根据 POSIX.1，nice 值是每个进程的属性；也
// 就是说，一个进程中的线程应该共享一个 nice 值。然而，在 Linux 上，nice 值是每个线
// 程的属性：同一进程中的不同线程可能有不同的 nice 值。nice 值的范围在不同的 UNIX
// 系统中有所不同。在现代 Linux 上，范围是 -20（高优先级）到 +19（低优先级）。在其他
// 一些系统上，范围是 -20 到 20。非常早期的 Linux 内核（Linux 2.0 之前）的范围是负
// 无穷到 15。nice 值对 SCHED_OTHER 进程相对调度的影响程度在不同的 UNIX 系统和不同
// 的 Linux 内核版本中也有所不同。随着 Linux 2.6.23 中 CFS 调度器的出现，Linux 采
// 用了一种算法，使得 nice 值的相对差异产生更强的影响。在当前实现中，两个进程的 nice
// 值每相差一个单位，调度器对优先级较高的进程的偏好程度就会增加 1.25 倍。这使得非常
// 低的 nice 值（+19）在系统上有任何其他更高优先级负载时，确实为进程提供很少的 CPU
// 资源，而高 nice 值（-20）则为需要的应用程序（例如，一些音频应用程序）提供大部分
// CPU 资源。在 Linux 上，RLIMIT_NICE 资源限制可用于定义非特权进程的 nice 值可以提
// 高的上限。
//
// nice 值与组调度。当调度非实时进程（即采用 SCHED_OTHER、SCHED_BATCH 和
// SCHED_IDLE 策略调度的进程）时，如果内核配置了 CONFIG_FAIR_GROUP_SCHED 选项（通
// 常都是如此），CFS 调度器会采用一种称为 “组调度” 的技术。在组调度模式下，线程会以
// “任务组” 为单位进行调度。任务组之间存在层次关系，其根节点是系统中的初始任务组，也
// 就是 “根任务组”。任务组在以下情况下形成：一个 CPU 控制组（cgroup）中的所有线程会
// 组成一个任务组。该任务组的父任务组是对应父 cgroup 的任务组。如果启用了自动分组功
// 能，那么所有（隐式地）被划分到同一个自动组（即通过 setsid(2) 创建的同一个会话）中
// 的线程会组成一个任务组。因此，每个新的自动组都是一个独立的任务组。根任务组是所有这
// 些自动组的父任务组。如果启用了自动分组功能，根任务组则由根 CPU cgroup 中未被隐式
// 划分到新自动组的所有进程组成。如果禁用了自动分组功能，根任务组则由根 CPU cgroup 中
// 的所有进程组成。如果禁用了组调度（即内核配置时未开启 CONFIG_FAIR_GROUP_SCHED），
// 那么系统中的所有进程理论上会被划分到同一个任务组中。在组调度模式下，一个线程的nice
// 值仅在与同一任务组中的其他线程进行调度决策时才会产生影响。这与 UNIX 系统中 nice
// 值的传统语义相比，会产生一些令人意外的结果。特别是，如果启用了自动分组功能（这在各
// 种发行版中是默认设置），那么对某个进程使用 setpriority(2) 或 nice(1) 命令，其影
// 响仅体现在与同一会话（通常是同一个终端窗口）中执行的其他线程的调度关系上。相反，对
// 于分别处于不同会话（例如不同的终端窗口，每个窗口中的作业都与不同的自动组相关联）中
// 的两个 CPU 密集型进程，修改其中一个会话中进程的 nice 值，在调度器的决策中，对另一
// 个会话中的进程没有任何影响。这里有一个可能有用的解决方法，即使用如下命令修改终端会
// 话中所有进程的自动组 nice 值：echo 10 > /proc/self/autogroup。
//
// 线程取消机制。在通常情况下，程序中的多个线程会并发执行，每个线程各司其职直至其决意
// 退出，随即会调用函数pthread_exit()或者从线程启动函数中返回。有时候需要将一个线程
// 取消，即向线程发送一个请求要求其立即退出。比如一组线程正在执行一个运算一旦某个线程
// 检测到错误发生需要其他线程退出，取消线程的功能这时就派上用场。还有一种情况，一个由
// 图形界面驱动的应用程序可能会提供一个“取消”按钮，以便用户可以终止后台某个线程正在执
// 行的任务。这种情况下，主线程（控制图形界面）需要请求后台线程退出。 ::
//
//      int pthread_cancel(pthread_t thread);
//      int pthread_setcancelstate(int state, int *oldstate);
//      int pthread_setcanceltype(int type, int *oldtype);
//      void pthread_testcancel(void);
//      void pthread_cleanup_push(void *(routine)(void *), void *arg);
//      void pthread_cleanup_pop(int execute);
//
//      state:  PTHREAD_CANCEL_DISABLE PTHREAD_CANCEL_ENABLE
//      type:   PTHREAD_CANCEL_ASYNCHRONOUS PTHREAD_CANCEL_DEFERED
//
// 发出取消请求后，函数pthread_cancel()会立即返回，不会等待目标线程的退出。准确地说，
// 目标线程会发生什么，何时发生，这都取决于线程取消状态（state）和类型（type）。函数
// pthread_setcancelstate() 和 pthread_setcanceltype() 会设定标志，允许线程对取消
// 请求的响应过程加以控制。函数 pthread_setcancelstate() 会将调用线程的取消性状态置
// 为参数state所给定的值。其中 PTHREAD_CANCEL_DISABLE 设置线程不可取消，如果此类线
// 程收到取消请求则会挂起取消请求，直至将取消状态置为启用。PTHREAD_CANCEL_ENABLE
// 设置线程可以取消，这是新建线程取消性状态的默认值。而参数 oldstate 可以获取前一状态
// 的值，如果对前一状态没有兴趣，Linux 允许将oldstate置为NULL。不过SUSv3并没有规范
// 这一特性，所以要保证应用的可移植性，就不能依赖这一特性，应该总是为oldstate设置一个
// 非NULL的值。如果线程执行的代码片段需要不间断地一气呵成，那么临时屏蔽线程的取消行状
// 态就变得很有必要。
//
// 如果线程的取消行状态为启用状态，那么对取消请求的处理则取决于线程的取消性类型，该类
// 型可以通过调用函数 pthread_setcanceltype() 进行设定。其中
// PTHREAD_CANCEL_ASYNCHRONOUS 可能会在任何时点（也许是立即取消，但不一定）取消线程。
// 异步取消的应用场景很少，后面单独介绍。另一个值 PTHREAD_CANCEL_DEFERED 将取消请求
// 保持在挂起状态，直至到达取消点（cancellation point），这也是新建线程的缺省类型。
// 当某线程调用fork()时，子进程会继承调用线程的取消性类型及状态。而当某线程调用exec()
// 时，会将新程序主线程的取消性类型及状态分别重置为 PTHREAD_CANCEL_ENABLE 和
// PTHREAD_CANCEL_DEFERRED。
//
// 若将线程的取消性状态和类型分别置为启用和延迟，仅当线程抵达某个取消点时，取消请求才
// 会起作用。取消点即是对由实现定义的一组函数之一加以调用。SUSv3规定，实现若提供了下
// 表所列的函数，则这些函数必须是取消点。其中的大部分函数都有能力将线程无限期地阻塞起
// 来。 ::
//
//      accept aio_suspend clock_nanosleep close connect creat fcntl(F_SETLKW)
//      fsync fdatasync getmsg getpmsg lockf(F_LOCK) mq_receive mq_send
//      mq_timedreceive mq_timedsend msgrcv msgsnd msync nanosleep open
//      pause poll pread pselect pthread_cond_timedwait pthread_cond_wait
//      pthread_join pthread_testcancel putmsg putpmsg pwrite read readv
//      recv recvfrom recvmsg select sem_timedwait sem_wait send sendmsg
//      sendto sigpause sigsuspend sigtimedwait sigwait sigwaitinfo sleep
//      system tcdrain usleep wait waitid waitpid write writev
//
// 除这些函数外，SUSv3还指定了大量函数，系统实现可以将其定义为取消点。其中包括 stdio
// dlopen syslog ntfw popen semop unlink，以及从诸如 utmp 之类的系统文件中获取信
// 息的各种函数。可移植应用程序必须正确处理这一情况：线程在调用这些函数时有可能遭到取
// 消。SUSv3规定，处理上述两组必须或可能是取消点的函数之外，不得将标准中的任何其他函数
// 视为取消点，即调用这些函数不会招致线程取消，可移植程序加以处理。SUSv4在必须的可取消
// 函数列表中增加了openat，并移除了函数sigpause和usleep。系统实现可随意将标准并未规范
// 的其他函数标记为取消点。任何可能造成阻塞的函数，有可能是因为需要访问文件，都是取消
// 点的理想候选对象。处于这一理由，glibc将其中的许多非标准函数标记为取消点。
//
// 线程一旦收到取消请求，且启用了取消状态并将类型置为延迟，则会在下次抵达取消点时终止。
// 如果该线程尚未分离（detached），那么为防止其编程僵尸线程，必须由其他线程对其进行连
// 接（join）。连接之后，返回至函数 pthread_join() 中第二个参数的值将是一个特殊值，
// PTHREAD_CANCED。
//
// 如果一个需要取消的线程，其执行的代码并没有调用存在取消点的函数，例如计算密集型循环，
// 这时线程永远也不会响应取消请求。函数 pthread_testcancel() 的目的很简单，就是产生
// 一个取消点。线程如果已有处于挂起状态的取消请求，那么只要调用该函数，线程就会随之终
// 止。当线程执行的代码未包含取消点时，可以周期性地调用 pthread_testcancel()，以确保
// 对其他线程向其发送的取消请求做出及时响应。
//
// 一旦有取消请求处于挂起状态，线程在执行到取消点时如果只是草草收场，这会将共享变量以及
// pthreads对象，例如互斥量置于一种不一致状态，可能导致进程中其他线程产生错误结果、死
// 锁，甚至造成程序崩溃。为规避这一问题，线程可以设置一个或多个清理函数，当线程遭到取消
// 时会自动运行这些函数，在线程终止之前可执行诸如修改全局变量，解锁互斥量等动作。每个
// 线程都可以拥有一个清理函数栈，当线程遭取消时，会该栈自顶向下依次执行清理函数，首先
// 会执行最近设置的函数，接着时次新的函数，依次类推。当执行完所以清理函数后，线程终止。
// 函数 pthread_cleanup_push() 和 pthread_cleanup_pop() 分别负责向调用线程的清理
// 函数栈添加和移除清理函数。push 时还传入了一个 arg 参数，这个参数在清理函数调用时当
// 作参数传给清理函数。 **注意**，若线程因调用 pthread_exit() 而终止，则也会自动执行
// 尚未从清理函数栈中弹出的清理函数，线程正常返回（return）则不会执行清理函数。尽管这
// 里把 pthread_cleanup_push() 和 pthread_cleanup_pop() 描述为函数，SUSv3 却允许
// 将它们实现为宏，可展开为分别由 { 和 } 所包裹的语句序列。并非所有的 UNIX 都这样做，
// 不过包括 LINUX 在内的很多系统都是使用宏来实现的。由于这个限制必须在线程相同的作用
// 域中已匹配对的形式调用这两个函数，pthread_cleanup_push 的宏定义可以包含字符 {，
// 这种情况下，在 pthread_cleanup_pop 的定义中会包含对应的匹配字符 }。
//
// 如果设定线程为可异步取消时（PTHREAD_CANCEL_ASYNCHRONOUS），可以在任何时点将其取
// 消（即执行任何机器指令时），取消动作不会拖延到下一个取消点才执行。异步取消的问题在
// 于，尽管清理函数依然会得以执行，但处理函数却无从得知线程的具体状态。清理函数无法知
// 道将在哪里发生取消动作，或者准确的说，清理函数不清除需要执行哪些清理步骤。此外，由
// 于可以在任意点都可能被取消，线程很可能还处于某种未知状态，例如一个动态分配的函数本
// 来是需要在清理函数中清理的，但可能在执行malloc()期间就被取消了，这极有可能造成后续
// 的混乱。作为一般性原则，可异步取消的线程不应该分配任何资源，也不能获取互斥量或锁，
// 这导致大量库函数无法使用，其中就包括pthreads函数的大部分。SUSv3中有3各例外是
// pthread_cancel() pthread_setcancelstate() pthread_setcanceltype()，规范明确
// 要求将它们实现为异步取消安全。换言之，异步取消功能鲜有应用场景，其中之一是取消在执
// 行计算密集型循环的线程。
//
// 综上所述，函数 pthread_cancel() 允许某线程向另外一个线程发送取消请求，要求目的线
// 程终止。目标线程如何响应，取决于其取消性状态和类型。如果禁用线程的取消性状态，那么
// 会挂起（pending）取消请求，直至将线程的取消性状态置为启用。如果启用取消状态，那么
// 线程何时响应则依赖于取消性类型。若类型为延迟取消，则在线程下一次调用某个取消点函数
// 时进行取消。如果为异步取消类型，取消动作随时可能发送。线程可以设置一个清理函数栈，
// 其中的清理函数属于有开发人员定义的函数，当线程遭到取消时，会自动调用这些函数以执行
// 清理工作，例如恢复共享变量状态或解锁互斥量。
//
// 线程实现模型，实现线程API的三种不同模型，其实现差异主要集中在线程如何与内核调度实体
// （KSE, Kernel Scheduling Entity）相映射。KSE是内核分配CPU以及其他系统资源的单位，
// 在早于线程而出现的传统UNIX中，KSE等同于进程。
//
// 多对一（M:1）实现，用户级线程。在M:1线程实现中，关乎线程创建、调度以及同步的所有细
// 节全部由进程内用户空间的线程库来处理。对于进程中存在的多个线程，内核一无所知。M:1
// 实现的优势不多，其中最大的有点在于，许多线程操作速度都很快，因为无需切换到内核模式。
// 此外由于线程库无需内核支持，所以M:1实现在系统间的移植相对要容易一些。不过M:1实现也
// 存在一些严重缺陷：当一线程发起系统调用比如read()时，控制由用户空间的线程库转交给内
// 核，这就意味着如果read()调用遭到阻塞，那么所有的线程都会被阻塞。内核无法调度进程中
// 的这些线程，因为内核并不知道进程中存在这些线程，也就无法在多处理器平台上将各线程调
// 度给不同的处理器。另外，也不可能将一进程中某线程的优先级调整为高于其他进程的中的线
// 程，这是没有一样的，因为对线程的调度完全在进程中处理。
//
// 一对一（1:1）实现，内核级线程。在1:1线程实现中，每一线程映射为以恶单独的KSE。内核
// 分别对每个线程做调度处理。线程同步操作通过内核系统调用实现。1:1实现消除了M:1实现的
// 种种弊端。遭阻塞的系统调用不会导致进程的所有线程被阻塞，在多处理硬件平台上，内核还
// 可以将进程中的多个线程调度到不同的CPU上。不过，因为需要切换到内核模式，所以诸如线程
// 创建、上下文切换以及同步操作就需要慢一些。另外，为每个线程分别维护一个KSE也需要开
// 销，如果应用程序包含大量线程，则可能对内核调度造成严重的负担，降低系统的整体性能。
// 尽管有这些缺点，1:1实现通常更胜于M:1实现，LinuxThreads和NPTL都采用1:1模型。在NPTL
// 的开发期间，为了使得包含数千计数的进程得以高效运行，投入了巨大的努力。对内核调度进行
// 了重写并设计了新的线程实现。
//
// 多对多（M:N）实现，两级模型。M:N实现旨在结合1:1和M:1模型的优点，避免二者的缺点。在
// M:N模型中，每个进程都可拥有多个与之相关的KSE，并且也可以把多个线程映射到一个KSE。
// 这种设计运行内核将同一应用的线程调度到不同的CPU上运行，同时也解决了随线程数量大而放
// 大的性能问题。M:N模型的最大问题是过于复杂，线程调度任务由内核及用户空间的线程库共同
// 承担，二者之间势必要进行分工协作和信息交换。在M:N模型下，按照SUSv3标准要求来管理信
// 号也极为复杂。最初增考虑采用M:N模型来实现NPTL线程库，但若要保证Linux调度器即使在处
// 理大量KSE的情况下也能应对自如，则需要对内核所作的改动范围过大，可能也没有必要，故而
// 否决了这一方案。
//
// 针对pthreads API，Linux下由两种实现：LinuxThreads 这是最初的Linux线程实现，NPTL
// （Native POSIX Threads Library）这是Linux线程实现的现代版，以取代LinuxThreads。
// NPTL的性能优于LinuxThreads，也更符合SUSv3的pthreads标准。对NPTL的支持需要修改内
// 核，这始于Linux 2.6。值得强调的是，LinuxThreads实现已经过时，并且glibc从2.4版本
// 开始也已不再支持它，所有新的线程库开发都基于NPTL。
#define _GNU_SOURCE
#include "direct/thrd.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

#define magic_thrd_debug 1

#define magic_thrd_failure  0x01
#define magic_thrd_started  0x02
#define magic_thrd_joined   0x04

#define defer_if(if_true_goto_defer) if (if_true_goto_defer) { goto label_defer; }
#define prerr_if(if_true_print_err, error, ...) if (if_true_print_err) { prerr(error, ## __VA_ARGS__); }
typedef struct thrds *thrds;

enum ethrd {
    ethrd_invpara,
    ethrd_start,
    ethrd_attrinit,
    ethrd_setstack,
    ethrd_attrdetr,
    ethrd_getattr,
    ethrd_join,
};

void prerr(int error, ...)
{
    va_list args;
    va_start(args, error);
    switch (error) {
        case ethrd_invpara: {
            struct thrds *t = va_arg(args, struct thrds*);
            if (t->count >= t->capacity) {
                printf("Error: thread capacity reached\n");
            } else {
                printf("Error: provided func is NULL\n");
            }
            break; }
        case ethrd_start:
            printf("Error: thread start failed %d\n", va_arg(args, int));
            break;
        case ethrd_attrinit:
            printf("Error: thread attr init failed %d\n", va_arg(args, int));
            break;
        case ethrd_setstack: {
            int n = va_arg(args, int);
            int stacksize = va_arg(args, int);
            printf("Error: thread setstack failed %d size %d\n", n, stacksize);
            break; }
        case ethrd_attrdetr:
            printf("Error: thread attr destroy failed %d\n", va_arg(args, int));
            break;
        case ethrd_getattr:
            printf("Error: getattr failed %d\n", va_arg(args, int));
            break;
        case ethrd_join:
            printf("Error: thread join failed %d\n", va_arg(args, int));
            break;
        default:
            break;
    }
    va_end(args);
}

struct thrditem {
    union { pthread_t tid; int stacksize; } u;
    thrdproc func;
    int flags;
    int id;
};

thrds threads_init(int mainid, int maxthreads, int sizethrds)
{
    assert(maxthreads > 0);

    if (sizethrds == 0)
        sizethrds = sizeof(struct thrds);

    assert(sizethrds >= (int)sizeof(struct thrds));
    assert(sizethrds % sizeof(void *) == 0);

    size_t alloc = sizethrds + maxthreads * sizeof(void*);
    struct thrds *t = (struct thrds *)malloc(alloc);
    memset(t, 0, alloc);

    t->item = (void **)((char *)t + sizethrds);
    t->maxthreads = maxthreads;

    t->item[0] = (void *)(intptr_t)mainid; // tmp save
    return t;
}

#define multptr(n) (((n) + sizeof(void *) - 1) & (~(sizeof(void *) - 1)))
#define mult_16(n) (int)(((unsigned)(n) + 15) & (~(unsigned)15))

struct thrd *threads_create(thrds t, thrdproc func, int stacksize, int thrdsize)
{
    assert(sizethrd >= 0);

    if (t->count >= t->maxthreads) {
        return NULL;
    }

    size_t alloc = multptr(sizeof(struct thrditem) + thrdsize);
    struct thrditem *item = (struct thrditem *)malloc(alloc);
    memset(item, 0, alloc);

    if (t->count == 0) {
        item->id = (int)(intptr_t)t->item[0] + 1;
    } else {
        item->id = (((struct thrditem *)(t->item[t->count - 1]))->id + 1;
    }

    item->func = func;
    item->u.stacksize = stacksize;

    t->item[t->count] = item;
    t->count += 1;

    return (struct thrd *)(item + 1);
}

struct thrd *threads_get(thrds t, int index)
{
    assert(index >= 0 && index < t->count);
    struct thrditem *item = (struct thrditem *)t->time[index];
    return (struct thrd *)(item + 1);
}

#define getthrditem(t) ((struct thrditem *)((char *)t - offsetof(struct thrditem, id)))

int thrd_get_id(struct thrd *thrd)
{
    if (thrd == NULL)
        return 0;
    return getthrditem(thrd)->id;
}

thrdproc dealthreadstart(struct thrditem *item)
{
#if magic_thrd_debug
    pthread_t tid = pthread_self();
    void *stack_addr = NULL;
    size_t stack_size = 0, guard_size = 0;
    pthread_attr_t attr;
    bool destroy = false;
    int n = 0;

    n = pthread_getattr_np(tid, &attr);
    defer_if(n != 0) else { destroy = true; }

    n = pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    defer_if(n != 0);

    n = pthread_attr_getguardsize(&attr, &guard_size);

label_defer:
    if (destroy) {
        pthread_attr_destroy(&attr);
    }
    if (n != 0) {
        prerr(ethrd_getattr, n);
    }
    printf("[thread %d] stack %p size %d-byte, guard size %d-byte\n",
        item->id, stack_addr, (int)stack_size, (int)guard_size);
#endif
    return item->func;
}

static void *threadstartproc(void *arg)
{
    dealthreadstart((struct thrditem *)arg)(((struct thrditem *)arg) + 1);
    return NULL;
}

int threadstacksize(long stacksize)
{
    if (stacksize <= 0) return 0;
    // https://www.man7.org/linux/man-pages/man3/sysconf.3.html
    // https://www.man7.org/linux/man-pages/man3/sysconf.3p.html
    // 从sysconf获得的值是系统配置常量，在进程的生命周期内不会改变。
    // _SC_PAGESIZE: Size of a page in bytes. Must not be less than 1.
    // _SC_THREAD_STACK_MIN: minimal pthread stack size (16384B/16KB)
    long pagesize = sysconf(_SC_PAGESIZE);
    long minsize = sysconf(_SC_THREAD_STACK_MIN);
    if (minsize > 0 && stacksize < minsize) {
        stacksize = minsize;
    }
    if (pagesize > 0) {
        long times = (stacksize + pagesize - 1) / pagesize;
        stacksize = pagesize * times;
    }
    return mult_16(stacksize); // stack align 16-byte
}

bool pthreadcreate(struct thrditem *item, int stacksize)
{
    pthread_t *tid = &item->u.tid;
    pthread_attr_t attr;
    bool created = false;
    int n;

    stacksize = threadstacksize(stacksize);
    if (stacksize <= 0) {
        n = pthread_create(tid, NULL, threadstartproc, item);
        prerr_if(n != 0, ethrd_start, n) else { created = true; }
        return created;
    }

    n = pthread_attr_init(&attr);
    if (n != 0) {
        prerr(ethrd_attrinit, n);
        return NULL;
    }

    n = pthread_attr_setstacksize(&attr, stacksize);
    prerr_if(n != 0, ethrd_setstack, n, stacksize);

    n = pthread_create(tid, &attr, threadstartproc, item);
    prerr_if(n != 0, ethrd_start, n) else { created = true; }

    n = pthread_attr_destroy(&attr);
    prerr_if(n != 0, ethrd_attrdetr, n);

    return created;
}

void threads_start(thrds t)
{
    struct thrditem *item;
    for (int i = 0; i < t->count; i += 1) {
        item = (struct thrditem *)t->item[i];
        if (item == NULL || item->flags) {
            continue;
        }
        if (pthreadcreate(item, item->stacksize)) {
            item->flags = magic_thrd_started;
        } else {
            item->flags = magic_thrd_failure;
        }
    }
}

void threads_join(thrds *p)
{
    struct thrds *t = *p;
    void *retv = NULL;

    if (t == NULL) {
        return;
    }

    for (int i = 0; i < t->count; i += 1) {
        struct thrditem *item = (struct thrditem *)t->item[i];
        if (item == NULL || !(item->flags & magic_thrd_started)) {
            continue;
        }

        int n = pthread_join(item->tid, &retv);
        prerr_if (n != 0, ethrd_join, n);

        item->flags = magic_thrd_joined;

#if magic_thrd_debug
        if ((intptr_t)retv == (intptr_t)PTHREAD_CANCELED) {
            printf("[thread %d] canceled join\n", item->id);
        } else {
            printf("[thread %d] join retval %d\n", item->id, (int)(intptr_t)retv);
        }
#endif

        free(t->item[i]);
        t->item[i] = NULL;
    }

    free(t);
    *p = NULL;
}
