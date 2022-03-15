#include "StdFile.h"
#include <algorithm>

#include "stdOpener.h"
#include "../NSEngine.h"
#include "../AnmOpener/AnmManagerN.h"

namespace StdOpener {

    StdFile::StdFile(std::string const& filename)
    {
        thstd_t* std = std_read_file(filename);

        for (auto e : std->entries)
        {
            entries.push_back({});
            entries.back().layer = e->header->unknown;
            entries.back().x = e->header->x;
            entries.back().y = e->header->y;
            entries.back().z = e->header->z;
            entries.back().width = e->header->width;
            entries.back().height = e->header->height;
            entries.back().depth = e->header->depth;
            for (auto q : e->quads)
                entries.back().quads.push_back({q->size, q->script_index, q->x, q->y, q->z, q->width, q->height});
        }

        for (auto i : std->instances) faces.push_back({i->object_id, i->x, i->y, i->z});

        uint32_t offset = 0;
        for (auto i : std->instrs)
        {
            script.push_back({i->time, i->type, offset});
            offset += 8;
            if (i->size > sizeof(i))
            {
                uint16_t nbArgs = (i->size - sizeof(i)) / 4;
                offset += nbArgs * 4;
                for (uint16_t j = 0; j < nbArgs; j++)
                {
                    int32_t S = *(int32_t*)((char*)i+8+j*4);
                    script.back().args.push_back({.S=S});
                }
            }
        }

        anm_file_name = std->header_10->anm_name;

        std_free(std);
    }

    StdFile::~StdFile()
    {
        Clear();
    }

    void StdFile::Init()
    {
        NSEngine::AnmManagerN::LoadFile(30, anm_file_name);
        auto en = entries;
        std::sort(faces.begin(), faces.end(), [en](face const& a, face const& b) { return en[a.entry_id].layer < en[b.entry_id].layer;});
        for (auto f : faces) spawnFace(f);
        theCam = NSEngine::activeCamera3D();
    }

    void StdFile::Clear()
    {
        for (auto vm : bgVms) NSEngine::AnmManagerN::deleteVM(vm->getID());
        bgVms.clear();
    }

    void StdFile::Update()
    {
        /* CHECK FOR INTERRUPTIONS */
        if (pending_switch_label != 0)
        {
            //find label
            uint32_t j = 0;
            for (auto i : script)
            {
                if (i.type == 16 && i.args[0].S == pending_switch_label)
                {
                    instr = j;
                    time = i.time;
                    stopped = false;
                }
                j++;
            }
            pending_switch_label = 0;
        }

        /* WAITING */
        if (stopped) return;

        /* RUN INSTRUCTIONS */
        uint32_t oldinstr = instr;
#define in script[instr]
        while (in.time <= time && !stopped)
        {
            execInstr(in);
            if (oldinstr != instr) execInstr(in);
            instr++;
            oldinstr = instr;
        }
#undef in

        time++;
    }

    void StdFile::spawnFace(StdFile::face const& f)
    {
        const entry& e = entries[f.entry_id];
        for (auto q : e.quads)
        {
            float x = f.x + e.x; if (q.x != e.x) x += q.x;
            float y = f.y + e.y; if (q.y != e.y) y += q.y;
            float z = f.z + e.z; if (q.z != e.z) z += q.z;
            bgVms.push_back(NSEngine::AnmManagerN::getVM(NSEngine::AnmManagerN::SpawnVM(30, q.script_index)));
            bgVms.back()->setPos2(x, y, z);
        }
    }

    void StdFile::execInstr(StdFile::instruction const& i)
    {
        if (theCam == nullptr) return;
#define S(n) (i.args[n].S)
#define f(n) (i.args[n].f)
#define a(n) ((i.args[n].S >> 24) & 255)
#define r(n) ((i.args[n].S >> 16) & 255)
#define g(n) ((i.args[n].S >>  8) & 255)
#define b(n) ((i.args[n].S >>  0) & 255)
        switch (i.type)
        {
            case 0: // stop
                stopped = true;
                return;
            case 1: // jmp
                for (uint32_t j = 0; j < script.size(); j++)
                    if (script[j].offset == S(0)) instr = j;
                time = S(1);
                return;
            case 2: // pos
                theCam->setPosition(f(0), -f(1), f(2));
                return;
            case 3: // posTime
                theCam->PositionTime(S(0), S(1), f(2), -f(3), f(4));
                return;
            case 4: // facing
                theCam->setLook(f(0), -f(1), f(2));
                return;
            case 5: // facingTime
                theCam->LookTime(S(0), S(1), f(2), -f(3), f(4));
                return;
            case 6: // up
                theCam->setUp(f(0), -f(1), f(2));
                return;
            case 7: // fov
                theCam->setFov(f(0));
                return;
            case 8: // fog
                theCam->setFog(r(0), g(0), b(0), a(0), f(1), f(2));
                return;
            case 9: // fogTime
                theCam->FogTime(S(0), S(1), r(2), g(2), b(2), a(2), f(3), f(4));
                return;
            case 10: // posBezier
                theCam->PositionBezier(S(0), f(2), f(3), f(4), f(5), f(6), f(7), f(8), f(9), f(10));
                return;
            case 11: // facingBezier
                theCam->LookBezier(S(0), f(2), f(3), f(4), f(5), f(6), f(7), f(8), f(9), f(10));
                return;
            case 12: // rockingMode
                // TODO
                return;
            case 13: // bgColor
                theCam->setClearColor(r(0), g(0), b(0));
                return;
            case 14: // sprite
                bgVms.push_back(NSEngine::AnmManagerN::getVM(NSEngine::AnmManagerN::SpawnVM(30, S(1))));
                return;
            case 15: // nop
                return;
            case 16: // interruptLabel
                return;
            case 17: // distortion
                // TODO
                return;
            case 18: // upTime
                theCam->UpTime(S(0), S(1), f(2), -f(3), f(4));
                return;
            case 19: // interruptBG
                for (auto vm : bgVms) vm->interrupt(7 + S(0));
                return;
            case 20: // drawDistance
                // unused
                return;
            case 21: // fovTime
                theCam->FovTime(S(0), S(1), f(2));
                return;
        }
#undef S
#undef f
#undef a
#undef r
#undef g
#undef b
    }

}
