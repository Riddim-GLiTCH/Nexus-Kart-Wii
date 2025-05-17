#include <kamek.hpp>
#include <MarioKartWii/UI/Page/Other/GlobeSearch.hpp>
#include <PulsarSystem.hpp>
#include <UI/UI.hpp>

kmWrite32(0x80609268, 0x7f63db78);
namespace Pulsar {
namespace UI {

void PatchGlobeSearchBMG(Pages::GlobeSearch* globeSearch) {
    globeSearch->countdown.Update();
    if(Pulsar::System::sInstance->deniesCount >= 3) globeSearch->messageWindow.LayoutUIControl::SetMessage(UI::BMG_TOO_MANY_DENIES);
}
kmCall(0x8060926c, PatchGlobeSearchBMG);

// Remove WW Button [Chadderz]
kmWrite16(0x8064B982, 0x00000005);
kmWrite32(0x8064BA10, 0x60000000);
kmWrite32(0x8064BA38, 0x60000000);
kmWrite32(0x8064BA50, 0x60000000);
kmWrite32(0x8064BA5C, 0x60000000);
kmWrite16(0x8064BC12, 0x00000001);
kmWrite16(0x8064BC3E, 0x00000484);
kmWrite16(0x8064BC4E, 0x000010D7);
kmWrite16(0x8064BCB6, 0x00000484);
kmWrite16(0x8064BCC2, 0x000010D7);

//Instant Course Voting [Ro]
kmWrite32(0x80643BC4, 0x60000000);
kmWrite32(0x80643C2C, 0x60000000);

//Always Show Timer After Online Race Results [Sponge]
kmWrite32(0x8064DB2C, 0x60000000);

//WW Bubble Skip [Ro]
kmWrite32(0x806075F4, 0x48000060);
kmWrite8(0x80609647, 0x0000003C);
kmWrite8(0x8060964F, 0x00000010);
kmWrite32(0x8060988C, 0x60000000);

}//namespace UI
}//namespace Pulsar