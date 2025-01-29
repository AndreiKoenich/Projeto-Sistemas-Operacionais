#include <cstdint>

const std::uint16_t MAX_SERVER_QUEUE_SIZE = 10;
const std::uint16_t SERVER_PORT_NUMBER = 8080;
const std::uint16_t FULL_DIRECTORY_NAME_SIZE = 1024;
const char SERVER_DIRECTORY_NAME[] = "sync_dir_SERVER";
const char CLIENT_DIRECTORY_NAME[] = "//sync_dir_SERVER//";

const uint16_t NUMBER_OF_DOWNLOAD_PACKET_FIELDS = 3;
const uint16_t NUMBER_OF_DOWNLOAD_ERROR_PACKET_FIELDS = 1;
const uint16_t NUMBER_OF_LIST_SERVER_PACKET_FIELDS = 2;
