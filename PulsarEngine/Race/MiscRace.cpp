#include <kamek.hpp>
#include <MarioKartWii/Race/racedata.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <MarioKartWii/UI/Ctrl/CtrlRace/CtrlRaceBalloon.hpp>
#include <MarioKartWii/UI/Ctrl/CtrlRace/CtrlRaceResult.hpp>
#include <MarioKartWii/Item/ItemManager.hpp>
#include <MarioKartWii/GlobalFunctions.hpp>
#include <Settings/Settings.hpp>
#include <Info.hpp>

namespace Pulsar {
namespace Race {
//From JoshuaMK, ported to C++ by Brawlbox and adapted as a setting
static int MiiHeads(RaceData* racedata, u32 unused, u32 unused2, u8 id) {
    CharacterId charId = racedata->racesScenario.players[id].characterId;
    const RKNet::RoomType roomType =  RKNet::Controller::sInstance->roomType;
    bool isDisabled = false;
    if(roomType == RKNet::ROOMTYPE_FROOM_HOST || roomType == RKNet::ROOMTYPE_FROOM_NONHOST) {
        isDisabled = System::sInstance->disableMiiHeads;
    }
    if(Settings::Mgr::GetSettingValue(Settings::SETTINGSTYPE_RACE, SETTINGRACE_RADIO_MII) == RACESETTING_MII_ENABLED && !isDisabled) {
        if(charId < MII_M) {
            if(id == 0) charId = MII_M;
            else if(RKNet::Controller::sInstance->connectionState != 0) charId = MII_M;
        }
    }
    return charId;
}
kmCall(0x807eb154, MiiHeads);
kmWrite32(0x807eb15c, 0x60000000);
kmWrite32(0x807eb160, 0x88de01b4);

//credit to XeR for finding the float address
static void BattleGlitchEnable() {
    const bool isEnabled = Settings::Mgr::GetSettingValue(Settings::SETTINGSTYPE_RACE, SETTINGRACE_RADIO_BATTLE) == RACESETTING_BATTLE_GLITCH_ENABLED;
    float maxDistance = 7500.0f;
    if(isEnabled) maxDistance = 75000.0f;
    RaceBalloons::maxDistanceNames = maxDistance;
}
RaceLoadHook BattleGlitch(BattleGlitchEnable);

kmWrite32(0x8085C914, 0x38000000); //times at the end of races in VS
static void DisplayTimesInsteadOfNames(CtrlRaceResult& result, u8 id) {
    result.FillFinishTime(id);
}
kmCall(0x8085d460, DisplayTimesInsteadOfNames); //for WWs

//don't hide position tracker (MrBean35000vr)
kmWrite32(0x807F4DB8, 0x38000001);

//Draggable blue shells
static void DraggableBlueShells(Item::PlayerObj& sub) {
    if(Settings::Mgr::GetSettingValue(Settings::SETTINGSTYPE_RACE, SETTINGRACE_RADIO_BLUES) == RACESETTING_DRAGGABLE_BLUES_DISABLED) {
        sub.isNotDragged = true;
    }
}
kmBranch(0x807ae8ac, DraggableBlueShells);

//Coloured Minimap
kmWrite32(0x807DFC24, 0x60000000);

//No Team Invincibility
kmWrite32(0x8056fd24, 0x38000000); //KartCollision::CheckKartCollision()
kmWrite32(0x80572618, 0x38000000); //KartCollision::CheckItemCollision()
kmWrite32(0x80573290, 0x38000000); //KartCollision::HandleFIBCollision()
kmWrite32(0x8068e2d0, 0x38000000); //PlayerEffects ctor
kmWrite32(0x8068e314, 0x38000000); //PlayerEffects ctor
kmWrite32(0x807a7f6c, 0x38c00000); //FIB are always red
kmWrite32(0x807b0bd4, 0x38000000); //pass TC to teammate
kmWrite32(0x807bd2bc, 0x38000000); //RaceGlobals
kmWrite32(0x807f18c8, 0x38000000); //TC alert


//CtrlItemWindow
kmWrite24(0x808A9C16, 'PUL'); //item_window_new -> item_window_PUL

const char* ChangeItemWindowPane(ItemId id, u32 itemCount) {
    bool feather = Info::IsFeather();
    bool megaTC = Info::IsMegaTC();
    const char* paneName;
    if(id == BLOOPER && feather) {
        if(itemCount == 2) paneName = "feather_2";
        else if(itemCount == 3) paneName = "feather_3";
        else paneName = "feather";
    }
    else if(id == THUNDER_CLOUD && megaTC) paneName = "megaTC";
    else paneName = GetItemIconPaneName(id, itemCount);
    return paneName;
}
kmCall(0x807f3648, ChangeItemWindowPane);
kmCall(0x807ef168, ChangeItemWindowPane);
kmCall(0x807ef3e0, ChangeItemWindowPane);
kmCall(0x807ef444, ChangeItemWindowPane);

kmWrite24(0x808A9FF3, 'PUL');

    // Dont Lose VR when Disconnecting [Bully]
    // kmWrite16(0x04856560, 0x60000000);

    // Hybrid Drift v2 [CLF78, Ismy]
    asmFunc HybridDrift1() {
        ASM(
    lwz r0, 0x14(r3);
    lwz r12, 0(r28);
    lwz r12, 0(r12);
    lwz r12, 0x14(r12);
    lwz r12, 0(r12);
    lwz r12, 0x4(r12);
    cmpwi r12, 2;
    beqlr;
    li r0, 0;
    blr;
        )
    }
    kmCall(0x8057930C, HybridDrift1);
    kmCall(0x80578DCC, HybridDrift1);

    asmFunc HybridDrift3() {
        ASM(
    lwz r0, 0x14(r3);
    rlwinm. r12, r0, 0, 18, 18;
    beq end2;
    ori r0, r0, 0x10;
    stw r0, 0x14(r3);
    end2:
    lwz r0, 0x4(r3);
    blr;
        )
    }
    kmCall(0x8057DFA8, HybridDrift3);

    asmFunc HybridDrift4() {
        ASM(
    lwz r0, 0x14(r4);
    rlwinm. r12, r0, 0, 18, 18;
    beq end2;
    ori r0, r0, 0x10;
    stw r0, 0x14(r4);
    end2:
    lwz r0, 0x4(r4);
    blr;
        )
    }
    kmCall(0x8057E018, HybridDrift4);

    asmFunc HybridDrift6() {
        ASM(
      rlwinm.   r0,r0,0,27,27;
      li        r0, 0;
      stw       r0, 0x1C8(r3);
        )
    }
    kmCall(0x8057E108, HybridDrift6);

    asmFunc HybridDrift7() {
        ASM(
    lwz r3, 0x4(r30);
    andi. r4, r3, 0x84;
    beq end;
    lwz r4, 0x14(r30);
    rlwinm r4, r4, 0, 28, 26;
    stw r4, 0x14(r30);
    rlwinm r3, r3, 0, 4, 2;
    stw r3, 0x4(r30);
    end:
    mr r3, r30;
    blr;
        )
    }
    kmCall(0x80594AA8, HybridDrift7);
    kmWrite8(0x8059450E, 0x00000020);
    kmWrite32(0x80594A60, 0x60000000);
    kmWrite32(0x805A35BC, 0x38600000);
    kmWrite16(0x80745AB0, 0x00004800);
}//namespace Race
}//namespace Pulsar