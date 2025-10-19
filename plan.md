需要实现的核心功能包括
- 开始界面
  - 界面 UI
  - 开始新游戏机制
  - 载入机制（保存、加载）
  - 退出机制
- 游戏内
  - 全局机制（暂停，分数，计时，结束）
  - 地图生成
  - 人物移动
  - 人物与箱子的交互
  - 箱子消除
  - 人物与道具互动
  - 道具机制

### 场景架构
采用 Qt 的 `QGraphicsView`/`QGraphicsScene`/`QGraphicsItem` 这一套框架来实现游戏内的场景。

HUD 采用独立的 `QWidget`，不作为 `QGraphicsView` 的某子控件（镜头的晃动不应该影响 HUD），单独地挂载在 `MainWindow` 下进行实现。

类 `Level` 继承 `QGraphicsScene` 代表一个场景。游戏内一共有三个场景 Start Menu, Single Player, Double Player，Start Menu 直接继承 `Level`；两种 Player 模式与 Level 之间还有一层 `TileLevel`，其实现了游戏内的全局机制、网格地图、部分道具功能、部分玩家功能。两种 Player 模式的 `TileLevel` 子类特化了道具类别、玩家数量（输入）等细节。

一个 Level 包含两个要素，玩家（HUD 附着玩家存在）和地图。游戏虽然有双人模式，但是实际上时本地的共享窗口的双人，所以在某种意义上还是单人，只算一个玩家。如果不考虑兼容多窗口的拓展实现的话，可以一个 `Level` 恰好包含一个 `Character*` 的成员 `puppet`。考虑到有多态的可能性，二者均为指针。

类 `PlayerController` 继承 `QObject` 代表一个玩家。这里将玩家控制器（`PlayerController`）与玩家所操纵的角色（`Character`）进行解耦。`PlayerController` 应当与游戏窗口一一对应，接受来自窗口的输入信号并组织所操控角色的行为。`PlayerController` 有一个所操纵的角色的指针 `Character* puppet`，一组 HUD `QList<UserInterface*> hudList`，以及一个 `QGraphicsView* camera`。`camera` 和 `hudList` 分别独立地挂载到窗口的根控件 `MainWindow` 下。

类 `UserInterface` 继承 `QWidget` 代表一个 HUD。一个 HUD 直接与一个窗口关联。暂时确定游戏中共有七种控件：开始界面，选择/输入地图大小界面，加载的选择界面，游戏内两种模式对应的 HUD，游戏结束，保存的选择界面，暂停界面。除了两种游戏模式的游戏内界面似乎并没有明显的继承关系。HUD 内需要含有表示对应玩家的 `PlayerController* player`

### 交互机制

不同场景之间的切换在 `MainWindow` 中实现。`MainWindow` 保存当前场景 `Level* currentLevel` 并实现一个模板函数 `template<class LevelClass> openLevel()`

### 更新机制

Qt ViewSceneItem 这一套框架自身提供了部分更新机制
- 逻辑更新：`QGraphicsScene::advance()` 会依次调用每个 item 的 `advance(phase)`，调用两次（`phase` 分别为 0 和 1）。通过设置 Timer 可以更新所有关卡内物体的状态。
- 输入传递：`QGraphicsView` 接受鼠标和键盘的输入操作，经过一些处理后转发给绑定的 scene，然后绑定的 scene 再转发给 item。由于我们的角色本身的动作逻辑和从输入组织动作这两件事之间是解耦的，所以这一套传递链是基本上没有用的（这套逻辑本来也不是用来处理游戏的，而是处理海量交互控件的）。不过在处理道具效果时判断鼠标选中的是哪一个中，这套逻辑是有用的。一种合理的处理逻辑应当通过 signal/slot 机制将 `QGraphicsView` 触发的输入事件转发到 `PlayerController`，然后由 controller 来决定如何组织角色的响应逻辑。
- 绘制更新：利用 `QGraphicsScene` 提供的自动重绘机制：item 状态发生改变时会调用 `update`，item 的 `update` 会向 view 发送重绘请求，在后续 view 的 `paintEvent` 事件中依次传递最终触发 item 的 `paint`。


### 开始界面

由 `Level` 的子类 `StartMenuLevel` 实现，`scene` 指向空白的 `QGraphicsScene`（保留创意的可能），`camera` 为默认，`hudList` 初始仅包含开始界面，随着用户操作可能添加/删除：确定地图大小界面，加载路径选择界面。

