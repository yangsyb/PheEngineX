#include "pch.h"
#include "PEngine.h"
#include "PStaticMesh.h"


int main()
{
	std::unique_ptr<Phe::PEngine> PheEngine = std::make_unique<Phe::PEngine>();
	PheEngine->Start();
	return 0;
}