#include <map>
#include <vector>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Constants.hpp"
#include "utils.h"
#include "AdvCommands.hpp"

using namespace app;
using namespace std;
using json = nlohmann::json;

namespace AdvCommands
{
	std::map<int32_t, vector<int32_t>> AdvCommands::Commands = map<int32_t, vector<int32_t>>();
	std::map<int32_t, ReadCommandCallback> AdvCommands::Callbacks = map<int32_t, ReadCommandCallback>();

	bool AdvCommands::SubscribeCommand(int32_t cmdID, ReadCommandCallback functionPTR)
	{
		auto pos = Callbacks.find(cmdID);

		if (pos != Callbacks.end())
		{
			printf("ERROR: AdvCommand %d already has been subscribed to\n", cmdID);
			return false;
		}
		else
		{
			Callbacks.emplace(cmdID, functionPTR);
			printf("AdvCommand %d has been subscribed to\n", cmdID);

			return true;
		}
	}

	void AdvCommands::Initialize()
	{
		auto dir = filesystem::absolute(std::format("{}/AdvCommands", Constants::RigbarthPath));
		for (const auto& dirEntry : filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
		{
			if (dirEntry.is_regular_file())
			{
				auto path = dirEntry.path();
				if (utils::string::iequals(path.extension().generic_string(), ".json"))
				{
					ifstream file(path);
					auto json = json::parse(file, nullptr, false);
					if (json.is_discarded())
					{
						printf("Invalid JSON: %s\n", path.generic_string().c_str());
						continue;
					}

					for (auto entry : json)
					{
						auto id = entry["ID"].get<int32_t>();
						auto pos = Commands.find(id);

						if (pos != Commands.end())
						{
							printf("ERROR: Duplicate AdvCommand ID: %d\n", id);
						}
						else
						{
							auto params = vector<int32_t>();
							for (auto& el : entry["Params"].items()) {
								auto value = el.value().get<string>();
								if (value == "string")
								{
									params.push_back(-1);
								}
								else if (value == "int")
								{
									params.push_back(4);
								}
							}
							Commands.emplace(id, params);
						}
					}
				}
			}
		}
	}
}