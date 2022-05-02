#pragma once
#include "stdafx.h"
#include "ExpOver.h"


cExpOver::cExpOver(OP_TYPE comp_op, char _size, void* mess) : m_comp_op(comp_op)
{
	ZeroMemory(&m_wsa_over, sizeof(m_wsa_over));
	m_wsa_buf.buf = reinterpret_cast<char*>(m_net_buf);
	m_wsa_buf.len = _size;
	memcpy(m_net_buf, mess, _size);
}

cExpOver::cExpOver(OP_TYPE comp_op) : m_comp_op(comp_op) {}

cExpOver::cExpOver()
{
	m_comp_op = OP_RECV;
}

cExpOver::~cExpOver()
{
}

