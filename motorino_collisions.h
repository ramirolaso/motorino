/*
 * jsengine.h
 *
 *  Created on: May 2, 2020
 *      Author: ramiro
 */
#include <stdlib.h>
#include "motorino_basics.h"
#ifndef JSENGINE_H_
#define JSENGINE_H_

enum CollisionType {LEFT,RIGHT,TOP,BOTTOM,CORNER};
typedef enum CollisionType CollisionType;

struct CollisionInfo {

	int new_x;
	int new_y;
	float new_vx;
	float new_vy;
	float new_ax;
	float new_ay;
	int floor_collision;
	Entity *affected_entity; // if a car crashes a wall, the would be the affected entity.
	Entity *other_entity; // if a car crashes a wall, the wall would be the other entity.
	CollisionType collision_type; //from affected entity perspective


};
typedef struct CollisionInfo CollisionInfo;

CollisionInfo resolve_elastic(Entity *player, Entity *entity);
CollisionInfo resolve_sides_elastic_collision(int dx, int dy, float vx,float vy, float ax,float ay,
		Point collidable_center, int collidable_left, int collidable_right,
		 Point entity_center, int entity_right, int entity_left,
	    float collidable_restitution,
		float entity_restitution);
CollisionInfo resolve_bottom_top_elastic_collision(int dx, int dy, float vx,float vy, float ax,float ay,
		Point collidable_center, int collidable_top, int collidable_bottom, Point entity_center,
	    int entity_top,int entity_bottom,float collidable_restitution,
		float entity_restitution);
CollisionInfo resolve_corner_elastic_collision(int dx, int dy, float vx,float vy, float ax,float ay,
		Point collidable_center, int collidable_left, int collidable_right,
		int collidable_top, int collidable_bottom, Point entity_center,
		int entity_right, int entity_left, int entity_top,
		int entity_bottom,float collidable_restitution,
		float entity_restitution);

#endif /* JSENGINE_H_ */
