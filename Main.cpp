/*
 BoostStomp - a STOMP (Simple Text Oriented Messaging Protocol) client
----------------------------------------------------
Copyright (c) 2012 Elias Karakoulakis <elias.karakoulakis@gmail.com>

SOFTWARE NOTICE AND LICENSE

BoostStomp is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

BoostStomp is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with BoostStomp .  If not, see <http://www.gnu.org/licenses/>.

for more information on the LGPL, see:
http://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License
*/

// 
// Main.cpp: a demo STOMP client
// (c) 2011 Elias Karakoulakis <elias.karakoulakis@gmail.com>
//

#include <string>
#include <sstream>
#include <iostream>
#include "unistd.h"

#include "BoostStomp.hpp"

using namespace STOMP;
using namespace std;

static BoostStomp*  stomp_client;
static string       notifications_topic = "/queue/zwave/monitor";

// -------------------------------------------------------
// a callback for any STOMP Frames in subscribed channels
// -------------------------------------------------------
STOMP::pfnOnStompMessage_t subscription_callback(STOMP::Frame* _frame) {
	cout << "--Incoming STOMP Frame--" << endl;
	cout << "  Headers:" << endl;
	STOMP::hdrmap::iterator it;
	for ( it = _frame->headers().begin() ; it != _frame->headers().end(); it++ )
	    cout << "\t" << (*it).first << "\t=>\t" << (*it).second << endl;
	//
	cout << "  Body: (size: " << _frame->body().size() << " chars):" << endl;
	cout << _frame->body() << endl;
	return 0;
}

// -----------------------------------------
int main(int argc, char *argv[]) {
// -----------------------------------------
    string  stomp_host = "localhost";
    int     stomp_port = 61613;

    try {
    	// initiate a new BoostStomp client
        stomp_client = new BoostStomp(stomp_host, stomp_port);

        // start the client, (by connecting to the STOMP server)
        stomp_client->start();

        // subscribe to a channel
        stomp_client->subscribe(notifications_topic, (STOMP::pfnOnStompMessage_t) &subscription_callback);

        // construct a headermap
        STOMP::hdrmap headers;
        headers["header1"] = string("value1");
        headers["header2:withcolon"] = string("value2");
        headers["header3"] = string("value3");
        string body = string("this is the FIRST message body");

        // add an outgoing message to the queue
        stomp_client->send(notifications_topic, headers, body);
        sleep(1);
        string body2 = string("this is the SECOND message");
        stomp_client->send(notifications_topic, headers, body2);
        sleep(1);
        string body3 = string("this is the THIRD message");
        stomp_client->send(notifications_topic, headers, body3);
        sleep(1);
    } 
    catch (std::exception& e)
    {
        cerr << "Error in BoostStomp: " << e.what() << "\n";
        return 1;
    } 
    
    return 0;
}
