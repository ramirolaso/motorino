/*
 * pengine.h
 *
 *  Created on: May 2, 2020
 *      Author: ramiro
 */
#include <stdbool.h>

#ifndef PENGINE_H_
#define PENGINE_H_
struct Point
{
  float x;
  float y;
};

typedef struct Point Point;


struct EntityStructure{
  Point min;
  Point max;

  float restitution;
  float friction;
  float vx;
  float vy;
  float ax;
  float ay;
  int is_jumping;
  int floating_entity;
  int is_collidable;
  int metadata_role; //this can be used freely by game logic, inteded to be used during collision process
};

typedef struct EntityStructure Entity;


struct Circle
{
  float radius;
  Point position;
};


Point calculate_center(Entity entity);
float calculate_height(Entity box);
float calculate_width(Entity box);
float calculate_left_x(Entity box);
float calculate_right_x(Entity box);
float calculate_top_y(Entity box);
float calculate_bottom_y(Entity box);

Point substract_points(Point position_a, Point position_b);
Point sum_points(Point position_a, Point position_b);

Entity create_participant(int width, int height,int pos_x, int pos_y,float restitution,int role_metadata);
Entity create_object(int width, int height,int pos_x, int pos_y,float restitution,float friction,int role_metadata);
Entity create_entity(int min_x, int min_y,int max_x, int max_y,float speed,float restitution,float friction,int floating_entity,int is_collidable,int role_metadata);
Entity create_entity_with_size_and_position(int width, int height,int pos_x, int pos_y,float speed,float restitution,float friction,int floating_entity,int is_collidableint,int role_metadata);

void move_entity_y(Entity *entity,int dy);
void move_entity_x(Entity *entity,int dx);
void move_entity(Entity *entity,int dx, int dy);
void move_entity_to_fixed_pos(Entity *box, int x, int y);

bool detect_entities_collision(Entity a, Entity b );
bool detect_circled_entities_collision( struct Circle a, struct Circle b );




#endif /* PENGINE_H_ */
