#define SFML_STATIC 1
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <chrono>
#include "Protocol.h"

using namespace std;

#ifdef _DEBUG
#pragma comment (lib, "lib/sfml-graphics-s-d.lib")
#pragma comment (lib, "lib/sfml-window-s-d.lib")
#pragma comment (lib, "lib/sfml-system-s-d.lib")
#pragma comment (lib, "lib/sfml-network-s-d.lib")
#else
#pragma comment (lib, "lib/sfml-graphics-s.lib")
#pragma comment (lib, "lib/sfml-window-s.lib")
#pragma comment (lib, "lib/sfml-system-s.lib")
#pragma comment (lib, "lib/sfml-network-s.lib")
#endif
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "ws2_32.lib")


sf::TcpSocket socket;

constexpr auto BUF_SIZE = 256;
constexpr auto SCREEN_WIDTH = 16;
constexpr auto SCREEN_HEIGHT = 16;

constexpr auto TILE_WIDTH = 65;
constexpr auto WINDOW_WIDTH = TILE_WIDTH * SCREEN_WIDTH + 10;   // size of window
constexpr auto WINDOW_HEIGHT = TILE_WIDTH * SCREEN_WIDTH + 10;
//constexpr auto BUF_SIZE = MAX_BUFFER;

int g_myid;
int g_x_origin;
int g_y_origin;

sf::RenderWindow* g_window;
sf::Font g_font;

class OBJECT {
private:
	bool m_showing;
	sf::Sprite m_sprite;
	sf::Text m_name;
	sf::Text m_chat;
	chrono::system_clock::time_point m_mess_end_time;
public:
	int m_x, m_y;

	OBJECT(sf::Texture& t, int x, int y, int x2, int y2) {
		m_showing = false;
		m_sprite.setTexture(t);
		m_sprite.setTextureRect(sf::IntRect(x, y, x2, y2));
		set_name("NONAME");
		m_mess_end_time = chrono::system_clock::now();
	}
	OBJECT() {
		m_showing = false;
	}
	void show()
	{
		m_showing = true;
	}
	void hide()
	{
		m_showing = false;
	}

	void a_move(int x, int y) {
		m_sprite.setPosition((float)x, (float)y);
	}

	void a_draw() {
		g_window->draw(m_sprite);
	}

	void move(int x, int y) {
		m_x = x;
		m_y = y;
	}
	void draw() {
		if (false == m_showing) return;
		float rx = (m_x - g_x_origin) * 65.0f + 8;
		float ry = (m_y - g_y_origin) * 65.0f + 8;
		m_sprite.setPosition(rx, ry);
		g_window->draw(m_sprite);
		if (m_mess_end_time < chrono::system_clock::now()) {
			m_name.setPosition(rx - 10, ry - 20);
			g_window->draw(m_name);
		}
		else {
			m_chat.setPosition(rx - 10, ry - 20);
			g_window->draw(m_chat);
		}
	}
	void set_name(const char str[]) {
		m_name.setFont(g_font);
		m_name.setString(str);
		m_name.setFillColor(sf::Color(255, 255, 0));
		m_name.setStyle(sf::Text::Bold);
	}
	void set_chat(const char str[]) {
		m_chat.setFont(g_font);
		m_chat.setString(str);
		m_chat.setFillColor(sf::Color(255, 255, 255));
		m_chat.setStyle(sf::Text::Bold);
		m_mess_end_time = chrono::system_clock::now() + chrono::seconds(3);
	}
};

OBJECT avatar;
OBJECT players[MAX_USER + MAX_NPC];

OBJECT white_tile;
OBJECT black_tile;

sf::Texture* board;
sf::Texture* pieces;

void client_initialize()
{
	board = new sf::Texture;
	pieces = new sf::Texture;
	if (false == g_font.loadFromFile("cour.ttf")) {
		cout << "Font Loading Error!\n";
		while (true);
	}
	board->loadFromFile("chessmap.bmp");
	pieces->loadFromFile("chess2.png");
	white_tile = OBJECT{ *board, 5, 5, TILE_WIDTH, TILE_WIDTH };
	black_tile = OBJECT{ *board, 69, 5, TILE_WIDTH, TILE_WIDTH };
	avatar = OBJECT{ *pieces, 128, 0, 64, 64 };
	for (auto& pl : players) {
		pl = OBJECT{ *pieces, 0, 0, 64, 64 };
	}
}

void client_finish()
{
	delete board;
	delete pieces;
}

