#include "OpenRABiz.h"
#include "spdlog/spdlog.h"

OpenRABiz::OpenRABiz(decltype(nlohmann::json::parse("placeholder").items().begin().value()) &v) {

	if (v.contains(key_id)) SetId(v[key_id]);
	if (v.contains(key_name)) SetName(v[key_name]);
	if (v.contains(key_address)) SetAddress(v[key_address]);
	if (v.contains(key_state)) SetState(v[key_state]);
	if (v.contains(key_ttl)) SetTTL(v[key_ttl]);
	if (v.contains(key_mod)) SetMod(v[key_mod]);
	if (v.contains(key_version)) SetVersion(v[key_version]);
	if (v.contains(key_modtitle)) SetModtitle(v[key_modtitle]);
	if (v.contains(key_modwebsite)) SetModwebsite(v[key_modwebsite]);
	if (v.contains(key_modicon32)) SetModicon32(v[key_modicon32]);
	if (v.contains(key_map)) SetMap(v[key_map]);
	if (v.contains(key_players)) SetPlayers(v[key_players]);
	if (v.contains(key_maxplayers)) SetMaxplayers(v[key_maxplayers]);
	if (v.contains(key_bots)) SetBots(v[key_bots]);
	if (v.contains(key_spectators)) SetSpectators(v[key_spectators]);
	if (v.contains(key_protected)) SetProtected(v[key_protected]);
	if (v.contains(key_authentication)) SetAuthentication(v[key_authentication]);
	if (v.contains(key_location)) SetLocation(v[key_location]);
	if (v.contains(key_started)) SetStarted(v[key_started]);
	if (v.contains(key_playtime)) SetPlaytime(v[key_playtime]);

	if (v.contains(key_clients)) {
		for (auto& [ck, cv] : v[key_clients].items()) {

			if (cv.contains(ClientInfo::key_name)
                && cv.contains(ClientInfo::key_fingerprint)
                && cv.contains(ClientInfo::key_color)
                && cv.contains(ClientInfo::key_faction)
                && cv.contains(ClientInfo::key_team)
                && cv.contains(ClientInfo::key_spawnpoint)
                && cv.contains(ClientInfo::key_isadmin)
                && cv.contains(ClientInfo::key_isspectator)
                && cv.contains(ClientInfo::key_isbot)
               )
            {
				/*ClientInfo ci{
					cv[ClientInfo::key_name].get<string>(),
					cv[ClientInfo::key_fingerprint].get<string>(),
					cv[ClientInfo::key_color].get<string>(),
					cv[ClientInfo::key_faction].get<string>(),
					cv[ClientInfo::key_team].get<uint16_t>(),
					cv[ClientInfo::key_spawnpoint].get<uint16_t>(),
					cv[ClientInfo::key_isadmin].get<bool>(),
					cv[ClientInfo::key_isspectator].get<bool>(),
					cv[ClientInfo::key_isbot].get<bool>()
				};

				SetClients(ci);*/
				SetClients(
					cv[ClientInfo::key_name].get<string>(),
					cv[ClientInfo::key_fingerprint].get<string>(),
					cv[ClientInfo::key_color].get<string>(),
					cv[ClientInfo::key_faction].get<string>(),
					cv[ClientInfo::key_team].get<uint16_t>(),
					cv[ClientInfo::key_spawnpoint].get<uint16_t>(),
					cv[ClientInfo::key_isadmin].get<bool>(),
					cv[ClientInfo::key_isspectator].get<bool>(),
					cv[ClientInfo::key_isbot].get<bool>()
				);
			}
		}
	}
}

template<class _Valty>
void OpenRABiz::SetLocation(const _Valty& _Val) {
	if (_Val.is_string()) location = _Val;
	else location = "¡ª";
};

vector<const ClientInfo*> OpenRABiz::GetHumans() const {
	vector<const ClientInfo*> vec;
	for (auto &c : clients) {
		if (!c.isbot) vec.emplace_back(&c);
	}
	return vec; // RVO - return value optimization
}

template<class... _Valty>
vector<ClientInfo>& OpenRABiz::SetClients(_Valty&&... _Val) {
	clients.emplace_back(std::forward<_Valty>(_Val)...); // just constructor emplace.
	return clients;
}

/*vector<ClientInfo>& OpenRABiz::SetClients(ClientInfo &t) {
	clients.emplace_back(t); // call copy constructor
	return clients;
};*/

//void D2KBiz::print(uint16_t index) const {
//	printf("Game@%u:\n", index);
//
//	printf("  Name%s: \033[7m%s\033[0m\n", 0 != protectedx ? "(\033[31mLOCK\033[0m)" : "", name.c_str());
//
//	printf("  Address: %s\n", address.c_str());
//	printf("  State: %s", 2 == state?"\033[32mPLAYING\033[0m":"\033[33mWAITING\033[0m");
//
//	if (2 == state) printf(" - In progress for %u minutes", playtime / 60);
//	printf("\n");
//
//	printf("  Mods: %s@%s\n", mod.c_str(), version.c_str());
//	printf("  Players: %u/%u+%u, Bots: %u\n", players, maxplayers, spectators, bots);
//	printf("  \t");
//
//	auto humans = move(GetHumans());
//	for (auto &h : humans) printf("\033[96m+%s\033[0m ", h->name.c_str());
//	if (0 == humans.size()) printf("......");
//	printf("\n");
//	printf("  Location: %s\n", location.c_str());
//	printf("---------------------------------------\n");
//}

void D2KBiz::print(uint16_t index) const {
	spdlog::info("Game@{}:", index);
	spdlog::info("  Name{}: {}", 0 != protectedx ? "(LOCK)" : "", name);

	spdlog::info("  Address: {}", address);
	if (Is_empty()) spdlog::info("  State: EMPTY");
	if (Is_waiting()) spdlog::info("  State: WAITING");
	if (Is_playing()) spdlog::info ("  State: PLAYING - In progress for {} minutes", playtime / 60);

	spdlog::info("  Mods: {}@{}", mod, version);
	spdlog::info("  Players: {}/{}+{}, Bots: {}", players, maxplayers, spectators, bots);

	auto humans = move(GetHumans());
	string players = "  \t";
	for (auto& h : humans) players += "+" + h->name + " ";
	if (0 != humans.size()) spdlog::info(players);
	spdlog::info("  Location: {}", location);
	spdlog::info("---------------------------------------");
}


