#include <cassert>

#include "../Components/Components.h"
#include "../ECS.h"
#include "raylib.h"

/**
 * @class Renderer
 * @brief Static class that handles the rendering
 *
 */
class Renderer {
 public:
  /**
   * @brief Static funtion that draws to a target
   *
   * Loops through every sprite
   * Checks if it uses the atlas
   * Then uses the appropriate api call to raylib
   *
   * @param ecs - Reference to the parent ecs
   * @param target - reference to the target you want to draw to
   */
  static void draw(const ECS& ecs, RenderTexture2D& target) {
    BeginTextureMode(target);
    ClearBackground(BLACK);

    const std::size_t totalSprites{ecs.getComponentCount<Sprite>()};

    for (std::size_t i{0}; i < totalSprites; ++i) {
      Entity entity{ecs.getEntityFromDenseIndex<Sprite>(i)};

      assert(ecs.hasComponent<Position>(entity));

      const Sprite& sprite{ecs.getComponent<Sprite>(entity)};
      const Position& pos{ecs.getComponent<Position>(entity)};

      if (sprite.useAtlas) {
        DrawTextureRec(*sprite.atlas, sprite.sourceRect, {pos.x, pos.y}, WHITE);
      } else {
        Rectangle rect{pos.x, pos.y, sprite.sourceRect.width,
                       sprite.sourceRect.height};
        DrawRectanglePro(rect, sprite.origin, sprite.rotation,
                         sprite.debugColor);
      }
    }

    DrawFPS(10, 10);
    EndTextureMode();
  }

  /**
   * @brief scales the target to the screen resolution
   *
   * Scales the target based on a ration calculated width the screen width and
   * height in reference to the targets width and height
   *
   * @param target - reference to the target to be scaled up
   */
  static void scaleToScreen(RenderTexture2D& target) {
    BeginDrawing();
    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    float scale = (((float)screenWidth / target.texture.width) <
                   ((float)screenHeight / target.texture.height))
                      ? ((float)screenWidth / target.texture.width)
                      : ((float)screenHeight / target.texture.height);

    DrawTexturePro(
        target.texture,
        (Rectangle){0.0f, 0.0f, (float)target.texture.width,
                    (float)-target.texture.height},
        (Rectangle){
            (screenWidth - ((float)target.texture.width * scale)) * 0.5f,
            (screenHeight - ((float)target.texture.height * scale)) * 0.5f,
            (float)target.texture.width * scale,
            (float)target.texture.height * scale},
        (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
  }
};
