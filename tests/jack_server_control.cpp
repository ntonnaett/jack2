/*
 	Copyright (C) 2008 Grame
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <jack/jack.h>
#include <jack/control.h>

jackctl_server_t * server;

static void print_value(union jackctl_parameter_value value, jackctl_param_type_t type)
{
    switch (type) {
    
        case JackParamInt:
            printf("parameter value = %d\n", value.i);
            break;
            
         case JackParamUInt:
            printf("parameter value = %u\n", value.ui);
            break;
            
         case JackParamChar:
            printf("parameter value = %c\n", value.c);
            break;
        
         case JackParamString:
            printf("parameter value = %s\n", value.str);
            break;
            
         case JackParamBool:
            printf("parameter value = %d\n", value.b);
            break;
     }
}

static void print_parameters(const JSList * node_ptr)
{
    while (node_ptr != NULL) {
        jackctl_parameter_t * parameter = (jackctl_parameter_t *)node_ptr->data;
        printf("\nparameter name = %s\n", jackctl_parameter_get_name(parameter));
        printf("parameter id = %c\n", jackctl_parameter_get_id(parameter));
        printf("parameter short decs = %s\n", jackctl_parameter_get_short_description(parameter));
        printf("parameter long decs = %s\n", jackctl_parameter_get_long_description(parameter));
        print_value(jackctl_parameter_get_default_value(parameter), jackctl_parameter_get_type(parameter));
        node_ptr = jack_slist_next(node_ptr);
    }
}

static void print_driver(jackctl_driver_t * driver)
{
    printf("\n--------------------------\n");
    printf("driver = %s\n", jackctl_driver_get_name(driver));
    printf("-------------------------- \n");
    print_parameters(jackctl_driver_get_parameters(driver)); 
}

static void print_internal(jackctl_internal_t * internal)
{
    printf("\n-------------------------- \n");
    printf("internal = %s\n", jackctl_internal_get_name(internal));
    printf("-------------------------- \n");
    print_parameters(jackctl_internal_get_parameters(internal));
}

int main(int argc, char *argv[])
{
    const JSList * drivers;
    const JSList * internals;
    const JSList * node_ptr;
     
	server = jackctl_server_create();
    
    printf("\n========================== \n");
    printf("List of drivers \n");
    printf("========================== \n");
    
    drivers = jackctl_server_get_drivers_list(server);
    node_ptr = drivers;
    while (node_ptr != NULL) {
        print_driver((jackctl_driver_t *)node_ptr->data);
        node_ptr = jack_slist_next(node_ptr);
    }
    
    printf("\n========================== \n");
    printf("List of internal clients \n");
    printf("========================== \n");
    
    internals = jackctl_server_get_internals_list(server);
    node_ptr = internals;
    while (node_ptr != NULL) {
        print_internal((jackctl_internal_t *)node_ptr->data);
        node_ptr = jack_slist_next(node_ptr);
    }
    
    jackctl_server_destroy(server);
    return 0;
}