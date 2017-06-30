#ifndef LOOKATCOMPONENT_H
#define LOOKATCOMPONENT_H

#include <engine/components/gamecomponent.h>

class CoreEngine;
class LookAtComponent : public GameComponent
{
public:
	virtual void update(float dt) override;
	virtual void addToEngine(CoreEngine &) override;

private:
	CoreEngine *m_engine;
};

#endif // LOOKATCOMPONENT_H
