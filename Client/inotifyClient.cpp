#include <iostream>
#include <cstring>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>

#include "clientConstants.hpp"
#include "clientUtils.hpp"

using namespace std;

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + NAME_MAX + 1))

bool isTemporaryFile(const string filename) {

    //cout << "Nome do arquivo sendo monitorado: " << filename << endl;
    if (filename.front() == '~' || filename.front() == '.')
        return true;
    else
        return false;
}

void* monitorClientDirectory (void* usernamePtr) {

    string username((char*)usernamePtr);

    char clientDirectory[FULL_DIRECTORY_NAME_SIZE];
    memset(clientDirectory,0,sizeof(clientDirectory));
    getcwd(clientDirectory,FULL_DIRECTORY_NAME_SIZE);
    strcat(clientDirectory,"/");
    strcat(clientDirectory,CLIENT_DIRECTORY_PREFIX);

    char* usernameStr = (char*)calloc(username.length()+1,sizeof(char));
    username.copy(usernameStr,username.length());
    usernameStr[username.length()] = '\0';
    strcat(clientDirectory,usernameStr);
    strcat(clientDirectory,"/");
    free(usernameStr);

    int inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        cerr << "Erro ao inicializar inotify." << endl;
        return NULL;
    }

    int watch_descriptor = inotify_add_watch(inotify_fd, clientDirectory, IN_MOVED_FROM | IN_MOVED_TO | IN_CLOSE_WRITE);
    
    if (watch_descriptor < 0) {
        cerr << "Erro ao adicionar watch ao diretório." << endl;
        close(inotify_fd);
        return NULL;
    }

    char buffer[BUF_LEN];
    cout << "Monitorando o diretório: " << clientDirectory << endl;

    while (true) {
        int length = read(inotify_fd, buffer, BUF_LEN);
        if (length < 0) {
            cerr << "Erro ao ler eventos do inotify." << endl;
            break;
        }

        for (int i = 0; i < length;) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len) {
                string filename = event->name;
                if (!isTemporaryFile(filename))
                {
                    if (event->mask & IN_CLOSE_WRITE)
                        cout << "Arquivo " << filename << " foi modificado." << endl;
                    else if (event->mask & IN_MOVED_FROM)
                        cout << "Arquivo " << filename << " foi movido para fora do diretorio." << endl;
                    else if (event->mask & IN_MOVED_TO)
                        cout << "Arquivo " << filename << " foi movido para dentro do diretorio." << endl;
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(inotify_fd, watch_descriptor);
    close(inotify_fd);

    return NULL;
}