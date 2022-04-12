#pragma once
#include "stdafx.h"
#include "ExpOver.h"


CEXP_OVER::CEXP_OVER(OP_TYPE comp_op, char num_bytes, void* mess) : m_comp_op(comp_op)
{
	ZeroMemory(&m_wsa_over, sizeof(m_wsa_over));
	m_wsa_buf.buf = reinterpret_cast<char*>(m_net_buf);
	m_wsa_buf.len = num_bytes;
	memcpy(m_net_buf, mess, num_bytes);
}

CEXP_OVER::CEXP_OVER(OP_TYPE comp_op) : m_comp_op(comp_op) {}

CEXP_OVER::CEXP_OVER()
{
	m_comp_op = OP_RECV;
}

CEXP_OVER::~CEXP_OVER()
{
}

