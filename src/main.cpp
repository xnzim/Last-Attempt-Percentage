#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

static float s_lastAttemptPercent = 0.0f;

// Helper function to format percentage based on settings
std::string formatPercentage(float percent) {
    bool showDecimals = Mod::get()->getSettingValue<bool>("show-decimals");

    if (showDecimals) {
        return fmt::format("Last: {:.2f}%", percent);
    }
    else {
        return fmt::format("Last: {:.0f}%", percent);
    }
}

// Helper function to get position based on setting
CCPoint getPositionForLabel(const std::string& position, const CCSize& winSize) {
    float padding = 10.0f;

    if (position == "Top Right") {
        return ccp(winSize.width - 10.0f, winSize.height - 0.0f);
    } else if (position == "Top Middle") {
        return ccp(winSize.width / 2, winSize.height - 20.0f);  // Below progress bar
    } else if (position == "Middle Left") {
        return ccp(10.0f, winSize.height / 2);
    } else if (position == "Middle Right") {
        return ccp(winSize.width - 10.0f, winSize.height / 2);
    } else if (position == "Bottom Left") {
        return ccp(10.0f, padding);
    } else if (position == "Bottom Middle") {
        return ccp(winSize.width / 2, padding);
    } else if (position == "Bottom Right") {
        return ccp(winSize.width - 10.0f, padding);
    }

    // Default to top right if unknown
    return ccp(winSize.width - 10.0f, winSize.height - 0.0f);
}

// Helper function to get anchor point based on position
CCPoint getAnchorForPosition(const std::string& position) {
    if (position.find("Right") != std::string::npos) {
        return ccp(1.0f, position.find("Bottom") != std::string::npos ? 0.0f :
                         position.find("Top") != std::string::npos ? 1.0f : 0.5f);
    } else if (position.find("Left") != std::string::npos) {
        return ccp(0.0f, position.find("Bottom") != std::string::npos ? 0.0f :
                         position.find("Top") != std::string::npos ? 1.0f : 0.5f);
    } else { // Middle
        return ccp(0.5f, position.find("Bottom") != std::string::npos ? 0.0f :
                         position.find("Top") != std::string::npos ? 1.0f : 0.5f);
    }
}

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields {
        CCLabelBMFont* lastPercentLabel = nullptr;
        bool m_hasDied = false;
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

        // Reset the % when entering a new level
        s_lastAttemptPercent = 0.0f;

        // Create the last attempt percentage label with saved value
        std::string labelText = formatPercentage(s_lastAttemptPercent);
        m_fields->lastPercentLabel = CCLabelBMFont::create(labelText.c_str(), "bigFont.fnt");
        if (m_fields->lastPercentLabel) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();

            // Get position setting and set label position accordingly
            std::string position = Mod::get()->getSettingValue<std::string>("label-position");
            m_fields->lastPercentLabel->setPosition(getPositionForLabel(position, winSize));
            m_fields->lastPercentLabel->setAnchorPoint(getAnchorForPosition(position));

            m_fields->lastPercentLabel->setScale(0.5f);
            m_fields->lastPercentLabel->setOpacity(180);

            this->addChild(m_fields->lastPercentLabel, 1000); // High z-order to ensure it's on top
        }

        return true;
    }

    void destroyPlayer(PlayerObject * player, GameObject * object) {
        PlayLayer::destroyPlayer(player, object);

        // Set a flag indicating the player has died
        m_fields->m_hasDied = true;
    }

    void resetLevel() {
        // Only record the percentage if the player has actually died in the previous attempt
        if (m_fields->m_hasDied) {
            float currentPercent = this->getCurrentPercent();

            // Only store the percentage if there was actual progress
            if (currentPercent > 0.05f) {
                s_lastAttemptPercent = currentPercent;
            }

            // Update the label to show the stored percentage
            if (m_fields->lastPercentLabel) {
                std::string labelText = formatPercentage(s_lastAttemptPercent);
                m_fields->lastPercentLabel->setString(labelText.c_str());
            }
        }

        PlayLayer::resetLevel();
        // Reset the death flag for the new attempt
        m_fields->m_hasDied = false;
    }
};