#pragma once


void place_argument(const char* option, const char*** destination);

void place_argument_with_value(const char* option_name, const char* option_value, const char*** destination);

void place_optional_argument_with_value(const char* option_name, const char* option_value, const char*** destination);

void terminate_arguments(const char*** destination);