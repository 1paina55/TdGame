#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "vector2.h"
#include "timer.h"
#include "route.h"
#include "animation.h"
#include "config_manager.h"
#include <functional>

class Enemy
{
public:
	typedef std::function<void(Enemy* enemy)> SkillCallback;
public:
	Enemy() 
	{
		timer_skill.set_one_shot(false);
		timer_skill.set_on_timeout([&] {on_skill_relasesd(this); });

		timer_sketch.set_one_shot(true);
		timer_sketch.set_wait_time(0.075);
		timer_sketch.set_on_timeout([&]() {is_show_sketch = false; });

		timer_restore_speed.set_one_shot(true);
		timer_restore_speed.set_on_timeout([&]() {speed = max_speed; });


	}
	
	~Enemy() = default;

	void on_update(double delta)
	{
		timer_skill.on_update(delta);
		timer_sketch.on_update(delta);
		timer_restore_speed.on_update(delta);

		Vector2 move_distance = velocity * delta;
		Vector2 target_distance = position_target - position;
		position += move_distance < target_distance ? move_distance : target_distance;

		if (target_distance.apprcx_zero())
		{
			idx_target++;
			refresh_position_target();

			direction = (position_target - position).normolize();
		}

		velocity.x = direction.x * speed * SIZE_TILE;  
	}
	
protected:
	Vector2 size;

	Timer timer_skill;

	Animation anim_up;
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;
	Animation anim_up_sketch;
	Animation anim_down_sketch;
	Animation anim_left_sketch;
	Animation anim_right_sketch;

	double hp = 0;
	double max_hp = 0;
	double speed = 0;
	double max_speed = 0;
	double damage = 0;
	double reward_rate = 0;
	double recover_interval = 0;
	double recover_intensity = 0;

private:
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;

	bool is_vaild = true;

	Timer timer_sketch;
	bool is_show_sketch = false;

	Animation* anima_current = nullptr;

	SkillCallback on_skill_relasesd;

	Timer timer_restore_speed;

	const Route* route = nullptr;
	int idx_target = 0;
	Vector2 position_target;

private:
	void refresh_position_target()
	{
		const Route::IdxList& idx_list = route->get_idx_list();

		if (idx_target < idx_list.size())
		{
			const SDL_Point& point = idx_list[idx_target];
			static const const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;

			position_target.x = rect_tile_map.x + point.x * SIZE_TILE + SIZE_TILE / 2;
			position_target.y = rect_tile_map.y + point.y * SIZE_TILE + SIZE_TILE / 2;
		}
	}
};



#endif // !_ENEMY_H_
