#include "pch.h"

OS::OS(Computer *theComputer)
{
	m_Computer = theComputer;
}

bool OS::Boot(const char *Filename)
{
	if (!Load(Filename)) return false;
	return true;
}
