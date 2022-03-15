import sys

if len(sys.argv) < 2:
    print("compileSprite.py : not enough arguments")
    exit(1)

spritename = sys.argv[1].split(".")[0]
filein = "assets/sprites/notcompiled/" + spritename + ".sp"
fileout = "assets/sprites/" + spritename + ".spr"

class frame:
    duration = 0
    ox = 0
    oy = 0
    u1 = 0
    v1 = 0
    u2 = 0
    v2 = 0


frames = []

texID = 0

with open(filein, "r") as f:
    for l in f:
        w = l.split()
        if len(w) == 0:
            continue
        if w[0] != "frame":
            if w[0] == "texture":
                texID = int(w[1])
            continue
        fr = frame()
        fr.duration = int(w[1])
        fr.ox = int(w[2])
        fr.oy = int(w[3])
        fr.u1 = int(w[4])
        fr.v1 = int(w[5])
        fr.u2 = int(w[6])
        fr.v2 = int(w[7])
        frames.append(fr)

with open(fileout, "wb") as f:
    f.write((texID).to_bytes(2, byteorder='little', signed=False))
    f.write((len(frames)).to_bytes(2, byteorder='little', signed=False))
    for fr in frames:
        f.write((fr.duration).to_bytes(2, byteorder='little', signed=False))
        f.write((fr.ox).to_bytes(2, byteorder='little', signed=True))
        f.write((fr.oy).to_bytes(2, byteorder='little', signed=True))
        f.write((fr.u1).to_bytes(2, byteorder='little', signed=False))
        f.write((fr.v1).to_bytes(2, byteorder='little', signed=False))
        f.write((fr.u2).to_bytes(2, byteorder='little', signed=False))
        f.write((fr.v2).to_bytes(2, byteorder='little', signed=False))
