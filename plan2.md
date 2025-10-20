### 通用框架

Qt 的 QGraphicsView / QGraphicsScene / QGraphicsItem 提供了一层十分好的结构关系，即摄像头 / 场景 / 物体，没有比这三者更适合来搭建核心框架的了。QGraphicsScene 提供了一个无限大的逻辑平面，可以在其中添加 QGraphicsItem，通过 addItem，removeItem。QGraphicsView 可以绑定到一个 QGraphicsScene 并指定显示一块矩形区域（内部逻辑是检测 QGraphicsScene 内这一块区域内的 QGraphicsItem 并调用其 paint）。QGraphicsScene 提供了一套 QGraphicsItem 的树结构关系（不清楚是否基于 QObject 的树结构关系），一个特性是当 parent 被移出场景时 child 也会被移除。

一些核心想法与思考（乱序）:

- QGraphicsItem 没有提供类似 UE 的 Actor 内组件系统，只有 QGraphicsScene 的一套树结构。目前实现中暂时将组件的概念暂时以这一套树结构替代。To be continued。

- 将玩家操控的角色与玩家的输入二者分离为 Character 和 Player Controller。Player Controller 接受输入信号并（在子类中）根据特定的逻辑来决策、组织 Character 的行为，Character 刻画的便是一组描述角色固有属性以及角色与其他物体的交互属性的谓词。比如 Character 的 ”（可以）向前移动“ 可以被 Player Controller 的 ”按键 W 被持续按下“ 触发。分离二者的一个好处可以实现在不同角色之间的灵活控制权切换。不过同时也使角色控制器与角色之间相互访问、交流数据变得麻烦，这对于耦合度高的一些操作而言是十分不利的。

  注意 Player Controller 不应该直接修改任何 Character 的属性，而是需要通过调用 Character 提供的 “尝试做某事” 的函数来实现对 Character 的修改（因为比如需要支持关卡暂停）。比如 Character 提供 “尝试向前移动” 供 Player Controller 调用，那么 Character 。另一方面，Character 为 Player Controller 提供的函数也不应该，比如直接修改自身的位置（因为存在碰撞），而是需要通过修改速度/加速度这样的描述“尝试移动”的自身属性来实现移动。然后再自身的 advance 函数中尝试兑现 Player Controller 请求。

- 考虑一个涉及多方的处理：在 QLink 中角色碰到时道具需要根据其种类对”游戏“施加各种效果：如生成提示、增加游戏时间、反转输入等。涉及角色、道具（场景内物体）、提示（场景内物体）、游戏全局量、玩家控制器多个主体。目前的实现中，我将这个操作完全放在了 Matcher Controller（Matcher 继承自 Character，Matcher Controller 继承自 Player Controller），实际上这是一个十分糟糕的决定。代替以上糟糕设计的解决方案：

  1. 应该还有一层抽象将特定的 Player Controller、Character、Level、Other Object整合起来，称之为 Game Instance。它是所有“特定游戏逻辑处理”的类的基类。所有涉及特定的多方交互的逻辑都应该对应的特定 Game Instance 的子类中。为了提供这样的可能性，以 Match Quest Game Instance 为例，其应当可以访问 Matcher，Matcher Controller，Tile Level 这些特定类的相关元素，这些元素很多时候下应当为 non-public，所以在基于 C++ 语法特性的限制，我们只能将 Match Quest Game Instance 设定为他们的友元[^1]。
	  
	  交互通常是双向的，比如 Matcher 与 Tile 的碰撞，这样的交互应当由 Matcher 还是 Tile 触发？或许规定一个统一的触发者特征（比如主动）可以决定。
	  交互是立即响应还是生成一个响应事件？一些简单的分析：立即响应通常比较方便。响应事件可以适用于一些会删除触发者的

