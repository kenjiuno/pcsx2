# [PCSX2-py-mon] PCSX2: Execution monitor with Python 3.8

## Overview

Add debugging support of running game on mainly int (interpreter) mode.

```
$ tree /h/Proj/pcsx2/bin
/h/Proj/pcsx2/bin
├── monitor
│   ├── __init__.py
│   ├── 00000000.py
│   ├── 93F8A60B.py
│   ├── F266B00B.py
│   └── main.py
├── pcsx2.exe
```

`*.py`

```py
#!pcsx2py
```

## Embedded functions

### pcsx2.ClearClient

```py
pcsx2.ClearClient
```

Clear break points.

### pcsx2.WriteLn

```py
pcsx2.WriteLn("")
```

`Console.WriteLn(L"%s", pzwOutput);`

Write line to pcsx2 debug window.

### pcsx2.AddBrk

```py
pcsx2.AddBrk(addr, callable)
```

```py
def bp(addr):
	def setbp(funct):
		print('Add bp: {0:x}'.format(addr))
		pcsx2.AddBrk(addr, funct)
	return setbp

@bp(0x002fdfc8)
def S_ASSERT():
	print("ASSERT!")
	print(read0Str(pcsx2.GetUL0('a0')))
```

Add breakpoint (max 32)

### pcsx2.GetUL0

```py
pcsx2.GetUL0(regName)
```

```py
pcsx2.GetUL0('ra')
```

Get lower 32 bits of given register name.

### pcsx2.SetUL0

```py
pcsx2.SetUL0(regName, long)
```

```py
pcsx2.SetUL0("a0", 0)
```

Update only lower 32 bits of given register name.

### pcsx2.ReadMem

```py
pcsx2.ReadMem(start, length)
```

```py
mem = pcsx2.ReadMem(0, 32*1024*1024)
```

Read EE memory.

### pcsx2.ReadUI16

```py
long = pcsx2.ReadUI16(addr)
```

```cpp
    u16 *pRd = (u16 *)PSM(off);
    return PyLong_FromLong(*pRd);
```

Read 2 bytes from EE memory.

### pcsx2.ReadUI32

```cpp
    u32 *pRd = (u32 *)PSM(off);
    return PyLong_FromLong(*pRd);
```

### pcsx2.ReadI16

```cpp
    s16 *pRd = (s16 *)PSM(off);
    return PyLong_FromLong(*pRd);
```

### pcsx2.ReadI32

```cpp
    s32 *pRd = (s32 *)PSM(off);
    return PyLong_FromLong(*pRd);
```

### pcsx2.ReadByte

```cpp
    u8 *pRd = (u8 *)PSM(off);
    return PyLong_FromLong(*pRd);
```

### pcsx2.WriteMem

```py
pcsx2.WriteMem(PyBytes, startIndex, addr, len)
```

Write bytes to EE memory.

### pcsx2.WriteByte

```py
pcsx2.WriteByte(addr, value)
```

```cpp
    memWrite8(off, val);
```

Write a byte to EE memory.

### pcsx2.WriteUI16

```cpp
    memWrite16(off, val);
```

### pcsx2.WriteUI32

```cpp
    memWrite32(off, val);
```

### pcsx2.AddRBrk

```py
index = pcsx2.AddRBrk(addr, length, callable)
```

Set memory read break point. Max 32 read break points.

### pcsx2.DelRBrk

```py
pcsx2.DelRBrk(index)
```

### pcsx2.AddWBrk

```py
index = pcsx2.AddWBrk(addr, length, callable)
```

Set memory write break point. Max 32 write break points.

### pcsx2.DelWBrk

```py
pcsx2.DelWBrk(index)
```

### pcsx2.opc

Return current `pc` (program counter) only for int mode.

### pcsx2.isRec

- True if rec mode.
- False if int mode.

### pcsx2.Error

`Console.Error(L"%s", pzwOutput);`

### pcsx2.Warning

`Console.Warning(L"%s", pzwOutput);`

### pcsx2.OnSuspend

```py
@pcsx2.OnSuspend
def onSuspend():
	dump()
	print('Suspend')
```

### pcsx2.OnResume

```py
@pcsx2.OnResume
def onResume():
	print('Resume')
```

### pcsx2.StartEETrace

### pcsx2.EndEETrace
