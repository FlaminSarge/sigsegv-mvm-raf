#ifndef _INCLUDE_SIGSEGV_UTIL_LUA_H_
#define _INCLUDE_SIGSEGV_UTIL_LUA_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace Util::Lua
{

    class LuaTimer
    {
    public:
        LuaTimer(int id, float delay, int repeats, int reffunc, int refparam) : m_iID(id), m_flNextCallTime(gpGlobals->curtime + delay), m_flDelay(delay), m_iRepeats(repeats), m_iRefFunc(reffunc), m_iRefParam(refparam) {}

        void Destroy(lua_State *l);

        int m_iID;
        float m_flNextCallTime;
        float m_flDelay;
        int m_iRepeats;
        int m_iRefFunc;
        int m_iRefParam;
    };

    class LuaState : public AutoList<LuaState>
    {
    public:
        LuaState();

        virtual ~LuaState();

        lua_State *GetState() {
            return l;
        }

        void SwitchState();

        void DoString(const char *str, bool execute);

        void DoFile(const char *str, bool execute);

        void CallGlobal(const char *str, int numargs);

        bool CheckGlobal(const char *str);

        int Call(int numargs, int numret);

        void UpdateTimers();

        virtual void TimerAdded() {}
        virtual void AllTimersRemoved() {}

        int AddTimer(float delay, int repeats, int reffunc, int refparam);
        bool StopTimer(int id);

        bool HasTimers() { return !timers.empty(); }

        void EntityDeleted(CBaseEntity *entity);
        void EntityCallbackAdded(CBaseEntity *entity);

        void Activate();
    private:
        lua_State *l;

        int m_iVectorMeta = LUA_NOREF;
        const void *m_pVectorMeta = nullptr;
        int m_iEntityMeta = LUA_NOREF;
        const void *m_pEntityMeta = nullptr;
        int m_iEntityTableStorage = LUA_NOREF;
        int m_iPropMeta = LUA_NOREF;

        std::vector<LuaTimer> timers;
        int m_iNextTimerID = 0;
        std::unordered_set<CBaseEntity *> callbackEntities;
    };

    void LFromVariant(lua_State *l, variant_t &variant);
    const EHANDLE *LEntityAlloc(lua_State *l, CBaseEntity *entity);
}
#endif