- 在现有实现中没有 Camera 这一层抽象，Player Controller 直接维护一个 QGraphicsView，这么做属实是不大合适。Camera 应当作为 Level 中的 Actor，内部维护一个 QGraphicsView，默认绑定到当前 Level，且指定一块以其坐标为中心的矩形区域。一个 Character 下可以有多个 Camera 作为子物体，但是必须指定一个默认的 Camera。

- 关卡内的物体（Actor）按照是否可以被 Player Controller 操控可以分为角色（Character）和其他物体（Non-Character Actor）。一个 Level 可以有多个 Player Controller。将 Player Controller 绑定到 Level 中时应为其提供一个默认的 Character，并进行对应的初始化。Level 是独立于 Player Controller 的存在，可以在不绑定 Player Controller 的情况下 Open Level，也可以创建 Player Controller 但是不绑定关卡。这一点在现有实现中做的并不是很好，因为我搞了一个 Start Memu Level 的空关卡。此外注意 Player Controller 未绑定至关卡时，其转发的来自 Window 的输入不应当被传递到 Character，这一点可以通过向子类提供一个封装处理后的接口实现（如在由 Window 的 onKeyPressed 事件触发的 Player Controller 的 onKeyPressed 中判断是否绑定，如果绑定了 Character 则调用 tachedOnKeyPressed，前者是 private，后者是 protected virtual）。

- 在现有实现中：Window 控制一个唯一的 Player Controller 和一个唯一的 Level；Player Controller 保存了 Window 的指针，用于显示 UI。这样的设计有一定瑕疵。进一步的构想：- 
	
	- Game Core 维护当前 Level、Player Controller、Window 之间的关系。
	
	- 从窗口到 Player Controller 是一个映射（即不能对应到多个 Player Controller）。输入事件的都源自于（焦点）窗口，窗口应当将输入事件转发到 Player Controller。可以实现窗口同玩家多开。

	- Player Controller 与 Camera 是一一对应的。Player Controller 中应当不涉及任何有关 Window 的操作。Window 在显示画面时应当从 Player Controller 获取 Camera，而不是 Player Controller 在绑定到 Character 时绑定窗口到 Character 指定的 Camera，这样 Player Controller 在切换 Character 时无需直接操作 Window。

	- Player Controller 管理所有 UI 界面。Window 从 Player Controller 获取所有应该显示的 UI 界面并绘制。

- 在现有实现中，UI 类都自己实现了绘制（直接调用 Qt 库）。需要增加一层抽象？如果增加的话，为了维持一致性，应当把 Level（基于 QGraphicsScene）和 Actor（基于 QGraphicsItem）也做一层抽象以及 Camera（基于 QGraphicsView）

- 在现有实现中，游戏的主循环是一个 Qt 实现的事件队列。

	- Window 的输入是一个事件，会被 Qt 的事件队列直接调用。
	- Level 的更新 advance 也是一个事件，在 Level 在构造函数中利用 Qt 的 QTimer 定时调用，在主循环中被 Qt 的事件队列直接调用。Level advance 中调用所有 Level 内的 Actor（包括 Character）的 advance 以及 Player Controller 的 advance。
	- Window 输入、Level 更新、Window 绘制作为三个独立的事件，决定了他们之间是可以 interleave 的，这与正常的游戏主循环的按顺序 input update draw 的
	- Actor 的某些特定行为也采用 QTimer / QTimeline 触发，造成结构破坏
	
	更加正确的做法应该是：
	
	- Game Core 的利用定时器调用更新事件，分别传递给 Player Controller、Level，Level 再传递给内部 Actor，Player Controller 再传递给内部 UI。

	- Actor 如果调用 QTimer/QTimeline 会越过 Level 直接被触发。目前实现中是为 Actor 增加了一个 pause 机制，在 Level 被暂停时会调用所有 Actor 的 pause，特殊的调用 QTimer/QTimeline 的 Actor 需手动重写 pause 函数。这个实现方法过于丑陋。如果需要支持 Actor 计时对于 Level 的局部性，需要手动实现一个与关卡有关的计时器（顺便提一嘴 QTimer 只有 start 没有 resume 也是一个莫名其妙的设计，似乎用意在于限制仅用 QTimer 实现高频的触发，在这样的情况下 resume 和 start 的时间差很小可以被忽略。QTimeLine 疑似就是采用 25ms 触发的类似 QTimer 的机制来实现的“假连续”）。Player Controller 和 UI 中存在类似的问题。此外 Player Controller 在关卡暂停时也应当阻塞对 Character 的控制。

	- Level 的 "advance" 和 Player Controller 的 "advance" 在向下一层传递时均需要进行一层“暂停判断”封装。这一层封装也为后续相关功能的添加提供了一定程度上的可拓展性。

