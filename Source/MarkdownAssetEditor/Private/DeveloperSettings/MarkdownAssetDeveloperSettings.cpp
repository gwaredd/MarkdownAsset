#include "DeveloperSettings/MarkdownAssetDeveloperSettings.h"

#include "ISettingsModule.h"

const UMarkdownAssetDeveloperSettings* UMarkdownAssetDeveloperSettings::Get()
{
	return GetDefault<UMarkdownAssetDeveloperSettings>();
}

#if WITH_EDITOR
void UMarkdownAssetDeveloperSettings::OpenEditorSettingWindow() const
{
	static ISettingsModule& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	SettingsModule.ShowViewer(GetContainerName(), GetCategoryName(), GetSectionName());
}
#endif //WITH_EDITOR 
