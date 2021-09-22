#include "spdlog/spdlog.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <unordered_map>

#include "OpenRABiz.h"
#include "utility/path.h"
#include "utility/defer.h"
#include "utility/console_cls.h"
#include "utility/detector_net.h"
#include "utility/process_handle.h"

#include "nlohmann/json.hpp"

using std::string;
using std::unordered_map;
struct OpenRAInfo {
	inline static const string WatchHost = std::move(R"(https://master.openra.net)");
	inline static const string WatchPath = std::move(R"(/games?protocol=2&type=json)");
};

bool GameConditionsFilter(unordered_map<string, D2KBiz> &rooms, std::string &&line) {
	if (rooms[line].Is_playing()) {
		spdlog::error("ROOM: [{}] is playing.", line);
		return false;
	} 
	if (rooms[line].Is_locked()) {
		spdlog::error("ROOM: [{}] is locked.", line);
		return false;
	} 
	if (DetectD2kProcess()) {
		spdlog::error("the game(d2k) has running.");
		return false;
	}
	return true;
}

int main(int argc, char *argv[]) {
	if (argc < 2 || !IsD2KPathExist(argv[1])) {
		spdlog::critical("PLS enter the correct OpenRA's D2K program path.");
		return 0;
	}

	for (;; std::this_thread::sleep_for(std::chrono::seconds(30)), ClearScreen()) {
		spdlog::info("Detecting...\n");

		if (DetectD2kProcess()) {
			spdlog::warn("D2K process is running. Stop the game lobby detecting until the next loop in 30 seconds.");
			continue;
		}

		auto raw = httprequest(OpenRAInfo::WatchHost, OpenRAInfo::WatchPath, generateOpenRAHeader());
		if (std::nullopt == raw) {
			spdlog::warn("The network operation error has been detected.Entering the next loop.");
			continue;
		}
		nlohmann::json gameRoomInfo = nlohmann::json::parse(raw.value()); //string pretty = gameRoomInfo.dump(4); LOG(INFO) << pretty

		uint16_t index = 0;
		bool has_waiting_room = false;
		unordered_map<string, D2KBiz> rooms;

		for (auto& [key, value] : gameRoomInfo.items()) {
			const D2KBiz d2kbz(value);

			if (!d2kbz.Is_20210321_d2k_version()) continue; //if (d2kbz.Is_empty() || d2kbz.Is_locked() || d2kbz.Is_playing()) continue;
			if (!d2kbz.Is_waiting()) continue;
			else {
				has_waiting_room = true;
				FLASHWINFO fwi{ sizeof FLASHWINFO, GetConsoleWindow(), FLASHW_ALL | FLASHW_TIMERNOFG, 10, 0 };
				FlashWindowEx(&fwi);
				//SetForegroundWindow(GetConsoleWindow());
			}

			d2kbz.print(index);
			rooms.insert({ move(std::to_string(index++)), d2kbz });
		}

		for (; has_waiting_room; ) {
#define LINEBUF 128
			static char line[LINEBUF];

			auto start = std::chrono::steady_clock::now();
			auto elapse = (nullptr, [&](...) -> bool {
				if (std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - start).count() >= 3) {
					spdlog::warn("Time lapse, looping...");
					return true;
				}
				return false;
			});

			std::cout << "ROOM: "; std::cin.getline(line, LINEBUF);
			string_view str_line(line);
			if ("q" == str_line || "Q" == str_line || "quit" == str_line || "QUIT" == str_line) {
				spdlog::info("{} to next loop.", str_line);
				break;
			}
			if (elapse()) break;

			if (auto it = rooms.find(line); it == rooms.end()) {
				spdlog::error("ROOM: [{}] not found.", line);
				continue;
			}
			if (!GameConditionsFilter(rooms, string(line))) continue;

			spdlog::info("Entering room =====>>> Game@{}: \"{}\"/{}", line, rooms[line].GetName(), rooms[line].GetLocation());

			sprintf_s(line, "\"%s\" Launch.URI=openra-%s-%s://%s/", argv[1], rooms[line].GetMod().c_str(), rooms[line].GetVersion().c_str(), rooms[line].GetAddress().c_str());
			OpenD2KandWaitForIt(line);
			spdlog::info("The game has been exit, looping...");
			break;
		}
	}

	return 0;
}