- 对象的删除机制。很显然并不能在 Game Core 触发的主循环的某一帧更新中直接 delete 某一个对象（某种程度上并不是很显然），因为同一帧中可能会有很多东西相互引用，贸然删除某个物体（且不加以维护对应的引用关系）很可能会导致莫名其妙的 SEGMENTATION  FAULT。比如我在写 Matcher 碰到 Tile 交互中，交互的响应方式为立即响应，交互的一种结果是 Tile 被删除，Matcher 和 Tile 的交互是在 Matcher 的 advance 中碰撞检测到后进行的。由于 Level 传递 advance 的机制是事先先收集所有场景内物体到一个列表然后遍历调用，如果直接 delete 掉某个交互的 Tile 那么这个列表中这个 Tile 的指针就会失效，导致潜在的 Segmentation Fault。这个傻逼的 Bug 还不是稳定触发的，取决于原有栈空间是否被破坏、Tile 的遍历顺序等，debug 了我一个多小时。正确的方式应当是延迟到如当前帧的末尾删除。
	
	- 对于 Actor 而言，其是可以独立于 Level 存在的（可以吗？可以），所以 Actor 的删除应在框架中的层次应当属于底层的资源管理，不由 Level 控制，Actor 删除时需要维护其 QObject 树结构关系（所有继承 QObject 都需要维护，通常而言 Qt 重写的 delete 会自动做这一份工作）、Level 中 QGraphicsScene 的 BSP 数据结构以及自定义的数据关系（所有继承类都有着一份义务，这是用户的职责，与框架提供者无关）。
	- Character 本身是 Actor，此外还需要额外维护与 Camera、Player Controller 的关系。
	- 对于 Player Controller 而言，需要维护其与 Level、Character、UI、Window 之间的关系。
	- 对于 Camera 而言，需要维护其与内部的 QGraphicsView （以及与 Window 之间的绑定，有吗？似乎可以没有）。
	- 对于 UI 而言，需要维护与对应的 Player Controller 的关系。
	
	实现删除的一种实现方式是注册一个 defferedDelete 事件，无论对象在这个框架中的身份是什么这都是可以的。

- 碰撞本身作为物理模拟（这意味着需要需要实现力、运动、质量等概念）的一部分存在，而后者应当作为一个集成在这一套框架之中的机制。目前实现中只在 Matcher（继承 Character 表示 Match Quest 中的角色）中特化处理了其碰撞，如果与 Tile 将要发生碰撞并且不触发删除那么不执行此次移动。
  
  自身碰撞体积通过重写 QGraphicsItem 的 boudning / shape 来实现（分别为 QRect 描述的外接矩形和 QPainterPath 描述的精细形状），判断碰撞通过 QGraphcisItem 的 collidingItems 和 collidesWithItem实现（前者通过调用 QGraphicsScene 的 BSP 树来缩减判定范围），碰撞判定每帧在 advance 的 phase = 1 阶段进行。在此基础之上需要实现一套物理引擎。To be continued。

- 对象之间的数据传递。似乎可以视为一种只读的 “交互” 从而归入以上讨论。To be continued。
### 特化机制

To be continued。

---
[^1]：参见 [[Diary/2025/October/10.19|10.19]] 日寄中对 C++ 成员访问权限限制机制的缺点的讨论