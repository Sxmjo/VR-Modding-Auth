#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/Class.hpp>
#include <BNM/Method.hpp>
#include <BNM/MethodBase.hpp>
#include <BNM/Field.hpp>
#include <BNM/FieldBase.hpp>
#include <BNM/Defaults.hpp>
#include <BNM/ComplexMonoStructures.hpp>
#include <BNM/BasicMonoStructures.hpp>
#include <BNM/Operators.hpp>
#include <random>
#include <string>
#include <dlfcn.h>
#include <BNMResolve.hpp>

using namespace BNM;

class AuthBypass {
public:
    static inline Class Message;
    static inline Class User;
    static inline Class OrgScopedID;
    static inline Class UserProof;

    static inline Class Systemm;
    static inline Class DirClass;
    static inline Class DirClass2;

    static inline BNM::Types::ulong orgScoped;
    static inline BNM::Types::ulong userId;
    static inline std::string orgScopedID_forBoards;

    inline static bool (*old_get_IsError)(void*);
    static bool new_get_IsError(void* instance) {
        return false;
    }

    inline static bool (*old_dirExists)(BNM::Structures::Mono::String*);
    static bool new_dirExists(BNM::Structures::Mono::String* path) {
        return false; // always block
    }

    inline static BNM::Structures::Mono::Array<BNM::Structures::Mono::String*>*
    (*old_dirGetDirs)(BNM::Structures::Mono::String*);

    static BNM::Structures::Mono::Array<BNM::Structures::Mono::String*>*
    new_dirGetDirs(BNM::Structures::Mono::String* path) {
        return old_dirGetDirs(path);
    }

    inline static void (*old_user_ctor)(void*, uintptr_t);
    static void new_user_ctor(void* instance, uintptr_t ptr) {
        Field<BNM::Types::ulong> idField = User.GetField("ID");
        idField[instance].Set(userId);
    }

    inline static void (*old_orgscoped)(void*, uintptr_t);
    static void new_orgscoped(void* instance, uintptr_t ptr) {
        Field<BNM::Types::ulong> idField = OrgScopedID.GetField("ID");
        idField[instance].Set(orgScoped);
    }

    inline static void (*old_userproof)(void*, uintptr_t);
    static void new_userproof(void* instance, uintptr_t ptr) {
        Field<BNM::Structures::Mono::String*> valueField = UserProof.GetField("Value");
        valueField[instance].Set(CreateMonoString("jtKX5GxKkQ1LmXl11kWgVXUmP4RmrwxNka8dy4uiaUpyGVERbiyvTUQblZSn55W0 "));
    }

    static inline BNM::Types::ulong genUlong(int digits) {
        BNM::Types::ulong min = 1;
        for (int i = 1; i < digits; ++i) min *= 10;
        BNM::Types::ulong max = min * 10 - 1;
        static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<BNM::Types::ulong> dist(min, max);
        return dist(rng);
    }

    static void StartBypass() {
        std::vector<uint64_t> userIds = {
                25295851090056732
        };

        std::vector<uint64_t> orgScopeds = {
                25329582050014166
        };

        userId = userIds[0];
        orgScoped = orgScopeds[0];
        orgScopedID_forBoards = std::to_string(orgScoped);

        Message = Class(BNM_OBFUSCATE("Oculus.Platform"), BNM_OBFUSCATE("Message"));
        MethodBase get_IsError = Message.GetMethod(BNM_OBFUSCATE("get_IsError"), 0);
        BasicHook(get_IsError, new_get_IsError, old_get_IsError);

        User = Class(BNM_OBFUSCATE("Oculus.Platform.Models"), BNM_OBFUSCATE("User"));
        MethodBase userctor = User.GetMethod(".ctor", 1);
        BasicHook(userctor, new_user_ctor, old_user_ctor);

        OrgScopedID = Class(BNM_OBFUSCATE("Oculus.Platform.Models"), BNM_OBFUSCATE("OrgScopedID"));
        MethodBase orgscopedctor = OrgScopedID.GetMethod(".ctor", 1);
        BasicHook(orgscopedctor, new_orgscoped, old_orgscoped);

        UserProof = Class(BNM_OBFUSCATE("Oculus.Platform.Models"), BNM_OBFUSCATE("UserProof"));
        MethodBase userproofctor = UserProof.GetMethod(".ctor", 1);
        BasicHook(userproofctor, new_userproof, old_userproof);

        Systemm = Class("System.Environment", "Exit");
        BasicHook(Systemm.GetMethod("Exit", 1), false, false);

        DirClass = Class("System.IO.Directory", "Exists");
        BasicHook(DirClass.GetMethod("Exists", 1), new_dirExists, old_dirExists);

        DirClass2 = Class("System.IO.Directory", "GetDirectories");
        BasicHook(DirClass2.GetMethod("GetDirectories", 1), new_dirGetDirs, old_dirGetDirs);
    }
};
