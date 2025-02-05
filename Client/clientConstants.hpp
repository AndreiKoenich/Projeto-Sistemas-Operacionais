#include <cstdint>
#include <cstring>

using namespace std;

const uint16_t CLIENT_PORT_NUMBER = 8080;
const uint16_t FULL_DIRECTORY_NAME_SIZE = 1024;
const uint16_t NUMBER_OF_PARAMETERS = 3;
const uint16_t NUMBER_OF_UPLOAD_PACKET_FIELDS = 3;
const uint16_t NUMBER_OF_HELLO_PACKET_FIELDS = 2;
const uint16_t NUMBER_OF_BYE_PACKET_FIELDS = 1;
const uint16_t NUMBER_OF_REQUEST_DOWNLOAD_PACKET_FIELDS = 2;
const uint16_t NUMBER_OF_REQUEST_LIST_SERVER_PACKET_FIELDS = 1;
const uint16_t NUMBER_OF_DELETE_INOTIFY_PACKET_FIELDS = 2;
const char CLIENT_DIRECTORY_PREFIX[] = "sync_dir_";

const string UPLOAD_COMMAND = "upload ";
const string DOWNLOAD_COMMAND = "download ";
const string DELETE_COMMAND = "delete ";
const string LIST_SERVER_COMMAND = "list_server";
const string LIST_CLIENT_COMMAND = "list_client";
const string HELP_COMMAND = "help";
const string EXIT_COMMAND = "exit";