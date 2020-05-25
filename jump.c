#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "motorino_basics.h"
#include "motorino_collisions.h"
#include "motorino_world.h"

enum EntityType {PLAYER,FLOOR,BLOCK,ENEMY};
typedef enum EntityType EntityType;

void draw_Box(Entity *Box) {
	al_draw_filled_rectangle(Box->min.x, Box->min.y, Box->max.x, Box->max.y,
			al_map_rgba_f(0, 0, 0.5, 0.5));
}
void draw_info(ALLEGRO_FONT *font, Entity entity) {
	int pos = 400;
	int spacing = 10;

	pos += spacing;
	al_draw_textf(font, al_map_rgb(255, 20, 255), 0, pos, 0,
			"Velocity on y :%f Velocity on x:%f ", entity.vy, entity.vx);
	pos += spacing;
	al_draw_textf(font, al_map_rgb(255, 20, 255), 0, pos, 0, "Is Jumping: %d ",
			entity.is_jumping);
}


void process_collision(World *world, CollisionInfo *info) {
	if (info != NULL && info->affected_entity->metadata_role == PLAYER
			&& info->other_entity->metadata_role == BLOCK) {
	  remove_entity(world,info->other_entity);

	}
}

int main() {
	al_init();
	al_install_keyboard();
	al_init_primitives_addon();
	int height = 800;
	int width = 600;

	if (!al_init_image_addon()) {
		printf("couldn't initialize image addon\n");
		return 1;
	}

	ALLEGRO_BITMAP *fly1 = al_load_bitmap("imgs/fly1.png");

	float elapsed_time = 1.0 / 30.0;
	ALLEGRO_TIMER *timer = al_create_timer(elapsed_time); //30 fps
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	ALLEGRO_DISPLAY *disp = al_create_display(height, width);
	ALLEGRO_FONT *font = al_create_builtin_font();

	//registramos los productores de eventos
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bool done = false;
	ALLEGRO_EVENT event;

	World world = { };
	init_world(&world, elapsed_time, 4, 0.7);

	//las cajitas
	struct EntityStructure floor = create_object(1600, 100, 400, 550, 0.2, 0.7,
			FLOOR);
	struct EntityStructure wall = create_object(50, 100, 500, 450, 0.1, 0.1, BLOCK);
	//struct EntityStructure walle = create_object(90,40,200,490,0.2,0.7);
	struct EntityStructure player = create_participant(50, 50, 300, 300, 0, PLAYER);



	add_entity_to_world(&world, &floor);
	add_entity_to_world(&world, &wall);
	//  add_entity_to_world(&world,&walle);
	add_entity_to_world(&world, &player);

	Point p_basic = { 0, 0 };

	al_start_timer(timer);
	while (!done) {
		//se queda esperando a que ocurra algo, un evento del teclado, del
		//timer. etc
		al_wait_for_event(queue, &event);
		calculate_state(&world);
		calculate_collisions(&world);
		process_collisions(&world, process_collision);
		//si colisiono

		switch (event.type) {
		case ALLEGRO_EVENT_TIMER:
			// once again, no game logic. fishy? maybe.
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		case ALLEGRO_EVENT_KEY_CHAR:
			if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
				if (player.is_jumping == 0) {
					player.ay = -50;
					player.is_jumping = 1;
				}
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				//No hacemos nada por el momento
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				//if (player.is_jumping == 0) {
				player.vx = -10;
				//}
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				//if (player.is_jumping == 0) {
				player.vx = 11;
				//}
			}
			if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){
				Entity other = create_object(6000, 100, 200, 200, 0.1, 0.1, BLOCK);
				Entity *pother = malloc(sizeof(struct EntityStructure));
				memcpy(pother,&other,sizeof(Entity)); //should keep track of this an free at the end...
				add_entity_to_world(&world, pother);
			}
		}

		Point p_center = calculate_center(player);
		int diff_x = (height / 2) - p_center.x;
		int diff_y = (width * 0.8) - p_center.y;
		move_world(&world, diff_x, diff_y);

		//A DIBUJAR!
		//limpiamos en cada frame
		al_clear_to_color(al_map_rgb(0, 0, 0));
		draw_info(font, player);

		//recorrer la lista del mundo y pintar

		EntityNode *head = world.firstEntity;
		while(head != NULL){
			draw_Box(head->entity);
			head = head->next;
		}


		al_flip_display();

	}
	//destructores!! aggh!
	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	destroy_world(&world);
	return 0;
}

