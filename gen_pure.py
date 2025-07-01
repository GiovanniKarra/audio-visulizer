import sys, librosa, numpy as np


args = sys.argv
if len(args) != 2:
	print("Usage: py gen_pure.py <frequency>")
	exit(1)

freq = int(args[-1])
sample_count, sample_rate = 1000000, 48000

file = "pure.giodio"

with open(file, "wb") as f:
	f.write(np.array([sample_count, sample_rate], dtype=np.int32).tobytes())


with open(file, "ab") as f:
	f.write(np.sin(2*np.pi*freq*np.linspace(-20, 20, sample_count)).astype(np.float32).tobytes())
