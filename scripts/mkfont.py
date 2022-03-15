import sys

filename = sys.argv[1]

class character:
    char = 0
    u1 = 0
    v1 = 0
    u2 = 0
    v2 = 0


def makeChar(line):
    numbers = line.strip("\n").split(" ")
    c = character()
    c.char = int(numbers[0])
    c.u1 = int(numbers[1])
    c.v1 = int(numbers[2])
    c.u2 = int(numbers[3])
    c.v2 = int(numbers[4])
    return c


lines = []

with open(filename, "r") as f:
    lines = f.readlines()


chars = []
tex = []


mode = "none"

for line in lines:
    if "TEXTURE" in line:
        mode = "tex"
        continue
    elif line == "characters\n":
        mode = "char"
        continue

    if mode == "tex":
        tex.append(line.strip("\n"))
    elif mode == "char":
        chars.append(makeChar(line))


def writeChar(file, char):
    file.write((char.char).to_bytes(1, byteorder="little", signed=True))
    file.write((char.u1).to_bytes(2, byteorder="little", signed=False))
    file.write((char.v1).to_bytes(2, byteorder="little", signed=False))
    file.write((char.u2).to_bytes(2, byteorder="little", signed=False))
    file.write((char.v2).to_bytes(2, byteorder="little", signed=False))


basename = filename.split("/")[len(filename.split("/"))-1].split(".")[0]
outputfile = "assets/fonts/"+basename+".fnt"

with open(outputfile, "wb") as f:
    f.write((len(chars)).to_bytes(1, byteorder="little", signed=False))
    for c in chars:
        writeChar(f, c)
    f.write((len(tex)).to_bytes(1, byteorder="little", signed=False))
    for s in tex:
        s += "\0"
        f.write(s.encode("ascii"))
