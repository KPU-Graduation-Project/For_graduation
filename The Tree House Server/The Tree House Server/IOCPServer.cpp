#pragma once
#include "stdafx.h"
#include "IOCPServer.h"
#include "ExpOver.h"

using namespace std;

void cIOCPServer::StartServer()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	this->BindAndListen(SERVER_PORT);
}

void cIOCPServer::CloseServer()
{

}

void cIOCPServer::BindAndListen(USHORT server_port)
{
	m_listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(m_listen_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));	
	listen(m_listen_socket, SOMAXCONN);

	m_h_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_listen_socket), m_h_IOCP, 0, 0);		

	 m_client_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	char	accept_buf[sizeof(SOCKADDR_IN) * 2 + 32 + 100];
	*(reinterpret_cast<SOCKET*>(&m_accept_over.m_net_buf)) = m_client_socket;
	ZeroMemory(&m_accept_over.m_wsa_over, sizeof(m_accept_over.m_wsa_over));
	m_accept_over.m_comp_op = OP_TYPE(OP_ACCEPT);

	
	cout << AcceptEx(m_listen_socket, m_client_socket, accept_buf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &m_accept_over.m_wsa_over) << endl;
	//int err_no = WSAGetLastError();
	//cout << "GQCS Error : ";
	//if (err_no == ERROR_INVALID_HANDLE)
	//{
	//	cout << "handle_error" << endl;
	//}
	////ErrorDisplay(err_no);
	//cout << err_no << endl;


	
	std::cout << "Server Launched\n";
}

void cIOCPServer::Disconnect()
{

}

cIOCPServer::cIOCPServer() {};
cIOCPServer::~cIOCPServer() {};

