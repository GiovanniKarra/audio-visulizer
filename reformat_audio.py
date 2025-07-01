import sys, librosa, numpy as np


args = sys.argv
if len(args) != 2:
	print("Usage: py reformat_audio <file.mp3>|<file.wav>")
	exit(1)

file = args[-1]

y, sample_rate = librosa.load(file, sr=None)
sample_count = y.shape[0]

new_file = file.split(".")[0] + ".giodio"

with open(new_file, "wb") as f:
	f.write(np.array([sample_count, sample_rate], dtype=np.int32).tobytes())


with open(new_file, "ab") as f:
	f.write(y.tobytes())
