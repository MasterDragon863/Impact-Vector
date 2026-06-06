#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"
#include "../ECS.h"

inline void RenderSystem(Registry &registry);

inline void Render(Registry &registry)
{
    BeginDrawing();
    ClearBackground(BLACK);
    RenderSystem(registry);
    DrawFPS(10, 10);
    EndDrawing();
}

inline void RenderSystem(Registry &registry)
{
    Position *positions{registry.getComponentArray<Position>()};
    Sprite *sprites{registry.getComponentArray<Sprite>()};

    for (Entity entity{0}; entity < registry.getEntityCount() && entity < registry.getMaxEntities(); ++entity)
    {

        std::uint32_t bitMask{ComponentBit::POSITION | ComponentBit::SPRITE};
        if (registry.EntityHasComponent(entity, bitMask))
        {
            Sprite &sprite{sprites[entity]};
            Position &position{positions[entity]};

            if (sprite.useAtlas)
            {
                DrawTextureRec(*sprite.atlas, sprite.sourceRect, {position.x, position.y}, WHITE);
            }
            else
            {
                Rectangle rect{Rectangle(position.x, position.y, sprite.sourceRect.width, sprite.sourceRect.height)};
                DrawRectanglePro(rect, sprite.origin, sprite.rotation, sprite.debugColor);
            }
        }
    }
}

class Renderer
{
public:
    static void draw(const ECS &ecs)
    {
        BeginDrawing();
        ClearBackground(BLACK);

        const std::size_t totalSprites {ecs.getComponentCount<Sprite>()};

        for (std::size_t i{0}; i < totalSprites; ++i)
        {
            Entity entity {ecs.getEntityFromDenseIndex<Sprite>(i)};

            if (ecs.hasComponent<Position>(entity))
            {
                const Sprite &sprite {ecs.getComponent<Sprite>(entity)};
                const Position &pos {ecs.getComponent<Position>(entity)};

                if (sprite.useAtlas)
                {
                    DrawTextureRec(*sprite.atlas, sprite.sourceRect, {pos.x, pos.y}, WHITE);
                }
                else
                {
                    Rectangle rect{Rectangle(pos.x, pos.y, sprite.sourceRect.width, sprite.sourceRect.height)};
                    DrawRectanglePro(rect, sprite.origin, sprite.rotation, sprite.debugColor);
                }
            }
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

};