#include "pch.h"

bool operator==(const PCB& pcb1, const PCB& pcb2)
{
	if (&pcb1 == &pcb2) return true;
	return ( //pcb1.running == pcb2.running &&
		pcb1.priority == pcb2.priority &&
		pcb1.arrived == pcb2.arrived);
}

bool operator != (const PCB& pcb1, const PCB& pcb2)
{
	return (!(pcb1 == pcb2));
}

bool operator > (const PCB& pcb1, const PCB& pcb2)
{
	//if (pcb1.running > pcb2.running)
	//	return true;
	//if (pcb1.running < pcb2.running)
	//	return false;
	if (pcb1.priority < pcb2.priority)
		return true;
	if (pcb1.priority > pcb2.priority)
		return false;
	if (pcb1.arrived < pcb2.arrived)
		return true;
	else
		return false;
}

bool operator < (const PCB& pcb1, const PCB& pcb2)
{
	return ((pcb1 != pcb2) && !(pcb1 > pcb2));
}

bool operator <= (const PCB& pcb1, const PCB& pcb2)
{
	return (pcb1 > pcb2 || pcb1 == pcb2);
}

bool operator >= (const PCB& pcb1, const PCB& pcb2)
{
	return (pcb1 > pcb2 || pcb1 == pcb2);
}
