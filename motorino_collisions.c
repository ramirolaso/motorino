/*
 * jsengine.c
 *
 *  Created on: May 2, 2020
 *      Author: ramiro
 */

#include "motorino_collisions.h"

CollisionInfo resolve_elastic(Entity *collidable, Entity *otherEntity) {

	Point collidable_center = calculate_center(*collidable);
	Point entity_center = calculate_center(*otherEntity);

	int entity_width = calculate_width(*otherEntity);
	int entity_width_half = entity_width * 0.5;

	int entity_height = calculate_height(*otherEntity);
	int entity_height_half = entity_height * 0.5;

	int player_width = calculate_width(*collidable);
	int player_width_half = player_width * 0.5;

	int player_height = calculate_height(*collidable);
	int player_height_half = player_height * 0.5;

	int entity_left = calculate_left_x(*otherEntity);
	int entity_right = calculate_right_x(*otherEntity);
	int entity_top = calculate_top_y(*otherEntity);
	int entity_bottom = calculate_bottom_y(*otherEntity);

	int collidable_left = calculate_left_x(*collidable);
	int collidable_right = calculate_right_x(*collidable);
	int collidable_top = calculate_top_y(*collidable);
	int collidable_bottom = calculate_bottom_y(*collidable);

	float dx = (entity_center.x - collidable_center.x) / entity_width_half;
	float dy = (entity_center.y - collidable_center.y) / entity_height_half;

	float abs_dx = fabs(dx);
	float abs_dy = fabs(dy);



	// If the distance between the normalized x and y
	// position is less than a small threshold (.1 in this case)
	// then this object is approaching from a corner
	float diff = fabs(abs_dx - abs_dy);
	CollisionInfo info;
	if (diff < 0.01) {
		info =  resolve_corner_elastic_collision(dx, dy, collidable->vx,collidable->vy,collidable->ax,collidable->ay,
				collidable_center, collidable_left, collidable_right,
				collidable_top, collidable_bottom, entity_center,
				entity_right, entity_left, entity_top,
				entity_bottom, collidable->restitution,
				otherEntity->restitution);

		// If the object is approaching from the sides
	} else if (abs_dx > abs_dy) {
		info = resolve_sides_elastic_collision(dx, dy, collidable->vx,collidable->vy,collidable->ax,collidable->ay,
				collidable_center, collidable_left, collidable_right,
				 entity_center,entity_right, entity_left,collidable->restitution,
				otherEntity->restitution);
	}else{
		info = resolve_bottom_top_elastic_collision(dx, dy, collidable->vx,collidable->vy,collidable->ax,collidable->ay,
				collidable_center,collidable_top, collidable_bottom, entity_center, entity_top,
				entity_bottom, collidable->restitution,
				otherEntity->restitution);
	}

	    move_entity_to_fixed_pos(collidable, info.new_x, info.new_y);
		//nuevas velocidades y aceleracion
		collidable->vx = info.new_vx;
		collidable->vy = info.new_vy;
		collidable->ax = info.new_ax;
		collidable->ay = info.new_ay;
		//esto no deberia decidirse aca
		if (info.floor_collision == 1) {
			collidable->is_jumping = 0;
		}

	return info;

}
CollisionInfo resolve_sides_elastic_collision(int dx, int dy, float vx,float vy, float ax,float ay,
		Point collidable_center, int collidable_left, int collidable_right,
		 Point entity_center, int entity_right, int entity_left,
	    float collidable_restitution,
		float entity_restitution) {

	int penetration;
	int new_y = collidable_center.y;
	int new_x = collidable_center.x;
	float new_vx = vx;
	float new_vy  = vy;
	float new_ax = ax;
	float new_ay = ay;
	int floor_collision=0;
	CollisionInfo info = {};
	// If the player is approaching from positive X
	if (dx < 0) {
	    penetration = entity_right - collidable_left;
		new_x = collidable_center.x + abs(penetration);
		info.collision_type = LEFT;

	} else {
		// If the player is approaching from negative X
		penetration = entity_left - collidable_right;
		new_x = collidable_center.x - abs(penetration);
		info.collision_type = RIGHT;
	}
	//calculamos el rebote
	new_vx = -(vx * (collidable_restitution + entity_restitution));
	if (abs(new_vx) < 2) {
		new_vx = 0;
	}

	info.new_x=new_x;
	info.new_y = new_y;
	info.new_ax = new_ax;
	info.new_ay = new_ay;
	info.new_vy = new_vy;
	info.new_vx=new_vx;
	info.floor_collision = floor_collision;

	return info;

}
CollisionInfo resolve_bottom_top_elastic_collision(int dx, int dy, float vx,float vy, float ax,float ay,
		Point collidable_center, int collidable_top, int collidable_bottom, Point entity_center,
	    int entity_top,int entity_bottom,float collidable_restitution,
		float entity_restitution) {

	int penetration;
	int new_y = collidable_center.y;
	int new_x = collidable_center.x;
	float new_vx = vx;
	float new_vy  = vy;
	float new_ax = ax;
	float new_ay = ay;
	int floor_collision=0;
	CollisionInfo info = {};

	if (dy < 0) {
		penetration = entity_bottom - collidable_top;
		new_y = collidable_center.y + abs(penetration);
		info.collision_type = TOP;

	} else {
		// If the player is approaching from negative Y
		penetration = entity_top - collidable_bottom;
		new_y = collidable_center.y - abs(penetration);

		//ver de modelar de otra manera
		floor_collision=1;
		//paramos la aceleracion en y
		new_ay = 0;
		info.collision_type = BOTTOM;
	}
	//calculamos el rebote

	new_vy = -(new_vy * (collidable_restitution+entity_restitution));
	if (abs(new_vy) < 2) {
		new_vy = 0;
	}

	info.new_x=new_x;
	info.new_y = new_y;
	info.new_ax = new_ax;
	info.new_ay = new_ay;
	info.new_vy = new_vy;
	info.new_vx=new_vx;
	info.floor_collision = floor_collision;

	return info;
}
CollisionInfo resolve_corner_elastic_collision(int dx, int dy, float vx,float vy, float ax,float ay,
		Point collidable_center, int collidable_left, int collidable_right,
		int collidable_top, int collidable_bottom, Point entity_center,
		int entity_right, int entity_left, int entity_top,
		int entity_bottom,float collidable_restitution,
		float entity_restitution) {

	int penetration;
	int new_y = collidable_center.y;
	int new_x = collidable_center.x;
	float new_vx = vx;
	float new_vy  = vy;
	float new_ax = ax;
	float new_ay = ay;
	CollisionInfo info = {};
	// If the player is approaching from positive X
	if (dx < 0) {
		// Set the player x to the right side
		int penetration = entity_right - collidable_left;
		new_x = collidable_center.x + abs(penetration);

		// If the player is approaching from negative X
	} else {

		// Set the player x to the left side
		//player.x = entity.getLeft() - player.width;
		int penetration = entity_left - collidable_right;
		new_x = collidable_center.x - abs(penetration);
	}
	// If the player is approaching from positive Y
	if (dy < 0) {
		int penetration = entity_bottom - collidable_top;
		new_y = collidable_center.y + abs(penetration);

		// If the player is approaching from negative Y
	} else {
		int penetration = entity_top - collidable_bottom;
		new_y = collidable_center.y - abs(penetration);
	}


	info.new_x=new_x;
	info.new_y = new_y;
	info.new_ax = new_ax;
	info.new_ay = new_ay;
	info.new_vy = new_vy;
	info.new_vx=new_vx;
	info.floor_collision = 0;
	info.collision_type = CORNER;

	return info;
}

