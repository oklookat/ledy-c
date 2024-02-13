#include "client.h"

using namespace client;

// 2 bytes.
static std::array<uint8_t, 2> u16toU8(uint16_t v) {
	std::array<uint8_t, 2> r{};
	std::memcpy(r.data(), &v, sizeof(v));
	return r;
}

// 4 bytes.
static std::array<uint8_t, 4> u32toU8(uint32_t v) {
	std::array<uint8_t, 4> r{};
	std::memcpy(r.data(), &v, sizeof(v));
	return r;
}

static std::vector<unsigned char> new5byteCommand(Command c, unsigned int v) {
	auto data = u32toU8(v);
	std::vector<unsigned char> result(5, 0);
	result[0] = c;
	size_t cmdI = 1;
	for (size_t i = 0; i < data.size(); i++)
	{
		result[cmdI] = data[i];
		cmdI++;
	}
	return result;
}

// <GRBGRBGRB>.
//
// 3 bytes per led.
static std::vector<unsigned char> rgbToU8(LEDS& leds) {
	size_t resultI = 0;
	std::vector<unsigned char> result(LEDS_COUNT * 3, 0);
	for (auto i = 0; i < LEDS_COUNT; i++)
	{
		result[resultI] = leds[i].G;
		result[resultI + 1] = leds[i].R;
		result[resultI + 2] = leds[i].B;
		resultI += 3;
	}
	return result;
}

static std::vector<unsigned char> newCommandSetColors(LEDS& leds) {

	std::vector<unsigned char> cmd(3 + LEDS_COUNT * 3, 0);

	cmd[0] = CommandSetColors;

	auto ledsBytes = rgbToU8(leds);

	// <2 BYTES UINT16 LEDS LENGTH>.
	auto ledsLenU8 = u16toU8(static_cast<uint16_t>(ledsBytes.size()));
	cmd[1] = ledsLenU8[0];
	cmd[2] = ledsLenU8[1];

	// <1 BYTE UINT8 RGB VALUES>.
	size_t cmdI = 3;
	for (size_t i = 0; i < ledsBytes.size(); i++)
	{
		cmd[cmdI] = ledsBytes[i];
		cmdI++;
	}

	return cmd;
}

static std::vector<unsigned char> newCommandSetColorCorrection(color::tCorrection v) {
	return new5byteCommand(CommandSetColorCorrection, v);
}

static std::vector<unsigned char> newCommandSetColorTemperature(color::tTemperature v) {
	return new5byteCommand(CommandSetColorTemperature, v);
}

Client::Client() {
	ix::initNetSystem();
}

Client::~Client()
{
	conn.close();
	ix::uninitNetSystem();
}

void Client::connect()
{
	auto ip = finder::findServer();

	conn.setUrl(std::format("ws://{}:{}", ip.to_string().c_str(), TARGET_WS_PORT));
	conn.setPingInterval(45);
	conn.setHandshakeTimeout(5);
	conn.disablePerMessageDeflate();
	conn.setMaxWaitBetweenReconnectionRetries(10 * 1000);

	conn.setOnMessageCallback([](const ix::WebSocketMessagePtr& msg)
		{

			if (msg->type == ix::WebSocketMessageType::Error)
			{
				throw std::runtime_error(std::format("WS ERROR: {}", msg->str.c_str()));
			} });

	conn.start();
}

void client::Client::setColors(LEDS& leds)
{
	conn.sendBinary(newCommandSetColors(leds));
}

void client::Client::setColorCorrection(color::tCorrection v)
{
	conn.sendBinary(newCommandSetColorCorrection(v));
}

void client::Client::setColorTemperature(color::tTemperature v)
{
	conn.sendBinary(newCommandSetColorTemperature(v));
}
