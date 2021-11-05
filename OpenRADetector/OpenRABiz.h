#pragma once

#include <vector>
#include <string>
#include <string_view>

#include "nlohmann/json.hpp"

using std::vector;
using std::string;
using std::string_view;

class ClientInfo {
public:
	ClientInfo(string_view name,
			   string_view fingerprint,
		       string_view color,
			   string_view faction, 
			   uint16_t    team,
			   uint16_t    spawnpoint,
		       bool        isadmin,
			   bool        isspectator,
		       bool        isbot) :
			name(name),
		    fingerprint(fingerprint),
		    color(color),
			faction(faction),
			team(team),
			spawnpoint(spawnpoint),
			isadmin(isadmin),
			isspectator(isspectator),
			isbot(isbot) {}

	ClientInfo(const ClientInfo &other) : 
			name(other.name),
		    fingerprint(other.fingerprint),
		    color(other.color),
			faction(other.faction),
			team(other.team),
			spawnpoint(other.spawnpoint),
			isadmin(other.isadmin),
			isspectator(other.isspectator),
			isbot(other.isbot) {}

	ClientInfo(ClientInfo &&other) :
			name(move(other.name)),
		    fingerprint(move(other.fingerprint)),
		    color(move(other.color)),
			faction(move(other.faction)),
			team(other.team),
			spawnpoint(other.spawnpoint),
			isadmin(other.isadmin),
			isspectator(other.isspectator),
			isbot(other.isbot) {}

	string   name;
	string   fingerprint;
	string   color;
	string   faction;
	uint16_t team;
	uint16_t spawnpoint;
	bool     isadmin;
	bool     isspectator;
	bool     isbot;

	inline const static string key_name = "name";
	inline const static string key_fingerprint = "fingerprint";
	inline const static string key_color = "color";
	inline const static string key_faction = "faction";
	inline const static string key_team = "team";
	inline const static string key_spawnpoint = "spawnpoint";
	inline const static string key_isadmin = "isadmin";
	inline const static string key_isspectator = "isspectator";
	inline const static string key_isbot = "isbot";
};

class OpenRABiz {
public:
	OpenRABiz() = default;
	OpenRABiz(decltype(nlohmann::json::parse("placeholder").items().begin().value()) &v);

	// getter...
	inline uint32_t GetId() const { return id; }
	inline string GetName() const { return name; }
	inline string GetAddress() const { return address; }
	inline string GetMod() const { return mod; }
	inline string GetVersion() const { return version; }
	inline uint16_t PlayersCount() const { return players; }
	inline uint16_t MaxPlayersCount() const { return maxplayers; }
	inline uint16_t BotsCount() const { return bots; }
	inline uint16_t SpectatorsCount() const { return spectators; }
	inline bool Protected() const { return protectedx; }
	inline string GetLocation() const { return location; }
	inline string GetStarted() const { return started; }
	inline uint32_t GetPlaytime() const { return playtime; }

	vector<const ClientInfo*> GetHumans() const;

	// setter...
	inline void SetId(uint32_t t) { id = t; }
	inline void SetName(string_view t) { name = t; }
	inline void SetAddress(string_view t) { address = t; }
	inline void SetState(uint16_t t) { state = t; }
	inline void SetTTL(uint16_t t) { ttl = t; }
	inline void SetMod(string_view t) { mod = t; }
	inline void SetVersion(string_view t) { version = t; }
	inline void SetModtitle(string_view t) { modtitle = t; }
	inline void SetModwebsite(string_view t) { modwebsite = t; }
	inline void SetModicon32(string_view t) { modicon32 = t; }
	inline void SetMap(string_view t) { map = t; }
	inline void SetPlayers(uint16_t t) { players = t; }
	inline void SetMaxplayers(uint16_t t) { maxplayers = t; }
	inline void SetBots(uint16_t t) { bots = t; }
	inline void SetSpectators(uint16_t t) { spectators = t; }
	inline void SetProtected(bool t) { protectedx = t; }
	inline void SetAuthentication(bool t) { authentication = t; }

	template<class _Valty>
	void SetLocation(const _Valty& _Val);

	inline void SetStarted(string_view t) { started = t; }
	inline void SetPlaytime(uint32_t t) { playtime = t; }

	template<class... _Valty>
	vector<ClientInfo>& SetClients(_Valty&&... _Val);
	//vector<ClientInfo>& SetClients(ClientInfo& t);

	inline const static string key_id = "id";
	inline const static string key_name = "name";
	inline const static string key_address = "address";
	inline const static string key_state = "state";
	inline const static string key_ttl = "ttl";
	inline const static string key_mod = "mod";
	inline const static string key_version = "version";
	inline const static string key_modtitle = "modtitle";
	inline const static string key_modwebsite = "modwebsite";
	inline const static string key_modicon32 = "modicon32";
	inline const static string key_map = "map";
	inline const static string key_players = "players";
	inline const static string key_maxplayers = "maxplayers";
	inline const static string key_bots = "bots";
	inline const static string key_spectators = "spectators";
	inline const static string key_protected = "protected";
	inline const static string key_authentication = "authentication";
	inline const static string key_location = "location";
	inline const static string key_started = "started";
	inline const static string key_playtime = "playtime";
	inline const static string key_clients = "clients";

protected:
	uint32_t id;
	string   name;
	string   address;
	uint16_t state;
	uint16_t ttl;
	string   mod;
	string   version;
	string   modtitle;
	string   modwebsite;
	string   modicon32;
	string   map;
	uint16_t players;
	uint16_t maxplayers;
	uint16_t bots;
	uint16_t spectators;
	bool     protectedx;
	bool     authentication;
	string   location;
	string   started;
	uint32_t playtime;

	vector<ClientInfo> clients;
};

class D2KBiz : public OpenRABiz {
public:
	using OpenRABiz::OpenRABiz;

	inline bool Is_20210321_d2k_version() const {
		return d2k_mod == mod && d2k_version == version;
	};

	inline bool Is_waiting() const {
		return 2 != state && players >= 1;
	}

	inline bool Is_playing() const {
		return 2 == state;
	}

	inline bool Is_empty() const {
		return 2 != state && 0 == players;
	}

	inline bool Is_locked() const {
		return 1 == protectedx;
	}

	void print(uint16_t index) const;

private:
	const string d2k_version = "release-20210321";
	const string d2k_mod = "d2k";
};

