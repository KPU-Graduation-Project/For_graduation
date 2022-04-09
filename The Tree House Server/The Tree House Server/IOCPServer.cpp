#pragma once
#include "stdafx.h"
#include "IOCPServer.h"
#include "ExpOver.h"

using namespace std;

void CIOCPServer::Open()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	this->BindAndListen(SERVER_PORT);
}

void CIOCPServer::Close()
{

}

void CIOCPServer::BindAndListen(USHORT server_port)
{
	m_listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(m_listen_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));

	listen(m_listen_socket, SOMAXCONN);

	m_h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_listen_socket), m_h_iocp, 0, 0);
		

	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	char	accept_buf[sizeof(SOCKADDR_IN) * 2 + 32 + 100];
	CEXP_OVER	accept_ex;
	*(reinterpret_cast<SOCKET*>(&accept_ex.m_net_buf)) = c_socket;
	ZeroMemory(&accept_ex.m_wsa_over, sizeof(accept_ex.m_wsa_over));
	accept_ex.m_comp_op = COMP_OP(OP_ACCEPT);

	AcceptEx(m_listen_socket, c_socket, accept_buf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &accept_ex.m_wsa_over);
	std::cout << "Accept Called\n";


	//WorkerThread();

}




void CIOCPServer::Disconnect()
{

}

CIOCPServer::CIOCPServer() {};
CIOCPServer::~CIOCPServer() {};

