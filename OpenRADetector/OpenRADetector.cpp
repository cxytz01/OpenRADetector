#include <iostream>
#include <optional>

#include <thread>
#include <chrono>

#include <string>
#include <unordered_map>

#include "nlohmann/json.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT // Install openssl 1.1.1, and configure openssl with vc++ https://stackoverflow.com/questions/31646663/unresolved-external-symbol-openssl-libraries-c
#include "httplib.h" // https://github.com/yhirose/cpp-httplib

#include "windows.h" //https://stackoverflow.com/questions/1372480/c-redefinition-header-files-winsock2-h

#include "OpenRABiz.h"
#include "process_handle.h"
#include "console_cls.h"
#include "defer.h"

#include "utility/color_test.h"

using std::string;
using std::unordered_map;

struct OpenRAInfo {
	const string WatchHost = R"(https://master.openra.net)";
	const string WatchPath = R"(/games?protocol=2&type=json)";
};

inline const httplib::Headers generateOpenRAHeader() {
	const static httplib::Headers headers = {
		{"Connection", "keep-alive"},
		{"Accept","application/json, text/javascript,*/*; q=0.01"},
		{"Origin", "https://www.openra.net"},
		{"Sec-Fetch-Site", "same-site"},
		{"Sec-Fetch-Mode", "cors"},
		{"Sec-Fetch-Dest", "empty"}
	};

	return headers;
}

std::optional<string> httprequest(string_view host, string_view path, const httplib::Headers headers) {
	if (auto res = httplib::Client(host.data()).Get(path.data(), headers); 200 != res->status) {
		std::cerr << res.error() << std::endl;
		return std::nullopt;
	}
	else return res->body;
}

int main(int argc, char *argv[]) {
	//color_test();

	/*defer _(nullptr, [](...) {
		std::cout << "detector exits." << std::endl;
		std::cin.get();
		});*/

	const OpenRAInfo d2k;

	for (;; std::this_thread::sleep_for(std::chrono::seconds(30)), ClearScreen()) {
		if (DetectD2kProcess()) continue;

		auto raw = httprequest(d2k.WatchHost, d2k.WatchPath, generateOpenRAHeader());
		if (std::nullopt == raw) continue;
		//LOG(INFO) << raw;

		nlohmann::json gameRoomInfo = nlohmann::json::parse(raw.value());
		//string pretty = gameRoomInfo.dump(4); LOG(INFO) << pretty

		uint16_t index = 0;
		bool has_waiting_room = false;
		unordered_map<string, D2KBiz> rooms;

		for (auto& [key, value] : gameRoomInfo.items()) {
			const D2KBiz d2kbz(value);
			if (!d2kbz.Is_20210321_d2k_version()) continue;

			if (!d2kbz.Is_waiting()) continue;
			//if (d2kbz.Is_empty() /*|| d2kbz.Is_locked() || d2kbz.Is_playing()*/) continue;

			d2kbz.print(index);
			rooms.insert({ move(std::to_string(index++)), d2kbz });

			if (d2kbz.Is_waiting()) {
				has_waiting_room = true;
				SetForegroundWindow(GetConsoleWindow());
			}
		}

		for (;has_waiting_room;) {
#define LINEBUF 128
			static char line[LINEBUF];

			auto start = std::chrono::steady_clock::now();

			std::cout << "ROOM: "; std::cin.getline(line, LINEBUF);

			string_view str_line(line);
			if ("q" == str_line || "Q" == str_line || "quit" == str_line || "QUIT" == str_line) {
				std::cout << " " << str_line << " to next loop." << std::endl;
				break;
			}

			auto end = std::chrono::steady_clock::now();
			auto elpase = std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
			if (elpase >= 3) {
				std::cout << "time lapse, loop..." << std::endl;
				break;
			}

			if (auto it = rooms.find(line); it != rooms.end()) {
				if (rooms[line].Is_playing()) {
					printf("  room: \"%s\" is playing.\n", line);
					continue;
				} 
				if (rooms[line].Is_locked()) {
					printf("  room: \"%s\" is locked.\n", line);
					continue;
				} 
				if (DetectD2kProcess()) {
					printf("  the game(d2k) has running.\n");
					continue;
				}

				//printf("Entering room =====>>> Game@%s: \"\033[7m%s\033[0m\"/\033[4m%s\033[0m\n", line, rooms[line].GetName().c_str(), rooms[line].GetLocation().c_str());
				printf("Entering room =====>>> Game@%s: \"%s\"/%s\n", line, rooms[line].GetName().c_str(), rooms[line].GetLocation().c_str());

				if (argc >= 2) {
					sprintf_s(line, "\"%s\" Launch.URI=openra-%s-%s://%s/", argv[1], rooms[line].GetMod().c_str(), rooms[line].GetVersion().c_str(), rooms[line].GetAddress().c_str());
					OpenD2KandWaitFor(line);
				}

				break; // break and wait for d2k exist;
			} else printf("  room: \"%s\" not found.\n", line);
		}
	}

	return 0;
}