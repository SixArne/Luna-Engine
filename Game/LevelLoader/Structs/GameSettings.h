#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

namespace Galaga
{
    struct PointSettings
    {
        int BEE_IN_FORMATION            {50};
        int BEE_DIVING                  {100};
        int BUTTERFLY_IN_FORMATION      {80};
        int BUTTERFLY_DIVING            {160};
        int BOSS_GALAGA_IN_FORMATION    {150};
        int BOSS_GALAGA_DIVING          {400};
    };

    struct EnemyHealthSettings
    {
        int BEE                         {1};
        int BUTTERFLY                   {1};
        int BOSS_GALAGA                 {2};
    };

    struct EnemySpeedSettings
    {
        int BEE                         {1};
        int BUTTERFLY                   {1};
        int BOSS_GALAGA                 {1};
    };

    struct PlayerSettings
    {
        int HEALTH                      {3};
        int BULLETS_PER_SCREEN          {2};
    };

    struct GameSettings
    {
        PointSettings points            {};
        EnemyHealthSettings enemyHealth {};
        EnemySpeedSettings enemySpeed   {};
        PlayerSettings playerSettings   {};
    };
}

#endif