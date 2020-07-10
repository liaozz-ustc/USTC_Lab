# <center>计算机体系结构Lab4实验报告</center>

<center>廖洲洲 PB17081504</center>

## 实验目的

- 实现BTB（Branch Target Buffer）和BHT（Branch History Table）两种动态分支预测器
- 体会动态分支预测对流水线性能的影响

## 实验内容

注：Lab2实现的cpu是预测不分支cpu，故对分支指令，其下一条指令总为PC+4,当在ID或者EX阶段发现要分支时则flush掉预测的指令，跳到分支指令。

### BTB

- BTB使用一个Buffer，里面记录了历史指令跳转信息。对于每一条跳转的Branch指令，它都将其写入buffer，记录其跳转的地址，并有一个标志位标记最近一次执行是否跳转。这样如果有一条在Buffer里的跳转指令将执行时，可以根据buffer记录的历史跳转信息，预测下一条要执行的指令地址，预测正确的话可以减小分支开销。  

- BTB只使用了1-bit的历史信息，也可以视作1-bit BHT。

- 当在BTB中未检查到匹配项，或匹配但状态位为0，则下一条指令为PC+4，但在EX段发现是需要跳转的Branch指令，则在EX阶段更新BTB表，（此时Br=1，跳转，同时flush)；是不需要跳转的Branch指令和其它指令则不用做其它事。

- 当在BTB中检查到匹配项，并且状态位为1，则使用Predicted PC作为下一PC。如果在EX阶段发现其跳转成功，不需要做任何事(令Br=0即可）；如果跳转失败，更新BTB表，并flush错误装载的指令，执行PC+4（在EX阶段能得到之前的PC，故再设置一个PC_4_Old选择之前的PC+4）。

  

### BHT