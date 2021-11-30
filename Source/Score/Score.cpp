/*!********************************************************************************************************
\File          : Score.h
\Copyright     : GPL-3.0 License
\Brief         : Provides Dcore trackign
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Score.h"
#include "AssetHelper.h"

uint32_t Score::score = 0;

SpriteSheet* sheet = nullptr;
std::vector<Sprite*> sprites(10, nullptr);
AnimationInstance coin;
std::vector<SpriteInstance> instances;

glm::vec2 spriteSize;

void Score::render()
{
    // render the coin
    coin.render();

    for (SpriteInstance& instance : instances) {
        instance.render();
    }
}

/// <summary>Take the current score and turn it into a list of sprites to render</summary>
void Score::parseScore(float uniformWidth)
{
    // Calculate the location of the corner
    glm::vec2 topRight = glm::vec2(uniformWidth / 2.0f, 1.0f);
    // offset it a little bit
    topRight += glm::vec2(-0.05, -0.05);
    topRight += glm::vec2(Camera::getPosition());
    coin.setPosition(glm::vec3(topRight + 0.5f * glm::vec2(-coin.getScale().x, -coin.getScale().y), 0.0));

    instances.clear();

    if (score == 0) {
        // Set a 0 instance
        instances.push_back(SpriteInstance(sprites[0],
                                           coin.getPosition() + glm::vec3(-1 * coin.getScale().x, 0, 0),
                                           glm::vec3(spriteSize, 1.0), glm::vec3(0, 0, 0)));
    } else {
        uint32_t scoreCount = score;
        glm::vec3 lastPos = coin.getPosition();
        float lastWidth = coin.getScale().x;

        while (scoreCount) {
            // get the unit
            uint32_t unit = scoreCount % 10;

            // Add a new score instance
            SpriteInstance scoreInstance(sprites[unit], lastPos - glm::vec3(lastWidth, 0, 0),
                                         glm::vec3(spriteSize, 1.0), glm::vec3(0, 0, 0));
            lastPos = scoreInstance.getPosition();
            lastWidth = scoreInstance.getScale().x;
            instances.push_back(scoreInstance);

            // Get the next unit
            scoreCount /= 10;
        }
    }
}

void Score::init(AnimatedSprite* coinPointer)
{
    sheet = new SpriteSheet("Textures/Numbers.png");
    SpriteInternals::activeSheets.push_back(sheet);
    coin = AnimationInstance(coinPointer);
    coin.setScale(Textures::getTexSize({28, 25}));

    // Store the sprite size since theyre all the same
    spriteSize = Textures::getTexSize({14, 19});

    // Now create all of the sprites
    sprites[0] = new Sprite(sheet, Textures::generateTexCoordinates({0, 0}, {14, 19}, {64, 64}));
    sprites[1] = new Sprite(sheet, Textures::generateTexCoordinates({14, 0}, {14, 19}, {64, 64}));
    sprites[2] = new Sprite(sheet, Textures::generateTexCoordinates({28, 0}, {14, 19}, {64, 64}));
    sprites[3] = new Sprite(sheet, Textures::generateTexCoordinates({41, 0}, {14, 19}, {64, 64}));
    sprites[4] = new Sprite(sheet, Textures::generateTexCoordinates({0, 19}, {14, 19}, {64, 64}));
    sprites[5] = new Sprite(sheet, Textures::generateTexCoordinates({14, 19}, {14, 19}, {64, 64}));
    sprites[6] = new Sprite(sheet, Textures::generateTexCoordinates({28, 19}, {14, 19}, {64, 64}));
    sprites[7] = new Sprite(sheet, Textures::generateTexCoordinates({41, 19}, {14, 19}, {64, 64}));
    sprites[8] = new Sprite(sheet, Textures::generateTexCoordinates({0, 38}, {14, 19}, {64, 64}));
    sprites[9] = new Sprite(sheet, Textures::generateTexCoordinates({14, 38}, {14, 19}, {64, 64}));

    score = 0;
}

void Score::deinit()
{
    score = 0;

    for (Sprite* sprite : sprites) {
        if (sprite) delete sprite;
    }

    if (sheet) delete sheet;
}
