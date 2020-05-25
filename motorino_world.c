/*
 * motorino_world.c
 *
 *  Created on: May 3, 2020
 *      Author: ramiro
 */

#include "motorino_world.h"
void apply_collision(World *world, EntityNode *this);
void apply_forces(World *world, Entity *e);

void calculate_state(World *world) {
	EntityNode *firstNode = world->firstEntity;
	apply_forces(world, firstNode->entity);
	EntityNode *node = firstNode->next;
	while (node != NULL) {
		apply_forces(world, node->entity);
		node = node->next;
	}
}
void apply_forces(World *world, Entity *e) {

	int signe = -1;
	if (e->vx >= 0)
		signe = 1;

	if (fabs(e->vx) > world->friction) {
		e->vx = (fabs(e->vx) - world->friction) * signe;
	} else {
		e->vx = 0;
	}

	if (e->floating_entity == 0)
		e->ay += world->gravity;

	e->vy += e->ay * world->frame_time;

	move_entity(e, e->vx, e->vy);
}
void calculate_collisions(World *world) {

	EntityNode *firstNode = world->firstEntity;
	apply_collision(world, firstNode);
	EntityNode *node = firstNode->next;
	while (node != NULL) {
		apply_collision(world, node);
		node = node->next;
	}
}
void add_collision(World *world, Entity *entity, CollisionInfo *collision_info) {
	CollisionNode *new_node = malloc(sizeof(CollisionNode));
	new_node->entity = entity;
	new_node->info = collision_info;
	new_node->previous = NULL;
	world->last_colission = new_node;
}

void apply_collision(World *world, EntityNode *this) {

	if (this->entity->is_collidable == 0)
		return;
	EntityNode *head = world->firstEntity;
	while (head != NULL) {
		if (this != head) {
			if (detect_entities_collision(*this->entity, *head->entity)) {
				CollisionInfo info = resolve_elastic(this->entity,
						head->entity);
				info.affected_entity = this->entity;
				info.other_entity = head->entity;
				add_collision(world, this->entity, &info);
			}
		}
		head = head->next;
	}

}

void init_world(World *world, double frame_time, int gravity, float friction) {
	world->n_entities = 0;
	world->friction = friction;
	world->gravity = gravity;
	world->frame_time = frame_time;
	world->lastEntity = NULL;
	world->firstEntity = NULL;
	world->last_colission = NULL;

}

void move_world(World *world, int dx, int dy) {
	EntityNode *head = world->firstEntity;
	while (head != NULL) {
		move_entity_x(head->entity, dx);
		move_entity_y(head->entity, dy);
		head = head->next;
	}
}
void add_entity_to_world(World *world, Entity *entity) {
	EntityNode *nextNode = malloc(sizeof(EntityNode));
	nextNode->entity = entity;
	nextNode->next = NULL;
	nextNode->previous = world->lastEntity;
	world->cantEntities += 1;
	if (world->firstEntity == NULL) {
		world->firstEntity = nextNode;
		world->firstEntity->previous = NULL;
		return;
	}
	if (world->lastEntity == NULL) {
		world->lastEntity = nextNode;
		world->lastEntity->previous = world->firstEntity;
		world->firstEntity->next = nextNode;

		return;
	}

	world->lastEntity->next = nextNode;
	world->lastEntity = nextNode;
}
void remove_entity(World *world, Entity *entity) {
	EntityNode *head = world->firstEntity;
	EntityNode *tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		if (tmp->entity == entity) {

			tmp->previous->next = head;
			if (tmp == world->lastEntity && tmp->previous != NULL){
				world->lastEntity = tmp->previous;
			}else if(tmp == world->lastEntity && tmp->previous == NULL){
				world->lastEntity =NULL;
			}
			free(tmp);
		}
	}
	head = NULL;
}

void process_collisions(World *world,
		void (*collision_game_process)(World *world, CollisionInfo *info)) {
	CollisionNode *last = world->last_colission;
	CollisionNode *tmp;
	while (last != NULL) {
		tmp = last;
		collision_game_process(world, tmp->info);
		last = last->previous;
		free(tmp);
	}
	world->last_colission = NULL;

}

void destroy_world(World *world) {

	EntityNode *head = world->firstEntity;
	EntityNode *tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
	head = NULL;

}

