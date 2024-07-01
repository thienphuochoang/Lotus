-- Define a table with the values of the first level
Level = {
    ----------------------------------------------------
    -- Table to define the list of assets
    ----------------------------------------------------
    assets = {
        [0] =
        { type = "texture", id = "tilemap-texture", file = "./assets/tilemaps/jungle.png" },
        { type = "texture", id = "soldier-1-texture", file = "./assets/images/soldier1.png" },
        { type = "texture", id = "soldier-2-texture", file = "./assets/images/soldier2.png" },
        { type = "texture", id = "bullet-texture",  file = "./assets/images/bullet.png" },
		{ type = "texture", id = "container-texture",  file = "./assets/images/Container_A.png" },
        { type = "font"   , id = "pico8-font",    file = "./assets/fonts/pico8.ttf", font_size = 10 }
    },

    ----------------------------------------------------
    -- table to define the map config variables
    ----------------------------------------------------
    tilemap = {
        map_file = "./assets/tilemaps/jungle.map",
        texture_asset_id = "tilemap-texture",
        num_rows = 20,
        num_cols = 25,
        tile_size = 32,
        scale = 2.0
    },

    ----------------------------------------------------
    -- table to define entities and their components
    ----------------------------------------------------
    entities = {
        [0] =
        {
            -- Player
            tag = "player",
            components = {
                transform = {
                    position = { x = 10.0, y = 10.0 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "soldier-1-texture",
                    width = 64,
                    height = 64,
                    z_index = 1,
                    fixed = false,
                    src_rect_x = 0,
                    src_rect_y = 0
                },
                animation = {
                    num_frames = 8,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 32,
                    offset = { x = 16, y = 16 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 150, y = 150 },
                    projectile_duration = 5, -- seconds
                    repeat_frequency = 0, -- seconds
                    hit_percentage_damage = 10,
                    friendly = true
                },
                keyboard_controller = {
                    up_velocity = { x = 0, y = -100 },
					down_velocity = { x = 0, y = 100 },
					left_velocity = { x = -100, y = 0 },
                    right_velocity = { x = 100, y = 0 }
                },
                camera_follow = {
                    follow = true
                }
            }
        },
        {
            group = "enemies",
            components = {
                transform = {
                    position = { x = 500.0, y = 200.0 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = -90.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "soldier-2-texture",
                    width = 64,
                    height = 64,
                    z_index = 2
                },
                boxcollider = {
                    width = 32,
                    height = 32,
                    offset = { x = 16, y = 16 }
                },
				animation = {
                    num_frames = 8,
                    speed_rate = 10 -- fps
                },
                health = {
                    health_percentage = 100
                },
				rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                projectile_emitter = {
                    projectile_velocity = { x = 100, y = 0 },
                    projectile_duration = 5, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 20,
                    friendly = false
                }
            }
        }
		{
            group = "obstacles",
            components = {
                transform = {
                    position = { x = 800.0, y = 200.0 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "container-texture",
                    width = 256,
                    height = 128,
                    z_index = 2
                },
                boxcollider = {
                    width = 256,
                    height = 128,
                    offset = { x = 0, y = 0 }
                }
            }
        }
    }
}