#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include "mainwindow.h"
#include <QSharedMemory>
#include <QLocalSocket>
#include <QLocalServer>
#include <QCommandLineParser>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

const QString SOCKET_NAME = "cihuishu_socket";

MainWindow* globalWindow = nullptr;

#ifdef Q_OS_WIN
// Windows hotkey handling
const int HOTKEY_ID = 1;
bool registerGlobalHotkey() {
    // Win + Shift + S (example: MOD_WIN | MOD_SHIFT, 'S')
    return RegisterHotKey(nullptr, HOTKEY_ID, MOD_WIN | MOD_SHIFT, 0x53);
}
#endif

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("Chinese helper background app");

    QCommandLineParser parser;
    parser.setApplicationDescription("Chinese helper background app");
    parser.addHelpOption();
    parser.addOption({{"t", "toggle"}, "Toggle window visibility"});
    parser.process(app);

    QLocalSocket socket;
    socket.connectToServer(SOCKET_NAME);
    if (socket.waitForConnected(100)) {
        // Another instance is running
        if (parser.isSet("toggle")) {
            socket.write("toggle");
            socket.flush();
            socket.waitForBytesWritten();
        }
        return 0; // Don't start a new instance
    }

    QLocalServer server;
    server.removeServer(SOCKET_NAME); // clean leftover
    server.listen(SOCKET_NAME);
    QObject::connect(&server, &QLocalServer::newConnection, [&]() {
        QLocalSocket *client = server.nextPendingConnection();
        if (client && client->waitForReadyRead(1000)) {
            QByteArray data = client->readAll();
            if (data == "toggle" && globalWindow) {
                globalWindow->toggleVisibility();
            }
        }
        client->disconnectFromServer();
    });

    MainWindow window;
    globalWindow = &window;

    QIcon icon = QIcon::fromTheme("system-run");
    if (icon.isNull()) {
        icon = QIcon(":/images/icon.png");
    }
    QSystemTrayIcon tray(icon);
    tray.setIcon(icon);
    QMenu *menu = new QMenu;
    menu->addAction("Toggle Window", [&]() { window.toggleVisibility(); });
    menu->addAction("Quit", &app, &QApplication::quit);
    tray.setContextMenu(menu);
    tray.show();

#ifdef Q_OS_WIN
    // Register hotkey
    if (!registerGlobalHotkey()) {
        qWarning() << "Failed to register global hotkey";
    }

    // Listen to hotkey messages
    app.installNativeEventFilter(new class HotkeyFilter);
#endif

    return app.exec();
}

#ifdef Q_OS_WIN
// Windows-specific: native event filter for hotkey
#include <QAbstractNativeEventFilter>

class HotkeyFilter : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY && msg->wParam == HOTKEY_ID && globalWindow) {
            globalWindow->toggleVisibility();
            *result = 0;
            return true;
        }
        return false;
    }
};
#endif
