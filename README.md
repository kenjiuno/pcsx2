# [PCSX2-execmon] PCSX2: Execution monitor with Python 3.8

## Overview

Add debugging support of running game on mainly int (interpreter) mode.

```
$ tree /h/Proj/pcsx2/bin
/h/Proj/pcsx2/bin
├── monitor
│   ├── __init__.py
│   ├── 00000000.py
│   ├── F266B00B.py
│   └── main.py
├── pcsx2.exe
```

`*.py`

```py
#!pcsx2py

# This volatile module `monitor.F266B00B` may be re-loaded and loose all variables on some events:
# - Game startup (on _reloadElfInfo)
# - Resume, where it is suspended by pressing ESC key (on AppCoreThread::Resume)
# Not:
# - Resume, where it is suspended by System menu → Pause
# - Load game state

import pcsx2

pcsx2.WriteLn('Hello')

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
cookie = pcsx2.AddBrk(addr, callable)
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

### pcsx2.DelBrk

```py
pcsx2.DelBrk(cookie)
```

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

### pcsx2.SetPC

```py
pcsx2.SetPC(long)
```

```py
pcsx2.SetPC(0x001ae004)
```

Set new `pc` (program counter), and refresh recompiler.

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
cookie = pcsx2.AddRBrk(addr, length, callable)

callable(hitAddr)
```

Set read break point of every load operators.

### pcsx2.DelRBrk

```py
pcsx2.DelRBrk(cookie)
```

### pcsx2.AddWBrk

```py
cookie = pcsx2.AddWBrk(addr, length, callable)

callable(hitAddr)
```

Set write break point of every store operators.

### pcsx2.DelWBrk

```py
pcsx2.DelWBrk(cookie)
```

### pcsx2.pc

```py
pc = pcsx2.pc()
```

Return current `pc` (program counter).

### pcsx2.opc

```py
opc = pcsx2.opc()
```

Return old (previous) `pc` (program counter) only for int mode.

### pcsx2.isRec

```py
mode = pcsx2.isRec()
```

- True if rec mode.
- False if int mode.

### pcsx2.ElfCRC

```py
crc = pcsx2.ElfCRC()
```

Return Game CRC in long value.

### pcsx2.Error

`Console.Error(L"%s", pzwOutput);`

### pcsx2.Warning

`Console.Warning(L"%s", pzwOutput);`

### pcsx2.OnSuspend

OnSuspend is holded only last one.

```py
@pcsx2.OnSuspend
def onSuspend():
	dump()
	print('Suspend')
```

### pcsx2.OnResume

OnResume is holded only last one.

```py
@pcsx2.OnResume
def onResume():
	print('Resume')
```

### pcsx2.StartEETrace and pcsx2.EndEETrace

Write `trac2` record data. Only available on int mode. Format: https://gitlab.com/kenjiuno/khkh_xldM/-/blob/master/ee1Dec/trac2.bt

```py
tracer = 0

@bp(0x00101e94)
def TickIncr():
    pcsx2.WriteLn('# Tick %d' % (pcsx2.GetUL0('t7'),))
    global tracer
    if tracer == 0:
        tracer = 1
        pcsx2.StartEETrace(str(exeDir.joinpath('oneround.bin')))
    elif tracer == 1:
        tracer = 2
        pcsx2.EndEETrace()
```

### pcsx2.SetRWTraceOptions

A read/write reporter.

```py
    pcsx2.SetRWTraceOptions(0, None)
    pcsx2.FlushRWTrace()
    pcsx2.SetRWTraceOptions(1, self.flush_rw_buffer)
```

```py
pcsx2.SetRWTraceOptions(flags, callable)
```

flags:

- 1 watch load opecodes
- 2 watch store opecodes
- 3 both

callable:

```py
from struct import *

class Class:
    def flush_rw_buffer(self, buff: bytes):
        for (pc, target, flags) in iter_unpack("<III", buff):
            pass
```

### pcsx2.FlushRWTrace

Flush queued read/write records buffer immediately.

```py
pcsx2.FlushRWTrace()
```
