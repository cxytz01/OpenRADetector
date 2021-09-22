#include "detector_net.h"

const httplib::Headers generateOpenRAHeader() {
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

std::optional<std::string> httprequest(std::string_view host, std::string_view path, const httplib::Headers headers) {
	if (auto res = httplib::Client(host.data()).Get(path.data(), headers); res == nullptr) return std::nullopt;
	else if (res->status != 200) return std::nullopt;
	else return res->body;
}
