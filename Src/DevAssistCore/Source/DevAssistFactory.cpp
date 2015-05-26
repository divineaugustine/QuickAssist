#include "stdafx.h"
#include <Core\DevAssistFactory.h>
#include "FileFactory.h"

	DevAssistFactory& DevAssistFactory::Instance()
	{
		static FileFactory obj;
		return obj;
	}

	DevAssistFactory::DevAssistFactory()
	{
	}

	DevAssistFactory::~DevAssistFactory()
	{
	}