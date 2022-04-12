#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "protocol.h"

enum COMP_OP { OP_RECV, OP_SEND, OP_ACCEPT };

class CEXP_OVER {
public:
	WSAOVERLAPPED	m_wsa_over;
	COMP_OP			m_comp_op;
	WSABUF			m_wsa_buf;
	unsigned char	m_net_buf[BUFSIZE];
public:
	CEXP_OVER(COMP_OP comp_op, char num_bytes, void* mess);

	CEXP_OVER(COMP_OP comp_op);

	CEXP_OVER();

	~CEXP_OVER();
};