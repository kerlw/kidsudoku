# KidSudoku 儿童数独

[![Join the chat at https://gitter.im/kerlw/kidsudoku](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/kerlw/kidsudoku?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

I'm a father of a  lovely 4-years old girl.
I wanna give her a gift -- a sudoku game, but she is too young to play the real sudoku, so I wanna make a simple one.

我是一个父亲，我有一个可爱的女儿，四岁了。
我希望给她一个礼物————数独游戏，但是她不可能玩真正的数独，只能是一个简单的适合儿童的数独。

## Design principles 设计原则
* stage by stage, simple to hard 关卡式，由简单逐渐变复杂
* hints only in simple stage 只在简单关卡中有提示
* error hints in every stage 错误提示总是存在
* though simple stage is not real sudoku, no need to guareent only one solution 简单关卡不是真正的数独，不需要保证唯一解


## Some idears 一些想法
* Android to IOS 先开发Android版，再进军IOS
* fruit/animal icons replace numbers 水果/动物图标替换数字
* make error hints easier to see and understand 让错误提示更显眼更易懂

## Road map 开发规划
1. a basicaly playable version with some internal stage 完成一个基本可玩的版本，内置一些关卡
	* 【done】make some stage data internal 内置一些关卡数据
	* 【done】make the game continuable, stage by stage 让游戏一关关的延续
	* 【done】point out the error 给出错误提示
	* 【done】load campaign or stage data from file 从文件加载战役、关卡数据
	* 【done】add reset/undo for each stage 在关卡中增加重置/撤消支持
	* 【done】add main scene, campaign scene 增加主界面、战役界面
	* 【done】random stage 随机生成的谜题
	* 【TODO】add bacground images to each cell 每个格子加入背景
+ an editor for the game 完成一个编辑器
	* 【done】edit single stage at first 从编辑单个关卡开始着手
	* 【done】edit campaigns which is made up by stages 编辑由关卡组成的战役
	* 【done】generator random puzzle according to specified size 根据指定的尺寸生成随机的谜题
	* 【doing】specify the resource for the campaign/stage 为战役/关卡设置特定的资源

## How to compile 如何编译代码
*  cocos2d-x 3.6 needed 需要cocos2d-x 3.6
* need cocos2d project's public source folder 'cocos2d'  需要cocos2d工程的公共目录cocos2d
* at the root directory of the source code, run 在代码根目录运行

> cocos run -p linux

for android: 编译目标android平台的话则运行

> cocos run -p android -j4

* If compile for android, android sdk/android ndk needed 如果是编译android版本，需要android的sdk和ndk
* set enviroment  variables according to the error tips 根据错误提示设定环境变量

*see also guid from cocos2d.org*

## Contact me 联系我
If you are a programmer and have interest in this project, mail me!  Any help would be appreciated.
如果你是对这个项目感兴趣的程序猿，联系我！欢迎一切助力，谢谢了先。

Email: kerl.d.w@gmail.com
