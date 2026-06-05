#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"

inline void RenderSystem(Registry& registry);

inline void Render(Registry& registry)
{
    BeginDrawing();
    ClearBackground(BLACK);
    RenderSystem(registry);
    DrawFPS(10, 10);
    EndDrawing();
}

inline void RenderSystem(Registry& registry)
{
    Position *positions {registry.getComponentArray<Position>()};
    Sprite *sprites {registry.getComponentArray<Sprite>()};

    for (Entity entity{0}; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        
        std::uint32_t bitMask {ComponentBit::POSITION | ComponentBit::SPRITE};
        if (registry.EntityHasComponent(entity, bitMask)) {
            Sprite &sprite {sprites[entity]};
            Position &position {positions[entity]};

            if (sprite.useAtlas) {
                DrawTextureRec(*sprite.atlas, sprite.sourceRect, {position.x, position.y}, WHITE);
            }
            else {
                Rectangle rect{Rectangle(position.x, position.y, sprite.sourceRect.width, sprite.sourceRect.height)};
                DrawRectanglePro(rect, sprite.origin, sprite.rotation, sprite.debugColor);
                
            }
        }
    }
}