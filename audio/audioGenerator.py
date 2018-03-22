# -*- coding: utf-8 -*-

from Recorder import *
from module309 import *

if __name__ == '__main__':
    frame = 13*[0]
    s = createSignal(frame)
    rec = Recorder(channels=2)
    with rec.open('sine.wav', 'wb') as recfile:
        recfile.record(duration=5.0)
        playSignal(s)
