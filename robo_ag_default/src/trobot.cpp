/***************************************************************************
 *   Copyright (C) 2009 by Felipe Kermentz Ferraz Costa                    *
 *   felipekfcosta@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "trobot.h"

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define fatal() \
       { printf("error\n"); }
#define BUFFER_SIZE 2048

typedef struct { // we store de data sent from server, and only send back what's been updated by the client
		float speed;
		float turn_angle;
		int x;
		int y;
		int z;
	} tprevious_data;


bool IsPackageValid (char *package, int nbytes) {
	if (nbytes > 3 && 
		package[0] == 'l' &&
		package[1] == 'p' &&
		package[2] == '1') return true;
	else return false;
}

// returns: false if package is invalid
// writes the new coordinates into the robot data structure, adds 1 to waypoint counter
bool GetWayPoint(char *package, int nbytes, TRobotClient *Robot) {
	if (!IsPackageValid(package, nbytes)) return false;
	if (nbytes < 3 + 3*sizeof(int) || package[3] != 'W') return false;
	int i = 4;
	memcpy(&Robot->data.waypoint[Robot->data.num_waypoints].x, &package[i], sizeof(int));
	i = i + sizeof(int);
	memcpy(&Robot->data.waypoint[Robot->data.num_waypoints].y, &package[i], sizeof(int));
	i = i + sizeof(int);
	memcpy(&Robot->data.waypoint[Robot->data.num_waypoints].z, &package[i], sizeof(int));
	Robot->data.num_waypoints++;
	return true;

}

// returns: 0 if package is invalid
//          1 : start signal
//          2 : stop signal
int GetStartStop(char *package, int nbytes) {
	if (!IsPackageValid(package, nbytes)) return 0;
	if (nbytes < 5) return 0;
	if (package[3] != 'S') return 0;
	if (package[4] == 'I') return 1;
	if (package[4] == 'O') return 2;
	return 0;
}

bool GetUpdate(char *package, int nbytes, TRobotClient *Robot) {
	if (!IsPackageValid(package, nbytes)) return false;
	if (nbytes < 4 || package[3] != 'U') return false;
	
	int i = 4;
	memcpy(&Robot->data.odometer, &package[i], sizeof(float));
	i = i + sizeof(float);
	memcpy(&Robot->data.compass, &package[i], sizeof(float));
	i = i + sizeof(float);
	memcpy(&Robot->data.speed, &package[i], sizeof(float));
	i = i + sizeof(float);
	memcpy(&Robot->data.turn_angle, &package[i], sizeof(float));
	i = i + sizeof(float);
	for (int j = 0; j < SIM_NUM_LASERS; j++) {
		memcpy(&Robot->data.laser[j], &package[i], sizeof(float));	
		i = i + sizeof(float);
	}	
	memcpy(&Robot->data.x, &package[i], sizeof(int));
	i = i + sizeof(int);
	memcpy(&Robot->data.y, &package[i], sizeof(int));
	i = i + sizeof(int);
	memcpy(&Robot->data.z, &package[i], sizeof(int));
	i = i + sizeof(int);
	
	memcpy(&Robot->data.elapsed_time, &package[i], sizeof(double));
	i = i + sizeof(double);
	return true;
}

void CreateUpdate(char *package, int *nbytes, tprevious_data* previous_data, TRobotClient *Robot) {
	package[0] = 'l';
	package[1] = 'p';
	package[2] = '1';
	package[3] = 'U';
	
	*nbytes = 6;
	
	if (previous_data->speed != Robot->data.speed && previous_data->turn_angle != Robot->data.turn_angle) {
		package[4] = 'S';
		package[5] = 'A';
		memcpy(&package[6], &Robot->data.speed, sizeof(float));
		*nbytes += sizeof(float);
		memcpy(&package[*nbytes], &Robot->data.turn_angle, sizeof(float));
		*nbytes += sizeof(float);
	} else if (previous_data->speed != Robot->data.speed) {
		package[4] = 'S';
		package[5] = '0';
		memcpy(&package[6], &Robot->data.speed, sizeof(float));
		*nbytes += sizeof(float);
	} else if (previous_data->turn_angle != Robot->data.turn_angle) {
		package[4] = '0';
		package[5] = 'A';
		memcpy(&package[6], &Robot->data.turn_angle, sizeof(float));
		*nbytes += sizeof(float);
	} else if (previous_data->x != Robot->data.x || previous_data->y != Robot->data.y || previous_data->z != Robot->data.z) {
	       package[4] = 'T';
	       package[5] = '0';
	       memcpy(&package[(*nbytes)], &Robot->data.x, sizeof(int));	
	       *nbytes += sizeof(int);
	       memcpy(&package[(*nbytes)], &Robot->data.y, sizeof(int));	
	       *nbytes += sizeof(int);
	       memcpy(&package[(*nbytes)], &Robot->data.z, sizeof(int));	
	       *nbytes += sizeof(int);
	} else {
		package[4] = '0';
		package[5] = '0';
	
	}
	

}
	
	
void *ThreadFunc(void *arg) { 
	TRobotClient *Robot = (TRobotClient*) arg;
	
	struct sockaddr_in name;     /* The socket address.    */
	struct hostent *hostinfo;    /* The server information */
	fd_set active_fd_set;  /* Array of file descriptors */
	
	
	/* Create the socket. 
	socket (NAMESPACE, STYLE, PROTOCOL)
	- namespace: local, other, internet: PF_INET || PF_INET6 
	- style: SOCK_STREAM, SOCK_DGRAM
	- protocol: 0 means 'auto' */

	Robot->socketfd = socket (PF_INET, SOCK_STREAM, 0); 
	
	if (Robot->socketfd < 0)
		fatal ();

	/* Give the socket a name. */
	name.sin_family = AF_INET;                   /* Ineternet family */
	name.sin_port = htons (Robot->srv_port);      /* Well-known port  */

	hostinfo = gethostbyname (Robot->srv_host);
    

	name.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	
	
         if (connect(Robot->socketfd,(struct sockaddr *) &name,sizeof(name)) < 0) fatal();

		
		
	int nbytes;
	char *package = (char*) malloc(BUFFER_SIZE); 
	
	package[0] = 'l';
	package[1] = 'p';
	package[2] = '1';
	package[3] = 'N';
	nbytes = strlen(Robot->robotname) + 1;
	memcpy(&package[4], &nbytes, sizeof(int));
	memcpy(&package[4 + sizeof(int)], &Robot->robotname, strlen(Robot->robotname) );
	
	//debug
	//printf("sent new robot package (len %ld): %s\nrobot name is: %s\n", strlen(Robot->robotname) + 4 + sizeof(int) + 2, package, Robot->robotname);
	nbytes = write (Robot->socketfd, package, strlen(Robot->robotname) + 4 + sizeof(int) + 2);
	
	
	/*
	fd_set active_fdset;  // Array of file descriptors 
	FD_ZERO (&active_fdset);
	FD_SET (Robot->socketfd, &active_fdset);
	*/
	
	tprevious_data previous_data;
	
	
      //23 apr 2010 - Disabling Nagle algorithm	in the protocol stack, so it sends packages
      // immeadiately, without waiting for more bytes to be available to send, which is more
      // appropriate for this real-time program,
     char flag = 1;
     setsockopt(Robot->socketfd, IPPROTO_TCP, 1, (char *) &flag, sizeof(int)); 
     
	
	for (;;) { // listen to the port for a update, then run the robot algorithm
			nbytes = read (Robot->socketfd, package, BUFFER_SIZE);  //new message from server
			//debug
			//printf("%s - robot status %d\n", package, Robot->status);
			
			if (GetUpdate(package, nbytes, Robot)) {
					//store previous robot input data
					previous_data.speed = Robot->data.speed;
					previous_data.turn_angle = Robot->data.turn_angle;
					previous_data.x = Robot->data.x;
					previous_data.y = Robot->data.y;
					previous_data.z = Robot->data.z;
				if (Robot->status == ST_RUNNING) {
					//printf("running robot alg\n"); //debug
					//execute users algorithm
					Robot->robot_function((void*) Robot);
					//create and send update package
					CreateUpdate(package, &nbytes, &previous_data, Robot);
					write (Robot->socketfd, package, nbytes+1);
				} else {
				    write (Robot->socketfd, "lp1\n", 4); //confirmation
				}
			} else if (GetStartStop(package, nbytes)) {
				printf("got start/stop signal\n"); //debug
				int status = GetStartStop(package, nbytes);
				if (status == 1) {
				    Robot->Start();
				    write (Robot->socketfd, "lp1\n", 4); //confirmation
				}
				if (status == 2) Robot->Stop();
			}
			if (GetWayPoint(package, nbytes, Robot)) {
			    write (Robot->socketfd, "lp1\n", 4); //confirmation
			    printf("got new waypoint coordinates\n"); //debug
			}
			
			
		}
}


TRobotClient::TRobotClient(char *robotname, void (*robot_function)(void*), char* hostname, int port) {
	//start a new thread handling the tcp connection, which will later be used to run the robot algorithm
	
	this->data.num_waypoints = 0;
	
	strcpy(srv_host, hostname);
	strcpy(this->robotname, robotname);
	srv_port = port;
	this->robot_function = robot_function;
	status = ST_STOPPED;
	
	data.x = -1; data.y = -1; data.z = -1;
	
	data.elapsed_time = 0;
	
	//**********************************
	// Starting the robot client thread
	int execute_thread;
	execute_thread = pthread_create(&robot_th,NULL,ThreadFunc,(void *) this);

}

int TRobotClient::Start() {
	status = ST_RUNNING;
	return 0;
}

int TRobotClient::Stop() {
	status = ST_STOPPED;
	return 0;
}

TRobotClient::~TRobotClient() {
	close (socketfd);
	pthread_exit(NULL); //close the thread
}