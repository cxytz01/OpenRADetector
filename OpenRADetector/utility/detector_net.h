#pragma once

#include <optional>
#include <string>
#include <string_view>

#define CPPHTTPLIB_OPENSSL_SUPPORT // Install openssl 1.1.1, and configure openssl with vc++ https://stackoverflow.com/questions/31646663/unresolved-external-symbol-openssl-libraries-c
#include "httplib.h" // https://github.com/yhirose/cpp-httplib

const httplib::Headers generateOpenRAHeader();
std::optional<std::string> httprequest(std::string_view host, std::string_view path, const httplib::Headers headers);