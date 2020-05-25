/*
 * motorino_world.h
 *
 *  Created on: May 3, 2020
 *      Author: ramiro
 *  TODO: Eliminar entitidades
 */
#include "motorino_basics.h"
#include "motorino_collisions.h"

#ifndef MOTORINO_WORLD_H_
#define MOTORINO_WORLD_H_


struct EntityNode{
	Entity * entity;
	struct EntityNode *next;
	struct EntityNode *previous;
};
typedef struct EntityNode EntityNode;

struct CollisionNode{
	CollisionInfo *info;
	Entity *entity;
	struct CollisionNode *previous;
};
typedef struct CollisionNode CollisionNode;

struct World
{
	double frame_time;
	int gravity;
	float friction;
	size_t n_entities;
	EntityNode *firstEntity;
	EntityNode *lastEntity;
	int cantEntities;
	CollisionNode *last_colission;
};
typedef struct World World;


void init_world (World * world,double frame_time,int gravity, float friction);
void move_world(World *world,int dx, int dy);
void add_entity_to_world(World *world, Entity *entity);
void remove_entity(World *world, Entity *entity);
void calculate_state(World *world);

void add_collision(World *world, Entity *entity,CollisionInfo *collision_info);
void destroy_collision_stack(World *world);

void calculate_collisions(World *world);
void process_collisions(World *world,void (*collision_game_process)(World *world, CollisionInfo *info));


void destroy_world(World *world);


#endif /* MOTORINO_WORLD_H_ */
