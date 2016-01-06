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
	IO::printf("Connecting to IP address: %s\n", mServerAddress.c_str());

	enetpp::client_connect_params params;
	params.set_channel_count(1);
	params.set_server_host_name_and_port(mServerAddress.c_str(), mPort);

	mClient.connect(params);
}

void Client::disconnect() {
	IO::printf("Disconnecting!");
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
		std::string message(reinterpret_cast<const char *>(data), size);
		IO::printf("Received %d bytes from server: %s\n", size, message.c_str());
	};

	if (hasPressedSpaceBar) {
		std::string message = "I have pressed da key yo!";
		mClient.send_packet(0, reinterpret_cast<const U8*>(message.c_str()), message.length(), ENET_PACKET_FLAG_RELIABLE);
	}

	mClient.consume_events(onConnect, onDisconnect, onReceiveData);
}