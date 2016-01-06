//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "network/client.h"
#include "base/io.h"
#include <enetpp/client_connect_params.h>

Client::Client(const std::string &ipAddress, const U16 port) {
	mServerAddress = ipAddress;
	mPort = port;
}

Client::~Client() {

}

void Client::connect() {
	enetpp::client_connect_params params;
	params.set_channel_count(1);
	params.set_server_host_name_and_port(mServerAddress.c_str(), mPort);

	mClient.connect(params);
}

void Client::disconnect() {
	mClient.disconnect();
}

void Client::pollEvents(bool hasPressedSpaceBar) {
	auto onConnect = []() {
		IO::printf("You have connected to the server!\n");
	};

	auto onDisconnect = []() {
		IO::printf("You have disconnected from the server!\n");
	};

	auto onReceiveData = [&](const U8 *data, size_t size) {
		IO::printf("%s", data);

		// send a message if you press the space bar.
		if (hasPressedSpaceBar) {
			std::string message = "I have pressed da key yo!\n";
			mClient.send_packet(0, reinterpret_cast<const U8*>(message.c_str()), message.length(), ENET_PACKET_FLAG_RELIABLE);
		}
	};

	mClient.consume_events(onConnect, onDisconnect, onReceiveData);
}