#include "raylib.h"
#include "../Registry.h"
#include "../Components/Components.h"
#include "../ECS.h"

inline void RenderSystem(Registry &registry);

inline void Render(Registry &registry, RenderTexture2D target)
{
    BeginTextureMode(target);
    ClearBackground(BLACK);
    RenderSystem(registry);
    DrawFPS(10, 10);
    EndTextureMode();
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
    static void draw(const ECS &ecs, RenderTexture2D target)
    {
        BeginTextureMode(target);
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
        EndTextureMode();
        BeginDrawing();
        ClearBackground(BLACK); 

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        float scale = (((float)screenWidth/target.texture.width) < ((float)screenHeight/target.texture.height)) 
                      ? ((float)screenWidth/target.texture.width) 
                      : ((float)screenHeight/target.texture.height);

        DrawTexturePro(
            target.texture, 
            (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
            (Rectangle){ 
                (screenWidth - ((float)target.texture.width * scale)) * 0.5f, 
                (screenHeight - ((float)target.texture.height * scale)) * 0.5f, 
                (float)target.texture.width * scale, 
                (float)target.texture.height * scale 
            },
            (Vector2){ 0, 0 }, 
            0.0f, 
            WHITE
        );
    EndDrawing();
    }

};