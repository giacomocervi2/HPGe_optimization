#include "HPGeActionInitialization.hh"

HPGeActionInitialization::HPGeActionInitialization()
{
}


HPGeActionInitialization::~HPGeActionInitialization()
{
}

void HPGeActionInitialization::BuildForMaster() const
{
	HPGeRunAction *runAction = new HPGeRunAction();
	SetUserAction(runAction);
}

void HPGeActionInitialization::Build() const
{
	HPGePrimaryGenerator *generator = new HPGePrimaryGenerator();
	SetUserAction(generator);

	HPGeRunAction *runAction = new HPGeRunAction();
	SetUserAction(runAction);
}
