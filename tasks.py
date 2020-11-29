from invoke import task
from pathlib import Path

import os
import sys

sys.path.append(str(Path('C:/Program Files/7-Zip')))


@task
# py -m invoke pack
def pack(c, docs=False):
    version = c.run('git describe --always').stdout.split('\n')[0]
    writeTo = Path('pcsx2-%s.7z' % (version, ))
    c.run('7z a -xr!*.exp -xr!*.lib -xr!*.idb -xr!*.pdb -xr!.vscode -xr!__pycache__ -xr!.vscode "%s" bin/pcsx2.exe bin/docs bin/Langs bin/plugins bin/shaders bin/cheats_ws.zip bin/GameIndex.dbf bin/monitor' % (str(writeTo), ))
