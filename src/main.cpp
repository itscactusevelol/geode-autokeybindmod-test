#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <windows.h>

using namespace geode::prelude;

bool isTriggered = false;

// Helper function to simulate pressing and releasing a key
void simulateKeypress(WORD vKey) {
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    // Press down
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vKey;

    // Release
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = vKey;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

class $modify(PlayLayer) {
    void postUpdate(float p0) {
        PlayLayer::postUpdate(p0);
        
        if (!Mod::get()->getSettingValue<bool>("enable-mod")) return;

        // Prevent division by zero at the very start of the level
        if (this->m_levelLength == 0) return;

        // Calculate percentage
        int percent = (this->m_player1->getPositionX() / this->m_levelLength) * 100;
        int triggerPercent = Mod::get()->getSettingValue<int>("percentage");

        if (percent >= triggerPercent && !isTriggered) {
            simulateKeypress(VK_F9); // Simulates F9
            isTriggered = true;
            Notification::create("Auto-Keybind Triggered!", NotificationIcon::Success)->show();
        }
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        PlayLayer::destroyPlayer(player, object);
        if (isTriggered) {
            simulateKeypress(VK_F9); 
            isTriggered = false;
        }
    }

    void levelComplete() {
        PlayLayer::levelComplete();
        if (isTriggered) {
            simulateKeypress(VK_F9); 
            isTriggered = false;
        }
    }

    void onQuit() {
        if (isTriggered) {
            simulateKeypress(VK_F9); 
            isTriggered = false;
        }
        PlayLayer::onQuit();
    }
};