void ProcessPacket(char* ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{

	case SC_PACKET::SC_PLAYER_DATA:
	{
		sc_player_data* packet = reinterpret_cast<sc_player_data*>(ptr);

		cout << "player "<<packet->id <<" position: " << packet->player_position.x << ", " << packet->player_position.y << ", "
			<< packet->player_position.z << endl;
		break;
	};
	//case SC_PACKET_LOGIN_OK:
	//{
	//	sc_packet_login_ok* packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
	//	g_myid = packet->id;
	//	avatar.m_x = packet->x;
	//	avatar.m_y = packet->y;
	//	g_x_origin = packet->x - SCREEN_WIDTH / 2;
	//	g_y_origin = packet->y - SCREEN_WIDTH / 2;
	//	avatar.move(packet->x, packet->y);
	//	avatar.show();
	//}
	//break;
	//case SC_PACKET_PUT_OBJECT:
	//{
	//	sc_packet_put_object* my_packet = reinterpret_cast<sc_packet_put_object*>(ptr);
	//	int id = my_packet->id;

	//	if (id < MAX_USER) { // PLAYER
	//		players[id].set_name(my_packet->name);
	//		players[id].move(my_packet->x, my_packet->y);
	//		players[id].show();
	//	}
	//	else {  // NPC
	//		players[id].set_name(my_packet->name);
	//		players[id].move(my_packet->x, my_packet->y);
	//		players[id].show();
	//	}
	//	break;
	//}
	//case SC_PACKET_MOVE:
	//{
	//	sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
	//	int other_id = my_packet->id;
	//	if (other_id == g_myid) {
	//		avatar.move(my_packet->x, my_packet->y);
	//		g_x_origin = my_packet->x - SCREEN_WIDTH / 2;
	//		g_y_origin = my_packet->y - SCREEN_WIDTH / 2;
	//	}
	//	else if (other_id < MAX_USER) {
	//		players[other_id].move(my_packet->x, my_packet->y);
	//	}
	//	else {
	//		players[other_id].move(my_packet->x, my_packet->y);
	//	}
	//	break;
	//}

	//case SC_PACKET_REMOVE_OBJECT:
	//{
	//	sc_packet_remove_object* my_packet = reinterpret_cast<sc_packet_remove_object*>(ptr);
	//	int other_id = my_packet->id;
	//	if (other_id == g_myid) {
	//		avatar.hide();
	//	}
	//	else if (other_id < MAX_USER) {
	//		players[other_id].hide();
	//	}
	//	else {
	//		players[other_id].hide();
	//	}
	//	break;
	//}

	//case SC_PACKET_CHAT:
	//{
	//	sc_packet_chat* my_packet = reinterpret_cast<sc_packet_chat*>(ptr);
	//	int other_id = my_packet->id;
	//	if (other_id == g_myid) {
	//		avatar.set_chat(my_packet->message);
	//	}
	//	else if (other_id < MAX_USER) {
	//		players[other_id].set_chat(my_packet->message);
	//	}
	//	else {
	//		players[other_id].set_chat(my_packet->message);
	//	}
	//	break;
	//}
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void process_data(char* net_buf, size_t io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[BUF_SIZE];

	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
}

bool client_main()
{
	char net_buf[BUF_SIZE];
	size_t	received;

	auto recv_result = socket.receive(net_buf, BUF_SIZE, received);
	if (recv_result == sf::Socket::Error)
	{
		wcout << L"Recv 에러!";
		while (true);
	}
	if (recv_result == sf::Socket::Disconnected)
	{
		wcout << L"서버 접속 종료.\n";
		return false;
	}
	if (recv_result != sf::Socket::NotReady)
		if (received > 0) process_data(net_buf, received);

	for (int i = 0; i < SCREEN_WIDTH; ++i)
		for (int j = 0; j < SCREEN_HEIGHT; ++j)
		{
			int tile_x = i + g_x_origin;
			int tile_y = j + g_y_origin;
			if ((tile_x < 0) || (tile_y < 0)) continue;
			if ((((tile_x / 3) + (tile_y / 3)) % 2) == 1) {
				white_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				white_tile.a_draw();
			}
			else
			{
				black_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				black_tile.a_draw();
			}
		}
	avatar.draw();
	for (auto& pl : players) pl.draw();
	sf::Text text;
	text.setFont(g_font);
	char buf[100];
	sprintf_s(buf, "(%d, %d)", avatar.m_x, avatar.m_y);
	text.setString(buf);
	g_window->draw(text);
	return true;
}

//void send_move_packet(char dr)
//{
//	cs_packet_move packet;
//	packet.size = sizeof(packet);
//	packet.type = CS_PACKET_MOVE;
//	packet.direction = dr;
//	size_t sent = 0;
//	socket.send(&packet, sizeof(packet), sent);
//}
//
//void send_login_packet(string& name)
//{
//	cs_packet_login packet;
//	packet.size = sizeof(packet);
//	packet.type = CS_PACKET_LOGIN;
//	strcpy_s(packet.name, name.c_str());
//	size_t sent = 0;
//	socket.send(&packet, sizeof(packet), sent);
//}

int main()
{
	wcout.imbue(locale("korean"));
	sf::Socket::Status status = socket.connect("127.0.0.1", SERVER_PORT);


	socket.setBlocking(false);

	if (status != sf::Socket::Done) {
		wcout << L"서버와 연결할 수 없습니다.\n";
		while (true);
	}

	client_initialize();
	string name{ "PL" };
	auto tt = chrono::duration_cast<chrono::milliseconds>
		(chrono::system_clock::now().
			time_since_epoch()).count();
	name += to_string(tt % 1000);
	//send_login_packet(name);
	avatar.set_name(name.c_str());
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT");
	g_window = &window;

	while (window.isOpen())
	{
		cs_player_data packet;
		packet.info.size = sizeof(cs_player_data);
		packet.info.type = CS_PACKET::CS_PLAYER_DATA;
		packet.player_position = SVector3{ (short)avatar.m_x, (short)avatar.m_y,0 };
		packet.player_rotation = SVector3{ 1,2,3 };
		size_t size = 0;
		socket.send(&packet, sizeof(packet), size);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				int direction = -1;
				switch (event.key.code) {
				case sf::Keyboard::Left:
					direction = 2;
					break;
				case sf::Keyboard::Right:
					direction = 3;
					break;
				case sf::Keyboard::Up:
					direction = 0;
					break;
				case sf::Keyboard::Down:
					direction = 1;
					break;
				case sf::Keyboard::Escape:
					window.close();
					break;
				}
				if (-1 != direction)
				{
					switch (direction) {
					case 0: avatar.m_y--; break;
					case 1: avatar.m_y++; break;
					case 2: avatar.m_x--; break;
					case 3: avatar.m_x++; break;
					}
				}
			}

			window.clear();
			if (false == client_main())
				window.close();
			window.display();
		}
		client_finish();

		return 0;
	}
}