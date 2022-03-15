headerstart = [
    "#ifndef ASSETS_H\n",
    "#define ASSETS_H\n",
    "\n",
    "int defaultTexture();\n",
    "int defaultFont();\n",
    "\n",
    "class gameassets {\n",
    "    public:\n",
    "        static void Init();\n"
]

headerend = [
    "};\n",
    "\n",
    "#endif"
]

cppstart = [
    "#include \"Assets.h\"\n",
    "#include \"NSEngine.h\"\n",
    "\n"
]

cppmid = [
    "\n",
    "int defaultTexID;\n"
    "int defaultTexture() { return defaultTexID; }\n",
    "int defaultFontID;\n",
    "int defaultFont() { return defaultFontID; }\n",
    "\n",
    "void gameassets::Init()\n",
    "{\n"
    "    defaultTexID = NSEngine::TextureManager::RegisterTexture(\
        \"assets/engine/textures/defaultTexture.png\");\n",
    "    defaultFontID = NSEngine::SpriteManager::RegisterFont(\
        \"assets/engine/fonts/Lato-Regular.ttf, 10\");\n"
]

cppend = [
    "}"
]


class asset:
    assetname = ""
    assettype = ""
    arg0 = ""
    arg1 = ""
    arg2 = ""
    arg3 = ""
    arg4 = ""
    arg5 = ""
    arg6 = ""
    arg7 = ""


def setSpriteTexture(sprfile, texid):
    lines = []
    temp = sprfile.split('/')
    name = temp.pop(len(temp)-1)
    name = name[:-1]
    temp.append("notcompiled")
    temp.append(name)
    sprfile = '/'.join(temp)
    tid = 2
    for a in assetslist:
        if a.assettype == "texture":
            if a.assetname == texid:
                break
            tid += 1
    with open(sprfile, "r") as f:
        lines = f.readlines()
    i = 0
    for l in lines:
        w = l.split()
        if w[0] != "texture":
            i += 1
            continue
        lines[i] = "texture " + str(tid) + "\n"
        break

    with open(sprfile, "w") as f:
        f.writelines(lines)


def assetlineheader(a):
    return "        static int " + a.assetname + ";\n"


def assetlinecpp1(a):
    return "int gameassets::" + a.assetname + ";\n"


def assetlinecpp2(a):
    if a.assettype == "texture":
        return "    " + a.assetname + " = NSEngine::TextureManager::\
            RegisterTexture(\"" + a.arg0 + \
            ("\", \"" + a.arg1, "")[a.arg1 == ""] + \
            ("\", \"" + a.arg2, "")[a.arg2 == ""] + \
            ("\", \"" + a.arg3, "")[a.arg3 == ""] + \
            "\");\n"
    elif a.assettype == "sprite":
        setSpriteTexture(a.arg0, a.arg1)
        return "    " + a.assetname + " = NSEngine::SpriteManager::\
            RegisterSprite(\"" + a.arg0 + "\");\n"
    elif a.assettype == "font":
        return "    " + a.assetname + " = NSEngine::SpriteManager::\
            RegisterFont(\"" + a.arg0 + "\", " + a.arg1 + ");\n"
    elif a.assettype == "spritefont":
        return "    " + a.assetname + " = NSEngine::SpriteManager::\
            RegisterFont(" + a.arg1 + ", " + a.arg2 + ", " + a.arg3 + \
            ", \" " + a.arg0 + "\", " + a.arg4 + ", " + a.arg5 + ", " + \
            a.arg6 + ", " + a.arg7 + ");\n"
    elif a.assettype == "spritefont2":
        return "    " + a.assetname + " = NSEngine::SpriteManager::\
            RegisterFont(\"" + a.arg0 + "\");\n"
    elif a.assettype == "tileset":
        return "    " + a.assetname + " = NSEngine::TilesetManager::\
            RegisterTileset(" + a.arg0 + ", " + a.arg1 + ");\n"
    elif a.assettype == "sfx":
        return "    " + a.assetname + " = NSEngine::AudioEngine::\
            AddSFX(\"" + a.arg0 + "\");\n"
    elif a.assettype == "music":
        return "    " + a.assetname + " = NSEngine::AudioEngine::\
            AddMusic(\"" + a.arg0 + "\");\n"
    else:
        return ""


assetslist = []


def genAssetsinFile(filename):
    with open(filename) as f:
        for l in f:
            w = l.split()
            if w == []:
                continue
            if w[0] == "subfile":
                genAssetsinFile(w[1])
                continue
            a = asset()
            a.assetname = w[1]
            a.assettype = w[0]
            a.arg0 = w[2]
            if len(w) > 3:
                a.arg1 = w[3]
            if len(w) > 4:
                a.arg2 = w[4]
            if len(w) > 5:
                a.arg3 = w[5]
            if len(w) > 6:
                a.arg4 = w[6]
            if len(w) > 7:
                a.arg5 = w[7]
            if len(w) > 8:
                a.arg6 = w[8]
            if len(w) > 9:
                a.arg7 = w[9]
            assetslist.append(a)


genAssetsinFile("resources.nsr")

with open("NSEngine/Assets.h", "w") as f:
    for l in headerstart:
        f.write(l)
    for a in assetslist:
        f.write(assetlineheader(a))
    for l in headerend:
        f.write(l)

with open("NSEngine/Assets.cpp", "w") as f:
    for l in cppstart:
        f.write(l)
    for a in assetslist:
        f.write(assetlinecpp1(a))
    for l in cppmid:
        f.write(l)
    for a in assetslist:
        f.write(assetlinecpp2(a))
    for l in cppend:
        f.write(l)
