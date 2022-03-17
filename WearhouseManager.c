#include <stdio.h>
#include "WearhouseManager.h"

#define MIN -99999
#define MAX  99999
Package *create_package(long priority, const char* destination){
	// TODO 3.1

	Package* pack = (Package*)malloc(sizeof(Package));
	if (pack == NULL) {
		printf("error\n");
		return NULL;
	}

	pack->priority = priority;
	if (destination) {
		pack->destination = malloc(sizeof(char) * strlen(destination));
		if (pack->destination == NULL) {
			printf("error\n");
			return NULL;
		}
		strcpy(pack->destination, (char*)destination);
	}

	return pack;
}

void destroy_package(Package* package){
	// TODO: 3.1
	free(package);
	return;
}

Manifest* create_manifest_node(void){
	// TODO: 3.1

	Manifest* node = (Manifest*)malloc(sizeof(Manifest));
	if (node == NULL) {
		printf("error");
		return NULL;
	}

	node->package = NULL;
	node->next = NULL;
	node->prev = NULL;
	return node;

}

void destroy_manifest_node(Manifest* manifest_node){
	// TODO: 3.1.
	//prima data dau free la pachet
	free(manifest_node->package);
	free(manifest_node);
	return;

}

Wearhouse* create_wearhouse(long capacity){
	// TODO: 3.2
	if (capacity == 0)
		return NULL;
	Wearhouse* wear_house = (Wearhouse*)malloc(sizeof(Wearhouse));
	if (wear_house == NULL) {
		printf("error");
		return NULL;
	}
	wear_house->size = 0;
	wear_house->capacity = capacity;
	wear_house->packages = (Package**)malloc(wear_house->capacity *sizeof(Package*));

	return wear_house;
}

Wearhouse *open_wearhouse(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w){
	// TODO: 3.2
	if (w) {
		if (w->size == 0) {
			return 1;
		}
		return 0;
	}
	return 0;
}

long wearhouse_is_full(Wearhouse *w){
	// TODO: 3.2
	if (w) {
		if (w->size == w->capacity)
			return 1;
		return 0;
	}
	return 0;
}

long wearhouse_max_package_priority(Wearhouse *w){
	// TODO: 3.2
	long maxp = w->packages[0]->priority;
	for (int i = 1; i < w->capacity; i++) {
		if (w->packages[i]->priority > maxp) {
			maxp = w->packages[i]->priority;
		}
	}
	return maxp;
}

long wearhouse_min_package_priority(Wearhouse *w){
	// TODO: 3.2
	long minp = w->packages[0]->priority;
	for (int i = 1; i < w->capacity; i++) {
		if (w->packages[i]->priority < minp) {
			minp = w->packages[i]->priority;
		}
	}
	return minp;
}


