#ifndef EXPLICTSCHEMA_H
#define EXPLICTSCHEMA_H

#include "isolver.h"

class ExplictSchema : public ISolver
{
protected:
	void step() override;
};

#endif // EXPLICTSCHEMA_H
