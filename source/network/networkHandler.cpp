//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "network/networkHandler.h"
#include "base/io.h"
#include <enetpp/server_listen_params.h>

U32 NetworkHandler::sUniqueId = 0;

NetworkHandler::NetworkHandler() {

}

NetworkHandler::~NetworkHandler() {

}

void NetworkHandler::start() {
	enetpp::server_listen_params<Connection> params;
	params.set_max_client_count(16);
	params.set_channel_count(1);
	params.set_listen_port(28000);
	params.set_initialize_client_function([](Connection &client, const char *ipAddress) {
		// Initializes the client by assining its IP address and its unique identifier.
		client.ipAddress = ipAddress;
		client.id = sUniqueId;
		sUniqueId++;
	});
	mServer.start_listening(params);
}

void NetworkHandler::stop() {
	mServer.stop_listening();
}

void NetworkHandler::pollEvents() {
	// receive events.
	auto onClientConnect = [](Connection &client) {
		IO::printf("Client %u with IP %s has joined the server.\n", client.id, client.ipAddress.c_str());
	};

	auto onClientDisconnect = [](U32 clientId) {
		IO::printf("Client %u has left the server.\n", clientId);
	};

	auto onReceiveData = [this](Connection &client, const U8 *data, size_t size) {
		std::string message(reinterpret_cast<const char *>(data), size);
		IO::printf("Receive %d bytes from Client %u: %s\n", size, client.id, message.c_str());

		// encode message back
		char *buffer;
		size_t len = asprintf(&buffer, "Client %u: %s", client.id, message.c_str());

		// send your message back, what you said
		mServer.send_packet_to_all_if(0, reinterpret_cast<U8*>(buffer), len, ENET_PACKET_FLAG_RELIABLE, [](const Connection &) { return true; } );

		free(buffer);
	};

	mServer.consume_events(onClientConnect, onClientDisconnect, onReceiveData);
}