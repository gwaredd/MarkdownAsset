#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FMarkdownAssetModule : public IModuleInterface
{
	public:

		virtual void StartupModule() override {}
		virtual void ShutdownModule() override {}
		virtual bool SupportsDynamicReloading() override { return true; }
};

IMPLEMENT_MODULE( FMarkdownAssetModule, MarkdownAsset );
