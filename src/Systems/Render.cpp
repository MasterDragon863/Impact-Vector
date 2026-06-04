#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"

inline void RenderSystem(const Registry& registry);

inline void Render(const Registry& registry)
{
    BeginDrawing();
    ClearBackground(BLACK);
    RenderSystem(registry);
    EndDrawing();
}

inline void RenderSystem(const Registry& registry)
{
    for (Entity entity = 0; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {
        
        std::uint32_t bitMask = ComponentBit::POSITION | ComponentBit::SPRITE;
        if (registry.EntityHasComponent(entity, bitMask)) {
            const Position position = registry.getComponentArray<Position>()[entity];
            const Sprite sprite = registry.getComponentArray<Sprite>()[entity];
            if (sprite.useAtlas) {
                DrawTextureRec(*sprite.atlas, sprite.sourceRect, {position.x, position.y}, WHITE);
            }
            else {
                DrawRectangle(position.x, position.y, sprite.sourceRect.width, sprite.sourceRect.height, sprite.debugColor);
            }
        }
    }
}