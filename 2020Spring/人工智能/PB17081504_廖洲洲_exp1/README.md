# Windows Cmd 命令行编译

## Digit

### A*

```powershell
cd digit\src
g++ Astar.cpp -Ofast -march=native -o Astar.exe
Astar.exe ..\input\input2.txt ..\input\dest.txt (Astar.exe 初始状态文件 目标状态文件)
```

在当前目录下输出同名文件

### IDA*

```powershell
cd digit\src
g++ IDAstar.cpp -Ofast -march=native -o IDAstar.exe
IDAstar.exe ..\input\input2.txt ..\input\dest.txt (IDAstar.exe 初始状态文件 目标状态文件)
```

在当前目录下输出同名文件

## Sudoku

### backtracking

```powershell
cd sudoku\src
g++ backtrack.cpp -o backtrack.exe
backtrack.exe ..\input\sudoku01.txt (backtrack.exe 输入文件)
```

在当前目录下输出同名文件

### degree

```powershell
cd sudoku\src
g++ degree.cpp -o degree.exe
degree.exe ..\input\sudoku01.txt (degree.exe 输入文件)
```

在当前目录下输出同名文件

### MRV

```powershell
cd sudoku\src
g++ MRV.cpp -o MRV.exe
MRV.exe ..\input\sudoku01.txt (MRV.exe 输入文件)
```

在当前目录下输出同名文件

### forwarding

```powershell
cd sudoku\src
g++ forwarding.cpp -o forwarding.exe
forwarding.exe ..\input\sudoku01.txt (forwarding.exe 输入文件)
```

在当前目录下输出同名文件

### MRV+forwarding

```powershell
cd sudoku\src
g++ MRVforwarding.cpp -o MRVforwarding.exe
MRVforwarding.exe ..\input\sudoku01.txt (MRVforwarding.exe 输入文件)
```

在当前目录下输出同名文件