void wearhouse_print_packages_info(Wearhouse *w){
	for(long i = 0; i < w->size; i++){
		printf("P: %ld %s\n",
				w->packages[i]->priority,
				w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse){
	// TODO: 3.2
	wearhouse_print_packages_info(wearhouse);
	for (int i = 0; i < wearhouse->size; i++) {
		free(wearhouse->packages[i]);
	}
	free(wearhouse);

}


Robot* create_robot(long capacity){
	// TODO: 3.2

	Robot* robot = (Robot*)malloc(sizeof(Robot));
	if (robot == NULL) 
		return NULL;

	robot->capacity = capacity;
	robot->size = 0;
	robot->next = NULL;
	robot->manifest = NULL;

	return robot;
}

int robot_is_full(Robot* robot){
	// TODO: 3.2
	if (robot) {
		if (robot->capacity == robot->size)
			return 1;
		return 0;
	}

	return 0;
}

int robot_is_empty(Robot* robot){
	// TODO: 3.2
	if (robot) {
		if (robot->size == 0)
			return 1;
		return 0;
	}

	return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority){
	// TODO: 3.2
	
	for (int i = 0; i < w->size; i++) {
		if (w->packages[i]->priority == priority) {
			return w->packages[i];
		}
	}
	return NULL;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package){
	// TODO: 3.2

	for (int i = 0; i < w->size; i++) {
		//caut pachetul in depozit
		if (w->packages[i] == package) {
			for (int j = i; j < w->size - 1; j++) {
				w->packages[j] = w->packages[j + 1];
			}
			break;
		}
	}
	w->size--;
	return;
}

void robot_load_one_package(Robot* robot, Package* package) {
	// TODO:  3.2
	if (robot_is_full(robot))
		return;

	Manifest* nod = robot->manifest;
	for (int i = 0; i < robot->size; i++) {
		//printf("-------------------------->%s ", nod->package->destination);
		nod = nod->next;
	}

	//creez un nou nod pentru robot
	Manifest* new_pack = create_manifest_node();
	new_pack->package = package;
	//printf("prio pach%ld\n", robot->manifest->package->priority);
	if (robot_is_empty(robot)) {
		//punem in robot pachetul 
		robot->manifest = new_pack;
		robot->next = NULL;
		robot->size++;
		return;
	}

	//iteram prin inventare cu first
	Manifest* first = robot->manifest;
	//printf("prio pach%ld\n", first->package->priority);
	//insert la inceputul listei;

	//daca primul pachet are ac prioriate cu primul=> compar destinatiile

	for (int i = 0; first != NULL; i++) {
		if (first->package->priority < new_pack->package->priority) {
			//insert inainte
			if (first->prev == NULL) {
				new_pack->next = first;
				first->prev = new_pack;
				robot->manifest = new_pack;
				robot->size++;
				return;
			}
			new_pack->next = first;
			new_pack->prev = first->prev;
			first->prev->next = new_pack;
			first->prev = new_pack;
			robot->size++;
			return;
		}

		if (first->next == NULL) {
			if (first->package->priority > new_pack->package->priority) {
				//adaug la sfarsit de tot
				first->next = new_pack;
				new_pack->prev = first;
				new_pack->next = NULL;
				robot->size++;
				return;
			}
		}

		while (first->package->priority == new_pack->package->priority) {
			if (strcmp(first->package->destination, new_pack->package->destination) > 0) {
				//il pun in fata lui
				if (first->prev == NULL) {
					new_pack->next = first;
					first->prev = new_pack;
					robot->manifest = new_pack;
					robot->size++;
					return;
				}
				new_pack->next = first;
				new_pack->prev = first->prev;
				first->prev->next = new_pack;
				first->prev = new_pack;
				robot->size++;
				return;
			}

			//altfel daca e mai 
			if (first->next == NULL) {
				//pun la sfarsit de tot
				first->next = new_pack;
				new_pack->prev = first;
				new_pack->next = NULL;
				robot->size++;
				return;
			}
			//altfel mai caut
			first = first->next;

		}

		first = first->next;
	}

}


long robot_load_packages(Wearhouse* wearhouse, Robot* robot){
	// TODO: 3.2
	
	long max_packages = 0;
	long pmax;

	while (robot->size != robot->capacity) {
		if (wearhouse->size == 0)
			break;
		//caut prioritatea maxima a pachetelor din depozit
		pmax = wearhouse_max_package_priority(wearhouse);
		//pachetul cu prioriatea maxima din depzit
		Package* pack = robot_get_wearhouse_priority_package(wearhouse, pmax);
		//incarc pachetul in robot
		robot_load_one_package(robot, pack);
		//elimin pachetul cu prioriatea maxima din depozit
		robot_remove_wearhouse_package(wearhouse, pack);
		max_packages++;
		//robot->size++;
	}
	return max_packages;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination){
	
	if (robot_is_empty(robot))
		return NULL;
	Package* pack = create_package(0, destination);
	Manifest* nod = robot->manifest;
	int gasit = 0;
	while (nod) {
		//caut prioritatea cea mai mare
		if (strcmp(nod->package->destination, destination) == 0) {
			if (nod->package->priority > pack->priority) {
				pack = nod->package;
				gasit = 1;
			}
		}
		nod = nod->next;
	}
	if (gasit == 0)
		return NULL;
	return pack;
}

void destroy_robot(Robot* robot){
	// TODO: 3.2

	Manifest* inventar = robot->manifest;
	while (inventar) {

		robot->manifest = robot->manifest->next;
		//free manifest
		free(inventar->package);
		free(inventar);

		if (inventar->next == NULL)
			break;
		inventar = inventar->next;
		
	}
	robot->manifest = NULL;
	robot->capacity = 0;
	robot->size = 0;
	free(robot);
}
void remove_pack(Robot* robot, Package* p) {

	if (robot->size == 0)
		return;

	if (robot->manifest->package == p) {
		robot->manifest->next->prev = NULL;
		robot->manifest = robot->manifest->next;
		robot->size--;
		return;
	}

	Manifest* head = robot->manifest;
	
	while (head) {
		if (head->package == p) {



			/*if (head == robot->manifest) {
				if (head->next == NULL) {
					robot->manifest = NULL;
					//robot->size--;
					return;
				}


				robot->manifest = robot->manifest->next;
				//robot->size--;
				return;
			}*/


			if (head->next == NULL) {
				head->prev->next = NULL;
				robot->size--;
				return;
			}
			head->next->prev = head->prev;
			head->prev->next = head->next;
			robot->size--;
			return;

		}
		head = head->next;
	}
	return;
}

void add_in_truck(Truck* truck, Package* p) {

	if (truck_is_full(truck))
		//return truck->manifest;
		return;

	Manifest* head = create_manifest_node();
	head->package = p;
	

	if (truck_is_empty(truck)) {
		//pun in primul din truck headul_robot;
		truck->manifest = head;
		truck->size++;
		return;
	}

	Manifest* lista = truck->manifest;
	for (int i = 0; i < truck->size; i++) {
		if (lista->next == NULL)
			break;
		lista = lista->next;
	}
	lista->next = head;
	head->next = NULL;
	truck->size++;
	return;
	/*
	//as pune in fata
	head->next = truck->manifest;
	truck->manifest->prev = head;
	truck->manifest = head;
	truck->size++;
	return;*/
}

void robot_unload_packages(Truck* truck, Robot* robot){
	// TODO: 3.3

	if (robot_is_empty(robot))
		return;
	printf("%s-", truck->destination);
//	robot_print_manifest_info(robot);
	
	Package* cautpachet = robot_get_destination_highest_priority_package(robot, truck->destination);

	
	if (cautpachet == NULL)
		return;
	add_in_truck(truck, cautpachet);
	remove_pack(robot, cautpachet);
	
	//robot->size--;
	while (!truck_is_full(truck)) {
		cautpachet = robot_get_destination_highest_priority_package(robot, truck->destination);
		if (cautpachet == NULL)
			return;

		add_in_truck(truck, cautpachet);
		remove_pack(robot, cautpachet);
		//robot->size--;
	}

	truck_print_info(truck);
	printf("---gata\n");
	//robot_print_manifest_info(robot);

	return;
}

// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot *parkinglot){
	int found_truck = 0;
	long size = 0;
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while(m_iterator != NULL){
		while(arrived_iterator != parkinglot->arrived_trucks){
			size  = truck_destination_robots_unloading_size(arrived_iterator);
			if(strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
					size < (arrived_iterator->capacity-arrived_iterator->size)){
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if(found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if(found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while(r_iterator != NULL){
		Package *pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if(m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if(prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot){
	Manifest *iterator = robot->manifest;
	while(iterator != NULL){
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}



Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time){
	// TODO: 3.3

	Truck* truck = (Truck*)malloc(sizeof(Truck));
	if (truck == NULL){
		printf("error");
		return NULL;
	}
	
	if (destination) {
		truck->destination = (char*)malloc(strlen(destination) * sizeof(char));
		strcpy(truck->destination, destination);
	}
	else
		truck->destination = NULL;
	truck->manifest = NULL;
	truck->unloading_robots = NULL;
	truck->capacity = capacity;
	truck->size = 0;
	truck->in_transit_time = 0;
	truck->transit_end_time = transit_time;
	truck->departure_time = departure_time;
	truck->next = NULL;
	return truck;
}

int truck_is_full(Truck *truck){
	// TODO: 3.3
	if (truck == NULL)
		return 0;
	if (truck->size == truck->capacity)
		return 1;
	return 0;
}

int truck_is_empty(Truck *truck){
	// TODO: 3.3
	if (truck == NULL)
		return 0;
	if (truck->size == 0)
		return 1;
	return 0;
}

long truck_destination_robots_unloading_size(Truck* truck) {
	// TODO: 3.3

	if (truck == NULL)
		return 0;
	if (truck->unloading_robots == NULL)
		return 0;
	//verific dc am ceva de descarcat
	if (truck->unloading_robots->manifest == NULL)
		return 0;

	Robot* robot = truck->unloading_robots;
	
	long suma = 0;
	while (robot) {

		while (strcmp(robot->manifest->package->destination, truck->destination) == 0) {
			suma += robot->size;
			if (robot->manifest->next == NULL)
				break;
			else
				robot->manifest = robot->manifest->next;
		}
		if (robot->next == NULL)
			break;
		else
			robot = robot->next;
	}
	return suma;
}

void truck_print_info(Truck* truck){
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
			truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while(m_iterator != NULL){
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while(r_iterator != NULL){
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}


void destroy_truck(Truck* truck){
	// TODO: 3.3

	if (truck == NULL)
		return;
	
	while (truck->manifest) {
		destroy_manifest_node(truck->manifest);
		if (truck->manifest->next == NULL)
			break;
		truck->manifest = truck->manifest->next;
	}
	
	while (truck->unloading_robots) {
		destroy_robot(truck->unloading_robots);
		if (truck->unloading_robots->next == NULL)
			break;
		truck->unloading_robots = truck->unloading_robots->next;
	}
	
	truck->size = 0;
	truck->departure_time = 0;
	truck->capacity = 0;
	truck->in_transit_time = 0;
	truck->transit_end_time = 0;
	truck->destination = NULL;
	truck->next = NULL;
	free(truck);
	return;
}


Parkinglot* create_parkinglot(void){
	// TODO: 3.4
	// Allocate parking lot
	Parkinglot* parkinglot = (Parkinglot*)malloc(sizeof(Parkinglot));
	if (parkinglot == NULL) 
		return NULL;

	parkinglot->arrived_trucks = create_truck(NULL, 0, 0, 0);
	if (parkinglot->arrived_trucks == NULL) 
		return NULL;

	parkinglot->departed_trucks = create_truck(NULL, 0, 0, 0);
	if (parkinglot->departed_trucks == NULL) 
		return NULL;

	parkinglot->pending_robots = create_robot(0);
	if (parkinglot->pending_robots == NULL) 
		return NULL;

	parkinglot->standby_robots = create_robot(0);
	if (parkinglot->standby_robots == NULL) 
		return NULL;

	//toate sunt liste circulare cu santinele 
	parkinglot->arrived_trucks->next = parkinglot->arrived_trucks;
	parkinglot->departed_trucks->next = parkinglot->departed_trucks;
	parkinglot->pending_robots->next = parkinglot->pending_robots;
	parkinglot->standby_robots->next = parkinglot->standby_robots;

	return parkinglot;
}

Parkinglot* open_parckinglot(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if(token[0] == 'T'){
			token = strtok(NULL, ",\n ");
			char *destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck *truck = create_truck(destination, capacity, transit_time, departure_time);

			if(arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}else if(token[0] == 'R'){
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot *robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot){
	// TODO: 3.4


	printf("ceva\n");
	if (robot->size) {
		Robot* santinela = parkinglot->pending_robots;
		Robot* x = parkinglot->pending_robots->next;
		Robot* y = parkinglot->pending_robots;
		//am de adaugat primul nod
	
		if (santinela == x) {
			santinela->next = robot;
			robot->next = santinela;
			return;
		}

		for (x = santinela->next; x != santinela; x = x->next) {
			
			if (robot->size > x->size) {
				y->next = robot;
				robot->next = x;
				return;
			}
		
			x = x->next;
			y = x->next;
		}
	}

	if (robot->capacity) {
		Robot* santinela = parkinglot->standby_robots;
		Robot* x = parkinglot->standby_robots->next;
		Robot* y = parkinglot->standby_robots;
		if (santinela == x) {
			santinela->next = robot;
			robot->next = santinela;
			return;
		}

		for (x = santinela->next; x != santinela; x = x->next) {

			if (robot->capacity > x->capacity) {
				y->next = robot;
				robot->next = x;
				return;
			}
		
			x = x->next;
			y = x->next;
		}
	}

	return;

}

void parkinglot_remove_robot(Parkinglot* parkinglot, Robot* robot) {
	// TODO: 3.4

	if (robot->size == 0) {
		Robot* santinelas = parkinglot->standby_robots;
		Robot* nod = santinelas->next;
		Robot* anterior = santinelas->next;
		while( nod != santinelas) {
			if (robot == nod && nod == santinelas->next) {
				//e primul 
				santinelas->next = nod->next;
				return;
			}
			nod = nod->next;
			if (nod == robot) 
				anterior->next = nod->next;

			return;
			anterior = anterior->next;
		}
		return;
	}
	else {
		Robot* santinelap = parkinglot->pending_robots;
		Robot* anteriorx = santinelap;
		Robot* x = santinelap->next;
		while(x != santinelap) {
			if (robot == x && x == santinelap->next) {
				//e primul 
				santinelap->next = x->next;
				return;
			}
			x = x->next;
			if (x == robot) {
				anteriorx->next = x->next;
				return;
			}
			anteriorx = anteriorx->next;
		}
		return;
	}
	return;
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot){
	// TODO: 3.4
	
	if (parkinglot->pending_robots != parkinglot->pending_robots->next) {
		return 1;
	}
	return 0;
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot){
	// TODO: 3.4
	Truck* nod = parkinglot->arrived_trucks->next;
	while (nod != parkinglot->arrived_trucks) {

		if (nod->size)
			return 0;
		nod = nod->next;
	}
	return 1;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot){
	// TODO: 3.4
	if (parkinglot->departed_trucks != parkinglot->departed_trucks->next) {
		return 1;
	}

	return 0;
}


void destroy_parkinglot(Parkinglot* parkinglot){
	// TODO: 3.4

	if (parkinglot == NULL)
		return;

	//2 noduri pentru fiecare dezalocare 
	//unul pnetru ce dezaloc acum si unul pentru iterat in continuare
	Truck* at = parkinglot->arrived_trucks->next;
	Truck* anterior_at = at;
	while(at != parkinglot->arrived_trucks) {
		anterior_at = anterior_at->next;
		destroy_truck(at);
		at = anterior_at;
	}

	Truck* dt = parkinglot->departed_trucks->next;
	Truck* anterior_dt = dt;
	while (dt != parkinglot->departed_trucks) {
		anterior_dt = anterior_dt->next;
		destroy_truck(dt);
		dt = anterior_dt;
	}

	Robot* rp = parkinglot->pending_robots->next;
	Robot* anterior_rp = parkinglot->pending_robots->next;
	while (rp != parkinglot->pending_robots) {
		anterior_rp = anterior_rp->next;
		destroy_robot(rp);
		rp = anterior_rp;
	}

	Robot* rs = parkinglot->standby_robots->next;
	Robot* anterior_rs = parkinglot->standby_robots->next;
	while (rs != parkinglot->standby_robots) {
		anterior_rs = anterior_rs->next;
		destroy_robot(rs);
		rs = anterior_rs;
	}

	free(parkinglot);
	return;
}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->arrived_trucks->next;
	while(iterator != parkinglot->arrived_trucks){

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->departed_trucks->next;
	while(iterator != parkinglot->departed_trucks){
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot) {
	Robot* iterator = parkinglot->standby_robots->next;
	while (iterator != parkinglot->standby_robots) {
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}


void truck_departed(Parkinglot* parkinglot, Truck* truck) {
	// TODO: 3.5
	// Search through arrived list, if exists node is found remove it
	// Note: this must remove the node from the list, NOT deallocate it

	Truck* enod = parkinglot->arrived_trucks->next;
	Truck* anterior_enod = parkinglot->arrived_trucks;

	while (enod != parkinglot->arrived_trucks) {
		if (truck == enod) {
			anterior_enod->next = enod->next;
			
			break;
		}
		anterior_enod = enod;
		enod = enod->next;
	}

	Truck* santinela = parkinglot->departed_trucks;
	Truck* nod = santinela->next;
	Truck* anterior = santinela;

	if (santinela->next == santinela) {
		santinela->next = truck;
		truck->next = santinela;
		return;
	}
	else {
		while (nod != santinela) {
			//timpul de plecare al tirului pe care vreau sa l introduc
			//este mai mic decat oricare nod la care sunt - bag in fata 

			if (truck->departure_time <= nod->departure_time) {
				//intruduc inainte
				truck->next = nod;
				anterior->next = truck;
				return;
			}

			if (nod->next == santinela) {
				if (truck->departure_time > nod->departure_time) {
					//adaug la sfarsit de tot
					nod->next = truck;
					truck->next = santinela;
					return;
				}
			}
			anterior = nod;
			nod = nod->next;
		}
	}
}


void truck_arrived(Parkinglot* parkinglot, Truck* truck) {
	if (parkinglot == NULL || truck == NULL) return;

	// TODO: 3.5
	// Search through departed list, if exists node is found remove it
	// Note: this must remove the node not deallocate it



	truck->size = 0;
	truck->manifest = NULL;

	Truck* elimin_nod = parkinglot->departed_trucks->next;
	//la inceput anteriorul este santinela;
	Truck* ant_elimin_nod = parkinglot->departed_trucks;
	//caut prin lista pana la sfarsit

	while (elimin_nod != parkinglot->departed_trucks) {

		if (elimin_nod == truck) {
			if (elimin_nod->next == parkinglot->departed_trucks) {
				parkinglot->departed_trucks->next = parkinglot->departed_trucks;
				//parkinglot->departed_trucks->size = 0;
				//parkinglot->departed_trucks->manifest = NULL;
				truck->size = 0;
				break;
			}
			ant_elimin_nod->next = elimin_nod->next;
			
			break;
		}
		ant_elimin_nod = elimin_nod;
		elimin_nod = elimin_nod->next;
	}


	//dupa ce am eliminat, adaug in addirved truckul
	Truck* santinela = parkinglot->arrived_trucks;
	Truck* nod = parkinglot->arrived_trucks->next;
	Truck* prev_nod = parkinglot->arrived_trucks;
	
	if (santinela == santinela->next) {
		//adaug la inceput oricum
		printf("cand ajung aici/////");
		parkinglot->arrived_trucks->next = truck;
		truck->next = parkinglot->arrived_trucks;
		return;
	}
	printf("mai ajung aiaci\n");
	while (nod != santinela) {
		printf("(((((((((");
		while (strcmp(truck->destination, nod->destination) == 0) { 
			if (truck->departure_time < nod->departure_time) {
				//pun inainte
				truck->next = nod;
				prev_nod->next = truck;
				//parkinglot->arrived_trucks->size++;
				printf("1\n");
				return;
			}
			if (nod->next == santinela) {
				if (truck->departure_time > nod->departure_time) {
					//pun la sfarsit de tot
					nod->next = truck;
					truck->next = santinela;
					//parkinglot->arrived_trucks->size++;
					printf("-------\n");
					return;
				}
			}
		
			prev_nod = nod;
			nod = nod->next;
			
		}
		//veific unde il pun
		if (strcmp(truck->destination, nod->destination) < 0) {
			//adaug in fata nodului 
			//daca e detinatia mai mica decat cea care urmeaza
			truck->next = nod;
			prev_nod->next = truck;
			printf("2\n");
			//parkinglot->arrived_trucks->size++;
			

			return;
		}
		if (nod->next == santinela) {
			//nseamna ca nu am gasit sa se poata pun in fata si e mai
			//mic decat oricare alta valoare, deci o voi pune 
			//la final de tot
			nod->next = truck;
			truck->next = santinela;
			
			//parkinglot->arrived_trucks->size++;
			printf("3\n");
			return;
		}


		prev_nod = nod;
		nod = nod->next;
	}
	printf(")))");
}

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck){
	// TODO:  3.5

	//am doua tiruri cu unul parcurg pe unul il scot
	Robot* robot_parcurgere = truck->unloading_robots;
	

	//cat timp am tiruri
	Robot* robot_de_scos;

	while (robot_parcurgere) {

		robot_de_scos = robot_parcurgere;
		robot_parcurgere = robot_parcurgere->next;
		parkinglot_add_robot(parkinglot, robot_de_scos);

	}

	truck->unloading_robots = NULL;

}

// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour){
	// TODO: 3.5

	Truck* caut_tir = parkinglot->arrived_trucks->next;
	Truck* ajut_la_transfer;

	while (caut_tir != parkinglot->arrived_trucks) {
		ajut_la_transfer = caut_tir;
		caut_tir = caut_tir->next;
		if (ajut_la_transfer->departure_time == day_hour) {
			//trbuie sa plece
			truck_departed(parkinglot, ajut_la_transfer);
		}
	}
}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot){
	// TODO: 3.5
	//caut in tiruri sa vad ce fac cu ele
	Truck* santinela = parkinglot->departed_trucks;
	Truck* tir = parkinglot->departed_trucks->next;
	
	while (tir != santinela) {
		tir->in_transit_time++;
		
		if (tir->in_transit_time == tir->transit_end_time) {
			//tirul a ajuns
			tir->in_transit_time = 0;
			truck_arrived(parkinglot, tir);
			return;
		}

		tir = tir->next;
	}

}

void robot_swarm_collect(Wearhouse *wearhouse, Parkinglot *parkinglot){
	Robot *head_robot = parkinglot->standby_robots;
	Robot *current_robot = parkinglot->standby_robots->next;
	while(current_robot != parkinglot->standby_robots){

		// Load packages from wearhouse if possible
		if(!robot_load_packages(wearhouse, current_robot)){
			break;
		}

		// Remove robot from standby list
		Robot *aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot *parkinglot){

	Robot *current_robot = parkinglot->pending_robots->next;

	while(current_robot != parkinglot->pending_robots){
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if(!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot){
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	while(arrived_iterator != parkinglot->arrived_trucks){
		Robot *current_robot = arrived_iterator->unloading_robots;
		while(current_robot != NULL){
			robot_unload_packages(arrived_iterator, current_robot);
			Robot *aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}

