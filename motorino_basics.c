#include "motorino_basics.h"




Point calculate_center(Entity Box)
{
  int x = Box.min.x + ((Box.max.x - Box.min.x)*0.5);
  int y = Box.min.y + ((Box.max.y - Box.min.y)*0.5);
  Point point = {x,y};
  return point;
}
float calculate_height(Entity box)
{
	return box.max.y - box.min.y;
}
float calculate_width(Entity box)
{
	return box.max.x - box.min.x;
}

float calculate_left_x(Entity box)
{
	 return calculate_center(box).x - (calculate_width(box)*0.5);
}
float calculate_right_x(Entity box)
{
	return calculate_center(box).x + (calculate_width(box)*0.5);
}
float calculate_top_y(Entity box)
{
	return calculate_center(box).y - (calculate_height(box)*0.5);
}
float calculate_bottom_y(Entity box)
{
	return calculate_center(box).y + (calculate_height(box)*0.5);
}

Point substract_points(Point position_a, Point position_b)
{
  Point point_final = {position_b.x-position_a.x,position_b.y-position_a.y};
  return point_final;

}
Point sum_points(Point position_a, Point position_b)
{
  Point point_final = {position_b.x+position_a.x,position_b.y+position_a.y};
  return point_final;

}

Entity create_entity(int min_x, int min_y,int max_x, int max_y,float speed,float restitution,float friction,int floating_entity,int is_collidable,int role_metadata)
{
  Point min = {min_x,min_y};
  Point max = {max_x,max_y};
  Entity e = {min,max,restitution,friction,speed,speed,0,0,1,floating_entity,is_collidable,role_metadata};
  return e;
}
void move_entity_x(Entity *Box,int dx)
{
  Box->min.x += dx;
  Box->max.x += dx;
}
void move_entity_y(Entity *Box,int dy)
{
  Box->max.y += dy;
  Box->min.y += dy;
}
void move_entity(Entity *Box,int dx, int dy)
{
  move_entity_x(Box,dx);
  move_entity_y(Box,dy);
}
void move_entity_to_fixed_pos(Entity *box, int x, int y)
{
	Point  center = calculate_center(*box);
	int dx =  x - center.x;
	int dy =  y - center.y;
	move_entity(box,dx,dy);
}

Entity create_entity_with_size_and_position(int width, int height,int pos_x, int pos_y,
		float speed,float restitution,float friction,int floating_entity,int is_collidable,int role_metadata){
	Entity box = create_entity(0,0,width,height,speed,restitution,friction,floating_entity,is_collidable,role_metadata);
	move_entity_to_fixed_pos(&box,pos_x,pos_y);
	return box;
}
Entity create_participant(int width, int height,int pos_x, int pos_y,float restitution,int role_metadata){
	return create_entity_with_size_and_position(width,height,pos_x,pos_y,0,restitution,0,0,1,role_metadata);
}
Entity create_object(int width, int height,int pos_x, int pos_y,float restitution,float friction,int role_metadata){
	return create_entity_with_size_and_position(width,height,pos_x,pos_y, 0,restitution,friction,1,0,role_metadata);
}

bool detect_entities_collision(Entity a, Entity b )
{

  if(a.max.x < b.min.x || a.min.x > b.max.x) return false;
  if(a.max.y < b.min.y || a.min.y > b.max.y) return false;

  return true;
}

bool detect_circled_entities_collision( struct Circle a, struct Circle b )
{
  float r = a.radius + b.radius;
  r *= r;
  return r < (((a.position.x + b.position.x)*(a.position.x + b.position.x)) + ((a.position.y + b.position.y)*(a.position.y + b.position.y)));
}


