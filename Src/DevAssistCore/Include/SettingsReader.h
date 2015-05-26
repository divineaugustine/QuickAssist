
#include <vector>
#include <string>
class RepositorySettings;
class CSettingsReader
{
public:

	static CSettingsReader& Instance()
	{
		static CSettingsReader obj;
		return obj;
	}

	bool ReadSettings( const std::wstring& csSettingsFileName, RepositorySettings& objSettings_o );
	bool WriteSettings( const RepositorySettings& objSettings_i, const std::wstring& csSettingsFileName );

private:
	CSettingsReader(){};
};