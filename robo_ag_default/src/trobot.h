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


#ifndef trobot_H
#define trobot_H

#define HOSTNAME_MAXLENGTH 150
// SIM_NUM_LASERS should be the same as the simulator server!
#define SIM_NUM_LASERS 19 
#define ST_RUNNING 1
#define ST_STOPPED 0

//size of waypoints vector
#define SIM_NUM_WAYPOINTS 10


#include <pthread.h>

typedef struct {
	float odometer;
	float compass;
	float speed;
	float turn_angle; 
	float laser[SIM_NUM_LASERS];
	int x, y, z; // robot coordinates
	int num_waypoints;
	double elapsed_time; //elapsed simulation time (real or not, depending on user's options) in seconds
	struct {
		int x, y, z; //waypoint coordinates
	} waypoint[SIM_NUM_WAYPOINTS];
} tcli_robot_data;

class TRobotClient {
	public:
		//constructor adds a new robot on the server
		TRobotClient(char *robotname, void (*robot_function)(void*), char* hostname, int port);
		~TRobotClient(); //kill the thread and close socket
		int Start(); 
		int Stop();
		
		tcli_robot_data data; // robot control settings and status

		
		char robotname[150];
		void (*robot_function)(void*);	
		char srv_host[HOSTNAME_MAXLENGTH];
		int srv_port;
		int status;
		pthread_t robot_th; // robot thread
		int socketfd;       // Socket file descriptor
};

#endif