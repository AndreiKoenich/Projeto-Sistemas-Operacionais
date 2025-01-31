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

void* monitorClientDirectory (string username) {

    char clientDirectory[FULL_DIRECTORY_NAME_SIZE];
    getClientDirectoryPath(username, clientDirectory);

    int inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        cerr << "Erro ao inicializar inotify." << endl;
        return;
    }

    int watch_descriptor = inotify_add_watch(inotify_fd, clientDirectory, IN_MODIFY | IN_CREATE | IN_DELETE);
    if (watch_descriptor < 0) {
        cerr << "Erro ao adicionar watch ao diretorio." << endl;
        close(inotify_fd);
        return;
    }

    char buffer[BUF_LEN];
    cout << "Monitorando o diretorio: " << clientDirectory << endl;

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
                if (event->mask & IN_CREATE) {
                    cout << "Arquivo " << filename << " foi criado." << endl;
                } else if (event->mask & IN_MODIFY) {
                    cout << "Arquivo " << filename << " foi modificado." << endl;
                } else if (event->mask & IN_DELETE) {
                    cout << "Arquivo " << filename << " foi deletado." << endl;
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(inotify_fd, watch_descriptor);
    close(inotify_fd);
}